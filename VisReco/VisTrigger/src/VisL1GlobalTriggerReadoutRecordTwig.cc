//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisTrigger/interface/VisL1GlobalTriggerReadoutRecordTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/VisL1FilterService.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMask.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMaskAlgoTrigRcd.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMaskTechTrigRcd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoResetTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/SbLinear.h>
#include <qstring.h>
#include <sstream>
#include <iomanip>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
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
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Trigger/L1GlobalTriggerReadoutRecord");
	
	if (! eventTwig) 
	    eventTwig = parent;
		
	return new VisL1GlobalTriggerReadoutRecordTwig (state, eventTwig, "[N/A] L1GlobalTriggerReadoutRecord (" + name + ")", 
							friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisL1GlobalTriggerReadoutRecordTwig::VisL1GlobalTriggerReadoutRecordTwig (IgState *state, IgTwig *parent,
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
    edm::TypeID id (typeid (L1GlobalTriggerReadoutRecord));
    tfService->registerTwig (id.friendlyClassName (), &createThisTwig);

    VisL1FilterService *trgService = VisL1FilterService::get (state);
    if (! trgService)
    {
	trgService = new VisL1FilterService (state);	
    }   
}

void
VisL1GlobalTriggerReadoutRecordTwig::onNewEvent (const edm::Event &event,
						 const edm::EventSetup &eventSetup)
{
    LOG (1, trace, LFfwvis, "VisL1GlobalTriggerReadoutRecordTwig[" << name ()
	 << "]::onNewEvent()\n");
    
    VisQueuedTwig::onBaseInvalidate ();

    m_text = QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
	     .arg (event.id ().run ())
	     .arg (event.id ().event ())
	     .arg (event.luminosityBlock ())
	     .arg (event.orbitNumber ())
	     .arg (event.bunchCrossing ())
	     .latin1 ();
    
    m_decision.clear ();
    m_algos.clear ();
    m_triggerMaskAlgoTrig.clear ();
    m_triggerMaskTechTrig.clear ();
    
    edm::Handle< L1GlobalTriggerReadoutRecord > gtRecord;
    edm::ESHandle< L1GtTriggerMask > l1GtTmTech;
    edm::ESHandle< L1GtTriggerMask > l1GtTmAlgo;

    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.get (visSel, gtRecord);
	
	    eventSetup.get< L1GtTriggerMaskTechTrigRcd >().get (l1GtTmTech); 
	    const L1GtTriggerMask *trigMask = l1GtTmTech.product ();
	    m_triggerMaskTechTrig = trigMask->gtTriggerMask ();

	    eventSetup.get< L1GtTriggerMaskAlgoTrigRcd >().get (l1GtTmAlgo);        
	    const L1GtTriggerMask *trigMaskAlgo = l1GtTmAlgo.product ();
	    m_triggerMaskAlgoTrig = trigMaskAlgo->gtTriggerMask ();

	    // get menu
	    edm::ESHandle<L1GtTriggerMenu> menuRcd;
	    eventSetup.get<L1GtTriggerMenuRcd> ().get (menuRcd) ;
	    const L1GtTriggerMenu *menu = menuRcd.product ();
	    const AlgorithmMap amap = menu->gtAlgorithmMap ();

	    VisL1FilterService *trgService = VisL1FilterService::get (state ());
	    ASSERT (trgService);
	
	    for (AlgorithmMap::const_iterator mit = amap.begin(), mitEnd = amap.end(); mit != mitEnd; ++mit)
	    {
		std::string strKey = mit->first;
		int algBitNumber = (mit->second).algoBitNumber ();
		QString algoName = QString ("%1: %2").arg (algBitNumber).arg (QString::fromStdString (strKey));

		std::pair<std::string, bool> apair;
		apair.first = algoName.latin1 ();
		apair.second = false;
		m_algos.push_back (apair);
		if (! trgService->initialized ())
		{
		    trgService->registerAlgo (algoName.latin1 (), QString ("%1").arg (algBitNumber).latin1 ());
		}	    
	    }
	    trgService->initialized (true);
	
	    if (gtRecord.isValid ())
	    {
		QString sizeStr = (QString ("%1").arg ((gtRecord->decision () ? ("True") : ("False"))));
		QString nameStr = QString::fromStdString (this->name ());
		int ib = nameStr.find ("[");
		int ie = nameStr.find ("]");
		nameStr.replace (ib + 1, ie - 1, sizeStr);
		this->name (nameStr.toStdString ());

		const DecisionWord dWord = gtRecord->decisionWord ();
		m_passed = gtRecord->decision ();
		m_decision = dWord;
		for (std::vector<std::pair<std::string, bool> >::iterator ait = m_algos.begin (), aitEnd = m_algos.end (); ait != aitEnd; ++ait)
		{
		    (*ait).second = menu->gtAlgorithmResult ((QString::fromStdString ((*ait).first).section (": ", 1, 1)).toStdString (), dWord); // ? ((*ait).second = true) : ((*ait).second = false);
		    // std::cout << "Map: " << (QString((*ait).first).section (": ", 1, 1)).latin1 () << ":" << std::boolalpha << (*ait).second << std::endl;
		    std::stringstream ss;
		    ss << std::boolalpha << (*ait).second;
		    trgService->algorithm ((*ait).first, ss.str ());
		}
	    }
	}
    }
    catch (lat::Error& e)
    {
	if (this->m_onError)
	    this->m_onError (&e);
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisL1GlobalTriggerReadoutRecordTwig::onNewEvent ");
	e.append (this->name ());    

	if (this->m_onCmsException)
	    this->m_onCmsException (&e);
    }
    catch (std::exception& e)
    {
	if (this->m_onException)
	    this->m_onException (&e);
    }
    catch (...)
    {
	if (this->m_onUnhandledException)
	    this->m_onUnhandledException ();
    }

    IgRepSet::invalidate (this, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}

void
VisL1GlobalTriggerReadoutRecordTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << m_text << "<br>";

    if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
    {
	text << "<table width='100%' border=1>"
	     << "<TR align = center>"
	     << "<TH>Decision</TH>"
	     << "<TD COLSPAN=8>Decision Word</TD>"
	     << "</TR>";
	text << setiosflags (std::ios::showpoint | std::ios::fixed);
	text.setf (std::ios::right, std::ios::adjustfield);

	text << "<TR align = left>"
	     << "<TD " << std::setw (3);
	m_passed ? (text << "bgcolor=#00ff00>True") : (text << "bgcolor=#ff0000>False"); 
	text << "</TD>"
	     << "<TD>" << std::setw (2) << std::setprecision (3);
    
	unsigned int i = 0;
	for (std::vector<bool>::const_iterator it = m_decision.begin (), itEnd = m_decision.end (); it != itEnd; ++it)
	{
	    (*it) ? (text << "<b>X</b>"):(text << "0");
	    if (((++i % 16) == 0) && i < m_decision.size ())
	    {
		text << "</TD><TD>";
	    } 
	    else if (i >= m_decision.size ())
		text << "</TD>";
	}
    
	text << "</TR>";
	i = 0;    
	text << "<TR align = left>"
	     << "<TD>Algo Mask</TD><TD>";
	for (std::vector<unsigned int>::const_iterator ait = m_triggerMaskAlgoTrig.begin (), aitEnd = m_triggerMaskAlgoTrig.end (); 
	     ait != aitEnd; ++ait)
	{
	    text << (*ait);
	    if (((++i % 16) == 0) && i < m_triggerMaskAlgoTrig.size ())
	    {
		text << "</TD><TD>";
	    } 
	    else if (i >= m_triggerMaskAlgoTrig.size ())
		text << "</TD>";
	}
	text << "</TD></TR>";
	i = 0;    
	text << "<TR align = left>"
	     << "<TD>Tech Mask</TD><TD>";
	for (std::vector<unsigned int>::const_iterator tit = m_triggerMaskTechTrig.begin (), titEnd = m_triggerMaskTechTrig.end (); 
	     tit != titEnd; ++tit)
	{
	    text << (*tit);
	    if (((++i % 16) == 0) && i < m_triggerMaskTechTrig.size ())
	    {
		text << "</TD><TD>";
	    } 
	    else if (i >= m_triggerMaskTechTrig.size ())
		text << "</TD>";
	}
	text << "</TD></TR>";
    
	text << "</table>";
	for (std::vector<std::pair<std::string, bool> >::const_iterator ait = m_algos.begin (), aitEnd = m_algos.end (); ait != aitEnd; ++ait)
	{
	    if ((*ait).second)
		text << (*ait).first << "<br>";
	}
    }
    
    // Send it over.
    rep->setText (text.str ());
}

void
VisL1GlobalTriggerReadoutRecordTwig::update (Ig3DRep *rep)
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

    SoFont* labelFont = new SoFont;
    labelFont->size.setValue (14.0);
    labelFont->name.setValue ("Arial");
    sep->addChild (labelFont);

    float x = -11.0;
    float y = 7.7;
    float z = 0.0;

    SbVec3f pos = SbVec3f (x, y, z);

    for (std::vector<std::pair<std::string, bool> >::const_iterator ait = m_algos.begin (), aitEnd = m_algos.end (); ait != aitEnd; ++ait)
    {
	if ((*ait).second)
	{	    
	    SoText2  *eventLabel = new SoText2;
	    eventLabel->string = (*ait).first.c_str ();

	    SoTranslation *labelTranslation = new SoTranslation;
	    
	    labelTranslation->translation = pos;
	    sep->addChild (labelTranslation);
	    sep->addChild (eventLabel);
	    y = 0.3;
	    pos = SbVec3f (0.0, -y, 0.0);
	}
    }
    SoResetTransform *reset = new SoResetTransform;
    sep->addChild (reset);
    
    rep->node ()->addChild (sep);
}

void
VisL1GlobalTriggerReadoutRecordTwig::update (IgRPhiRep *rep)
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

    SoFont* labelFont = new SoFont;
    labelFont->size.setValue (14.0);
    labelFont->name.setValue ("Arial");
    sep->addChild (labelFont);

    float x = -8.0;
    float y = 7.7;
    float z = 0.0;

    SbVec3f pos = SbVec3f (x, y, z);

    for (std::vector<std::pair<std::string, bool> >::const_iterator ait = m_algos.begin (), aitEnd = m_algos.end (); ait != aitEnd; ++ait)
    {
	if ((*ait).second)
	{	    
	    SoText2  *eventLabel = new SoText2;
	    eventLabel->string = (*ait).first.c_str ();

	    SoTranslation *labelTranslation = new SoTranslation;
	    
	    labelTranslation->translation = pos;
	    sep->addChild (labelTranslation);
	    sep->addChild (eventLabel);
	    y = 0.3;
	    pos = SbVec3f (0.0, -y, 0.0);
	}
    }
    SoResetTransform *reset = new SoResetTransform;
    sep->addChild (reset);
    
    rep->node ()->addChild (sep);
}

void
VisL1GlobalTriggerReadoutRecordTwig::update (IgRZRep *rep)
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

    SoFont* labelFont = new SoFont;
    labelFont->size.setValue (14.0);
    labelFont->name.setValue ("Arial");
    sep->addChild (labelFont);

    float x = -11.0;
    float y = 7.7;
    float z = -12.0;

    SbVec3f pos = SbVec3f (x, y, z);

    for (std::vector<std::pair<std::string, bool> >::const_iterator ait = m_algos.begin (), aitEnd = m_algos.end (); ait != aitEnd; ++ait)
    {
	if ((*ait).second)
	{	    
	    SoText2  *eventLabel = new SoText2;
	    eventLabel->string = (*ait).first.c_str ();

	    SoTranslation *labelTranslation = new SoTranslation;
	    
	    labelTranslation->translation = pos;
	    sep->addChild (labelTranslation);
	    sep->addChild (eventLabel);
	    y = 0.3;
	    pos = SbVec3f (0.0, -y, 0.0);
	}
    }
    SoResetTransform *reset = new SoResetTransform;
    sep->addChild (reset);
    
    rep->node ()->addChild (sep);
}
