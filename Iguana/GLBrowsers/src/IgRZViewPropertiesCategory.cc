#include "Iguana/GLBrowsers/interface/IgRZViewPropertiesCategory.h"
#include "Iguana/GLBrowsers/interface/IgRZBrowser.h"
#include "Iguana/Studio/interface/IgQtAppContextService.h"
#include "Iguana/GLBrowsers/interface/IgQtAxisAlignment.h"
#include <QHBoxLayout>

IG_DEFINE_STATE_ELEMENT (IgRZViewPropertiesCategory, "/Control Centre/Categories/RZ View Properties");

IgRZViewPropertiesCategory::IgRZViewPropertiesCategory (IgState *state,
							IgRZBrowser *source)
    : IgControlCategory (state, "RZ View"),
      m_source (source),
      m_axisAlignment (0)
{
    int		row   = bodyLayout ()->numRows ();
    assert (row >= 0);
    
    //Add ComboBox to change the type of selected light
    QHBoxLayout *topRow = new QHBoxLayout (topLayout ());
    topRow->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));


    // Alignment widget
    QWidget *area = bodyArea ();    
    QWidget     *vlab = makeIndentedLabel ("Align Camera",
                                            area);
    bodyLayout ()->addWidget (vlab, row, 0);

    m_axisAlignment = new IgQtAxisAlignment (area);

    m_axisAlignment->setMaxAxisAlign (1.);
    m_axisAlignment->setMinAxisAlign (0.);
    bodyLayout ()->addWidget (makeSpacedBox (area, m_axisAlignment->widget ()),
                              row++, 1);

    connect (m_axisAlignment,	SIGNAL (dirty ()),
             this,		SLOT (alignAxis ()));    
    state->put (s_key, this);
}

IgRZViewPropertiesCategory::~IgRZViewPropertiesCategory (void)
{
    state ()->detach (s_key);    
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
IgRZViewPropertiesCategory::alignAxis (void)
{
    assert (m_source);
    if (m_axisAlignment->xAlign ())
      m_source->viewPlaneX ();
    else if (m_axisAlignment->yAlign ())
      m_source->viewPlaneY ();
    else if (m_axisAlignment->zAlign ())
      m_source->viewPlaneZ ();
    m_axisAlignment->setClean ();    
}
//////////////////////////////////////////////////////////////////////
void
IgRZViewPropertiesCategory::registerBrowser (IgState *state, IgRZBrowser *source)
{
    IgQtAppContextService *cs = IgQtAppContextService::get (state);
    assert (cs);
    cs->addOnFocusIn (lat::CreateCallback (this,
					   &IgRZViewPropertiesCategory::browserChanged,
					   source));
    IgControlCategory::registerMe (state);
    m_source = source;
}

void 
IgRZViewPropertiesCategory::browserChanged (IgRZBrowser *source)
{
    if (source)
      m_source = source;	
}
