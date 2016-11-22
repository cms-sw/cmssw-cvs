def shifteelayout(i, p, *rows): i["00 Shift/EcalEndcap/" + p] = DQMItem(layout=rows)

shifteelayout(dqmitems, "00 Event Type",
  [{ 'path': "EcalEndcap/EcalInfo/EVTTYPE", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

shifteelayout(dqmitems, "01 Integrity Summary",
  [{ 'path': "EcalEndcap/EESummaryClient/EEIT EE - integrity quality summary", 'description': "Integrity quality summary. Expected all green color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "EcalEndcap/EESummaryClient/EEIT EE + integrity quality summary", 'description': "Integrity quality summary. Expected all green color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

shifteelayout(dqmitems, "02 PedestalOnline RMS",
  [{ 'path': "EcalEndcap/EESummaryClient/EEPOT EE - pedestal G12 RMS map", 'description': "RMS of the pedestals in ADC counts. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (calibration and physics). Expected RMS for ECAL endcap is 1.9 ADC counts (120 MeV). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "EcalEndcap/EESummaryClient/EEPOT EE + pedestal G12 RMS map", 'description': "RMS of the pedestals in ADC counts. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (calibration and physics). Expected RMS for ECAL endcap is 1.9 ADC counts (120 MeV). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

shifteelayout(dqmitems, "03 Occupancy Rechits EE -",
  [{ 'path': "EcalEndcap/EEOccupancyTask/EEOT rec hit occupancy EE -", 'description': "Map of the occupancy of ECAL calibrated reconstructed hits. Expect uniform color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "EcalEndcap/EEOccupancyTask/EEOT rec hit occupancy EE - projection R", 'description': "R projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "EcalEndcap/EEOccupancyTask/EEOT rec hit occupancy EE - projection phi", 'description': "Phi projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

shifteelayout(dqmitems, "03 Occupancy Rechits EE +",
  [{ 'path': "EcalEndcap/EEOccupancyTask/EEOT rec hit occupancy EE +", 'description': "Map of the occupancy of ECAL calibrated reconstructed hits. Expect uniform color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "EcalEndcap/EEOccupancyTask/EEOT rec hit occupancy EE + projection R", 'description': "R projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "EcalEndcap/EEOccupancyTask/EEOT rec hit occupancy EE + projection phi", 'description': "Phi projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

shifteelayout(dqmitems, "04 Occupancy Trigger Primitives EE -",
  [{ 'path': "EcalEndcap/EEOccupancyTask/EEOT TP digi thr occupancy EE -", 'description': "Map of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). Expect uniform color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "EcalEndcap/EEOccupancyTask/EEOT TP digi thr occupancy EE - projection R", 'description': "R projection of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "EcalEndcap/EEOccupancyTask/EEOT TP digi thr occupancy EE - projection phi", 'description': "Phi projection of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

shifteelayout(dqmitems, "04 Occupancy Trigger Primitives EE +",
  [{ 'path': "EcalEndcap/EEOccupancyTask/EEOT TP digi thr occupancy EE +", 'description': "Map of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). Expect uniform color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "EcalEndcap/EEOccupancyTask/EEOT TP digi thr occupancy EE + projection R", 'description': "R projection of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "EcalEndcap/EEOccupancyTask/EEOT TP digi thr occupancy EE + projection phi", 'description': "Phi projection of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

shifteelayout(dqmitems, "05 Clusters Energy EE -",
  [{ 'path': "EcalEndcap/EEClusterTask/EECLT BC energy map EE -", 'description': "Average energy (in GeV) of 5x5 basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "EcalEndcap/EEClusterTask/EECLT BC energy projection R EE -", 'description': "R projection of 5x5 basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "EcalEndcap/EEClusterTask/EECLT BC energy projection phi EE -", 'description': "phi projection of 5x5 basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

shifteelayout(dqmitems, "05 Clusters Energy EE +",
  [{ 'path': "EcalEndcap/EEClusterTask/EECLT BC energy map EE +", 'description': "Average energy (in GeV) of 5x5 basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "EcalEndcap/EEClusterTask/EECLT BC energy projection R EE +", 'description': "R projection of 5x5 basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "EcalEndcap/EEClusterTask/EECLT BC energy projection phi EE +", 'description': "phi projection of 5x5 basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

shifteelayout(dqmitems, "06 LaserL1 Quality",
  [{ 'path': "EcalEndcap/EESummaryClient/EELT EE - laser quality summary L1", 'description': "Quality summary of laser events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "EcalEndcap/EESummaryClient/EELT EE + laser quality summary L1", 'description': "Quality summary of laser events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

shifteelayout(dqmitems, "07 LedL1 Quality", 
  [{ 'path': "EcalEndcap/EESummaryClient/EELDT EE - led quality summary L1", 'description': "Quality summary of led events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }], 
  [{ 'path': "EcalEndcap/EESummaryClient/EELDT EE + led quality summary L1", 'description': "Quality summary of led events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

shifteelayout(dqmitems, "08 Pedestal Gain12 Quality",
  [{ 'path': "EcalEndcap/EESummaryClient/EEPT EE - pedestal quality G12 summary", 'description': "Quality summary of pedestal events for Gain 12. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "EcalEndcap/EESummaryClient/EEPT EE + pedestal quality G12 summary", 'description': "Quality summary of pedestal events for Gain 12. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

# shifteelayout(dqmitems, "09 DCC EventSize",
#   [{ 'path': "EcalEndcap/EESelectiveReadoutTask/EESRT DCC event size", 'description': "Average sector event size (kB). Expected below 100 kB/event. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }],
#   [None])

# shifteelayout(dqmitems, "10 Selective Readout High Interest TTs",
#   [{ 'path': "EcalEndcap/EESelectiveReadoutTask/EESRT high interest TT Flags EE -", 'description': "Occupancy of high interest (energy of Trigger Tower over threshold) flags of selective readout. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
#   [{ 'path': "EcalEndcap/EESelectiveReadoutTask/EESRT high interest TT Flags EE +", 'description': "Occupancy of high interest (energy of Trigger Tower over threshold) flags of selective readout. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

