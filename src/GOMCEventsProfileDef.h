GOMC_PROFILE_EVENT(INITIALIZE, "initialization")
GOMC_PROFILE_EVENT(MC_RUN, "MC_run")
GOMC_PROFILE_EVENT(DESTRUCTION, "destruction")
GOMC_PROFILE_EVENT(READ_INPUT_FILES, "read_input_files")
// I/O output
GOMC_PROFILE_EVENT(FILE_IO_OUTPUT, "output_files")
GOMC_PROFILE_EVENT(BLK_OUTPUT, "block_file")
GOMC_PROFILE_EVENT(DIST_OUTPUT, "distribution_file")
GOMC_PROFILE_EVENT(HIST_OUTPUT, "histogram_file")
GOMC_PROFILE_EVENT(DCD_OUTPUT, "dcd_xst_file")
GOMC_PROFILE_EVENT(DCD_RESTART_OUTPUT, "dcd_xsc_restart_file")
GOMC_PROFILE_EVENT(PDB_OUTPUT, "pdb_file")
GOMC_PROFILE_EVENT(PDB_RESTART_OUTPUT, "pdb_restart_file")
GOMC_PROFILE_EVENT(PSF_MERGED_OUTPUT, "psf_merged_file")
GOMC_PROFILE_EVENT(PSF_RESTART_OUTPUT, "psf_restart_file")
GOMC_PROFILE_EVENT(CHECKPOINT_OUTPUT, "checkpoint_file")
GOMC_PROFILE_EVENT(CONSOLE_OUTPUT, "console_file")
GOMC_PROFILE_EVENT(FREE_ENERGY_OUTPUT, "free_energy_file")
// MC moves
GOMC_PROFILE_EVENT(DISPLACE, "translate_move")
GOMC_PROFILE_EVENT(ROTATE, "rotate_move")
GOMC_PROFILE_EVENT(MULTIPARTICLE, "multiParticle_move")
GOMC_PROFILE_EVENT(MULTIPARTICLE_BM, "multiParticleBM_move")
GOMC_PROFILE_EVENT(INTRA_SWAP, "intraSwap_move")
GOMC_PROFILE_EVENT(INTRA_MEMC, "intraMEMC_move")
GOMC_PROFILE_EVENT(CRANKSHAFT, "crankshaft_move")
GOMC_PROFILE_EVENT(REGROWTH, "regrowth_move")
GOMC_PROFILE_EVENT(SWAP, "swap_move")
GOMC_PROFILE_EVENT(TARGETED_SWAP, "swap_move")
GOMC_PROFILE_EVENT(MEMC, "MEMC_move")
GOMC_PROFILE_EVENT(VOL_TRANSFER, "volumeChange_move")
GOMC_PROFILE_EVENT(CFCMC, "CFCMC_move")
// prep MC moves
GOMC_PROFILE_EVENT(PREP_DISPLACE, "prepare_translate_move")
GOMC_PROFILE_EVENT(PREP_ROTATE, "prepare_rotate_move")
GOMC_PROFILE_EVENT(PREP_MULTIPARTICLE, "prepare_multiParticle_move")
GOMC_PROFILE_EVENT(PREP_MULTIPARTICLE_BM, "prepare_multiParticleBM_move")
GOMC_PROFILE_EVENT(PREP_INTRA_SWAP, "prepare_intraSwap_move")
GOMC_PROFILE_EVENT(PREP_INTRA_MEMC, "prepare_intraMEMC_move")
GOMC_PROFILE_EVENT(PREP_CRANKSHAFT, "prepare_crankshaft_move")
GOMC_PROFILE_EVENT(PREP_REGROWTH, "prepare_regrowth_move")
GOMC_PROFILE_EVENT(PREP_SWAP, "prepare_swap_move")
GOMC_PROFILE_EVENT(PREP_TARGETED_SWAP, "prepare_swap_move")
GOMC_PROFILE_EVENT(PREP_MEMC, "prepare_MEMC_move")
GOMC_PROFILE_EVENT(PREP_VOL_TRANSFER, "prepare_volumeChange_move")
GOMC_PROFILE_EVENT(PREP_CFCMC, "prepare_CFCMC_move")
//transform MC moves
GOMC_PROFILE_EVENT(TRANS_DISPLACE, "transform_translate_move")
GOMC_PROFILE_EVENT(TRANS_ROTATE, "transform_rotate_move")
GOMC_PROFILE_EVENT(TRANS_MULTIPARTICLE, "transform_multiParticle_move")
GOMC_PROFILE_EVENT(TRANS_MULTIPARTICLE_BM, "transform_multiParticleBM_move")
GOMC_PROFILE_EVENT(TRANS_INTRA_SWAP, "transform_intraSwap_move")
GOMC_PROFILE_EVENT(TRANS_INTRA_MEMC, "transform_intraMEMC_move")
GOMC_PROFILE_EVENT(TRANS_CRANKSHAFT, "transform_crankshaft_move")
GOMC_PROFILE_EVENT(TRANS_REGROWTH, "transform_regrowth_move")
GOMC_PROFILE_EVENT(TRANS_SWAP, "transform_swap_move")
GOMC_PROFILE_EVENT(TRANS_TARGETED_SWAP, "transform_swap_move")
GOMC_PROFILE_EVENT(TRANS_MEMC, "transform_MEMC_move")
GOMC_PROFILE_EVENT(TRANS_VOL_TRANSFER, "transform_volumeChange_move")
GOMC_PROFILE_EVENT(TRANS_CFCMC, "transform_CFCMC_move")
//calcEn MC moves
GOMC_PROFILE_EVENT(CALC_EN_DISPLACE, "cal_energy_translate_move")
GOMC_PROFILE_EVENT(CALC_EN_ROTATE, "cal_energy_rotate_move")
GOMC_PROFILE_EVENT(CALC_EN_MULTIPARTICLE, "cal_energy_multiParticle_move")
GOMC_PROFILE_EVENT(CALC_EN_MULTIPARTICLE_BM, "cal_energy_multiParticleBM_move")
GOMC_PROFILE_EVENT(CALC_EN_INTRA_SWAP, "cal_energy_intraSwap_move")
GOMC_PROFILE_EVENT(CALC_EN_INTRA_MEMC, "cal_energy_intraMEMC_move")
GOMC_PROFILE_EVENT(CALC_EN_CRANKSHAFT, "cal_energy_crankshaft_move")
GOMC_PROFILE_EVENT(CALC_EN_REGROWTH, "cal_energy_regrowth_move")
GOMC_PROFILE_EVENT(CALC_EN_SWAP, "cal_energy_swap_move")
GOMC_PROFILE_EVENT(CALC_EN_TARGETED_SWAP, "cal_energy_swap_move")
GOMC_PROFILE_EVENT(CALC_EN_MEMC, "cal_energy_MEMC_move")
GOMC_PROFILE_EVENT(CALC_EN_VOL_TRANSFER, "cal_energy_volumeChange_move")
GOMC_PROFILE_EVENT(CALC_EN_CFCMC, "cal_energy_CFCMC_move")
//accept MC moves
GOMC_PROFILE_EVENT(ACC_DISPLACE, "accept_translate_move")
GOMC_PROFILE_EVENT(ACC_ROTATE, "accept_rotate_move")
GOMC_PROFILE_EVENT(ACC_MULTIPARTICLE, "accept_multiParticle_move")
GOMC_PROFILE_EVENT(ACC_MULTIPARTICLE_BM, "accept_multiParticleBM_move")
GOMC_PROFILE_EVENT(ACC_INTRA_SWAP, "accept_intraSwap_move")
GOMC_PROFILE_EVENT(ACC_INTRA_MEMC, "accept_intraMEMC_move")
GOMC_PROFILE_EVENT(ACC_CRANKSHAFT, "accept_crankshaft_move")
GOMC_PROFILE_EVENT(ACC_REGROWTH, "accept_regrowth_move")
GOMC_PROFILE_EVENT(ACC_SWAP, "accept_swap_move")
GOMC_PROFILE_EVENT(ACC_TARGETED_SWAP, "accept_swap_move")
GOMC_PROFILE_EVENT(ACC_MEMC, "accept_MEMC_move")
GOMC_PROFILE_EVENT(ACC_VOL_TRANSFER, "accept_volumeChange_move")
GOMC_PROFILE_EVENT(ACC_CFCMC, "accept_CFCMC_move")
// energy, virial, force term for LJ and real part of Ewald
GOMC_PROFILE_EVENT(EN_SYSTEM_TOTAL, "energy_system_total(intra,inter,self,corr)")
GOMC_PROFILE_EVENT(EN_BOX_INTRA, "energy_box_intra(intra,self,corr)")
GOMC_PROFILE_EVENT(EN_BOX_INTER, "energy_box_inter")
GOMC_PROFILE_EVENT(EN_BOX_FORCE, "energy_box_force")
GOMC_PROFILE_EVENT(BOX_TORQUE, "energy_box_torque")
GOMC_PROFILE_EVENT(EN_BOX_VIRIAL, "energy_box_virial")
GOMC_PROFILE_EVENT(EN_MOL_INTER, "energy_molecule_inter")
GOMC_PROFILE_EVENT(EN_MOL_INTRA, "energy_molecule_intra")
GOMC_PROFILE_EVENT(EN_CBMC_INTER, "energy_CBMC_inter")
GOMC_PROFILE_EVENT(EN_CBMC_INTRA_NB, "energy_CBMC_intra_nonbonded")
GOMC_PROFILE_EVENT(FREE_ENERGY, "free_energy")
// energy, virial, force term for self, correction, and recip Ewald
GOMC_PROFILE_EVENT(RECIP_INIT, "ewald_box_recip_initialization")
GOMC_PROFILE_EVENT(RECIP_BOX_SETUP, "ewald_box_recip_setup")
GOMC_PROFILE_EVENT(RECIP_BOX_ENERGY, "ewald_box_recip_energy")
GOMC_PROFILE_EVENT(RECIP_BOX_FORCE, "ewald_box_recip_force")
GOMC_PROFILE_EVENT(RECIP_BOX_VIRIAL, "ewald_box_recip_virial")
GOMC_PROFILE_EVENT(SELF_BOX, "ewald_box_self_energy")
GOMC_PROFILE_EVENT(CORR_BOX, "ewald_box_correction_energy")
GOMC_PROFILE_EVENT(RECIP_MOL_ENERGY, "ewald_molecule_recip_energy")
GOMC_PROFILE_EVENT(RECIP_SWAP_ENERGY, "ewald_molecule_swap_recip_energy")
GOMC_PROFILE_EVENT(RECIP_MEMC_ENERGY, "ewald_molecule_MEMC_recip_energy")
GOMC_PROFILE_EVENT(RECIP_CFCMC_ENERGY, "ewald_molecule_CFCMC_recip_energy")
GOMC_PROFILE_EVENT(SELF_MOL, "ewald_molecule_self_energy")
GOMC_PROFILE_EVENT(SELF_SWAP, "ewald_molecule_swap_self_energy")
GOMC_PROFILE_EVENT(CORR_MOL, "ewald_molecule_correction_energy")
GOMC_PROFILE_EVENT(CORR_SWAP, "ewald_molecule_swap_correction_energy")
// dummy events
GOMC_PROFILE_EVENT(DUMMY_EVENT1, "Dummy Event1")
GOMC_PROFILE_EVENT(DUMMY_EVENT2, "Dummy Event2")
GOMC_PROFILE_EVENT(DUMMY_EVENT3, "Dummy Event3")
GOMC_PROFILE_EVENT(DUMMY_EVENT4, "Dummy Event4")
GOMC_PROFILE_EVENT(DUMMY_EVENT5, "Dummy Event5")
GOMC_PROFILE_EVENT(DUMMY_EVENT6, "Dummy Event6")
GOMC_PROFILE_EVENT(DUMMY_EVENT7, "Dummy Event7")
GOMC_PROFILE_EVENT(DUMMY_EVENT8, "Dummy Event8")
GOMC_PROFILE_EVENT(DUMMY_EVENT9, "Dummy Event9")
GOMC_PROFILE_EVENT(DUMMY_EVENT10, "Dummy Event10")
GOMC_PROFILE_EVENT(DUMMY_EVENT11, "Dummy Event11")
GOMC_PROFILE_EVENT(DUMMY_EVENT12, "Dummy Event12")
GOMC_PROFILE_EVENT(DUMMY_EVENT13, "Dummy Event13")
GOMC_PROFILE_EVENT(DUMMY_EVENT14, "Dummy Event14")
GOMC_PROFILE_EVENT(DUMMY_EVENT15, "Dummy Event15")
GOMC_PROFILE_EVENT(DUMMY_EVENT16, "Dummy Event16")
GOMC_PROFILE_EVENT(DUMMY_EVENT17, "Dummy Event17")
GOMC_PROFILE_EVENT(DUMMY_EVENT18, "Dummy Event18")
GOMC_PROFILE_EVENT(DUMMY_EVENT19, "Dummy Event19")
GOMC_PROFILE_EVENT(DUMMY_EVENT20, "Dummy Event20")
GOMC_PROFILE_EVENT(DUMMY_EVENT21, "Dummy Event21")
GOMC_PROFILE_EVENT(DUMMY_EVENT22, "Dummy Event22")



