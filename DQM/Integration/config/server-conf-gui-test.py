import os.path
global CONFIGDIR
BASEDIR   = os.path.dirname(os.path.dirname(__file__))
CONFIGDIR = os.path.normcase(os.path.abspath(__file__)).rsplit('/', 1)[0]

LAYOUTS = ["%s/%s-layouts.py" % (CONFIGDIR, x) for x in
	   ("csc", "dt", "eb", "ee", "es","hcal", "hlt", "hlx", "l1t", "l1temulator", "rpc", "pixel", "sistrip")]
LAYOUTS += ["%s/shift_%s_layout.py" % (CONFIGDIR, x) for x in
            ("csc", "dt", "eb", "ee", "es","hcal", "hlt", "hlx", "l1t", "l1temulator", "rpc", "pixel", "sistrip" , "fed" )]

modules = ("GuiDQM", "GuiEventDisplay")
envsetup = "export QUIET_ASSERT=a"

#server.instrument  = 'valgrind --num-callers=999 `cmsvgsupp` --error-limit=no'
#server.instrument  = 'valgrind --tool=helgrind --num-callers=999 --error-limit=no'
#server.instrument  = 'igprof -t python -pp'
server.serverDir   = '/home/dqm/gui'
server.baseUrl     = '/dqm/gui-test'
server.title       = 'CMS data quality'
server.serviceName = 'GUI test'

server.plugin('render', BASEDIR + "/style/*.cc")
#server.extend('EVDSnapshotUpload', '/home/dqm/iguana-snapshots')
#server.source('EVDSnapshot', 'evd', '/home/dqm/iguana-snapshots')
server.source('DQMUnknown', 'unknown', 9091)
server.source('DQMLive', 'dqm', 'srv-c2d05-19:9090', '--listen 9091',
	      '--load ' + server.pathOfPlugin('render'))
server.source('DQMArchive', 'file',
              '/home/dqm/idx', '^/Global/', '--listen 9091',
              '--load ' + server.pathOfPlugin('render'))
server.source('DQMLayout', 'layouts', *LAYOUTS)

execfile(CONFIGDIR + "/dqm-services.py")
execfile(CONFIGDIR + "/workspaces-online.py")
