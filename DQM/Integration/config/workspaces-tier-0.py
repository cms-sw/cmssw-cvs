server.workspace('DQMQuality', 0, 'Summaries', 'Summary')
server.workspace('DQMSummary', 1, 'Summaries', 'Reports')
server.workspace('DQMShift',   2, 'Summaries', 'Shift')
server.workspace('DQMContent', 3, 'Summaries', 'Everything', '^')

server.workspace('DQMContent', 10, 'Tracker/Muons', 'Pixel', '^Pixel/',
                 'Pixel/Layouts/00 - Pixel_Error_Summary',
                 'Pixel/Layouts/01 - Pixel_Digi_Summary',
                 'Pixel/Layouts/02 - Pixel_Cluster_Summary',
                 'Pixel/Layouts/03 - Pixel_Track_Summary')

server.workspace('DQMContent', 10, 'Tracker/Muons', 'SiStrip', '^(SiStrip|Tracking)/',
                 'SiStrip/Layouts/00 - ReportSummary',
                 'SiStrip/Layouts/01 - FED-Detected Errors',
                 'SiStrip/Layouts/02 - OnTrackCluster (StoN)',
                 'SiStrip/Layouts/03 - OffTrackCluster (Total Number)',
                 'SiStrip/Layouts/04 - Tracks - CKF',
                 'SiStrip/Layouts/05 - Tracks - Cosmic Track Finder',
                 'SiStrip/Layouts/06 - Tracks - Rod Search')

server.workspace('DQMContent', 12, 'Tracker/Muons', 'CSC', '^CSC/')
server.workspace('DQMContent', 13, 'Tracker/Muons', 'DT', '^DT/')
server.workspace('DQMContent', 14, 'Tracker/Muons', 'RPC', '^RPC/') 

server.workspace('DQMContent', 21, 'Calorimeter', 'EcalBarrel', '^EcalBarrel/')
server.workspace('DQMContent', 22, 'Calorimeter', 'EcalEndcap', '^EcalEndcap/')
server.workspace('DQMContent', 23, 'Calorimeter', 'EcalPreshower', '^EcalPreshower')
server.workspace('DQMContent', 24, 'Calorimeter', 'HCAL', '^Hcal/',
                 'Hcal/Layouts/HCAL DataFormat Problems',
                 'Hcal/Layouts/HCAL Unsuppressed Channels')

server.workspace('DQMContent', 31, 'Trigger/Lumi', 'L1T', '^L1T/')
server.workspace('DQMContent', 32, 'Trigger/Lumi', 'L1TEMU', '^L1TEMU/')
server.workspace('DQMContent', 33, 'Trigger/Lumi', 'HLT', '^HLT/')
server.workspace('DQMContent', 34, 'Trigger/Lumi', 'HLX', '^HLX')

server.workspace('DQMContent', 41, 'POG', 'Muons', '^Muons/')
server.workspace('DQMContent', 42, 'POG', 'JetMet', '^JetMET/')
server.workspace('DQMContent', 43, 'POG', 'EGamma', '^Egamma/')


