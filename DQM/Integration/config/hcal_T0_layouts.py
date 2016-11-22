def hcallayout(i, p, *rows): i["Hcal/Layouts/" + p] = DQMItem(layout=rows)
hcallayout(dqmitems, "HCAL DataFormat Problems",
           [{ 'path': "Hcal/DataFormatMonitor/ HardwareWatchCells",
              'description': "A Data Format error indicates that the data received from this channel was somehow corrupted or compromised.  This plot is the sum of bad digis over all depths. iPhi (0 to 72) by iEta (-41 to 41) More at <a href=https://twiki.cern.ch/twiki/bin/view/CMS/HcalDQMHistograms>HcalDQMHistograms</a>" }]
                                 )



hcallayout(dqmitems, "HCAL Unsuppressed Channels",
           [{ 'path': "Hcal/DataFormatMonitor/HTR Plots/Fraction UnSuppressed Events",
              'description': "This shows the fraction of unsuppressed events for each FED and SPIGOT. More (eventually)  at <a href=https://twiki.cern.ch/twiki/bin/view/CMS/HcalDQMHistograms>HcalDQMHistograms</a>" }]
                      )
