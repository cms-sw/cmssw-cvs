import FWCore.ParameterSet.Config as cms

source = cms.Source("PoolSource",
                    fileNames = cms.untracked.vstring(
    "file:/data2/demattia/Data/MuonGun/MuonGun_0.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_10.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_11.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_12.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_13.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_14.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_15.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_16.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_17.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_18.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_19.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_1.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_20.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_21.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_22.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_23.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_24.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_25.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_26.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_27.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_28.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_29.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_2.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_30.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_31.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_32.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_33.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_34.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_35.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_36.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_37.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_38.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_39.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_3.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_40.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_41.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_42.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_43.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_44.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_45.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_46.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_47.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_48.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_49.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_4.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_50.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_51.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_52.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_53.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_54.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_55.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_56.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_57.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_58.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_59.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_5.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_60.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_61.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_62.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_63.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_64.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_65.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_66.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_67.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_68.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_69.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_6.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_70.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_71.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_72.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_73.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_74.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_75.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_76.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_77.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_78.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_79.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_7.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_80.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_81.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_82.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_83.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_84.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_85.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_86.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_87.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_88.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_89.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_8.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_90.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_91.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_92.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_93.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_94.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_95.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_96.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_97.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_98.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_99.root",
    "file:/data2/demattia/Data/MuonGun/MuonGun_9.root"
    )
                    )
