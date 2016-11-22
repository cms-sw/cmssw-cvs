#ifndef IGCONTROLCENTRE_H
#define IGCONTROLCENTRE_H

# include "Iguana/GLBrowsers/interface/config.h"
# include <QDialog>
# include <map>
# include <vector>

class IgControlCategory;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class IgState;
class QFocusEvent;

class IGUANA_GL_BROWSERS_API IgControlCentre : public QDialog
{
    Q_OBJECT

public:
    IgControlCentre (QWidget* parent = 0, 
		     const char* name = 0, 
		     bool modal = FALSE, 
		     Qt::WFlags fl = 0);
    ~IgControlCentre (void);

    void		addCategory (IgControlCategory *category);
    void      		removeCategory (IgControlCategory *category);
    void       		selectCategory (IgControlCategory *category);
    void       		selectCategory (const QString& name);

    QListWidget		*iconView (void);
    
    // Overrides from base class.
    virtual void	show (void);
    virtual void	hide (void);
    void		updateOnWindowActive (void);
    
public slots:
    virtual void 	apply (void);
    virtual void 	revert (void);
    virtual void 	help (void);
    virtual void 	back (void);
    virtual void	enableCategory (IgControlCategory *category,
                                        bool state = true);
					
private slots:
    void       		selectIconItem (QListWidgetItem*);
    void    		dirtyCategory (void);
    void		cleanCategory (void);
    void		showCategory (IgControlCategory *category);
    void		hideCategory (void);
    void		addListWidgetItems (QString name);   
    void		showListWidget (QString name);   
    void		hideListWidget (void);

protected:
    void 		windowActivationChange  (bool oldActive);
    
    QGridLayout		*IgControlCentreLayout;
    QHBoxLayout		*m_layout1;
    QGridLayout		*m_layout2;

protected slots:
    virtual void 	languageChange();

private:
    typedef std::map<QString, std::pair<IgControlCategory *, bool> > CategoryMap;
    typedef std::vector<QListWidgetItem*> ListWidgetList;
    typedef std::map<QString, std::pair<void*, bool> > ListWidgetArch;
    void		updateControlCentre (QString name);
    void 		removeCategory (QString name, ListWidgetArch *toplevel);
    bool 		hasCategory (const IgControlCategory *category);
    void		deleteListWidgetArch (ListWidgetArch *toplevel);
    void		enableCategory (QString name,
                                        ListWidgetArch *toplevel,
					bool state = true);
					  
    ListWidgetArch	m_iconViewArch;
    CategoryMap		m_categories;
    ListWidgetList	m_iconViewList;
    IgControlCategory	*m_current;

    QListWidget		*m_iconView;
    QPushButton		*m_close;
    QPushButton		*m_revert;
    QPushButton		*m_apply;
    QPushButton		*m_help;
    QPushButton		*m_back;
    QString		m_name;
    QString		m_baseName;
    bool		m_updateOnWindowActive;
    bool		m_focusChanged;
};

#endif // IGCONTROLCENTRE_H
