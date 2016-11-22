def shifteslayout(i, p, *rows): i["00 Shift/EcalPreshower/" + p] = DQMItem(layout=rows)

shifteslayout(dqmitems, "01-IntegritySummary",
  [{ 'path': "EcalPreshower/ESIntegrityClient/ES Integrity Summary 1 Z 1 P 1", 'description': "ES+ Front Integrity Summary 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a>" },
   { 'path': "EcalPreshower/ESIntegrityClient/ES Integrity Summary 1 Z -1 P 1", 'description': "ES- Front Integrity Summary 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a>" }],
  [{ 'path': "EcalPreshower/ESIntegrityClient/ES Integrity Summary 1 Z 1 P 2", 'description': "ES+ Rear Integrity Summary 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a>" },
   { 'path': "EcalPreshower/ESIntegrityClient/ES Integrity Summary 1 Z -1 P 2", 'description': "ES- Rear Integrity Summary 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a>" }])

shifteslayout(dqmitems, "02-OccupancySummary",
  [{ 'path': "EcalPreshower/ESOccupancyTask/ES RecHit 2D Occupancy Z 1 P 1", 'description': "ES RecHit 2D Occupancy Z 1 P 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a>" },
   { 'path': "EcalPreshower/ESOccupancyTask/ES RecHit 2D Occupancy Z -1 P 1", 'description': "ES RecHit 2D Occupancy Z -1 P 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a>" }],
  [{ 'path': "EcalPreshower/ESOccupancyTask/ES RecHit 2D Occupancy Z 1 P 2", 'description': "ES RecHit 2D Occupancy Z 1 P 2 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a>" },
   { 'path': "EcalPreshower/ESOccupancyTask/ES RecHit 2D Occupancy Z -1 P 2", 'description': "ES RecHit 2D Occupancy Z -1 P 2 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a>" }])

shifteslayout(dqmitems, "03-RechitEnergySummary",
  [{ 'path': "EcalPreshower/ESOccupancyTask/ES RecHit Energy Z 1 P 1", 'description': "ES RecHit Energy Z 1 P 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a>" },
   { 'path': "EcalPreshower/ESOccupancyTask/ES RecHit Energy Z -1 P 1", 'description': "ES RecHit Energy Z -1 P 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a>" }],
  [{ 'path': "EcalPreshower/ESOccupancyTask/ES RecHit Energy Z 1 P 2", 'description': "ES RecHit Energy Z 1 P 2 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a>" },
   { 'path': "EcalPreshower/ESOccupancyTask/ES RecHit Energy Z -1 P 2", 'description': "ES RecHit Energy Z -1 P 2 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a>" }])

