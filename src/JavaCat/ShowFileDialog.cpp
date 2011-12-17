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





#include <qmultilineedit.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qapplication.h>
#include <qdir.h>
 
#include "ShowFileDialog.h"
 
ShowFileDialog::ShowFileDialog(const QCString & filename)
    : QDialog(0, filename) {
  QVBoxLayout * vbox = new QVBoxLayout(this);
 
  vbox->addWidget(new QLabel("You can specify the editor through the environment dialog",
                             this));
 
  e = new QMultiLineEdit(this);
  
  QFile f(filename);
  
  if (f.open(IO_ReadOnly)) {
    QTextStream t(&f);
    
    e->setText(t.read());
  }
 
  QFont font = e->font();
 
  font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  e->setFont(font);
 
  vbox->addWidget(e);
}

void ShowFileDialog::polish() {
  QDialog::polish();
  
  int w = QApplication::desktop()->width();
  int h = QApplication::desktop()->height();
  int cx = w/2;
  int cy = h/2;
  bool virtual_desktop = FALSE;
  
  // try to read .boumlrc
  // note : QFile fp(QDir::home().absFilePath(".boumlrc")) doesn't work
  // if the path contains non latin1 characters, for instance cyrillic !
  QString s = QDir::home().absFilePath(".boumlrc");
  FILE * fp = fopen((const char *) s, "r");













  
  if (fp != 0) {
    char line[512];
      
    while (fgets(line, sizeof(line) - 1, fp) != 0) {
      if (!strncmp(line, "DESKTOP ", 8)) {
	int l, t, r, b;
	
	if (sscanf(line+8, "%d %d %d %d", &l, &t, &r, &b) == 4) {
	  if (!((r == 0) && (t == 0) && (r == 0) && (b == 0)) &&
	      !((r < 0) || (t < 0) || (r < 0) || (b < 0)) &&
	      !((r <= l) || (b <= t))) {
	    w = r - l + 1;
	    h = b - t + 1;
	    cx = (r + l) /2;
	    cy = (t + b) / 2;
	    virtual_desktop = TRUE;
	  }
	}
	
	break;
      }
    }
    
    fclose(fp);
  }
  
  resize(w/2, h/2);
  
  if (virtual_desktop) 
    move(x() + cx - (x() + width() / 2), 
	 y() + cy - (y() + height() / 2));
}

ShowFileDialog::~ShowFileDialog() {
} 
