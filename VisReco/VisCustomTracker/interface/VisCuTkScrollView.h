/****************************************************************************
**	VisCuTkScrollView.h
**
****************************************************************************/
#ifndef VIS_CU_TK_SCROLL_VIEW_H
#define VIS_CU_TK_SCROLL_VIEW_H

#include <QScrollArea>
#include <QMouseEvent>

class VisCuTkMapWindow;
class VisCuTkScrollView : public QScrollArea
{
    Q_OBJECT
      
public:
    VisCuTkScrollView( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0);
    ~VisCuTkScrollView();
protected:
    virtual void mousePressEvent( QMouseEvent* );
    virtual void mouseReleaseEvent( QMouseEvent* );
private:
    QPoint m_drag_old_pos;
    VisCuTkMapWindow * gp;
};
#endif // CU_TK_SCROLL_VIEW_H
