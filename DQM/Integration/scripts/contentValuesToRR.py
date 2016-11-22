#!/usr/bin/env python

from contentValuesLib import *
 
SERVER_URL = "http://cmswbm.cms/runregistry/xmlrpc"

class OptionParser(optparse.OptionParser):
  """ Option parser class """
  def __init__(self):
    optparse.OptionParser.__init__(self, usage="%prog [options] root_file ...", version="%prog 0.0.1", conflict_handler="resolve")
    self.add_option("--shift", "-s", action="store", type="choice", dest="shift", choices=("online","offline"), help="specify shift type: online or offline values allowed")
    self.add_option("--url", action="store", type="string", dest="url", default=SERVER_URL, help="specify RR XML-RPC server URL. Default is " + SERVER_URL)
    self.add_option("--debug", "-d", action="store_true", dest="debug", default=False, help="print values and exit. Do not write to RR")

if __name__ == "__main__":
  
  # Create option parser and get options/arguments
  optManager  = OptionParser()
  (opts, args) = optManager.parse_args()
  opts = opts.__dict__

  # Check if at least one root file defined (can be many!)
  if len(args) == 0:
    print "At least one ROOT file must be priovided, use --help for hit"
    sys.exit(1)

  # Check if shift type is provided (mandatory!)
  if not opts['shift']:
    print "Shift type must be provided, use --help for hit"
    sys.exit(1)

  # Check if all files exists and are accessible
  for rfile in args:
    try:
      os.stat(rfile)
    except:
      print "File [", rfile, "] not exists or is not accessible?"
      sys.exit(2)

  server = xmlrpclib.ServerProxy(opts['url'])

  # Lets extract values from files one-by-one, construct hashmap and submit to
  # defined XML-RPC url 
  for rfile in args:

    (run_number, values) = getSummaryValues(file_name = rfile, shift_type = opts['shift'], translate = True)
    dataset = getDatasetName(rfile)

    if run_number == None:
      print "Run number does not determined. Skipping file: %s" % rfile
      continue
    
    if dataset == None:
      print "Dataset name do not determined. Skipping file: %s" % rfile
      continue

    if values == None or len(values) == 0:
      print "No content summary values found. Skipping file: %s" % rfile
      continue

    try:
      json = dict2json(values)
      if opts['debug']:
        print "Run number: %d" % run_number
        print "Dataset: %s" % dataset
        print "Data: ", values
      else:
        result = server.SummaryValuesWriter.write(run_number, dataset, json)
        print "RR: %d rows modified for run# %d dataset %s" % (result, run_number, dataset)
    except xmlrpclib.Error, errstring:
      print "ERROR", errstring
      sys.exit(3)
  
  sys.exit(0)


