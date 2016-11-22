def shiftrpclayout(i, p, *rows): i["00 Shift/RPC/" + p] = DQMItem(layout=rows)

shiftrpclayout(dqmitems, "01-TOP_Summary_Wheel+2",
  [{ 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftRPC>Description</a>" }])

shiftrpclayout(dqmitems, "02-TOP_Summary_Wheel+1",
  [{ 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftRPC>Description</a>" }])

shiftrpclayout(dqmitems, "03-TOP_Summary_Wheel0",
  [{ 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel0", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftRPC>Description</a>" }])

shiftrpclayout(dqmitems, "04-TOP_Summary_Wheel-1",
  [{ 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel-1", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftRPC>Description</a>" }])

shiftrpclayout(dqmitems, "05-TOP_Summary_Wheel-2",
  [{ 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel-2", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftRPC>Description</a>" }])
