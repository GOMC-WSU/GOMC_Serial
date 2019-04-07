/*******************************************************************************
GPU OPTIMIZED MONTE CARLO (GOMC) 2.31
Copyright (C) 2018  GOMC Group
A copy of the GNU General Public License can be found in the COPYRIGHT.txt
along with this program, also can be found at <http://www.gnu.org/licenses/>.
********************************************************************************/
#include "ReplicaExchangeController.h"
#include <time.h>

using namespace std; 

#define PROBABILITYCUTOFF 100
//NOTE: requires C99 support...
  typedef uint32_t uint32;  // unsigned integer type, at least 32 bits
  typedef int32_t int32;

/* we don't bother evaluating if events are more rare than exp(-100) = 3.7x10^-44 */

ReplicaExchangeController::ReplicaExchangeController(vector<Simulation*>* sims){

    simsRef = sims;
    //  For now, exchangeRate == the arg for RunNSteps; which removes the necessity to check if this step is an exchange step
    //  We simply exchange after each burst
    exchangeRate= (*simsRef)[0]->getExchangeInterval();
    totalSteps = (*simsRef)[0]->getTotalSteps();
    if ((*simsRef)[0]->getReplExSeed() != -1){
      const uint32 oneSeed = (uint32)(*simsRef)[0]->getReplExSeed();
      rand = MTRand( oneSeed );
    } else {
      rand = MTRand();
    }

    checkerForIncreasingMontonicityOfTemp = 0;
    std::string multiSimTitle = (*simsRef)[0]->getMultiSimTitle();

    for ( int i = 0; i < (*simsRef).size(); i++){
        if (exchangeRate != (*simsRef)[i]->getExchangeInterval()){
          std::cout << "Error: Each replica must have equal exchange rate. " << (*simsRef)[i]->getConfigFileName() <<
          " differs from the others!\n";
          exit(EXIT_FAILURE);
        }
        if (exchangeRate != (*simsRef)[i]->getExchangeInterval()){
          std::cout << "Error: Each replica must have equal exchange rate. " << (*simsRef)[i]->getConfigFileName() <<
          " differs from the others!\n";
          exit(EXIT_FAILURE);
        }
        if (multiSimTitle.compare((*simsRef)[i]->getMultiSimTitle())){
          std::cout << "Error: Each replica must have the same multiSimTitle. " << (*simsRef)[i]->getConfigFileName() <<
          " differs from the others!\n";
          exit(EXIT_FAILURE);
        }
        if (totalSteps != (*simsRef)[i]->getTotalSteps()){
          std::cout << "Error: Each replica must have number of total steps. " << (*simsRef)[i]->getConfigFileName() <<
          " differs from the others!\n";
          exit(EXIT_FAILURE);
        }
        if ( (*simsRef)[i]->getT_in_K() > checkerForIncreasingMontonicityOfTemp ){
          checkerForIncreasingMontonicityOfTemp = (*simsRef)[i]->getT_in_K();
        } else {
            std::cout << "Error: List the conf files in increasing temperature order. " << (*simsRef)[i]->getConfigFileName() <<
            " is not in order of least to greatest for temperature!\n";
            exit(EXIT_FAILURE);
        }
    }
    if (exchangeRate > 0) {
      roundedUpDivison = ((*simsRef)[0]->getTotalSteps() + exchangeRate - 1) / exchangeRate;
    } else {
      exchangeRate = totalSteps;
      int j;
      #pragma omp parallel for private(j)
      for (j = 0; j < (*simsRef).size(); j++){
        // Note that RunNSteps overwrites startStep before returning to the step it left off on
        (*simsRef)[j]->RunNSteps(exchangeRate);
      }
      return;
    }

    InitRecordKeeper();

    std::stringstream replica_log_stream;
    replica_log_stream << (*simsRef)[0]->getMultiSimTitle() <<
    "/replica_log.txt";
    std::string const fileName = replica_log_stream.str();
    std::string const alias = "Replica Log File";
    fplog = fopen(fileName.c_str(), "w");

}
ReplicaExchangeController::~ReplicaExchangeController(){
  DestroyRecordKeeper();
  fclose(fplog);
  int i = (*simsRef).size();
  for (int j = 0; j < i; j++){
    delete (*simsRef)[j];
  }
}

void ReplicaExchangeController::runMultiSim(){

    double delta;
    double ediff;
    double probability;
    int     a;
    int     b;
    int     ap;
    int     bp;
    timer = (*simsRef)[0]->getClock();

    fprintf(fplog, "\nInitializing Replica Exchange\n");
    fprintf(fplog, "Repl  There are %lu replicas\n", (*simsRef).size());
    fprintf(fplog, "\nReplica exchange in temperature\n");
            for (int i = 0; i < (*simsRef).size(); i++)
            {
                fprintf(fplog, " %5.1f", (*simsRef)[i]->getT_in_K());
            }
            fprintf(fplog, "\n");
    fprintf(fplog, "\nReplica exchange interval: %lu\n", (*simsRef)[0]->getExchangeInterval());
    fprintf(fplog, "\nReplica random seed: %d\n", (*simsRef)[0]->getReplExSeed());
    fprintf(fplog, "\nReplica exchange information below: ex and x = exchange, pr = probability\n\n");

    ulong step = 0;
    int j;
    bool swapStates = true;
    swapStates = (*simsRef)[0]->getReplExParams()->exchangeStates;
    for (ulong i = 0; i < roundedUpDivison; i++){

        #pragma omp parallel for private(j)
        for (j = 0; j < (*simsRef).size(); j++){
          // Note that RunNSteps overwrites startStep before returning to the step it left off on
          (*simsRef)[j]->RunNSteps(exchangeRate);
        }
        
        for (int i = 0; i < (*simsRef).size(); i++)
          re.pind[i] = re.ind[i];


        step += exchangeRate;        
        if (exchangeRate!=totalSteps  && (*simsRef)[0]->getEquilSteps() <= step){
         // replicaLog.file << "Replica exchange at step " << step << std::endl;
          timer->SetStop();
          fprintf(fplog, "\nReplica exchange at step %lu time %.5f\n", step, timer->GetTimDiff());
          parityOfSwaps = ((*simsRef)[0]->getStartStep() / exchangeRate) % 2;

          for (int i = 1; i < (*simsRef).size(); i++){
            if ((*simsRef)[i]->getEquilSteps() < ((*simsRef)[i]->getStartStep() + exchangeRate)) {

              a = re.pind[i-1];
              b = re.pind[i];

              //  To alternate between swapping even replicas and repl_id+1 {0,1} {2,3} ... on even parity and 
              //  odd replicas and repl_id+1 {1,2} ... on odd parity
              if (i % 2 == parityOfSwaps){
                  delta = calc_delta(fplog, a, b, a, b);
                  if (delta <= 0) {
                    if(swapStates){
                      exchangeStates(a, b);
                    } else {
                      exchangeConfigurations(a, b);
                    }
                    re.prob[i] = 1;
                    re.bEx[i] = true;
                  } else {
                     if (delta > PROBABILITYCUTOFF){
                      re.prob[i] = 0;
                      re.bEx[i] = false;
                    }
                    else {
                      re.prob[i] = exp(-delta);
                    }
                    if (rand.rand() < re.prob[i]){
                    if(swapStates){
                      exchangeStates(a, b);
                    } else {
                      exchangeConfigurations(a, b);
                    }
                      re.bEx[i] = true;
                    } else {
                      re.bEx[i] = false;
                    }
                  }
                  re.prob_sum[i] += re.prob[i];
                  if (re.bEx[i]) {
                    /* swap these two */
                    int tmp       = re.pind[i-1];
                    re.pind[i-1] = re.pind[i];
                    re.pind[i]   = tmp;
                    re.nexchange[i]++;  /* statistics for back compatibility */
                  }
                } else {
                  re.prob[i] = -1;
                  re.bEx[i]  = false;
                }
              }
            }
            print_ind(fplog, "ex", (*simsRef).size(), re.ind, re.bEx);
            print_prob(fplog, "pr", (*simsRef).size(), re.prob);
            //replicaLog.file << "\n" << std::endl;
            re.nattempt[parityOfSwaps]++;
             for (int i = 0; i < re.nrepl; i++) {
                re.nmoves[re.ind[i]][re.pind[i]] += 1;
                re.nmoves[re.pind[i]][re.ind[i]] += 1;
            }
        }
      }
      print_replica_exchange_statistics(fplog, &re);  
}

void ReplicaExchangeController::exchangeStates(int a, int b){
  double swapperForT_in_K = (*simsRef)[a]->getT_in_K(); 
  double swapperForBeta = (*simsRef)[a]->getBeta();
  CPUSide * swapperForCPUSide = (*simsRef)[a]->getCPUSide();
  (*simsRef)[a]->setT_in_K((*simsRef)[b]->getT_in_K());
  (*simsRef)[a]->setBeta((*simsRef)[b]->getBeta());
  (*simsRef)[a]->setCPUSide((*simsRef)[b]->getCPUSide());
  (*simsRef)[b]->setT_in_K(swapperForT_in_K);
  (*simsRef)[b]->setBeta(swapperForBeta);
  (*simsRef)[b]->setCPUSide(swapperForCPUSide);
  Simulation * swapperForReplica = (*simsRef)[a];
  (*simsRef)[a] = (*simsRef)[b];
  (*simsRef)[b] = swapperForReplica;
}

void ReplicaExchangeController::exchangeConfigurations(int a, int b){

  Coordinates * swapperForCoordinates = new Coordinates(  (*simsRef)[a]->getSystem()->boxDimRef, 
                                      (*simsRef)[a]->getSystem()->com,
                                      (*simsRef)[a]->getSystem()->molLookupRef,
                                      (*simsRef)[a]->getSystem()->prng,
                                      (*simsRef)[a]->getStaticValues()->mol);

  swapperForCoordinates->operator=((*simsRef)[a]->getSystem()->coordinates);
  (*simsRef)[a]->getSystem()->coordinates.operator=((*simsRef)[b]->getSystem()->coordinates);
  (*simsRef)[b]->getSystem()->coordinates.operator=(*swapperForCoordinates);
  delete swapperForCoordinates;

  COM * swapperForCOM = new COM(  (*simsRef)[a]->getSystem()->boxDimRef,
                                  (*simsRef)[a]->getSystem()->coordinates,
                                  (*simsRef)[a]->getSystem()->molLookupRef,
                                  (*simsRef)[a]->getStaticValues()->mol);
  swapperForCOM->operator=((*simsRef)[a]->getSystem()->com);                     
  (*simsRef)[a]->getSystem()->com.operator=((*simsRef)[b]->getSystem()->com);
  (*simsRef)[b]->getSystem()->com.operator=(*swapperForCOM);
  delete swapperForCOM;

  SystemPotential * swapperForSystemPotential = new SystemPotential();
  swapperForSystemPotential->operator=((*simsRef)[a]->getSystem()->potential); 
  (*simsRef)[a]->getSystem()->potential.operator=((*simsRef)[b]->getSystem()->potential);
  (*simsRef)[b]->getSystem()->potential=(*swapperForSystemPotential);
  delete swapperForSystemPotential;

  (*simsRef)[a]->getSystem()->cellList.RebuildNeighbors(0);
  (*simsRef)[b]->getSystem()->cellList.RebuildNeighbors(0);
}

double ReplicaExchangeController::calc_delta(FILE * fplog, int a, int b, int ap, int bp){
  
  double delta;

  #if ENSEMBLE == NPT || ENSEMBLE == NVT || ENSEMBLE == GCMC

  double epot_a = (*simsRef)[a]->getEpot();
  double epot_b = (*simsRef)[b]->getEpot();
  double beta_a = (*simsRef)[a]->getBeta();
  double beta_b = (*simsRef)[b]->getBeta(); 

  double ediff = epot_b - epot_a;
  delta = -(beta_b - beta_a)*ediff;

  #endif



  /* GROMACS Abraham, et al. (2015) SoftwareX 1-2 19-25 */
  #if ENSEMBLE == NPT || ENSEMBLE == NVT
    fprintf(fplog, "Repl %d <-> %d  dE_term = %10.3e (units?)\n", a, b, delta);
  #endif

  /*  GROMACS Abraham, et al. (2015) SoftwareX 1-2 19-25 */
  #if ENSEMBLE == NPT
    double pres_a = (*simsRef)[a]->getPressure();
    double vol_a = (*simsRef)[a]->getVolume();
    double pres_b = (*simsRef)[b]->getPressure();
    double vol_b = (*simsRef)[b]->getVolume();
    double dpV = (beta_a * pres_a - beta_b * pres_b) * (vol_b - vol_a);
    fprintf(fplog, "  dpV = %10.3e  d = %10.3e\n", dpV, delta + dpV);
    delta += dpV;
  #endif

  /*  Faller, Roland, Qiliang Yan, and Juan J. de Pablo. "Multicanonical parallel tempering." The Journal of chemical physics 116.13 (2002): 5419-5423.
      Eq (3)*/
  #if ENSEMBLE == GCMC

    double deltaBetaMuN   = 0;

    for (uint i = 0; i < ((*simsRef)[a]->getSystem())->molLookup.GetNumKind(); i++){
      deltaBetaMuN += ((beta_a * (*simsRef)[a]->getChemicalPotential(i) -
        beta_b * (*simsRef)[b]->getChemicalPotential(i) ) * (
         (*simsRef)[b]->getNumOfParticles(i) - (*simsRef)[a]->getNumOfParticles(i)));
    }
    fprintf(fplog, "  dMuN = %10.3e  d = %10.3e\n", deltaBetaMuN, delta - deltaBetaMuN);
    delta -= deltaBetaMuN;
  #endif

  /*  Ortiz et al Chemical physics letters 368.3-4 (2003): 452-457.
      Eq (8) */
  #if ENSEMBLE == GEMC

  delta = 0;
  double dpV = 0;
  for (uint i = 0; i < BOX_TOTAL; ++i) { 
      delta += -(((*simsRef)[b]->getBeta() - (*simsRef)[a]->getBeta())*
        ((*simsRef)[b]->getEpotBox(i) - (*simsRef)[a]->getEpotBox(i)));

      if ((*simsRef)[b]->getKindOfGEMC()){
        dpV += ((*simsRef)[a]->getBeta() * (*simsRef)[a]->getPressure() - 
          (*simsRef)[b]->getBeta() * (*simsRef)[b]->getPressure()) * 
          ((*simsRef)[b]->getVolume(i) - (*simsRef)[a]->getVolume(i));
      }
  }
  fprintf(fplog, "Repl %d <-> %d  dE_term = %10.3e (kT)\n", a, b, delta);
  if ((*simsRef)[b]->getKindOfGEMC())
        fprintf(fplog, "  dpV = %10.3e  d = %10.3e\n", dpV, delta + dpV);

  delta += dpV; 

  #endif

  return delta;
}

void ReplicaExchangeController::InitRecordKeeper(){
  re.nrepl = (*simsRef).size();
  re.nattempt[0] = 0;
  re.nattempt[1] = 0;
  re.prob = new double[re.nrepl];
  re.bEx =  new bool[re.nrepl];
  re.prob_sum = new double[re.nrepl];
  re.nexchange = new int[re.nrepl];
  re.nmoves = new int*[re.nrepl];
  for (int i = 0; i < (*simsRef).size(); i++) {
    re.nmoves[i] = new int[re.nrepl];
    for (int j = 0; j < (*simsRef).size(); j++){
      re.nmoves[i][j] = 0;
    }
  }
  re.ind = new int[re.nrepl];
  re.pind = new int[re.nrepl];

  for (int i = 0; i < (*simsRef).size(); i++){
    re.ind[i] = i;
    re.pind[i] = i;
    re.bEx[i] = 0;
    re.prob[i] = 0;
    re.prob_sum[i] = 0;
    re.nexchange[i] = 0;
  }

  #if ENSEMBLE == NPT
    re.bNPT = true;
  #else
    re.bNPT = false;
  #endif
}

void ReplicaExchangeController::DestroyRecordKeeper(){
  for (int i = 0; i < re.nrepl; i++) {
    delete[] re.nmoves[i];
  }
  delete[] re.nmoves;
  delete[] re.prob;
  delete[] re.bEx;
  delete[] re.prob_sum;
  delete[] re.nexchange;
  delete[] re.ind;
  delete[] re.pind;  
}

void ReplicaExchangeController::print_ind(FILE * fplog, const char *leg, int n, int *ind, bool *bEx){
  int i;

    fprintf(fplog, "Repl %2s %2d", leg, ind[0]);
    for (i = 1; i < n; i++)
    {
        fprintf(fplog, " %c %2d", (bEx != NULL && bEx[i]) ? 'x' : ' ', ind[i]);
    }
    fprintf(fplog, "\n");
}

void ReplicaExchangeController::print_prob(FILE * fplog, const char *leg, int n, double *prob){
    int  i;
    char buf[8];

    fprintf(fplog, "Repl %2s ", leg);
    for (i = 1; i < n; i++)
    {
        if (prob[i] >= 0)
        {
            sprintf(buf, "%4.2f", prob[i]);
            fprintf(fplog, "  %3s", buf[0] == '1' ? "1.0" : buf+1);
        }
        else
        {
            fprintf(fplog, "     ");
        }
    }
    fprintf(fplog, "\n");
}

void ReplicaExchangeController::print_transition_matrix(FILE * fplog, int n, int **nmoves, int *nattempt)
{
     int   i, j, ntot;
    float Tprint;

    ntot = nattempt[0] + nattempt[1];
    fprintf(fplog, "\n");
    fprintf(fplog, "Repl");
    for (i = 0; i < n; i++)
    {
        fprintf(fplog, "    ");  /* put the title closer to the center */
    }
    fprintf(fplog, "Empirical Transition Matrix\n");

    fprintf(fplog, "Repl");
    for (i = 0; i < n; i++)
    {
        fprintf(fplog, "%8d", (i+1));
    }
    fprintf(fplog, "\n");

    for (i = 0; i < n; i++)
    {
        fprintf(fplog, "Repl");
        for (j = 0; j < n; j++)
        {
            Tprint = 0.0;
            if (nmoves[i][j] > 0)
            {
                Tprint = nmoves[i][j]/(2.0*ntot);
            }
            fprintf(fplog, "%8.4f", Tprint);
        }
        fprintf(fplog, "%3d\n", i);
    }
}

void ReplicaExchangeController::print_replica_exchange_statistics(FILE * fplog, RecordKeeper * re){
    int  i;

    fprintf(fplog, "\nReplica exchange statistics\n");

   // if (re->nex == 0)
   // {
        fprintf(fplog, "Repl  %d attempts, %d odd, %d even\n",
                re->nattempt[0]+re->nattempt[1], re->nattempt[1], re->nattempt[0]);

        fprintf(fplog, "Repl  average probabilities:\n");
        for (i = 1; i < re->nrepl; i++)
        {
            if (re->nattempt[i%2] == 0)
            {
                re->prob[i] = 0;
            }
            else
            {
                re->prob[i] =  re->prob_sum[i]/re->nattempt[i%2];
            }
        }
        print_ind(fplog, "", re->nrepl, re->ind, NULL);
        print_prob(fplog, "", re->nrepl, re->prob);

        fprintf(fplog, "Repl  number of exchanges:\n");
        print_ind(fplog, "", re->nrepl, re->ind, NULL);
        print_count(fplog, "", re->nrepl, re->nexchange);

        fprintf(fplog, "Repl  average number of exchanges:\n");
        for (i = 1; i < re->nrepl; i++)
        {
            if (re->nattempt[i%2] == 0)
            {
                re->prob[i] = 0;
            }
            else
            {
                re->prob[i] =  ((double)re->nexchange[i])/re->nattempt[i%2];
            }
        }
        print_ind(fplog, "", re->nrepl, re->ind, NULL);
        print_prob(fplog, "", re->nrepl, re->prob);

        fprintf(fplog, "\n");
    //}
    /* print the transition matrix */
    print_transition_matrix(fplog, re->nrepl, re->nmoves, re->nattempt);
}

void ReplicaExchangeController::print_count(FILE *fplog, const char *leg, int n, int *count){
    int i;

    fprintf(fplog, "Repl %2s ", leg);
    for (i = 1; i < n; i++)
    {
        fprintf(fplog, " %4d", count[i]);
    }
    fprintf(fplog, "\n");
}




