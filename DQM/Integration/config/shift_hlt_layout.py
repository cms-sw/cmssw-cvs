def hltlayout(i, p, *rows): i["00 Shift/HLT/" + p] = DQMItem(layout=rows)
  
hltlayout(dqmitems,"00 Counts per muon HLT path",
  	[{'path': "HLT/HLTMonMuon/Summary/PassingBits_Summary", 'description': "event count per muon path. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/TriggerShiftHLTGuide\">here</a>."}])

hltlayout(dqmitems,"01 Path efficiencies wrt L1MuOpen",
  	[{'path': "HLT/HLTMonMuon/Summary/Ratio_HLT_L1MuOpen", 'description': "Muon path efficiency with respect to the L1MuOpen. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/TriggerShiftHLTGuide\">here</a>."}])

hltlayout(dqmitems,"02 HLT Muon Occupancy",
  	[{'path': "HLT/HLTMonMuon/L1PassThrough/Level1/HLTMuonL1_etaphi", 'description': "Occupancy distribution for muon candidates passing any L1Muon path. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/TriggerShiftHLTGuide\">here</a>."}])

hltlayout(dqmitems,"03 HLT Muon Eta",
  	[{'path': "HLT/HLTMonMuon/L1PassThrough/Level1/HLTMuonL1_eta", 'description': "Eta distribution for muon candidates passing any L1Muon path. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/TriggerShiftHLTGuide\">here</a>."}])

hltlayout(dqmitems,"04 HLT Muon Phi",
  	[{'path': "HLT/HLTMonMuon/L1PassThrough/Level1/HLTMuonL1_phi", 'description': "Phi distribution for muon candidates passing any L1Muon path. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/TriggerShiftHLTGuide\">here</a>."}])

