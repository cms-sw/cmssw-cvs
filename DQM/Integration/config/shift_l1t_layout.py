def l1tlayout(i, p, *rows): i["00 Shift/L1T/" + p] = DQMItem(layout=rows)
  
l1tlayout(dqmitems,"00 Global Trigger",
  	[{'path': "L1T/L1TGT/algo_bits", 'description': "Global Trigger bits. x-axis: GT algorithm number; y-axis: number of events with given bit on.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}],
	[{'path': "L1T/L1TGT/tt_bits", 'description': "Global Trigger Technical bits. x-axis: technical trigger algorithm number; y-axis: number of events with given bit on.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}])

l1tlayout(dqmitems,"01 Global Muon Trigger",
  	[{'path': "L1T/L1TGMT/Regional_trigger", 'description': "Regional Muon Trigger Contribution. x-axis: muon regional trigger; y-axis: number of triggers from given subsystem.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}],
	[{'path': "L1T/L1TGMT/GMT_etaphi", 'description': "GMT Phi vs Eta. x-axis: phi in degrees; y-axis: eta; z-axis: number of GMT candidates in given phi/eta bin.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}])

l1tlayout(dqmitems,"02 Global Calorimeter Trigger",
  	[{'path': "L1T/L1TGCT/IsoEmRankEtaPhi", 'description': "(Eta, Phi) map of Isolated Electron Occupancy. x-axis: phi (0-17) y-axis: eta (0-21) z-axis: number of isolated electron candidates. Electrons are not found in HF so eta bins 0-3 and 18-21 should be empty. Currently both ECAL and HCAL use the non-isolated electron path so this histogram should be completely empty. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."},
	{'path': "L1T/L1TGCT/NonIsoEmRankEtaPhi", 'description': "(Eta,Phi) map of Non Isolated Electron Occupancy. x-axis: phi (0-17) y-axis: eta (0-21) z-axis: number of non-isolated electron candidates. Electrons are not found in HF so eta bins 0-3 and 18-21 should be empty.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}],
  	[{'path': "L1T/L1TGCT/AllJetsEtEtaPhi", 'description': "(Eta,Phi) map of Central Jet Occupancy. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."},
	{'path': "L1T/L1TGCT/TauJetsEtEtaPhi", 'description': "(Eta,Phi) map of Forward Jet Occupancy. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}])
