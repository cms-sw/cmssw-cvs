def shiftpixellayout(i, p, *rows): i["00 Shift/Pixel/" + p] = DQMItem(layout=rows)
shiftpixellayout(dqmitems, "00 - Pixel_ERROR_Summary",
  [{ 'path': "Pixel/Barrel/SUMOFF_NErrors_Barrel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Endcap/SUMOFF_NErrors_Endcap", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/AdditionalPixelErrors/SUMOFF_NErrors_FEDErrors", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}]
)
shiftpixellayout(dqmitems, "01 - Pixel_DIGI_Summary",
  [{ 'path': "Pixel/Barrel/SUMOFF_adc_Barrel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Barrel/SUMOFF_ndigis_Barrel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Endcap/SUMOFF_adc_Endcap", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Endcap/SUMOFF_ndigis_Endcap", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}]
)
shiftpixellayout(dqmitems, "02 - Pixel_CLUSTER_Summary",
  [{ 'path': "Pixel/Barrel/SUMOFF_charge_Barrel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Barrel/SUMOFF_nclusters_Barrel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Endcap/SUMOFF_charge_Endcap", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Endcap/SUMOFF_nclusters_Endcap", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Clusters/OnTrack/charge_siPixelClusters", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Clusters/OnTrack/size_siPixelClusters", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Clusters/OffTrack/charge_siPixelClusters", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Clusters/OffTrack/size_siPixelClusters", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}]
)
shiftpixellayout(dqmitems, "03 - Pixel_TRACK_Summary",
  [{ 'path': "Pixel/Tracks/ntracks_rsWithMaterialTracksP5", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftOfflineSiPixel>Description for the Central DQM Shifter</a>"}]
)
