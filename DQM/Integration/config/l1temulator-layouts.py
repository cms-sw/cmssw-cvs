def l1temulayout(i, p, *rows): i["L1TEMU/Layouts/" + p] = DQMItem(layout=rows)

def l1temucommon(i, dir, name):i["L1TEMU/Layouts/00-Global-Summary/%s" % name] = \
    DQMItem(layout=[["L1TEMU/%s/%s" % (dir, name)]]) 

l1temucommon(dqmitems, "common", "sysrates")
l1temucommon(dqmitems, "common", "errorflag")
l1temucommon(dqmitems, "common", "sysncandData")
l1temucommon(dqmitems, "common", "sysncandEmul")

def l1t_rct_expert(i, p, *rows): i["L1TEMU/Layouts/03-L1TdeRCT-Summary/" + p] = DQMItem(layout=rows)
l1t_rct_expert(dqmitems, "rctIsoEmEff1",
  [{ 'path': "L1TEMU/L1TdeRCT/IsoEm/rctIsoEmEff1", 'description': "Isolated electrons efficiency, For details see - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/RCTDataQualityMonitoring>RCTDQM</a> CAL/RCT/GCT mapping is here <a href=https://twiki.cern.ch/twiki/pub/CMS/RCTDataQualityMonitoring/RCTGCTCAL.jpeg> mapping </a>" }])

l1t_rct_expert(dqmitems, "rctNisoEmEff1",
  [{ 'path': "L1TEMU/L1TdeRCT/NisoEm/rctNisoEmEff1", 'description': "Non-Isolated electrons efficiency, For details see - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/RCTDataQualityMonitoring>RCTDQM</a> CAL/RCT/GCT mapping is here <a href=https://twiki.cern.ch/twiki/pub/CMS/RCTDataQualityMonitoring/RCTGCTCAL.jpeg> mapping </a>" }])

l1t_rct_expert(dqmitems, "rctRegEff2D",
  [{ 'path': "L1TEMU/L1TdeRCT/RegionData/rctRegEff2D", 'description': "Regional efficiency, For details see - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/RCTDataQualityMonitoring>RCTDQM</a> CAL/RCT/GCT mapping is here <a href=https://twiki.cern.ch/twiki/pub/CMS/RCTDataQualityMonitoring/RCTGCTCAL.jpeg> mapping </a>" }])


