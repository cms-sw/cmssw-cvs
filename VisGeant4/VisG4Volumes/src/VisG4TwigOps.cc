#include "VisGeant4/VisG4Volumes/interface/VisG4TwigOps.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4VolumeTwig.h"
#include "Iguana/Studio/interface/IgTwigOps.h"
#include "Iguana/Studio/interface/IgQtObjectMenu.h"
#include "Iguana/Studio/interface/IgQtObjectMenuMessage.h"
#include "VisGeant4/VisG4Core/interface/VisG4VisTransform3D.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4VolumePicker.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4Details.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4Filter.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4FilterTable.h"
#include "Iguana/Controls/interface/IgMatEdMainWidget.h"
//-ap #include "Iguana/Controls/src/IgMatEdMainMenu.h"

#include "Iguana/Framework/interface/IgRepSet.h"

#include <QApplication>
#include <QPushButton>
#include <QDialog>
#include <QLayout>
#include <QLabel>
#include <QColorDialog>
#include <QInputDialog>
#include <QComboBox>
#include <QMessageBox>
#include <QString>
#include <QProgressDialog>
#include <QListWidget>

#include <G4VPhysicalVolume.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4Material.hh>
#include <G4VSolid.hh>
#include <G4VPVParameterisation.hh>
#include <G4VoxelLimits.hh>
#include <G4AffineTransform.hh>
#include <Randomize.hh>

void
VisG4TwigOps::menuAction (IgQtObjectMenuMessage message)
{
  static const IgQtObjectMenu::ItemDef defs [] = {
    { -1, MENU_LOGICAL, "Logical Children",
      SLOT (actionLogical (int)), MENU_LOGICAL },
    { -1, MENU_GET_MASS, "Get Mass",
      SLOT (actionGetMass (int)), MENU_GET_MASS },

    { -1, MENU_SHOW_CATEGORY, "Show By Category", 0, -1 },
    { MENU_SHOW_CATEGORY, CAT_SENSITIVE,	"Sensitive",
      SLOT (actionShowCategory (int)), CAT_SENSITIVE },
    { MENU_SHOW_CATEGORY, CAT_INSENSITIVE,	"Any Insensitive",
      SLOT (actionShowCategory (int)), CAT_INSENSITIVE },
    { MENU_SHOW_CATEGORY, CAT_FILTER,	"User Defined Filter",
      SLOT (actionApplyFilter (int)), CAT_FILTER },
#if 0
    { MENU_SHOW_CATEGORY, CAT_BEAMPIPE,        "Beampipe",
      SLOT(actionShowCategory(int)), CAT_BEAMPIPE },
    { MENU_SHOW_CATEGORY, CAT_CABLE,           "Cables",
      SLOT(actionShowCategory(int)), CAT_CABLE },
    { MENU_SHOW_CATEGORY, CAT_COOLING,         "Cooling",
      SLOT(actionShowCategory(int)), CAT_COOLING },
    { MENU_SHOW_CATEGORY, CAT_MAGNET,          "Magnet",
      SLOT(actionShowCategory(int)), CAT_MAGNET },
    { MENU_SHOW_CATEGORY, CAT_SUPPORT,         "Support",
      SLOT(actionShowCategory(int)), CAT_SUPPORT },
    { MENU_SHOW_CATEGORY, CAT_SENSITIVE_CONT,  "Sensitive Container",
      SLOT(actionShowCategory(int)), CAT_SENSITIVE_CONT },
#endif

    { -1, MENU_SHOW_MATERIAL, "Show By Material...",
      SLOT(actionShowMaterial(int)), MENU_SHOW_MATERIAL },

    { -1, MENU_HIDE_CATEGORY, "Hide By Category", 0, -1 },
    { MENU_HIDE_CATEGORY, CAT_SENSITIVE,       "Sensitive",
      SLOT(actionHideCategory(int)), CAT_SENSITIVE },
    { MENU_HIDE_CATEGORY, CAT_INSENSITIVE,     "Any Insensitive",
      SLOT(actionHideCategory(int)), CAT_INSENSITIVE },
#if 0
    { MENU_HIDE_CATEGORY, CAT_BEAMPIPE,        "Beampipe",
      SLOT(actionHideCategory(int)), CAT_BEAMPIPE },
    { MENU_HIDE_CATEGORY, CAT_CABLE,           "Cables",
      SLOT(actionHideCategory(int)), CAT_CABLE },
    { MENU_HIDE_CATEGORY, CAT_COOLING,         "Cooling",
      SLOT(actionHideCategory(int)), CAT_COOLING },
    { MENU_HIDE_CATEGORY, CAT_MAGNET,          "Magnet",
      SLOT(actionHideCategory(int)), CAT_MAGNET },
    { MENU_HIDE_CATEGORY, CAT_SUPPORT,         "Support",
      SLOT(actionHideCategory(int)), CAT_SUPPORT },
    { MENU_HIDE_CATEGORY, CAT_SENSITIVE_CONT,  "Sensitive Container",
      SLOT(actionHideCategory(int)), CAT_SENSITIVE_CONT },
#endif

    { -1, MENU_HIDE_MATERIAL, "Hide By Material...",
      SLOT(actionShowMaterial(int)), MENU_HIDE_MATERIAL },

    { -1, MENU_RENDER_SEP, 0, 0, -1 },
    { -1, MENU_APPEARANCE, "Appearance...",
      SLOT(actionAppearance(int)), MENU_APPEARANCE },		
    { -1, MENU_RENDER,  "Render", 0, -1 },
    { MENU_RENDER, MENU_RENDER_AS_IS, "As Is",
      SLOT(actionRender(int)), MENU_RENDER_AS_IS },
    { MENU_RENDER, MENU_RENDER_SOLID, "Solid",
      SLOT(actionRender(int)), MENU_RENDER_SOLID },
    { MENU_RENDER, MENU_RENDER_WIREFRAME, "Wireframe",
      SLOT(actionRender(int)), MENU_RENDER_WIREFRAME },
    { -1, MENU_RENDER_ALL,  "Render All", 0, -1 },
    { MENU_RENDER_ALL, MENU_RENDER_SOLID, "Solid",
      SLOT(actionRenderAll(int)), MENU_RENDER_SOLID },
    { MENU_RENDER_ALL, MENU_RENDER_WIREFRAME, "Wireframe",
      SLOT(actionRenderAll(int)), MENU_RENDER_WIREFRAME },
#if 0
    { -1, MENU_COLOUR, "Set Colour...",
      SLOT(actionColour(int)), MENU_COLOUR },
    { -1, MENU_TRANSPARENCY, "Set Transparency...",
      SLOT(actionTransparency(int)), MENU_TRANSPARENCY },
#endif
  };
  static const int ndefs = sizeof (defs)/sizeof (defs[0]);

  m_menu = message.menu ();
  if (! (m_twig = dynamic_cast<VisG4VolumeTwig *> (message.object ())))
    m_menu->removeFromDefs (defs, ndefs);
  else
  {
    m_menu->insertFromDefs (this, defs, ndefs);
    menuUpdate ();
  }
}

void
VisG4TwigOps::menuUpdate (void)
{
  assert(m_menu);
  // Update menu items to reflec the state of this twig.
  m_menu->changeItem (MENU_LOGICAL, m_twig->logical ()
		      ? "Physical Children" : "Logical Children");
  m_menu->setItemEnabled (MENU_GET_MASS, m_twig->logical ());

  const G4VisAttributes *attrs = m_twig->volume ()
				 ->GetLogicalVolume ()
				 ->GetVisAttributes ();
  assert(attrs);

  bool		isset = attrs->IsForceDrawingStyle ();
  bool		solid = (attrs->GetForcedDrawingStyle ()
			 == G4VisAttributes::solid);
  IgQtObjectMenu	*submenu = m_menu->subMenu (MENU_RENDER);
  submenu->setItemChecked (MENU_RENDER_AS_IS, !isset);
  submenu->setItemChecked (MENU_RENDER_SOLID, isset && solid);
  submenu->setItemChecked (MENU_RENDER_WIREFRAME, isset && !solid);
}

//////////////////////////////////////////////////////////////////////
void
VisG4TwigOps::actionLogical (int /* id */)
{
  assert(m_twig);
  QApplication::setOverrideCursor (Qt::waitCursor);
  m_twig->logical (! m_twig->logical ());
  IgRepSet::invalidate (m_twig, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
  QApplication::restoreOverrideCursor ();
}

//////////////////////////////////////////////////////////////////////
void
VisG4TwigOps::actionGetMass (int /* id */)
{
  assert(m_twig);

  QApplication::setOverrideCursor (Qt::waitCursor);
    
  double mass = VisG4TwigOps::getMass (m_twig->volume ()->GetLogicalVolume (), m_twig->volume ()->GetLogicalVolume ()->GetMaterial ()) / kg;

  QApplication::restoreOverrideCursor ();    
   
  QString text = QString ("Mass = %1 Kg")
		 .arg (mass, 0, 'g', 7);

  QMessageBox::information ((QWidget *) 0, "Logical Volume Mass", text);
}

//////////////////////////////////////////////////////////////////////
void
VisG4TwigOps::actionShowCategory (int id)
{
  QApplication::setOverrideCursor (Qt::waitCursor);
  // Enable upwards if in show (FIXME: Reuse IgTwigOps)
  IgTwig *update = m_twig;
  for (IgTwig *t = m_twig->parent (); t; update = t, t = t->parent ())
    t->selfTraverse (true);

  doShowCategory (true, id, m_twig);
  IgRepSet::invalidate (update, IgTwig::FLAGS_MASK | IgTwig::STRUCTURE_MASK);
  QApplication::restoreOverrideCursor ();
}

void
VisG4TwigOps::actionHideCategory (int id)
{
  QApplication::setOverrideCursor (Qt::waitCursor);
  doShowCategory (false, id, m_twig);
  QApplication::restoreOverrideCursor ();
}

//
// FIXME: Temporary function to calculate mass - 
// to be replaced with native G4 GetMass function 
// when we'll move to 7.0.
//
double
VisG4TwigOps::getMass (G4LogicalVolume *vol, G4Material *material)
{
  G4LogicalVolume *logVol = vol;
  G4Material *logMaterial = material;
  if (!logMaterial)
  {
    QMessageBox::warning ((QWidget *) 0, "Logical Volume Mass: InvalidSetup", "No material associated to the logical volume!");
    return 0;
  }
    
  G4VSolid *logSolid = logVol->GetSolid ();
  if (!logSolid)
  {
    QMessageBox::warning ((QWidget *) 0, "Logical Volume Mass: InvalidSetup", "No solid associated to the logical volume!");
    return 0;
  }
    
  G4double globalDensity = logMaterial->GetDensity ();
  G4double mass = estimateCubicVolume (logSolid) * globalDensity;

  // For each daughter in the tree, subtract the mass occupied
  // and add the real daughter's one computed recursively

  int nDaughters = logVol->GetNoDaughters ();

  QProgressDialog progress("Calculating mass...", "Abort", 0, nDaughters);
    
  for(int it = 0; it < nDaughters; ++it)
  {
    progress.setValue(it);
    qApp->processEvents ();

    if(progress.wasCanceled())
      break;

    G4VPhysicalVolume *physDaughter = logVol->GetDaughter (it);
    G4LogicalVolume *logDaughter = physDaughter->GetLogicalVolume ();
    G4double subMass = 0.0;
    G4VSolid* daughterSolid = 0;
    G4Material* daughterMaterial = 0;
	
    // Compute the mass to subtract and to add for each daughter
    // considering its multiplicity (i.e. replicated or not) and
    // eventually its parameterisation (by solid and/or by material)
    //
    for (G4int i = 0; i < physDaughter->GetMultiplicity (); i++)
    {
      G4VPVParameterisation *physParam = physDaughter->GetParameterisation ();
      if (physParam)
      {
	daughterSolid = physParam->ComputeSolid (i, physDaughter);
	daughterSolid->ComputeDimensions (physParam, i, physDaughter);
	daughterMaterial = physParam->ComputeMaterial (i, physDaughter);
      }
      else
      {
	daughterSolid = logDaughter->GetSolid ();
	daughterMaterial = logDaughter->GetMaterial();
      }
      subMass = estimateCubicVolume (daughterSolid) * globalDensity;

      // Subtract the daughter's portion for the mass and add the real
      // daughter's mass computed recursively
      //
      mass = mass - subMass + getMass (logDaughter, daughterMaterial);
    }
  }
  progress.setValue(nDaughters);
  return mass;
}

//
// FIXME: Temporary function to calculate mass - 
// to be replaced with native G4 GetMass function 
// when we'll move to 7.0.
//
double
VisG4TwigOps::estimateCubicVolume (G4VSolid *solid)
{
    G4int nStat = 1000000;
    G4double epsilon = 0.001;
    G4VSolid *solidVol = solid;
    
    G4int iInside=0;
    G4double px,py,pz,minX,maxX,minY,maxY,minZ,maxZ,volume;
    G4bool yesno;
    G4ThreeVector p;
    EInside in;
    
    // values needed for CalculateExtent signature

    G4VoxelLimits limit;                // Unlimited
    G4AffineTransform origin;

    // min max extents of pSolid along X,Y,Z

    yesno = solidVol->CalculateExtent(kXAxis,limit,origin,minX,maxX);
    yesno = solidVol->CalculateExtent(kYAxis,limit,origin,minY,maxY);
    yesno = solidVol->CalculateExtent(kZAxis,limit,origin,minZ,maxZ);
    
    // limits

    if(nStat < 100)    nStat   = 100;
    if(epsilon > 0.01) epsilon = 0.01;

    for(G4int i = 0; i < nStat; i++ )
    {
	px = minX+(maxX-minX)*G4UniformRand();
	py = minY+(maxY-minY)*G4UniformRand();
	pz = minZ+(maxZ-minZ)*G4UniformRand();
	p  = G4ThreeVector(px,py,pz);
	in = solidVol->Inside(p);
	if(in != kOutside) iInside++;    
    }
    volume = (maxX-minX)*(maxY-minY)*(maxZ-minZ)*iInside/nStat;
    return volume;
}

void
VisG4TwigOps::doShowCategory (bool show, int id, VisG4VolumeTwig *twig)
{
    assert(twig);

    // Set twig visibility
    bool matches = false;
    switch (id)
    {
    case CAT_SENSITIVE:
	matches = (twig->volume ()->GetLogicalVolume ()
		   ->GetSensitiveDetector () != 0);
	break;

    case CAT_INSENSITIVE:
	matches = (twig->volume ()->GetLogicalVolume ()
		   ->GetSensitiveDetector () == 0);
	break;

    case CAT_BEAMPIPE:
    case CAT_CABLE:
    case CAT_COOLING:
    case CAT_MAGNET:
    case CAT_SUPPORT:
    case CAT_SENSITIVE_CONT:
    default:
	assert(false);
    }

    // Update twig itself
    if (matches)
	twig->selfVisible (show);
    twig->selfTraverse (true);
    twig->expand ();

    // Update children
    for (unsigned i = twig->children (); i; --i)
    {
	IgTwig *child = twig->child (i-1);
	assert(dynamic_cast<VisG4VolumeTwig *> (child));
	doShowCategory (show, id, static_cast<VisG4VolumeTwig *> (child));
    }
}

//////////////////////////////////////////////////////////////////////
void
VisG4TwigOps::actionShowMaterial (int id)
{
  // Pop up the material selector.  First populate the list with
  // known materials, select the ones we've selected in the past,
  // and then get the selection response from the user.
  QDialog	*matdiag = new QDialog (0, "Materials", true);
  QWidget 	*vbox = new QWidget;
  QVBoxLayout 	*vlayout = new QVBoxLayout;
  QLabel	*title = new QLabel (matdiag);
  QListWidget	*materials = new QListWidget (matdiag);
  QWidget 	*hbox = new QWidget;
  QHBoxLayout	*buttons = new QHBoxLayout (0, 0, 6);
  QPushButton	*ok = new QPushButton (matdiag);
  QPushButton *cancel = new QPushButton (matdiag);
  hbox->setLayout(buttons);
    
  matdiag->resize (300, 300);
  matdiag->setCaption ("Materials");

  title->setText ("Please select materials to show");
  ok->setText ("OK");
  cancel->setText ("Cancel");
  vlayout->addWidget (title);

  materials->setSelectionMode(QAbstractItemView::ExtendedSelection);
  vlayout->addWidget (materials);
  vbox->setLayout(vlayout);

  buttons->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));
  buttons->addWidget (ok);
  buttons->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));
  buttons->addWidget (cancel);
  buttons->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));
  //layout->addLayout (buttons);

  QGridLayout *grid = new QGridLayout;
  grid->addWidget(vbox);
  grid->addWidget(hbox);    
  matdiag->setLayout(grid);

  connect (ok, SIGNAL(clicked()), matdiag, SLOT(accept()));
  connect (cancel, SIGNAL(clicked()), matdiag, SLOT(reject()));

  // FIXME: Include only materials available in this subtree?
  // FIXME: This should be one integrated twig control centre that
  // follows current selection, and provides a widget stack to show
  // various selection criteria like by material, category, level,
  // etc.  (as well as other twig operations)
  assert(G4Material::GetMaterialTable ());
  const G4MaterialTable *mt = G4Material::GetMaterialTable ();
  for (G4MaterialTable::const_iterator i = mt->begin(); i != mt->end(); ++i)
  {
    QListWidgetItem * item = new QListWidgetItem(QString::fromStdString((*i)->GetName ().c_str ()));
    materials->addItem(item);
    materials->setCurrentItem(item);
    //materials->insertItem ((*i)->GetName ().c_str ());
    //materials->setSelected (materials->count ()-1,
    //			m_selected.count ((*i)->GetName ()));
  }
  materials->sortItems();

  if (matdiag->exec () != QDialog::Accepted)
    return;

  for(int i = 0; i < materials->count(); ++i)
  {
    QListWidgetItem * item = materials->item(i);
    std::string name(item->text().toStdString());
    if (item->isSelected())
      m_selected.insert (name);
    else
      m_selected.erase (name);
  }

  // FIXME: Enable upwards?  Does this material cut apply from here
  // downwards or is it a global operation for the whole scene graph
  // at render time?  If the latter, it belongs to a top-level menu
  // and not here.  For now make it work like the others.
  QApplication::setOverrideCursor (Qt::waitCursor);
  bool	show = id == MENU_SHOW_MATERIAL;
  IgTwig	*update = m_twig;
  for (IgTwig *t = m_twig->parent(); show && t; update = t, t = t->parent())
    t->selfTraverse (true);

  // Update nested twigs to match this material cut
  doShowMaterial (show, m_twig);

  // FIXME: menu needs to be updated!
  IgRepSet::invalidate (update, IgTwig::FLAGS_MASK | IgTwig::STRUCTURE_MASK);
  QApplication::restoreOverrideCursor ();
}

void
VisG4TwigOps::doShowMaterial (bool show, VisG4VolumeTwig *twig)
{
  assert(twig);

  // Set twig visibility
  if (m_selected.count (twig->volume ()->GetLogicalVolume ()
			->GetMaterial ()->GetName ()))
    twig->selfVisible (show);
  twig->expand ();
  twig->selfTraverse (true);

  unsigned inum = twig->children ();
  QProgressDialog progress("Show Material...", "Abort", 0, inum);

  // Update children
  for(unsigned i = inum; i; --i)
  {
    progress.setValue(i);
    qApp->processEvents ();

    if (progress.wasCanceled ())
      break;

    IgTwig *child = twig->child (i-1);
    assert(dynamic_cast<VisG4VolumeTwig *> (child));
    doShowMaterial (show, static_cast<VisG4VolumeTwig *> (child));
  }
  progress.setValue(inum);
}

//////////////////////////////////////////////////////////////////////
void
VisG4TwigOps::actionRender (int id)
{ 
  assert(m_changed.empty ()); 
  doRender (id, m_twig, false); 
  m_changed.clear (); 
}

void
VisG4TwigOps::actionRenderAll (int id)
{
  // Update twig and all its children.  Don't expand the tree.
  QApplication::setOverrideCursor (Qt::waitCursor);
  assert(m_changed.empty ());
  doRender (id, m_twig, true);
  m_changed.clear ();
  QApplication::restoreOverrideCursor ();
}

void
VisG4TwigOps::doRender (int id, VisG4VolumeTwig *twig, bool recurse)
{
  // VisG4GeometryTwig guarantees that every logical volume has vis
  // attrs and that they are unique.  (FIXME: If this volume is in
  // physical mode, the attrs are still shared by many twigs and we
  // have to be careful about notifying they all have changed, or
  // rather, make sure each gets separate visualisation settings.)

  bool		changed = false;
  G4VisAttributes	*attrs = const_cast<G4VisAttributes *>
				 (twig->volume ()
				  ->GetLogicalVolume ()
				  ->GetVisAttributes ());
  assert(attrs);
  switch (id)
  {
  case MENU_RENDER_AS_IS:
    // Retain current force style, but turn force off
    changed = attrs->IsForceDrawingStyle ();
    if (attrs->GetForcedDrawingStyle () == G4VisAttributes::solid)
      attrs->SetForceSolid (false);
    else
      attrs->SetForceWireframe (false);
    break;

  case MENU_RENDER_SOLID:
    changed = (! attrs->IsForceDrawingStyle ()
	       || (attrs->GetForcedDrawingStyle ()
		   != G4VisAttributes::solid));
    attrs->SetForceSolid (true);
    break;

  case MENU_RENDER_WIREFRAME:
    changed = (! attrs->IsForceDrawingStyle ()
	       || (attrs->GetForcedDrawingStyle ()
		   != G4VisAttributes::wireframe));
    attrs->SetForceWireframe (true);
    break;

  default:
    assert(false);
    break;
  }

  if (changed || m_changed.count (attrs))
  {
    IgRepSet::invalidate (twig, IgTwig::SELF_MASK);
    m_changed.insert (attrs);
  }

  for (unsigned i = twig->children (); recurse && i; --i)
  {
    IgTwig *child = twig->child (i-1);
    assert(dynamic_cast<VisG4VolumeTwig *> (child));
    doRender (id, static_cast<VisG4VolumeTwig *> (child), true);
  }
}

void
VisG4TwigOps::actionColour (int /* id */)
{
  // VisG4GeometryTwig guarantees that every logical volume has vis
  // attrs and that they are unique.  (FIXME: If this volume is in
  // physical mode, the attrs are still shared by many twigs and we
  // have to be careful about notifying they all have changed, or
  // rather, make sure each gets separate visualisation settings.)

  G4VisAttributes *attrs = const_cast<G4VisAttributes *>
			   (m_twig->volume ()
			    ->GetLogicalVolume ()
			    ->GetVisAttributes ());
  assert(attrs);

  // Get the old colour.
  G4Colour	colour = attrs->GetColour ();

  // Get the new colour.
  QColor	old (int (colour.GetRed () * 255),
		     int (colour.GetGreen () * 255),
		     int (colour.GetBlue () * 255));
  QColor	c = QColorDialog::getColor (old, 0);

  // If the dialog was accepted, change the colour.
  if	(c.isValid ())
  {
    attrs->SetColour (c.red () / 255.,
		      c.green () / 255.,
		      c.blue () / 255.,
		      colour.GetAlpha ());

    IgRepSet::invalidate (m_twig, IgTwig::SELF_MASK);
  }
}

void
VisG4TwigOps::actionTransparency (int /* id */)
{
  // VisG4GeometryTwig guarantees that every logical volume has vis
  // attrs and that they are unique.  (FIXME: If this volume is in
  // physical mode, the attrs are still shared by many twigs and we
  // have to be careful about notifying they all have changed, or
  // rather, make sure each gets separate visualisation settings.)

  G4VisAttributes *attrs = const_cast<G4VisAttributes *>
			   (m_twig->volume ()
			    ->GetLogicalVolume ()
			    ->GetVisAttributes ());
  assert(attrs);

  // Get the old colour.
  G4Colour	colour = attrs->GetColour ();

  // Get the new colour.
  bool	ok = false;
  double	alpha = QInputDialog::getDouble
			("IGUANA", "Please select transparency (1.0 = opaque)",
			 colour.GetAlpha (), 0, 1.0, 3, &ok);
  if (ok)
  {
    attrs->SetColour (colour.GetRed (),
		      colour.GetGreen (),
		      colour.GetBlue (),
		      alpha);
    IgRepSet::invalidate (m_twig, IgTwig::SELF_MASK);
  }
}

void
VisG4TwigOps::actionApplyFilter (int id)
{
  // Pop up the filter selector.  First populate the list with
  // known filters, select the ones we've selected in the past,
  // and then get the selection response from the user.
  QDialog	*filterDialog = new QDialog (0, "Filters", true);
  QWidget 	*vbox = new QWidget;
  QVBoxLayout 	*vlayout = new QVBoxLayout;
  QLabel	*title = new QLabel (filterDialog);
  QListWidget	*filters = new QListWidget (filterDialog);
  QWidget 	*hbox = new QWidget;
  QHBoxLayout	*buttons = new QHBoxLayout (0, 0, 6);
  QComboBox	*boolOperation = new QComboBox (filterDialog);    
  QPushButton	*ok = new QPushButton (filterDialog);
  QPushButton 	*cancel = new QPushButton (filterDialog);
  hbox->setLayout(buttons);
    
  filterDialog->resize (300, 300);
  filterDialog->setCaption ("Filters");

  title->setText ("Please select the filter to apply");
  ok->setText ("OK");
  cancel->setText ("Cancel");
  vlayout->addWidget (title);

  filters->setSelectionMode(QAbstractItemView::ExtendedSelection);
  vlayout->addWidget (filters);

  boolOperation->insertItem ("Must match all selected filters.");
  boolOperation->insertItem ("Must match at least one of the selected filters.");
  boolOperation->insertItem ("Must match only one of the selected filters.");
  vlayout->addWidget (boolOperation);    
  vbox->setLayout(vlayout);
    
  buttons->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));
  buttons->addWidget (ok);
  buttons->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));
  buttons->addWidget (cancel);
  buttons->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));

  QGridLayout *grid = new QGridLayout;
  grid->addWidget(vbox);
  grid->addWidget(hbox);    
  filterDialog->setLayout(grid);

  connect (ok, SIGNAL(clicked()), filterDialog, SLOT(accept()));
  connect (cancel, SIGNAL(clicked()), filterDialog, SLOT(reject()));

  // FIXME: Include only materials available in this subtree?
  // FIXME: This should be one integrated twig control centre that
  // follows current selection, and provides a widget stack to show
  // various selection criteria like by material, category, level,
  // etc.  (as well as other twig operations)
    
  assert(VisG4FilterTable::instance ());
  VisG4FilterTable *ft = VisG4FilterTable::instance ();

  assert(ft->begin () != ft->end ());
    
  for (VisG4FilterTable::const_iterator i = ft->begin (); i != ft->end (); ++i)
  {
    VisG4Filter &filter = *(i->second);

    QListWidgetItem * item = new QListWidgetItem(QString::fromStdString(filter.name()));
    filters->addItem(item);
    filters->setCurrentItem(item);
    //    filters->count ()-1,
    //	  m_selected.count (filter.name ().c_str ()));
  }
    
  filters->sortItems();
    
    
  if (filterDialog->exec () != QDialog::Accepted)
    return;

  VisG4VolumeTwig::m_visualizationFilter = 0;
  VisG4Filter *currentFilter=0;
  VisG4Filter *previousFilter=0;    

  int boolOperationValue = boolOperation->currentItem ();

  for(int i = 0; i < filters->count(); ++i)
  {
    QListWidgetItem * item = filters->item(i);
    std::string name (item->text().toStdString());
    if(item->isSelected())
    {
      m_selected.insert(name);	    
      currentFilter = (*ft)[name.c_str ()];
      assert(currentFilter);
	    
      if (VisG4VolumeTwig::m_visualizationFilter == 0)
      {		
	VisG4VolumeTwig::m_visualizationFilter = currentFilter;
      }
	    
      if (previousFilter != 0)
      {
	previousFilter->addFilter (currentFilter);		
      }
	    
      currentFilter->setCompositionOperation ((VisG4Filter::CompositionOperations) boolOperationValue);	    
      previousFilter = currentFilter;
	    
      std::cerr << currentFilter->name () << std::endl;	    
    }
    else	
      m_selected.erase(name);
  }	
    
  if (VisG4VolumeTwig::m_visualizationFilter != 0)
  {	
    QApplication::setOverrideCursor (Qt::waitCursor);
    // Enable upwards if in show (FIXME: Reuse IgTwigOps)
    IgTwig *update = m_twig;
    for (IgTwig *t = m_twig->parent (); t; update = t, t = t->parent ())
      t->selfTraverse (true);
    
    doApplyFilter (true, id, m_twig);
    IgRepSet::invalidate (update, IgTwig::FLAGS_MASK | IgTwig::STRUCTURE_MASK);
    QApplication::restoreOverrideCursor ();
  }    
}

void
VisG4TwigOps::doApplyFilter (bool show, int id, VisG4VolumeTwig *twig)
{
  assert(twig);

  // Dig out some common variables.
  VisG4VolumeTwig	*root = twig;
  while (VisG4VolumeTwig *x = dynamic_cast<VisG4VolumeTwig *> (root->parent()))
    root = x;        
  assert(root);    

  G4VPhysicalVolume	*volume = root->volume ();
  assert(volume);

  bool		match = false;    

  for (unsigned int i = 0; i < twig->paths (); ++i)
  {
    bool			replica = false;
    G4VPhysicalVolume	*pv = volume;
    G4LogicalVolume		*lv = 0;
    G4VSolid		*solid = 0;
    G4Transform3D		pos;

    VisG4VolumePicker::apply (twig->path (i), pv, lv, solid, pos, replica);
    assert(solid);
    assert(pv);
    assert(lv);

    // Create a VisSoMaterialDetails structure which describes the
    // current twig.
    VisSoMaterialDetails details;
    details << VisG4MaterialDetails (lv->GetMaterial ());
    details << VisG4PVDetails (pv);	
    details << VisG4PathDetails (twig->fullName ());	

    //Enable the twig only if the details pass the test
    if (VisG4VolumeTwig::m_visualizationFilter->compute (details))
    {
      match |= true;	
    }
  }
    
  if (match == true)
    twig->selfVisible (show);
  twig->selfTraverse (true);
  twig->expand ();

        
  // Update children
  for (unsigned i = twig->children (); i; --i)
  {
    IgTwig *child = twig->child (i-1);
    assert(dynamic_cast<VisG4VolumeTwig *> (child));
    doApplyFilter (show, id, static_cast<VisG4VolumeTwig *> (child));
  }    
}

void
VisG4TwigOps::actionAppearance(int /* id */)
{
  QDialog *materialDialog = new QDialog(0, "Appearance", true);
  QWidget *vbox = new QWidget;
  QVBoxLayout *vlayout = new QVBoxLayout;
  QLabel	*title = new QLabel (materialDialog);
  IgMatEdMainWidget  *materialEditor = new IgMatEdMainWidget(materialDialog);    
  QWidget *hbox = new QWidget;
  QHBoxLayout	*buttons = new QHBoxLayout (0, 0, 6);
  QPushButton *save = new QPushButton (materialDialog);
  QPushButton	*ok = new QPushButton (materialDialog);
  QPushButton *cancel = new QPushButton (materialDialog);
  hbox->setLayout(buttons);
      
  materialDialog->resize (700, 300);
  materialDialog->setCaption ("Appearance");

  title->setText ("Configure current object material");    
  save->setText ("Save");
  ok->setText ("OK");
  cancel->setText ("Cancel");
  vlayout->addWidget(title);
  vlayout->addWidget(materialEditor);
  vbox->setLayout(vlayout);

  QGridLayout *grid = new QGridLayout;
  grid->addWidget(vbox);
  grid->addWidget(hbox);    
  materialDialog->setLayout(grid);

  buttons->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));
  buttons->addWidget (save);
  buttons->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));
  buttons->addWidget (ok);
  buttons->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));
  buttons->addWidget (cancel);
  buttons->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));

  connect (save, SIGNAL(clicked()), materialDialog, SLOT(accept()));
  connect (save, SIGNAL(clicked()), materialDialog, SLOT(show()));
  connect (ok, SIGNAL(clicked()), materialDialog, SLOT(accept()));
  connect (cancel, SIGNAL(clicked()), materialDialog, SLOT(reject()));

  // Get the name of the material associated with the twig.
  VisG4VolumeTwig	*root = m_twig;
  while (VisG4VolumeTwig *x = dynamic_cast<VisG4VolumeTwig *> (root->parent()))
    root = x;        
  assert(root);    

  G4VPhysicalVolume	*volume = root->volume ();
  assert(volume);
    
  bool		replica = false;
  G4VPhysicalVolume	*pv = volume;
  G4LogicalVolume	*lv = 0;
  G4VSolid		*solid = 0;
  G4Transform3D	pos;    

  assert(m_twig->paths ());
    
  VisG4VolumePicker::apply (m_twig->path (0), pv, lv, solid, pos, replica);

  materialEditor->setCurrentG4Material (QString::fromStdString (lv->GetMaterial ()->GetName ()));
        
  save->show ();
  ok->show ();
  cancel->show ();
  
  if (materialDialog->exec () != QDialog::Accepted)
    return;    

  materialEditor->saveMaterialMap ();
  materialEditor->saveSoMaterials ();    
    
  IgRepSet::invalidate (m_twig, IgTwig::SELF_MASK);
}
