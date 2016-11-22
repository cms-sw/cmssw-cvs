import os.path
global CONFIGDIR
BASEDIR   = os.path.dirname(os.path.dirname(__file__))
CONFIGDIR = os.path.normcase(os.path.abspath(__file__)).rsplit('/', 1)[0]

LAYOUTS = ["%s/%s-layouts.py" % (CONFIGDIR, x) for x in
	   ("csc", "dt", "eb", "ee", "hcal", "hlt", "hlx", "l1t", "l1temulator", "rpc", "pixel", "sistrip")]
LAYOUTS += ["%s/shift_%s_layout.py" % (CONFIGDIR, x) for x in
            ("csc", "dt", "eb", "ee", "hcal", "hlt", "hlx", "l1t", "l1temulator", "rpc", "pixel", "sistrip" , "fed" )]

modules = ("GuiDQM", "GuiEventDisplay")
envsetup = "export QUIET_ASSERT=a"

server.serverDir   = '/home/dqm/gui'
server.baseUrl     = '/dqm/online-test'
server.title       = 'CMS data quality'
server.serviceName = 'Online test'

server.plugin('render', BASEDIR + "/style/*.cc")
server.extend('EVDSnapshotUpload', '/home/dqm/iguana-snapshots')
server.source('EVDSnapshot', 'evd', '/home/dqm/iguana-snapshots')
server.source('DQMUnknown', 'unknown', 'DQMArchive', 9097)
server.source('DQMLive', 'dqm',
              '--listen 9091', '--collector localhost:9090',
              '--load ' + server.pathOfPlugin('render'))
server.source('DQMArchive', 'file',
              '/home/dqm/dqm.db', '--listen 9097',
              '--load ' + server.pathOfPlugin('render'))
server.source('DQMLayout', 'layouts', *LAYOUTS)

execfile(CONFIGDIR + "/dqm-services.py")
execfile(CONFIGDIR + "/workspaces-online.py")
