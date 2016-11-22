
def rpclayout(i, p, *rows): i["RPC/Layouts/" + p] = DQMItem(layout=rows)

########### define varialbles for frequently used strings ############# 
rpclink = "   >>> <a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftRPC>Description</a>";
dead = "RPC Dead Channels Fraction ";
onebin = "RPC Cluster Size In First Bin Normalized by Entries ";
top = "RPC TOP Summary Histogram ";
asymmetry = "RPC AsymmetryLeftRight ";
numberofdigi = "RPC NumberOfDigi ";
noisy = "RPC NoisyStrips ";


################### Links to TOP Summary Histograms #################################
rpclayout(dqmitems, "RPC_Summary/TOP_Summary_ChamberQuality",
          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel2", 'description': top + rpclink },
           { 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel1", 'description': top + rpclink  }],
          
          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel0", 'description': top + rpclink  },
           { 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel-1", 'description': top + rpclink  },
           { 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel-2", 'description': top + rpclink  }]
          )          


rpclayout(dqmitems, "RPC_Summary/TOP_Summary_Distribution",
          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Distribution_Wheel2", 'description': top + rpclink },
           { 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Distribution_Wheel1", 'description': top + rpclink  }],

          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Distribution_Wheel0", 'description': top + rpclink  },
           { 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Distribution_Wheel-1", 'description': top + rpclink  },
           { 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Distribution_Wheel-2", 'description': top + rpclink  }]
          )      

###---> Dead Channels Fraction 
rpclayout(dqmitems, "RPC_Summary/RPC_DeadChanelsFraction",
          [{ 'path': "RPC/RecHits/SummaryHistograms/DeadChannelFraction_Roll_vs_Sector_Wheel2", 'description': dead + rpclink },
           { 'path': "RPC/RecHits/SummaryHistograms/DeadChannelFraction_Roll_vs_Sector_Wheel1", 'description': dead + rpclink }],

          [{ 'path': "RPC/RecHits/SummaryHistograms/DeadChannelFraction_Roll_vs_Sector_Wheel0", 'description': dead + rpclink },
           { 'path': "RPC/RecHits/SummaryHistograms/DeadChannelFraction_Roll_vs_Sector_Wheel-1", 'description': dead + rpclink },
           { 'path': "RPC/RecHits/SummaryHistograms/DeadChannelFraction_Roll_vs_Sector_Wheel-2", 'description': dead + rpclink }]
          )          

###---> ClusterSize1bin
rpclayout(dqmitems, "RPC_Summary/RPC_ClusterSizeInFirstBin",
          [{ 'path': "RPC/RecHits/SummaryHistograms/ClusterSizeIn1Bin_Roll_vs_Sector_Wheel2", 'description': onebin+rpclink },
           { 'path': "RPC/RecHits/SummaryHistograms/ClusterSizeIn1Bin_Roll_vs_Sector_Wheel1", 'description':  onebin+rpclink }],

          [{ 'path': "RPC/RecHits/SummaryHistograms/ClusterSizeIn1Bin_Roll_vs_Sector_Wheel0", 'description': onebin+rpclink },
           { 'path': "RPC/RecHits/SummaryHistograms/ClusterSizeIn1Bin_Roll_vs_Sector_Wheel-1", 'description': onebin+rpclink },
           { 'path': "RPC/RecHits/SummaryHistograms/ClusterSizeIn1Bin_Roll_vs_Sector_Wheel-2", 'description': onebin+rpclink }]
          )
          
###---> AsymmetryLeftRight_Roll_vs_Sector_Wheel
rpclayout(dqmitems, "RPC_Summary/RPC_AsymmetryLeftRight",
          [{ 'path': "RPC/RecHits/SummaryHistograms/AsymmetryLeftRight_Roll_vs_Sector_Wheel2", 'description': asymmetry+rpclink },
           { 'path': "RPC/RecHits/SummaryHistograms/AsymmetryLeftRight_Roll_vs_Sector_Wheel1", 'description': asymmetry+rpclink   }],

          [{ 'path': "RPC/RecHits/SummaryHistograms/AsymmetryLeftRight_Roll_vs_Sector_Wheel0", 'description': asymmetry+rpclink  },
           { 'path': "RPC/RecHits/SummaryHistograms/AsymmetryLeftRight_Roll_vs_Sector_Wheel-1", 'description': asymmetry+rpclink  },
           { 'path': "RPC/RecHits/SummaryHistograms/AsymmetryLeftRight_Roll_vs_Sector_Wheel-2", 'description': asymmetry+rpclink  }]
          )

###---> NumberOfDigi_Roll_vs_Sector_Wheel
rpclayout(dqmitems, "RPC_Summary/RPC_NumberOfDigi",
          [{ 'path': "RPC/RecHits/SummaryHistograms/NumberOfDigi_Mean_Roll_vs_Sector_Wheel2", 'description': numberofdigi+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/NumberOfDigi_Mean_Roll_vs_Sector_Wheel1", 'description': numberofdigi+rpclink }],

          [{ 'path': "RPC/RecHits/SummaryHistograms/NumberOfDigi_Mean_Roll_vs_Sector_Wheel0", 'description': numberofdigi+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/NumberOfDigi_Mean_Roll_vs_Sector_Wheel-1", 'description': numberofdigi+rpclink },
           { 'path': "RPC/RecHits/SummaryHistograms/NumberOfDigi_Mean_Roll_vs_Sector_Wheel-2", 'description': numberofdigi+rpclink }]
          )

###---> RPCNoisyStrips_Roll_vs_Sector_Wheel
rpclayout(dqmitems, "RPC_Summary/RPC_NoisyStrips",
          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCNoisyStrips_Roll_vs_Sector_Wheel2", 'description': noisy+rpclink },
           { 'path': "RPC/RecHits/SummaryHistograms/RPCNoisyStrips_Roll_vs_Sector_Wheel1", 'description': noisy+rpclink }],

          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCNoisyStrips_Roll_vs_Sector_Wheel0", 'description': noisy+rpclink },
           { 'path': "RPC/RecHits/SummaryHistograms/RPCNoisyStrips_Roll_vs_Sector_Wheel-1", 'description': noisy+rpclink  },
           { 'path': "RPC/RecHits/SummaryHistograms/RPCNoisyStrips_Roll_vs_Sector_Wheel-2", 'description': noisy+rpclink }]
          )



############### Summary Collection by Wheels ######################
###---> Wheel2 Sumaries <---###
rpclayout(dqmitems, "SummariesByWheels/Wheel+2_Summaries",
          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel2", 'description': top+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/DeadChannelFraction_Roll_vs_Sector_Wheel2", 'description':  dead+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/ClusterSizeIn1Bin_Roll_vs_Sector_Wheel2", 'description':  onebin+rpclink}],
          
          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCNoisyStrips_Roll_vs_Sector_Wheel2", 'description':  noisy+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/NumberOfDigi_Mean_Roll_vs_Sector_Wheel2", 'description': numberofdigi+rpclink},
           {'path': "RPC/RecHits/SummaryHistograms/AsymmetryLeftRight_Roll_vs_Sector_Wheel2", 'description': asymmetry+rpclink}],
          )

###---> Wheel1 Sumaries
rpclayout(dqmitems, "SummariesByWheels/Wheel+1_Summaries",
          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel1", 'description': top+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/DeadChannelFraction_Roll_vs_Sector_Wheel1", 'description':  dead+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/ClusterSizeIn1Bin_Roll_vs_Sector_Wheel1", 'description':  onebin+rpclink}],
          
          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCNoisyStrips_Roll_vs_Sector_Wheel1", 'description':  noisy+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/NumberOfDigi_Mean_Roll_vs_Sector_Wheel1", 'description': numberofdigi+rpclink},
           {'path': "RPC/RecHits/SummaryHistograms/AsymmetryLeftRight_Roll_vs_Sector_Wheel1", 'description': asymmetry+rpclink}],
          )
        
###---> Wheel0 Sumaries
rpclayout(dqmitems, "SummariesByWheels/Wheel0_Summaries",
          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel0", 'description': top+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/DeadChannelFraction_Roll_vs_Sector_Wheel0", 'description':  dead+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/ClusterSizeIn1Bin_Roll_vs_Sector_Wheel0", 'description':  onebin+rpclink}],
          
          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCNoisyStrips_Roll_vs_Sector_Wheel0", 'description':  noisy+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/NumberOfDigi_Mean_Roll_vs_Sector_Wheel0", 'description': numberofdigi+rpclink},
           {'path': "RPC/RecHits/SummaryHistograms/AsymmetryLeftRight_Roll_vs_Sector_Wheel0", 'description': asymmetry+rpclink}],
          )

###---> Wheel-1 Sumaries
rpclayout(dqmitems, "SummariesByWheels/Wheel-1_Summaries",
          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel-1", 'description': top+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/DeadChannelFraction_Roll_vs_Sector_Wheel-1", 'description':  dead+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/ClusterSizeIn1Bin_Roll_vs_Sector_Wheel-1", 'description':  onebin+rpclink}],
          
          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCNoisyStrips_Roll_vs_Sector_Wheel-1", 'description':  noisy+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/NumberOfDigi_Mean_Roll_vs_Sector_Wheel-1", 'description': numberofdigi+rpclink},
           {'path': "RPC/RecHits/SummaryHistograms/AsymmetryLeftRight_Roll_vs_Sector_Wheel-1", 'description': asymmetry+rpclink}],
          )

###---> Wheel_-2 Sumaries
rpclayout(dqmitems, "SummariesByWheels/Wheel-2_Summaries",
          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCChamberQuality_Roll_vs_Sector_Wheel-2", 'description': top+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/DeadChannelFraction_Roll_vs_Sector_Wheel-2", 'description':  dead+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/ClusterSizeIn1Bin_Roll_vs_Sector_Wheel-2", 'description':  onebin+rpclink}],
          
          [{ 'path': "RPC/RecHits/SummaryHistograms/RPCNoisyStrips_Roll_vs_Sector_Wheel-2", 'description':  noisy+rpclink},
           { 'path': "RPC/RecHits/SummaryHistograms/NumberOfDigi_Mean_Roll_vs_Sector_Wheel-2", 'description': numberofdigi+rpclink},
           {'path': "RPC/RecHits/SummaryHistograms/AsymmetryLeftRight_Roll_vs_Sector_Wheel-2", 'description': asymmetry+rpclink}],
          )

##################### Occupancy Histograms Layouts #############################
rpclayout(dqmitems, "Occupancy_Summary/Wheels/RPC_BarrelOccupancy",
          [{ 'path': "RPC/RecHits/SummaryHistograms/Occupancy_for_Barrel", 'description': "RPC BarrelOccupancy Summary view" + rpclink }])

rpclayout(dqmitems, "Occupancy_Summary/Wheels/RPC_EndCapOccupancy",
          [{ 'path': "RPC/RecHits/SummaryHistograms/Occupancy_for_EndcapPositive", 'description': "RPC EndCap Occupancy Summary view  " + rpclink },
          { 'path': "RPC/RecHits/SummaryHistograms/Occupancy_for_EndcapNegative", 'description': "RPC EndCap Occupancy Summary view  " + rpclink }])

rpclayout(dqmitems, "Occupancy_Summary/Wheels/RPC_Wheel_Occupancy",
          [{'path': "RPC/RecHits/SummaryHistograms/1DOccupancy_Wheel_2", 'description': "1DOccupancy_Wheel_2  " + rpclink},
           {'path': "RPC/RecHits/SummaryHistograms/1DOccupancy_Wheel_1", 'description': "1DOccupancy_Wheel_1  " + rpclink}],
          
          [{'path': "RPC/RecHits/SummaryHistograms/1DOccupancy_Wheel_0", 'description': "1DOccupancy_Wheel_0  " + rpclink},
           {'path': "RPC/RecHits/SummaryHistograms/1DOccupancy_Wheel_-1", 'description': "1DOccupancy_Wheel_-1  " + rpclink},
           {'path': "RPC/RecHits/SummaryHistograms/1DOccupancy_Wheel_-2", 'description': "1DOccupancy_Wheel_-2  " + rpclink}])

WheelOcup = "Normalized Occupancy summary view for Wheel";
rpclayout(dqmitems, "Occupancy_Summary/Wheels/Normalized_Occupancy",
          [{'path': "RPC/RecHits/SummaryHistograms/OccupancyNormByGeoAndRPCEvents_Wheel2",  'description': WheelOcup+"+2"+rpclink},
           {'path': "RPC/RecHits/SummaryHistograms/OccupancyNormByGeoAndRPCEvents_Wheel1",  'description': WheelOcup+"+1"+rpclink}],
          [{'path': "RPC/RecHits/SummaryHistograms/OccupancyNormByGeoAndRPCEvents_Wheel0",  'description': WheelOcup+" 0"+rpclink},
           {'path': "RPC/RecHits/SummaryHistograms/OccupancyNormByGeoAndRPCEvents_Wheel-1",  'description': WheelOcup+"-1"+rpclink},
           {'path': "RPC/RecHits/SummaryHistograms/OccupancyNormByGeoAndRPCEvents_Wheel-2",  'description': WheelOcup+"-2"+rpclink}]
)


WheelOcup = "Normalized Occupancy summary view for Wheel";
rpclayout(dqmitems, "Occupancy_Summary/Wheels/Occupancy",
          [{'path': "RPC/RecHits/SummaryHistograms/Occupancy_Roll_vs_Sector_Wheel_2",  'description': WheelOcup+"+2"+rpclink},
           {'path': "RPC/RecHits/SummaryHistograms/Occupancy_Roll_vs_Sector_Wheel_1",  'description': WheelOcup+"+1"+rpclink}],
          [{'path': "RPC/RecHits/SummaryHistograms/Occupancy_Roll_vs_Sector_Wheel_0",  'description': WheelOcup+" 0"+rpclink},
           {'path': "RPC/RecHits/SummaryHistograms/Occupancy_Roll_vs_Sector_Wheel_-1",  'description': WheelOcup+"-1"+rpclink},
           {'path': "RPC/RecHits/SummaryHistograms/Occupancy_Roll_vs_Sector_Wheel_-2",  'description': WheelOcup+"-2"+rpclink}]
)


rpclayout(dqmitems, "Occupancy_Summary/Sectors/SectorOccupancy_Wheel_2",
          [{'path': "RPC/RecHits/Barrel/Wheel_2/SummaryBySectors/Occupancy_Wheel_2_Sector_1", 'description': "Occupancy_Wheel_2_Sector_1  " + rpclink},
           {'path': "RPC/RecHits/Barrel/Wheel_2/SummaryBySectors/Occupancy_Wheel_2_Sector_2", 'description': " Occupancy_Wheel_2_Sector_2 " + rpclink},
           {'path': "RPC/RecHits/Barrel/Wheel_2/SummaryBySectors/Occupancy_Wheel_2_Sector_3", 'description': "Occupancy_Wheel_2_Sector_3  " + rpclink}],
          
          [{'path': "RPC/RecHits/Barrel/Wheel_2/SummaryBySectors/Occupancy_Wheel_2_Sector_4", 'description': "Occupancy_Wheel_2_Sector_4  " + rpclink},
           {'path': "RPC/RecHits/Barrel/Wheel_2/SummaryBySectors/Occupancy_Wheel_2_Sector_5", 'description': "Occupancy_Wheel_2_Sector_5  " + rpclink},
           {'path': "RPC/RecHits/Barrel/Wheel_2/SummaryBySectors/Occupancy_Wheel_2_Sector_6", 'description': "Occupancy_Wheel_2_Sector_6  " + rpclink}],
          
          [{'path': "RPC/RecHits/Barrel/Wheel_2/SummaryBySectors/Occupancy_Wheel_2_Sector_7", 'description': "Occupancy_Wheel_2_Sector_7  " + rpclink},
           {'path': "RPC/RecHits/Barrel/Wheel_2/SummaryBySectors/Occupancy_Wheel_2_Sector_8", 'description': "Occupancy_Wheel_2_Sector_8  " + rpclink},
           {'path': "RPC/RecHits/Barrel/Wheel_2/SummaryBySectors/Occupancy_Wheel_2_Sector_9", 'description': "Occupancy_Wheel_2_Sector_9  " + rpclink}],
          
          [ {'path': "RPC/RecHits/Barrel/Wheel_2/SummaryBySectors/Occupancy_Wheel_2_Sector_10", 'description': "Occupancy_Wheel_2_Sector_10  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_2/SummaryBySectors/Occupancy_Wheel_2_Sector_11", 'description': "Occupancy_Wheel_2_Sector_11  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_2/SummaryBySectors/Occupancy_Wheel_2_Sector_12", 'description': " Occupancy_Wheel_2_Sector_12 " + rpclink}])

rpclayout(dqmitems, "Occupancy_Summary/Sectors/SectorOccupancy_Wheel_1",
          [ { 'path': "RPC/RecHits/Barrel/Wheel_1/SummaryBySectors/Occupancy_Wheel_1_Sector_1", 'description': "Occupancy_Wheel_1_Sector_1  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_1/SummaryBySectors/Occupancy_Wheel_1_Sector_2", 'description': "Occupancy_Wheel_1_Sector_2  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_1/SummaryBySectors/Occupancy_Wheel_1_Sector_3", 'description': "Occupancy_Wheel_1_Sector_3  " + rpclink}],
          
          [ { 'path': "RPC/RecHits/Barrel/Wheel_1/SummaryBySectors/Occupancy_Wheel_1_Sector_4", 'description': "Occupancy_Wheel_1_Sector_4  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_1/SummaryBySectors/Occupancy_Wheel_1_Sector_5", 'description': "Occupancy_Wheel_1_Sector_5  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_1/SummaryBySectors/Occupancy_Wheel_1_Sector_6", 'description': "Occupancy_Wheel_1_Sector_6  " + rpclink}],
          
          [ { 'path': "RPC/RecHits/Barrel/Wheel_1/SummaryBySectors/Occupancy_Wheel_1_Sector_7", 'description': "Occupancy_Wheel_1_Sector_7  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_1/SummaryBySectors/Occupancy_Wheel_1_Sector_8", 'description': "Occupancy_Wheel_1_Sector_8  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_1/SummaryBySectors/Occupancy_Wheel_1_Sector_9", 'description': "Occupancy_Wheel_1_Sector_9  " + rpclink}],
          
          [ { 'path': "RPC/RecHits/Barrel/Wheel_1/SummaryBySectors/Occupancy_Wheel_1_Sector_10", 'description': "Occupancy_Wheel_1_Sector_10  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_1/SummaryBySectors/Occupancy_Wheel_1_Sector_11", 'description': "Occupancy_Wheel_1_Sector_11  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_1/SummaryBySectors/Occupancy_Wheel_1_Sector_12", 'description': "Occupancy_Wheel_1_Sector_12  " + rpclink}])


rpclayout(dqmitems, "Occupancy_Summary/Sectors/SectorOccupancy_Wheel_0",
          [ { 'path': "RPC/RecHits/Barrel/Wheel_0/SummaryBySectors/Occupancy_Wheel_0_Sector_1", 'description': "Occupancy_Wheel_0_Sector_1  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_0/SummaryBySectors/Occupancy_Wheel_0_Sector_2", 'description': "Occupancy_Wheel_0_Sector_2  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_0/SummaryBySectors/Occupancy_Wheel_0_Sector_3", 'description': "Occupancy_Wheel_0_Sector_3  " + rpclink}],
          
          [ { 'path': "RPC/RecHits/Barrel/Wheel_0/SummaryBySectors/Occupancy_Wheel_0_Sector_4", 'description': "Occupancy_Wheel_0_Sector_4  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_0/SummaryBySectors/Occupancy_Wheel_0_Sector_5", 'description': "Occupancy_Wheel_0_Sector_5  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_0/SummaryBySectors/Occupancy_Wheel_0_Sector_6", 'description': "Occupancy_Wheel_0_Sector_6  " + rpclink}],
          
          [ { 'path': "RPC/RecHits/Barrel/Wheel_0/SummaryBySectors/Occupancy_Wheel_0_Sector_7", 'description': "Occupancy_Wheel_0_Sector_7  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_0/SummaryBySectors/Occupancy_Wheel_0_Sector_8", 'description': "Occupancy_Wheel_0_Sector_8  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_0/SummaryBySectors/Occupancy_Wheel_0_Sector_9", 'description': "Occupancy_Wheel_0_Sector_9  " + rpclink}],
          
          [ { 'path': "RPC/RecHits/Barrel/Wheel_0/SummaryBySectors/Occupancy_Wheel_0_Sector_10", 'description': "Occupancy_Wheel_0_Sector_10  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_0/SummaryBySectors/Occupancy_Wheel_0_Sector_11", 'description': "Occupancy_Wheel_0_Sector_11  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_0/SummaryBySectors/Occupancy_Wheel_0_Sector_12", 'description': "Occupancy_Wheel_0_Sector_12  " + rpclink}])

rpclayout(dqmitems, "Occupancy_Summary/Sectors/SectorOccupancy_Wheel_-1",
          [ { 'path': "RPC/RecHits/Barrel/Wheel_-1/SummaryBySectors/Occupancy_Wheel_-1_Sector_1", 'description': "Occupancy_Wheel_-1_Sector_1  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-1/SummaryBySectors/Occupancy_Wheel_-1_Sector_2", 'description': "Occupancy_Wheel_-1_Sector_2  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-1/SummaryBySectors/Occupancy_Wheel_-1_Sector_3", 'description': "Occupancy_Wheel_-1_Sector_3  " + rpclink}],
          
          [ { 'path': "RPC/RecHits/Barrel/Wheel_-1/SummaryBySectors/Occupancy_Wheel_-1_Sector_4", 'description': "Occupancy_Wheel_-1_Sector_4  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-1/SummaryBySectors/Occupancy_Wheel_-1_Sector_5", 'description': "Occupancy_Wheel_-1_Sector_5  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-1/SummaryBySectors/Occupancy_Wheel_-1_Sector_6", 'description': "Occupancy_Wheel_-1_Sector_6  " + rpclink}],
          
          [ { 'path': "RPC/RecHits/Barrel/Wheel_-1/SummaryBySectors/Occupancy_Wheel_-1_Sector_7", 'description': "Occupancy_Wheel_-1_Sector_7  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-1/SummaryBySectors/Occupancy_Wheel_-1_Sector_8", 'description': "Occupancy_Wheel_-1_Sector_8  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-1/SummaryBySectors/Occupancy_Wheel_-1_Sector_9", 'description': " Occupancy_Wheel_-1_Sector_9 " + rpclink}],
          
          [ { 'path': "RPC/RecHits/Barrel/Wheel_-1/SummaryBySectors/Occupancy_Wheel_-1_Sector_10", 'description': "Occupancy_Wheel_-1_Sector_10  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-1/SummaryBySectors/Occupancy_Wheel_-1_Sector_11", 'description': "Occupancy_Wheel_-1_Sector_11  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-1/SummaryBySectors/Occupancy_Wheel_-1_Sector_12", 'description': "Occupancy_Wheel_-1_Sector_12  " + rpclink}])

rpclayout(dqmitems, "Occupancy_Summary/Sectors/SectorOccupancy_Wheel_-2",
          [ { 'path': "RPC/RecHits/Barrel/Wheel_-2/SummaryBySectors/Occupancy_Wheel_-2_Sector_1", 'description': "Occupancy_Wheel_-2_Sector_1  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-2/SummaryBySectors/Occupancy_Wheel_-2_Sector_2", 'description': "Occupancy_Wheel_-2_Sector_2  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-2/SummaryBySectors/Occupancy_Wheel_-2_Sector_3", 'description': "Occupancy_Wheel_-2_Sector_3  " + rpclink}],
          
          [ { 'path': "RPC/RecHits/Barrel/Wheel_-2/SummaryBySectors/Occupancy_Wheel_-2_Sector_4", 'description': "Occupancy_Wheel_-2_Sector_4  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-2/SummaryBySectors/Occupancy_Wheel_-2_Sector_5", 'description': "Occupancy_Wheel_-2_Sector_5  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-2/SummaryBySectors/Occupancy_Wheel_-2_Sector_6", 'description': "Occupancy_Wheel_-2_Sector_6  " + rpclink}],
          
          [ { 'path': "RPC/RecHits/Barrel/Wheel_-2/SummaryBySectors/Occupancy_Wheel_-2_Sector_7", 'description': "Occupancy_Wheel_-2_Sector_7  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-2/SummaryBySectors/Occupancy_Wheel_-2_Sector_8", 'description': "Occupancy_Wheel_-2_Sector_8  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-2/SummaryBySectors/Occupancy_Wheel_-2_Sector_9", 'description': "Occupancy_Wheel_-2_Sector_9  " + rpclink}],
          
          [ { 'path': "RPC/RecHits/Barrel/Wheel_-2/SummaryBySectors/Occupancy_Wheel_-2_Sector_10", 'description': "Occupancy_Wheel_-2_Sector_10  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-2/SummaryBySectors/Occupancy_Wheel_-2_Sector_11", 'description': "Occupancy_Wheel_-2_Sector_11  " + rpclink},
            { 'path': "RPC/RecHits/Barrel/Wheel_-2/SummaryBySectors/Occupancy_Wheel_-2_Sector_12", 'description': "Occupancy_Wheel_-2_Sector_12  " + rpclink}])
