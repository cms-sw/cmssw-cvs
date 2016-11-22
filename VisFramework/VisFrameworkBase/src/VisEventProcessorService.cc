#include "VisFramework/VisFrameworkBase/interface/VisEventProcessorService.h"
#include "FWCore/Framework/interface/EventProcessor.h"
#include "FWCore/MessageLogger/interface/JobReport.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ServiceRegistry/interface/ServiceRegistry.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/Presence.h"
#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/PluginManager/interface/PresenceFactory.h"
#include "FWCore/PluginManager/interface/ProblemTracker.h"
#include "FWCore/PluginManager/interface/standard.h"
#include "Iguana/Framework/interface/IgArgsElement.h"
#include <QDateTime>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <fstream>
#include <sys/stat.h> 
#include <boost/program_options.hpp>

IG_DEFINE_STATE_ELEMENT (VisEventProcessorService, "Services/Framework/Visualisation Event Processor");

VisEventProcessorService::VisEventProcessorService (IgState *state)
    : m_ep (0),
      m_jobRep (new edm::JobReport ()),
      m_callEndJob (false),
      m_running (false),
      m_state (state)
{
    ASSERT (state);
    state->put (s_key, this);

    init ();
}

void
VisEventProcessorService::init (void) 
{
    if (this->m_onStatusMessage)
	this->m_onStatusMessage ("EventProcessor initialization...");
	
    // We must initialize the plug-in manager first
    try 
    {
	edmplugin::PluginManager::configure (edmplugin::standard::config ());
    } 
    catch (cms::Exception& e)
    {
	if (this->m_onCmsException)
	    this->m_onCmsException (&e);
    }
   
    try 
    {
	m_messageServicePresence = boost::shared_ptr<edm::Presence>(edm::PresenceFactory::get ()->
								    makePresence ("MessageServicePresence").release ());
    } 
    catch (cms::Exception& e) 
    {
	if (this->m_onCmsException)
	    this->m_onCmsException (&e);
    }
}

VisEventProcessorService::~VisEventProcessorService (void)
{
    ASSERT (m_state);
    if (m_ep.get () && m_callEndJob) m_ep->endJob ();
    m_state->detach (s_key);
}

std::string 
VisEventProcessorService::configFile (void)
{
    if (this->m_onStatusMessage)
	this->m_onStatusMessage ("Creating an empty configuration....");

    std::string configString = 
	"import FWCore.ParameterSet.Config as cms\n\n"
	"from FWCore.MessageLogger.MessageLogger_cfi import *\n"
	"process = cms.Process(\"IGUANA\")\n"
	"process.load(\"Configuration.StandardSequences.FrontierConditions_GlobalTag_cff\")\n"
        "process.GlobalTag.globaltag = \'MC_31X_V5::All\'\n"
	"process.load(\"Configuration.StandardSequences.Geometry_cff\")\n"
	"process.load(\"Configuration.StandardSequences.MagneticField_cff\")\n"
	"process.load(\"Configuration.StandardSequences.Reconstruction_cff\")\n\n"
	"process.load(\"VisFramework.VisFrameworkBase.VisConfigurationService_cff\")\n"
	"process.source = cms.Source(\"EmptySource\")\n";

    QDateTime dt = QDateTime::currentDateTime ();
    QString cfgFilename = "iguana-EmptySource-" + dt.toString ("hh:mm:ss.zzz-dd.MM.yyyy") + "_cfg.py";
    
    QFile iguanaConfigFile (cfgFilename);
    if (! iguanaConfigFile.exists ())
    {    
	if (iguanaConfigFile.open (QIODevice::WriteOnly)) 
	{
	    QTextStream cfgStream (&iguanaConfigFile);
	    cfgStream << QString::fromStdString (configString);
	    iguanaConfigFile.close ();
	}	
    }

    return cfgFilename.latin1 ();
}

std::string 
VisEventProcessorService::configRootFile (std::string rootFileName)
{
    std::string configstring =	
	"import FWCore.ParameterSet.Config as cms\n\n"
	"from FWCore.MessageLogger.MessageLogger_cfi import *\n"
	"process = cms.Process(\"IGUANA\")\n"
	"process.load(\"Configuration.StandardSequences.FrontierConditions_GlobalTag_cff\")\n"
        "process.GlobalTag.globaltag = \'MC_31X_V5::All\'\n"
	"process.load(\"Configuration.StandardSequences.Geometry_cff\")\n"
	"process.load(\"Configuration.StandardSequences.MagneticField_cff\")\n"
	"process.load(\"Configuration.StandardSequences.Reconstruction_cff\")\n\n"
	"process.load(\"VisFramework.VisFrameworkBase.VisConfigurationService_cff\")\n"
	"process.source = cms.Source(\"PoolSource\",\n"
	"    fileNames = cms.untracked.vstring('";

    if (rootFileName.compare (0, 7, "/castor") == 0)
	configstring += "rfio:";
    else if (rootFileName.compare (0, 6, "/store") == 0)
	configstring += "";
    else
    {
	struct stat stFileInfo;
	int intStat = stat (rootFileName.c_str (), &stFileInfo);
	if (intStat == 0)
	    configstring += "file:";
    }
    
    configstring += rootFileName;
    configstring += "')\n)\n";

    configstring += "process.VisConfigurationService.EnabledTwigs = ('* Muon *',\n"
                    "                                                '* Track *',\n"
                    "                                                '*Segment*',\n"
                    "                                                '* Muon *',\n"
                    "                                                '* Tracking RecHit *',\n"
                    "                                                '/Objects/CMS Event and Detector/Magnet',\n"
		    "                                                '/Objects/Event Collections/Run and Event Number')\n\n";
    
    QDateTime dt = QDateTime::currentDateTime ();
    QString cfgFilename = "iguana-" + dt.toString ("hh:mm:ss.zzz-dd.MM.yyyy") + "_cfg.py";
    
    QFile iguanaConfigFile (cfgFilename);
    if (! iguanaConfigFile.exists ())
    {    
	if (iguanaConfigFile.open (QIODevice::WriteOnly)) 
	{
	    QTextStream cfgStream (&iguanaConfigFile);
	    cfgStream << QString::fromStdString (configstring);
	    iguanaConfigFile.close ();
	}	
    }

    return cfgFilename.latin1 ();
}

void
VisEventProcessorService::initEventProcessor (void)
{    
    IgArgsElement *args = IgArgsElement::get (m_state);
    m_running = true;

    char const* const kParameterSetOpt = "parameter-set";    
    char const* const kParameterSetCommandOpt = "parameter-set,p";
    char const* const kJobreportCommandOpt = "jobreport,j";
    char const* const kEnableJobreportCommandOpt = "enablejobreport,e";
    char const* const kJobModeCommandOpt = "mode,m";
    char const* const kHelpCommandOpt = "help,h";
    char const* const kStrictOpt = "strict";

    std::string descString (args->argv ()[0]);
    descString += " [options] [--";
    descString += kParameterSetOpt;
    descString += "] config_file \nAllowed options";
    boost::program_options::options_description desc (descString);
    
    desc.add_options ()
	(kHelpCommandOpt, "produce help message")
	(kParameterSetCommandOpt, boost::program_options::value<std::string> (), "configuration file")
	(kJobreportCommandOpt, boost::program_options::value<std::string> (),
         "file name to use for a job report file: default extension is .xml")
	(kEnableJobreportCommandOpt, 
         "enable job report files (if any) specified in configuration file")
	(kJobModeCommandOpt, boost::program_options::value<std::string> (),
         "Job Mode for MessageLogger defaults - default mode is grid")
	(kStrictOpt, "strict parsing");

    boost::program_options::positional_options_description p;
    p.add (kParameterSetOpt, -1);

    boost::program_options::options_description all_options ("All Options");
    all_options.add (desc);
    
    boost::program_options::variables_map vm;
    try 
    {
	store (boost::program_options::command_line_parser (args->args (),args->argv ()).options (all_options).positional (p).run (), vm);
	notify (vm);
    } 
    catch (boost::program_options::error const& iException) 
    {
	edm::LogError ("FwkJob") << "Exception from command line processing: " << iException.what ();
	edm::LogSystem ("CommandLineProcessing") << "Exception from command line processing: " << iException.what () << "\n";
	return;
    }    
    try 
    {
	//
	// Decide whether to enable creation of job report xml file 
	//  We do this first so any errors will be reported
	// 
	std::auto_ptr<std::ofstream> jobReportStreamPtr;
	if (vm.count ("jobreport")) 
	{
	    std::string jobReportFile = vm ["jobreport"].as<std::string> ();
	    jobReportStreamPtr = std::auto_ptr<std::ofstream> (new std::ofstream (jobReportFile.c_str ()));
	} 
	else if (vm.count ("enablejobreport")) 
	{
	    jobReportStreamPtr = std::auto_ptr<std::ofstream> (new std::ofstream ("IguanaJobReport.xml"));
	} 
	//
	// Make JobReport Service up front
	// 
	//NOTE: JobReport must have a lifetime shorter than jobReportStreamPtr so that when the JobReport destructor
	// is called jobReportStreamPtr is still valid	
	// Make JobReport Service up front
	std::auto_ptr<edm::JobReport> jobRep (new edm::JobReport (jobReportStreamPtr.get ()));  
	edm::ServiceToken jobReportToken = 
	    edm::ServiceRegistry::createContaining (jobRep);
	
	std::string fileName;
	
	if (vm.count (kParameterSetOpt)) 
	    fileName = (vm [kParameterSetOpt].as<std::string> ());

	boost::shared_ptr<edm::ProcessDesc> processDesc;
	if (fileName.size() > 4 && fileName.substr (fileName.size () - 5) == ".root")
	{
	    std::string configString = configRootFile (fileName);
	    fileName = configString;
	}
	else if (fileName.empty ()) 
	{
	    std::string configString = configFile ();
	    fileName = configString;
	}
	processDesc = edm::readConfigFile (fileName);

	if (this->m_onStartingProcessor)
	    this->m_onStartingProcessor ();
	
	// Specify default services to be enabled with their default parameters.
	// The parameters for these can be overridden from the configuration files.
	std::vector<std::string> defaultServices;
	defaultServices.reserve (5);

	// These cannot be overridden from the configuration files.
	// An exception will be thrown if any of these is specified there.
	std::vector<std::string> forcedServices;
	forcedServices.reserve(2);

	defaultServices.push_back ("MessageLogger");
	defaultServices.push_back ("InitRootHandlers");
	defaultServices.push_back ("AdaptorConfig");
	defaultServices.push_back ("EnableFloatingPointExceptions");
	defaultServices.push_back ("UnixSignalService");

	forcedServices.push_back ("JobReportService");
	forcedServices.push_back ("SiteLocalConfigService");

	processDesc->addServices(defaultServices, forcedServices);

	if (this->m_onStatusMessage)
	    this->m_onStatusMessage ("Waiting for CMSSW to load EventProcessor and its services. Please, be patient, it takes a while.........");

	std::auto_ptr<edm::EventProcessor> procP (new edm::EventProcessor (processDesc, 
									   jobReportToken, 
									   edm::serviceregistry::kTokenOverrides));
	m_ep = procP;
	    
	if (this->m_onProcessorStarted)
	    this->m_onProcessorStarted ();	    
    }
    catch (lat::Error &e)
    {
	if (this->m_onFrameworkException)
	    this->m_onFrameworkException (&e);
    }
    catch (std::exception &e)
    {
	if (this->m_onException)
	    this->m_onException (&e);
    }
    catch (...)
    {
	if (this->m_onUnhandledException)
	    this->m_onUnhandledException ();
    }
}

void
VisEventProcessorService::onStartingProcessor (lat::Callback callback)
{
    m_onStartingProcessor = callback;    
}

void
VisEventProcessorService::onStoppingProcessor (lat::Callback callback)
{
    m_onStoppingProcessor = callback;    
}

void
VisEventProcessorService::onProcessorStarted (lat::Callback callback)
{
    m_onProcessorStarted = callback;    
}

void
VisEventProcessorService::onProcessorStopped (lat::Callback callback)
{
    m_onProcessorStopped = callback;    
}

void
VisEventProcessorService::onException (lat::Callback1<std::exception *> callback)
{
    m_onException = callback;    
}

void
VisEventProcessorService::onCmsException (lat::Callback1<cms::Exception *> callback)
{
    m_onCmsException = callback;    
}

void
VisEventProcessorService::onUnhandledException (lat::Callback callback)
{
    m_onUnhandledException = callback;    
}

void
VisEventProcessorService::onFrameworkException (lat::Callback1<lat::Error *> callback)
{
    m_onFrameworkException = callback;    
}

void
VisEventProcessorService::onConfigFailure (lat::Callback1<cms::Exception *> callback)
{
    m_onConfigFailure = callback;    
}

void
VisEventProcessorService::onStatusMessage (lat::Callback1<const std::string&> callback)
{
    m_onStatusMessage = callback;    
}

void
VisEventProcessorService::scheduleAbort  (void)
{
    m_running = false;
}

bool
VisEventProcessorService::running  (void)
{
    return m_running;
}
