#include "Iguana/GLBrowsers/interface/IgGL2PSPrintCategory.h"
#include "Iguana/Framework/interface/IgState.h"
#include "Iguana/GLBrowsers/interface/Ig3DBaseBrowser.h"
#include "Iguana/GL2PS/interface/gl2ps.h"
#include <QCheckBox>
#include <QGridLayout>

bool IgGL2PSPrintCategory::updated = false;

IG_DEFINE_STATE_ELEMENT (IgGL2PSPrintCategory, "/ControlCentre/Categories/View Properties/GL2PS Printing");

IgGL2PSPrintCategory::IgGL2PSPrintCategory (IgState *state)
    : IgCommonViewCategory (state, "GL2PS Printing"),
      m_gl2psSimpleLineOffset (0),
      m_gl2psBestRoot         (0),
      m_gl2psOcclusionCulling (0),
      m_gl2psBackGround       (0),      
      m_gl2psLandScape        (0)
{
    addGL2PSOptionTab ();
    state->put (s_key, this);
}

IgGL2PSPrintCategory::~IgGL2PSPrintCategory (void)
{ state()->detach (s_key); }

// Print Options
void
IgGL2PSPrintCategory::addGL2PSOptionTab (void)
{
    QWidget	*printOptions = new QWidget (topArea ());
    topLayout ()->addWidget (printOptions);
    
    QVBoxLayout *printOptionsVLayout = new QVBoxLayout (printOptions, 5, 6, "printOptionsLayout");
    QGridLayout	*printOptionsLayout  = new QGridLayout (printOptionsVLayout);
    

    m_gl2psSimpleLineOffset = new QCheckBox (printOptions, "m_gl2psSimpleLineOffset");
    m_gl2psSimpleLineOffset->setText (tr ("Simple Line Offset"));
    printOptionsLayout->addWidget (m_gl2psSimpleLineOffset, 0, 0);
    
    m_gl2psBestRoot = new QCheckBox (printOptions, "m_gl2psBestRoot");
    m_gl2psBestRoot->setText (tr ("Best Root"));
    printOptionsLayout->addWidget (m_gl2psBestRoot, 0, 1);
    
    m_gl2psOcclusionCulling = new QCheckBox (printOptions, "m_gl2psOcclusionCulling");
    m_gl2psOcclusionCulling->setText (tr ("Occlusion Culling"));
    printOptionsLayout->addWidget (m_gl2psOcclusionCulling, 1, 0);
    
    m_gl2psBackGround = new QCheckBox (printOptions, "m_gl2psBackGround");
    m_gl2psBackGround->setText (tr ("Back ground"));
    printOptionsLayout->addWidget (m_gl2psBackGround, 1, 1);
    
    m_gl2psLandScape = new QCheckBox (printOptions, "m_gl2psLandScape");
    m_gl2psLandScape->setText (tr ("Land scape orientation"));
    printOptionsLayout->addWidget (m_gl2psLandScape, 2, 0);
    
    printOptionsVLayout->addItem (new QSpacerItem ( 0, 0, QSizePolicy::Expanding,
						    QSizePolicy::Expanding));

    connect (m_gl2psSimpleLineOffset, SIGNAL (clicked (void)),
	     this,		      SLOT   (setGL2PSOptions(void)));
    connect (m_gl2psBestRoot,         SIGNAL (clicked (void)),
	     this,		      SLOT   (setGL2PSOptions(void)));
    connect (m_gl2psOcclusionCulling, SIGNAL (clicked (void)),
	     this,		      SLOT   (setGL2PSOptions(void)));
    connect (m_gl2psBackGround,       SIGNAL (clicked (void)),
	     this,		      SLOT   (setGL2PSOptions(void)));
    connect (m_gl2psLandScape,        SIGNAL (clicked (void)),
	     this,		      SLOT   (setGL2PSOptions(void)));
}

void 
IgGL2PSPrintCategory::browserChanged (Ig3DBaseBrowser *source)
{
    if (source)
    {
	IgCommonViewCategory::browserChanged (source);
	if (!updated)
	    updateGL2PSOptions ();
        else
	    setGL2PSOptions ();
    }
}

//////////////////////////////////////////////////////////////////////////
void
IgGL2PSPrintCategory::updateGL2PSOptions (void)
{
    int gl2psOptions = m_source->getGL2PSOptions ();
    m_gl2psBackGround->setChecked (gl2psOptions & GL2PS_DRAW_BACKGROUND);
    m_gl2psSimpleLineOffset->setChecked (gl2psOptions & GL2PS_SIMPLE_LINE_OFFSET);
    m_gl2psBestRoot->setChecked (gl2psOptions & GL2PS_BEST_ROOT);
    m_gl2psOcclusionCulling->setChecked (gl2psOptions & GL2PS_OCCLUSION_CULL);
    m_gl2psLandScape->setChecked (gl2psOptions & GL2PS_LANDSCAPE);
    updated = true;
}

void
IgGL2PSPrintCategory::setGL2PSOptions (void)
{
    int gl2psOptions = 0;
    
    if ( m_gl2psBackGround->isChecked ())
	gl2psOptions |= GL2PS_DRAW_BACKGROUND;
    if ( m_gl2psSimpleLineOffset->isChecked ())
	gl2psOptions |= GL2PS_SIMPLE_LINE_OFFSET;
    if ( m_gl2psBestRoot->isChecked ())
	gl2psOptions |= GL2PS_BEST_ROOT;
    if ( m_gl2psOcclusionCulling->isChecked ())
	gl2psOptions |= GL2PS_OCCLUSION_CULL;
    if ( m_gl2psLandScape->isChecked ())
	gl2psOptions |= GL2PS_LANDSCAPE;
      
    m_source->setGL2PSOptions (gl2psOptions);
}
