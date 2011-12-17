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

#include <stdlib.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qsplitter.h>
#include <qwindowsstyle.h> 
#include <qmotifstyle.h> 
#include <qmotifplusstyle.h> 
#include <qwhatsthis.h>
#include <qfiledialog.h> 
#include <qfile.h>
#include <qdir.h>
#include <qdatastream.h> 
#include <qtextstream.h>

#include "JavaCatWindow.h"
#include "BrowserView.h"
#include "Package.h"
#include "BrowserSearchDialog.h"
#include "CommentView.h"
#include "UmlCom.h"
#include "Class.h"

#include "filesave.xpm"
#include "fileopen.xpm"
#include "browsersearch.xpm"
#include "scan.xpm"
#include "left.xpm"
#include "right.xpm"

JavaCatWindow * JavaCatWindow::the;

const char * OpenText = "";
const char * SaveText = "";
const char * ScanText =
  "To add the <i>java classes</i> defined under the choosen directory.";
const char * SearchText = "To search an item in the <i>browser</i>.";
const char * UpText = "To select the <i>browser</i> current item's parent.";
const char * LeftText = "Go to previous selected <i>class</i>";
const char * RightText = "Go to next selected <i>class</i>";

JavaCatWindow::JavaCatWindow() : QMainWindow(0, "Java Catalog", WDestructiveClose) {
  the = this;
  setCaption("Java Catalog");
  commented = 0;
  
  QPopupMenu * menu;
  QPixmap pixmap;
  
  QToolBar * tools = new QToolBar(this, "operations");
  
  addToolBar(tools, "Operations", Top, TRUE);
  
  menu = new QPopupMenu(this);
  menuBar()->insertItem("&File", menu);
  
  pixmap = QPixmap(fileopen);
  QWhatsThis::add(new QToolButton(pixmap, "Open", QString::null,
				  this, SLOT(load()), tools, "open"),
		  OpenText);
  menu->setWhatsThis(menu->insertItem(pixmap, "&Open", this,
				      SLOT(load()), CTRL+Key_O),
		     OpenText);
  
  pixmap = QPixmap(filesave);
  QWhatsThis::add(new QToolButton(pixmap, "Save", QString::null,
				  this, SLOT(save()), tools, "save"),
		  SaveText);
  menu->setWhatsThis(menu->insertItem(pixmap, "&Save", this,
				      SLOT(save()), CTRL+Key_S),
		     SaveText);
  
  menu->insertSeparator();
  pixmap = QPixmap(::scan);
  QWhatsThis::add(new QToolButton(pixmap, "Scan", QString::null,
				  this, SLOT(scan()), tools, "scan"),
		  ScanText);
  menu->setWhatsThis(menu->insertItem(pixmap, "S&can", this,
				      SLOT(scan()), CTRL+Key_C),
		     ScanText);
  
  menu->insertSeparator();
  menu->insertItem("&Quit", this, SLOT(quit()), CTRL+Key_Q);
  
  menu = new QPopupMenu(this);
  menuBar()->insertItem("&Browse", menu);
  
  pixmap = QPixmap(browsersearch);
  QWhatsThis::add(new QToolButton(pixmap, "Search", QString::null,
				  this, SLOT(browser_search()), tools, "search"),
		  SearchText);
  menu->setWhatsThis(menu->insertItem(pixmap, "&Search", this,
				      SLOT(browser_search()), CTRL+Key_S),
		     SearchText);
  
  pixmap = QPixmap(left);
  QWhatsThis::add(new QToolButton(pixmap, "Back", QString::null,
				  this, SLOT(historic_back()),
				  tools, "back"),
		  LeftText);
  
  pixmap = QPixmap(right);
  QWhatsThis::add(new QToolButton(pixmap, "Forward", QString::null,
				  this, SLOT(historic_forward()),
				  tools, "forward"),
		  RightText);
  
  (void)QWhatsThis::whatsThisButton(tools);

  //
  
  menu = new QPopupMenu(this);
  menuBar()->insertItem("&Style", menu);
  
#if !defined(QT_NO_STYLE_MOTIF)
  menu->insertItem("Motif", this, SLOT(motif_style()));
#endif
#if !defined(QT_NO_STYLE_MOTIFPLUS)
  menu->insertItem("MotifPlus", this, SLOT(motifplus_style()));
#endif
  menu->insertItem("Windows", this, SLOT(windows_style()));
  
  //
  
  menuBar()->insertSeparator();
  menu = new QPopupMenu(this);
  menuBar()->insertItem("&Help", menu);
  
  menu->insertItem("&About", this, SLOT(about()), Key_F1);
  menu->insertItem("About&Qt", this, SLOT(aboutQt()));
  menu->insertSeparator();
  menu->insertItem("What's This", this, SLOT(whatsThis()), SHIFT+Key_F1);
    
  //
  
  spl = new QSplitter(QSplitter::Vertical, this, "spl");
  
  browser = new BrowserView(spl);
  comment = new CommentView(spl);
  
  connect(comment, SIGNAL(refer(const QString &)),
	  browser, SLOT(refer(const QString &)));
  
  spl->moveToFirst(browser);

  QValueList<int> lsz = spl->sizes();
  int h = lsz.first() + lsz.last();
  
  lsz.first() = (h*3)/4;
  lsz.last() = h - lsz.first();
  
  spl->setSizes(lsz);
  
  spl->setResizeMode(comment, QSplitter::KeepSize);
  setCentralWidget(spl);
}

void remove_crlf(char * s)
{
  int len = strlen(s);
  
  if (len != 0) {
    if (s[len - 1] == '\n')
      s[--len] = 0;
    
    if ((len != 0) && (s[len - 1] == '\r'))
      s[len - 1] = 0;
  }
}

void JavaCatWindow::load() {
  QString here = QDir::currentDirPath();
  QString start;
  
  // note : QFile fp(QDir::home().absFilePath(".boumlcat")) doesn't work
  // if the path contains non latin1 characters, for instance cyrillic !
  QString s = QDir::home().absFilePath(".boumlcat");
  FILE * fp = fopen((const char *) s, "r");
  














  
  if (fp != 0) {
    char line[512];
 
    if (fgets(line, sizeof(line) - 1, fp) != 0) {
      remove_crlf(line);
      start = line;
    }
    
    fclose(fp);
  }

  QString path =
    QFileDialog::getOpenFileName(start, "*.cat", this);
  
  if (! path.isEmpty()) {
    QApplication::setOverrideCursor(Qt::waitCursor);
    
    QFile f(path);
    
    if (f.open(IO_ReadOnly)) {
      if ((fp = fopen((const char *) s, "w")) != 0) {
	fwrite((const char *) path, 1, path.length(), fp);
	fputc('\n', fp);
	fclose(fp);
      }
      
      QDataStream dt(&f);

      Package::get_root()->restore_children(dt);
      Package::get_root()->setOpen(TRUE);
    }
	       
    QApplication::restoreOverrideCursor();
  }
  
  QDir::setCurrent(here);
}

void JavaCatWindow::save() {
  QString path =
    QFileDialog::getSaveFileName(QString::null, "*.cat", this);
  
  if (! path.isEmpty()) {
    QApplication::setOverrideCursor(Qt::waitCursor);
    
    if (path.right(4).upper() != ".CAT")
      path += ".cat";
    
    QFile f(path);
    
    if (f.open(IO_WriteOnly)) {
      QDataStream dt(&f);
      
      Package::get_root()->backup_children(dt);
    }
	       
    QApplication::restoreOverrideCursor();
  }
}

void JavaCatWindow::scan() {
  int n;
  
  Package::scan_dir(n);
  UmlCom::message("");
}

void JavaCatWindow::browser_search() {
  BrowserSearchDialog dialog(QCursor::pos());
  
  dialog.exec();
}

void JavaCatWindow::historic_back() {
  Class::historic_back();
}

void JavaCatWindow::historic_forward() {
  Class::historic_forward();
}

void JavaCatWindow::quit() {
  QApplication::exit(0);
}

void JavaCatWindow::motif_style() {
#if !defined(QT_NO_STYLE_MOTIF)
  QApplication::setStyle(new QMotifStyle);
#endif
}

void JavaCatWindow::motifplus_style() {
#if !defined(QT_NO_STYLE_MOTIFPLUS)
  QApplication::setStyle(new QMotifPlusStyle);
#endif
}

void JavaCatWindow::windows_style() {
#ifndef QT_NO_STYLE_WINDOWS
  QApplication::setStyle(new QWindowsStyle);
#endif
}

void JavaCatWindow::about() {
  QMessageBox::about(this, "Java Catalog", "<p>release <b>2.14.1</b></p>" );
}

void JavaCatWindow::aboutQt() {
  QMessageBox::aboutQt(this, "Java Catalog");
}

void JavaCatWindow::trace(QString s)
{
  the->comment->append(s);
  the->comment->update();

  // sometimes the contains/scrollbar does not show all
  // change the split position to force correct updating
  
  QValueList<int> sz = the->spl->sizes();

  if (sz[1] & 1) {
    sz[0] += 1;
    sz[1] -= 1;
  }
  else {
    sz[0] -= 1;
    sz[1] += 1;
  }
  
  the->spl->setSizes(sz);
}

void JavaCatWindow::trace(QCString s)
{
  trace(QString(s));
}

void JavaCatWindow::clear_trace()
{
  the->comment->setText(QString::null);
}
