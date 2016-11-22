import os.path
global CONFIGDIR
BASEDIR   = os.path.dirname(os.path.dirname(__file__))
CONFIGDIR = os.path.normcase(os.path.abspath(__file__)).rsplit('/', 1)[0]

LAYOUTS = ["%s/%s-layouts.py" % (CONFIGDIR, x) for x in
	   ("csc", "dt", "eb", "ee", "hcal", "hlt", "hlx", "l1t", "l1temulator", "rpc", "pixel", "sistrip")]
LAYOUTS += ["%s/shift_%s_layout.py" % (CONFIGDIR, x) for x in
            ("csc", "dt", "eb", "ee", "hcal", "hlt", "hlx", "l1t", "l1temulator", "rpc", "pixel", "sistrip" , "fed" )]

modules = ("GuiDQM",)
envsetup = "export QUIET_ASSERT=a"

server.port        = 8888
server.serverDir   = BASEDIR + "/gui"
server.baseUrl     = '/dqm/devtest'
server.title       = 'CMS data quality'
server.serviceName = 'GUI test'

server.plugin('render', BASEDIR + "/style/*.cc")
server.source('DQMUnknown', 'unknown', 9191)
server.source('DQMLive', 'dqm', 'localhost:9190', '--listen 9191',
              '--load ' + server.pathOfPlugin('render'))
server.source('DQMArchive', 'file', BASEDIR + "/idx", '^/Global/',
              '--listen 9191', '--load ' + server.pathOfPlugin('render'))
server.source('DQMLayout', 'layouts', *LAYOUTS)

execfile(CONFIGDIR + "/dqm-services.py")
execfile(CONFIGDIR + "/workspaces-online.py")
