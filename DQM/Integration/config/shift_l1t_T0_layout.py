def l1tlayout(i, p, *rows): i["00 Shift/L1T/" + p] = DQMItem(layout=rows)
  
l1tlayout(dqmitems,"00 Global Trigger",
  	[{'path': "L1T/L1TGT/algo_bits", 'description': "Global Trigger bits. x-axis: GT algorithm number; y-axis: number of events with given bit on.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}],
	[{'path': "L1T/L1TGT/event_lumi", 'description': "GT Event Number vs. Luminosity Section. x-axis: lumionosity segment of a given run; y-axis: average event number for that lumi segment.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."},
	{'path': "L1T/L1TGT/tt_bits", 'description': "Global Trigger Technical bits. x-axis: technical trigger algorithm number; y-axis: number of events with given bit on.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}])

l1tlayout(dqmitems,"01 Global Muon Trigger",
  	[{'path': "L1T/L1TGMT/Regional_trigger", 'description': "Regional Muon Trigger Contribution. x-axis: muon regional trigger; y-axis: number of triggers from given subsystem.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."},
	{'path': "L1T/L1TGMT/GMT_candlumi", 'description': "GMT Number of Candidates vs Luminosity Section. x-axis: lumionosity segment of a given run; y-axis: number of GMT candidates collected in given lumi segment.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}],
	[{'path': "L1T/L1TGMT/GMT_phi", 'description': "GMT Phi value. x-axis: phi in degrees; y-axis: number of GMT candidates in given phi bin.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."},
	{'path': "L1T/L1TGMT/GMT_etaphi", 'description': "GMT Phi vs Eta. x-axis: phi in degrees; y-axis: eta; z-axis: number of GMT candidates in given phi/eta bin.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}])

l1tlayout(dqmitems,"02 Global Calorimeter Trigger",
  	[{'path': "L1T/L1TGCT/IsoEmOccEtaPhi", 'description': "(Eta, Phi) map of Isolated Electron Occupancy. x-axis: phi (0-17) y-axis: eta (0-21) z-axis: number of isolated electron candidates. Electrons are not found in HF so eta bins 0-3 and 18-21 should be empty. Currently both ECAL and HCAL use the non-isolated electron path so this histogram should be completely empty. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."},
	{'path': "L1T/L1TGCT/NonIsoEmOccEtaPhi", 'description': "(Eta,Phi) map of Non Isolated Electron Occupancy. x-axis: phi (0-17) y-axis: eta (0-21) z-axis: number of non-isolated electron candidates. Electrons are not found in HF so eta bins 0-3 and 18-21 should be empty.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}],
  	[{'path': "L1T/L1TGCT/CenJetsOccEtaPhi", 'description': "(Eta,Phi) map of Central Jet Occupancy. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."},
	{'path': "L1T/L1TGCT/ForJetsOccEtaPhi", 'description': "(Eta,Phi) map of Forward Jet Occupancy. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}])

l1tlayout(dqmitems,"03 Regional Calorimeter Trigger",
  	[{'path': "L1T/L1TRCT/RctEmIsoEmOccEtaPhi", 'description': "Isolated Electron Candidates occupancy plot. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."},
	{'path': "L1T/L1TRCT/RctEmNonIsoEmOccEtaPhi", 'description': "Non Isolated Electron Candidates occupancy plot. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}],
	[{'path': "L1T/L1TRCT/RctRegionsOccEtaPhi", 'description': "Regional Candidates occupancy plot. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}])

l1tlayout(dqmitems,"04 DT Track Finder",
  	[{'path': "L1T/L1TDTTF/DTTF_TRACKS/INTEG/Occupancy Summary", 'description': "DTTF occupancy plot (divided by the total number of events with a DTTF track) for each sector of each logical wheel. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."},
  	{'path': "L1T/L1TDTTF/DTTF_TRACKS/INTEG/BX Summary", 'description': "DTTF BX plot (proportionate to total number of events) for each sector of each logical wheel. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}],
  	[{'path': "L1T/L1TDTTF/DTTF_TRACKS/INTEG/2nd Track Summary", 'description': "DTTF plot of fraction of events with 2nd track. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."},
  	{'path': "L1T/L1TDTTF/DTTF_TRACKS/INTEG/Fractional High Quality Summary", 'description': "DTTF high quality (quality>3) plot. For each bin, the fraction of high quality tracks to the total bin occupancy is calculated.  For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}])

l1tlayout(dqmitems,"05 CSC Track Finder",
  	[{'path': "L1T/L1TCSCTF/CSCTF_occupancies", 'description': "CSCTF occupancy plot. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}],
  	[{'path': "L1T/L1TCSCTF/CSCTF_Chamber_Occupancies", 'description': "Temperature plot of Chamber Occupancies as reported by the CSCTF. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}])

l1tlayout(dqmitems,"06 RPC",
  	[{'path': "L1T/L1TRPCTF/RPCTF_muons_tower_phipacked", 'description': "Occupancy plot. Tower is proportional to eta (standard trigger scale). Phi uses trigger phi scale (0...143). HotChannels and DeadChannels quality test are performed on this histo. Should be ~flat in each tower. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."},
	{'path': "L1T/L1TRPCTF/RPCTF_ntrack", 'description': "Number of muon candidates found per event. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}],
	[{'path': "L1T/L1TRPCTF/RPCTF_pt_value", 'description': "Pt value of muon candidates. During CRUZET it should be equal to 140 (due to patterns used). For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."},
	{'path': "L1T/L1TRPCTF/RPCTF_bx", 'description': "Tells if delays are properly set on our FEDs (bin 0 of the histogram should be way above other bins). For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}])

l1tlayout(dqmitems,"07 Timing",
  	[{'path': "L1T/BXSynch/BxDiffAllFed", 'description': "BX shift wrt reference fed (GT), for all feds. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}])

l1tlayout(dqmitems,"08 L1 Bits (from EvF)",
  	[{'path': "L1T/L1Scalers_EvF/bxNum", 'description': "GT Bunch Crossing Occupancy. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."},
	{'path': "L1T/L1Scalers_EvF/l1AlgoBits_Vs_Bx", 'description': "Bunch Crossing Occupancy Vs GT Algo Bit. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}],
	[{'path': "L1T/L1Scalers_EvF/l1TechAlgoBits_Vs_Bx", 'description': "Bunch Crossing Occupancy Vs GT Tech. Bit. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."},
	{'path': "L1T/L1Scalers_EvF/l1Correlations", 'description': "GT Algo bis correlation. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}])

l1tlayout(dqmitems,"09 L1 Rates (based on EvF products)",
  	[{'path': "L1T/L1Scalers_EvF/algo_cur_rate", 'description': "Lumi Section Rate per Algo Bit. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}],
	[{'path': "L1T/L1Scalers_EvF/tt_cur_rate", 'description': "Lumi Section Rate per Technical Bit. For more information please click <a href=\"https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftTrigger\">here</a>."}])
