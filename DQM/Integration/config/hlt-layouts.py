def hltlayout(i, p, *rows): i["HLT/Layouts/" + p] = DQMItem(layout=rows)

def hlt_evInfo_single(i, dir, name):
  i["HLT/Layouts/00-FourVector-Summary/%s" % name] = \
    DQMItem(layout=[["HLT/%s/%s" % (dir, name)]]) 



# list of summary GT histograms (dqmitems, dirPath , histoName)
hlt_evInfo_single(dqmitems, "FourVectorHLT", "HLT1Electron_etaphi")
