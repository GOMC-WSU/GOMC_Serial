set(sources 
   src/BlockOutput.cpp
   src/BoxDimensions.cpp
   src/BoxDimensionsNonOrth.cpp
   src/CBMC.cpp
   src/CellList.cpp
   src/ConfigSetup.cpp
   src/ConsoleOutput.cpp
   src/Coordinates.cpp
   src/CPUSide.cpp
   src/CalculateEnergy.cpp
   src/EnPartCntSampleOutput.cpp
   src/Ewald.cpp
   src/EwaldCached.cpp
   src/FFConst.cpp
   src/FFDihedrals.cpp
   src/FFParticle.cpp
   src/FFSetup.cpp
   src/Forcefield.cpp
   src/Geometry.cpp
   src/HistOutput.cpp
   src/InputFileReader.cpp
   src/Main.cpp
   src/MoleculeKind.cpp
   src/MoleculeLookup.cpp
   src/Molecules.cpp
   src/MolSetup.cpp
   src/MoveSettings.cpp
   src/NoEwald.cpp
   src/OutConst.cpp
   src/OutputVars.cpp
   src/PDBSetup.cpp
   src/PDBOutput.cpp
   src/PRNGSetup.cpp
   src/PSFOutput.cpp
   src/Reader.cpp
   src/Simulation.cpp
   src/StaticVals.cpp
   src/System.cpp
   src/cbmc/DCGraph.cpp
   src/cbmc/DCFreeHedron.cpp
   src/cbmc/DCFreeHedronSeed.cpp
   src/cbmc/DCLinkedHedron.cpp
   src/cbmc/DCHedron.cpp
   src/cbmc/DCLinear.cpp
   src/cbmc/DCOnSphere.cpp
   src/cbmc/DCRotateCOM.cpp
   src/cbmc/DCSingle.cpp
   src/cbmc/TrialMol.cpp)

set(headers
   src/BlockOutput.h
   src/BoxDimensions.h
   src/BoxDimensionsNonOrth.h
   src/CalculateEnergy.h
   src/CBMC.h
   src/CellList.h
   src/Clock.h
   src/COM.h
   src/ConfigSetup.h
   src/ConsoleOutput.h
   src/ConstField.h
   src/Coordinates.h
   src/CoordinateSetup.h
   src/CPUSide.h
   src/EnergyTypes.h
   src/EnPartCntSampleOutput.h
   src/EnsemblePreprocessor.h
   src/Ewald.h
   src/EwaldCached.h  
   src/FFAngles.h
   src/FFBonds.h
   src/FFConst.h
   src/FFDihedrals.h
   src/FFParticle.h
   src/FFSetup.h
   src/FFShift.h
   src/FFSwitch.h
   src/FFSwitchMartini.h
   src/FixedWidthReader.h
   src/Forcefield.h
   src/FxdWidthWrtr.h
   src/Geometry.h
   src/HistOutput.h
   src/InputAbstracts.h
   src/InputFileReader.h
   src/IntraSwap.h
   src/MersenneTwister.h
   src/MoleculeExchange1.h
   src/MoleculeExchange2.h
   src/MoleculeExchange3.h
   src/MoleculeKind.h
   src/MoleculeLookup.h
   src/MoleculeTransfer.h
   src/Molecules.h
   src/MolPick.h
   src/MolSetup.h
   src/MoveBase.h
   src/MoveConst.h
   src/MoveSettings.h
   src/NoEwald.h
   src/OutConst.h
   src/OutputAbstracts.h
   src/OutputVars.h
   src/PDBConst.h
   src/PDBOutput.h
   src/PDBSetup.h
   src/PRNG.h
   src/PRNGSetup.h
   src/PSFOutput.h
   src/Reader.h
   src/Regrowth.h
   src/SeedReader.h
   src/Setup.h
   src/SimEventFrequency.h
   src/Simulation.h
   src/StaticVals.h
   src/SubdividedArray.h
   src/System.h
   src/TransformMatrix.h
   src/Writer.h
   src/XYZArray.h
   src/cbmc/DCComponent.h
   src/cbmc/DCData.h
   src/cbmc/DCFreeHedron.h
   src/cbmc/DCFreeHedronSeed.h
   src/cbmc/DCLinkedHedron.h
   src/cbmc/DCGraph.h
   src/cbmc/DCHedron.h
   src/cbmc/DCLinear.h
   src/cbmc/DCOnSphere.h
   src/cbmc/DCRotateCOM.h
   src/cbmc/DCSingle.h
   src/cbmc/TrialMol.h)

set(libHeaders
   lib/BasicTypes.h
   lib/BitLib.h
   lib/GeomLib.h
   lib/NumLib.h
   lib/StrLib.h
   lib/StrStrmLib.h
   lib/VectorLib.h)

set(cudaHeaders
    src/GPU/ConstantDefinitionsCUDAKernel.cuh
    src/GPU/CalculateMinImageCUDAKernel.cuh
    src/GPU/CalculateEnergyCUDAKernel.cuh
    src/GPU/CalculateForceCUDAKernel.cuh
    src/GPU/CalculateEwaldCUDAKernel.cuh
    src/GPU/VariablesCUDA.cuh)

set(cudaSources
    src/GPU/CalculateEnergyCUDAKernel.cu
    src/GPU/CalculateForceCUDAKernel.cu
    src/GPU/CalculateEwaldCUDAKernel.cu
    src/GPU/ConstantDefinitionsCUDAKernel.cu)

source_group("Header Files" FILES ${headers})
source_group("Lib Headers" FILES ${libHeaders})
source_group("CUDA Header Files" FILES ${cudaHeaders})
source_group("CUDA Source Files" FILES ${cudaSources})
