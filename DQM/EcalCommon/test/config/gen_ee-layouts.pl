#!/usr/bin/env perl

$prefix = "EcalEndcap";

@Lasers = ("1","2","3","4");
@Leds = ("1","2");
@MGPAGains = ("01","06","12");
@MGPAGainsPN = ("01","16");

open LAYOUT, ">ee-layouts.py";

print LAYOUT <<EOF00;
def ecalendcaplayout(i, p, *rows): i["$prefix/Layouts/" + p] = DQMItem(layout=rows)
def eeshifterlayout(i, p, *rows): i["$prefix/Layouts/00 Shift/" + p] = DQMItem(layout=rows)
def eeecalshifterlayout(i, p, *rows): i["$prefix/Layouts/01 ECAL Shift/" + p] = DQMItem(layout=rows)
def eelayout(i, p, *rows): i["$prefix/Layouts/02 ECAL Expert Shift/" + p] = DQMItem(layout=rows)

# Quick Collections
ecalendcaplayout(dqmitems, "00 Global Summary EcalEndcap",
  [{ 'path': "$prefix/EESummaryClient/EE global summary EE -", 'description': "EcalEndcap (z<0): overall status of crystals.  Legend: green = good;  red = bad;  yellow = no entries.  <a href=https://twiki.cern.ch/twiki/bin/view/CMS/EcalDQM>EcalDQM</a>" },
   { 'path': "$prefix/EESummaryClient/EE global summary EE +", 'description': "EcalEndcap (z>0): overall status of crystals.  Legend: green = good;  red = bad;  yellow = no entries.  <a href=https://twiki.cern.ch/twiki/bin/view/CMS/EcalDQM>EcalDQM</a>" }])

ecalendcaplayout(dqmitems, "01 Occupancy Summary EcalEndcap",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit thr occupancy EE -", 'description': "EcalEndcap (z<0): occupancy of calibrated hits with energy > 1 GeV. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/EcalDQM>EcalDQM</a>" },
   { 'path': "$prefix/EEOccupancyTask/EEOT rec hit thr occupancy EE +", 'description': "EcalEndcap (z>0): occupancy of calibrated hits with energy > 1 GeV. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/EcalDQM>EcalDQM</a>" }])

ecalendcaplayout(dqmitems, "02 Cluster Summary EcalEndcap",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy map EE -", 'description': "EcalEndcap (z<0): average energy of 5x5 basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/EcalDQM>EcalDQM</a>" },
   { 'path': "$prefix/EEClusterTask/EECLT BC energy map EE +", 'description': "EcalEndcap (z>0): average energy of hybrid basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/EcalDQM>EcalDQM</a>" }])

# ALL shifters
eeshifterlayout(dqmitems, "00 Event Type",
  [{ 'path': "$prefix/EcalInfo/EVTTYPE", 'description': "Frequency of the event types found in the DQM event-stream. If the calibration sequence is ON, histograms should show entries in COSMICS_GLOBAL, LASER_GAP, PEDESTAL_GAP, TESTPULSE_GAP. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

eeshifterlayout(dqmitems, "01 Integrity Summary",
  [{ 'path': "$prefix/EESummaryClient/EEIT EE - integrity quality summary", 'description': "Quality summary checking that data for each crystal follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EEIT EE + integrity quality summary", 'description': "Quality summary checking that data for each crystal follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

eeshifterlayout(dqmitems, "02 StatusFlags Summary",
  [{ 'path': "$prefix/EESummaryClient/EESFT EE - front-end status summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EESFT EE + front-end status summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

eeshifterlayout(dqmitems, "03 PedestalOnline RMS",
  [{ 'path': "$prefix/EESummaryClient/EEPOT EE - pedestal G12 RMS map", 'description': "RMS of the pedestals in ADC counts. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (calibration and physics). Expected RMS for ECAL endcap is 1.9 ADC counts (120 MeV): almost blue color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EEPOT EE + pedestal G12 RMS map", 'description': "RMS of the pedestals in ADC counts. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (calibration and physics). Expected RMS for ECAL endcap is 1.9 ADC counts (120 MeV): almost blue color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

eeshifterlayout(dqmitems, "04 Timing Summary",
  [{ 'path': "$prefix/EESummaryClient/EETMT EE - timing quality summary", 'description': "Quality summary of the crystal reconstructed hits in EE - with amplitude above 16 ADC counts. Hardware timing is adjusted with readout tower granularity, but finer setting can be reached. Expected all green color. Legend: green = good;  red = bad;  yellow = no sufficient statistics.  <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EETMT EE + timing quality summary", 'description': "Quality summary of the crystal reconstructed hits in EE + with amplitude above 16 ADC counts. Hardware timing is adjusted with readout tower granularity, but finer setting can be reached. Expected all green color. Legend: green = good;  red = bad;  yellow = no sufficient statistics.  <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

eeshifterlayout(dqmitems, "05 Occupancy Rechits EE -",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE -", 'description': "Map of the occupancy of ECAL calibrated reconstructed hits. Expect uniform color. Legend: the darkness of the blue scale is proportional to the average occupancy of the crystal. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE - projection eta", 'description': "Eta projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE - projection phi", 'description': "Phi projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

eeshifterlayout(dqmitems, "05 Occupancy Rechits EE +",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE +", 'description': "Map of the occupancy of ECAL calibrated reconstructed hits. Expect uniform color. Legend: the darkness of the blue scale is proportional to the average occupancy of the crystal. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE + projection eta", 'description': "Eta projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE + projection phi", 'description': "Phi projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

eeshifterlayout(dqmitems, "06 Occupancy Trigger Primitives EE -",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE -", 'description': "Map of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). Expect uniform color. Legend: the darkness of the blue scale is proportional to the average occupancy of the trigger tower. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE - projection eta", 'description': "Eta projection of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE - projection phi", 'description': "Phi projection of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

eeshifterlayout(dqmitems, "06 Occupancy Trigger Primitives EE +",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE +", 'description': "Map of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). Expect uniform color. Legend: the darkness of the blue scale is proportional to the average occupancy of the trigger tower. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE + projection eta", 'description': "Eta projection of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE + projection phi", 'description': "Phi projection of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

eeshifterlayout(dqmitems, "07 Clusters Energy EE -",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy map EE -", 'description': "Average energy (in GeV) of 5x5 basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy projection eta EE -", 'description': "Eta projection of 5x5 basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC energy projection phi EE -", 'description': "Phi projection of 5x5 basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

eeshifterlayout(dqmitems, "07 Clusters Energy EE +",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy map EE +", 'description': "Average energy (in GeV) of 5x5 basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy projection eta EE +", 'description': "Eta projection of 5x5 basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC energy projection phi EE +", 'description': "Phi projection of 5x5 basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

eeshifterlayout(dqmitems, "08 LaserL1 Quality",
  [{ 'path': "$prefix/EESummaryClient/EELT EE - laser quality summary L1", 'description': "Quality summary of blue laser events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. Sectors are filled as the calibration sequence reach them: expected all yellow at beginning of run, then becoming green sector by sector. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EELT EE + laser quality summary L1", 'description': "Quality summary of blue laser events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. Sectors are filled as the calibration sequence reach them: expected all yellow at beginning of run, then becoming green sector by sector. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

eeshifterlayout(dqmitems, "09 LedL1 Quality", 
  [{ 'path': "$prefix/EESummaryClient/EELDT EE - led quality summary L1", 'description': "Quality summary of led events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. Sectors are filled as the calibration sequence reach them: expected all yellow at beginning of run, then becoming green sector by sector. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }], 
  [{ 'path': "$prefix/EESummaryClient/EELDT EE + led quality summary L1", 'description': "Quality summary of led events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. Sectors are filled as the calibration sequence reach them: expected all yellow at beginning of run, then becoming green sector by sector. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

eeshifterlayout(dqmitems, "10 Pedestal Gain12 Quality",
  [{ 'path': "$prefix/EESummaryClient/EEPT EE - pedestal quality G12 summary", 'description': "Quality summary of pedestal events for Gain 12. Pedestals are evaluated using all the 10 samples. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. Sectors are filled as the calibration sequence reach them: expected all yellow at beginning of run, then becoming green sector by sector. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EEPT EE + pedestal quality G12 summary", 'description': "Quality summary of pedestal events for Gain 12. Pedestals are evaluated using all the 10 samples. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. Sectors are filled as the calibration sequence reach them: expected all yellow at beginning of run, then becoming green sector by sector. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

eeshifterlayout(dqmitems, "11 TestPulse Gain12 Quality",
  [{ 'path': "$prefix/EESummaryClient/EETPT EE - test pulse quality G12 summary", 'description': "Quality summary of test pulse events for Gain 12. Expect green where the calibration sequence fired, yellow elsewhere. Red spots are failed channels. Sectors are filled as the calibration sequence reach them: expected all yellow at beginning of run, then becoming green sector by sector. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EETPT EE + test pulse quality G12 summary", 'description': "Quality summary of test pulse events for Gain 12. Expect green where the calibration sequence fired, yellow elsewhere. Red spots are failed channels. Sectors are filled as the calibration sequence reach them: expected all yellow at beginning of run, then becoming green sector by sector. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

eeshifterlayout(dqmitems, "12 DCC EventSize",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT DCC event size", 'description': "Average event size per ecah ECAL barrel DCC calculated only on physics events. Expected about 2 kB/DCC. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }],
  [None])

eeshifterlayout(dqmitems, "13 Selective Readout High Interest TTs",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT high interest TT Flags EE -", 'description': "Rate of high interest (E_T of Trigger Tower over threshold) flags of selective readout.  <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT high interest TT Flags EE +", 'description': "Rate of high interest (E_T of Trigger Tower over threshold) flags of selective readout. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])


# ECAL shifters
eeecalshifterlayout(dqmitems, "00 Event Type",
  [{ 'path': "$prefix/EcalInfo/EVTTYPE", 'description': "Frequency of the event types found in the DQM event-stream. If the calibration sequence is ON, histograms should show entries in COSMICS_GLOBAL, LASER_GAP, PEDESTAL_GAP, TESTPULSE_GAP. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "01 Electronics/00 Integrity Summary",
  [{ 'path': "$prefix/EESummaryClient/EEIT EE - integrity quality summary", 'description': "Quality summary checking that data for each crystal follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EEIT EE + integrity quality summary", 'description': "Quality summary checking that data for each crystal follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])
    
eeecalshifterlayout(dqmitems, "01 Electronics/01 Integrity Errors",
  [{ 'path': "$prefix/EESummaryClient/EEIT integrity quality errors summary", 'description': "Crystals integrity error counter by sector. Expect 0 entries in every bin. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "01 Electronics/02 PN Integrity Summary",
  [{ 'path': "$prefix/EESummaryClient/EEIT PN integrity quality summary", 'description': "Quality summary checking that data for each PN diode follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries; white means that no MEM is attached to that DCC (by the readout point of view). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eeecalshifterlayout(dqmitems, "01 Electronics/03 FrontEnd Summary",
  [{ 'path': "$prefix/EESummaryClient/EESFT EE - front-end status summary", 'description': "DCC front-end status quality summary. Expected all green color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EESFT EE + front-end status summary", 'description': "DCC front-end status quality summary. Expected all green color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "01 Electronics/04 CalibrationSequence Errors",
  [{ 'path': "$prefix/EERawDataTask/EERDT trigger type errors", 'description': "Number of trigger type mismatches between the global trigger and ECAL DCC header. It is filled once per DCC with weight 1/36. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "02 Noise/00 PedestalOnline Quality",
  [{ 'path': "$prefix/EESummaryClient/EEPOT EE - pedestal quality summary G12", 'description': "Pedestal quality summary. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (on physics events only). Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EEPOT EE + pedestal quality summary G12", 'description': "pedestal quality summary. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (on physics events only). Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "02 Noise/01 PedestalOnline Rms",
  [{ 'path': "$prefix/EESummaryClient/EEPOT EE - pedestal G12 RMS map", 'description': "RMS of the pedestals in ADC counts. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (on physics events only). Expected RMS for ECAL endcap is 1.9 ADC counts (120 MeV): almost blue color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EEPOT EE + pedestal G12 RMS map", 'description': "RMS of the pedestals in ADC counts. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (on physics events only). Expected RMS for ECAL endcap is 1.9 ADC counts (120 MeV): almost blue color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "02 Noise/02 PedestalOnline FEDAverage",
  [{ 'path': "$prefix/EESummaryClient/EEPOT pedestal G12 mean", 'description': "Average mean of the pedestals in ADC counts for each FED. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (calibration and physics). Expected mean is 200 ADC counts.<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EESummaryClient/EEPOT pedestal G12 rms", 'description': "Average RMS of the pedestals in ADC counts for each FED. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (calibration and physics). Expected mean is 200 ADC counts.<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "03 Occupancy/00 Summary",
  [{ 'path': "$prefix/EcalInfo/EEMM DCC", 'description': "Occupancy of DCC. Non-uniform distribution in one bin means that the sector has not been readout in some event. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EcalInfo/EEMM digi number profile", 'description': "Average occupancy of digis per sector. Expect flat distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EcalInfo/EEMM hit number profile", 'description': "Average occupancy of reconstructed hits per sector. Expect flat distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EcalInfo/EEMM TP digi number profile", 'description': "Average occupancy of trigger primitives per sector. Expect flat distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "03 Occupancy/01 Digis EE -",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT digi occupancy EE -", 'description': "Map of the occupancy of ECAL digis. Expect uniform color. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT digi occupancy EE - projection eta", 'description': "Eta projection of the occupancy of ECAL digis. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT digi occupancy EE - projection phi", 'description': "Phi projection of the occupancy of ECAL digis. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "03 Occupancy/01 Digis EE +",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT digi occupancy EE +", 'description': "Map of the occupancy of ECAL digis. Expect uniform color. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT digi occupancy EE + projection eta", 'description': "Eta projection of the occupancy of ECAL digis. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT digi occupancy EE + projection phi", 'description': "Phi projection of the occupancy of ECAL digis. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "03 Occupancy/02 Rec Hits (Unfiltered) EE -",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE -", 'description': "Map of the occupancy of ECAL calibrated reconstructed hits. Expect uniform color. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE - projection eta", 'description': "Eta projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE - projection phi", 'description': "Phi projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "03 Occupancy/02 Rec Hits (Unfiltered) EE +",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE +", 'description': "Map of the occupancy of ECAL calibrated reconstructed hits. Expect uniform color. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE + projection eta", 'description': "Eta projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE + projection phi", 'description': "Phi projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "03 Occupancy/03 Rec Hits (Filtered) EE -",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit thr occupancy EE -", 'description': "Map of the occupancy of ECAL calibrated reconstructed hits. Only the good reconstructed hits with energy E > 500 MeV are considered. Expect uniform color. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit thr occupancy EE - projection eta", 'description': "Eta projection of the occupancy of ECAL calibrated reconstructed hits. Only the good reconstructed hits with energy E > 500 MeV are considered. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT rec hit thr occupancy EE - projection phi", 'description': "Phi projection of the occupancy of ECAL calibrated reconstructed hits. Only the good reconstructed hits with energy E > 500 MeV are considered. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "03 Occupancy/03 Rec Hits (Filtered) EE +",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit thr occupancy EE +", 'description': "Map of the occupancy of ECAL calibrated reconstructed hits. Only the good reconstructed hits with energy E > 500 MeV are considered. Expect uniform color. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit thr occupancy EE + projection eta", 'description': "Eta projection of the occupancy of ECAL calibrated reconstructed hits. Only the good reconstructed hits with energy E > 500 MeV are considered. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT rec hit thr occupancy EE + projection phi", 'description': "Phi projection of the occupancy of ECAL calibrated reconstructed hits. Only the good reconstructed hits with energy E > 500 MeV are considered. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "03 Occupancy/04 Trigger Primitives Digis (All) EE -",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi occupancy EE -", 'description': "Map of the occupancy of ECAL trigger primitives. Expect uniform color. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi occupancy EE - projection eta", 'description': "Eta projection of the occupancy of ECAL trigger primitives. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT TP digi occupancy EE - projection phi", 'description': "Phi projection of the occupancy of ECAL trigger primitives. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "03 Occupancy/04 Trigger Primitives Digis (All) EE +",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi occupancy EE +", 'description': "Map of the occupancy of ECAL trigger primitives. Expect uniform color. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi occupancy EE + projection eta", 'description': "Eta projection of the occupancy of ECAL trigger primitives. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT TP digi occupancy EE + projection phi", 'description': "Phi projection of the occupancy of ECAL trigger primitives. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "03 Occupancy/05 Trigger Primitives (Threshold) EE -",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE -", 'description': "Map of the occupancy of ECAL trigger primitives with an E_T > 1 GeV (4 ADC counts). Darker regions mean noisy towers. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE - projection eta", 'description': "Eta projection of the occupancy of ECAL trigger primitives with an E_T > 1 GeV (4 ADC counts). Expect uniform distribution, spikes mean noisy towers. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE - projection phi", 'description': "Phi projection of the occupancy of ECAL trigger primitives with an E_T > 1 GeV (4 ADC counts). Expect uniform distribution, spikes mean noisy towers. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "03 Occupancy/05 Trigger Primitives (Threshold) EE +",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE +", 'description': "Map of the occupancy of ECAL trigger primitives with an E_T > 1 GeV (4 ADC counts). Darker regions mean noisy towers. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE + projection eta", 'description': "Eta projection of the occupancy of ECAL trigger primitives with an E_T > 1 GeV (4 ADC counts). Expect uniform distribution, spikes mean noisy towers. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE + projection phi", 'description': "Phi projection of the occupancy of ECAL trigger primitives with an E_T > 1 GeV (4 ADC counts). Expect uniform distribution, spikes mean noisy towers. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "03 Occupancy/06 TestPulseEvents",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT test pulse digi occupancy EE -", 'description': "Occupancy of test pulse events only (if any in the calibration sequence). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT test pulse digi occupancy EE +", 'description': "Occupancy of test pulse events only (if any in the calibration sequence). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "03 Occupancy/07 LaserEvents",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT laser digi occupancy EE -", 'description': "Occupancy of laser events only (if any in the calibration sequence). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT laser digi occupancy EE +", 'description': "Occupancy of laser events only (if any in the calibration sequence). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "03 Occupancy/08 LedEvents",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT led digi occupancy EE -", 'description': "Occupancy of led events only (if any in the calibration sequence). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT led digi occupancy EE +", 'description': "Occupancy of led events only (if any in the calibration sequence). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "03 Occupancy/09 PedestalEvents",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT pedestal digi occupancy EE -", 'description': "Occupancy of pedestal events only (if any in the calibration sequence). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT pedestal digi occupancy EE +", 'description': "Occupancy of pedestal events only (if any in the calibration sequence). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "03 Occupancy/10 PN Digis",
  [{ 'path': "$prefix/EESummaryClient/EEOT PN digi occupancy summary", 'description': "Occupancy of PN diodes (expect entries only for calibration events). Digis are expected only in EE+/-2,3,7,8 which have MEM box attached (by the readout point of view). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

# to be put in when we will remove cosmics plots
eeecalshifterlayout(dqmitems, "04 Energy/00 RecHit Energy",
  [{ 'path': "$prefix/EESummaryClient/EEOT EE - energy summary", 'description': "Average energy of calibrated rechit with a cosmic selection. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EEOT EE + energy summary", 'description': "Average energy of calibrated rechit with a cosmic selection. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])
#   [{ 'path': "$prefix/EESummaryClient/EECT EE - cosmic summary", 'description': "Average energy of calibrated rechit with a cosmic selection. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
#   [{ 'path': "$prefix/EESummaryClient/EECT EE + cosmic summary", 'description': "Average energy of calibrated rechit with a cosmic selection. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "04 Energy/00 RecHit Energy Spectrum",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit spectrum EE -", 'description': "Average energy of calibrated rechit with a cosmic selection. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit spectrum EE +", 'description': "Average energy of calibrated rechit with a cosmic selection. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "04 Energy/01 Clusters Energy",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy", 'description': "Calibrated energy (GeV) of 5x5 basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EEClusterTask/EECLT SC seed crystal energy", 'description': "Energy (GeV) of the crystal seeding a cluster. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "04 Energy/02 Basic Clusters",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC number", 'description': "Multiplicity of reconstructed hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC size", 'description': "Numbers of crystals contained in hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "04 Energy/03 Basic Clusters Occupancy EE -",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC number map EE -", 'description': "Average number of basic cluster / event. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC number projection eta EE -", 'description': "Average number of basic cluster / event. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC number projection phi EE -", 'description': "Average number of basic cluster / event. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "04 Energy/03 Basic Clusters Occupancy EE +",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC number map EE +", 'description': "Average number of basic cluster / event. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC number projection eta EE +", 'description': "Average number of basic cluster / event. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC number projection phi EE +", 'description': "Average number of basic cluster / event. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "04 Energy/04 Basic Clusters Energy EE -",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy map EE -", 'description': "Average energy (in GeV) of 5x5 basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy projection eta EE -", 'description': "Eta projection of 5x5 basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC energy projection phi EE -", 'description': "Phi projection of 5x5 basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "04 Energy/04 BasicClusters Energy EE +",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy map EE +", 'description': "Average energy (in GeV) of 5x5 basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy projection eta EE +", 'description': "Eta projection of 5x5 basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC energy projection phi EE +", 'description': "Phi projection of 5x5 basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "04 Energy/05 Basic Clusters Transverse Energy EE -",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC ET map EE -", 'description': "Average transverse energy (in GeV) of hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC ET projection eta EE -", 'description': "Average transverse energy (in GeV) of hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC ET projection phi EE -", 'description': "Average transverse energy (in GeV) of hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "04 Energy/05 Basic Clusters Transverse Energy EE +",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC ET map EE +", 'description': "Average transverse energy (in GeV) of hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC ET projection eta EE +", 'description': "Average transverse energy (in GeV) of hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC ET projection phi EE +", 'description': "Average transverse energy (in GeV) of hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "04 Energy/06 Basic Clusters Size EE -",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC size map EE -", 'description': "Number of crystals constituent the basic cluster. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC size projection eta EE -", 'description': "Number of crystals constituent the basic cluster. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC size projection phi EE -", 'description': "Number of crystals constituent the basic cluster. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "04 Energy/06 Basic Clusters Size EE +",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC size map EE +", 'description': "Number of crystals constituent the basic cluster. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC size projection eta EE +", 'description': "Number of crystals constituent the basic cluster. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC size projection phi EE +", 'description': "Number of crystals constituent the basic cluster. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "04 Energy/07 SuperClusters 1D",
  [{ 'path': "$prefix/EEClusterTask/EECLT SC number", 'description': "Multiplicity of reconstructed multi-5x5 superclusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EEClusterTask/EECLT SC size (crystal)", 'description': "Numbers of crystals contained in multi-5x5 superclusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "04 Energy/08 Peaks",
  [{ 'path': "$prefix/EEClusterTask/EECLT dicluster invariant mass Pi0 sel", 'description': "Invariant mass of 2 ECAL supercluster. Complete combinatorics is shown of clusters with ET>1.0 GeV, low sigmaIEtaIEta, and Pt(candidate)>1.5 GeV. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT dicluster invariant mass JPsi sel", 'description': "Invariant mass of 2 ECAL supercluster. Complete combinatorics is shown of clusters with ET>1.0 GeV, low sigmaIEtaIEta, and Pt(candidate)>1.5 GeV. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EEClusterTask/EECLT dicluster invariant mass Z0 sel", 'description': "Invariant mass of 2 ECAL supercluster. Complete combinatorics is shown of clusters with ET>1.0 GeV, low sigmaIEtaIEta, and Pt(candidate)>1.5 GeV. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT dicluster invariant mass high sel", 'description': "Invariant mass of 2 ECAL supercluster. Complete combinatorics is shown of clusters with ET>1.0 GeV, low sigmaIEtaIEta, and Pt(candidate)>1.5 GeV. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "05 Timing/00 Timing Summary",
  [{ 'path': "$prefix/EESummaryClient/EETMT EE - timing quality summary", 'description': "Quality summary of the crystal reconstructed hits in EE - with amplitude above 16 ADC counts. Hardware timing is adjusted with readout tower granularity, but finer setting can be reached. Expected all green color. Legend: green = good;  red = bad;  yellow = no sufficient statistics.  <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EETMT EE + timing quality summary", 'description': "Quality summary of the crystal reconstructed hits in EE + with amplitude above 16 ADC counts. Hardware timing is adjusted with readout tower granularity, but finer setting can be reached. Expected all green color. Legend: green = good;  red = bad;  yellow = no sufficient statistics.  <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

eeecalshifterlayout(dqmitems, "05 Timing/01 Timing Mean",
  [{ 'path': "$prefix/EESummaryClient/EETMT EE - timing mean 1D summary", 'description': "Mean timing of all the channels in EE -. Timing is expected within 5.5 - 6.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EESummaryClient/EETMT EE + timing mean 1D summary", 'description': "Mean timing of all the channels in EE +. Timing is expected within 5.5 - 6.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EESummaryClient/EETMT timing mean", 'description': "Mean timing of all the channels in each DCC of EE. Timing is expected within 5.5 - 6.5 clocks. The error bar represents the spreads among the crystal of each DCC. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "05 Timing/02 Timing RMS",
  [{ 'path': "$prefix/EESummaryClient/EETMT EE - timing rms 1D summary", 'description': "Timing RMS of all the channels in EE -. Timing RMS is expected < 0.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EESummaryClient/EETMT EE + timing rms 1D summary", 'description': "Timing RMS of all the channels in EE +. Timing RMS is expected < 0.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EESummaryClient/EETMT timing rms", 'description': "Timing RMS of all the channels in each DCC of EE. The error bar represents the spreads among the crystal of each DCC. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "05 Timing/03 Timing Map EE -",
  [{ 'path': "$prefix/EETimingTask/EETMT timing map EE -", 'description': "Average timing (in clock units) of the seeds of the good rechits (good shape and amplitude > 960 MeV). Expected about 5.5 clocks. Readout tower binning (5x5 crystals) is used. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EETimingTask/EETMT timing projection eta EE -", 'description': "Average timing (in clock units) of the seeds of the good rechits (good shape and amplitude > 960 MeV). Expected about 5.5 clocks. Readout tower binning (5x5 crystals) is used. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETimingTask/EETMT timing projection phi EE -", 'description': "Average timing (in clock units) of the seeds of the good rechits (good shape and amplitude > 960 MeV). Expected about 5.5 clocks. Readout tower binning (5x5 crystals) is used. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "05 Timing/03 Timing Map EE +",
  [{ 'path': "$prefix/EETimingTask/EETMT timing map EE +", 'description': "Average timing (in clock units) of the seeds of the good rechits (good shape and amplitude > 960 MeV). Expected about 5.5 clocks. Readout tower binning (5x5 crystals) is used. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EETimingTask/EETMT timing projection eta EE +", 'description': "Average timing (in clock units) of the seeds of the good rechits (good shape and amplitude > 960 MeV). Expected about 5.5 clocks. Readout tower binning (5x5 crystals) is used. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETimingTask/EETMT timing projection phi EE +", 'description': "Average timing (in clock units) of the seeds of the good rechits (good shape and amplitude > 960 MeV). Expected about 5.5 clocks. Readout tower binning (5x5 crystals) is used. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "05 Timing/04 Timing Vs Amplitude EE",
  [{ 'path': "$prefix/EETimingTask/EETMT timing vs amplitude summary EE +", 'description': "Average timing (in clock units) of the seeds of the good rechits (good shape and amplitude > 960 MeV). Expected about 5.5 clocks. Readout tower binning (5x5 crystals) is used. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EETimingTask/EETMT timing vs amplitude summary EE -", 'description': "Average timing (in clock units) of the seeds of the good rechits (good shape and amplitude > 960 MeV). Expected about 5.5 clocks. Readout tower binning (5x5 crystals) is used. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "05 Timing/04 Single Event Timing",
  [{ 'path': "$prefix/EETimingTask/EETMT timing 1D summary EE -", 'description': "Single event timing (in clock units) of the good rechits (good shape and amplitude > 500 MeV). Expected about 5.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETimingTask/EETMT timing 1D summary EE +", 'description': "Single event timing (in clock units) of the good rechits (good shape and amplitude > 500 MeV). Expected about 5.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EETimingTask/EETMT timing EE+ - EE-", 'description': "Event by event difference between the average timing in EE+ and EE- (in clock units) of the good rechits (good shape and amplitude > 500 MeV). Expected 0. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETimingTask/EETMT timing min vs Et min", 'description': "x-axis: Minimum of sum(E_T) of good rechits with E_T>300 MeV between EE- and EE+. Expect small E_T in one of the two endcaps in case of no collisions. y-axis: Minimum of the weighted (with weight = E_T of rechit) time average of good rechits with E_T>300 MeV. Expect one endcap earlier respect the other one in case of no collisions. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/00 TP Spectra EE -",
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT Et spectrum Real Digis EE -", 'description': "Average transverse energy (4 ADC count = 1 GeV) of real L1 trigger primitives. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EETriggerTowerTask/Emulated/EETTT Et spectrum Emulated Digis EE -", 'description': "Average transverse energy (4 ADC count = 1 GeV) of emulated L1 trigger primitives. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETriggerTowerTask/Emulated/EETTT Et spectrum Emulated Digis max EE -", 'description': "Average transverse energy (4 ADC count = 1 GeV) of emulated L1 trigger primitives which gives the max Et. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/00 TP Spectra EE +",
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT Et spectrum Real Digis EE +", 'description': "Average transverse energy (4 ADC count = 1 GeV) of real L1 trigger primitives. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EETriggerTowerTask/Emulated/EETTT Et spectrum Emulated Digis EE +", 'description': "Average transverse energy (4 ADC count = 1 GeV) of emulated L1 trigger primitives. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETriggerTowerTask/Emulated/EETTT Et spectrum Emulated Digis max EE +", 'description': "Average transverse energy (4 ADC count = 1 GeV) of emulated L1 trigger primitives which gives the max Et. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/01 TriggerPrimitives ET",
  [{ 'path': "$prefix/EESummaryClient/EETTT EE - Et trigger tower summary", 'description': "Average transverse energy of L1 trigger primitives. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EESummaryClient/EETTT EE + Et trigger tower summary", 'description': "Average transverse energy of L1 trigger primitives. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/02 TP data matching Emulator EE -",
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT TP matching index EE -", 'description': "Sample of the emulated TP that matches the real TP. -1 means no match. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT max TP matching index EE -", 'description': "Sample of the emulated TP that givest the largest ET. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/02 TP data matching Emulator EE +",
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT TP matching index EE +", 'description': "Sample of the emulated TP that matches the real TP. -1 means no match. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT max TP matching index EE +", 'description': "Sample of the emulated TP that givest the largest ET. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/03 Trigger Most Frequent Timing",
  [{ 'path': "$prefix/EESummaryClient/EETTT EE - Trigger Primitives Timing summary", 'description': "Sample of the emulated TP that more often matches the real TP. Matched sample appear in non-red colors. Match with on-time primitives appear yellow (expected). No match at all appears red. No events appear white. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EETTT EE + Trigger Primitives Timing summary", 'description': "Sample of the emulated TP that more often matches the real TP. Matched sample appear in non-red colors. Match with on-time primitives appear yellow (expected). No match at all appears red. No events appear white. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/04 Fraction of Non Single Timing",
  [{ 'path': "$prefix/EESummaryClient/EETTT EE - Trigger Primitives Non Single Timing summary", 'description': "Fraction of events where the TP has a matching with emulator different by the most probable (expected empty).  <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EETTT EE + Trigger Primitives Non Single Timing summary", 'description': "Fraction of events where the TP has a matching with emulator different by the most probable (expected empty). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/05 Timing Calo Triggers",
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT TCC timing calo triggers Real Digis EE -", 'description': "TP sample matching with emulator divided by TCC number (in EE 4 TCC / 1 DCC). Expect all TCCs at sample = 3. Only events with HLT bit = HLT_EgammaSuperClusterOnly_L1R are considered. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT TCC timing calo triggers Real Digis EE +", 'description': "TP sample matching with emulator divided by TCC number (in EE 4 TCC / 1 DCC). Expect all TCCs at sample = 3. Only events with HLT bit = HLT_EgammaSuperClusterOnly_L1R are considered. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/06 Timing Muon Triggers",
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT TCC timing muon triggers Real Digis EE -", 'description': "TP sample matching with emulator divided by TCC number (in EB 1 TCC = 1 DCC). Expect all TCCs at sample = 3. Only events with HLT bit = HLT_L1MuOpen are considered. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT TCC timing muon triggers Real Digis EE +", 'description': "TP sample matching with emulator divided by TCC number (in EB 1 TCC = 1 DCC). Expect all TCCs at sample = 3. Only events with HLT bit = HLT_L1MuOpen are considered. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/07 Trigger Quality",
  [{ 'path': "$prefix/EESummaryClient/EETTT EE - emulator error quality summary", 'description': "Result of the comparison of Et and Fine Grain Veto between real and emulated trigger primitives. Non matched trigger primitives appear red. Expect green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EETTT EE + emulator error quality summary", 'description': "Result of the comparison of Et and Fine Grain Veto between real and emulated trigger primitives. Non matched trigger primitives appear red. Expect green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/08 TTF mismatch",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT TT flag mismatch EE -", 'description': "Occupancy of trigger primitives of high or medium interest which have not been fully readout. Expected empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT TT flag mismatch EE +", 'description': "Occupancy of trigger primitives of high or medium interest which have not been fully readout. Expected empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/09 Trigger Timing Occupancy EE +",
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT Et vs bx Real Digis EE +", 'description': "Average Et of trigger primitives with Et>0 vs the bunch crossing number. Finer binning is used around the abort gap and at the beginning of the physics gap, larger in the middle of the orbit. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT TP occupancy vs bx Real Digis EE +", 'description': "Average number of trigger primitives with Et>0 vs the bunch crossing number. Finer binning is used around the abort gap and at the beginning of the physics gap, larger in the middle of the orbit. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/09 Trigger Timing Occupancy EE -",
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT Et vs bx Real Digis EE -", 'description': "Average Et of trigger primitives with Et>0 vs the bunch crossing number. Finer binning is used around the abort gap and at the beginning of the physics gap, larger in the middle of the orbit. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT TP occupancy vs bx Real Digis EE -", 'description': "Average number of trigger primitives with Et>0 vs the bunch crossing number. Finer binning is used around the abort gap and at the beginning of the physics gap, larger in the middle of the orbit. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/10 SelectiveReadout EventSize",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT event size EE -", 'description': "ECAL endcap event size (kB). Expected below 100 kB/event. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT event size EE +", 'description': "ECAL endcap event size (kB). Expected below 100 kB/event. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/11 SelectiveReadout AverageEventSize",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT DCC event size", 'description': "Average sector event size (kB). Expected below 100 kB/event. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [None])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/12 FullReadout SRFlags",
  [{ 'path': "EcalEndcap/EESelectiveReadoutTask/EESRT full readout SR Flags EE -", 'description': "Rate of full readout Selective Readout flags. The selective readout of the ECAL was designed to reduce the ECAL data size by a factor 20. If any tower is fully read out more than 5% of the times, this tower will increase the payload and introduce deadtime. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "EcalEndcap/EESelectiveReadoutTask/EESRT full readout SR Flags EE +", 'description': "Rate of full readout Selective Readout flags. The selective readout of the ECAL was designed to reduce the ECAL data size by a factor 20. If any tower is fully read out more than 5% of the times, this tower will increase the payload and introduce deadtime. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/13 SelectiveReadout Payload EE -",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT high interest payload EE -", 'description': "Size of high interest trigger towers (kB). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{'path': "$prefix/EESelectiveReadoutTask/EESRT low interest payload EE -", 'description': "Size of low interest trigger towers (kB). <a href=https://\twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } } ])

eeecalshifterlayout(dqmitems, "06 Trigger + Selective Readout/14 SelectiveReadout Payload EE +",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT high interest payload EE +", 'description': "Size of high interest trigger towers (kB). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT low interest payload EE +", 'description': "Size of low interest trigger towers (kB). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])
EOF00

foreach $laser (@Lasers) {

print LAYOUT <<EOF01;

eeecalshifterlayout(dqmitems, "07 Calibration/00 Laser/00 L$laser Quality",
  [{ 'path': "$prefix/EESummaryClient/EELT EE - laser quality summary L$laser", 'description': "Quality summary of laser events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EELT EE + laser quality summary L$laser", 'description': "Quality summary of laser events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "07 Calibration/00 Laser/01 L$laser Errors",
  [{ 'path': "$prefix/EESummaryClient/EELT laser quality errors summary L$laser", 'description': "Errors occupancy for laser events for every sector. Expect 0 entries if no errors or no laser events in a given sector. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "07 Calibration/00 Laser/02 L$laser Amplitude",
  [{ 'path': "$prefix/EESummaryClient/EELT laser L$laser amplitude summary", 'description': "Laser L$laser average amplitude for fired light modules in EE. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EELT laser L$laser amplitude over PN summary", 'description': "Laser L$laser average VPT/PN ratio for fired light modules in EE. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "07 Calibration/00 Laser/03 L$laser Timing",
  [{ 'path': "$prefix/EESummaryClient/EELT laser L$laser timing summary", 'description': "Laser L$laser average timing (in pulse shape samples) for fired light modules in EE. Expect value around 6.5. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eeecalshifterlayout(dqmitems, "07 Calibration/00 Laser/04 L$laser PNQuality",
  [{ 'path': "$prefix/EESummaryClient/EELT PN laser quality summary L$laser", 'description': "Quality summary of laser L$laser events on PN diodes. Expect green where the laser sequence fired, yellow elsewhere. White for DCC without MEM boxes attached (EE+/-1,4,5,6,9). Red spots are failed PNs. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EELT PN laser quality errors summary L$laser", 'description': "Error occupancy for laser L$laser events on PN diodes for every sector. Expect 0 entries if no errors or no laser events in a given sector. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])
EOF01

}

foreach $led (@Leds) {

print LAYOUT <<EOF02;

eeecalshifterlayout(dqmitems, "07 Calibration/01 Led/00 L$led Quality", 
  [{ 'path': "$prefix/EESummaryClient/EELDT EE - led quality summary L$led", 'description': "Quality summary of led events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }], 
  [{ 'path': "$prefix/EESummaryClient/EELDT EE + led quality summary L$led", 'description': "Quality summary of led events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "07 Calibration/01 Led/01 L$led Errors",
  [{ 'path': "$prefix/EESummaryClient/EELDT led quality errors summary L$led", 'description': "Errors occupancy for led events for every sector. Expect 0 entries if no errors or no laser events in a given sector. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "07 Calibration/01 Led/02 L$led Amplitude",
  [{ 'path': "$prefix/EESummaryClient/EELDT led L$led amplitude summary", 'description': "Led L$led average amplitude for fired light modules in EE. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EELDT led L$led amplitude over PN summary", 'description': "Led L$led average VPT/PN ratio for fired light modules in EE. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "07 Calibration/01 Led/03 L$led Timing",
  [{ 'path': "$prefix/EESummaryClient/EELDT led L$led timing summary", 'description': "Led L$led average timing (in pulse shape samples) for fired light modules in EE. Expect value around 6.5. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eeecalshifterlayout(dqmitems, "07 Calibration/01 Led/04 L$led PNQuality",
  [{ 'path': "$prefix/EESummaryClient/EELDT PN led quality summary L$led", 'description': "Quality summary of led L$led events on PN diodes. Expect green where the laser sequence fired, yellow elsewhere. White for DCC without MEM boxes attached (EE+/-1,4,5,6,9). Red spots are failed PNs. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EELDT PN led quality errors summary L$led", 'description': "Error occupancy for led L$led events on PN diodes for every sector. Expect 0 entries if no errors or no laser events in a given sector. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])
EOF02

}

foreach $gain (@MGPAGains) {

print LAYOUT <<EOF03;

eeecalshifterlayout(dqmitems, "07 Calibration/02 Pedestal/00 Quality Gain$gain",
  [{ 'path': "$prefix/EESummaryClient/EEPT EE - pedestal quality G$gain summary", 'description': "Quality summary of pedestal events for Gain $gain in EE -. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EEPT EE + pedestal quality G$gain summary", 'description': "Quality summary of pedestal events for Gain $gain in EE +. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])
EOF03

}

foreach $gainPN (@MGPAGainsPN) {

print LAYOUT <<EOF03PN;

eeecalshifterlayout(dqmitems, "07 Calibration/02 Pedestal/01 PNQuality Gain$gainPN",
  [{ 'path': "$prefix/EESummaryClient/EEPT PN pedestal quality G$gainPN summary", 'description': "Quality summary of pedestal events on PN diodes for Gain $gainPN. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed PNs. White for DCC without MEM boxes attached (EE+/-1,4,5,6,9). Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])
EOF03PN

}

foreach $gain (@MGPAGains) {

print LAYOUT <<EOF04;

eeecalshifterlayout(dqmitems, "07 Calibration/03 TestPulse/00 Quality Gain$gain",
  [{ 'path': "$prefix/EESummaryClient/EETPT EE - test pulse quality G$gain summary", 'description': "Quality summary of test pulse events for Gain $gain in EE -. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EETPT EE + test pulse quality G$gain summary", 'description': "Quality summary of test pulse events for Gain $gain in EE +. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "07 Calibration/03 TestPulse/01 Amplitude Gain$gain",
  [{ 'path': "$prefix/EESummaryClient/EETPT test pulse amplitude G$gain summary", 'description': "Test pulse average amplitude in MGPA gain $gain for crystals for each EE sector. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])
EOF04

}

foreach $gainPN (@MGPAGainsPN) {

print LAYOUT <<EOF05PN;

eeecalshifterlayout(dqmitems, "07 Calibration/03 TestPulse/05 PNQuality Gain$gainPN",
  [{ 'path': "$prefix/EESummaryClient/EETPT PN test pulse quality G$gainPN summary", 'description': "PN diodes quality summary of test pulse events for Gain $gainPN. Expect green where the test pulse sequence fired, yellow elsewhere. Red spots are failed PNs. White for DCC without MEM boxes attached (EE+/-1,4,5,6,9). Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])
EOF05PN

}

print LAYOUT <<EOFTREND;

eeecalshifterlayout(dqmitems, "08 Trend/00 Crystal Digis Number",
  [{ 'path': "$prefix/EETrendTask/AverageNumberOfEEDigiVs5Minutes", 'description': "Average number of endcap crystal digis (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETrendTask/AverageNumberOfEEDigiVs1Hour", 'description': "Average number of endcap crystal digis (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])
   
eeecalshifterlayout(dqmitems, "08 Trend/01 Hits Number",
  [{ 'path': "$prefix/EETrendTask/AverageNumberOfEcalRecHitVs5Minutes", 'description': "Average number of endcap reconstructed hits (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETrendTask/AverageNumberOfEcalRecHitVs1Hour", 'description': "Average number of endcap reconstructed hits (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "08 Trend/02 EE - FEDs Size ",
  [{ 'path': "$prefix/EETrendTask/AverageNumberOfFEDEEminusRawDataVs5Minutes", 'description': "Average EE - FED size, in kBytes. (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETrendTask/AverageNumberOfFEDEEminusRawDataVs1Hour", 'description': "Average EE - FED size, in kBytes. (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eeecalshifterlayout(dqmitems, "08 Trend/02 EE + FEDs Size ",
  [{ 'path': "$prefix/EETrendTask/AverageNumberOfFEDEEplusRawDataVs5Minutes", 'description': "Average EE + FED size, in kBytes. (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETrendTask/AverageNumberOfFEDEEplusRawDataVs1Hour", 'description': "Average EE + FED size, in kBytes. (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

EOFTREND

# detailed layouts

for (my $i=1; $i<=18; $i++ ) {

my $ee1;
my $ee2;

if ($i>= 1 && $i<= 9) {
#  $ee1=sprintf("EE-/EE-%02d",$i);
  $ee1=sprintf("EE-%02d",$i);
  $ee2=sprintf("EE-%02d",$i);
}

if ($i>=10 && $i<=18) {
#  $ee1=sprintf("EE+/EE+%02d",$i-9);
  $ee1=sprintf("EE+%02d",$i-9);
  $ee2=sprintf("EE+%02d",$i-9);
}

print LAYOUT <<EOF02;

eelayout(dqmitems, "01 By Supermodule/$ee1/00 Integrity (Channel Global)",
  [{ 'path': "$prefix/EEIntegrityClient/EEIT data integrity quality $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT digi occupancy $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/01 Integrity (Mem Box)",
  [{ 'path': "$prefix/EEIntegrityClient/EEIT data integrity quality MEM $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT MEM digi occupancy $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/02 PedestalOnline",
  [{ 'path': "$prefix/EEPedestalOnlineClient/EEPOT pedestal quality G12 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEPedestalOnlineClient/EEPOT pedestal mean G12 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEPedestalOnlineClient/EEPOT pedestal rms G12 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/03 Timing",
  [{ 'path': "$prefix/EETimingClient/EETMT timing quality $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETimingClient/EETMT timing mean $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETimingClient/EETMT timing rms $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/04 Trigger",
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT EmulError $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT Et map Real Digis $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/05 Trigger",
  [{ 'path': "$prefix/EETriggerTowerClient/EETTT Trigger Primitives Timing $ee2", 'description': "Sample of the emulated TP that more often matches the real TP. Matched sample appear in magenta. No match at all appears red. No events appear white. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETriggerTowerClient/EETTT Non Single Timing $ee2", 'description': "Fraction of events where the TP has a matching with emulator different by the most probable (expected empty). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/06 Cosmic",
  [{ 'path': "$prefix/EECosmicTask/Sel/EECT energy sel $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EECosmicTask/Spectrum/EECT 1x1 energy spectrum $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EECosmicTask/Spectrum/EECT 3x3 energy spectrum $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/07 StatusFlags",
  [{ 'path': "$prefix/EEStatusFlagsTask/FEStatus/EESFT front-end status $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEStatusFlagsTask/FEStatus/EESFT MEM front-end status $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/08 StatusFlags",
  [{ 'path': "$prefix/EEStatusFlagsTask/FEStatus/EESFT front-end status bits $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/00 Integrity/00 Channel Integrity",
  [{ 'path': "$prefix/EEIntegrityTask/Gain/EEIT gain $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEIntegrityTask/ChId/EEIT ChId $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/00 Integrity/01 Gain Integrity",
  [{ 'path': "$prefix/EEIntegrityTask/GainSwitch/EEIT gain switch $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/00 Integrity/02 TT Integrity",
  [{ 'path': "$prefix/EEIntegrityTask/TTId/EEIT TTId $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEIntegrityTask/TTBlockSize/EEIT TTBlockSize $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/00 Integrity/03 MemBox Channel Integrity",
  [{ 'path': "$prefix/EEIntegrityTask/MemChId/EEIT MemChId $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEIntegrityTask/MemGain/EEIT MemGain $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/00 Integrity/04 MemBox TT Integrity",
  [{ 'path': "$prefix/EEIntegrityTask/MemTTId/EEIT MemTTId $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEIntegrityTask/MemSize/EEIT MemSize $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/01 Pedestal/00 Gain01",
  [{ 'path': "$prefix/EEPedestalClient/EEPT pedestal quality G01 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEPedestalClient/EEPT pedestal mean G01 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEPedestalClient/EEPT pedestal rms G01 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/01 Pedestal/01 Gain06",
  [{ 'path': "$prefix/EEPedestalClient/EEPT pedestal quality G06 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEPedestalClient/EEPT pedestal mean G06 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEPedestalClient/EEPT pedestal rms G06 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/01 Pedestal/02 Gain12",
  [{ 'path': "$prefix/EEPedestalClient/EEPT pedestal quality G12 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEPedestalClient/EEPT pedestal mean G12 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEPedestalClient/EEPT pedestal rms G12 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/01 Pedestal/03 PNs Quality",
  [{ 'path': "$prefix/EEPedestalClient/EEPT pedestal quality PNs G01 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEPedestalClient/EEPT pedestal quality PNs G16 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/01 Pedestal/04 PNs Response",
  [{ 'path': "$prefix/EEPedestalTask/PN/Gain01/EEPDT PNs pedestal $ee2 G01", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEPedestalClient/EEPDT PNs pedestal rms $ee2 G01", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EEPedestalTask/PN/Gain16/EEPDT PNs pedestal $ee2 G16", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEPedestalClient/EEPDT PNs pedestal rms $ee2 G16", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/02 TestPulse/00 Gain01",
  [{ 'path': "$prefix/EETestPulseClient/EETPT test pulse quality G01 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETestPulseClient/EETPT test pulse amplitude G01 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETestPulseClient/EETPT test pulse shape G01 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/02 TestPulse/01 Gain06",
  [{ 'path': "$prefix/EETestPulseClient/EETPT test pulse quality G06 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETestPulseClient/EETPT test pulse amplitude G06 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETestPulseClient/EETPT test pulse shape G06 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/02 TestPulse/02 Gain12",
  [{ 'path': "$prefix/EETestPulseClient/EETPT test pulse quality G12 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETestPulseClient/EETPT test pulse amplitude G12 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETestPulseClient/EETPT test pulse shape G12 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/02 TestPulse/03 PNs Quality",
  [{ 'path': "$prefix/EETestPulseClient/EETPT test pulse quality PNs G01 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETestPulseClient/EETPT test pulse quality PNs G16 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/02 TestPulse/04 PNs Gain01",
  [{ 'path': "$prefix/EETestPulseTask/PN/Gain01/EEPDT PNs pedestal $ee2 G01", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETestPulseClient/EEPDT PNs pedestal rms $ee2 G01", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EETestPulseTask/PN/Gain01/EEPDT PNs amplitude $ee2 G01", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/02 TestPulse/05 PNs Gain16",
  [{ 'path': "$prefix/EETestPulseTask/PN/Gain16/EEPDT PNs pedestal $ee2 G16", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETestPulseClient/EEPDT PNs pedestal rms $ee2 G16", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EETestPulseTask/PN/Gain16/EEPDT PNs amplitude $ee2 G16", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/03 Laser/00 Laser L1",
  [{ 'path': "$prefix/EELaserClient/EELT laser quality L1 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EELaserClient/EELT amplitude L1A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },  
   { 'path': "$prefix/EELaserClient/EELT amplitude L1B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/03 Laser/01 Laser L4",
  [{ 'path': "$prefix/EELaserClient/EELT laser quality L4 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EELaserClient/EELT amplitude L4A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },  
   { 'path': "$prefix/EELaserClient/EELT amplitude L4B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/03 Laser/02 Laser PNs",
  [{ 'path': "$prefix/EELaserClient/EELT laser quality L1 PNs G01 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" },
   { 'path': "$prefix/EELaserClient/EELT laser quality L1 PNs G16 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EELaserClient/EELT laser quality L4 PNs G01 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" },
   { 'path': "$prefix/EELaserClient/EELT laser quality L4 PNs G16 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/03 Laser/L1/00 Laser",
  [{ 'path': "$prefix/EELaserClient/EELT amplitude L1A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT amplitude over PN L1A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELaserClient/EELT amplitude L1B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT amplitude over PN L1B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/03 Laser/L1/01 Laser",
  [{ 'path': "$prefix/EELaserClient/EELT laser timing L1A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT laser shape L1A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELaserClient/EELT laser timing L1B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT laser shape L1B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/03 Laser/L1/02 Laser",
  [{ 'path': "$prefix/EELaserTask/Laser1/PN/Gain01/EELT PNs pedestal $ee2 G01 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT PNs pedestal rms $ee2 G01 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELaserTask/Laser1/PN/Gain01/EELT PNs amplitude $ee2 G01 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/03 Laser/L1/03 Laser",
  [{ 'path': "$prefix/EELaserTask/Laser1/PN/Gain16/EELT PNs pedestal $ee2 G16 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT PNs pedestal rms $ee2 G16 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELaserTask/Laser1/PN/Gain16/EELT PNs amplitude $ee2 G16 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/03 Laser/L4/00 Laser",
  [{ 'path': "$prefix/EELaserClient/EELT amplitude L4A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT amplitude over PN L4A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELaserClient/EELT amplitude L4B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT amplitude over PN L4B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/03 Laser/L4/01 Laser",
  [{ 'path': "$prefix/EELaserClient/EELT laser timing L4A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT laser shape L4A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELaserClient/EELT laser timing L4B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT laser shape L4B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/03 Laser/L4/02 Laser",
  [{ 'path': "$prefix/EELaserTask/Laser4/PN/Gain01/EELT PNs pedestal $ee2 G01 L4", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT PNs pedestal rms $ee2 G01 L4", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELaserTask/Laser4/PN/Gain01/EELT PNs amplitude $ee2 G01 L4", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/03 Laser/L4/03 Laser",
  [{ 'path': "$prefix/EELaserTask/Laser4/PN/Gain16/EELT PNs pedestal $ee2 G16 L4", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT PNs pedestal rms $ee2 G16 L4", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELaserTask/Laser4/PN/Gain16/EELT PNs amplitude $ee2 G16 L4", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/04 Led/00 Led",
  [{ 'path': "$prefix/EELedClient/EELDT led quality L1 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EELedClient/EELDT amplitude L1A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }, 
   { 'path': "$prefix/EELedClient/EELDT amplitude L1B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/04 Led/01 Led PNs",
  [{ 'path': "$prefix/EELedClient/EELDT led quality L1 PNs G01 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" },
   { 'path': "$prefix/EELedClient/EELDT led quality L1 PNs G16 $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/04 Led/L1/00 Led",
  [{ 'path': "$prefix/EELedClient/EELDT amplitude L1A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELedClient/EELDT amplitude over PN L1A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELedClient/EELDT amplitude L1B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELedClient/EELDT amplitude over PN L1B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])
   
eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/04 Led/L1/01 Led",
  [{ 'path': "$prefix/EELedClient/EELDT led timing L1A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELedClient/EELDT led shape L1A $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELedClient/EELDT led timing L1B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELedClient/EELDT led shape L1B $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])
   
eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/04 Led/L1/02 Led",
  [{ 'path': "$prefix/EELedTask/Led1/PN/Gain01/EELDT PNs pedestal $ee2 G01 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELedClient/EELDT PNs pedestal rms $ee2 G01 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELedTask/Led1/PN/Gain01/EELDT PNs amplitude $ee2 G01 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/04 Led/L1/03 Led",
  [{ 'path': "$prefix/EELedTask/Led1/PN/Gain16/EELDT PNs pedestal $ee2 G16 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELedClient/EELDT PNs pedestal rms $ee2 G16 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELedTask/Led1/PN/Gain16/EELDT PNs amplitude $ee2 G16 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/05 Timing/00 Timing",
  [{ 'path': "$prefix/EETimingClient/EETMT timing quality $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETimingClient/EETMT timing $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/05 Trigger/00 FineGrainVeto Errors",
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT EmulFineGrainVetoError $ee2", 'description': "Occupancy of mismatches between real Trigger Primitive and emulated one on the fine grain veto. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/06 Cosmic/00 Cosmic",
  [{ 'path': "$prefix/EECosmicTask/Sel/EECT energy sel $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit energy $ee2", 'description': "Average energy (in GeV) of hits over threshold (150 MeV). Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "01 By Supermodule/$ee1/Experts/06 Cosmic/01 Cosmic",
  [{ 'path': "$prefix/EECosmicTask/Spectrum/EECT 1x1 energy spectrum $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EECosmicTask/Spectrum/EECT 3x3 energy spectrum $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])
EOF02

}

print LAYOUT <<EOF991;

eelayout(dqmitems, "00 By Task/00 Summary/00 Global Summary",
  [{ 'path': "$prefix/EESummaryClient/EE global summary EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EE global summary EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/00 Summary/01 Integrity Summary",
  [{ 'path': "$prefix/EESummaryClient/EEIT EE - integrity quality summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EEIT EE + integrity quality summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/00 Summary/02 Occupancy Summary",
  [{ 'path': "$prefix/EESummaryClient/EEOT EE - digi occupancy summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EEOT EE + digi occupancy summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/00 Summary/03 Cosmic Summary",
  [{ 'path': "$prefix/EESummaryClient/EECT EE - cosmic summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EECT EE + cosmic summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/00 Summary/04 PedestalOnline Summary",
  [{ 'path': "$prefix/EESummaryClient/EEPOT EE - pedestal quality summary G12", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EEPOT EE + pedestal quality summary G12", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/00 Summary/05 LaserL1 Summary",
  [{ 'path': "$prefix/EESummaryClient/EELT EE - laser quality summary L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EELT EE + laser quality summary L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/00 Summary/06 Led Summary",
  [{ 'path': "$prefix/EESummaryClient/EELDT EE - led quality summary L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EELDT EE + led quality summary L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/00 Summary/07 Timing Summary",
  [{ 'path': "$prefix/EESummaryClient/EETMT EE - timing quality summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EETMT EE + timing quality summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/00 Summary/08 Trigger Summary",
  [{ 'path': "$prefix/EESummaryClient/EETTT EE - Et trigger tower summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EETTT EE + Et trigger tower summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/00 Summary/09 Trigger Summary",
  [{ 'path': "$prefix/EESummaryClient/EETTT EE - emulator error quality summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EETTT EE + emulator error quality summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/00 Summary/10 Trigger Summary",
  [{ 'path': "$prefix/EESummaryClient/EETTT EE - Trigger Primitives Timing summary", 'description': "Sample of the emulated TP that more often matches the real TP. Matched sample appear in magenta. No match at all appears red. No events appear white. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EETTT EE + Trigger Primitives Timing summary", 'description': "Sample of the emulated TP that more often matches the real TP. Matched sample appear in magenta. No match at all appears red. No events appear white. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/00 Summary/11 StatusFlags Summary",
  [{ 'path': "$prefix/EESummaryClient/EESFT EE - front-end status summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EESFT EE + front-end status summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

EOF991

for (my $i=1; $i<=18; $i++ ) {

my $ee1;
my $ee2;

if ($i>= 1 && $i<= 9) {
#  $ee1=sprintf("EE-/EE-%02d",$i);
  $ee1=sprintf("EE-%02d",$i);
  $ee2=sprintf("EE-%02d",$i);
}

if ($i>=10 && $i<=18) {
#  $ee1=sprintf("EE+/EE+%02d",$i-9);
  $ee1=sprintf("EE+%02d",$i-9);
  $ee2=sprintf("EE+%02d",$i-9);
}

print "$ee1\n";

print LAYOUT <<EOF992;

eelayout(dqmitems, "00 By Task/01 Integrity/00 Integrity Summary",
  [{ 'path': "$prefix/EESummaryClient/EEIT EE - integrity quality summary", 'description': "Quality summary of EE - checking that data for each crystal follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/01 Integrity/01 Integrity Summary",
  [{ 'path': "$prefix/EESummaryClient/EEIT EE + integrity quality summary", 'description': "Quality summary of EE + checking that data for each crystal follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/01 Integrity/$ee1/00 Channel Global Integrity",
  [{ 'path': "$prefix/EEIntegrityClient/EEIT data integrity quality $ee2", 'description': "Quality summary checking that data for each crystal follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT digi occupancy $ee2", 'description': "Occupancy of ECAL digis. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/01 Integrity/$ee1/01 Channel Integrity",
  [{ 'path': "$prefix/EEIntegrityTask/Gain/EEIT gain $ee2", 'description': "Occupancy of integrity errors of type: GAIN ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEIntegrityTask/ChId/EEIT ChId $ee2", 'description': "Occupancy of integrity errors of type: CHANNEL ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/01 Integrity/$ee1/02 Gain Integrity",
  [{ 'path': "$prefix/EEIntegrityTask/GainSwitch/EEIT gain switch $ee2", 'description': "Occupancy of integrity errors of type: GAIN SWITCH. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eelayout(dqmitems, "00 By Task/01 Integrity/$ee1/03 TT Integrity",
  [{ 'path': "$prefix/EEIntegrityTask/TTId/EEIT TTId $ee2", 'description': "Occupancy of integrity errors of type: TRIGGER TOWER ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEIntegrityTask/TTBlockSize/EEIT TTBlockSize $ee2", 'description': "Occupancy of integrity errors of type: TRIGGER TOWER BLOCK SIZE. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/01 Integrity/$ee1/04 MemBox Global Integrity",
  [{ 'path': "$prefix/EEIntegrityClient/EEIT data integrity quality MEM $ee2", 'description': "Quality summary checking that data for each PN follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT MEM digi occupancy $ee2", 'description': "Occupancy of ECAL PNs digis. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/01 Integrity/$ee1/05 MemBox Channel Integrity",
  [{ 'path': "$prefix/EEIntegrityTask/MemChId/EEIT MemChId $ee2", 'description': "Occupancy of errors for PNs of type: MEM CHANNEL ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEIntegrityTask/MemGain/EEIT MemGain $ee2", 'description': "Occupancy of errors for PNs of type: MEM GAIN ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/01 Integrity/$ee1/06 MemBox TT Integrity",
  [{ 'path': "$prefix/EEIntegrityTask/MemTTId/EEIT MemTTId $ee2", 'description': "Occupancy of integrity errors for PNs of type: TRIGGER TOWER ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEIntegrityTask/MemSize/EEIT MemSize $ee2", 'description': "Occupancy of errors for PNs of type: MEM SIZE. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/02 PedestalOnline/00 PedestalOnline Summary",
  [{ 'path': "$prefix/EESummaryClient/EEPOT EE - pedestal quality summary G12", 'description': "Quality summary of EE - checking the quality of the pedestals from the first 3/10 samples of the pulse shape for all the events. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/02 PedestalOnline/01 PedestalOnline Summary",
  [{ 'path': "$prefix/EESummaryClient/EEPOT EE + pedestal quality summary G12", 'description': "Quality summary of EE + checking the quality of the pedestals from the first 3/10 samples of the pulse shape for all the events. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/02 PedestalOnline/02 PedestalOnline Rms",
  [{ 'path': "$prefix/EESummaryClient/EEPOT EE - pedestal G12 RMS map", 'description': "RMS of the pedestals in ADC counts. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (calibration and physics). Expected RMS for ECAL endcap is 1.9 ADC counts (120 MeV). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/02 PedestalOnline/03 PedestalOnline Rms",
  [{ 'path': "$prefix/EESummaryClient/EEPOT EE + pedestal G12 RMS map", 'description': "RMS of the pedestals in ADC counts. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (calibration and physics). Expected RMS for ECAL endcap is 1.9 ADC counts (120 MeV). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/02 PedestalOnline/$ee1/Gain12",
  [{ 'path': "$prefix/EEPedestalOnlineClient/EEPOT pedestal quality G12 $ee2", 'description': "Quality histogram checking the quality of the pedestals from the first 3/10 samples of the pulse shape for all the events. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEPedestalOnlineClient/EEPOT pedestal mean G12 $ee2", 'description': "Mean of the pedestal value on all the events for the crystals of supermodule $ee2. Expected within 175-225 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEPedestalOnlineClient/EEPOT pedestal rms G12 $ee2", 'description': "RMS of the pedestal value on all the events for the crystals of supermodule $ee2. Expected < 2.5 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])
EOF992

foreach $gain (@MGPAGains) {

print LAYOUT <<EOF993;

eelayout(dqmitems, "00 By Task/03 Pedestal/00 Pedestal Summary Gain$gain",
  [{ 'path': "$prefix/EESummaryClient/EEPT EE - pedestal quality G$gain summary", 'description': "Quality summary of pedestal events for gain $gain on EE -. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EEPT EE + pedestal quality G$gain summary", 'description': "Quality summary of pedestal events for gain $gain on EE +. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/03 Pedestal/$ee1/00 Gain$gain",
  [{ 'path': "$prefix/EEPedestalClient/EEPT pedestal quality G$gain $ee2", 'description': "Quality histogram of pedestal events on crystals for Gain $gain on sector $ee2. Expect green if the pedestal sequence fired the sector, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEPedestalClient/EEPT pedestal mean G$gain $ee2", 'description': "Pedestal mean for all the crystals of the sector $ee2 for Gain $gain. Expected mean is between 175-225 ADC counts for all the MGPA gains. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEPedestalClient/EEPT pedestal rms G$gain $ee2", 'description': "Pedestal RMS for all the crystals of the sector $ee2 for Gain $gain. Expected RMS is < 1.0, 1.5, 2.5 ADC counts for MGPA gain 1, 6, 12, respectively. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])
EOF993

}

foreach $gainPN (@MGPAGainsPN) {

print LAYOUT <<EOF994;

eelayout(dqmitems, "00 By Task/03 Pedestal/$ee1/01 PNs Gain$gainPN",
  [{ 'path': "$prefix/EEPedestalClient/EEPT pedestal quality PNs G$gainPN $ee2", 'description': "Quality histogram of pedestal events on PNs for Gain $gainPN. Expect green if the pedestal sequence fired the sector, yellow elsewhere. Red spots are failed PNs. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEPedestalTask/PN/Gain$gainPN/EEPDT PNs pedestal $ee2 G$gainPN", 'description': "Pedestal mean for all the 10 PNs of the sector $ee2 for Gain $gainPN. Expected mean is around 750 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEPedestalClient/EEPDT PNs pedestal rms $ee2 G$gainPN", 'description': "Pedestal rms for all the 10 PNs of the sector $ee2 for Gain $gainPN. Expected rms is around 1 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])
EOF994

}

foreach $gain (@MGPAGains) {

print LAYOUT <<EOF995;

eelayout(dqmitems, "00 By Task/04 TestPulse/00 TestPulse Summary Gain$gain",
  [{ 'path': "$prefix/EESummaryClient/EETPT EE - test pulse quality G$gain summary", 'description': "Quality summary of pedestal events for Gain $gain on EE -. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EETPT EE + test pulse quality G$gain summary", 'description': "Quality summary of pedestal events for Gain $gain on EE +. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])
EOF995

}

foreach $gain (@MGPAGains) {

print LAYOUT <<EOF996;

eelayout(dqmitems, "00 By Task/04 TestPulse/$ee1/00 Gain$gain",
  [{ 'path': "$prefix/EETestPulseClient/EETPT test pulse quality G$gain $ee2", 'description': "Quality histogram of testpulse events on crystals for Gain $gain. Expect green if the testpulse sequence fired the sector $ee2, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETestPulseClient/EETPT test pulse amplitude G$gain $ee2", 'description': "Testpulse mean for all the crystals of the sector $ee2 for Gain $gain. Each channel should be within 20% of the average of the others, RMS < 300 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETestPulseClient/EETPT test pulse shape G$gain $ee2", 'description': "Pulse shape of a reference crystal averaged on all the events for Gain $gain. Clear pulse should be seen. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])
EOF996

}

foreach $gainPN (@MGPAGainsPN) {

print LAYOUT <<EOF997;

eelayout(dqmitems, "00 By Task/04 TestPulse/$ee1/01 PNs Quality Gain$gainPN",
  [{ 'path': "$prefix/EETestPulseClient/EETPT test pulse quality PNs G$gainPN $ee2", 'description': "Quality histogram of testpulse events on PNs for Gain $gain. Expect green if the testpulse sequence fired the sector $ee2, yellow elsewhere. Red spots are failed PNs. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eelayout(dqmitems, "00 By Task/04 TestPulse/$ee1/02 PNs Variables Gain$gainPN",
  [{ 'path': "$prefix/EETestPulseTask/PN/Gain$gainPN/EETPT PNs pedestal $ee2 G$gainPN", 'description': "Pedestals of the PNs in tespulse events. It should be > 200 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETestPulseClient/EETPT PNs pedestal rms $ee2 G$gainPN", 'description': "RMS of the pedestals of the PNs in testpulse events. It should be < 1ADC count in Gain 01 and < 3 ADC counts in Gain 16. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EETestPulseTask/PN/Gain$gainPN/EETPT PNs amplitude $ee2 G$gainPN", 'description': "Amplitude of the PNs in testpulse events. It should be > 100 ADC counts and each channel within 20% of the others. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])
EOF997

}

foreach $laser (@Lasers) {

print LAYOUT <<EOF999;

eelayout(dqmitems, "00 By Task/05 Laser/00 LaserL$laser Summary",
  [{ 'path': "$prefix/EESummaryClient/EELT EE - laser quality summary L$laser", 'description': "Quality summary of laser events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EELT EE + laser quality summary L$laser", 'description': "Quality summary of laser events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/05 Laser/$ee1/L$laser/00 Quality",
  [{ 'path': "$prefix/EELaserClient/EELT laser quality L$laser $ee2", 'description': "Quality histogram of laser events on crystals. Expect green if the laser sequence fired supermodule $ee2, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EELaserClient/EELT laser quality L$laser PNs G01 $ee2", 'description': "Quality histogram of laser events on PNs in Gain 1. Expect green if the laser sequence fired supermodule $ee2, yellow elsewhere. Red spots are failed PNs. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" },
   { 'path': "$prefix/EELaserClient/EELT laser quality L$laser PNs G16 $ee2", 'description': "Quality histogram of laser events on PNs in Gain 16. Expect green if the laser sequence fired supermodule $ee2, yellow elsewhere. Red spots are failed PNs. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/05 Laser/$ee1/L$laser/01 Amplitude",
  [{ 'path': "$prefix/EELaserClient/EELT amplitude L$laser $ee2", 'description': "Average amplitude for each channel of $ee2 (error is the RMS of the distribution). Quality cuts are: amplitude > 100 ADC; RMS < 30% of the mean. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELaserClient/EELT amplitude over PN L$laser $ee2", 'description': "Average APD/PN for each channel of $ee2 (error is the RMS of the distribution). Quality cuts are: APD/PN > XXX. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/05 Laser/$ee1/L$laser/02 Timing",
  [{ 'path': "$prefix/EELaserClient/EELT laser timing L$laser $ee2", 'description': "Average timing for each channel of $ee2 (error is the RMS of the distribution). Quality cuts are: timing is within 5.5 - 6.5 clocks. RMS is < 0.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELaserClient/EELT laser timing mean L$laser $ee2", 'description': "Mean timing of the channels in $ee2. Timing is expected within 5.5 - 6.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT laser timing rms L$laser $ee2", 'description': "Timing RMS of the channels in $ee2. Timing RMS is expected < 0.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/05 Laser/$ee1/L$laser/03 Pulse Shape",
  [{ 'path': "$prefix/EELaserClient/EELT laser shape L$laser $ee2", 'description': "Pulse shape of a reference crystal (crystal 1 of $ee2) averaged on all the events. Clear pulse should be seen. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELaserTask/Laser$laser/EELT shape $ee2 L$laser", 'description': "Pulse shape of all the crystals in $ee2, averaged on all the events. Clear pulses should be seen. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/05 Laser/$ee1/L$laser/04 PNs Gain01",
  [{ 'path': "$prefix/EELaserTask/Laser$laser/PN/Gain01/EELT PNs pedestal $ee2 G01 L$laser", 'description': "Average pedestals of the 10 PNs of $ee2 in Gain 1. Expected around 760 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT PNs pedestal rms $ee2 G01 L$laser", 'description': "RMS of the pedestals of PN diodes of $ee2 in Gain 1. Expected around 2.0 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELaserTask/Laser$laser/PN/Gain01/EELT PNs amplitude $ee2 G01 L$laser", 'description': "Amplitudes of the 10 PNs of $ee2 in Gain1. Expected values around 1500 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eelayout(dqmitems, "00 By Task/05 Laser/$ee1/L$laser/05 PNs Gain16",
  [{ 'path': "$prefix/EELaserTask/Laser$laser/PN/Gain16/EELT PNs pedestal $ee2 G16 L$laser", 'description': "Average pedestals of the 10 PNs of $ee2 in Gain 16. Expected around 760 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELaserClient/EELT PNs pedestal rms $ee2 G16 L$laser", 'description': "RMS of the pedestals of PN diodes of $ee2 in Gain 16. Expected around 2.0 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELaserTask/Laser$laser/PN/Gain16/EELT PNs amplitude $ee2 G16 L$laser", 'description': "Amplitudes of the 10 PNs of $ee2 in Gain16. Expected values around 1500 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

EOF999

}

foreach $led (@Leds) {

print LAYOUT <<EOF998;

eelayout(dqmitems, "00 By Task/06 Led/00 LedL$led Summary",
  [{ 'path': "$prefix/EESummaryClient/EELDT EE - led quality summary L$led", 'description': "Quality summary of led events. Expect green where the led sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESummaryClient/EELDT EE + led quality summary L$led", 'description': "Quality summary of led events. Expect green where the led sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/06 Led/$ee1/L$led/00 Quality",
  [{ 'path': "$prefix/EELedClient/EELDT led quality L$led $ee2", 'description': "Quality histogram of led events on crystals. Expect green if the led sequence fired supermodule $ee2, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EELedClient/EELDT led quality L$led PNs G01 $ee2", 'description': "Quality histogram of led events on PNs in Gain 1. Expect green if the led sequence fired supermodule $ee2, yellow elsewhere. Red spots are failed PNs. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" },
   { 'path': "$prefix/EELedClient/EELDT led quality L$led PNs G16 $ee2", 'description': "Quality histogram of led events on PNs in Gain 16. Expect green if the led sequence fired supermodule $ee2, yellow elsewhere. Red spots are failed PNs. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/06 Led/$ee1/L$led/01 Amplitude",
  [{ 'path': "$prefix/EELedClient/EELDT amplitude L$led $ee2", 'description': "Average amplitude for each channel of $ee2 (error is the RMS of the distribution). Quality cuts are: amplitude > 100 ADC; RMS < 30% of the mean. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELedClient/EELDT amplitude over PN L$led $ee2", 'description': "Average APD/PN for each channel of $ee2 (error is the RMS of the distribution). Quality cuts are: APD/PN > XXX. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/06 Led/$ee1/L$led/02 Timing",
  [{ 'path': "$prefix/EELedClient/EELDT led timing L$led $ee2", 'description': "Average timing for each channel of $ee2 (error is the RMS of the distribution). Quality cuts are: timing is within 5.5 - 6.5 clocks. RMS is < 0.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELedClient/EELDT led timing mean L$led $ee2", 'description': "Mean timing of the channels in $ee2. Timing is expected within 5.5 - 6.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELedClient/EELDT led timing rms L$led $ee2", 'description': "Timing RMS of the channels in $ee2. Timing RMS is expected < 0.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/06 Led/$ee1/L$led/03 Pulse Shape",
  [{ 'path': "$prefix/EELedClient/EELDT led shape L$led $ee2", 'description': "Pulse shape of a reference crystal (crystal 1 of $ee2) averaged on all the events. Clear pulse should be seen. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELedTask/Led$led/EELDT shape $ee2 L$led", 'description': "Pulse shape of all the crystals in $ee2, averaged on all the events. Clear pulses should be seen. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/06 Led/$ee1/L$led/04 PNs Gain01",
  [{ 'path': "$prefix/EELedTask/Led$led/PN/Gain01/EELDT PNs pedestal $ee2 G01 L$led", 'description': "Average pedestals of the 10 PNs of $ee2 in Gain 1. Expected around 760 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELedClient/EELDT PNs pedestal rms $ee2 G01 L$led", 'description': "RMS of the pedestals of PN diodes of $ee2 in Gain 1. Expected around 2.0 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELedTask/Led$led/PN/Gain01/EELDT PNs amplitude $ee2 G01 L$led", 'description': "Amplitudes of the 10 PNs of $ee2 in Gain1. Expected values around 1500 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eelayout(dqmitems, "00 By Task/06 Led/$ee1/L$led/05 PNs Gain16",
  [{ 'path': "$prefix/EELedTask/Led$led/PN/Gain16/EELDT PNs pedestal $ee2 G16 L$led", 'description': "Average pedestals of the 10 PNs of $ee2 in Gain 16. Expected around 760 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EELedClient/EELDT PNs pedestal rms $ee2 G16 L$led", 'description': "RMS of the pedestals of PN diodes of $ee2 in Gain 16. Expected around 2.0 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EELedTask/Led$led/PN/Gain16/EELDT PNs amplitude $ee2 G16 L$led", 'description': "Amplitudes of the 10 PNs of $ee2 in Gain16. Expected values around 1500 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

EOF998

}

print LAYOUT <<EOF994;

eelayout(dqmitems, "00 By Task/07 Timing/00 Timing Summary",
  [{ 'path': "$prefix/EESummaryClient/EETMT EE - timing quality summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/07 Timing/01 Timing Summary",
  [{ 'path': "$prefix/EESummaryClient/EETMT EE + timing quality summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/07 Timing/$ee1/00 Timing Quality",
  [{ 'path': "$prefix/EETimingClient/EETMT timing quality $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETimingClient/EETMT timing mean $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EETimingClient/EETMT timing rms $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/07 Timing/$ee1/01 Timing Value",
  [{ 'path': "$prefix/EETimingClient/EETMT timing $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EETimingTask/EETMT timing 1D $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/07 Timing/$ee1/02 Timing vs Amplitude",
  [{ 'path': "$prefix/EETimingTask/EETMT timing $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EETimingTask/EETMT timing vs amplitude $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/08 Trigger/00 Trigger Summary",
  [{ 'path': "$prefix/EESummaryClient/EETTT EE - Trigger Primitives Timing summary", 'description': "Sample of the emulated TP that more often matches the real TP. Matched sample appear in non-red colors. Match with on-time primitives appear yellow (expected). No match at all appears red. No events appear white. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/08 Trigger/01 Trigger Summary",
  [{ 'path': "$prefix/EESummaryClient/EETTT EE + Trigger Primitives Timing summary", 'description': "Sample of the emulated TP that more often matches the real TP. Matched sample appear in non-red colors. Match with on-time primitives appear yellow (expected). No match at all appears red. No events appear white. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/08 Trigger/$ee1/00 TransverseEnergy",
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT EmulError $ee2", 'description': "Number of errors comparing the values of Et for real and emulated trigger primitives. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT Et map Real Digis $ee2", 'description': "Average transverse energy (ADC counts. 4 ADCs = 1GeV) for real trigger primitives. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/08 Trigger/$ee1/01 TriggerPrimitivesTiming",
  [{ 'path': "$prefix/EETriggerTowerClient/EETTT Trigger Primitives Timing $ee2", 'description': "Sample of the emulated TP that more often matches the real TP. Matched sample appear in magenta. No match at all appears red. No events appear white. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
    [{ 'path': "$prefix/EETriggerTowerClient/EETTT Non Single Timing $ee2", 'description': "Fraction of events where the TP has a matching with emulator different by the most probable (expected empty). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/08 Trigger/$ee1/02 FineGrainVeto Errors",
  [{ 'path': "$prefix/EETriggerTowerTask/EETTT EmulFineGrainVetoError $ee2", 'description': "Occupancy of mismatches between real Trigger Primitive and emulated one on the fine grain veto. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eelayout(dqmitems, "00 By Task/09 Cosmic/00 Cosmic Summary",
  [{ 'path': "$prefix/EESummaryClient/EECT EE - cosmic summary", 'description': "Average energy (in GeV) of hits selected with cosmics analysis. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/09 Cosmic/01 Cosmic Summary",
  [{ 'path': "$prefix/EESummaryClient/EECT EE + cosmic summary", 'description': "Average energy (in GeV) of hits selected with cosmics analysis. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/09 Cosmic/$ee1/00 Cosmic",
  [{ 'path': "$prefix/EECosmicTask/Sel/EECT energy sel $ee2", 'description': "Average energy (in GeV) of hits selected with cosmics analysis. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit energy $ee2", 'description': "Average energy (in GeV) of hits over threshold (150 MeV). Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/09 Cosmic/$ee1/01 Cosmic",
  [{ 'path': "$prefix/EECosmicTask/Spectrum/EECT 1x1 energy spectrum $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EECosmicTask/Spectrum/EECT 3x3 energy spectrum $ee2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/10 StatusFlags/00 FrontEnd Summary",
  [{ 'path': "$prefix/EESummaryClient/EESFT EE - front-end status summary", 'description': "DCC front-end status quality summary. Expected all green color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])
eelayout(dqmitems, "00 By Task/10 StatusFlags/01 FrontEnd Summary",
  [{ 'path': "$prefix/EESummaryClient/EESFT EE + front-end status summary", 'description': "DCC front-end status quality summary. Expected all green color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/10 StatusFlags/$ee1/00 StatusFlags",
  [{ 'path': "$prefix/EEStatusFlagsTask/FEStatus/EESFT front-end status $ee2", 'description': "Occupancy of DCC front-end errors. Status ACTIVE (i.e. full-readout) or SUPPRESSED (i.e. zero-suppression applied) are not considered as errors and do not create entries in this plot. Expected all empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEStatusFlagsTask/FEStatus/EESFT MEM front-end status $ee2", 'description': "Occupancy of DCC front-end errors for the MEM boxes (TTs: 69-70). Status ACTIVE (i.e. full-readout) or SUPPRESSED (i.e. zero-suppression applied) are not considered as errors and do not create entries in this plot. Expected all empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/10 StatusFlags/$ee1/01 StatusFlags",
  [{ 'path': "$prefix/EEStatusFlagsTask/FEStatus/EESFT front-end status bits $ee2", 'description': "Occupancy of DCC front-end statuses by type. ACTIVE and SUPPRESSED are normal entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])
EOF994

}

print LAYOUT <<EOF993;

eelayout(dqmitems, "00 By Task/11 Cluster/00 BasicClusters 1D",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC size", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC number", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eelayout(dqmitems, "00 By Task/11 Cluster/01 BasicClusters Energy",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy map EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy projection eta EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC energy projection phi EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/11 Cluster/02 BasicClusters Energy",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy map EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC energy projection eta EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC energy projection phi EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/11 Cluster/03 BasicClusters Size",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC size map EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC size projection eta EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC size projection phi EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/11 Cluster/04 BasicClusters Size",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC size map EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC size projection eta EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC size projection phi EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/11 Cluster/05 BasicClusters TransverseEnergy",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC ET map EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC ET projection eta EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC ET projection phi EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/11 Cluster/06 BasicClusters TransverseEnergy",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC ET map EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC ET projection eta EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC ET projection phi EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/11 Cluster/07 BasicClusters Multiplicity",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC number map EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC number projection eta EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC number projection phi EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/11 Cluster/08 BasicClusters Multiplicity",
  [{ 'path': "$prefix/EEClusterTask/EECLT BC number map EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEClusterTask/EECLT BC number projection eta EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT BC number projection phi EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/11 Cluster/09 SuperClusters 1D",
  [{ 'path': "$prefix/EEClusterTask/EECLT SC energy", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT SC size", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EEClusterTask/EECLT SC number", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eelayout(dqmitems, "00 By Task/11 Cluster/12 SuperClusters Seeds",
  [{ 'path': "$prefix/EEClusterTask/EECLT SC single crystal cluster seed occupancy map EE -", 'description': "Occupancy of seeds of superclusters formed by exactly one crystal. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EEClusterTask/EECLT SC single crystal cluster seed occupancy map EE +", 'description': "Occupancy of seeds of superclusters formed by exactly one crystal. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/11 Cluster/13 SuperClusters Seeds",
  [{ 'path': "$prefix/EEClusterTask/EECLT SC energy vs seed crystal energy", 'description': "supercluster energy vs seed crystal energy. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [None])

eelayout(dqmitems, "00 By Task/11 Cluster/14 ClusterShapes",
  [{ 'path': "$prefix/EEClusterTask/EECLT s1s9", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EEClusterTask/EECLT s9s25", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/11 Cluster/15 Full Combinatorics (Selected)",
  [{ 'path': "$prefix/EEClusterTask/EECLT dicluster invariant mass Pi0 sel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT dicluster invariant mass JPsi sel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EEClusterTask/EECLT dicluster invariant mass Z0 sel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT dicluster invariant mass high sel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/11 Cluster/16 Best Pair Combinatorics (No sel)",
  [{ 'path': "$prefix/EEClusterTask/EECLT dicluster invariant mass Pi0", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT dicluster invariant mass JPsi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EEClusterTask/EECLT dicluster invariant mass Z0", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEClusterTask/EECLT dicluster invariant mass high", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

EOF993

print LAYOUT <<EOF995;

eelayout(dqmitems, "00 By Task/12 Occupancy/00 Occupancy",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT digi occupancy EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT digi occupancy EE - projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT digi occupancy EE - projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/12 Occupancy/01 Occupancy",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT digi occupancy EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT digi occupancy EE + projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT digi occupancy EE + projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/12 Occupancy/02 Occupancy",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE - projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE - projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/12 Occupancy/03 Occupancy",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE + projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT rec hit occupancy EE + projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/12 Occupancy/04 Occupancy",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit thr occupancy EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit thr occupancy EE - projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT rec hit thr occupancy EE - projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/12 Occupancy/05 Occupancy",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit thr occupancy EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT rec hit thr occupancy EE + projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT rec hit thr occupancy EE + projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/12 Occupancy/06 Occupancy",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi occupancy EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi occupancy EE - projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT TP digi occupancy EE - projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/12 Occupancy/07 Occupancy",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi occupancy EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi occupancy EE + projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT TP digi occupancy EE + projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/12 Occupancy/08 Occupancy",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE - projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE - projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/12 Occupancy/09 Occupancy",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE + projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EEOccupancyTask/EEOT TP digi thr occupancy EE + projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/12 Occupancy/10 Occupancy",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT test pulse digi occupancy EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT test pulse digi occupancy EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/12 Occupancy/11 Occupancy",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT laser digi occupancy EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT laser digi occupancy EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/12 Occupancy/12 Occupancy",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT led digi occupancy EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT led digi occupancy EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/12 Occupancy/13 Occupancy",
  [{ 'path': "$prefix/EEOccupancyTask/EEOT pedestal digi occupancy EE -", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EEOccupancyTask/EEOT pedestal digi occupancy EE +", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/13 RawData/00 RawData",
  [{ 'path': "$prefix/EERawDataTask/EERDT event type calibration BX", 'description': "Event type in ECAL DCC header in the calibration BX. It should contain only calibration events. It is filled once per DCC with weight 1/36. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EERawDataTask/EERDT calibration event errors", 'description': "Number of events with at least one FED containing calibration events during physics gap or physics events during the calibration BX. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/13 RawData/01 RawData",
  [{ 'path': "$prefix/EERawDataTask/EERDT event type pre calibration BX", 'description': "Event type in ECAL DCC header in the BXs reserved to physics events preceding calibration BX. It should contain only physics events. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EERawDataTask/EERDT event type post calibration BX", 'description': "Event type in ECAL DCC header in the BXs reserved to physics events following calibration BX. It should contain only physics events. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/13 RawData/02 RawData",
  [{ 'path': "$prefix/EERawDataTask/EERDT CRC errors", 'description': "Number of fatal errors related to the event size. It is filled once per DCC with weight 1/36. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EERawDataTask/EERDT trigger type errors", 'description': "Number of trigger type mismatches between the global trigger and ECAL DCC header. It is filled once per DCC with weight 1/36. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/13 RawData/03 RawData",
  [{ 'path': "$prefix/EERawDataTask/EERDT run number errors", 'description': "Number of run-number mismatches between the global trigger and ECAL DCC header. It is filled once per DCC with weight 1/36. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EERawDataTask/EERDT orbit number errors", 'description': "Number of orbit-number mismatches between the global trigger and ECAL DCC header. It is filled once per DCC with weight 1/36. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/13 RawData/04 RawData",
  [{ 'path': "$prefix/EERawDataTask/EERDT L1A DCC errors", 'description': "Number of level 1 event mismatches between the global trigger and ECAL DCC header. It is filled once per DCC with weight 1/36. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EERawDataTask/EERDT bunch crossing DCC errors", 'description': "Number of bunch-crossing mismatches between the global trigger and ECAL DCC header. It is filled once per DCC with weight 1/36. It should be empty.<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/13 RawData/05 RawData",
  [{ 'path': "$prefix/EERawDataTask/EERDT L1A FE errors", 'description': "Number of level 1 event mismatches between the ECAL DCC header and its front-end cards. It is filled once per front-end card in DCC with weight 1/n(front-end cards). It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EERawDataTask/EERDT bunch crossing FE errors", 'description': "Number of bunch-crossing mismatches between the ECAL DCC header and its front-end cards. It is filled once per front-end card in DCC with weight 1/n(front-end cards). It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/13 RawData/06 RawData",
  [{ 'path': "$prefix/EERawDataTask/EERDT L1A TCC errors", 'description': "Number of level 1 event mismatches between the ECAL DCC header and its TCCs. It is filled once per TCC in DCC with weight 1/n(TCC). It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EERawDataTask/EERDT bunch crossing TCC errors", 'description': "Number of bunch-crossing mismatches between the ECAL DCC header and its TCCs. It is filled once per TCC in DCC with weight 1/n(TCC). It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/13 RawData/07 RawData",
  [{ 'path': "$prefix/EERawDataTask/EERDT L1A SRP errors", 'description': "Number of level 1 event mismatches between the ECAL DCC header and its SRP block. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EERawDataTask/EERDT bunch crossing SRP errors", 'description': "Number of bunch-crossing mismatches between the ECAL DCC header and its SRP block. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/14 SelectiveReadout/00 EventSize",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT event size EE -", 'description': "Event size for the readout crystals in the ECAL endcap. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT event size EE +", 'description': "Event size for the readout crystals in the ECAL endcap. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/14 SelectiveReadout/01 DCC EventSize",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT DCC event size", 'description': "Average event size per ecah ECAL endcap DCC. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT event size vs DCC", 'description': "Event size distribution per each ECAL endcap DCC. Finer binning is used in the zero-suppressed region (0-0.608 kB), while granularity of one fully readout tower (0.608 kB) has been used for higher sizes. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>"}])

eelayout(dqmitems, "00 By Task/14 SelectiveReadout/02 ReadoutUnitSRForced",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT readout unit with SR forced EE -", 'description': "Rate of readout units with Selective Readout Forced. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT readout unit with SR forced EE +", 'description': "Rate of readout units with Selective Readout Forced. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/14 SelectiveReadout/03 FullReadout",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT full readout SR Flags EE -", 'description': "Rate of full readout Selective Readout flags. The selective readout of the ECAL was designed to reduce the ECAL data size by a factor 20. If any tower is fully read out more than 5% of the times, this tower will increase the payload and introduce deadtime. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT full readout SR Flags EE +", 'description': "Rate of full readout Selective Readout flags. The selective readout of the ECAL was designed to reduce the ECAL data size by a factor 20. If any tower is fully read out more than 5% of the times, this tower will increase the payload and introduce deadtime. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/14 SelectiveReadout/04 HighInterestTTFlags",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT high interest TT Flags EE -", 'description': "Rate of high interest Trigger Tower flags. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT high interest TT Flags EE +", 'description': "Rate of high interest Trigger Tower flags. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/14 SelectiveReadout/05 LowInterestTTFlags",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT low interest TT Flags EE -", 'description': "Rate of low interest Trigger Tower flags. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT low interest TT Flags EE +", 'description': "Rate of low interest Trigger Tower flags. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/14 SelectiveReadout/07 HighInterestPayload",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT high interest payload EE -", 'description': "Payload of the high interest trigger towers (kB). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT high interest payload EE +", 'description': "Payload of the high interest trigger towers (kB). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/14 SelectiveReadout/08 LowInterestPayload",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT low interest payload EE -", 'description': "Payload of the low interest trigger towers (kB). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT low interest payload EE +", 'description': "Payload of the low interest trigger towers (kB). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/14 SelectiveReadout/09 TowerSize",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT tower event size EE -", 'description': "Event size distribution per tower. This plot allows to verify the correctness of the ZS thresholds applied. The average of the plot has to be ~ 2kB / 68 ~ 30 bytes.<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT tower event size EE +", 'description': "Event size distribution per tower. This plot allows to verify the correctness of the ZS thresholds applied. The average of the plot has to be ~ 2kB / 68 ~ 30 bytes.<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eelayout(dqmitems, "00 By Task/14 SelectiveReadout/10 ZSFilterOutput HighInterest",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT high interest ZS filter output EE -", 'description': "crystal amplitude as reconstructed in DCC. This is useful for the emulation of the zero suppression filter. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT high interest ZS filter output EE +", 'description': "crystal amplitude as reconstructed in DCC. This is useful for the emulation of the zero suppression filter. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eelayout(dqmitems, "00 By Task/14 SelectiveReadout/11 ZSFilterOutput LowInterest",
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT low interest ZS filter output EE -", 'description': "crystal amplitude as reconstructed in DCC. This is useful for the emulation of the zero suppression filter. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EESelectiveReadoutTask/EESRT low interest ZS filter output EE +", 'description': "crystal amplitude as reconstructed in DCC. This is useful for the emulation of the zero suppression filter. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

EOF995

close LAYOUT;

