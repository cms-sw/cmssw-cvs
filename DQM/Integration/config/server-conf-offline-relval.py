import os.path
global CONFIGDIR
BASEDIR   = os.path.dirname(os.path.dirname(__file__))
CONFIGDIR = os.path.normcase(os.path.abspath(__file__)).rsplit('/', 1)[0]

LAYOUTS = [CONFIGDIR + "/shift_hlt_relval_layout.py"]
LAYOUTS += [CONFIGDIR + "/hlt_relval-layouts.py"]

modules = ("GuiDQM",)
envsetup = "export QUIET_ASSERT=a"

server.port        = 8050
server.serverDir   = '/data/dqm/relval/gui'
server.baseUrl     = '/dqm/relval'
server.title       = 'CMS data quality'
server.serviceName = 'CERN RelVal'

server.plugin('render', BASEDIR + "/style/*.cc")
server.source('DQMUnknown', 'unknown', 'DQMArchive', 8051)
server.source('DQMArchive', 'file',
              '/data/dqm/relval/dqm.db', '--listen 8051',
              '--load ' + server.pathOfPlugin('render'))
server.extend('DQMFileAccess', '/dev/null', '/data/dqm/relval/upload')
server.source('DQMLayout', 'layouts', *LAYOUTS)

execfile(CONFIGDIR + "/dqm-services.py")
execfile(CONFIGDIR + "/workspaces-relval.py")
