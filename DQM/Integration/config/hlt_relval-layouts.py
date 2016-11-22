###
# """
# file: DQM/Integration/config/hlt_relval-layouts.py
# 
# Layout file for trigger release validation
#  facilitating organization/regrouping of subsystem histograms
# -use subsystem top-folder as specified in previous stages eg
#  def trigval<subsys>(i, p, *rows): i["HLT/<subsys>/Preselection" + p] = DQMItem(layout=rows)
# """
###


###---- EGAMMA selection goes here: ----

def trigvalegammaZ(i, p, *rows): i["HLT/HLTEgammaValidation/Zee Preselection/" + p] = DQMItem(layout=rows)

trigvalegammaZ(dqmitems,"doubleEle5SWL1R/total",
           [{'path': "HLT/HLTEgammaValidation/HLT_DoubleEle5_SW_L1RDQMZee/efficiency_by_step_MC_matched", 'description':"per-event efficiency (MC matched) for doubleEle5SWL1R"},
            {'path': "HLT/HLTEgammaValidation/HLT_DoubleEle5_SW_L1RDQMZee/efficiency_by_step", 'description':"per-event efficiency (MC matched) for doubleEle5SWL1R"}])

trigvalegammaZ(dqmitems,"doubleEle5SWL1R/kinematics",
           [{'path': "HLT/HLTEgammaValidation/HLT_DoubleEle5_SW_L1RDQMZee/gen_et", 'description':"per-event efficiency (MC matched) for doubleEle5SWL1R"},
            {'path': "HLT/HLTEgammaValidation/HLT_DoubleEle5_SW_L1RDQMZee/gen_eta", 'description':"per-event efficiency (MC matched) for doubleEle5SWL1R"}])

trigvalegammaZ(dqmitems,"doubleEle5SWL1R/L1 match",
           [{'path': "HLT/HLTEgammaValidation/HLT_DoubleEle5_SW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoDoubleElectronEt5L1MatchFilterRegional_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoDoubleElectronEt5L1MatchFilterRegional in doubleEle5SWL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_DoubleEle5_SW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoDoubleElectronEt5L1MatchFilterRegional_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoDoubleElectronEt5L1MatchFilterRegional in doubleEle5SWL1R_vs_et"}])

trigvalegammaZ(dqmitems,"doubleEle5SWL1R/Et cut",
           [{'path': "HLT/HLTEgammaValidation/HLT_DoubleEle5_SW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoDoubleElectronEt5EtFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoDoubleElectronEt5EtFilter in doubleEle5SWL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_DoubleEle5_SW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoDoubleElectronEt5EtFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoDoubleElectronEt5EtFilter in doubleEle5SWL1R_vs_et"}])

trigvalegammaZ(dqmitems,"doubleEle5SWL1R/Hcal isolation",
           [{'path': "HLT/HLTEgammaValidation/HLT_DoubleEle5_SW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoDoubleElectronEt5HcalIsolFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonHLTnonIsoIsoDoubleElectronEt5HcalIsolFilter in doubleEle5SWL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_DoubleEle5_SW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoDoubleElectronEt5HcalIsolFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonHLTnonIsoIsoDoubleElectronEt5HcalIsolFilter in doubleEle5SWL1R_vs_et"}])

trigvalegammaZ(dqmitems,"doubleEle5SWL1R/pixel match",
           [{'path': "HLT/HLTEgammaValidation/HLT_DoubleEle5_SW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoDoubleElectronEt5PixelMatchFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoDoubleElectronEt5PixelMatchFilter in doubleEle5SWL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_DoubleEle5_SW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoDoubleElectronEt5PixelMatchFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoDoubleElectronEt5PixelMatchFilter in doubleEle5SWL1R_vs_et"}])



trigvalegammaZ(dqmitems,"Ele10LWL1R/total",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMZee/efficiency_by_step_MC_matched", 'description':"per-event efficiency (MC matched) for Ele10LWL1R"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMZee/efficiency_by_step", 'description':"per-event efficiency (MC matched) for Ele10LWL1R"}])

trigvalegammaZ(dqmitems,"Ele10LWL1R/kinematics",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMZee/gen_et", 'description':"per-event efficiency (MC matched) for Ele10LWL1R"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMZee/gen_eta", 'description':"per-event efficiency (MC matched) for Ele10LWL1R"}])

trigvalegammaZ(dqmitems,"Ele10LWL1R/L1 match",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional in Ele10LWL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional in Ele10LWL1R_vs_et"}])

trigvalegammaZ(dqmitems,"Ele10LWL1R/Et cut",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter in Ele10LWL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter in Ele10LWL1R_vs_et"}])

trigvalegammaZ(dqmitems,"Ele10LWL1R/Hcal isolation",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter in Ele10LWL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter in Ele10LWL1R_vs_et"}])

trigvalegammaZ(dqmitems,"Ele10LWL1R/pixel match",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter in Ele10LWL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter in Ele10LWL1R_vs_et"}])



trigvalegammaZ(dqmitems,"Ele10LWEleIdL1R/total",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_by_step_MC_matched", 'description':"per-event efficiency (MC matched) for Ele10LWEleIdL1R"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_by_step", 'description':"per-event efficiency (MC matched) for Ele10LWEleIdL1R"}])

trigvalegammaZ(dqmitems,"Ele10LWEleIdL1R/kinematics",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/gen_et", 'description':"per-event efficiency (MC matched) for Ele10LWEleIdL1R"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/gen_eta", 'description':"per-event efficiency (MC matched) for Ele10LWEleIdL1R"}])

trigvalegammaZ(dqmitems,"Ele10LWEleIdL1R/L1 match",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional_vs_et_MC_matched" , 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional in Ele10LWEleIdL1R_vs_et"}])

trigvalegammaZ(dqmitems,"Ele10LWEleIdL1R/Et cut",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter_vs_et_MC_matched",  'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter in Ele10LWEleIdL1R_vs_et"}])

trigvalegammaZ(dqmitems,"Ele10LWEleIdL1R/cluster shape cut",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter in Ele10LWEleIdL1R_vs_et"}])

trigvalegammaZ(dqmitems,"Ele10LWEleIdL1R/Hcal isolation",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter_vs_et_MC_matched",  'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter in Ele10LWEleIdL1R_vs_et"}])

trigvalegammaZ(dqmitems,"Ele10LWEleIdL1R/pixel match",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter_vs_et_MC_matched",  'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter in Ele10LWEleIdL1R_vs_et"}])

trigvalegammaZ(dqmitems,"Ele10LWEleIdL1R/ 1oE - 1op cut",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter in Ele10LWEleIdL1R_vs_et"}])

trigvalegammaZ(dqmitems,"Ele10LWEleIdL1R/ delta-eta cut",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter in Ele10LWEleIdL1R_vs_et"}])

trigvalegammaZ(dqmitems,"Ele10LWEleIdL1R/ delta-phi cut",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter in Ele10LWEleIdL1R_vs_et"}])



                            

def hltlayoutW(i, p, *rows): i["HLT/HLTEgammaValidation/Wenu Preselection/" + p] = DQMItem(layout=rows)

hltlayoutW(dqmitems,"Ele10LWL1R/total",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMWenu/efficiency_by_step_MC_matched", 'description':"per-event efficiency (MC matched) for Ele10LWL1R"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMWenu/efficiency_by_step", 'description':"per-event efficiency (MC matched) for Ele10LWL1R"}])

hltlayoutW(dqmitems,"Ele10LWL1R/kinematics",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMWenu/gen_et", 'description':"per-event efficiency (MC matched) for Ele10LWL1R"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMWenu/gen_eta", 'description':"per-event efficiency (MC matched) for Ele10LWL1R"}])

hltlayoutW(dqmitems,"Ele10LWL1R/L1 match",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional in Ele10LWL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional in Ele10LWL1R_vs_et"}])

hltlayoutW(dqmitems,"Ele10LWL1R/Et cut",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter in Ele10LWL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter in Ele10LWL1R_vs_et"}])

hltlayoutW(dqmitems,"Ele10LWL1R/Hcal isolation",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter in Ele10LWL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter in Ele10LWL1R_vs_et"}])

hltlayoutW(dqmitems,"Ele10LWL1R/pixel match",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter in Ele10LWL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter in Ele10LWL1R_vs_et"}])



hltlayoutW(dqmitems,"Ele10LWEleIdL1R/total",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_by_step_MC_matched", 'description':"per-event efficiency (MC matched) for Ele10LWEleIdL1R"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_by_step", 'description':"per-event efficiency (MC matched) for Ele10LWEleIdL1R"}])

hltlayoutW(dqmitems,"Ele10LWEleIdL1R/kinematics",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/gen_et", 'description':"per-event efficiency (MC matched) for Ele10LWEleIdL1R"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/gen_eta", 'description':"per-event efficiency (MC matched) for Ele10LWEleIdL1R"}])

hltlayoutW(dqmitems,"Ele10LWEleIdL1R/L1 match",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional_vs_et_MC_matched" , 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional in Ele10LWEleIdL1R_vs_et"}])

hltlayoutW(dqmitems,"Ele10LWEleIdL1R/Et cut",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter_vs_et_MC_matched",  'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter in Ele10LWEleIdL1R_vs_et"}])

hltlayoutW(dqmitems,"Ele10LWEleIdL1R/cluster shape cut",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter in Ele10LWEleIdL1R_vs_et"}])

hltlayoutW(dqmitems,"Ele10LWEleIdL1R/Hcal isolation",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter_vs_et_MC_matched",  'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter in Ele10LWEleIdL1R_vs_et"}])

hltlayoutW(dqmitems,"Ele10LWEleIdL1R/pixel match",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter_vs_et_MC_matched",  'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter in Ele10LWEleIdL1R_vs_et"}])

hltlayoutW(dqmitems,"Ele10LWEleIdL1R/ 1oE - 1op cut",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter in Ele10LWEleIdL1R_vs_et"}])

hltlayoutW(dqmitems,"Ele10LWEleIdL1R/ delta-eta cut",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter in Ele10LWEleIdL1R_vs_et"}])

hltlayoutW(dqmitems,"Ele10LWEleIdL1R/ delta-phi cut",
           [{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter_vs_eta_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter in Ele10LWEleIdL1R_vs_eta"},
            {'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter_vs_et_MC_matched", 'description':"per-object (MC matched) for hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter in Ele10LWEleIdL1R_vs_et"}])






###---- MUON selection goes here: ----

def trigvalmuon(i, p, *rows): i["HLT/Muon/Preselection" + p] = DQMItem(layout=rows)
#trigvalmuon(dqmitems,"", [{'path': "", 'description':""}, {'path': "", 'description':""}])

###---- TAU selection goes here: ----
def trigvaltau(i, p, *rows): i["HLT/HLTTAU/Preselection" + p] = DQMItem(layout=rows)

###---- JETMET selection goes here: ----
#def trigvaljetmet(i, p, *rows): i["HLT//Preselection" + p] = DQMItem(layout=rows)

###---- BJET selection goes here: ----
#def trigvalbjet(i, p, *rows): i["HLT//Preselection" + p] = DQMItem(layout=rows)

###---- ALCA selection goes here: ----
def trigvalalca(i, p, *rows): i["HLT/AlCaEcalPi0/Preselection" + p] = DQMItem(layout=rows)

###---- TOP selection goes here: ----
def trigvaltop(i, p, *rows): i["HLT/Top/Preselection" + p] = DQMItem(layout=rows)

###---- HEAVYFLAVOR selection goes here: ----
def trigvalbphys(i, p, *rows): i["HLT/HeavyFlavor/Preselection" + p] = DQMItem(layout=rows)

###---- SUSYEXO selection goes here: ----
def trigvalsusybsm(i, p, *rows): i["HLT/SusyExo/Preselection" + p] = DQMItem(layout=rows)

###---- HIGGS selection goes here: ----
#def trigvalhiggs(i, p, *rows): i["HLT//Preselection" + p] = DQMItem(layout=rows)
###---- QCD selection goes here: ----
#def trigvalqcd(i, p, *rows): i["HLT//Preselection" + p] = DQMItem(layout=rows)
