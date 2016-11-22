#!/usr/bin/env perl

$prefix = "EcalBarrel";

@Lasers = ("1","2","3","4");
@MGPAGains = ("01","06","12");
@MGPAGainsPN = ("01","16");

open LAYOUT, ">eb-layouts.py";

print LAYOUT <<EOF00;
def ecalbarrellayout(i, p, *rows): i["$prefix/Layouts/" + p] = DQMItem(layout=rows)
def ebshifterlayout(i, p, *rows): i["$prefix/Layouts/00 Shift/" + p] = DQMItem(layout=rows)
def ebecalshifterlayout(i, p, *rows): i["$prefix/Layouts/01 ECAL Shift/" + p] = DQMItem(layout=rows)
def eblayout(i, p, *rows): i["$prefix/Layouts/02 ECAL Expert Shift/" + p] = DQMItem(layout=rows)

# Quick Collections
ecalbarrellayout(dqmitems, "00 Global Summary EcalBarrel",
  [{ 'path': "$prefix/EBSummaryClient/EB global summary", 'description': "EcalBarrel: overall status of crystals.  Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/EcalDQM>EcalDQM</a>" }])

ecalbarrellayout(dqmitems, "01 Occupancy Summary EcalBarrel",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit thr occupancy", 'description': "EcalBarrel: occupancy of calibrated hits with energy > 1 GeV. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/EcalDQM>EcalDQM</a>" }])

ecalbarrellayout(dqmitems, "02 Cluster Summary EcalBarrel",
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC energy map", 'description': "EcalBarrel: average energy of hybrid basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/EcalDQM>EcalDQM</a>" }])

# ALL shifters
ebshifterlayout(dqmitems, "00 Event Type",
  [{ 'path': "$prefix/EcalInfo/EVTTYPE", 'description': "Frequency of the event types found in the DQM event-stream. If the calibration sequence is ON, histograms should show entries in COSMICS_GLOBAL, LASER_GAP, PEDESTAL_GAP, TESTPULSE_GAP. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

ebshifterlayout(dqmitems, "01 Integrity Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBIT integrity quality summary", 'description': "Quality summary checking that data for each crystal follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. Quality criteria: each crystal is required to have integrity errors in less than 1% of events. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

ebshifterlayout(dqmitems, "02 StatusFlags Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBSFT front-end status summary", 'description': "DCC front-end status quality summary. Expected all green color. Quality criteria: Each tower is required not to have front-end errors among the following: TIMEOUT, HEADER, CHANNEL ID, LINK, BLOCKSIZE, L1A SYNC, BX SYNC, L1A+BX SYNC, FIFO+L1A, H PARITY, v PARITY. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

ebshifterlayout(dqmitems, "03 PedestalOnline RMS",
  [{ 'path': "$prefix/EBSummaryClient/EBPOT pedestal G12 RMS map", 'description': "RMS of the pedestals in ADC counts. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (calibration and physics). Expected RMS for ECAL barrel is 1.1 ADC counts (43 MeV): almost blue color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

ebshifterlayout(dqmitems, "04 Timing Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBTMT timing quality summary", 'description': "Quality summary of the good crystals reconstructed hits with energy > 400 MeV. Hardware timing is adjusted with readout tower granularity, but finer setting can be reached. Expected all green color. Legend: green = good;  red = bad;  yellow = no sufficient statistics. Quality criteria: Average timing in each crystal for calibrated rechits with energy > 480 MeV, good DB status and rechit flag = kGood OR KOutOfTime is evaluated if more than 2 hits pass the above cuts. The following criteria are applied: -12.5 < mean < 12.5 ns and RMS < 62.5 ns. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

ebshifterlayout(dqmitems, "05 Occupancy Rechits",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit occupancy", 'description': "Map of the occupancy of ECAL calibrated reconstructed hits. Expect uniform color. Legend: the darkness of the blue scale is proportional to the average occupancy of the crystal. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit occupancy projection eta", 'description': "Eta projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBOccupancyTask/EBOT rec hit occupancy projection phi", 'description': "Phi projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

ebshifterlayout(dqmitems, "06 Occupancy Trigger Primitives",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT TP digi thr occupancy", 'description': "Map of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). Expect uniform color. Legend: the darkness of the blue scale is proportional to the average occupancy of the trigger tower. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT TP digi thr occupancy projection eta", 'description': "Eta projection of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBOccupancyTask/EBOT TP digi thr occupancy projection phi", 'description': "Phi projection of the occupancy of ECAL trigger primitives with energy > 4 ADC counts (~1 GeV). Expect uniform distribution. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

ebshifterlayout(dqmitems, "07 Clusters Energy",
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC energy map", 'description': "Average energy (in GeV) of hybrid basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC energy projection eta", 'description': "Eta projection of hybrid basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT BC energy projection phi", 'description': "Phi projection of hybrid basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

ebshifterlayout(dqmitems, "08 Blue Laser (L1) Quality",
  [{ 'path': "$prefix/EBSummaryClient/EBLT laser quality summary L1", 'description': "Quality summary of blue laser events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. Supermodules are filled as the calibration sequence reach them: expected all yellow at beginning of run, then becoming green half-supermodule by half-supermodule (i.e. 1 light module). Legend: green = good;  red = bad;  yellow = no entries. Quality criteria: mean amplitude > 100 ADCs; mean amplitude within 40% of the mean amplitude in the supermodule; RMS < 30% mean amplitude. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

ebshifterlayout(dqmitems, "08 Infrared Laser (L4) Quality",
  [{ 'path': "$prefix/EBSummaryClient/EBLT laser quality summary L4", 'description': "Quality summary of infrared laser events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. Supermodules are filled as the calibration sequence reach them: expected all yellow at beginning of run, then becoming green half-supermodule by half-supermodule (i.e. 1 light module). Legend: green = good;  red = bad;  yellow = no entries. Quality criteria: mean amplitude > 100 ADCs; mean amplitude within 40% of the mean amplitude in the supermodule; RMS < 30% mean amplitude. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

ebshifterlayout(dqmitems, "10 Pedestal Gain12 Quality",
  [{ 'path': "$prefix/EBSummaryClient/EBPT pedestal quality G12 summary", 'description': "Quality summary of pedestal events for Gain 12. Pedestals are evaluated using all the 10 samples. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. upermodules are filled as the calibration sequence reach them: expected all yellow at beginning of run, then becoming green supermodule by supermodule. Legend: green = good;  red = bad;  yellow = no entries. Quality criteria: 175 < mean < 225 ADCs; RMS < 2.0 ADCs. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

ebshifterlayout(dqmitems, "11 TestPulse Gain12 Quality",
  [{ 'path': "$prefix/EBSummaryClient/EBTPT test pulse quality G12 summary", 'description': "Quality summary of test pulse events for Gain 12. Expect green where the test pulse sequence fired, yellow elsewhere. Red spots are failed channels. Supermodules are filled as the calibration sequence reach them: expected all yellow at beginning of run, then becoming green. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

ebshifterlayout(dqmitems, "12 DCC EventSize",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT DCC event size", 'description': "Average event size per ecah ECAL barrel DCC calculated only on physics events. Expected about 2 kB/DCC. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>", 'draw': { 'withref': "yes" } }])

ebshifterlayout(dqmitems, "13 Selective Readout High Interest TTs",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT high interest TT Flags", 'description': "Rate of high interest (E_T of Trigger Tower over threshold) flags of selective readout. Expected higher occupancy at lower |eta| (the threshold is on transverse energy). Calibration events are excluded. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])


# ECAL shifters
ebecalshifterlayout(dqmitems, "00 Event Type",
  [{ 'path': "$prefix/EcalInfo/EVTTYPE", 'description': "Frequency of the event types found in the DQM event-stream. If the calibration sequence is ON, histograms should show entries in COSMICS_GLOBAL, LASER_GAP, PEDESTAL_GAP, TESTPULSE_GAP. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "01 Electronics/00 Integrity Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBIT integrity quality summary", 'description': "Quality summary checking that data for each crystal follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. Quality criteria: each crystal is required to have integrity errors in less than 1% of events. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "01 Electronics/01 Integrity Errors",
  [{ 'path': "$prefix/EBSummaryClient/EBIT integrity quality errors summary", 'description': "Crystals integrity error counter by supermodule. Expect 0 entries in every bin. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "01 Electronics/02 PN Integrity Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBIT PN integrity quality summary", 'description': "Quality summary checking that data for each PN diode follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries.  Quality criteria: each crystal is required to have integrity errors in less than 1% of events. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

ebecalshifterlayout(dqmitems, "01 Electronics/03 FrontEnd Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBSFT front-end status summary", 'description': "DCC front-end status quality summary. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. Quality criteria: Each tower is required not to have front-end errors among the following: TIMEOUT, HEADER, CHANNEL ID, LINK, BLOCKSIZE, L1A SYNC, BX SYNC, L1A+BX SYNC, FIFO+L1A, H PARITY, v PARITY. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "01 Electronics/04 CalibrationSequence Errors",
  [{ 'path': "$prefix/EBRawDataTask/EBRDT trigger type errors", 'description': "Number of trigger type mismatches between the global trigger and ECAL DCC header (). It is filled once per DCC with weight 1/36. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "02 Noise/00 PedestalOnline Quality",
  [{ 'path': "$prefix/EBSummaryClient/EBPOT pedestal quality summary G12", 'description': "pedestal quality summary. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (on physics events only). Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. Quality criteria: 175 < mean < 225 ADCs, RMS < 2 ADCs. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "02 Noise/01 PedestalOnline Rms",
  [{ 'path': "$prefix/EBSummaryClient/EBPOT pedestal G12 RMS map", 'description': "RMS of the pedestals in ADC counts. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (on physics events only). Expected RMS for ECAL barrel is 1.1 ADC counts (43 MeV). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "02 Noise/02 PedestalOnline FEDAverage",
  [{ 'path': "$prefix/EBSummaryClient/EBPOT pedestal G12 mean", 'description': "Average mean of the pedestals in ADC counts for each FED. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (on physics events only). Expected mean is 200 ADC counts.<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBSummaryClient/EBPOT pedestal G12 rms", 'description': "Average RMS of the pedestals in ADC counts for each FED. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (on physics events only). Expected rms is 1.1 ADC counts (43 MeV). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "03 Occupancy/00 Summary",
  [{ 'path': "$prefix/EcalInfo/EBMM DCC", 'description': "Occupancy of DCC. Non-uniform distribution in one bin means that the supermodule has not been readout in some event. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EcalInfo/EBMM digi number profile", 'description': "Average occupancy of digis per supermodule. Expect flat distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EcalInfo/EBMM hit number profile", 'description': "Average occupancy of reconstructed hits per supermodule. Expect flat distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EcalInfo/EBMM TP digi number profile", 'description': "Average occupancy of trigger primitives per supermodule. Expect flat distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "03 Occupancy/01 Digis",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT digi occupancy", 'description': "Map of the occupancy of ECAL digis. Expect uniform color. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT digi occupancy projection eta", 'description': "Eta projection of the occupancy of ECAL digis. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBOccupancyTask/EBOT digi occupancy projection phi", 'description': "Phi projection of the occupancy of ECAL digis. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "03 Occupancy/02 RecHits (Unfiltered)",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit occupancy", 'description': "Map of the occupancy of ECAL calibrated reconstructed hits. Expect uniform color. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit occupancy projection eta", 'description': "Eta projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBOccupancyTask/EBOT rec hit occupancy projection phi", 'description': "Phi projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "03 Occupancy/03 RecHits (Filtered)",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit thr occupancy", 'description': "Map of the occupancy of ECAL calibrated reconstructed hits. Only the good reconstructed hits with energy E > 300 MeV are considered. Expect uniform color. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit thr occupancy projection eta", 'description': "Eta projection of the occupancy of ECAL calibrated reconstructed hits. Only the good reconstructed hits with energy E > 300 MeV are considered. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBOccupancyTask/EBOT rec hit thr occupancy projection phi", 'description': "Phi projection of the occupancy of ECAL calibrated reconstructed hits. Expect uniform distribution. Only the good reconstructed hits with energy E > 300 MeV are considered. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "03 Occupancy/04 Trigger Primitives Digis (All)",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT TP digi occupancy", 'description': "Map of the occupancy of ECAL trigger primitives. Expect uniform color. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT TP digi occupancy projection eta", 'description': "Eta projection of the occupancy of ECAL trigger primitives. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBOccupancyTask/EBOT TP digi occupancy projection phi", 'description': "Phi projection of the occupancy of ECAL trigger primitives. Expect uniform distribution. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "03 Occupancy/05 Trigger Primitives (Threshold)",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT TP digi thr occupancy", 'description': "Map of the occupancy of ECAL trigger primitives with an E_T > 1 GeV (4 ADC counts). Darker regions mean noisy towers. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT TP digi thr occupancy projection eta", 'description': "Eta projection of the occupancy of ECAL trigger primitives with an E_T > 1 GeV (4 ADC counts). Expect uniform distribution, spikes mean noisy towers. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBOccupancyTask/EBOT TP digi thr occupancy projection phi", 'description': "Phi projection of the occupancy of ECAL trigger primitives with an E_T > 1 GeV (4 ADC counts). Expect uniform distribution, spikes mean noisy towers. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "03 Occupancy/06 TestPulseEvents",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT test pulse digi occupancy", 'description': "Occupancy of test pulse events only (if any in the calibration sequence). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

ebecalshifterlayout(dqmitems, "03 Occupancy/07 LaserEvents",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT laser digi occupancy", 'description': "Occupancy of laser events only (if any in the calibration sequence). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

ebecalshifterlayout(dqmitems, "03 Occupancy/09 PedestalEvents",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT pedestal digi occupancy", 'description': "Occupancy of pedestal events only (if any in the calibration sequence). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

ebecalshifterlayout(dqmitems, "03 Occupancy/10 PN Digis",
  [{ 'path': "$prefix/EBSummaryClient/EBOT PN digi occupancy summary", 'description': "Occupancy of PN diodes (expect entries only for calibration events). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

# to be put back in when we will remove cosmics summary
ebecalshifterlayout(dqmitems, "04 Energy/00 RecHit Energy",
  [{ 'path': "$prefix/EBSummaryClient/EBOT energy summary", 'description': "Average energy (in GeV) of good hits. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit spectrum", 'description': "Average energy (in GeV) of good hits. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "04 Energy/01 Clusters Energy",
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC energy", 'description': "Calibrated energy (GeV) of hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT SC seed crystal energy", 'description': "Calibrated energy (GeV) of the crystals seeding a cluster. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "04 Energy/02 Basic Clusters",
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC number", 'description': "Multiplicity of reconstructed hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC size", 'description': "Numbers of crystals contained in hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "04 Energy/03 Basic Clusters Occupancy",
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC number map", 'description': "Average number of basic cluster / event. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC number projection eta", 'description': "Eta projection of average number of basic cluster / event. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT BC number projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "04 Energy/04 Basic Clusters Energy",
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC energy map", 'description': "Average energy (in GeV) of hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC energy projection eta", 'description': "Eta projection of hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT BC energy projection phi", 'description': "Phi projection of hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "04 Energy/05 Basic Clusters Transverse Energy",
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC ET map", 'description': "Average transverse energy (in GeV) of hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC ET projection eta", 'description': "Average transverse energy (in GeV) of hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT BC ET projection phi", 'description': "Average transverse energy (in GeV) of hybrid basic clusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "04 Energy/06 Basic Clusters Size",
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC size map", 'description': "Number of crystals constituent the basic cluster. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC size projection eta", 'description': "Number of crystals constituent the basic cluster. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT BC size projection phi", 'description': "Number of crystals constituent the basic cluster. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "04 Energy/07 Basic Cluster Shape",
  [{ 'path': "$prefix/EBClusterTask/EBCLT s1s9", 'description': "Ratio of E(max)/E(3x3) of all basic clusters. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT s1s9 thr", 'description': "Ratio of E(max)/E(3x3) of basic clusters with a seed crystal with E>3.0 GeV. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "04 Energy/08 SuperClusters 1D",
  [{ 'path': "$prefix/EBClusterTask/EBCLT SC number", 'description': "Multiplicity of reconstructed hybrid superclusters. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT SC size (crystal)", 'description': "Numbers of crystals contained in hybrid superclusters.  Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "04 Energy/09 Peaks",
  [{ 'path': "$prefix/EBClusterTask/EBCLT dicluster invariant mass Pi0 sel", 'description': "Invariant mass of 2 ECAL supercluster. Complete combinatorics is shown of clusters with ET>1.0 GeV, low sigmaIEtaIEta, and Pt(candidate)>1.5 GeV. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT dicluster invariant mass JPsi sel", 'description': "Invariant mass of 2 ECAL supercluster. Complete combinatorics is shown of clusters with ET>1.0 GeV, low sigmaIEtaIEta, and Pt(candidate)>1.5 GeV. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT dicluster invariant mass Z0 sel", 'description': "Invariant mass of 2 ECAL supercluster. Complete combinatorics is shown of clusters with ET>1.0 GeV, low sigmaIEtaIEta, and Pt(candidate)>1.5 GeV. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT dicluster invariant mass high sel", 'description': "Invariant mass of 2 ECAL supercluster. Complete combinatorics is shown of clusters with ET>1.0 GeV, low sigmaIEtaIEta, and Pt(candidate)>1.5 GeV. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "05 Timing/00 Timing Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBTMT timing quality summary", 'description': "Quality summary of good calibrated reconstructed hits. Hardware timing is adjusted with readout tower granularity, but finer setting can be reached. Expected all green color. Legend: green = good;  red = bad;  yellow = no sufficient statistics. Quality criteria: Average timing in each crystal for calibrated rechits with energy > 480 MeV, good DB status and rechit flag = kGood OR KOutOfTime is evaluated if more than 2 hits pass the above cuts. The following criteria are applied: -12.5 < mean < 12.5 ns and RMS < 62.5 ns. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcal>DQMShiftEcal</a>" }])

ebecalshifterlayout(dqmitems, "05 Timing/01 Timing Mean",
  [{ 'path': "$prefix/EBSummaryClient/EBTMT timing mean 1D summary", 'description': "Mean timing of all the channels in EB along the run. Calibrated rechits are used with energy > 480 MeV, good DB status and rechit flag = kGood OR KOutOfTime. Timing is expected centered 0 ns. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBSummaryClient/EBTMT timing mean", 'description': "Mean timing of all the channels in each DCC of EB along the run. Calibrated rechits are used with energy > 480 MeV, good DB status and rechit flag = kGood OR KOutOfTime. Timing is expected centered 0 ns. The error bar represents the spreads among the crystal of each DCC. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "05 Timing/02 Timing RMS",
  [{ 'path': "$prefix/EBSummaryClient/EBTMT timing rms 1D summary", 'description': "Average timing RMS along the run of all the channels in EB. Timing RMS is expected < 10 ns. Calibrated rechits are used with energy > 480 MeV, good DB status and rechit flag = kGood OR KOutOfTime. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBSummaryClient/EBTMT timing rms", 'description': "Average timing RMS along the run of all the channels in each DCC of EB. Calibrated rechits are used with energy > 480 MeV, good DB status and rechit flag = kGood OR KOutOfTime. The error bar represents the spreads among the crystal of each DCC. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "05 Timing/03 Timing Map",
  [{ 'path': "$prefix/EBTimingTask/EBTMT timing map", 'description': "Average timing of the good calibrated rechits. Expected about 0 ns. Calibrated rechits are used with energy > 480 MeV, good DB status and rechit flag = kGood OR KOutOfTime. Readout tower binning (5x5 crystals) is used. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBTimingTask/EBTMT timing projection eta", 'description': "Average timing of the good rechits. Expected about 0 ns. Calibrated rechits are used with energy > 480 MeV, good DB status and rechit flag = kGood OR KOutOfTime. Readout tower binning (5x5 crystals) is used. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBTimingTask/EBTMT timing projection phi", 'description': "Average timing of the good rechits. Expected about 0 ns. Calibrated rechits are used with energy > 480 MeV, good DB status and rechit flag = kGood OR KOutOfTime. Readout tower binning (5x5 crystals) is used. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "05 Timing/04 Timing Vs Amplitude",
  [{ 'path': "$prefix/EBTimingTask/EBTMT timing vs amplitude summary", 'description': "Average timing of the good rechits vs. amplitude. Expected about 0 ns timing for amplitudes above 480 MeV. Calibrated rechits are used with good DB status and rechit flag = kGood OR KOutOfTime. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [None])

ebecalshifterlayout(dqmitems, "05 Timing/04 Single Event Timing",
  [{ 'path': "$prefix/EBTimingTask/EBTMT timing 1D summary", 'description': "Single event timing of the good rechits. Expected centered around 0 ns. Calibrated rechits are used with energy > 480 MeV, good DB status and rechit flag = kGood OR KOutOfTime. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [None])

ebecalshifterlayout(dqmitems, "06 L1 Trigger/00 TP Spectra",
  [{ 'path': "$prefix/EBTriggerTowerTask/EBTTT Et spectrum Real Digis", 'description': "Average transverse energy (4 ADC count = 1 GeV) of real L1 trigger primitives. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBTriggerTowerTask/Emulated/EBTTT Et spectrum Emulated Digis", 'description': "Average transverse energy (4 ADC count = 1 GeV) of emulated L1 trigger primitives. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBTriggerTowerTask/Emulated/EBTTT Et spectrum Emulated Digis max", 'description': "Average transverse energy (4 ADC count = 1 GeV) of emulated L1 trigger primitives which gives the max Et. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "06 L1 Trigger/01 TriggerPrimitives ET",
  [{ 'path': "$prefix/EBSummaryClient/EBTTT Et trigger tower summary", 'description': "Average transverse energy of real L1 trigger primitives (4 ADC count = 1 GeV). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "06 L1 Trigger/02 Trigger Primitives Occupancy (Threshold)",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT TP digi thr occupancy", 'description': "Map of the occupancy of ECAL trigger primitives with an E_T > 1 GeV (4 ADC counts). Darker regions mean noisy towers. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT TP digi thr occupancy projection eta", 'description': "Eta projection of the occupancy of ECAL trigger primitives with an E_T > 1 GeV (4 ADC counts). Expect uniform distribution, spikes mean noisy towers. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBOccupancyTask/EBOT TP digi thr occupancy projection phi", 'description': "Phi projection of the occupancy of ECAL trigger primitives with an E_T > 1 GeV (4 ADC counts). Expect uniform distribution, spikes mean noisy towers. Physics events only. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "06 L1 Trigger/03 TP data matching Emulator",
  [{ 'path': "$prefix/EBTriggerTowerTask/EBTTT TP matching index", 'description': "Sample of the emulated TP that matches the real TP. -1 means no match. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTriggerTowerTask/EBTTT max TP matching index", 'description': "Sample of the emulated TP that givest the largest ET. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "06 L1 Trigger/04 Trigger Most Frequent Timing",
  [{ 'path': "$prefix/EBSummaryClient/EBTTT Trigger Primitives Timing summary", 'description': "Sample of the emulated TP that more often matches the real TP. Matched sample appear in non-red colors. Match with on-time primitives appear yellow (expected). No match at all appears red. No events appear white. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "06 L1 Trigger/05 Fraction of Non Single Timing",
  [{ 'path': "$prefix/EBSummaryClient/EBTTT Trigger Primitives Non Single Timing summary", 'description': "Fraction of events where the TP has a matching with emulator different by the most probable (expected empty). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "06 L1 Trigger/06 TP Match index Calo or Muon Triggers",
  [{ 'path': "$prefix/EBTriggerTowerTask/EBTTT TCC timing calo triggers Real Digis", 'description': "TP sample matching with emulator divided by TCC number (in EB 1 TCC = 1 DCC). Expect all TCCs at sample = 3. Only events with HLT bit = HLT_EgammaSuperClusterOnly_L1R are considered. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTriggerTowerTask/EBTTT TCC timing muon triggers Real Digis", 'description': "TP sample matching with emulator divided by TCC number (in EB 1 TCC = 1 DCC). Expect all TCCs at sample = 3. Only events with HLT bit = HLT_L1MuOpen are considered. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "06 L1 Trigger/07 Trigger Quality",
  [{ 'path': "$prefix/EBSummaryClient/EBTTT emulator error quality summary", 'description': "Result of the comparison of Et and Fine Grain Veto between real and emulated trigger primitives. Non matched trigger primitives appear red. Expect green color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "06 L1 Trigger/08 TTF mismatch",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT TT flag mismatch", 'description': "Occupancy of trigger primitives of high or medium interest which have not been fully readout. Expected empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "06 L1 Trigger/09 Trigger Timing Occupancy",
  [{ 'path': "$prefix/EBTriggerTowerTask/EBTTT Et vs bx Real Digis", 'description': "Average Et of trigger primitives with Et>0 vs the bunch crossing number. Finer binning is used around the abort gap and at the beginning of the physics gap, larger in the middle of the orbit. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTriggerTowerTask/EBTTT TP occupancy vs bx Real Digis", 'description': "Average number of trigger primitives with Et>0 vs the bunch crossing number. Finer binning is used around the abort gap and at the beginning of the physics gap, larger in the middle of the orbit. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "07 Selective Readout/00 EventSize",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT event size", 'description': "Event size for the readout crystals in the ECAL barrel. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT tower event size", 'description': "Event size distribution per tower. This plot allows to verify the correctness of the ZS thresholds applied. The average of the plot has to be ~ 2kB / 68 ~ 30 bytes.<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }] )

ebecalshifterlayout(dqmitems, "07 Selective Readout/01 DCC EventSize",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT DCC event size", 'description': "Average event size per ecah ECAL barrel DCC. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT event size vs DCC", 'description': "Event size distribution per each ECAL barrel DCC. Finer binning is used in the zero-suppressed region (0-0.608 kB), while granularity of one fully readout tower (0.608 kB) has been used for higher sizes. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>"}])

ebecalshifterlayout(dqmitems, "07 Selective Readout/02 SRFlags",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT readout unit with SR forced", 'description': "Rate of readout units with Selective Readout Forced. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT full readout SR Flags", 'description': "Rate of full readout Selective Readout flags. The selective readout of the ECAL was designed to reduce the ECAL data size by a factor 20. If any tower is fully read out more than 5% of the times, this tower will increase the payload and introduce deadtime. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "07 Selective Readout/03 TTFlags",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT high interest TT Flags", 'description': "Rate of high interest Trigger Tower flags. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT low interest TT Flags", 'description': "Rate of low interest Trigger Tower flags. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "07 Selective Readout/04 Payload",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT high interest payload", 'description': "Payload of the high interest trigger towers (kB). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT low interest payload", 'description': "Payload of the low interest trigger towers (kB). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

ebecalshifterlayout(dqmitems, "07 Selective Readout/05 ZSFilterOutput",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT high interest ZS filter output", 'description': "crystal amplitude as reconstructed in DCC. This is useful for the emulation of the zero suppression filter. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT low interest ZS filter output", 'description': "crystal amplitude as reconstructed in DCC. This is useful for the emulation of the zero suppression filter. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

EOF00

foreach $laser (@Lasers) {

    $color = "None";
    if($laser==1) { $color = "Blue"; }
    if($laser==2) { $color = "Green"; }
    if($laser==3) { $color = "Red"; }
    if($laser==4) { $color = "Infrared"; }
    
print LAYOUT <<EOF01;

ebecalshifterlayout(dqmitems, "08 Calibration/00 Laser/00 $color Laser (L$laser) Quality",
  [{ 'path': "$prefix/EBSummaryClient/EBLT laser quality summary L$laser", 'description': "Quality summary of $color laser events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. Quality criteria: mean amplitude > 100 ADCs; mean amplitude within 40% of the mean amplitude in the supermodule; RMS < 30% mean amplitude. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "08 Calibration/00 Laser/01 $color Laser (L$laser) Errors",
  [{ 'path': "$prefix/EBSummaryClient/EBLT laser quality errors summary L$laser", 'description': "Errors occupancy for $color laser events for every supermodule. Expect 0 entries if no errors or no laser events in a given supermodule. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "08 Calibration/00 Laser/02 $color Laser (L$laser) Amplitude",
  [{ 'path': "$prefix/EBSummaryClient/EBLT laser L$laser amplitude summary", 'description': "$color laser average amplitude for fired light modules. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBSummaryClient/EBLT laser L$laser amplitude over PN summary", 'description': "$color laser APD/PN ratio for fired light modules. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "08 Calibration/00 Laser/03 $color Laser (L$laser) Timing",
  [{ 'path': "$prefix/EBSummaryClient/EBLT laser L$laser timing summary", 'description': "$color laser average timing (in pulse shape samples) for fired light modules. Expect value around 6.5. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

ebecalshifterlayout(dqmitems, "08 Calibration/00 Laser/04 $color Laser (L$laser) PNQuality",
  [{ 'path': "$prefix/EBSummaryClient/EBLT PN laser quality summary L$laser", 'description': "Quality summary of $color laser events on PN diodes. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed PNs. Legend: green = good;  red = bad;  yellow = no entries. Quality criteria: amplitude (G1 and G16) > 100 ADCs; 650 < PN pedestal < 850 ADCs;  PN RMS < 10 ADCs. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBSummaryClient/EBLT PN laser quality errors summary L$laser", 'description': "Error occupancy for laser L$laser events on PN diodes for every supermodule. Expect 0 entries if no errors or no laser events in a given supermodule. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])
EOF01

}

foreach $gain (@MGPAGains) {

print LAYOUT <<EOF02;

ebecalshifterlayout(dqmitems, "08 Calibration/01 Pedestal/00 Quality Gain$gain",
  [{ 'path': "$prefix/EBSummaryClient/EBPT pedestal quality G$gain summary", 'description': "Quality summary of pedestal events on crystals for Gain $gain. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. Quality criteria: gain 1: 175 < mean < 225 ADCs; RMS < 1.0 ADCs; gain 6: 175 < mean < 225 ADCs; RMS < 1.2 ADCs; gain 12: 175 < mean < 225 ADCs; RMS < 2.0 ADCs. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])
EOF02

}

foreach $gainPN (@MGPAGainsPN) {

print LAYOUT <<EOF03;

ebecalshifterlayout(dqmitems, "08 Calibration/01 Pedestal/01 PNQuality Gain$gainPN",
  [{ 'path': "$prefix/EBSummaryClient/EBPT PN pedestal quality G$gainPN summary", 'description': "Quality summary of pedestal events on PN diodes for Gain $gainPN. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed PNs. Legend: green = good;  red = bad;  yellow = no entries.  Quality criteria for gain 1 and gain 16: 650 < mean < 850 ADCs; RMS < 10 ADCs. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])
EOF03

}

foreach $gain (@MGPAGains) {

print LAYOUT <<EOF04;

ebecalshifterlayout(dqmitems, "08 Calibration/02 TestPulse/00 Quality Gain$gain",
  [{ 'path': "$prefix/EBSummaryClient/EBTPT test pulse quality G$gain summary", 'description': "Crystals quality summary of test pulse events for Gain $gain. Expect green where the test pulse sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries.  *Quality criteria*: mean amplitude > 100 !ADCs; mean amplitude within 20% of the mean amplitude in the supermodule; !RMS < 300 ADCs. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "08 Calibration/02 TestPulse/01 Amplitude Gain$gain",
  [{ 'path': "$prefix/EBSummaryClient/EBTPT test pulse amplitude G$gain summary", 'description': "Test pulse average amplitude in MGPA gain $gain for crystals for each EB superodule. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])
EOF04

}

foreach $gainPN (@MGPAGainsPN) {

print LAYOUT <<EOF05;

ebecalshifterlayout(dqmitems, "08 Calibration/02 TestPulse/05 PNQuality Gain$gainPN",
  [{ 'path': "$prefix/EBSummaryClient/EBTPT PN test pulse quality G$gainPN summary", 'description': "PN diodes quality summary of test pulse events for Gain $gainPN. Expect green where the test pulse sequence fired, yellow elsewhere. Red spots are failed PNs. Legend: green = good;  red = bad;  yellow = no entries. Quality criteria for gain 1 (gain 16): amplitude > 12.5 (200) ADCs; 650 < pedestal mean < 850 ADCs; pedestal RMS < 10 ADCs. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])
EOF05

}

print LAYOUT <<EOFTREND;

ebecalshifterlayout(dqmitems, "09 Trend/00 Crystal Digis Number",
  [{ 'path': "$prefix/EBTrendTask/AverageNumberOfEBDigiVs5Minutes", 'description': "Average number of barrel crystal digis (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTrendTask/AverageNumberOfEBDigiVs1Hour", 'description': "Average number of barrel crystal digis (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])
   
ebecalshifterlayout(dqmitems, "09 Trend/01 PN Diodes Digis Number",
  [{ 'path': "$prefix/EBTrendTask/AverageNumberOfEcalPnDiodeDigiVs5Minutes", 'description': "Average number of barrel PN diodes digis (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTrendTask/AverageNumberOfEcalPnDiodeDigiVs1Hour", 'description': "Average number of barrel PN diodes digis (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "09 Trend/02 Hit with Threshold Occupancy",
  [{ 'path': "$prefix/EBTrendClient/Average of EBOT rec hit thr occupancy Vs 5Minutes", 'description': "Average occupancy for barrel reconstructed hits with energy > 1 GeV and good quality, i.e. noisy channels removed. (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTrendClient/Average of EBOT rec hit thr occupancy Vs 1Hour", 'description': "Average occupancy for barrel reconstructed hits with energy > 1 GeV and good quality, i.e. noisy channels removed. (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "09 Trend/03 TP Digis Occupancy",
  [{ 'path': "$prefix/EBTrendClient/Average of EBOT TP digi thr occupancy Vs 5Minutes", 'description': "Average occupancy of TP digis with ET >~ 1 GeV (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTrendClient/Average of EBOT TP digi thr occupancy Vs 1Hour", 'description': "Average occupancy of TP digis with ET >~ 1 GeV (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "09 Trend/04 Basic Clusters Number",
  [{ 'path': "$prefix/EBTrendTask/AverageNumberOfBasicClusterSizeVs5Minutes", 'description': "Average number of basic clusters (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTrendTask/AverageNumberOfBasicClusterSizeVs1Hour", 'description': "Average number of basic clusters (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "09 Trend/05 Basic Cluster Energy",
  [{ 'path': "$prefix/EBTrendClient/Average of EBCLT BC energy Vs 5Minutes", 'description': "Average energy of reconstructed barrel basic clusters. (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTrendClient/Average of EBCLT BC energy Vs 1Hour", 'description': "Average energy of reconstructed barrel basic clusters. (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "09 Trend/06 Super Clusters Number",
  [{ 'path': "$prefix/EBTrendTask/AverageNumberOfSuperClusterSizeVs5Minutes", 'description': "Average number of super clusters (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTrendTask/AverageNumberOfSuperClusterSizeVs1Hour", 'description': "Average number of super clusters (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "09 Trend/07 Super Cluster Energy",
  [{ 'path': "$prefix/EBTrendClient/Average of EBCLT SC energy Vs 5Minutes", 'description': "Average energy of reconstructed barrel basic clusters. (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTrendClient/Average of EBCLT SC energy Vs 1Hour", 'description': "Average energy of reconstructed barrel basic clusters. (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "09 Trend/08 Pedestal Mean",
  [{ 'path': "$prefix/EBTrendClient/Average of EBPOT pedestal G12 mean Vs 5Minutes", 'description': "Average pedestal mean in barrel crystals (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTrendClient/Average of EBPOT pedestal G12 mean Vs 1Hour", 'description': "Average pedestal mean in barrel crystals (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "09 Trend/09 Pedestal RMS",
  [{ 'path': "$prefix/EBTrendClient/Average of EBPOT pedestal G12 rms Vs 5Minutes", 'description': "Average pedestal rms in barrel crystals (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTrendClient/Average of EBPOT pedestal G12 rms Vs 1Hour", 'description': "Average pedestal rms in barrel crystals (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "09 Trend/10 Integrity Errors Number",
  [{ 'path': "$prefix/EBTrendTask/AverageNumberOfIntegrityErrorVs5Minutes", 'description': "Average number of integrity errors (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTrendTask/AverageNumberOfIntegrityErrorVs1Hour", 'description': "Average number of integrity errors (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "09 Trend/11 Barrel FEDs Size ",
  [{ 'path': "$prefix/EBTrendTask/AverageNumberOfFEDEBRawDataVs5Minutes", 'description': "Average barrel FED size, in kBytes. (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTrendTask/AverageNumberOfFEDEBRawDataVs1Hour", 'description': "Average barrel FED size, in kBytes. (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

ebecalshifterlayout(dqmitems, "09 Trend/12 Selective Readout Flags Size ",
  [{ 'path': "$prefix/EBTrendTask/AverageNumberOfEBSRFlagVs5Minutes", 'description': "Average number of Selective Readout Flags. (5 minutes range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTrendTask/AverageNumberOfEBSRFlagVs1Hour", 'description': "Average number of Selective Readout Flags. (1 hour range). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

EOFTREND

# detailed layouts
for (my $i=1; $i<=36; $i++ ) {

my $eb1;
my $eb2;

if ($i>= 1 && $i<=18) {
#  $eb1=sprintf("EB-/EB-%02d",$i);
  $eb1=sprintf("EB-%02d",$i);
  $eb2=sprintf("EB-%02d",$i);
}

if ($i>=19 && $i<=36) {
#  $eb1=sprintf("EB+/EB+%02d",$i-18);
  $eb1=sprintf("EB+%02d",$i-18);
  $eb2=sprintf("EB+%02d",$i-18);
}

print LAYOUT <<EOF06;

eblayout(dqmitems, "01 By Supermodule/$eb1/00 Integrity (Channel Global)",
  [{ 'path': "$prefix/EBIntegrityClient/EBIT data integrity quality $eb2", 'description': "uality summary checking that data for each crystal follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT digi occupancy $eb2", 'description': "Occupancy of ECAL digis. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "01 By Supermodule/$eb1/01 Integrity (Mem Box)",
  [{ 'path': "$prefix/EBIntegrityClient/EBIT data integrity quality MEM $eb2", 'description': "Quality summary checking that data for each PNs follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT MEM digi occupancy $eb2", 'description': "Occupancy of ECAL PN digis. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "01 By Supermodule/$eb1/02 PedestalOnline",
         [{ 'path': "$prefix/EBPedestalOnlineClient/EBPOT pedestal quality G12 $eb2", 'description': "Quality summary checking the quality of the pedestals from the first 3/10 samples of the pulse shape for all the events. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBPedestalOnlineClient/EBPOT pedestal mean G12 $eb2", 'description': "Mean of the pedestal value on all the events for the crystals of supermodule $eb2. Expected within 175-225 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBPedestalOnlineClient/EBPOT pedestal rms G12 $eb2", 'description': "RMS of the pedestals in ADC counts. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (calibration and physics). Expected RMS for ECAL barrel is 1.1 ADC counts (43 MeV). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/03 Timing",
  [{ 'path': "$prefix/EBTimingClient/EBTMT timing quality $eb2", 'description': "Quality summary checking the goodness of the pulse shape timing. It needs at least 10 events with amplitude over threshold (12 ADC counts) to apply quality flag. Otherwise it is yellow. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTimingClient/EBTMT timing mean $eb2", 'description': "Mean timing of the channels in $eb2. Timing is expected within 5.5 - 6.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBTimingClient/EBTMT timing rms $eb2", 'description': "Timing RMS of the channels in $eb2. Timing RMS is expected < 0.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/04 Trigger",
  [{ 'path': "$prefix/EBTriggerTowerTask/EBTTT EmulError $eb2", 'description': "Mismatches between hardware L1 trigger and software emulator. Currently only ET value of the Trigger Primitive is checked. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTriggerTowerTask/EBTTT Et map Real Digis $eb2", 'description': "Average transverse energy (ADC counts: 4 ADCs = 1 GeV) for real trigger primitives. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "01 By Supermodule/$eb1/05 Trigger",
  [{ 'path': "$prefix/EBTriggerTowerClient/EBTTT Trigger Primitives Timing $eb2", 'description': "Sample of the emulated TP that more often matches the real TP. Matched sample appear in magenta. No match at all appears red. No events appear white. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTriggerTowerClient/EBTTT Non Single Timing $eb2", 'description': "Fraction of events where the TP has a matching with emulator different by the most probable (expected empty). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "01 By Supermodule/$eb1/06 Energy",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit energy $eb2", 'description': "Average energy of the rechits selected among good (rechit quality flag and channel status) reconstructed hits. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT energy spectrum $eb2", 'description': "Calibrated energy spectrum of the good reconstructed hits (rechit quality flag and channel status). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/07 StatusFlags",
  [{ 'path': "$prefix/EBStatusFlagsTask/FEStatus/EBSFT front-end status $eb2", 'description': "Occupancy of DCC front-end status errors. Status ACTIVE (i.e. full-readout) or SUPPRESSED (i.e. zero-suppression applied) are not considered as errors and do not create entries in this plot. Expected all empty.  <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBStatusFlagsTask/FEStatus/EBSFT MEM front-end status $eb2", 'description': "Occupancy of DCC front-end errors for the MEM boxes (TTs: 69-70). Status ACTIVE (i.e. full-readout) or SUPPRESSED (i.e. zero-suppression applied) are not considered as errors and do not create entries in this plot. Expected all empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }]) 

eblayout(dqmitems, "01 By Supermodule/$eb1/08 StatusFlags",
  [{ 'path': "$prefix/EBStatusFlagsTask/FEStatus/EBSFT front-end status bits $eb2", 'description': "Occupancy of DCC front-end statuses by type. ACTIVE and SUPPRESSED are normal entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None]) 

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/00 Integrity/00 Channel Integrity",
  [{ 'path': "$prefix/EBIntegrityTask/Gain/EBIT gain $eb2", 'description': "Occupancy of integrity errors of type: GAIN ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBIntegrityTask/ChId/EBIT ChId $eb2", 'description': "Occupancy of integrity errors of type: CHANNEL ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/00 Integrity/01 Gain Integrity",
  [{ 'path': "$prefix/EBIntegrityTask/GainSwitch/EBIT gain switch $eb2", 'description': "Occupancy of integrity errors of type: GAIN SWITCH. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/00 Integrity/02 TT Integrity",
  [{ 'path': "$prefix/EBIntegrityTask/TTId/EBIT TTId $eb2", 'description': "Occupancy of integrity errors of type: TRIGGER TOWER ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBIntegrityTask/TTBlockSize/EBIT TTBlockSize $eb2", 'description': "Occupancy of integrity errors of type: TRIGGER TOWER BLOCK SIZE. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/00 Integrity/03 MemBox Channel Integrity",
  [{ 'path': "$prefix/EBIntegrityTask/MemChId/EBIT MemChId $eb2", 'description': "Occupancy of errors for PNs of type: MEM CHANNEL ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBIntegrityTask/MemGain/EBIT MemGain $eb2", 'description': "Occupancy of errors for PNs of type: MEM GAIN ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/00 Integrity/04 MemBox TT Integrity",
  [{ 'path': "$prefix/EBIntegrityTask/MemTTId/EBIT MemTTId $eb2", 'description': "Occupancy of integrity errors for PNs of type: TRIGGER TOWER ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBIntegrityTask/MemSize/EBIT MemSize $eb2", 'description': "Occupancy of errors for PNs of type: MEM SIZE. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/01 Pedestal/00 Gain01",
  [{ 'path': "$prefix/EBPedestalClient/EBPT pedestal quality G01 $eb2", 'description': "Quality histogram checking the quality of the pedestals from the 10 samples of the pulse shape for the pedestal events in Gain 1. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBPedestalClient/EBPT pedestal mean G01 $eb2", 'description': "Mean of the pedestal value on the pedestal events for the crystals of supermodule $eb2. Expected within 175-225 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBPedestalClient/EBPT pedestal rms G01 $eb2", 'description': "RMS of the pedestal value on the pedestal events for the crystals of supermodule $eb2. Expected < 2.0 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/01 Pedestal/01 Gain06",
  [{ 'path': "$prefix/EBPedestalClient/EBPT pedestal quality G06 $eb2", 'description': "Quality histogram checking the quality of the pedestals from the 10 samples of the pulse shape for the pedestal events in Gain 6. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBPedestalClient/EBPT pedestal mean G06 $eb2", 'description': "Mean of the pedestal value on the pedestal events for the crystals of supermodule $eb2. Expected within 175-225 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBPedestalClient/EBPT pedestal rms G06 $eb2", 'description': "RMS of the pedestal value on the pedestal events for the crystals of supermodule $eb2. Expected < 2.0 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/01 Pedestal/02 Gain12",
  [{ 'path': "$prefix/EBPedestalClient/EBPT pedestal quality G12 $eb2", 'description': "Quality histogram checking the quality of the pedestals from the 10 samples of the pulse shape for the pedestal events in Gain 12. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBPedestalClient/EBPT pedestal mean G12 $eb2", 'description': "Mean of the pedestal value on the pedestal events for the crystals of supermodule $eb2. Expected within 175-225 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBPedestalClient/EBPT pedestal rms G12 $eb2", 'description': "RMS of the pedestal value on the pedestal events for the crystals of supermodule $eb2. Expected < 2.0 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/01 Pedestal/03 PNs Quality",
  [{ 'path': "$prefix/EBPedestalClient/EBPT pedestal quality PNs G01 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBPedestalClient/EBPT pedestal quality PNs G16 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/01 Pedestal/04 PNs Response",
  [{ 'path': "$prefix/EBPedestalTask/PN/Gain01/EBPDT PNs pedestal $eb2 G01", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBPedestalClient/EBPDT PNs pedestal rms $eb2 G01", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBPedestalTask/PN/Gain16/EBPDT PNs pedestal $eb2 G16", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBPedestalClient/EBPDT PNs pedestal rms $eb2 G16", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/02 TestPulse/00 Gain01",
  [{ 'path': "$prefix/EBTestPulseClient/EBTPT test pulse quality G01 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTestPulseClient/EBTPT test pulse amplitude G01 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBTestPulseClient/EBTPT test pulse shape G01 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/02 TestPulse/01 Gain06",
  [{ 'path': "$prefix/EBTestPulseClient/EBTPT test pulse quality G06 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTestPulseClient/EBTPT test pulse amplitude G06 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBTestPulseClient/EBTPT test pulse shape G06 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/02 TestPulse/02 Gain12",
  [{ 'path': "$prefix/EBTestPulseClient/EBTPT test pulse quality G12 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTestPulseClient/EBTPT test pulse amplitude G12 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBTestPulseClient/EBTPT test pulse shape G12 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/02 TestPulse/03 PNs Quality",
  [{ 'path': "$prefix/EBTestPulseClient/EBTPT test pulse quality PNs G01 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTestPulseClient/EBTPT test pulse quality PNs G16 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/02 TestPulse/04 PNs Gain01",
  [{ 'path': "$prefix/EBTestPulseTask/PN/Gain01/EBTPT PNs pedestal $eb2 G01", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBTestPulseClient/EBTPT PNs pedestal rms $eb2 G01", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBTestPulseTask/PN/Gain01/EBTPT PNs amplitude $eb2 G01", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/02 TestPulse/05 PNs Gain01",
  [{ 'path': "$prefix/EBTestPulseTask/PN/Gain16/EBTPT PNs pedestal $eb2 G16", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBTestPulseClient/EBTPT PNs pedestal rms $eb2 G16", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBTestPulseTask/PN/Gain16/EBTPT PNs amplitude $eb2 G16", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/03 Laser/00 Laser",
  [{ 'path': "$prefix/EBLaserClient/EBLT laser quality L1 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBLaserClient/EBLT laser quality L4 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/03 Laser/01 Laser",
  [{ 'path': "$prefix/EBLaserClient/EBLT laser quality L1 PNs G01 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" },
   { 'path': "$prefix/EBLaserClient/EBLT laser quality L1 PNs G16 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBLaserClient/EBLT laser quality L4 PNs G01 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" },
   { 'path': "$prefix/EBLaserClient/EBLT laser quality L4 PNs G16 $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/03 Laser/L1/00 Laser",
  [{ 'path': "$prefix/EBLaserClient/EBLT amplitude L1A $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT amplitude over PN L1A $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBLaserClient/EBLT amplitude L1B $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT amplitude over PN L1B $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/03 Laser/L1/01 Laser",
  [{ 'path': "$prefix/EBLaserClient/EBLT laser timing L1A $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT laser shape L1A $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBLaserClient/EBLT laser timing L1B $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT laser shape L1B $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/03 Laser/L1/02 Laser",
  [{ 'path': "$prefix/EBLaserTask/Laser1/PN/Gain01/EBLT PNs pedestal $eb2 G01 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT PNs pedestal rms $eb2 G01 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBLaserTask/Laser1/PN/Gain01/EBLT PNs amplitude $eb2 G01 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/03 Laser/L1/03 Laser",
  [{ 'path': "$prefix/EBLaserTask/Laser1/PN/Gain16/EBLT PNs pedestal $eb2 G16 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT PNs pedestal rms $eb2 G16 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBLaserTask/Laser1/PN/Gain16/EBLT PNs amplitude $eb2 G16 L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/03 Laser/L4/00 Laser",
  [{ 'path': "$prefix/EBLaserClient/EBLT amplitude L4A $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT amplitude over PN L4A $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBLaserClient/EBLT amplitude L4B $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT amplitude over PN L4B $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/03 Laser/L4/01 Laser",
  [{ 'path': "$prefix/EBLaserClient/EBLT laser timing L4A $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT laser shape L4A $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBLaserClient/EBLT laser timing L4B $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT laser shape L4B $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/03 Laser/L4/02 Laser",
  [{ 'path': "$prefix/EBLaserTask/Laser4/PN/Gain01/EBLT PNs pedestal $eb2 G01 L4", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT PNs pedestal rms $eb2 G01 L4", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBLaserTask/Laser4/PN/Gain01/EBLT PNs amplitude $eb2 G01 L4", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/03 Laser/L4/03 Laser",
  [{ 'path': "$prefix/EBLaserTask/Laser4/PN/Gain16/EBLT PNs pedestal $eb2 G16 L4", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT PNs pedestal rms $eb2 G16 L4", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBLaserTask/Laser4/PN/Gain16/EBLT PNs amplitude $eb2 G16 L4", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/04 Timing/00 Timing",
  [{ 'path': "$prefix/EBTimingClient/EBTMT timing quality $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTimingClient/EBTMT timing $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/05 Trigger/00 FineGrainVeto Errors",
  [{ 'path': "$prefix/EBTriggerTowerTask/EBTTT EmulFineGrainVetoError $eb2", 'description': "Occupancy of mismatches between real Trigger Primitive and emulated one on the fine grain veto. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/06 Energy/Rechits Energy",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit energy $eb2", 'description': "Average energy of the rechits selected among good (rechit quality flag and channel status) reconstructed rechits. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "01 By Supermodule/$eb1/Experts/06 Energy/Rechits Energy Spectrum",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT energy spectrum $eb2", 'description': "Calibrated energy spectrum of the good reconstructed hits (rechit quality flag and channel status). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])
EOF06

}

print LAYOUT <<EOF991;

eblayout(dqmitems, "00 By Task/00 Summary/00 Global Summary",
  [{ 'path': "$prefix/EBSummaryClient/EB global summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/00 Summary/01 Integrity Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBIT integrity quality summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/00 Summary/02 Occupancy Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBOT digi occupancy summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/00 Summary/03 Energy Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBOT energy summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/00 Summary/04 PedestalOnline Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBPOT pedestal quality summary G12", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/00 Summary/05 LaserL1 Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBLT laser quality summary L1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/00 Summary/07 Timing Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBTMT timing quality summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/00 Summary/08 Trigger Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBTTT Et trigger tower summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/00 Summary/09 Trigger Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBTTT emulator error quality summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/00 Summary/10 Trigger Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBTTT Trigger Primitives Timing summary", 'description': "Sample of the emulated TP that more often matches the real TP. Matched sample appear in magenta. No match at all appears red. No events appear white. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/00 Summary/11 StatusFlags Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBSFT front-end status summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

EOF991

for (my $i=1; $i<=36; $i++ ) {

my $eb1;
my $eb2;

if ($i>= 1 && $i<=18) {
#  $eb1=sprintf("EB-/EB-%02d",$i);
  $eb1=sprintf("EB-%02d",$i);
  $eb2=sprintf("EB-%02d",$i);
}

if ($i>=19 && $i<=36) {
#  $eb1=sprintf("EB+/EB+%02d",$i-18);
  $eb1=sprintf("EB+%02d",$i-18);
  $eb2=sprintf("EB+%02d",$i-18);
}

print "$eb1\n";

print LAYOUT <<EOF992;

eblayout(dqmitems, "00 By Task/01 Integrity/00 Integrity Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBIT integrity quality summary", 'description': "Quality summary checking that data for each crystal follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/01 Integrity/$eb1/00 Channel Global Integrity",
  [{ 'path': "$prefix/EBIntegrityClient/EBIT data integrity quality $eb2", 'description': "Quality summary checking that data for each crystal follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT digi occupancy $eb2", 'description': "Occupancy of ECAL digis. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/01 Integrity/$eb1/01 Channel Integrity",
  [{ 'path': "$prefix/EBIntegrityTask/Gain/EBIT gain $eb2", 'description': "Occupancy of integrity errors of type: GAIN ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBIntegrityTask/ChId/EBIT ChId $eb2", 'description': "Occupancy of integrity errors of type: CHANNEL ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/01 Integrity/$eb1/02 Gain Integrity",
  [{ 'path': "$prefix/EBIntegrityTask/GainSwitch/EBIT gain switch $eb2", 'description': "Occupancy of integrity errors of type: GAIN SWITCH. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eblayout(dqmitems, "00 By Task/01 Integrity/$eb1/03 TT Integrity",
  [{ 'path': "$prefix/EBIntegrityTask/TTId/EBIT TTId $eb2", 'description': "Occupancy of integrity errors of type: TRIGGER TOWER ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBIntegrityTask/TTBlockSize/EBIT TTBlockSize $eb2", 'description': "Occupancy of integrity errors of type: TRIGGER TOWER BLOCK SIZE. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/01 Integrity/$eb1/04 MemBox Global Integrity",
  [{ 'path': "$prefix/EBIntegrityClient/EBIT data integrity quality MEM $eb2", 'description': "Quality summary checking that data for each PN follows all the formatting rules and all the constraints which are dictated by the design of the electronics. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT MEM digi occupancy $eb2", 'description': "Occupancy of ECAL PNs digis. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/01 Integrity/$eb1/05 MemBox Channel Integrity",
  [{ 'path': "$prefix/EBIntegrityTask/MemChId/EBIT MemChId $eb2", 'description': "Occupancy of errors for PNs of type: MEM CHANNEL ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBIntegrityTask/MemGain/EBIT MemGain $eb2", 'description': "Occupancy of errors for PNs of type: MEM GAIN ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/01 Integrity/$eb1/06 MemBox TT Integrity",
  [{ 'path': "$prefix/EBIntegrityTask/MemTTId/EBIT MemTTId $eb2", 'description': "Occupancy of integrity errors for PNs of type: TRIGGER TOWER ID. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBIntegrityTask/MemSize/EBIT MemSize $eb2", 'description': "Occupancy of errors for PNs of type: MEM SIZE. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/02 PedestalOnline/00 PedestalOnline Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBPOT pedestal quality summary G12", 'description': "Quality summary checQuality summary checking the quality of the pedestals from the first 3/10 samples of the pulse shape for all the events. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/02 PedestalOnline/01 PedestalOnline Rms",
  [{ 'path': "$prefix/EBSummaryClient/EBPOT pedestal G12 RMS map", 'description': "RMS of the pedestals in ADC counts. Pedestal is evaluated using the first 3/10 samples of the pulse shape for all the events (calibration and physics). Expected RMS for ECAL barrel is 1.1 ADC counts (43 MeV). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/02 PedestalOnline/$eb1/Gain12",
  [{ 'path': "$prefix/EBPedestalOnlineClient/EBPOT pedestal quality G12 $eb2", 'description': "Quality histogram checking the quality of the pedestals from the first 3/10 samples of the pulse shape for all the events. Expected all green color. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBPedestalOnlineClient/EBPOT pedestal mean G12 $eb2", 'description': "Mean of the pedestal value on all the events for the crystals of supermodule $eb2. Expected within 175-225 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBPedestalOnlineClient/EBPOT pedestal rms G12 $eb2", 'description': "RMS of the pedestal value on all the events for the crystals of supermodule $eb2. Expected < 2.0 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])
EOF992

foreach $gain (@MGPAGains) {

print LAYOUT <<EOF993;

eblayout(dqmitems, "00 By Task/03 Pedestal/00 Pedestal Summary Gain$gain",
  [{ 'path': "$prefix/EBSummaryClient/EBPT pedestal quality G$gain summary", 'description': "Quality summary of pedestal events for Gain $gain. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/03 Pedestal/$eb1/00 Gain$gain",
  [{ 'path': "$prefix/EBPedestalClient/EBPT pedestal quality G$gain $eb2", 'description': "Quality histogram of pedestal events on crystals for Gain $gain on supermodule $eb2. Expect green if the pedestal sequence fired the supermodule, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBPedestalClient/EBPT pedestal mean G$gain $eb2", 'description': "Pedestal mean for all (1700) the crystals of the supermodule $eb2 for Gain $gain. Expected mean is between 175-225 ADC counts for all the MGPA gains. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBPedestalClient/EBPT pedestal rms G$gain $eb2", 'description': "Pedestal RMS for all (1700) the crystals of the supermodule $eb2 for Gain $gain. Expected RMS is < 1.0, 1.2, 2.0 ADC counts for MGPA gain 1, 6, 12, respectively. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])
EOF993

}

foreach $gainPN (@MGPAGainsPN) {

print LAYOUT <<EOF994;

eblayout(dqmitems, "00 By Task/03 Pedestal/$eb1/01 PNs Gain$gainPN",
  [{ 'path': "$prefix/EBPedestalClient/EBPT pedestal quality PNs G$gainPN $eb2", 'description': "Quality histogram of pedestal events on PNs for Gain $gainPN. Expect green if the pedestal sequence fired the supermodule, yellow elsewhere. Red spots are failed PNs. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBPedestalTask/PN/Gain$gainPN/EBPDT PNs pedestal $eb2 G$gainPN", 'description': "Pedestal mean for all the 10 PNs of the supermodule $eb2 for Gain $gainPN. Expected mean is around 750 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBPedestalClient/EBPDT PNs pedestal rms $eb2 G$gainPN", 'description': "Pedestal rms for all the 10 PNs of the supermodule $eb2 for Gain $gain.PN Expected rms is around 1 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])
EOF994

}

foreach $gain (@MGPAGains) {

print LAYOUT <<EOF995;

eblayout(dqmitems, "00 By Task/04 TestPulse/00 TestPulse Summary Gain$gain",
  [{ 'path': "$prefix/EBSummaryClient/EBTPT test pulse quality G$gain summary", 'description': "Quality summary of testpulse events for Gain $gain. Expect green where the pedestal sequence fired, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])
EOF995

}

foreach $gain (@MGPAGains) {

print LAYOUT <<EOF996;

eblayout(dqmitems, "00 By Task/04 TestPulse/$eb1/00 Gain$gain",
  [{ 'path': "$prefix/EBTestPulseClient/EBTPT test pulse quality G$gain $eb2", 'description': "Quality histogram of testpulse events on crystals for Gain $gain. Expect green if the testpulse sequence fired the supermodule $eb2, yellow elsewhere. Red spots are failed channels. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTestPulseClient/EBTPT test pulse amplitude G$gain $eb2", 'description': "Testpulse mean for all the crystals of the supermodule $eb2 for Gain $gain. Each channel should be within 20% of the average of the others, RMS < 300 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBTestPulseClient/EBTPT test pulse shape G$gain $eb2", 'description': "Pulse shape of a reference crystal averaged on all the events for Gain $gain. Clear pulse should be seen. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])
EOF996

}

foreach $gainPN (@MGPAGainsPN) {

print LAYOUT <<EOF997;

eblayout(dqmitems, "00 By Task/04 TestPulse/$eb1/01 PNs Quality Gain$gainPN",
  [{ 'path': "$prefix/EBTestPulseClient/EBTPT test pulse quality PNs G$gainPN $eb2", 'description': "Quality histogram of testpulse events on PNs for Gain $gain. Expect green if the testpulse sequence fired the supermodule $eb2, yellow elsewhere. Red spots are failed PNs. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eblayout(dqmitems, "00 By Task/04 TestPulse/$eb1/02 PNs Variables Gain$gainPN",
  [{ 'path': "$prefix/EBTestPulseTask/PN/Gain$gainPN/EBTPT PNs pedestal $eb2 G$gainPN", 'description': "Pedestals of the PNs in tespulse events. It should be > 200 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBTestPulseClient/EBTPT PNs pedestal rms $eb2 G$gainPN", 'description': "RMS of the pedestals of the PNs in testpulse events. It should be < 1ADC count in Gain 01 and < 3 ADC counts in Gain 16. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBTestPulseTask/PN/Gain$gainPN/EBTPT PNs amplitude $eb2 G$gainPN", 'description': "Amplitude of the PNs in testpulse events. It should be > 100 ADC counts and each channel within 20% of the others. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])
EOF997

}

foreach $laser (@Lasers) {

print LAYOUT <<EOF999;

eblayout(dqmitems, "00 By Task/05 Laser/00 LaserL$laser Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBLT laser quality summary L$laser", 'description': "Quality summary of laser events. Expect green where the laser sequence fired, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/05 Laser/$eb1/L$laser/00 Quality",
  [{ 'path': "$prefix/EBLaserClient/EBLT laser quality L$laser $eb2", 'description': "Quality histogram of laser events on crystals. Expect green if the laser sequence fired supermodule $eb2, yellow elsewhere. Red spots are failed channels. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBLaserClient/EBLT laser quality L$laser PNs G01 $eb2", 'description': "Quality histogram of laser events on PNs in Gain 1. Expect green if the laser sequence fired supermodule $eb2, yellow elsewhere. Red spots are failed PNs. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" },
   { 'path': "$prefix/EBLaserClient/EBLT laser quality L$laser PNs G16 $eb2", 'description': "Quality histogram of laser events on PNs in Gain 16. Expect green if the laser sequence fired supermodule $eb2, yellow elsewhere. Red spots are failed PNs. Legend: green = good;  red = bad;  yellow = no entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/05 Laser/$eb1/L$laser/01 Amplitude",
  [{ 'path': "$prefix/EBLaserClient/EBLT amplitude L$laser $eb2", 'description': "Average amplitude for each channel of $eb2 (error is the RMS of the distribution). Quality cuts are: amplitude > 100 ADC; amplitude is within 40% of the 1700 crystals amplitude; RMS < 30% of the mean. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBLaserClient/EBLT amplitude over PN L$laser $eb2", 'description': "Average APD/PN for each channel of $eb2 (error is the RMS of the distribution). Quality cuts are: APD/PN > 50. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/05 Laser/$eb1/L$laser/02 Timing",
  [{ 'path': "$prefix/EBLaserClient/EBLT laser timing L$laser $eb2", 'description': "Average timing for each channel of $eb2 (error is the RMS of the distribution). Quality cuts are: timing is within 5.5 - 6.5 clocks. RMS is < 0.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBLaserClient/EBLT laser timing mean L$laser $eb2", 'description': "Mean timing of the channels in $eb2. Timing is expected within 5.5 - 6.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT laser timing rms L$laser $eb2", 'description': "Timing RMS of the channels in $eb2. Timing RMS is expected < 0.5 clocks. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/05 Laser/$eb1/L$laser/03 PulseShape",
  [{ 'path': "$prefix/EBLaserClient/EBLT laser shape L$laser $eb2", 'description': "Pulse shape of a reference crystal (crystal 1 of $eb2) averaged on all the events. Clear pulse should be seen. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBLaserTask/Laser$laser/EBLT shape $eb2 L$laser", 'description': "Pulse shape of all the 1700 crystals in $eb2, averaged on all the events. Clear pulses should be seen. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/05 Laser/$eb1/L$laser/04 PNs Gain01",
  [{ 'path': "$prefix/EBLaserTask/Laser$laser/PN/Gain01/EBLT PNs pedestal $eb2 G01 L$laser", 'description': "Average pedestals of the 10 PNs of $eb2 in Gain 1. Expected around 760 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT PNs pedestal rms $eb2 G01 L$laser", 'description': "RMS of the pedestals of PN diodes of $eb2 in Gain 1. Expected around 2.0 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBLaserTask/Laser$laser/PN/Gain01/EBLT PNs amplitude $eb2 G01 L$laser", 'description': "Amplitudes of the 10 PNs of $eb2 in Gain 1. Expected values around 1500 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eblayout(dqmitems, "00 By Task/05 Laser/$eb1/L$laser/05 PNs Gain16",
  [{ 'path': "$prefix/EBLaserTask/Laser$laser/PN/Gain16/EBLT PNs pedestal $eb2 G16 L$laser", 'description': "Average pedestals of the 10 PNs of $eb2 in Gain 16. Expected around 760 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBLaserClient/EBLT PNs pedestal rms $eb2 G16 L$laser", 'description': "RMS of the pedestals of PN diodes of $eb2 in Gain 16. Expected around 2.0 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBLaserTask/Laser$laser/PN/Gain16/EBLT PNs amplitude $eb2 G16 L$laser", 'description': "Amplitudes of the 10 PNs of $eb2 in Gain 16. Expected values around 1500 ADC counts. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

EOF999

}


print LAYOUT <<EOF994;

eblayout(dqmitems, "00 By Task/07 Timing/00 Timing Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBTMT timing quality summary", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/07 Timing/$eb1/00 Timing Quality",
  [{ 'path': "$prefix/EBTimingClient/EBTMT timing quality $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTimingClient/EBTMT timing mean $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBTimingClient/EBTMT timing rms $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/07 Timing/$eb1/01 Timing Value",
  [{ 'path': "$prefix/EBTimingClient/EBTMT timing $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBTimingTask/EBTMT timing 1D $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/07 Timing/$eb1/02 Timing Maps",
  [{ 'path': "$prefix/EBTimingTask/EBTMT timing $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBTimingTask/EBTMT timing vs amplitude $eb2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/08 Trigger/00 Trigger Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBTTT Trigger Primitives Timing summary", 'description': "Sample of the emulated TP that more often matches the real TP. Matched sample appear in non-red colors. Match with on-time primitives appear yellow (expected). No match at all appears red. No events appear white. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/08 Trigger/$eb1/00 TransverseEnergy",
  [{ 'path': "$prefix/EBTriggerTowerTask/EBTTT EmulError $eb2", 'description': "Number of errors comparing the values of Et for real and emulated trigger primitives. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTriggerTowerTask/EBTTT Et map Real Digis $eb2", 'description': "Average transverse energy (ADC counts: 4 ADCs = 1 GeV) for real trigger primitives. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/08 Trigger/$eb1/01 TriggerPrimitivesTiming",
  [{ 'path': "$prefix/EBTriggerTowerClient/EBTTT Trigger Primitives Timing $eb2", 'description': "Sample of the emulated TP that more often matches the real TP. Matched sample appear in magenta. No match at all appears red. No events appear white. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBTriggerTowerClient/EBTTT Non Single Timing $eb2", 'description': "Fraction of events where the TP has a matching with emulator different by the most probable (expected empty). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/08 Trigger/$eb1/02 FineGrainVeto Errors",
  [{ 'path': "$prefix/EBTriggerTowerTask/EBTTT EmulFineGrainVetoError $eb2", 'description': "Occupancy of mismatches between real Trigger Primitive and emulated one on the fine grain veto. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eblayout(dqmitems, "00 By Task/09 Energy/00 Energy Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBOT energy summary", 'description': "Average energy (in GeV) of good reconstructed hits (good rechit flag and channel status required). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/09 Energy/$eb1/00 Energy",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit energy $eb2", 'description': "Average energy of the rechits selected among good (rechit quality flag and channel status) reconstructed hits. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/09 Energy/$eb1/01 Energy",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT energy spectrum $eb2", 'description': "Calibrated energy spectrum of the good reconstructed hits (rechit quality flag and channel status). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/10 StatusFlags/00 StatusFlags Summary",
  [{ 'path': "$prefix/EBSummaryClient/EBSFT front-end status summary", 'description': "DCC front-end status quality summary. Expected all green color. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/10 StatusFlags/$eb1/00 StatusFlags",
  [{ 'path': "$prefix/EBStatusFlagsTask/FEStatus/EBSFT front-end status $eb2", 'description': "Occupancy of DCC front-end status errors. Status ACTIVE (i.e. full-readout) or SUPPRESSED (i.e. zero-suppression applied) are not considered as errors and do not create entries in this plot. Expected all empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBStatusFlagsTask/FEStatus/EBSFT MEM front-end status $eb2", 'description': "Occupancy of DCC front-end errors for the MEM boxes (TTs: 69-70). Status ACTIVE (i.e. full-readout) or SUPPRESSED (i.e. zero-suppression applied) are not considered as errors and do not create entries in this plot. Expected all empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/10 StatusFlags/$eb1/01 StatusFlags",
  [{ 'path': "$prefix/EBStatusFlagsTask/FEStatus/EBSFT front-end status bits $eb2", 'description': "Occupancy of DCC front-end statuses by type. ACTIVE and SUPPRESSED are normal entries. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])
EOF994

}

print LAYOUT <<EOF993;

eblayout(dqmitems, "00 By Task/11 Cluster/00 BasicClusters 1D",
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC energy", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT BC size", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC number", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eblayout(dqmitems, "00 By Task/11 Cluster/01 BasicClusters Energy",
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC energy map", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC energy projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT BC energy projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/11 Cluster/02 BasicClusters Size",
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC size map", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC size projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT BC size projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/11 Cluster/03 BasicClusters TransverseEnergy",
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC ET map", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC ET projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT BC ET projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/11 Cluster/04 BasicClusters Multiplicity",
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC number map", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT BC number projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT BC number projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/11 Cluster/05 SuperClusters 1D",
  [{ 'path': "$prefix/EBClusterTask/EBCLT SC energy", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT SC size (crystal)", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT SC number", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   None])

eblayout(dqmitems, "00 By Task/11 Cluster/06 SuperClusters Seeds",
  [{ 'path': "$prefix/EBClusterTask/EBCLT SC seed crystal energy", 'description': "energy (GeV) of the crystal seeding a supercluster. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/11 Cluster/07 SuperClusters Seeds",
  [{ 'path': "$prefix/EBClusterTask/EBCLT SC single crystal cluster seed occupancy map", 'description': "Occupancy of seeds of superclusters formed by exactly one crystal. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT SC energy vs seed crystal energy", 'description': "supercluster energy vs seed crystal energy. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/11 Cluster/08 ClusterShapes",
  [{ 'path': "$prefix/EBClusterTask/EBCLT s1s9", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT s9s25", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/11 Cluster/09 Full Combinatorics (Selected)",
  [{ 'path': "$prefix/EBClusterTask/EBCLT dicluster invariant mass Pi0 sel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT dicluster invariant mass JPsi sel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT dicluster invariant mass Z0 sel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT dicluster invariant mass high sel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/11 Cluster/10 Best Pair Combinatorics (No sel)",
  [{ 'path': "$prefix/EBClusterTask/EBCLT dicluster invariant mass Pi0", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT dicluster invariant mass JPsi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBClusterTask/EBCLT dicluster invariant mass Z0", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBClusterTask/EBCLT dicluster invariant mass high", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])
EOF993

print LAYOUT <<EOF995;
eblayout(dqmitems, "00 By Task/12 Occupancy/00 Occupancy",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT digi occupancy", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT digi occupancy projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBOccupancyTask/EBOT digi occupancy projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/12 Occupancy/01 Occupancy",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit occupancy", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit occupancy projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBOccupancyTask/EBOT rec hit occupancy projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/12 Occupancy/02 Occupancy",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit thr occupancy", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT rec hit thr occupancy projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBOccupancyTask/EBOT rec hit thr occupancy projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/12 Occupancy/03 Occupancy",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT TP digi occupancy", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT TP digi occupancy projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBOccupancyTask/EBOT TP digi occupancy projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/12 Occupancy/04 Occupancy",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT TP digi thr occupancy", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBOccupancyTask/EBOT TP digi thr occupancy projection eta", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } },
   { 'path': "$prefix/EBOccupancyTask/EBOT TP digi thr occupancy projection phi", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/12 Occupancy/05 Occupancy",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT test pulse digi occupancy", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eblayout(dqmitems, "00 By Task/12 Occupancy/06 Occupancy",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT laser digi occupancy", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eblayout(dqmitems, "00 By Task/12 Occupancy/07 Occupancy",
  [{ 'path': "$prefix/EBOccupancyTask/EBOT pedestal digi occupancy", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [None])

eblayout(dqmitems, "00 By Task/13 RawData/00 RawData",
  [{ 'path': "$prefix/EBRawDataTask/EBRDT event type calibration BX", 'description': "Event type in ECAL DCC header in the calibration BX. It should contain only calibration events. It is filled once per DCC with weight 1/36. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBRawDataTask/EBRDT calibration event errors", 'description': "Number of events with at least one FED containing calibration events during physics gap or physics events during the calibration BX. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/13 RawData/01 RawData",
  [{ 'path': "$prefix/EBRawDataTask/EBRDT event type pre calibration BX", 'description': "Event type in ECAL DCC header in the BXs reserved to physics events preceding calibration BX. It should contain only physics events. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBRawDataTask/EBRDT event type post calibration BX", 'description': "Event type in ECAL DCC header in the BXs reserved to physics events following calibration BX. It should contain only physics events. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/13 RawData/02 RawData",
  [{ 'path': "$prefix/EBRawDataTask/EBRDT CRC errors", 'description': "Number of fatal errors related to the event size. It is filled once per DCC with weight 1/36. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBRawDataTask/EBRDT trigger type errors", 'description': "Number of trigger type mismatches between the global trigger and ECAL DCC header. It is filled once per DCC with weight 1/36. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/13 RawData/03 RawData",
  [{ 'path': "$prefix/EBRawDataTask/EBRDT run number errors", 'description': "Number of run-number mismatches between the global trigger and ECAL DCC header. It is filled once per DCC with weight 1/36. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBRawDataTask/EBRDT orbit number errors", 'description': "Number of orbit-number mismatches between the global trigger and ECAL DCC header. It is filled once per DCC with weight 1/36. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/13 RawData/04 RawData",
  [{ 'path': "$prefix/EBRawDataTask/EBRDT L1A DCC errors", 'description': "Number of level 1 event mismatches between the global trigger and ECAL DCC header. It is filled once per DCC with weight 1/36. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBRawDataTask/EBRDT bunch crossing DCC errors", 'description': "Number of bunch-crossing mismatches between the global trigger and ECAL DCC header. It is filled once per DCC with weight 1/36. It should be empty.<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/13 RawData/05 RawData",
  [{ 'path': "$prefix/EBRawDataTask/EBRDT L1A FE errors", 'description': "Number of level 1 event mismatches between the ECAL DCC header and its front-end cards. It is filled once per front-end card in DCC with weight 1/n(front-end cards). It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBRawDataTask/EBRDT bunch crossing FE errors", 'description': "Number of bunch-crossing mismatches between the ECAL DCC header and its front-end cards. It is filled once per front-end card in DCC with weight 1/n(front-end cards). It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/13 RawData/06 RawData",
  [{ 'path': "$prefix/EBRawDataTask/EBRDT L1A TCC errors", 'description': "Number of level 1 event mismatches between the ECAL DCC header and its TCCs. It is filled once per TCC in DCC with weight 1/n(TCC). It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBRawDataTask/EBRDT bunch crossing TCC errors", 'description': "Number of bunch-crossing mismatches between the ECAL DCC header and its TCCs. It is filled once per TCC in DCC with weight 1/n(TCC). It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/13 RawData/07 RawData",
  [{ 'path': "$prefix/EBRawDataTask/EBRDT L1A SRP errors", 'description': "Number of level 1 event mismatches between the ECAL DCC header and its SRP block. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBRawDataTask/EBRDT bunch crossing SRP errors", 'description': "Number of bunch-crossing mismatches between the ECAL DCC header and its SRP block. It should be empty. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/14 SelectiveReadout/00 EventSize",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT event size", 'description': "Event size for the readout crystals in the ECAL barrel. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT tower event size", 'description': "Event size distribution per tower. This plot allows to verify the correctness of the ZS thresholds applied. The average of the plot has to be ~ 2kB / 68 ~ 30 bytes.<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }] )

eblayout(dqmitems, "00 By Task/14 SelectiveReadout/01 DCC EventSize",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT DCC event size", 'description': "Average event size per ecah ECAL barrel DCC. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT event size vs DCC", 'description': "Event size distribution per each ECAL barrel DCC. Finer binning is used in the zero-suppressed region (0-0.608 kB), while granularity of one fully readout tower (0.608 kB) has been used for higher sizes. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>"}])

eblayout(dqmitems, "00 By Task/14 SelectiveReadout/02 SRFlags",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT readout unit with SR forced", 'description': "Rate of readout units with Selective Readout Forced. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT full readout SR Flags", 'description': "Rate of full readout Selective Readout flags. The selective readout of the ECAL was designed to reduce the ECAL data size by a factor 20. If any tower is fully read out more than 5% of the times, this tower will increase the payload and introduce deadtime. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/14 SelectiveReadout/03 TTFlags",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT high interest TT Flags", 'description': "Rate of high interest Trigger Tower flags. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }],
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT low interest TT Flags", 'description': "Rate of low interest Trigger Tower flags. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>" }])

eblayout(dqmitems, "00 By Task/14 SelectiveReadout/04 Payload",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT high interest payload", 'description': "Payload of the high interest trigger towers (kB). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT low interest payload", 'description': "Payload of the low interest trigger towers (kB). <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

eblayout(dqmitems, "00 By Task/14 SelectiveReadout/05 ZSFilterOutput",
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT high interest ZS filter output", 'description': "crystal amplitude as reconstructed in DCC. This is useful for the emulation of the zero suppression filter. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }],
  [{ 'path': "$prefix/EBSelectiveReadoutTask/EBSRT low interest ZS filter output", 'description': "crystal amplitude as reconstructed in DCC. This is useful for the emulation of the zero suppression filter. <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftEcalExpert>DQMShiftEcalExpert</a>", 'draw': { 'withref': "yes" } }])

EOF995

close LAYOUT;

