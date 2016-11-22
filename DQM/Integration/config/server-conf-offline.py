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

server.port        = 8080
server.serverDir   = '/data/test/dqm/offline/gui'
server.baseUrl     = '/dqm/offline'
server.title       = 'CMS data quality'
server.serviceName = 'Offline'

server.plugin('render', BASEDIR + "/style/*.cc")
server.source('DQMUnknown', 'unknown', 8081)
server.source('DQMArchive', 'file', '/data/test/ix', '^/Global/', '--listen 8081',
              '--load ' + server.pathOfPlugin('render'))
server.extend('DQMFileAccess', '/dev/null', '/data/test/dqm/offline/upload')
server.source('DQMLayout', 'layouts', *LAYOUTS)

execfile(CONFIGDIR + "/dqm-services.py")
execfile(CONFIGDIR + "/workspaces-tier-0.py")
