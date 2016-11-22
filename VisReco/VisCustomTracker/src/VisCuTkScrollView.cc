#define QT_NO_EMIT
#include "VisReco/VisCustomTracker/interface/VisCuTkScrollView.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkMapWindow.h"
#include <iostream>


VisCuTkScrollView::VisCuTkScrollView( QWidget* parent,  const char* name, Qt::WFlags fl)
    : QScrollArea(parent)
{
gp = dynamic_cast<VisCuTkMapWindow *>(parent);
}
VisCuTkScrollView::~VisCuTkScrollView()
{
}
void VisCuTkScrollView::mousePressEvent(QMouseEvent *e)
{
 m_drag_old_pos = e->pos();
}
void VisCuTkScrollView::mouseReleaseEvent(QMouseEvent *e)
{
 QPoint d = m_drag_old_pos - e->pos();
 gp->pan(e->pos(), d.x(),d.y());

}
