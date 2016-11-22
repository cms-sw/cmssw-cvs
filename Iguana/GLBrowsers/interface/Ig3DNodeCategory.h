#ifndef IGUANA_GL_BROWSERS_IG_SO_NODE_CATEGORY_H
# define IGUANA_GL_BROWSERS_IG_SO_NODE_CATEGORY_H

# include "Iguana/GLBrowsers/interface/config.h"
# include "Iguana/GLBrowsers/interface/IgControlCategory.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <QWidget>
# include <map>

class SoNode;
class SoGroup;
class SoField;
class IgState;
class Ig3DFieldControl;
class IgQtRangeControlUInt;
class QTreeWidget;
class QTreeWidgetItem;
class Ig3DBaseBrowser;
class Ig3DBaseRep;
class IgQtRangeValuePrecision;

class IGUANA_GL_BROWSERS_API Ig3DNodeCategory
  : public IgControlCategory,
    public IgStateElement
{
    Q_OBJECT
    
    IG_DECLARE_STATE_ELEMENT (Ig3DNodeCategory);
    
    typedef std::map<QTreeWidgetItem *, SoNode*> nodeMap;
    
    typedef std::map<SoField*, Ig3DFieldControl*> fieldControlMap;
    
    typedef std::map<QString, QString> fieldLimitsMap;
    
    enum FieldOptions {
      CREATE,
      DELETE,
      SET,
      SAVE_LIMITS
    };
    
public:
  Ig3DNodeCategory  (IgState *state, Ig3DBaseBrowser *source);
  ~Ig3DNodeCategory (void);

  virtual void	attach (void);
  virtual void	detach (void);
  virtual void	update (void);
  virtual void	apply (void);
  void		browserFocusIn  (Ig3DBaseBrowser *browser);

    virtual void	manipulate (SoNode  *node);
    virtual void	clearCategory (void);
    virtual void	clearBodyArea (void);
    virtual void	registerBrowser (IgState *state,
                                         Ig3DBaseBrowser *browser);    

public slots:
  virtual void	show (void);
  virtual void	hide (void);
  virtual void 	focusIn (void);
  virtual void	saveSettings (SoField* field);
  virtual void	saveNode (void);

protected slots:
    virtual void	manipulateItem (QTreeWidgetItem *item);
    
protected:
  virtual void	focusInEvent(QFocusEvent *event);
  virtual void	addFields (SoNode  *node, const char *name = 0);
  virtual bool	addNode (SoNode *node, QTreeWidgetItem *parent = 0);
  virtual void	itrateOverFields (SoNode* node, FieldOptions type);
  virtual void	itrateOverFields (FieldOptions type);
    
    virtual void	 fieldMFloat  (SoField*, FieldOptions);
    virtual void	 fieldMInt32  (SoField*, FieldOptions);
    virtual void	 fieldMUInt32 (SoField*, FieldOptions);
    virtual void	 fieldMShort  (SoField*, FieldOptions);
    virtual void	 fieldMUShort (SoField*, FieldOptions);
    virtual void	 fieldSFloat  (SoField*, FieldOptions);
    virtual void	 fieldSInt32  (SoField*, FieldOptions);
    virtual void	 fieldSUInt32 (SoField*, FieldOptions);
    virtual void	 fieldSShort  (SoField*, FieldOptions);
    virtual void	 fieldSUShort (SoField*, FieldOptions);
    virtual void	 fieldMColor  (SoField*, FieldOptions);
    virtual void	 fieldSColor  (SoField*, FieldOptions);
    virtual void	 fieldMString (SoField*, FieldOptions);
    virtual void	 fieldSString (SoField*, FieldOptions);
    virtual void	 fieldSBool   (SoField*, FieldOptions);
    virtual void	 fieldMBool   (SoField*, FieldOptions);
    virtual void	 fieldSVec2f  (SoField*, FieldOptions);
    virtual void	 fieldSVec3f  (SoField*, FieldOptions);
    virtual void	 fieldSVec4f  (SoField*, FieldOptions);
    virtual void	 fieldSRotation (SoField*, FieldOptions);
    virtual void	 fieldSPlane  (SoField*, FieldOptions);

private:    
    bool		hasControllableFields (SoNode *);
    
    //Fields to control and their indexs in the FieldContainer
    fieldControlMap 	m_fields;
    Ig3DBaseBrowser	*m_source;
    SoNode		*m_currentNode;
    QTreeWidget		*m_nodeTree;
    nodeMap		m_nodeMap;
    fieldLimitsMap	m_fieldLimitsMap;
    
    // undefined semantics
    Ig3DNodeCategory (const Ig3DNodeCategory &);
    Ig3DNodeCategory &operator= (const Ig3DNodeCategory &);
};

#endif // IGUANA_GL_BROWSERS_IG_SO_NODE_CATEGORY_H
