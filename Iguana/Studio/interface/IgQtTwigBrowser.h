#ifndef IGUANA_STUDIO_IG_QT_TWIG_BROWSER_H
# define IGUANA_STUDIO_IG_QT_TWIG_BROWSER_H

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgBrowser.h"
# include "Iguana/Framework/interface/IgModel.h"
# include <QTreeWidget>
# include <QList>
# include <map>

class IgState;
class IgSite;
class IgSelectionMessage;
class IgQtTwigRep;

class IGUANA_STUDIO_API IgQtTwigBrowser : public QTreeWidget,
					  public IgBrowser,
					  public IgModel
{
  Q_OBJECT
public:
  IgQtTwigBrowser(IgState *state, IgSite *site);
  ~IgQtTwigBrowser(void);

  virtual void	browse(IgRepresentable *object);
  virtual void	select(IgQtTwigRep *rep);

  static const char *catalogLabel(void);

public slots:
  virtual void	onSelectionChange(void);
  void 		slotContextMenu(const QPoint&);
  void		slotExpand(QTreeWidgetItem *item, int col);

protected:
  // Browser messages
  virtual void	selectMessage(IgSelectionMessage message);
  virtual void	mousePressEvent(QMouseEvent *e);
  virtual void	mouseReleaseEvent(QMouseEvent *e);

  int		isInsideChecker(int x);

private:
  void		contextMenu(QTreeWidgetItem *item, const QPoint& pos);

  IgState      *m_state;
  bool		m_discardRelease;

  // undefined semantics
  IgQtTwigBrowser(const IgQtTwigBrowser &);
  IgQtTwigBrowser &operator=(const IgQtTwigBrowser &);
};

#endif // IGUANA_STUDIO_IG_QT_TWIG_BROWSER_H
