/*******************************************************************************
GPU OPTIMIZED MONTE CARLO (GOMC) 2.20
Copyright (C) 2018  GOMC Group
A copy of the GNU General Public License can be found in the COPYRIGHT.txt
along with this program, also can be found at <http://www.gnu.org/licenses/>.
********************************************************************************/
#ifndef EWALD_H
#define EWALD_H

#include "BasicTypes.h"
#include "EnergyTypes.h"
#include "Molecules.h"
#include "Forcefield.h"
#include "TrialMol.h"
#include <vector>
#include <stdio.h>
#include <cstring>
#include <cassert>
#ifdef _OPENMP
#include <omp.h>
#endif
//
//    Calculating Electrostatic calculation without caching Fourier terms.
//    Energy Calculation functions for Ewald summation method
//    Calculating self, correction and reciprocate part of ewald
//
//    Developed by Y. Li and Mohammad S. Barhaghi
//
//

class StaticVals;
class System;
class Forcefield;
class Molecules;
class MoleculeLookup;
class MoleculeKind;
class Coordinates;
class COM;
class XYZArray;
class BoxDimensions;
class CalculateEnergy;


class Ewald
{
  //friend class CalculateEnergy;
public:

  Ewald(StaticVals & stat, System & sys);
  ~Ewald();

  virtual void Init();

  virtual void AllocMem();

  //setup reciprocate term for a box
  virtual void BoxReciprocalSetup(uint box, XYZArray const& molCoords);

  //calculate reciprocate energy term for a box
  virtual double BoxReciprocal(uint box) const;

  //calculate reciprocate term for displacement and rotation move
  virtual double MolReciprocal(XYZArray const& molCoords, const uint molIndex,
                               const uint box, XYZ const*const newCOM = NULL);

  //calculate reciprocate term in destination box for swap move
  virtual double SwapDestRecip(const cbmc::TrialMol &newMol, const uint box,
                               const int sourceBox, const int molIndex);

  //calculate reciprocate term in source box for swap move
  virtual double SwapSourceRecip(const cbmc::TrialMol &oldMol,
                                 const uint box, const int molIndex);

  //restore cosMol and sinMol
  virtual void RestoreMol(int molIndex);

  uint findLargeImage();

  //update sinMol and cosMol
  virtual void exgMolCache();

private:
  double currentEnergyRecip[BOXES_WITH_U_NB];

protected:
  const Forcefield& forcefield;
  const Molecules& mols;
  const Coordinates& currentCoords;
  const MoleculeLookup& molLookup;
  const BoxDimensions& currentAxes;
  const COM& currentCOM;
  const SystemPotential &sysPotRef;

  bool electrostatic, ewald;
  double alpha;
  double recip_rcut, recip_rcut_Sq;
  uint *imageSize;
  uint *imageSizeRef;
  //const uint imageTotal = GetImageSize();
  uint imageTotal;
  uint imageLarge;
  uint *kmax;
  double **sumRnew; //cosine serries
  double **sumInew; //sine serries
  double **sumRref;
  double **sumIref;

  double **kx, **kxRef;
  double **ky, **kyRef;
  double **kz, **kzRef;
  double **hsqr, **hsqrRef;
  double **prefact, **prefactRef;

  std::vector<int> particleKind;
  std::vector<int> particleMol;
  std::vector<double> particleCharge;
};



#endif /*EWALD_H*/
