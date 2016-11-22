def shiftpixellayout(i, p, *rows): i["00 Shift/Pixel/" + p] = DQMItem(layout=rows)
shiftpixellayout(dqmitems, "00 - Pixel_Error_Summary",   
  [{ 'path': "Pixel/Barrel/SUMRAW_NErrors_Barrel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Endcap/SUMRAW_NErrors_Endcap", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/AdditionalPixelErrors/SUMRAW_NErrors_FEDErrors", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftSiPixel>Description for the Central DQM Shifter</a>"}]
)
shiftpixellayout(dqmitems, "01 - Pixel_DIGI_Summary",
  [{ 'path': "Pixel/Barrel/SUMDIG_adc_Barrel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Barrel/SUMDIG_ndigis_Barrel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Endcap/SUMDIG_adc_Endcap", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Endcap/SUMDIG_ndigis_Endcap", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftSiPixel>Description for the Central DQM Shifter</a>"}]
)
shiftpixellayout(dqmitems, "02 - Pixel_CLUSTER_Summary",
  [{ 'path': "Pixel/Barrel/SUMCLU_charge_Barrel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Barrel/SUMCLU_nclusters_Barrel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Barrel/SUMCLU_size_Barrel", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Endcap/SUMCLU_charge_Endcap", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Endcap/SUMCLU_nclusters_Endcap", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftSiPixel>Description for the Central DQM Shifter</a>"}],
  [{ 'path': "Pixel/Endcap/SUMCLU_size_Endcap", 'description': "<a href=https://twiki.cern.ch/twiki/bin/view/CMS/DQMShiftSiPixel>Description for the Central DQM Shifter</a>"}]
)
