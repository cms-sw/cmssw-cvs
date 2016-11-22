import sys, os, optparse, re
import ROOT, xmlrpclib
 
FOLDERS     = { 
  'reportSummaryContents': 'DQM', 
  'CertificationContents': 'Cert', 
  'DAQContents': 'DAQ', 
  'DCSContents': 'DCS' 
}

SUBSYSTEMS  = {
  'CSC' :        'CSC',
  'DT' :         'DT',
  'EcalBarrel' : 'ECAL',
  'EcalEndcap' : 'ECAL',
  'Hcal' :       'HCAL',
  'L1T' :        'L1T',
  'L1TEMU' :     'L1T',
  'Pixel' :      'PIX',
  'RPC' :        'RPC',
  'SiStrip' :    'STRIP'
}

def getDatasetName(file_name):
  """ Method to get dataset name from the file name"""
  d = None
  try:
    d = re.search("(__[a-zA-Z0-9-_]+)+", file_name).group(0)
    d = re.sub("__", "/", d)
  except:
    d = None
  return d

def getSummaryValues(file_name, shift_type, translate):
  """ Method to extract keys from root file and return dict """
  ROOT.gROOT.Reset()

  run_number = None
  result = {}

  f = ROOT.TFile(file_name, 'READ')

  root = f.GetDirectory("DQMData")
  if root == None: return (run_number, result)
  
  run = None
  for key in root.GetListOfKeys():
    if re.match("^Run [0-9]+$", key.ReadObj().GetName()) and key.IsFolder():
      run_number = int(re.sub("^Run ", "", key.ReadObj().GetName()))
      run = key.ReadObj()
      break

  if run == None: return (run_number, result)

  for sub in run.GetListOfKeys():

    sub_name = sub.ReadObj().GetName()
    if not SUBSYSTEMS.has_key(sub_name): continue

    sub_key = sub_name
    if translate:
      sub_key = SUBSYSTEMS[sub_name]

    if not result.has_key(sub_key):
      result[sub_key] = {}

    evInfo = sub.ReadObj().GetDirectory("Run summary/EventInfo")
    if evInfo == None: continue

    for folder_name in FOLDERS.keys():

      folder = evInfo.GetDirectory(folder_name)
      if folder == None: continue
      
      folder_id = folder_name
      if translate:
        folder_id = FOLDERS[folder_name]
        if folder_id == 'DQM' and shift_type != None:
          folder_id = 'DQM ' + shift_type.upper()

      result[sub_key][folder_id] = {}

      for value in folder.GetListOfKeys():
        full_name = value.ReadObj().GetName()
        if not value.IsFolder() and re.match("^<.+>f=-{,1}[0-9\.]+</.+>$", full_name):
          value_name = re.sub("<(?P<n>[^>]+)>.+", "\g<n>", full_name)
          value_numb = float(re.sub("<.+>f=(?P<n>-{,1}[0-9\.]+)</.+>", "\g<n>", full_name))
          result[sub_key][folder_id][value_name] = value_numb

  f.Close()

  return (run_number, result)

def dict2json(d):
  """ Convert dictionary (embedded) to json string """
  s = '{'
  i = 0
  for k in d.keys():
    i = i + 1
    s = s + '"%s"=' % re.sub('"', '\\"', k)
    if type(d[k]) == type({}):
      s = s + dict2json(d[k])
    elif type(d[k]) == type(1.0) or type(d[k]) == type(1):
      s = s + str(d[k])
    else:
      s = s + '"%s"' % re.sub('"', '\\"', d[k])
    if i < len(d): s = s + ','  

  s = s + '}'
  return s

