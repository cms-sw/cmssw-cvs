//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisFEDRawDataCollectionTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>
#include <DataFormats/FEDRawData/interface/FEDHeader.h>
#include <DataFormats/FEDRawData/interface/FEDTrailer.h>
#include <DataFormats/FEDRawData/interface/FEDNumbering.h> 
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/SbLinear.h>
#include <qstring.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <sys/time.h> 

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

static const unsigned int GTEVMId = 812;

//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

namespace 
{
    VisQueuedTwig *
    createThisTwig (IgState *state, IgTwig *parent,
		    const std::string &name,
		    const std::string &friendlyName,
		    const std::string &modLabel,
		    const std::string &instanceName,
		    const std::string &processName)
    {
	IgTwig *rootTwig = IgDocumentData::get (state)->root ();
	IgTwig *eventTwig = 0;
	eventTwig = rootTwig->lookup ("/Objects/Event Collections/FEDRawData");
	
	if (! eventTwig) 
	    eventTwig = parent;
		
	return new VisFEDRawDataCollectionTwig (state, eventTwig, "[N/A] FEDRawData (" + name + ")", 
						friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisFEDRawDataCollectionTwig::VisFEDRawDataCollectionTwig (IgState *state, IgTwig *parent,
							  const std::string &name /* = "" */,
							  const std::string &friendlyName /* = "" */,
							  const std::string &moduleLabel /* = "" */,
							  const std::string &instanceName /* = "" */,
							  const std::string &processName /* = "" */)
    : VisQueuedTwig (state, parent, name),
      m_text ("no info"),
      m_friendlyName (friendlyName),
      m_moduleLabel (moduleLabel),
      m_instanceName (instanceName),
      m_processName (processName)
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID id (typeid (FEDRawDataCollection));
    tfService->registerTwig (id.friendlyClassName (), &createThisTwig);
}

void
VisFEDRawDataCollectionTwig::onNewEvent (const edm::Event &event,
					 const edm::EventSetup &eventSetup)
{    
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    const edm::Timestamp time = event.time ();

    timeval eventTime;
    eventTime.tv_sec = time.value () >> 32;
    eventTime.tv_usec = 0xFFFFFFFF & time.value ();

    boost::posix_time::ptime bt0 = boost::posix_time::from_time_t(0);
    boost::posix_time::ptime bt = bt0 + boost::posix_time::seconds(eventTime.tv_sec)
      + boost::posix_time::microseconds(eventTime.tv_usec);

    std::stringstream oss;
    oss << bt;
    std::string contents (oss.str ());

    m_text = (QString ("%1 GMT: Run %2, Event %3, LS %4")
	      .arg (contents.c_str ())
	      .arg (event.id ().run ())
	      .arg (event.id ().event ())
 	      .arg (event.luminosityBlock ())
	      .latin1 ());

    typedef std::vector<edm::Handle<FEDRawDataCollection> > Collections;
    Collections rawdata;
    try 
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, rawdata);	    
	}
    }
    catch (cms::Exception& e)
    {
	if (this->m_onCmsException)
	    this->m_onCmsException (&e);
    }
    catch (lat::Error &e) 
    {
	if (this->m_onError)
	    this->m_onError (&e);
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
    if (! rawdata.empty ())
    {
        Collections::iterator i (rawdata.begin ()), end (rawdata.end ());
	for (; i != end; ++i)
	{
	  const FEDRawData& data = (*i)->FEDData(GTEVMId);
	  size_t size = data.size ();

	  QString sizeStr = (QString ("%1").arg (size));
	  QString nameStr = QString::fromStdString (this->name ());
	  int ib = nameStr.find ("[");
	  int ie = nameStr.find ("]");
	  nameStr.replace (ib + 1, ie - 1, sizeStr);
	    
	  this->name (nameStr.toStdString ());
	}
    }
    
    IgRepSet::invalidate (this, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}

void
VisFEDRawDataCollectionTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->setText (m_text);
}

void
VisFEDRawDataCollectionTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    SoAnnotation *sep = new SoAnnotation;
    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sep->addChild (mat);
    SoText2  *eventLabel = new SoText2;
    eventLabel->string = m_text.c_str ();

    SoFont* labelFont = new SoFont;
    labelFont->size.setValue (18.0);
    labelFont->name.setValue ("Arial");
    sep->addChild (labelFont);

    SoTranslation *eventLabelTranslation = new SoTranslation;
	    
    SbVec3f pos = SbVec3f (-11.0,
			   7.0,
			   0.0);

    eventLabelTranslation->translation = pos;
    sep->addChild (eventLabelTranslation);
    sep->addChild (eventLabel);

    rep->node ()->addChild (sep);
}

void
VisFEDRawDataCollectionTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    SoAnnotation *sep = new SoAnnotation;    

    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sep->addChild (mat);

    SoText2  *eventLabel = new SoText2;
    eventLabel->string = m_text.c_str ();

    SoFont* labelFont = new SoFont;
    labelFont->size.setValue (18.0);
    labelFont->name.setValue ("Arial");
    sep->addChild (labelFont);

    SoTranslation *eventLabelTranslation = new SoTranslation;
	    
    SbVec3f pos = SbVec3f (-8.0,
			   8.0,
			   0.0);

    eventLabelTranslation->translation = pos;
    sep->addChild (eventLabelTranslation);
    sep->addChild (eventLabel);

    rep->node ()->addChild (sep);
}

void
VisFEDRawDataCollectionTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    SoAnnotation *sep = new SoAnnotation;    

    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sep->addChild (mat);

    SoText2  *eventLabel = new SoText2;
    eventLabel->string = m_text.c_str ();

    SoFont* labelFont = new SoFont;
    labelFont->size.setValue (18.0);
    labelFont->name.setValue ("Arial");
    sep->addChild (labelFont);

    SoTranslation *eventLabelTranslation = new SoTranslation;
	    
    SbVec3f pos = SbVec3f (-11.0,
			   8.0,
			   -10.0);

    eventLabelTranslation->translation = pos;
    sep->addChild (eventLabelTranslation);
    sep->addChild (eventLabel);

    rep->node ()->addChild (sep);
}

void
VisFEDRawDataCollectionTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    SoAnnotation *sep = new SoAnnotation;    

    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0x8b898900, rgbcomponents); // snow4
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sep->addChild (mat);

    SoText2  *eventLabel = new SoText2;
    eventLabel->string = m_text.c_str ();

    SoFont* labelFont = new SoFont;
    labelFont->size.setValue (18.0);
    labelFont->name.setValue ("Arial");
    sep->addChild (labelFont);

    SoTranslation *eventLabelTranslation = new SoTranslation;
	    
    SbVec3f pos = SbVec3f (-11.0,
			   7.0,
			   0.0);

    eventLabelTranslation->translation = pos;
    sep->addChild (eventLabelTranslation);
    sep->addChild (eventLabel);

    rep->node ()->addChild (sep);
}
