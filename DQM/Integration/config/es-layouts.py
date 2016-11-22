def ecalpreshowerlayout(i, p, *rows): i["EcalPreshower/Layouts/" + p] = DQMItem(layout=rows)

# Quick Collections
ecalpreshowerlayout(dqmitems, "01-IntegritySummary-EcalPreshower",
  [{ 'path': "EcalPreshower/ESIntegrityClient/ES Integrity Summary 1 Z 1 P 1", 'description': "ES+ Front Integrity Summary 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a> <br/> <table width=100%> <tr><td>1 - not used<td>2 - fiber problem<td>3 - OK<td>4 - FED problem<td><tr>5 - KCHIP problem<td>6 - ES counters are not synced with GT counters (see ESRawDataTask) <td> 7 - more than one problem<td>8 - SLink CRC error</table> " },
   { 'path': "EcalPreshower/ESIntegrityClient/ES Integrity Summary 1 Z -1 P 1", 'description': "ES- Front Integrity Summary 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a> <br/> <table width=100%> <tr><td>1 - not used<td>2 - fiber problem<td>3 - OK<td>4 - FED problem<td><tr>5 - KCHIP problem<td>6 - ES counters are not synced with GT counters (see ESRawDataTask) <td> 7 - more than one problem<td>8 - SLink CRC error</table> " }],
  [{ 'path': "EcalPreshower/ESIntegrityClient/ES Integrity Summary 1 Z 1 P 2", 'description': "ES+ Rear Integrity Summary 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a> <br/> <table width=100%> <tr><td>1 - not used<td>2 - fiber problem<td>3 - OK<td>4 - FED problem<td><tr>5 - KCHIP problem<td>6 - ES counters are not synced with GT counters (see ESRawDataTask) <td> 7 - more than one problem<td>8 - SLink CRC error</table> " },
   { 'path': "EcalPreshower/ESIntegrityClient/ES Integrity Summary 1 Z -1 P 2", 'description': "ES- Rear Integrity Summary 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a> <br/> <table width=100%> <tr><td>1 - not used<td>2 - fiber problem<td>3 - OK<td>4 - FED problem<td><tr>5 - KCHIP problem<td>6 - ES counters are not synced with GT counters (see ESRawDataTask) <td> 7 - more than one problem<td>8 - SLink CRC error</table> " }])

ecalpreshowerlayout(dqmitems, "02-OccupancySummary-EcalPreshower",
  [{ 'path': "EcalPreshower/ESOccupancyTask/ES RecHit 2D Occupancy Z 1 P 1", 'description': "ES RecHit 2D Occupancy Z 1 P 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a> " },
   { 'path': "EcalPreshower/ESOccupancyTask/ES RecHit 2D Occupancy Z -1 P 1", 'description': "ES RecHit 2D Occupancy Z -1 P 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a> " }],
  [{ 'path': "EcalPreshower/ESOccupancyTask/ES RecHit 2D Occupancy Z 1 P 2", 'description': "ES RecHit 2D Occupancy Z 1 P 2 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a> " },
   { 'path': "EcalPreshower/ESOccupancyTask/ES RecHit 2D Occupancy Z -1 P 2", 'description': "ES RecHit 2D Occupancy Z -1 P 2 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a> " }])

ecalpreshowerlayout(dqmitems, "03-RechitEnergySummary-EcalPreshower",
  [{ 'path': "EcalPreshower/ESOccupancyTask/ES RecHit Energy Z 1 P 1", 'description': "ES RecHit Energy Z 1 P 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a> " },
   { 'path': "EcalPreshower/ESOccupancyTask/ES RecHit Energy Z -1 P 1", 'description': "ES RecHit Energy Z -1 P 1 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a> " }],
  [{ 'path': "EcalPreshower/ESOccupancyTask/ES RecHit Energy Z 1 P 2", 'description': "ES RecHit Energy Z 1 P 2 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a> " },
   { 'path': "EcalPreshower/ESOccupancyTask/ES RecHit Energy Z -1 P 2", 'description': "ES RecHit Energy Z -1 P 2 - <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftPreshower>DQMShiftPreshower</a> " }])

