###
# """
# file: DQM/Integration/config/shift_hlt_relval_layout.py
# 
# This layout file contains hlt me selection for release validation
# -each subsystem specifies below O(10) most important validation histograms 
# -histogram folder is specified as 
#  def hltval<subsys>(i, p, *rows): i["00 Shift/TRIGGER Validation/HLT <subsys>" + p] = DQMItem(layout=rows)
# -selected histograms are specified as
#  hltval<subsys>(dqmitems,"doubleEle5SWL1R",
#  [{'path': "path to histogram as HLT/<subsys>/<my folder>/<my histo>",
#   'description':"summary histogram description"}])
# """
###


###---- GENERIC - FourVector selection goes here: ####
######################################################

###---- GENERIC - FourVector Muon
def trigvalFVMuon(i, p, *rows): i["00 Shift/HLT/FourVector/Muon/" + p] = DQMItem(layout=rows)
  

trigvalFVMuon(dqmitems,"Eff HLT to MC",
[{'path': "HLT/FourVector_Val/client/HLT_Mu9/custom-eff/HLT_Mu9_wrt__mcEt_Eff_OnToMc", 'description':"Efficiency of HLT to MC for path HLT_Mu9"}])

trigvalFVMuon(dqmitems,"Eff HLT to MC eta-phi",
[{'path': "HLT/FourVector_Val/client/HLT_Mu9/custom-eff/HLT_Mu9_wrt__mcEtamcPhi_Eff_OnToMc", 'description':"Efficiency of HLT to MC for path HLT_Mu9"}])

trigvalFVMuon(dqmitems,"Eff HLT to L1",
[{'path': "HLT/FourVector_Val/client/HLT_Mu9/custom-eff/HLT_Mu9_wrt__l1Et_Eff_OnToL1", 'description':"Efficiency of HLT to L1 for path HLT_Mu9"}])

trigvalFVMuon(dqmitems,"Eff HLT to RECO",
[{'path': "HLT/FourVector_Val/client/HLT_Mu9/custom-eff/HLT_Mu9_wrt__offEt_Eff_OnToOff", 'description':"Efficiency of HLT to RECO for path HLT_Mu9"}])

trigvalFVMuon(dqmitems,"Eff L1 to MC"
[{'path': "HLT/FourVector_Val/client/HLT_Mu9/custom-eff/HLT_Mu9_wrt__mcEt_Eff_L1ToMc", 'description':"Efficiency of L1 to MC for path HLT_Mu9"}])

trigvalFVMuon(dqmitems,"Eff L1 to RECO",
[{'path': "HLT/FourVector_Val/client/HLT_Mu9/custom-eff/HLT_Mu9_wrt__offEt_Eff_L1ToOff", 'description':"Efficiency of L1 to RECO for path HLT_Mu9"}])

###---- GENERIC - FourVector Electron
def trigvalFVEle(i, p, *rows): i["00 Shift/HLT/FourVector/Electron/" + p] = DQMItem(layout=rows)
  

trigvalFVEle(dqmitems,"Eff HLT to MC",
[{'path': "HLT/FourVector_Val/client/HLT_Ele10_LW_L1R/custom-eff/HLT_Ele10_LW_L1R_wrt__mcEt_Eff_OnToMc", 'description':"Efficiency of HLT to MC for path HLT_Ele10_LW_L1R"}])

trigvalFVMuon(dqmitems,"Eff HLT to MC eta-phi",
[{'path': "HLT/FourVector_Val/client/HLT_Ele10_LW_L1R/custom-eff/HLT_Ele10_LW_L1R_wrt__mcEtamcPhi_Eff_OnToMc", 'description':"Efficiency of HLT to MC for path HLT_Ele10_LW_L1R"}])

trigvalFVEle(dqmitems,"Eff HLT to L1",
[{'path': "HLT/FourVector_Val/client/HLT_Ele10_LW_L1R/custom-eff/HLT_Ele10_LW_L1R_wrt__l1Et_Eff_OnToL1", 'description':"Efficiency of HLT to L1 for path HLT_Ele10_LW_L1R"}])

trigvalFVEle(dqmitems,"Eff HLT to RECO",
[{'path': "HLT/FourVector_Val/client/HLT_Ele10_LW_L1R/custom-eff/HLT_Ele10_LW_L1R_wrt__offEt_Eff_OnToOff", 'description':"Efficiency of HLT to RECO for path HLT_Ele10_LW_L1R"}])

trigvalFVEle(dqmitems,"Eff L1 to MC"
[{'path': "HLT/FourVector_Val/client/HLT_Ele10_LW_L1R/custom-eff/HLT_Ele10_LW_L1R_wrt__mcEt_Eff_L1ToMc", 'description':"Efficiency of L1 to MC for path HLT_Ele10_LW_L1R"}])

trigvalFVEle(dqmitems,"Eff L1 to RECO",
[{'path': "HLT/FourVector_Val/client/HLT_Ele10_LW_L1R/custom-eff/HLT_Ele10_LW_L1R_wrt__offEt_Eff_L1ToOff", 'description':"Efficiency of L1 to RECO for path HLT_Ele10_LW_L1R"}])

###---- GENERIC - FourVector Jet
def trigvalFVJet(i, p, *rows): i["00 Shift/HLT/FourVector/Jet/" + p] = DQMItem(layout=rows)
  

trigvalFVJet(dqmitems,"Eff HLT to MC",
[{'path': "HLT/FourVector_Val/client/HLT_Jet50/custom-eff/HLT_Jet50_wrt__mcEt_Eff_OnToMc", 'description':"Efficiency of HLT to MC for path HLT_Jet50"}])

trigvalFVMuon(dqmitems,"Eff HLT to MC eta-phi",
[{'path': "HLT/FourVector_Val/client/HLT_Jet50/custom-eff/HLT_Jet50_wrt__mcEtamcPhi_Eff_OnToMc", 'description':"Efficiency of HLT to MC for path HLT_Jet50"}])

trigvalFVJet(dqmitems,"Eff HLT to L1",
[{'path': "HLT/FourVector_Val/client/HLT_Jet50/custom-eff/HLT_Jet50_wrt__l1Et_Eff_OnToL1", 'description':"Efficiency of HLT to L1 for path HLT_Jet50"}])

trigvalFVJet(dqmitems,"Eff HLT to RECO",
[{'path': "HLT/FourVector_Val/client/HLT_Jet50/custom-eff/HLT_Jet50_wrt__offEt_Eff_OnToOff", 'description':"Efficiency of HLT to RECO for path HLT_Jet50"}])

trigvalFVJet(dqmitems,"Eff L1 to MC"
[{'path': "HLT/FourVector_Val/client/HLT_Jet50/custom-eff/HLT_Jet50_wrt__mcEt_Eff_L1ToMc", 'description':"Efficiency of L1 to MC for path HLT_Jet50"}])

trigvalFVJet(dqmitems,"Eff L1 to RECO",
[{'path': "HLT/FourVector_Val/client/HLT_Jet50/custom-eff/HLT_Jet50_wrt__offEt_Eff_L1ToOff", 'description':"Efficiency of L1 to RECO for path HLT_Jet50"}])

###---- GENERIC - FourVector Photon
def trigvalFVPho(i, p, *rows): i["00 Shift/HLT/FourVector/Photon/" + p] = DQMItem(layout=rows)
  

trigvalFVPho(dqmitems,"Eff HLT to MC",
[{'path': "HLT/FourVector_Val/client/HLT_Photon15_L1R/custom-eff/HLT_Photon15_L1R_wrt__mcEt_Eff_OnToMc", 'description':"Efficiency of HLT to MC for path HLT_Photon15_L1R"}])

trigvalFVMuon(dqmitems,"Eff HLT to MC eta-phi",
[{'path': "HLT/FourVector_Val/client/HLT_Photon15_L1R/custom-eff/HLT_Photon15_L1R_wrt__mcEtamcPhi_Eff_OnToMc", 'description':"Efficiency of HLT to MC for path HLT_Photon15_L1R"}])

trigvalFVPho(dqmitems,"Eff HLT to L1",
[{'path': "HLT/FourVector_Val/client/HLT_Photon15_L1R/custom-eff/HLT_Photon15_L1R_wrt__l1Et_Eff_OnToL1", 'description':"Efficiency of HLT to L1 for path HLT_Photon15_L1R"}])

trigvalFVPho(dqmitems,"Eff HLT to RECO",
[{'path': "HLT/FourVector_Val/client/HLT_Photon15_L1R/custom-eff/HLT_Photon15_L1R_wrt__offEt_Eff_OnToOff", 'description':"Efficiency of HLT to RECO for path HLT_Photon15_L1R"}])

trigvalFVPho(dqmitems,"Eff L1 to MC"
[{'path': "HLT/FourVector_Val/client/HLT_Photon15_L1R/custom-eff/HLT_Photon15_L1R_wrt__mcEt_Eff_L1ToMc", 'description':"Efficiency of L1 to MC for path HLT_Photon15_L1R"}])

trigvalFVPho(dqmitems,"Eff L1 to RECO",
[{'path': "HLT/FourVector_Val/client/HLT_Photon15_L1R/custom-eff/HLT_Photon15_L1R_wrt__offEt_Eff_L1ToOff", 'description':"Efficiency of L1 to RECO for path HLT_Photon15_L1R"}])


###---- EGAMMA selection goes here: ----

def trigvalegammaZ(i, p, *rows): i["00 Shift/HLT/Egamma/Zee Preselection/" + p] = DQMItem(layout=rows)
  
trigvalegammaZ(dqmitems,"doubleEle5SWL1R",
[{'path': "HLT/HLTEgammaValidation/HLT_DoubleEle5_SW_L1RDQMZee/efficiency_by_step_MC_matched", 'description':"per-event efficiency (MC matched) for doubleEle5SWL1R"}])

trigvalegammaZ(dqmitems,"Ele10LWL1R",
[{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMZee/efficiency_by_step_MC_matched", 'description':"per-event efficiency (MC matched) for Ele10LWL1R"}])

trigvalegammaZ(dqmitems,"Ele10LWEleIdL1R",
[{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMZee/efficiency_by_step_MC_matched", 'description':"per-event efficiency (MC matched) for Ele10LWEleIdL1R"}])

def trigvalegammaW(i, p, *rows): i["00 Shift/HLT/Egamma/Wenu Preselection/" + p] = DQMItem(layout=rows)

trigvalegammaW(dqmitems,"Ele10LWL1R",
[{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_L1RDQMWenu/efficiency_by_step_MC_matched", 'description':"per-event efficiency (MC matched) for Ele10LWL1R"}])

trigvalegammaW(dqmitems,"Ele10LWEleIdL1R",
[{'path': "HLT/HLTEgammaValidation/HLT_Ele10_LW_EleId_L1RDQMWenu/efficiency_by_step_MC_matched", 'description':"per-event efficiency (MC matched) for Ele10LWEleIdL1R"}])


###---- MUON selection goes here: ----

def trigvalmuon(i, p, *rows): i["00 Shift/HLT/Muon/" + p] = DQMItem(layout=rows)
trigvalmuon(dqmitems,"Efficiency L1 vs Gen",
  	[{'path': "HLT/Muon/Distributions/HLT_IsoMu9/genEffEta_L1Filtered", 'description': "L1 trigger efficiency for IsoMu9 versus eta. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/MuonHLTOfflinePerformance\">here</a>."}])



###---- TAU selection goes here: ----

def trigvaltau(i, p, *rows): i["00 Shift/HLT/Tau/" + p] = DQMItem(layout=rows)
trigvaltau(dqmitems,"Single Tau",
  	[{'path': "HLT/HLTTAU/SingleTau/Triggers", 'description': "Accepted events per single tau path. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/TauTriggerValidation\">here</a>."}])



###---- JETMET selection goes here: ----
def trigvaljetmet(i, p, *rows): i["00 Shift/HLT/JetMET/" + p] = DQMItem(layout=rows)

###---- BJET selection goes here: ----
def trigvalbjet(i, p, *rows): i["00 Shift/HLT/BJet/" + p] = DQMItem(layout=rows)

###---- ALCA selection goes here: ----

def trigvalalca(i, p, *rows): i["00 Shift/HLT/AlCa/" + p] = DQMItem(layout=rows)
trigvalalca(dqmitems,"Pi0 inv mass",
           [{'path': "HLT/AlCaEcalPi0/Pi0InvmassEB", 'description': "Pi0 Invariant mass EB . For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideSpecialHLTOfflinePerformance\">here</a>."}])
trigvalalca(dqmitems,"event energy eb",
            [{'path': "HLT/EcalPhiSym/eventEnergyEB", 'description': "Event energy EB . For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideSpecialHLTOfflinePerformance\">here</a>."}])



###---- HEAVYFLAVOR selection goes here: ----
def trigvalbphys(i, p, *rows): i["00 Shift/HLT/HeavyFlavor/" + p] = DQMItem(layout=rows)
trigvalbphys(dqmitems,"Quakonium efficiency",
             [{'path': "HLT/HeavyFlavor/QuarkoniumEfficiencies/genQuakonium_genPt", 'description': ". For more information please click <a href=\"\">here</a>."}])



###---- TOP selection goes here: ----
def trigvaltop(i, p, *rows): i["00 Shift/HLT/Top/" + p] = DQMItem(layout=rows)
trigvaltop(dqmitems,"Efficiency IsoEle18 vs Eta",
  	[{'path': "HLT/Top/EffEta_IsoEle18", 'description': "Trigger efficiency for IsoEle18 versus eta. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideTopHLTOfflinePerformance\">here</a>."}])



###---- SUSYEXO selection goes here: ----
def trigvalsusybsm(i, p, *rows): i["00 Shift/HLT/SusyExo/" + p] = DQMItem(layout=rows)
trigvalsusybsm(dqmitems,"HltBits",
               [{'path': "HLT/SusyExo/TriggerBits/HltBits", 'description': "HLT trigger bits. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideSUSYBSMHLTOfflinePerformance\">here</a>."}])



###---- HIGGS selection goes here: ----
def trigvalhiggs(i, p, *rows): i["00 Shift/HLT/Higgs/" + p] = DQMItem(layout=rows)
#trigvalhiggs(dqmitems,"", [{'path': "HLT/", 'description': ". For more information please click <a href=\"\">here</a>."}])


###---- QCD selection goes here: ----
def trigvalqcd(i, p, *rows): i["00 Shift/HLT/QCD/" + p] = DQMItem(layout=rows)
#trigvalqcd(dqmitems,"", [{'path': "HLT/", 'description': ". For more information please click <a href=\"\">here</a>."}])




#trigvallayout(dqmitems,"HLT Histograms",
#  	[{'path': "HLT/", 'description': "Trigger efficiency . For more information please click <a href=\"https:\">here</a>."}])






