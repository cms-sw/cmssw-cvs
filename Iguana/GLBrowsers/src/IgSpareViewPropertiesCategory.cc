#include "Iguana/GLBrowsers/interface/IgSpareViewPropertiesCategory.h"
#include "Iguana/GLBrowsers/interface/IgSpareBrowser.h"
#include "Iguana/Studio/interface/IgQtAppContextService.h"
#include "Iguana/GLBrowsers/interface/IgQtAxisAlignment.h"
#include <QHBoxLayout>

IG_DEFINE_STATE_ELEMENT (IgSpareViewPropertiesCategory, "/Control Centre/Categories/Spare View Properties");

IgSpareViewPropertiesCategory::IgSpareViewPropertiesCategory (IgState *state,
							      IgSpareBrowser *source)
    : IgControlCategory (state, "Spare View"),
      m_source (source),
      m_axisAlignment (0)
{
    int		row   = bodyLayout ()->numRows ();
    ASSERT (row >= 0);
    
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

IgSpareViewPropertiesCategory::~IgSpareViewPropertiesCategory (void)
{
    state ()->detach (s_key);    
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
IgSpareViewPropertiesCategory::alignAxis (void)
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
IgSpareViewPropertiesCategory::registerBrowser (IgState *state, IgSpareBrowser *source)
{
    IgQtAppContextService *cs = IgQtAppContextService::get (state);
    assert (cs);
    cs->addOnFocusIn (lat::CreateCallback (this,
					   &IgSpareViewPropertiesCategory::browserChanged,
					   source));
    IgControlCategory::registerMe (state);
    m_source = source;
}

void 
IgSpareViewPropertiesCategory::browserChanged (IgSpareBrowser *source)
{
    if (source)
      m_source = source;	
}
