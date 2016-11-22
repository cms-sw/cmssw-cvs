import os.path
global BASEDIR
BASEDIR = os.path.dirname(os.path.abspath(__file__)).rsplit('/', 1)[0]

modules = ("GuiDQM",)
envsetup = "export QUIET_ASSERT=a"

server.port        = 8030
server.serverDir   = BASEDIR + "/gui"
server.baseUrl     = '/dqm/ecal'
server.title       = 'CMS data quality'
server.serviceName = 'Ecal'

server.plugin('render', BASEDIR + "/style/*.cc")
server.extend('DQMRenderLink', server.pathOfPlugin('render'))

server.extend('DQMFileAccess', None, None,
              { "dqm": BASEDIR + "/files" })

server.source('DQMUnknown')
server.source('DQMOverlay')
server.source('DQMStripChart')
server.source('DQMLive', 'localhost:9090')
server.source('DQMArchive', BASEDIR + '/idx', '^/Global/')
server.source('DQMLayout',
              BASEDIR + '/config/ecal-layouts.py',
              BASEDIR + '/config/ecal_overview_layouts.py',
              BASEDIR + '/config/eb-layouts.py',
              BASEDIR + '/config/ee-layouts.py',
              BASEDIR + '/config/shift_eb_layout.py',
              BASEDIR + '/config/shift_ee_layout.py')

execfile(BASEDIR + "/config/dqm-services.py")
execfile(BASEDIR + "/config/workspaces-ecal.py")

