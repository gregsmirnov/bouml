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

// because QT is generally compiled without thread support
// I do not use a thread calling the external editor waiting
// and waiting for its end, and I use a timer which get each
// second the edit file content





#include <stdio.h>
#include <qtextstream.h> 
#include <qfile.h>
#include <qtabdialog.h>

#include "DialogTimer.h"

DialogTimer::DialogTimer(QString s, QString p, QTabDialog * d, post_edit pf)
    : QTimer(d), current(s), path(p), dlg(d), f(pf) {
  connect(this, SIGNAL(timeout()), this, SLOT(readfile()));
}

void DialogTimer::readfile() {
  FILE * fp;
  
  if ((fp = fopen((const char *) path, "rb")) != 0) {  
    QString result;
    










    QFile fi;
    
    fi.open(IO_ReadOnly, fp);
    
    QTextStream ts(&fi);
    
    ts.setEncoding(QTextStream::Latin1);
    
    result = ts.read();
    fi.close();

    fclose(fp);
    
    if (result != current) {
      current = result;
      f(dlg, result);
    }
  }
}

