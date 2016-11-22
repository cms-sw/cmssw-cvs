import os.path
global CONFIGDIR
BASEDIR   = os.path.dirname(os.path.dirname(__file__))
CONFIGDIR = os.path.normcase(os.path.abspath(__file__)).rsplit('/', 1)[0]

LAYOUTS = ["%s/shift_%s_T0_layout.py" % (CONFIGDIR, x) for x in
           ("eb", "ee", "csc", "rpc", "hcal", "l1t", "l1temulator", "hlt", "pixel", "sistrip", "dt", "muons", "jetmet", "egamma")]
LAYOUTS += ["%s/%s_T0_layouts.py" % (CONFIGDIR, x) for x in
           ("pixel","sistrip","hcal")]

modules = ("GuiDQM",)
envsetup = "export QUIET_ASSERT=a"

server.port        = 8070
server.serverDir   = '/data/dqm/tier-1/gui'
server.baseUrl     = '/dqm/tier-1'
server.title       = 'CMS data quality'
server.serviceName = 'Tier-1'

server.plugin('render', BASEDIR + "/style/*.cc")
server.source('DQMUnknown', 'unknown', 'DQMArchive', 8071)
server.source('DQMArchive', 'file',
              '/data/dqm/tier-1/dqm.db', '--listen 8071',
              '--load ' + server.pathOfPlugin('render'))
server.extend('DQMFileAccess', '/dev/null', '/data/dqm/tier-1/upload')
server.source('DQMLayout', 'layouts', *LAYOUTS)

execfile(CONFIGDIR + "/dqm-services.py")
execfile(CONFIGDIR + "/workspaces-tier-0.py")
