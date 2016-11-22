#include "Iguana/GLBrowsers/interface/IgQtApplicationCategory.h"
#include "Iguana/Framework/interface/IgState.h"
#include <QApplication>
#include <QComboBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QStyleFactory>

IG_DEFINE_STATE_ELEMENT (IgQtApplicationCategory, "/ControlCentre/Categories/View Properties/Qt Application");

IgQtApplicationCategory::IgQtApplicationCategory (IgState *state)
    : IgCommonViewCategory (state, "Qt Application"),
      m_windowStyle	(0)
{
    addQApplicationTab ();
    updateWindowStyle (qApp->styleSheet ());
    state->put (s_key, this);
}

IgQtApplicationCategory::~IgQtApplicationCategory (void)
{ state()->detach (s_key); }

void
IgQtApplicationCategory::addQApplicationTab (void)
{
    QWidget	*qApplicationOptions = new QWidget (topArea ());
    topLayout ()->addWidget (qApplicationOptions);
    
    QVBoxLayout	*qApplicationVLayout  = new QVBoxLayout (qApplicationOptions);
    QGridLayout	*qApplicationLayout  = new QGridLayout (qApplicationVLayout);
    qApplicationLayout->setMargin (20);

    //Window Style
    QWidget 	*vlab1 = makeIndentedLabel ("Window Style",
                                             qApplicationOptions);
    qApplicationLayout->addWidget (vlab1, 0, 0);
    m_windowStyle = new QComboBox (qApplicationOptions);
    QStringList styleList = QStyleFactory::keys ();
    
    //adding available styles to the combobox
    for (QStringList::const_iterator it = styleList.begin ();
         it != styleList.end ();
	 it++ )
        m_windowStyle->insertItem (*it);
    qApplicationLayout->
	addWidget (makeBox (qApplicationOptions, m_windowStyle), 0, 1);

    qApplicationVLayout->addItem (new QSpacerItem ( 0, 0, QSizePolicy::Expanding,
						    QSizePolicy::Expanding));
    connect (m_windowStyle,	SIGNAL (activated (const QString &)),
             this,		SLOT   (setWindowStyle (const QString &)));
}

//////////////////////////////////////////////////////////////////////////
void
IgQtApplicationCategory::setWindowStyle (const QString & style)
{
    if (style != qApp->styleSheet ())
	qApp->setStyleSheet (style);
}

void
IgQtApplicationCategory::updateWindowStyle (const QString & style)
{
    for (int i = 0; i < m_windowStyle->count (); i++)
    {
	if (m_windowStyle->text (i) == style)
	{
	    m_windowStyle->setCurrentItem (i);
	    break;
	}
    }
}
