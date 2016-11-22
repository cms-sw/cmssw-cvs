def shifthlxlayout(i, p, *rows): i["00 Shift/HLX/" + p] = DQMItem(layout=rows)

shifthlxlayout(dqmitems, "Shifter HLX Lumi Summary",
  [ {'path':"HLX/Luminosity/LumiIntegratedEtSum", 'description':"<a href=https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki_SystemOperations>Lumi Shift Histos</a>"},
    {'path':"HLX/Luminosity/LumiIntegratedOccSet1",'description':"<a href=https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki_SystemOperations>Lumi Shift Histos</a>"},
    {'path':"HLX/Luminosity/LumiIntegratedOccSet2",'description':"<a href=https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki_SystemOperations>Lumi Shift Histos</a>"} ])

shifthlxlayout(dqmitems, "Shifter HLX HF Compare Summary",
  [ {'path':"HLX/HFCompare/HFCompareEtSum",'description':"<a href=https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki_SystemOperations>Lumi Shift Histos</a>"}],
  [ {'path':"HLX/HFCompare/HFCompareOccBelowSet1",'description':"<a href=https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki_SystemOperations>Lumi Shift Histos</a>"},
    {'path':"HLX/HFCompare/HFCompareOccBelowSet2",'description':"<a href=https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki_SystemOperations>Lumi Shift Histos</a>"}])

shifthlxlayout(dqmitems, "Shifter HLX Averages Summary",
  [ {'path':"HLX/Average/AvgEtSum",'description':"<a href=https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki_SystemOperations>Lumi Shift Histos</a>"}],
  [ {'path':"HLX/Average/AvgOccBelowSet1",'description':"<a href=https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki_SystemOperations>Lumi Shift Histos</a>"},
    {'path':"HLX/Average/AvgOccBelowSet2",'description':"<a href=https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki_SystemOperations>Lumi Shift Histos</a>"}])

shifthlxlayout(dqmitems, "Shifter HLX CheckSum Summary",
  [ {'path':"HLX/CheckSums/SumAllOccSet1",'description':"<a href=https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki_SystemOperations>Lumi Shift Histos</a>"},
    {'path':"HLX/CheckSums/SumAllOccSet2",'description':"<a href=https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki_SystemOperations>Lumi Shift Histos</a>"} ])


