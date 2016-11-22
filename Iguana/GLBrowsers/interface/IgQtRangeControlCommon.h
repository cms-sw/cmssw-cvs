#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_CONTROL_COMMON_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_CONTROL_COMMON_H

# include "Iguana/GLBrowsers/interface/IgControlItem.h"

class QFrame;
class QWidget;
class IgQtPixButton;
class IgQtLineEdit;
class QSlider;
class QVBoxLayout;

class IGUANA_GL_BROWSERS_API IgQtRangeControlCommon : public IgControlItem
{
    Q_OBJECT
public:
    enum StepType {
	ADD,
	MULTIPLE
    };

    enum Parts {
	/// Makes the control behave in rotation-like manner measuring degrees
	StepperPart		= 1,
	ValuePart		= 2,
	SliderPart		= 4,
	IndexPart		= 8,
	AllParts		= (StepperPart
				   | ValuePart
				   | SliderPart
				   | IndexPart)
    };

    IgQtRangeControlCommon (QWidget *parent, int parts,
		            const char *name = 0);

    StepType		getStepType (void) const;

    QWidget *		widget (void) const;
    void		addFwdButton (QWidget *w);
    void		addRevButton (QWidget *w);
    void		addFwdWidget (QWidget *w);
    void		addRevWidget (QWidget *w);
    void		insertWidget (QWidget *w, int index = -1);

    bool		isValueEditable (void) const;
    bool		isRangeEditable (void) const;
    bool		isStepEditable  (void) const;
    bool		isPrecisionEditable (void) const;

    void		valueEditable (bool editable = true);
    void		rangeEditable (bool editable = true);
    void		stepEditable  (bool editable = true);
    void		precisionEditable  (bool editable = true);
    bool		isEmitSettingsChanged (void) const;
    
    static QString	getSettingItem (QString &setting);
    static void		addSettingItem (const QString &item, QString &setting);
    
    using IgControlItem::setDirty;
public slots:
    void		setStepType (StepType type);

    virtual void	revMin (void);
    virtual void	revLong (void);
    virtual void	revSmall (void);
    virtual void	fwdSmall (void);
    virtual void	fwdLong (void);
    virtual void	fwdMax (void);

    virtual void	setDirty (bool dirty);

    virtual void	resetChangedValue (bool);

    virtual void	show (void);
    virtual void	hide (void);
    virtual void	editSettingsChanged (void);
    virtual void	setEmitSettingsChanged (bool value=true);

signals:
   void			settingsChanged (void);
   
protected slots:

    virtual void	sliderChanged (int newval);
    virtual void	textChanged (const QString &newval);

protected:
    enum MoveDirection { MoveFWD, MoveREV };

    virtual void 	emitSignals (void);
    virtual void	fwdEnable (bool enable);
    virtual void	revEnable (bool enable);

    virtual void	initialize (int parts);

    StepType		m_stepType;
    bool		m_fwdEnabled;
    bool		m_revEnabled;

    QFrame		*m_widget;
    QVBoxLayout		*m_layout;
    QWidget		*m_row;
    IgQtPixButton	*m_revMin;
    IgQtPixButton	*m_revLong;
    IgQtPixButton	*m_revSmall;
    IgQtLineEdit	*m_text;
    IgQtPixButton	*m_fwdSmall;
    IgQtPixButton	*m_fwdLong;
    IgQtPixButton	*m_fwdMax;
    QSlider		*m_slider;
    bool		m_resetChangedValue;
    bool		m_valueEditable;
    bool		m_stepEditable;
    bool		m_rangeEditable;
    bool		m_precisionEditable;
    bool		m_emitSettingsChanged;
};

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_CONTROL_COMMON_H
