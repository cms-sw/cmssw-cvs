#ifndef IGUANA_GL_BROWSERS_IG_3DRANGE_MCONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DRANGE_MCONTROL_H

# include "Iguana/GLBrowsers/interface/Ig3DFieldControl.h"
# include "Iguana/GLBrowsers/interface/IgControlCategory.h"
# include <assert.h>

template <class C1, class C2, class C3>
class IGUANA_GL_BROWSERS_API Ig3DRangeMControl : public Ig3DFieldControl
{
public:
  typedef Ig3DRangeMControl<C1, C2, C3> inherited;

  Ig3DRangeMControl (IgControlCategory *pane, QString label,
		     int parts = IgQtRangeMControl::ValuePart);
  // implicit copy constructor
  // implicit assignment operator
  // implicit destructor

  C1 *		field (void) const
    { return static_cast<C1 *> (Ig3DFieldControl::field ()); }
    
  void		setField (C1 *field)
    {
      Ig3DFieldControl::setField (field);
      if (field){
        m_value->resize (field->getNum ());
        m_value->setDirty (false);
      }
    }
    
  virtual const C2*	fieldValue (void) const
    { assert(field ()); return field ()->getValues (0); }

  C3 *		control (void) const
    { return m_value; }
    
  virtual const C2*	value (void) const
    { return &(m_value->value ()[0]); }

  virtual void	setValue (unsigned int size, const C2* newval)
    { m_value->setValue (size, newval); setDirty (true); }

  virtual void	setValue (const std::vector<C2>& newval)
    { m_value->setValue (newval); setDirty (true); }

  QString	getSettingString (void) const
    { return control ()->getSettingString (); }
    
  void	setSettingString (QString &setting)
    { control ()->setSettingString (setting); }
    
protected:
  virtual void	updateValue (void)
    { setValue (m_value->mySize (), fieldValue ()); }
    
  virtual void	applyValue (void)
    { field ()->setValues (0, m_value->mySize (), value ()); }

private:
  C3	*m_value;
};

template <class C1, class C2, class C3>
Ig3DRangeMControl<C1, C2, C3>::Ig3DRangeMControl (IgControlCategory *pane,
						  QString label,
						  int parts /* = IgQtRangeControl::ValuePart */)
  : Ig3DFieldControl (pane)
{
  assert(pane);
  assert(pane->bodyArea ());
  assert(pane->bodyLayout ());

  QWidget	*area = pane->bodyArea ();
  int		row = gridRows ();

  assert(row >= 0);

  // Add a new row to the grid, label on the left and the value on the right
  QWidget 	*vlab = makeIndentedLabel (label, area);
  addGridWidget (vlab, row, 0);

  m_value = new C3 (area, parts);
  addGridWidget (makeSpacedBox (area, m_value->widget ()), row, 1);

  // Always reflect the state of the real value control and vice versa.
  connect (m_value,            SIGNAL (dirty    ()), 
	   this,               SLOT   (setDirty ()));
  connect (this,               SIGNAL (clean    ()),
	   m_value,            SLOT   (setClean ()));
  connect (pane,               SIGNAL (tracking          (bool)), 
	   m_value,            SLOT   (resetChangedValue (bool)));
  connect (m_value,            SIGNAL (settingsChanged (void)),
	   this,	       SLOT   (fieldSettingsChanged (void)));
}

#endif // IGUANA_GL_BROWSERS_IG_3DRANGE_MCONTROL_H
