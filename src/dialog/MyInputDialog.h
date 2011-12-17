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

#ifndef MYINPUTDIALOG_H
#define MYINPUTDIALOG_H

#include "qdialog.h"

// something like QInputDialog, I do not use this last because
// there is no way to place it near the mouse : it is always placed
// on the screen center

class QComboBox;
class QValidator;

class LineEdit;

class MyInputDialog : public QDialog {
  Q_OBJECT
    
  protected:
    LineEdit * le;
    QComboBox * cb;
    
    static QSize previous_size;
      
    MyInputDialog(const char * title, const QString & msg,
		  const QString & init);
    MyInputDialog(const char * title, const QString & msg,
		  const QStringList & l, const QString & init, bool existing);
    virtual ~MyInputDialog();
    
  public:
    static QString getText(const char * title, const QString & msg,
			   const QString & init, BooL & ok,
			   const QValidator * v = 0);
    static QString getText(const char * title, const QString & msg,
			   const QStringList & l, const QString & init,
			   bool existing, BooL & ok);
    
  protected slots:
    virtual void polish();
    virtual void accept();
};

#endif
