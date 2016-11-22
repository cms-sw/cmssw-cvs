

###---- GENERIC - FourVector selection goes here: ####
######################################################

###---- GENERIC - FourVector Muon
def trigvalFVMuon(i, p, *rows): i["00 Shift/HLT/FourVector/Muon/" + p] = DQMItem(layout=rows)
  

trigvalFVMuon(dqmitems,"Eff HLT to L1",
[{'path': "HLT/FourVector/client/HLT_Mu9/custom-eff/HLT_Mu9_wrt__l1Et_Eff_OnToL1", 'description':"Efficiency of HLT to L1 for path HLT_Mu9"}])

trigvalFVMuon(dqmitems,"Eff HLT to RECO",
[{'path': "HLT/FourVector/client/HLT_Mu9/custom-eff/HLT_Mu9_wrt__offEt_Eff_OnToOff", 'description':"Efficiency of HLT to RECO for path HLT_Mu9"}])

###---- GENERIC - FourVector Electron
def trigvalFVEle(i, p, *rows): i["00 Shift/HLT/FourVector/Electron/" + p] = DQMItem(layout=rows)
  

trigvalFVEle(dqmitems,"Eff HLT to L1",
[{'path': "HLT/FourVector/client/HLT_Ele10_LW_L1R/custom-eff/HLT_Ele10_LW_L1R_wrt__l1Et_Eff_OnToL1", 'description':"Efficiency of HLT to L1 for path HLT_Ele10_LW_L1R"}])

trigvalFVEle(dqmitems,"Eff HLT to RECO",
[{'path': "HLT/FourVector/client/HLT_Ele10_LW_L1R/custom-eff/HLT_Ele10_LW_L1R_wrt__offEt_Eff_OnToOff", 'description':"Efficiency of HLT to RECO for path HLT_Ele10_LW_L1R"}])

###---- GENERIC - FourVector Jet
def trigvalFVJet(i, p, *rows): i["00 Shift/HLT/FourVector/Jet/" + p] = DQMItem(layout=rows)
  

trigvalFVJet(dqmitems,"Eff HLT to L1",
[{'path': "HLT/FourVector/client/HLT_Jet50/custom-eff/HLT_Jet50_wrt__l1Et_Eff_OnToL1", 'description':"Efficiency of HLT to L1 for path HLT_Jet50"}])

trigvalFVJet(dqmitems,"Eff HLT to RECO",
[{'path': "HLT/FourVector/client/HLT_Jet50/custom-eff/HLT_Jet50_wrt__offEt_Eff_OnToOff", 'description':"Efficiency of HLT to RECO for path HLT_Jet50"}])

###---- GENERIC - FourVector Photon
def trigvalFVPho(i, p, *rows): i["00 Shift/HLT/FourVector/Photon/" + p] = DQMItem(layout=rows)
  

trigvalFVPho(dqmitems,"Eff HLT to L1",
[{'path': "HLT/FourVector/client/HLT_Photon15_L1R/custom-eff/HLT_Photon15_L1R_wrt__l1Et_Eff_OnToL1", 'description':"Efficiency of HLT to L1 for path HLT_Photon15_L1R"}])

trigvalFVPho(dqmitems,"Eff HLT to RECO",
[{'path': "HLT/FourVector/client/HLT_Photon15_L1R/custom-eff/HLT_Photon15_L1R_wrt__offEt_Eff_OnToOff", 'description':"Efficiency of HLT to RECO for path HLT_Photon15_L1R"}])



def hltlayout(i, p, *rows): i["00 Shift/HLT/" + p] = DQMItem(layout=rows)
  
hltlayout(dqmitems,"00-HLT Shift Histogram",
  	[{'path': "HLTOffline/FourVectorHLTOfflinehltResults/HLT_L1Mu_NOn", 'description': "Multiplicity of HLT muons passing HLT_L1Mu path.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineHLT\">here</a>."}])
  
hltlayout(dqmitems,"01-HLT Shift Histogram",
  	[{'path': "HLTOffline/FourVectorHLTOfflinehltResults/HLT_L1Mu_etOn", 'description': "Et of the HLT muons. (never mind that the histogram title reads offline.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineHLT\">here</a>."}])
  
hltlayout(dqmitems,"02-HLT Shift Histogram",
  	[{'path': "HLTOffline/FourVectorHLTOfflinehltResults/HLT_L1Mu_etOff", 'description': "Et of the RECO muons matched (eta-phi) with the triggering HLT muons.   For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineHLT\">here</a>."}])
  
hltlayout(dqmitems,"04-HLT Shift Histogram",
  	[{'path': "HLTOffline/FourVectorHLTOfflinehltResults/HLT_L1Mu_etaphiOn", 'description': "X=eta and Y=phi for HLT muons triggering this path.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineHLT\">here</a>."}])
  
hltlayout(dqmitems,"05-HLT Shift Histogram",
  	[{'path': "HLTOffline/FourVectorHLTOfflinehltResults/HLT_L1Mu_etaphiOff", 'description': "X=eta and Y=phi for RECO muons that are matched (eta-phi) with HLT muons triggering this path.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineHLT\">here</a>."}])
  
hltlayout(dqmitems,"06-HLT Shift Histogram",
  	[{'path': "HLTOffline/FourVectorHLTOfflinehltResults/HLT_L1Mu_etaphiL1", 'description': "X=eta and Y=phi for L1 muons that are matched (eta-phi) with HLT muons triggering this path.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineHLT\">here</a>."}])
  
hltlayout(dqmitems,"07-HLT Shift Histogram",
  	[{'path': "HLTOffline/FourVectorHLTOfflineClient/paths/HLT_L1Mu/efficiencies/HLT_L1Mu_eta_Eff-OnToL1", 'description': "Fraction of L1 muons that triggered this HLT paht as function of eta.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineHLT\">here</a>."}])
  
hltlayout(dqmitems,"08-HLT Shift Histogram",
  	[{'path': "HLTOffline/FourVectorHLTOfflineClient/paths/HLT_L1Mu/efficiencies/HLT_L1Mu_phi_Eff-OnToL1", 'description': "Fraction of L1 muons that triggered this HLT paht as function of phi.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineHLT\">here</a>."}])
