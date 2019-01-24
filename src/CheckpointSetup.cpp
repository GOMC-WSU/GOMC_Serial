/*******************************************************************************
GPU OPTIMIZED MONTE CARLO (GOMC) 2.31
Copyright (C) 2018  GOMC Group
A copy of the GNU General Public License can be found in the COPYRIGHT.txt
along with this program, also can be found at <http://www.gnu.org/licenses/>.
********************************************************************************/

#include <stdint.h>
#include "CheckpointSetup.h"
#include "MoleculeLookup.h"
#include "System.h"

namespace
{
  union dbl_input_union {
    char bin_value[8];
    double dbl_value;
  };

  union uint32_input_union {
    char bin_value[8];
    uint32_t uint_value;
  };
}

CheckpointSetup::CheckpointSetup(System & sys, StaticVals const& statV) :
  moveSetRef(sys.moveSettings), molLookupRef(sys.molLookupRef),
  boxDimRef(sys.boxDimRef),  molRef(statV.mol), prngRef(sys.prng),
  coordCurrRef(sys.coordinates), filename("checkpoint.dat")
{
}

void CheckpointSetup::ReadAll()
{
  openInputFile();
  readStepNumber();
  readBoxDimensionsData();
  readRandomNumbers();
  readCoordinates();
  readMoleculeLookupData();
  readMoveSettingsData();
  closeInputFile();
}

void CheckpointSetup::readStepNumber()
{
  stepNumber = readUintIn8Chars();
}

void CheckpointSetup::readBoxDimensionsData()
{
  // read the number of boxes
  totalBoxes = readUintIn8Chars();
  axis.resize(totalBoxes);
  cosAngle.resize(totalBoxes);

  for(int b=0; b<totalBoxes; b++) {
    axis[b].resize(3);
    cosAngle[b].resize(3);
    axis[b][0] = readDoubleIn8Chars();
    axis[b][1] = readDoubleIn8Chars();
    axis[b][2] = readDoubleIn8Chars();
    cosAngle[b][0] = readDoubleIn8Chars();
    cosAngle[b][1] = readDoubleIn8Chars();
    cosAngle[b][2] = readDoubleIn8Chars();
  }
}

void CheckpointSetup::readRandomNumbers()
{
  // First let's read the state array
  // the length of the array is 624
  const int N = 624;
  if(saveArray)
    delete [] saveArray;
  saveArray = new uint32_t[N];
  for(int i=0; i<N; i++) {
    saveArray[i] = readUintIn8Chars();
  }

  // Read the location of pointer in state
  seedLocation = readUintIn8Chars();

  // Read the "left" value so we can restore
  seedLeft = readUintIn8Chars();

  // let's save seedValue just in case
  // not sure if that is used or not, or how important it is
  seedValue = readUintIn8Chars();
}

void CheckpointSetup::readCoordinates()
{
  // first let's read the count
  coordLength = readUintIn8Chars();

  // now let's read the coordinates one by one
  coords.Init(coordLength);
  for(int i=0; i<coordLength; i++) {
    coords[i].x = readDoubleIn8Chars();
    coords[i].y = readDoubleIn8Chars();
    coords[i].z = readDoubleIn8Chars();
  }
}

void CheckpointSetup::readMoleculeLookupData()
{
  // read the size of molLookup array
  molLookupVec.resize(readUintIn8Chars());
  // read the molLookup array itself
  for(int i=0; i<molLookupVec.size(); i++) {
    molLookupVec[i] = readUintIn8Chars();
  }

  // read the size of boxAndKindStart array
  boxAndKindStartVec.resize(readUintIn8Chars());
  // read the BoxAndKindStart array
  for(int i=0; i<boxAndKindStartVec.size(); i++) {
    boxAndKindStartVec[i] = readUintIn8Chars();
  }

  // read numKinds
  numKinds = readUintIn8Chars();

  //read the size of fixedAtom array
  fixedAtomVec.resize(readUintIn8Chars());
  //read the fixedAtom array itself
  for(int i=0; i<fixedAtomVec.size(); i++) {
    fixedAtomVec[i] = readUintIn8Chars();
  }
}

void CheckpointSetup::readMoveSettingsData()
{
  uint size_x, size_y, size_z;

  // read size of scale
  size_x = readUintIn8Chars();
  size_y = readUintIn8Chars();
  size_z = readUintIn8Chars();

  // read scale array
  scaleVec.resize(size_x);
  for(int i=0; i<size_x; i++) {
    scaleVec[i].resize(size_y);
    for(int j=0; j<size_y; j++) {
      scaleVec[i][j].resize(size_z);
      for(int k=0; k<size_z; k++) {
        scaleVec[i][j][k] = readDoubleIn8Chars();
      }
    }
  }

  // read size of acceptPercent
  size_x = readUintIn8Chars();
  size_y = readUintIn8Chars();
  size_z = readUintIn8Chars();

  // read acceptPercent array
  acceptPercentVec.resize(size_x);
  for(int i=0; i<size_x; i++) {
    acceptPercentVec[i].resize(size_y);
    for(int j=0; j<size_y; j++) {
      acceptPercentVec[i][j].resize(size_z);
      for(int k=0; k<size_z; k++) {
        acceptPercentVec[i][j][k] = readDoubleIn8Chars();
      }
    }
  }

  // read size of accepted
  size_x = readUintIn8Chars();
  size_y = readUintIn8Chars();
  size_z = readUintIn8Chars();

  // read accepted array
  acceptedVec.resize(size_x);
  for(int i=0; i<size_x; i++) {
    acceptedVec[i].resize(size_y);
    for(int j=0; j<size_y; j++) {
      acceptedVec[i][j].resize(size_z);
      for(int k=0; k<size_z; k++) {
        acceptedVec[i][j][k] = readUintIn8Chars();
      }
    }
  }

  // print size of tries
  size_x = readUintIn8Chars();
  size_y = readUintIn8Chars();
  size_z = readUintIn8Chars();

  // print tries array
  triesVec.resize(size_x);
  for(int i=0; i<size_x; i++) {
    triesVec[i].resize(size_y);
    for(int j=0; j<size_y; j++) {
      triesVec[i][j].resize(size_z);
      for(int k=0; k<size_z; k++) {
        triesVec[i][j][k] = readUintIn8Chars();
      }
    }
  }

  // print size of tempAccepted
  size_x = readUintIn8Chars();
  size_y = readUintIn8Chars();
  size_z = readUintIn8Chars();

  // print tempAccepted array
  tempAcceptedVec.resize(size_x);
  for(int i=0; i<size_x; i++) {
    tempAcceptedVec[i].resize(size_y);
    for(int j=0; j<size_y; j++) {
      tempAcceptedVec[i][j].resize(size_z);
      for(int k=0; k<size_z; k++) {
        tempAcceptedVec[i][j][k] = readUintIn8Chars();
      }
    }
  }

  // print size of tempTries
  size_x = readUintIn8Chars();
  size_y = readUintIn8Chars();
  size_z = readUintIn8Chars();

  // print tempTries array
  tempTriesVec.resize(size_x);
  for(int i=0; i<size_x; i++) {
    tempTriesVec[i].resize(size_y);
    for(int j=0; j<size_y; j++) {
      tempTriesVec[i][j].resize(size_z);
      for(int k=0; k<size_z; k++) {
        tempTriesVec[i][j][k] = readUintIn8Chars();
      }
    }
  }
}

void CheckpointSetup::openInputFile()
{
  inputFile = fopen(filename.c_str(), "rb");
  if(inputFile == NULL) {
    fprintf(stderr, "Error opening checkpoint input file %s\n",
            filename.c_str());
    exit(EXIT_FAILURE);
  }
}

void CheckpointSetup::closeInputFile()
{
  if(inputFile)
    fclose(inputFile);
}

double CheckpointSetup::readDoubleIn8Chars()
{
  if(inputFile == NULL) {
    fprintf(stderr, "Error opening checkpoint output file %s\n",
            filename.c_str());
    exit(EXIT_FAILURE);
  }
  dbl_input_union temp;
  fscanf(inputFile, "%c%c%c%c%c%c%c%c",
         &temp.bin_value[0],
         &temp.bin_value[1],
         &temp.bin_value[2],
         &temp.bin_value[3],
         &temp.bin_value[4],
         &temp.bin_value[5],
         &temp.bin_value[6],
         &temp.bin_value[7]);
  return temp.dbl_value;
}

uint32_t CheckpointSetup::readUintIn8Chars()
{
  uint32_t data;
  if(inputFile == NULL) {
    fprintf(stderr, "Error opening checkpoint output file %s\n",
            filename.c_str());
    exit(EXIT_FAILURE);
  }
  uint32_input_union temp;
  fscanf(inputFile, "%c%c%c%c%c%c%c%c",
         &temp.bin_value[0],
         &temp.bin_value[1],
         &temp.bin_value[2],
         &temp.bin_value[3],
         &temp.bin_value[4],
         &temp.bin_value[5],
         &temp.bin_value[6],
         &temp.bin_value[7]);
  return temp.uint_value;
}