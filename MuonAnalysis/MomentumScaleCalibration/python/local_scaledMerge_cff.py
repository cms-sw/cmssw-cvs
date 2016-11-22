import FWCore.ParameterSet.Config as cms

source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
    "file:/data2/demattia/Data/CSA08/SubSample/ZsubSample/ZsubSample.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_0.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_10.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_11.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_12.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_13.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_14.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_15.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_16.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_17.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_18.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_19.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_1.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_20.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_21.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_22.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_23.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_24.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_25.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_26.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_27.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_28.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_29.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_2.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_30.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_31.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_32.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_33.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_34.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_35.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_36.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_37.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_38.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_39.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_3.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_40.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_41.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_42.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_43.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_44.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_45.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_46.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_47.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_48.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_49.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_4.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_50.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_51.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_52.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_53.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_54.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_55.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_56.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_57.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_58.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_59.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_5.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_60.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_61.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_62.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_63.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_64.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_65.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_66.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_67.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_68.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_69.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_6.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_70.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_71.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_72.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_73.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_74.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_75.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_76.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_77.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_78.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_79.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_7.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_80.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_81.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_82.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_83.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_84.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_85.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_86.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_87.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_88.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_89.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_8.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_90.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_91.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_92.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_93.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_94.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_95.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_96.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_97.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_98.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_99.root",
    "file:/data2/demattia/Data/CSA08/Y/Filter_Y_9.root"
    )
    )
