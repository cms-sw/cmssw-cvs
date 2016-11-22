#ifndef IGUANA_GL_BROWSERS_IG_EXAMINER_VIEWER_CATEGORY_H
# define IGUANA_GL_BROWSERS_IG_EXAMINER_VIEWER_CATEGORY_H

# include "Iguana/GLBrowsers/interface/IgCommonViewCategory.h"
# include "Iguana/Framework/interface/IgStateElement.h"

class QWidget;
class QCheckBox;
class QComboBox;
class IgQtRangeControlFloat;
class IgQtRangeControlUInt;

class IGUANA_GL_BROWSERS_API IgExaminerViewerCategory
    : public IgCommonViewCategory,
      public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgExaminerViewerCategory);    
    Q_OBJECT
public:
    IgExaminerViewerCategory (IgState *state);
    ~IgExaminerViewerCategory ();

    virtual void		browserChanged 	(Ig3DBaseBrowser *browser);    

private slots:
    virtual void		setStillDrawStyle	(int style);
    virtual void		updateStillDrawStyle	(int style);
    virtual void		setInteractiveDrawStyle	(int style);
    virtual void		updateInteractiveDrawStyle(int style);
    virtual void		setBufferType		(int type);
    virtual void		updateBufferType	(int type);
    virtual void		setSeekTime		(float time);
    virtual void		setStereoOffset		(float offset);
    virtual void		setFeedbackSize		(unsigned int size);
    virtual void		setAutoClipping		(bool option);
    virtual void		setAnimation		(bool option);
    virtual void		setStereoViewing	(bool option);
    virtual void		setDetailSeek		(bool option);
    virtual void		setCursor		(bool option);
    
private:
    void			addExaminerViewerTab 	(void);
    
    QComboBox			*m_stillDrawStyle;
    QComboBox			*m_interactiveDrawStyle;
    QComboBox			*m_bufferType;
    IgQtRangeControlFloat	*m_seekTime;
    IgQtRangeControlFloat	*m_stereoOffset;
    IgQtRangeControlUInt	*m_feedbackSize;
    QCheckBox			*m_autoClipping;
    QCheckBox			*m_animation;
    QCheckBox			*m_stereoViewing;
    QCheckBox			*m_detailSeek;
    QCheckBox			*m_cursor;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_EXAMINER_VIEWER_CATEGORY_H
