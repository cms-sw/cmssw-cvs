def shiftrpclayout(i, p, *rows): i["00 Shift/RPC/" + p] = DQMItem(layout=rows)

shiftrpclayout(dqmitems, "00-BarrelOccupancy",
  [{ 'path': "RPC/RecHits/SummaryHistograms/BarrelOccupancy", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftRPC>Description</a>" }])
