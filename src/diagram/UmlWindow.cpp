// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// home   : http://bouml.free.fr
//
// *************************************************************************





#include <qcursor.h>
#include <qworkspace.h>
#include <qsplitter.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qmultilineedit.h>
#include <qstatusbar.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#ifndef QT_NO_PRINTER
#include <qprinter.h>
#endif
#include <qwhatsthis.h>
#include <qapplication.h>
#include <qfiledialog.h>
#include <qfileinfo.h> 
#include <qwindowsstyle.h> 
#include <qmotifstyle.h> 
#include <qmotifplusstyle.h> 
//#include <qcdestyle.h> 
//#include <qsgistyle.h>

#include "UmlWindow.h"
#include "UmlDesktop.h"
#include "BrowserView.h"
#include "BrowserPackage.h"
#include "BrowserDiagram.h"
#include "BrowserClass.h"
#include "BrowserState.h"
#include "BrowserActivity.h"
#include "BrowserUseCase.h"
#include "BrowserComponent.h"
#include "BrowserArtifact.h"
#include "BrowserDeploymentNode.h"
#include "DiagramWindow.h"
#include "DiagramView.h"
#include "GenerationSettings.h"
#include "TraceDialog.h"
#include "EnvDialog.h"
#include "HelpDialog.h"
#include "Tool.h"
#include "ToolDialog.h"
#include "BrowserSearchDialog.h"
#include "ShortcutDialog.h"
#include "GreetingsDialog.h"
#include "BasicData.h"
#include "ToolCom.h"
#include "About.h"
#include "UmlGlobal.h"
#include "UmlPixmap.h"
#include "DialogUtil.h"
#include "MenuTitle.h"
#include "Shortcut.h"
#include "myio.h"
#include "strutil.h"
#include "mu.h"
#include "err.h"
#include "translate.h"

#include "filesave.xpm"
#include "fileopen.xpm"
#include "fileprint.xpm"
#include "browsersearch.xpm"

static QString TemplateProject;

QString template_project()
{
  return TemplateProject;
}

void set_template_project(QString s)
{
  TemplateProject = s;
}

//

static int Counter;

UmlWindow * UmlWindow::the;

QString projectNewText() { return TR("To create a <i>new project</i>.<br><br>"
				     "This will ask you to indicate where to create a new directory having the name of the project "
				     "where all the project files will be placed."); }
QString projectNewFromTemplateText() { return TR("To create a <i>new project</i> from an already existing one.<br><br>"
						 "This will ask you to indicate where to create a new directory having the name of the project "
						 "where all the project files will be placed."); }
QString projectOpenText() { return TR("To open a <i>project</i>.<br><br>"
				      "You can also select the <b>Open command</b> from the Project menu."); }
QString projectSaveText() { return TR("To save the project.<br><br>"
				      "You can also select the Save command from the Project menu."); }
QString projectSaveAsText() { return TR("To save all in a new project."); }
QString diagramPrintText() { return TR("To print the current diagram."); }
QString cppText() { return TR("To manage or not C++ and to set or not the C++ definition/declaration "
			      "to the default value when a class/operation/attribute/relation/artefact is created"); }
QString javaText() { return TR("To manage or not Java and to set or not the Java definition "
			       "to the default value when a class/operation/attribute/relation/artefact is created"); }
QString phpText() { return TR("To manage or not Php and to set or not the Php definition "
			      "to the default value when a class/operation/attribute/relation/artefact is created"); }
QString pythonText() { return TR("To manage or not Python and to set or not the Python definition "
				 "to the default value when a class/operation/attribute/relation/artefact is created"); }
QString idlText() { return TR("To manage or not IDL and to set or not the IDL definition/declaration "
			      "to the default value when a class/operation/attribute/relation/artefact is created"); }
QString verboseText() { return TR("To ask or not for a verbose code generation"); }
QString preserve_bodiesText() { return TR("To preserve or not the operations's body"); }
QString add_operation_profileText() { return TR("To write the operation profile at the beginning of the temporary file when you edit an operation's body"); }
QString viewBrowserStereotypeText() { return TR("To show or hide the <i>stereotypes</i> "
						"in the <b>Browser</b>."); }
QString browserUpText() { return TR("To select the <i>browser</i> current item's parent."); }
QString browserSearchText() { return TR("To search an item in the <i>browser</i>."); }
QString fontSizeMenuText() { return TR("To set the point size of the base font used in the "
				       "<i>browser</i> and the diagrams."); }
QString formatMenuText() { return TR("To set the default format of the diagrams."); }
static QString prevText() { return TR("To select the previously selected element in the <i>browser</i>."); }
static QString nextText() { return TR("To select the next selected element in the <i>browser</i>."); }
static QString completionText() { return TR("To ask or not for an auto completion (non case sensitive) in choice list (<i>combo box</i>)"); }

UmlWindow::UmlWindow(bool batch) : QMainWindow(0, "Bouml", WDestructiveClose) {
  setCaption("Bouml");
  
  the = this;
  commented = 0;
  format = IsoA4;
  
  //
  // buttons and menus
  //
  
  QPixmap openIcon, saveIcon;
  
  projectTools = new QToolBar(this, "project operations");
  addToolBar(projectTools, TR("Toolbar"), Top, TRUE);
  
  openIcon = QPixmap(fileopen);
  QToolButton * projectOpen
    = new QToolButton(openIcon, TR("Open Project"), QString::null,
		      this, SLOT(load()), projectTools, "open project");
  
  saveIcon = QPixmap(filesave);
  QToolButton * projectSave
    = new QToolButton(saveIcon, TR("Save Project"), QString::null,
		      this, SLOT(save()), projectTools, "save project");
  
#ifndef QT_NO_PRINTER
  QPixmap printIcon;
  
  printIcon = QPixmap(fileprint);
  QToolButton * diagramPrint
    = new QToolButton(printIcon, TR("Print diagram"), QString::null,
		      this, SLOT(print()), projectTools, "print diagram");
  QWhatsThis::add(diagramPrint, diagramPrintText());
#endif
  
  QPixmap searchIcon = QPixmap(browsersearch);
  QToolButton * browserSearch
    = new QToolButton(searchIcon, TR("Browser search"), QString::null,
		      this, SLOT(browser_search()), projectTools, "browser search");
  QWhatsThis::add(browserSearch, browserSearchText());
  
  prev = new QToolButton(*leftPixmap, TR("previous selected"), QString::null,
			 this, SLOT(prev_select()), projectTools, "previous selected");
  QWhatsThis::add(prev, prevText());
  
  next = new QToolButton(*rightPixmap, TR("next selected"), QString::null,
			 this, SLOT(next_select()), projectTools, "next selected");
  QWhatsThis::add(next, nextText());
  
  (void)QWhatsThis::whatsThisButton(projectTools);

  QWhatsThis::add(projectOpen, projectOpenText());
  QWhatsThis::add(projectSave, projectSaveText());
  
  projectMenu = new QPopupMenu(this);
  menuBar()->insertItem(TR("&Project"), projectMenu);
  connect(projectMenu, SIGNAL(aboutToShow()),
	  this, SLOT(projectMenuAboutToShow()));
    
  windowsMenu = new QPopupMenu(this);
  windowsMenu->setCheckable(TRUE);
  connect(windowsMenu, SIGNAL(aboutToShow()),
	  this, SLOT(windowsMenuAboutToShow()));
  menuBar()->insertItem(TR("&Windows"), windowsMenu);
  
  toolMenu = new QPopupMenu(this);
  connect(toolMenu, SIGNAL(aboutToShow()), this, SLOT(toolMenuAboutToShow()));
  menuBar()->insertItem(TR("&Tools"), toolMenu);

  langMenu = new QPopupMenu(this);
  menuBar()->insertItem(TR("&Languages"), langMenu);
  langMenu->setCheckable(TRUE);
  connect(langMenu, SIGNAL(aboutToShow()),
	  this, SLOT(langMenuAboutToShow()));
  
  use_cpp_id =
    langMenu->insertItem(TR("C++ management and default declaration/definition"), this, SLOT(use_cpp()));
  langMenu->setItemChecked(use_cpp_id, GenerationSettings::cpp_get_default_defs());
  langMenu->setWhatsThis(use_cpp_id, cppText());
  use_java_id =
    langMenu->insertItem(TR("Java management and default definition"), this, SLOT(use_java()));
  langMenu->setItemChecked(use_java_id, GenerationSettings::java_get_default_defs());
  langMenu->setWhatsThis(use_java_id, javaText());
  use_php_id =
    langMenu->insertItem(TR("Php management and default definition"), this, SLOT(use_php()));
  langMenu->setItemChecked(use_php_id, GenerationSettings::php_get_default_defs());
  langMenu->setWhatsThis(use_php_id, phpText());
  use_python_id =
    langMenu->insertItem(TR("Python management and default definition"), this, SLOT(use_python()));
  langMenu->setItemChecked(use_python_id, GenerationSettings::python_get_default_defs());
  langMenu->setWhatsThis(use_python_id, pythonText());
  use_idl_id =
    langMenu->insertItem(TR("Idl management and default declaration"), this, SLOT(use_idl()));
  langMenu->setItemChecked(use_idl_id, GenerationSettings::idl_get_default_defs());
  langMenu->setWhatsThis(use_idl_id, idlText());
  
  langMenu->insertSeparator();
  verbose_gen_id =
    langMenu->insertItem(TR("Verbose code generation"), this, SLOT(verbose()));
  langMenu->setWhatsThis(verbose_gen_id, verboseText());

  preserve_bodies_id =
    langMenu->insertItem(TR("Preserve operations's body"), this, SLOT(preserve()));
  langMenu->setWhatsThis(preserve_bodies_id, preserve_bodiesText());

  add_operation_profile_id =
    langMenu->insertItem(TR("Add operation profile on body edition"), this,
			 SLOT(addoperationprofile()));
  langMenu->setWhatsThis(add_operation_profile_id, add_operation_profileText());
  
  miscMenu = new QPopupMenu(this);
  menuBar()->insertItem(TR("&Miscellaneous"), miscMenu);
  miscMenu->setCheckable(TRUE);
  connect(miscMenu, SIGNAL(aboutToShow()),
	  this, SLOT(miscMenuAboutToShow()));
  
  show_browser_stereotypes_id =
    miscMenu->insertItem(TR("Show s&tereotypes in browser"), this,
			 SLOT(show_stereotypes()));
  miscMenu->setItemChecked(show_browser_stereotypes_id, TRUE);
  miscMenu->setWhatsThis(show_browser_stereotypes_id, viewBrowserStereotypeText());
  
  completion_id =
    miscMenu->insertItem(TR("Completion in dialog"), this,
			 SLOT(do_completion()));
  miscMenu->setItemChecked(completion_id, TRUE);
  miscMenu->setWhatsThis(completion_id, completionText());
  
  QPopupMenu * pmstyle = new QPopupMenu(this);
  bool used = FALSE;

#ifndef QT_NO_STYLE_MOTIF
  pmstyle->insertItem("Motif", this, SLOT(motif_style()));
  used = TRUE;
#endif
#ifndef QT_NO_STYLE_MOTIFPLUS
  pmstyle->insertItem("MotifPlus", this, SLOT(motifplus_style()));
  used = TRUE;
#endif
  //pmstyle->insertItem("Sgi", this, SLOT(sgi_style()));
  //pmstyle->insertItem("Cde", this, SLOT(cde_style()));
#ifndef QT_NO_STYLE_WINDOWS
  pmstyle->insertItem("Windows", this, SLOT(windows_style()));
  used = TRUE;
#endif
  
  if (used)
    miscMenu->insertItem(TR("Style"), pmstyle);
  style = '?';
  
  fontSizeMenu = new QPopupMenu(this);
  fontSizeMenu->setCheckable(TRUE);
  connect(fontSizeMenu, SIGNAL(aboutToShow()),
	  this, SLOT(fontSizeMenuAboutToShow()));

#define FONTSIZEMIN 6
#define FONTSIZESUP 16
  
  for (int i = FONTSIZEMIN; i < FONTSIZESUP; i += 1)
    fontSizeMenu->insertItem(QString::number(i), this,
			     SLOT(setFontSize(int)), 0, i);

  int id = miscMenu->insertItem(TR("Font size"), fontSizeMenu);
  miscMenu->setWhatsThis(id, fontSizeMenuText());
  
  formatMenu = new QPopupMenu(this);
  formatLandscapeMenu = new QPopupMenu(this);  
  connect(formatMenu, SIGNAL(aboutToShow()),
	  this, SLOT(formatMenuAboutToShow()));
  init_format_menu(formatMenu, formatLandscapeMenu);

  id = miscMenu->insertItem(TR("Diagram default format"), formatMenu);
  miscMenu->setWhatsThis(id, formatMenuText());
  
  miscMenu->insertSeparator();
  
  shortcut_id =
    miscMenu->insertItem(TR("Edit shortcuts"), this, SLOT(edit_shortcuts()));
  
  menuBar()->insertSeparator();
  
  miscMenu->insertItem(TR("Set environment"), this, SLOT(edit_env()));
  
  img_root_dir_id = 
    miscMenu->insertItem(TR("Set images root dir"), this, SLOT(edit_image_root_dir()));
  
  QPopupMenu * help = new QPopupMenu(this);
  menuBar()->insertItem(TR("&Help"), help);
  
  help->insertItem(TR("&About"), this, SLOT(about()), ::Qt::Key_F2);
  help->insertItem(TR("About&Qt"), this, SLOT(aboutQt()));
  help->insertSeparator();
  help->insertItem(TR("&Help"), this, SLOT(help()), ::Qt::Key_F1);
  help->insertItem(TR("What's This"), this, SLOT(whatsThis()), ::Qt::SHIFT+::Qt::Key_F1);
    
  //
  // sub windows
  //
  
  spl1 = new QSplitter(QSplitter::Horizontal, this, "spl1");
  spl2 = new QSplitter(QSplitter::Vertical, spl1, "spl2");
  
  browser = new BrowserView(spl1);
  
  ws = new QWorkspace(spl2);
  
  comment = new MultiLineEdit(spl2);
  set_commented(0);
  connect(comment, SIGNAL(textChanged()), this, SLOT(comment_changed())); 
  
  spl1->moveToFirst(browser);
  spl2->moveToFirst(ws);
  
  int w = (UmlDesktop::width() * 15)/16;
  
  resize(w, (UmlDesktop::height() * 7)/8);
  UmlDesktop::tocenter(this);
  
  // browser width = total width/4
  QValueList<int> lsz = spl1->sizes();
  
  lsz.first() = w/4;
  lsz.last() = w - lsz.first();
  
  spl1->setSizes(lsz);
  
  // on resize try to resize only the diagrams
  spl1->setResizeMode(browser, QSplitter::KeepSize);
  spl2->setResizeMode(comment, QSplitter::KeepSize);

#ifdef _WS_QWS_
  // Qt/Embedded XOR drawing not yet implemented.
  spl1->setOpaqueResize(TRUE);
  spl2->setOpaqueResize(TRUE);
#endif
  
  setCentralWidget(spl1);  
  
  //
  // read historic
  //
  
  // note : QFile fp(QDir::home().absFilePath(".bouml")) doesn't work
  // if the path contains non latin1 characters, for instance cyrillic !
  QString s = homeDir().absFilePath(".bouml");
  FILE * fp = fopen((const char *) s, "r");
  
  if (fp != 0) {
    char line[512];
 
    while (fgets(line, sizeof(line) - 1, fp) != 0) {
      remove_crlf(line);
      if (!strncmp(line, "compteur d utilisation ", 23)) {
	Counter = atoi(line + 23);
	break;
      }
      else
	historic.append(line);
    }
    
    fclose(fp);
  }
    
  if (!batch && ((Counter++ % 30) == 0)) {
    GreetingsDialog d;
    
    d.exec();
  }
    
  //
  
  clear_select_historic();
  
  //
  
  statusBar()->message(TR("Ready"), 2000);
}

UmlWindow::~UmlWindow() {
  BrowserView::remove_temporary_files();
  set_user_id(-1);
  exit(0);
}

void UmlWindow::init_format_menu(QPopupMenu * m, QPopupMenu * lm)
{
  m->setCheckable(TRUE);
  lm->setCheckable(TRUE);
  
  int i;
  
  for (i = 0; i <= IsoA5; i += 1)
    m->insertItem(QString("Iso ") + stringify((CanvasFormat) i), this,
		  SLOT(setFormat(int)), 0, i);
  
  for (; i <= UsE; i += 1)
    m->insertItem(QString("Ansi ") + stringify((CanvasFormat) i), this,
		  SLOT(setFormat(int)), 0, i);
  
  for (; i != IsoA0Landscape; i += 1)
    m->insertItem(stringify((CanvasFormat) i), this,
		  SLOT(setFormat(int)), 0, i);
  
  m->insertSeparator();
  m->insertItem(TR("Landscape formats"), lm);
    
  for (; i <= IsoA5Landscape; i += 1)
    lm->insertItem(QString("Iso ") + stringify((CanvasFormat) i), this,
		   SLOT(setFormat(int)), 0, i);
  
  for (; i <= UsELandscape; i += 1)
    lm->insertItem(QString("Ansi ") + stringify((CanvasFormat) i), this,
		   SLOT(setFormat(int)), 0, i);
  
  for (; i != CanvasFormatSup; i += 1)
    lm->insertItem(stringify((CanvasFormat) i), this,
		   SLOT(setFormat(int)), 0, i);
}

void UmlWindow::projectMenuAboutToShow() {
  abort_line_construction();
  
  projectMenu->clear();
  
  if (!BrowserNode::edition_active()) {
    (void) user_id(); // force boumlrc read to have TemplateProject
    
    int id;
    bool enabled =  (browser->get_project() != 0);
    QPixmap openIcon = QPixmap(fileopen);
    QPixmap saveIcon = QPixmap(filesave);
    
    id = projectMenu->insertItem(TR("&New"), this, SLOT(newProject()));
    projectMenu->setWhatsThis(id, projectNewText());
    
    if (!TemplateProject.isEmpty()) {
      id = projectMenu->insertItem(TR("Create from &Template"), this, SLOT(newFromTemplate()));
      projectMenu->setWhatsThis(id, projectNewFromTemplateText());
    }
    
    id = projectMenu->insertItem(openIcon, TR("&Open"), this, SLOT(load()),
				 ::Qt::CTRL+::Qt::Key_O);
    projectMenu->setWhatsThis(id, projectOpenText());
    
    id = projectMenu->insertItem(saveIcon, TR("&Save"), this, SLOT(save()),
				 ::Qt::CTRL+::Qt::Key_S);
    projectMenu->setWhatsThis(id, projectSaveText());
    projectMenu->setItemEnabled(id, enabled);
    
    id = projectMenu->insertItem(TR("Save &as..."), this, SLOT(saveAs()),
				 ::Qt::CTRL+::Qt::SHIFT+::Qt::Key_S);
    projectMenu->setWhatsThis(id, projectSaveAsText());
    projectMenu->setItemEnabled(id, enabled);
    
    projectMenu->insertSeparator();
    projectMenu->insertItem(TR("&Close"), this, SLOT(close()),
			    ::Qt::CTRL+::Qt::Key_W);
    projectMenu->setItemEnabled(id, enabled);
    projectMenu->insertItem(TR("&Quit"), this, SLOT(quit()),
			    ::Qt::CTRL+::Qt::Key_Q);
    
    // edit
    
    if (enabled) {
      bool writable = browser->get_project()->is_writable();
      QPopupMenu * ed = new QPopupMenu(this);
    
      projectMenu->insertSeparator();
      projectMenu->insertItem(TR("Edit"), ed);
      
      id = ed->insertItem(TR("Edit generation settings"), this, SLOT(edit_gen_settings()));
      ed->setWhatsThis(id, TR("to set how an Uml type is compiled in C++ etc..., \
to set the default parameter passing, to set the default code \
produced for an attribute etc..., and to set the root directories"));
      ed->setItemEnabled(id, writable);
      
      id = ed->insertItem(TR("Edit default stereotypes"), this, SLOT(edit_stereotypes()));
      ed->setWhatsThis(id, TR("to set the default stereotypes list"));
      ed->setItemEnabled(id, writable);
      
      ed->insertSeparator();
      
      id = ed->insertItem(TR("Edit class settings"), this, SLOT(edit_class_settings()));
      ed->setWhatsThis(id, TR("to set the sub classes settings"));
      ed->setItemEnabled(id, writable);
      
      id = ed->insertItem(TR("Edit drawing settings"), this, SLOT(edit_drawing_settings()));
      ed->setWhatsThis(id, TR("to set how the sub <i>diagrams</i>'s items must be drawn"));
      ed->setItemEnabled(id, writable);
    }
    
    // historic
    
    projectMenu->insertSeparator();
    QString whats = TR("to open this project.<br><br>The historic is saved in <i>%1</i>",
		       homeDir().absFilePath(".bouml"));
    
    for (int i = 0; i < int(historic.count()); i += 1) {
      id = projectMenu->insertItem(*historic.at(i),
				   this, SLOT(historicActivated(int)));
      projectMenu->setItemParameter(id, i);
      projectMenu->setWhatsThis(id, whats);
    }
  }
  else
    msg_warning("Bouml",
		TR("Nothing available while a dialog is opened"));
}

void UmlWindow::historicActivated(int id) {
  QStringList::Iterator it = historic.at(id);
  
  if (it != historic.end()) {
    close();
  
    if (browser->get_project() == 0)
      load(*it);
    else
      statusBar()->message(TR("Loading aborted"), 2000);
  }
}

void UmlWindow::clear()
{
  while (! the->ws->windowList().isEmpty()) {
    DiagramWindow * w = (DiagramWindow *) the->ws->windowList().at(0);
    
    w->dont_save();
    w->close(TRUE);
  }
  
  if (BrowserSearchDialog::get() != 0)
    BrowserSearchDialog::get()->close(TRUE);
}

void UmlWindow::toolMenuAboutToShow() {
  abort_line_construction();
  
  bool cpp = GenerationSettings::cpp_get_default_defs();
  bool java = GenerationSettings::java_get_default_defs();
  bool php = GenerationSettings::php_get_default_defs();
  bool python = GenerationSettings::python_get_default_defs();
  bool idl = GenerationSettings::idl_get_default_defs();
  bool lang_except_idl = cpp || java || php || python;
  
  toolMenu->clear();
  toolMenu->insertItem(TR("Show &Trace Window"), this, SLOT(show_trace()));
  if (browser->get_project() != 0) {
    if (lang_except_idl | idl) {
      toolMenu->insertSeparator();
      if (cpp)
	toolMenu->insertItem(TR("Generate C++"), this, SLOT(cpp_generate()), ::Qt::CTRL+::Qt::Key_G);
      if (java)
	toolMenu->insertItem(TR("Generate Java"), this, SLOT(java_generate()), ::Qt::CTRL+::Qt::Key_J);
      if (php)
	toolMenu->insertItem(TR("Generate Php"), this, SLOT(php_generate()), ::Qt::CTRL+::Qt::Key_H);
      if (python)
	toolMenu->insertItem(TR("Generate Python"), this, SLOT(python_generate()), ::Qt::CTRL+::Qt::Key_Y);
      if (idl)
	toolMenu->insertItem(TR("Generate Idl"), this, SLOT(idl_generate()), ::Qt::CTRL+::Qt::Key_I);
    }
    if (!BrowserNode::edition_active()) {
      if (lang_except_idl) {
	toolMenu->insertSeparator();
	if (cpp) {
	  toolMenu->insertItem(TR("Reverse C++"), this, SLOT(cpp_reverse()));
	  toolMenu->insertItem(TR("Roundtrip C++"), this, SLOT(cpp_roundtrip()));
	}
	if (java) {
	  toolMenu->insertItem(TR("Reverse Java"), this, SLOT(java_reverse()));
	  toolMenu->insertItem(TR("Roundtrip Java"), this, SLOT(java_roundtrip()));
	}
	if (php)
	  toolMenu->insertItem(TR("Reverse Php"), this, SLOT(php_reverse()));

#warning reverse python

#if 0
	if (python)
	  toolMenu->insertItem(TR("Reverse Python"), this, SLOT(python_reverse()));
#endif
	if (java) {
	  toolMenu->insertSeparator();
	  toolMenu->insertItem(TR("Java Catalog"), this, SLOT(java_catalog()));
	}
	if (preserve_bodies()) {
	  toolMenu->insertSeparator();
	  if (cpp)
	    toolMenu->insertItem(TR("Roundtrip C++ bodies"), this, SLOT(cpp_roundtripbody()));
	  if (java)
	    toolMenu->insertItem(TR("Roundtrip Java bodies"), this, SLOT(java_roundtripbody()));
	  if (php)
	    toolMenu->insertItem(TR("Roundtrip Php bodies"), this, SLOT(php_roundtripbody()));
	  if (python)
	    toolMenu->insertItem(TR("Roundtrip Python bodies"), this, SLOT(python_roundtripbody()));
	}
      }
      if (BrowserClass::find("UmlBaseItem") != 0) {
	toolMenu->insertSeparator();
	toolMenu->insertItem(TR("Plug-out upgrade"), this, SLOT(plug_out_upgrade()));
      }
      Tool::menu_insert(toolMenu, UmlProject, this, SLOT(run_tool(int)));
    }
    if (browser->get_project()->is_writable()) {
      toolMenu->insertSeparator();
      toolMenu->insertItem(TR("Tools settings"), this, SLOT(tool_settings()));
      toolMenu->insertItem(TR("Import settings"), this, SLOT(import_tool_settings()));
    }
  }
}
  
void UmlWindow::set_commented(BrowserNode * bn)
{
  if (the != 0) {
    // not on exit
    UmlWindow * him = the;
    bool same = (him->commented == bn);
    
    the = 0;	// to do nothing in comment_changed() which is called
    
    him->commented = bn;
    
    if (bn != 0) {
      him->comment->setText(bn->get_comment());
      him->comment->setReadOnly(!bn->is_writable());
      him->statusBar()->message(bn->get_data()->definition(TRUE, TRUE));
      if (! same)
	him->is_selected(bn);
    }
    else {
      him->comment->clear();
      him->comment->setReadOnly(TRUE);
      him->statusBar()->clear();
    }
    the = him;
  }
}

void UmlWindow::update_comment_if_needed(BrowserNode * bn)
{
  if ((the != 0) && (the->commented == bn))
    set_commented(bn);
}

void UmlWindow::comment_changed()
{
  if ((the != 0) && (the->commented != 0) &&
      (the->commented->get_comment() != the->comment->text())) {
    the->commented->set_comment(the->comment->text());
    the->commented->modified();
    the->commented->package_modified();
  }
}

void UmlWindow::set_message(const QString & m)
{
  the->statusBar()->message(m);
}

QWorkspace * UmlWindow::get_workspace()
{
  return the->ws;
}

CanvasFormat UmlWindow::default_format()
{
  return the->format;
}

QString UmlWindow::images_root_dir()
{
  return the->img_root_dir;
}

void UmlWindow::set_images_root_dir(QString s)
{
  the->img_root_dir = s;
}

void UmlWindow::newProject() {
  abort_line_construction();
  if (!BrowserNode::edition_active()) {
    close();
    
    if (browser->get_project() == 0) {
      QString f = QFileDialog::getSaveFileName(last_used_directory(), "*", this,
					       0, TR("Enter a folder name, this folder will be created and will name the new project"));
      
      if (!f.isEmpty()) {
	set_last_used_directory(f);
	
	QDir di;
	
	if (di.mkdir(f)) {
	  di.cd(f);
	  browser->set_project(di);
	  Tool::defaults();
	  browser->get_project()->BrowserPackage::save_all(FALSE);
	  
	  msg_warning(TR("New project"),
		      TR("Do not forget to set the target languages list\n"
		      "through the 'Languages' menu\n"
		      "\n"
		      "If you program in Java, the Java Catalog plug-out\n"
		      "will help you, use it !"));
	}
	else
	  msg_critical(TR("New project"), TR("Cannot create directory ") + f);
      }
    }
  }
}

void UmlWindow::newFromTemplate() {
  abort_line_construction();
  if (!BrowserNode::edition_active()) {
    close();
    load(TemplateProject, true);
  }
}

void UmlWindow::load() {
  abort_line_construction();
  if (!BrowserNode::edition_active()) {
    close();
    
    if (browser->get_project() == 0) {
      QString fn = QFileDialog::getOpenFileName(last_used_directory(), "*.prj", this);
      
      if (!fn.isEmpty()) {
	set_last_used_directory(fn);
	load(fn);
      }
      else
	statusBar()->message(TR("Loading aborted"), 2000);
    }
  }
}

void UmlWindow::historic_add(QString fn)
{
  QStringList::Iterator it = the->historic.find(fn);
  
  if (it != the->historic.end())
    the->historic.remove(it);
  
  the->historic.prepend(fn);
  
  // note : QFile fp(QDir::home().absFilePath(".bouml")) doesn't work
  // if the path contains non latin1 characters, for instance cyrillic !
  QString s = homeDir().absFilePath(".bouml");
  FILE * fp = fopen((const char *) s, "w");
  
  if (fp != 0) {
    int rank;

    for (it = the->historic.begin(), rank = 0;
	 (it != the->historic.end()) && (rank != 10);
	 ++it, rank += 1) {
      (void) fputs((const char *) *it, fp);
      fputc('\n', fp);
    }
    
    fprintf(fp, "compteur d utilisation %d\n", Counter);
    
    fclose(fp);
  }
}

void UmlWindow::load_it()
{
  the->load();
}

void UmlWindow::load_it(QString fn)
{
  // plug-out
  close_it();
  the->load(fn);
}

void UmlWindow::reload_it()
{
  close_it();
  the->load(the->historic.first());
}

void UmlWindow::load(QString fn, bool forcesaveas) {
  clear_select_historic();
  
  QFileInfo fi(fn);
  
  if (!fi.exists()) {
    return;
  }
  
  QDir di(fi.dirPath(TRUE));
  QString s = fi.fileName();
  
  s.truncate(s.length() - 4);	// QFileInfo::baseName remove all after first dot
  
  if (di.dirName() != s) {
    msg_critical("Uml",
		 TR("The name of the project and the name of\n"
		 "the directory containing it must be the same\n\n") +
		 TR("Project name : ") + s + "\n" +
		 TR("Directory name : ") + di.dirName());
    close_it();
    return;
  }
   
  historic_add(fi.absFilePath());
  
  QApplication::setOverrideCursor(::Qt::waitCursor);
  set_in_import(FALSE);
  browser->set_project(di);
  BrowserNode::pre_load();
  
  unsigned format;
  
  PRE_TRY;
  try {
    format = browser->get_project()->load(TRUE);
  }
  catch (int) {
    QApplication::restoreOverrideCursor();
    close_it();
    clear_select_historic(); // in case selection are done by error
  
    return;
  }
  POST_TRY;
  
  BrowserNode::post_load(FALSE);
  idmax_add_margin();
  browser->get_project()->setOpen(TRUE);
  QApplication::restoreOverrideCursor();
  setCaption("Bouml : " + fn);
  
  if (format < 20) {
    BrowserClass::plug_out_conversion();
    
    QString new_st = GenerationSettings::new_java_enums();
    
    BrowserClass::new_java_enums(new_st);
    
    browser->get_project()->package_modified();
    msg_warning("BOUML",
		"Project conversion done.<br><br>"
		"A <i>save-as</i> is forced now to save the result "
		"in a new project, then the project will be closed");
    saveAs();
    close_it();		// write access of added items not ok
  }
  else if (format < 22) {
    QString new_st = GenerationSettings::new_java_enums();
    bool must_save_as = BrowserClass::new_java_enums(new_st);

    QString m = ("Class stereotype <i>enum_pattern</i> added.<br><br>"
		 "The classes having the stereotype <i>enum</i> and a non empty "
		 "Java definition have now the stereotype <i>enum_pattern</i>.<br><br>");

    if (!new_st.isEmpty())
      m = "The class stereotype <i>enum_pattern</i> is renamed <i>" + new_st +
	"</i>and the classes having the stereotype <i>enum_pattern</i> have now the stereotype <i>" +
	  new_st + "</i><br><br>" + m;
  
    if (must_save_as)
      m += "A <i>save-as</i> is forced now to save the result in a new project";
    
    msg_warning("Bouml", m);
    
    if (must_save_as || forcesaveas) {
      if (! saveas_it())
	close_it();
    }
    else
      browser->get_project()->package_modified();
  }
  else if (forcesaveas) {
    if (! saveas_it())
      close_it();
  }
  else if ((user_id() != 0) && (my_baseName(fi) == "empty")) {
    set_user_id(-1);
    saveAs();
  }
  else {
    BrowserView::remove_temporary_files();
    read_session();
  }
  
  clear_select_historic(); // in case selection are done by error
}

void UmlWindow::save() {
  abort_line_construction();
  if (browser->get_project()) {
    if (!BrowserNode::edition_active()) {
      // hide the window else the user see that 
      // all the not-open diagram window will be open
      // to save their content
      QApplication::setOverrideCursor(::Qt::waitCursor);
      ws->hide();
      BrowserPackage::save_all(TRUE);
      ws->show();
      QApplication::restoreOverrideCursor();
    }
    else
      msg_warning("Bouml",
		  TR("Saving can't be done while a dialog is opened"));
  }
}

void UmlWindow::save_it()
{
  the->save();
}

void UmlWindow::saveAs() {
  abort_line_construction();
  saveas_it();
}

bool UmlWindow::saveas_it()
{
  if (the->browser->get_project() && !BrowserNode::edition_active()) {
    for (;;) {
      QString f = QFileDialog::getSaveFileName(last_used_directory(), "*", the,
					       0, TR("Enter a folder name, this folder will be created and will name the new project"));
      
      if (!f.isEmpty()) {
	set_last_used_directory(f);
	
	QDir d(f);
	
	if (d.dirName() == "empty")
	  msg_critical("Error", TR("'empty' is reserved to the empty plug-out"));
	else {
	  QDir di;
	  
	  while (!di.mkdir(f)) {
	    if (msg_critical("Error", TR("Cannot create directory\n") + f,
			     QMessageBox::Retry, QMessageBox::Abort)
		!= QMessageBox::Retry) {
	      if (!strcmp(the->browser->get_project()->get_name(), "empty"))
		exit(0);
	      return FALSE;
	    }
	  }
	  
	  QApplication::setOverrideCursor(::Qt::waitCursor);
	  di.cd(f);
	  the->ws->hide();
	  if (the->browser->save_as(di))
	    BrowserPackage::save_all(FALSE);
	  the->ws->show();
	  QApplication::restoreOverrideCursor();
	  the->setCaption("Bouml : " + f);
	  return TRUE;
	}
      }
      else if (!strcmp(the->browser->get_project()->get_name(), "empty"))
	exit(0);
      else
	return FALSE;
    }
  }
  
  return FALSE;
}


bool UmlWindow::can_close() {
  if (browser->get_project()) {
    if (BrowserPackage::must_be_saved()) {
      switch (msg_warning("Bouml",
			  TR("The project is modified, save it ?\n"),
			  QMessageBox::Yes, QMessageBox::No,
			  QMessageBox::Cancel)) {
      case QMessageBox::Yes:
	ws->hide();
	BrowserPackage::save_all(TRUE);
	ws->show();
	break;
      case QMessageBox::Cancel:
	statusBar()->message(TR("Close aborted"), 2000);
	return FALSE;
      }
    }
  }
  
  return TRUE;
}

void UmlWindow::close() {
  abort_line_construction();
  if (!BrowserNode::edition_active() && can_close())
    close_it();
}

void UmlWindow::do_close()
{
  if (the)
    the->close();
}

void UmlWindow::closeEvent(QCloseEvent *) {
  quit();
#if 0
  if (can_close()) {
    save_session();
    e->accept();
  }
#endif
}

void UmlWindow::close_it()
{
  if (user_id() != 0) {
    clear_select_historic();
    
    ToolCom::close_all();
    
    the->save_session();
    
    QApplication::setOverrideCursor(::Qt::waitCursor);
    
    // close all diagram windows
    // do not hide ws, else a future diagram window opening will crash !
    the->clear();
    
    // empty the browser
    set_commented(0);
    the->browser->clear();
    
    // remove tools
    the->toolMenu->clear();
    
    QApplication::restoreOverrideCursor();
    the->setCaption("Bouml");
  }
  
  Tool::init();
  GenerationSettings::init();
  BrowserPackage::init();
  BrowserClass::init();
  BrowserState::init();
  BrowserActivity::init();
  BrowserUseCase::init();
  BrowserComponent::init();
  BrowserArtifact::init();
  BrowserDeploymentNode::init();
  DiagramView::init();
}

void UmlWindow::quit() {
  abort_line_construction();
  if (!BrowserNode::edition_active() && can_close()) {
    if (browser->get_project() != 0) {
      save_session();
      BrowserView::remove_temporary_files();
      // delete lock
      set_user_id(-1);
    }
    QApplication::exit(0);
  }
}

void UmlWindow::do_quit()
{
  if (the)
    the->quit();
}

void UmlWindow::save_session() {
  if ((browser->get_project() == 0) || (user_id() <= 0))
    return;
  
  QDir d = BrowserView::get_dir();
  char fn[32];
  
  sprintf(fn, "%d.session", user_id());
  
  QFile fp(d.absFilePath(fn));
      
  if (open_file(fp, IO_WriteOnly, TRUE) != -1) {
    QTextStream st(&fp);
    
    st.setEncoding(QTextStream::Latin1);
    
    st << "window_sizes " << width() << " " << height() << " "
       << spl1->sizes().first() << " " << spl1->sizes().last() << " "
       << spl2->sizes().first() << " " << spl2->sizes().last() << '\n';

    switch (style) {
    case 'w': st << "windows_style\n"; break;
    case 'm' : st << "motif_style\n"; break;
    case '+' : st << "motifplus_style\n"; break;
    default: break;
    }
    
    QWidgetList windows = ws->windowList();
    
    if (! windows.isEmpty()) {
      st << "diagrams\n";    
      for (unsigned i = 0; i != windows.count(); i += 1) {
	if (ws->activeWindow() == windows.at(i))
	  st << "  active";
	((DiagramWindow *) windows.at(i))->save_session(st);
      }
      st << "end\n";
    }
    
    browser->get_project()->save_session(st);
    
    if (verbose_generation())
      st << "verbose\n";       
  }
}

void UmlWindow::read_session() {
  if (user_id() <= 0)
    return;
  
  QDir d = BrowserView::get_dir();
  char fn[32];
  
  sprintf(fn, "%d.session", user_id());
  
  QFile fp(d.absFilePath(fn));
  int size;
      
  if ((size = open_file(fp, IO_ReadOnly, TRUE)) != -1) {
    char * s = new char[size + 1];
    
    if (fp.readBlock(s, size) != -1) {
      s[size] = 0;
      
      PRE_TRY;
      try {
	char * st = s;
      
	read_keyword(st, "window_sizes");
	
	int w = read_unsigned(st);
	
	resize(w, read_unsigned(st));
	{
	  extern QApplication * theApp;
	  theApp->processEvents(/*500*/);
	}
	
	QValueList<int> lsz = spl1->sizes();
  
	lsz.first() = read_unsigned(st);
	lsz.last() = read_unsigned(st);
  
	spl1->setSizes(lsz);
	
	lsz = spl2->sizes();
  
	lsz.first() = read_unsigned(st);
	lsz.last() = read_unsigned(st);
  
	spl2->setSizes(lsz);
	{
	  extern QApplication * theApp;
	  theApp->processEvents(/*500*/);
	}
	
	const char * k = read_keyword(st);
  
	if (!strcmp(k, "windows_style")) {
	  windows_style();
	  k = read_keyword(st);
	}
	else if (!strcmp(k, "motif_style")) {
	  motif_style();
	  k = read_keyword(st);
	}
	else if (!strcmp(k, "motifplus_style")) {
	  motifplus_style();
	  k = read_keyword(st);
	}

	// diagrams managed in BrowserPackage::read_session
	browser->get_project()->read_session(st, k);
	
	bool verb = FALSE;

	while (!at_end(st)) {
	  if (!strcmp(read_keyword(st), "verbose")) {
	    verb = TRUE;
	    break;
	  }
	  // keywork not managed by this (old) release
	}

	if (verb != verbose_generation())
	  toggle_verbose_generation();
      }
      catch (...) {
      }
      POST_TRY;
    }
    delete [] s;
  }
}

void UmlWindow::print_it() 
{
  the->print();
}

void UmlWindow::print() {
  abort_line_construction();
#ifndef QT_NO_PRINTER
  DiagramWindow * dw = ((DiagramWindow *) ws->activeWindow());
  if (dw) {
    QPrinter printer;

    printer.setOrientation((format >= IsoA0Landscape)
			   ? QPrinter::Landscape
			   : QPrinter::Portrait);

    static bool initialized = FALSE;
    static QPrinter::ColorMode cm = QPrinter::Color;
    static QPrinter::PageSize ps = QPrinter::A4;
    static bool fp = FALSE;

    if (initialized) {
      printer.setColorMode(cm);
      printer.setPageSize(ps);
      printer.setFullPage(fp);
    }

    if (printer.setup(this)) {
      initialized = TRUE;
      cm = printer.colorMode();
      ps = printer.pageSize();
      fp = printer.fullPage();
      
      QPopupMenu m(0);
  
      m.insertItem(new MenuTitle(TR("Choose"), m.font()),  -1);
      m.insertSeparator();
      m.insertItem(TR("Print on 1 page"), 1);
      m.insertItem(TR("Print on 4 pages"), 2);
      m.insertItem(TR("Print on 9 pages"), 3);

      int div = m.exec(QCursor::pos());
      
      if ((div >= 1) && (div <= 3)) {
	QApplication::setOverrideCursor(::Qt::waitCursor);
	dw->get_view()->print(printer, div);
	QApplication::restoreOverrideCursor();
      }
    }
  }
#endif
}

void UmlWindow::edit_gen_settings() {
  browser->get_project()->edit_gen_settings();
}

void UmlWindow::edit_stereotypes() {
  browser->get_project()->edit_stereotypes();
}

void UmlWindow::edit_class_settings() {
  browser->get_project()->edit_class_settings();
}

void UmlWindow::edit_drawing_settings() {
  browser->get_project()->edit_drawing_settings();
}

void UmlWindow::use_cpp() {
  GenerationSettings::cpp_set_default_defs(!GenerationSettings::cpp_get_default_defs());
}

void UmlWindow::use_java() {
  GenerationSettings::java_set_default_defs(!GenerationSettings::java_get_default_defs());
}

void UmlWindow::use_php() {
  GenerationSettings::php_set_default_defs(!GenerationSettings::php_get_default_defs());
}

void UmlWindow::use_python() {
  GenerationSettings::python_set_default_defs(!GenerationSettings::python_get_default_defs());
}

void UmlWindow::use_idl() {
  GenerationSettings::idl_set_default_defs(!GenerationSettings::idl_get_default_defs());
}

void UmlWindow::verbose() {
  toggle_verbose_generation();
}

void UmlWindow::preserve() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0) {
    if (!prj->is_writable())
      msg_critical("Bouml", TR("Unchanged : project is read-only"));
    else {
      toggle_preserve_bodies();
      if (! preserve_bodies())
	msg_warning("Bouml",
		    TR("Warning : <i>Preserve operations's body</i> set to false.<br><br>"
		       "If you had modified body of operation outside Bouml without "
		       "using <i>roundtrip body</i> after these modifications, you "
		       "will loose them.<br>"
		       "If needed, set <i>Preserve operations's body</i> to true, apply "
		       "<i>roundtrip body</i>, then set <i>Preserve operations's body</i> to false"));
      else if (add_operation_profile())
	toggle_add_operation_profile();
      prj->modified();
    }
  }
}

void UmlWindow::addoperationprofile() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0) {
    if (!prj->is_writable())
      msg_critical("Bouml", TR("Unchanged : project is read-only"));
    else {
      toggle_add_operation_profile();
      if (add_operation_profile() && preserve_bodies()) {
	toggle_preserve_bodies();
	msg_critical("Bouml",
		     TR("Warning : <i>Preserve operations's body</i> toggle is cleared !<br><br>"
			"Next code generations will replace operations's body<br><br>"
			"If you had modified body of operation outside Bouml without "
			"using <i>roundtrip body</i> after these modifications, you "
			"will loose them.<br>"
			"If needed, set <i>Preserve operations's body</i> to true, apply "
			"<i>roundtrip body</i>, then set <i>Preserve operations's body</i> to false"));
      }
      prj->modified();
    }
  }
}

void UmlWindow::edit_shortcuts() {
  ShortcutDialog d;
  
  d.exec();
}

void UmlWindow::edit_env() {
  EnvDialog::edit(FALSE);
  
  if (BrowserView::get_project() == 0)
    // id was set by reading boumlrc
    set_user_id(-1);
  else
    read_boumlrc();
}

void UmlWindow::edit_image_root_dir() {
  QString s = 
    QFileDialog::getExistingDirectory(img_root_dir, 0, 0,
				      TR("Select images root directory"));
  
  if (! s.isEmpty()) {
    img_root_dir = s;
    the->browser->get_project()->package_modified();
  }
}

void UmlWindow::langMenuAboutToShow() {
  abort_line_construction();
  
  langMenu->setItemChecked(use_cpp_id,
			   GenerationSettings::cpp_get_default_defs());
  langMenu->setItemChecked(use_java_id,
			   GenerationSettings::java_get_default_defs());
  langMenu->setItemChecked(use_php_id,
			   GenerationSettings::php_get_default_defs());
  langMenu->setItemChecked(use_python_id,
			   GenerationSettings::python_get_default_defs());
  langMenu->setItemChecked(use_idl_id,
			   GenerationSettings::idl_get_default_defs());
  langMenu->setItemChecked(verbose_gen_id, verbose_generation());
  
  BrowserPackage * prj = browser->get_project();
  bool enabled = (prj != 0);
  
  langMenu->setItemEnabled(use_cpp_id, enabled);
  langMenu->setItemEnabled(use_java_id, enabled);
  langMenu->setItemEnabled(use_php_id, enabled);
  langMenu->setItemEnabled(use_python_id, enabled);
  langMenu->setItemEnabled(use_idl_id, enabled);
  langMenu->setItemEnabled(verbose_gen_id, enabled);
  if (enabled) {
    langMenu->setItemChecked(preserve_bodies_id, preserve_bodies());
    langMenu->setItemChecked(add_operation_profile_id, add_operation_profile());
  }
  langMenu->setItemEnabled(preserve_bodies_id, enabled && prj->is_writable());
  langMenu->setItemEnabled(add_operation_profile_id, enabled && prj->is_writable());
}

void UmlWindow::miscMenuAboutToShow() {
  abort_line_construction();
  
  BrowserPackage * prj = browser->get_project();
  bool enabled = (prj != 0);
  
  miscMenu->setItemEnabled(shortcut_id, enabled);
  miscMenu->setItemEnabled(img_root_dir_id, 
			   enabled && browser->get_project()->is_writable());
  miscMenu->setItemEnabled(show_browser_stereotypes_id, enabled);
}

void UmlWindow::fontSizeMenuAboutToShow() {
  abort_line_construction();
  
  int ps = NormalFont.pointSize();
  BrowserNode * prj = browser->get_project();
  bool enabled = (prj != 0) && prj->is_writable();

  for (int i = FONTSIZEMIN; i < FONTSIZESUP; i += 1) {
    fontSizeMenu->setItemEnabled(i, enabled);
    fontSizeMenu->setItemChecked(i, i == ps);
    fontSizeMenu->setWhatsThis(i, fontSizeMenuText());
  }
}

void UmlWindow::setFontSize(int i) {
  if (i == NormalFont.pointSize())
    return;
  
  BrowserNode * prj = browser->get_project();
  
  if (prj != 0) {
    if (!prj->is_writable() &&
	(msg_warning("Bouml",
		     TR("Project file is read-only, new font "
			"size will not be saved, continue ?\n"),
		     QMessageBox::Yes, QMessageBox::No)
	 != QMessageBox::Yes))
      return;
    
    prj->package_modified();
    
    
    resize_font(i);
    
    // force browser redisplay
    
    bool is_open = prj->isOpen();
    
    prj->setOpen(!is_open);
    prj->setOpen(is_open);
    
    // force diagrams redisplay
    QWidgetList windows = ws->windowList();
    
    for (i = 0; i < int(windows.count()); ++i) {
      DiagramView * view = ((DiagramWindow *) windows.at(i))->get_view();
      double z = view->the_canvas()->zoom();
      
      view->set_zoom(z + 0.1);
      view->set_zoom(z);
    }
    
    DrawingSettings::modified();
  }
}

void UmlWindow::formatMenuAboutToShow() {
  abort_line_construction();
  
  int i;
  
  for (i = IsoA0; i != IsoA0Landscape; i += 1) {
    formatMenu->setItemChecked(i, i == (int) format);
    formatMenu->setWhatsThis(i, formatMenuText());
  }
  
  for (; i != CanvasFormatSup; i += 1) {
    formatLandscapeMenu->setItemChecked(i, i == (int) format);
    formatLandscapeMenu->setWhatsThis(i, formatMenuText());
  }
}

void UmlWindow::setFormat(int i) {
  BrowserNode * prj = browser->get_project();
  
  if (prj != 0) {
    if (!prj->is_writable() &&
	(msg_warning("Bouml",
		     TR("Project file is read-only, default "
			"format will not be saved, continue ?\n"),
		     QMessageBox::Yes, QMessageBox::No)
	 != QMessageBox::Yes))
      return;
    
    format = (CanvasFormat) i;
    
    prj->package_modified();
  }
}

void UmlWindow::set_default_format(CanvasFormat f)
{
  the->format = f;
}


void UmlWindow::browser_search() {
  abort_line_construction();
  if (browser->get_project() != 0) {
    if (BrowserSearchDialog::get() == 0)
      (new BrowserSearchDialog())->show();
    else
      BrowserSearchDialog::get()->raise();
  }
}

void UmlWindow::browser_search_it() {
  the->browser_search();
}

void UmlWindow::show_stereotypes() {
  miscMenu->setItemChecked(show_browser_stereotypes_id,
			   BrowserNode::toggle_show_stereotypes());
}

void UmlWindow::do_completion() {
  bool cmpltn = !completion();

  miscMenu->setItemChecked(completion_id, cmpltn);
  set_completion(cmpltn);
}

void UmlWindow::motif_style() {
#ifndef QT_NO_STYLE_MOTIF
  QApplication::setStyle(new QMotifStyle);
  style = 'm';
#endif
}

void UmlWindow::motifplus_style() {
#ifndef QT_NO_STYLE_MOTIFPLUS
  QApplication::setStyle(new QMotifPlusStyle);
  style = '+';
#endif
}

//void UmlWindow::sgi_style() {
//  QApplication::setStyle(new QSGIStyle);
//}
//
//void UmlWindow::cde_style() {
//  QApplication::setStyle(new QCDEStyle);
//}

void UmlWindow::windows_style() {
#ifndef QT_NO_STYLE_WINDOWS
  QApplication::setStyle(new QWindowsStyle);
  style = 'w';
#endif
}


void UmlWindow::show_trace() {
  TraceDialog::show_it();
}

void UmlWindow::cpp_generate() {
  BrowserPackage * prj = browser->get_project();
  bool preserve = preserve_bodies();
  
  if (prj != 0)
    ToolCom::run((verbose_generation()) 
		 ? ((preserve) ? "cpp_generator -v -p" : "cpp_generator -v")
		 : ((preserve) ? "cpp_generator -p" : "cpp_generator"),
		 prj);
}

void UmlWindow::java_generate() {
  BrowserPackage * prj = browser->get_project();
  bool preserve = preserve_bodies();
  
  if (prj != 0)
    ToolCom::run((verbose_generation()) 
		 ? ((preserve) ? "java_generator -v -p" : "java_generator -v")
		 : ((preserve) ? "java_generator -p" : "java_generator"),
		 prj);
}

void UmlWindow::php_generate() {
  BrowserPackage * prj = browser->get_project();
  bool preserve = preserve_bodies();
  
  if (prj != 0)
    ToolCom::run((verbose_generation()) 
		 ? ((preserve) ? "php_generator -v -p" : "php_generator -v")
		 : ((preserve) ? "php_generator -p" : "php_generator"),
		 prj);
}

void UmlWindow::python_generate() {
  BrowserPackage * prj = browser->get_project();
  bool preserve = preserve_bodies();
  
  if (prj != 0)
    ToolCom::run((verbose_generation()) 
		 ? ((preserve) ? "python_generator -v -p" : "python_generator -v")
		 : ((preserve) ? "python_generator -p" : "python_generator"),
		 prj);
}

void UmlWindow::idl_generate() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run((verbose_generation()) ? "idl_generator -v" : "idl_generator",
		 prj);
}

void UmlWindow::java_catalog() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run("java_catalog", prj);
}

#if 0
void UmlWindow::doc_generate() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run("doc_generator", prj);
}
#endif

void UmlWindow::cpp_reverse() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run("cpp_reverse", prj);
}

void UmlWindow::java_reverse() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run("java_reverse", prj);
}

void UmlWindow::cpp_roundtrip() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run("cpp_roundtrip", prj);
}

void UmlWindow::java_roundtrip() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run("java_roundtrip", prj);
}

void UmlWindow::php_reverse() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run("php_reverse", prj);
}

void UmlWindow::python_reverse() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run("python_reverse", prj);
}

void UmlWindow::cpp_roundtripbody() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run((verbose_generation()) ? "roundtrip_body -v c++" : "roundtrip_body c++",
		 prj);
}

void UmlWindow::java_roundtripbody() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run((verbose_generation()) ? "roundtrip_body -v java" : "roundtrip_body java",
		 prj);
}

void UmlWindow::php_roundtripbody() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run((verbose_generation()) ? "roundtrip_body -v php" : "roundtrip_body php",
		 prj);
}

void UmlWindow::python_roundtripbody() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run((verbose_generation()) ? "roundtrip_body -v python" : "roundtrip_body python",
		 prj);
}

void UmlWindow::plug_out_upgrade() {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run("plug_out_upgrade", prj);
}

void UmlWindow::run_tool(int param) {
  BrowserPackage * prj = browser->get_project();
  
  if (prj != 0)
    ToolCom::run(Tool::command(param), prj);
}

void UmlWindow::tool_settings() {
  ToolDialog dlg;
  
  if (dlg.exec()== QDialog::Accepted) {
    browser->get_project()->modified();
  }
}

void UmlWindow::import_tool_settings() {
  if (Tool::import())
    browser->get_project()->modified();
}

void UmlWindow::help() {
  if (BrowserView::get_project() == 0)
    read_boumlrc();
  
  const char * topic = 
    (browser->get_project() == 0)
      ? "starting"
      : ((BrowserView::selected_item() == 0)
	 ? "browseritems"
	 : BrowserView::selected_item()->help_topic());
  
  HelpDialog::show(topic);
  
  if (BrowserView::get_project() == 0)
    // id was set by reading boumlrc
    set_user_id(-1);
}

void UmlWindow::about() {
  AboutDialog d;
  
  d.exec();
}

void UmlWindow::aboutQt() {
  QMessageBox::aboutQt(this, "Bouml");
}

void UmlWindow::windowsMenuAboutToShow() {
  abort_line_construction();
  
  windowsMenu->clear();
  
  int cascadeId = windowsMenu->insertItem(TR("&Cascade"), ws, SLOT(cascade()));
  int tileId = windowsMenu->insertItem(TR("&Tile"), ws, SLOT(tile()));
  int preferredId = windowsMenu->insertItem(TR("&Preferred geometry"), this, SLOT(preferred_geometry()));
  int closeAllId = windowsMenu->insertItem(TR("C&lose all"), this, SLOT(close_all_windows()));
  
  QWidgetList windows = ws->windowList();
  
  if (windows.isEmpty()) {
    windowsMenu->setItemEnabled(cascadeId, FALSE);
    windowsMenu->setItemEnabled(tileId, FALSE);
    windowsMenu->setItemEnabled(preferredId, FALSE);
    windowsMenu->setItemEnabled(closeAllId, FALSE);
  }
  else {
    windowsMenu->insertSeparator();
    
    for (int i = 0; i < int(windows.count()); ++i) {
      int id = windowsMenu->insertItem(windows.at(i)->caption(),
				       this, SLOT(windowsMenuActivated(int)));
      windowsMenu->setItemParameter(id, i);
      windowsMenu->setItemChecked(id, ws->activeWindow() == windows.at(i));
    }
  }
  
  windows = dialogs();
  
  if (! windows.isEmpty()){
    windowsMenu->insertSeparator();
  
    for (int j = 0; j < int(windows.count()); ++j) {
      int id = windowsMenu->insertItem(windows.at(j)->caption(),
				       this, SLOT(dialogsMenuActivated(int)));
      windowsMenu->setItemParameter(id, j);
    }
  }
}

void UmlWindow::preferred_geometry() {
  QWidgetList l = the->ws->windowList();
  QWidget * w;
  
  for (w = l.first(); w != 0; w = l.next())
    ((DiagramWindow *) w)->get_view()->preferred_size_zoom();
}

void UmlWindow::close_all_windows() {
  QWidgetList l = the->ws->windowList();
  QWidget * w;
  
  for (w = l.first(); w != 0; w = l.next())
    ((DiagramWindow *) w)->close();
  
  l = dialogs();
  for (w = l.first(); w != 0; w = l.next())
    w->close();
}

void UmlWindow::windowsMenuActivated(int id) {
  QWidget* w = ws->windowList().at(id);
  
  if (w) {
    w->showNormal();
    w->setFocus();
  }
}

void UmlWindow::dialogsMenuActivated(int id) {
  QWidget* w = dialogs().at(id);
  
  if (w) {
    w->showNormal();
    w->raise();
  }
}

void UmlWindow::abort_line_construction() {
  QWidgetList l = the->ws->windowList();
  QWidget * w;
  
  for (w = l.first(); w != 0; w = l.next())
    ((DiagramWindow *) w)->get_view()->abort_line_construction();
}

//

void UmlWindow::keyPressEvent(QKeyEvent * e) {
  QString s = Shortcut::shortcut(e->key(), e->state());
  
  if (!s.isEmpty()) {
    e->accept();

    if (s == "Save")
      UmlWindow::save_it();
    else if (s == "Save as")
      UmlWindow::saveas_it();
    else if (s == "Browser search")
      UmlWindow::browser_search_it();
    else if (s == "Close")
      UmlWindow::do_close();
    else if (s == "Quit")
      UmlWindow::do_quit();
    else if (s == "Open project")
      UmlWindow::load_it();
  }
  else
    QMainWindow::keyPressEvent(e);
}

//

static bool OnHistoric = FALSE;
static QValueList<BrowserNode *>::Iterator HistoricIterator;

void UmlWindow::clear_select_historic()
{
  the->select_historic.clear();
  HistoricIterator = the->select_historic.end();
  
  the->prev->setPixmap(*leftUnavailablePixmap);
  the->next->setPixmap(*rightUnavailablePixmap);
}

void UmlWindow::is_selected(BrowserNode * bn) {
  if (OnHistoric)
    OnHistoric = FALSE;
  else if ((HistoricIterator == select_historic.end()) ||
	   (*HistoricIterator != bn)) {
    ++HistoricIterator;
    while (HistoricIterator != select_historic.end())
      HistoricIterator = select_historic.remove(HistoricIterator);
    
    if (select_historic.count() == 50)
      select_historic.remove(select_historic.first());
    
    select_historic.append(bn);
    HistoricIterator = select_historic.end();
    --HistoricIterator;
    
    prev->setPixmap((HistoricIterator != select_historic.begin())
		    ? *leftPixmap : *leftUnavailablePixmap);
    next->setPixmap(*rightUnavailablePixmap);
  }
}

void UmlWindow::next_select() {
  if (++HistoricIterator != select_historic.end()) {
    OnHistoric = TRUE;
    browser->select(*HistoricIterator);
    
    the->prev->setPixmap(*leftPixmap);
    ++HistoricIterator;
    the->next->setPixmap((HistoricIterator != select_historic.end())
			 ? *rightPixmap : *rightUnavailablePixmap);
  }
  
  --HistoricIterator; 
}

void UmlWindow::prev_select() {
  if (HistoricIterator != select_historic.begin()) {
    OnHistoric = TRUE;
    browser->select(*--HistoricIterator);
    
    the->prev->setPixmap((HistoricIterator != select_historic.begin())
			 ? *leftPixmap : *leftUnavailablePixmap);
    the->next->setPixmap(*rightPixmap);  
  }
}

void UmlWindow::historic_forget(BrowserNode * bn) {
  the->select_historic.remove(bn);
}
