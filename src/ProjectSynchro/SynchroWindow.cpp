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





#include <qapplication.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qwindowsstyle.h> 
#include <qmotifstyle.h> 
#include <qmotifplusstyle.h> 
#include <qfiledialog.h> 
#include <qfile.h>
#include <qdir.h>
#include <qtextstream.h>
#include <qhbox.h>
#include <qvbox.h>

#include "SynchroWindow.h"
#include "BrowserView.h"
#include "BrowserNode.h"
#include "SynchroDialog.h"
#include "myio.h"

SynchroWindow * SynchroWindow::the;

SynchroWindow::SynchroWindow() : QMainWindow(0, "Project synchro", WDestructiveClose) {
  the = this;
  setCaption("Project synchro");
  
  QPopupMenu * menu;
  QPixmap pixmap;
  
  menu = new QPopupMenu(this);
  menuBar()->insertItem("&Project", menu);

  // open
  
#include "fileopen.xpm"
  pixmap = QPixmap(fileopen);
  menu->insertItem(pixmap, "&Open", this,
		   SLOT(load()), CTRL+Key_O);
  
  // quit
  
  menu->insertItem("&Quit", this, SLOT(quit()), CTRL+Key_Q);
  
  // synchronize
  
  menu->insertSeparator();
  menu->insertItem("&Synchonize", this, SLOT(synchronize()), CTRL+Key_S);
  
  // style
  
  menu = new QPopupMenu(this);
  menuBar()->insertItem("&Style", menu);
  
#if !defined(QT_NO_STYLE_MOTIF)
  menu->insertItem("Motif", this, SLOT(motif_style()));
#endif
#if !defined(QT_NO_STYLE_MOTIFPLUS)
  menu->insertItem("MotifPlus", this, SLOT(motifplus_style()));
#endif
  menu->insertItem("Windows", this, SLOT(windows_style()));
  
  // help & about
  
  menuBar()->insertSeparator();
  menu = new QPopupMenu(this);
  menuBar()->insertItem("&Help", menu);
  
  menu->insertItem("&About", this, SLOT(about()), Key_F1);
  menu->insertItem("About&Qt", this, SLOT(aboutQt()));
  
  //
  
  hbox = new QHBox(this);
  setCentralWidget(hbox);
}

SynchroWindow::~SynchroWindow() {
  BrowserView * browser;
  
  for (browser = browsers.first(); browser!= 0; browser = browsers.next())
    browser->close();
}

QString my_baseName(QFileInfo & fi)
{
  QString fn = fi.fileName();
  int index = fn.findRev('.');
  
  return (index == -1)
    ? fn
    : fn.left(index);
}

void SynchroWindow::load(QString path) {
  QFileInfo fi(path);
  
  if (!fi.exists()) {
    QMessageBox::critical(0, "Synchro project", 
			  path + " doesn't exist");
    return;
  }
 
  QDir dir(fi.dirPath(TRUE));
  BrowserView * browser;
  
  path = dir.canonicalPath();
  
  for (browser = browsers.first(); browser!= 0; browser = browsers.next()) {
    if (browser->get_dir().canonicalPath() == path) {
      QMessageBox::critical(0, "Project synchro",
			    "Project instance already read");
      return;
    }
  }

  if (has_backup_files(dir))
    QMessageBox::critical(0, "Synchro project", path + " contains .bak files");
  else if (! dir.mkdir("all.lock"))
    QMessageBox::critical(0, "Synchro project", 
			  (dir.exists("all.lock"))
			  ? "\
The project is already locked by 'Project control' or 'Project syncho'\n\
(the directory '" + path +"/all.lock' exists)"
			  : "Can't create directory '" + path +"/all.lock'");
  else {
    const QFileInfoList * l = dir.entryInfoList("*.lock");
    
    if (l != 0) {
      QListIterator<QFileInfo> it(*l);
      QFileInfo * pfi;
      QString ids;
      
      while ((pfi = it.current()) != 0) {
	if (pfi->isDir() && (my_baseName(*pfi) != "all"))
	  ids += " " + my_baseName(*pfi);
	++it;
      }
      
      if (! ids.isEmpty()) {
	QMessageBox::critical(0, "Synchro project", 
			      "The project " + path + " is edited by the users having these IDs :" + ids);
	dir.rmdir("all.lock");
	return;
      }
    }
    
    QVBox * vbox = new QVBox(hbox);
    BrowserView * browser = new BrowserView(vbox);
    
    QApplication::setOverrideCursor(Qt::waitCursor);
    browser->set_project(dir);
    
    bool r = browser->get_project()->load(dir);
    
    QApplication::restoreOverrideCursor();
    
    if (! r) {
      browser->close();
      delete vbox;
      // note : all.lock will be deleted by BrowserView
    }
    else {
      if (browsers.isEmpty())
	project_name = fi.fileName();
      vbox->show();
      browsers.append(browser);
    }
  }
}

void SynchroWindow::load() {
  QString path =
    QFileDialog::getOpenFileName(QString::null,
				 (project_name.isEmpty())
				 ? "*.prj"
				 : project_name,
				 this);
  
  if (! path.isEmpty()) {
    load(path);
    
    if (browsers.count() > 1)
      BrowserView::update(browsers);
  }
}

// prog removed from argc & argv
void SynchroWindow::load(int argc, char ** argv) {
  // check it is the same project
  QString name;
  int index;
  
  for (index = 0; index < argc; index += 1) {
    QFileInfo fi(argv[index]);
    
    if (!fi.exists()) {
      QMessageBox::critical(0, "Project synchro", 
			    argv[index] + QString(" doesn't exist"));
      return;
    }
    
    if (fi.extension(FALSE) != "prj") {
      QMessageBox::critical(0, "Project synchro", 
			    argv[index] + QString(" is not a project"));
      return;
    }
    
    if (name.isEmpty())
      name = my_baseName(fi);
    else if (my_baseName(fi) != name) {
      QMessageBox::critical(0, "Project synchro", "not the same project");
      return;
    }
  }
  
  // all is ok, load them
  for (index = 0; index < argc; index += 1)
    load(argv[index]);

  if (browsers.count() > 1)
    BrowserView::update(browsers);
}

void SynchroWindow::quit() {
  BrowserView * browser;
    
  for (browser = browsers.first(); browser!= 0; browser = browsers.next())
    browser->close();
  
  QApplication::exit(0);
}

void SynchroWindow::abort()
{
  the->quit();
}

void SynchroWindow::synchronize() {
  if (browsers.count() > 1) {
    SynchroDialog d(browsers);
    
    if (d.exec() == QDialog::Accepted) {
      QMessageBox::information(0, "Project synchro", "synchronization done");
      quit();
    }
  }
}

void SynchroWindow::motif_style() {
#if !defined(QT_NO_STYLE_MOTIF)
  QApplication::setStyle(new QMotifStyle);
#endif
}

void SynchroWindow::motifplus_style() {
#if !defined(QT_NO_STYLE_MOTIFPLUS)
  QApplication::setStyle(new QMotifPlusStyle);
#endif
}

void SynchroWindow::windows_style() {
#ifndef QT_NO_STYLE_WINDOWS
  QApplication::setStyle(new QWindowsStyle);
#endif
}

void SynchroWindow::about() {
  QMessageBox::about(this, "Project synchro", "<p>Version <b>1.2.4</b></p>" );
}

void SynchroWindow::aboutQt() {
  QMessageBox::aboutQt(this, "Project synchro");
}
