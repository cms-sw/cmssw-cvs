def shifteblayout(i, p, *rows): i["00 Shift/EcalBarrel/" + p] = DQMItem(layout=rows)

shifteblayout(dqmitems, "00 Event Type",
  [{ 'path': "EcalBarrel/EcalInfo/EVTTYPE", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

shifteblayout(dqmitems, "01 Integrity Summary",
  [{ 'path': "EcalBarrel/EBSummaryClient/EBIT integrity quality summary", 'description': "Integrity quality summary. Expected all green color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

shifteblayout(dqmitems, "02 PedestalOnline RMS",
  [{ 'path': "EcalBarrel/EBSummaryClient/EBPOT pedestal G12 RMS map", 'description': "RMS of the pedestals in ADC counts. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (calibration and physics). Expected RMS for ECAL barrel is 1.1 ADC counts (43 MeV). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

shifteblayout(dqmitems, "03 Occupancy Rechits",
  [{ 'path': "EcalBarrel/EBOccupancyTask/EBOT rec hit occupancy", 'description': "Map of the occupancy of ECAL calibrated reconstructed hits. Expect uniform color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "EcalBarrel/EBOccupancyTask/EBOT rec hit occupancy projection eta", 'description': "Eta projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "EcalBarrel/EBOccupancyTask/EBOT rec hit occupancy projection phi", 'description': "Phi projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

shifteblayout(dqmitems, "04 Occupancy Trigger Primitives",
  [{ 'path': "EcalBarrel/EBOccupancyTask/EBOT TP digi thr occupancy", 'description': "Map of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). Expect uniform color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "EcalBarrel/EBOccupancyTask/EBOT TP digi thr occupancy projection eta", 'description': "Eta projection of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "EcalBarrel/EBOccupancyTask/EBOT TP digi thr occupancy projection phi", 'description': "Phi projection of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

shifteblayout(dqmitems, "05 Clusters Energy",
  [{ 'path': "EcalBarrel/EBClusterTask/EBCLT BC energy map", 'description': "Average energy (in GeV) of hybrid basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "EcalBarrel/EBClusterTask/EBCLT BC energy projection eta", 'description': "Eta projection of hybrid basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "EcalBarrel/EBClusterTask/EBCLT BC energy projection phi", 'description': "Phi projection of hybrid basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

shifteblayout(dqmitems, "06 LaserL1 Quality",
  [{ 'path': "EcalBarrel/EBSummaryClient/EBLT laser quality summary L1", 'description': "Quality summary of laser events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

shifteblayout(dqmitems, "08 Pedestal Gain12 Quality",
  [{ 'path': "EcalBarrel/EBSummaryClient/EBPT pedestal quality G12 summary", 'description': "Quality summary of pedestal events for Gain 12. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

# shifteblayout(dqmitems, "09 DCC EventSize",
#   [{ 'path': "EcalBarrel/EBSelectiveReadoutTask/EBSRT DCC event size", 'description': "Average event size per ecah ECAL barrel DCC. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

# shifteblayout(dqmitems, "10 Selective Readout High Interest TTs",
#   [{ 'path': "EcalBarrel/EBSelectiveReadoutTask/EBSRT high interest TT Flags", 'description': "Occupancy of high interest (energy of Trigger Tower over threshold) flags of selective readout. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

