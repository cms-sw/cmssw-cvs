import os.path
global CONFIGDIR
BASEDIR   = os.path.dirname(os.path.dirname(__file__))
CONFIGDIR = os.path.normcase(os.path.abspath(__file__)).rsplit('/', 1)[0]

LAYOUTS = ["%s/shift_%s_T0_layout.py" % (CONFIGDIR, x) for x in
           ("eb", "ee", "csc", "rpc", "hcal", "l1t", "l1temulator", "hlt", "pixel", "sistrip", "dt", "muons", "jetmet", "egamma")]

modules = ("GuiDQM",)
envsetup = "export QUIET_ASSERT=a"

server.port        = 8060
server.serverDir   = '/data/dqm/dev/gui'
server.baseUrl     = '/dqm/dev'
server.title       = 'CMS data quality'
server.serviceName = 'CERN Development'

server.plugin('render', BASEDIR + "/style/*.cc")
server.source('DQMUnknown', 'unknown', 'DQMArchive', 8063)
server.source('DQMLive', 'live',
              '--listen 8062', '--collector localhost:8061',
              '--load ' + server.pathOfPlugin('render'))
server.source('DQMArchive', 'file',
              '/data/dqm/dev/dqm.db', '--listen 8063',
              '--load ' + server.pathOfPlugin('render'))
server.extend('DQMFileAccess', '/dev/null', '/data/dqm/dev/upload')

server.source('DQMLayout', 'layouts', *LAYOUTS)

execfile(CONFIGDIR + "/dqm-services.py")
execfile(CONFIGDIR + "/workspaces-dev.py")
