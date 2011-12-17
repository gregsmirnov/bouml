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

#ifndef ENVDIALOG_H
#define ENVDIALOG_H

#include <qdialog.h>

class QLineEdit;
class QComboBox;

// singleton

class EnvDialog : public QDialog {
  Q_OBJECT
    
  public:
    static void edit(bool conv, bool noid = FALSE);
    
  private:
    EnvDialog(bool conv, bool noid);
  
    bool conversion;
    QLineEdit * ed_id;
    QLineEdit * ed_doc;
    QLineEdit * ed_navigator; 
    QLineEdit * ed_template;
    QLineEdit * ed_editor;
    QLineEdit * ed_lang;
    QComboBox * cb_charset;
    QLineEdit * ed_xmin;
    QLineEdit * ed_xmax;
    QLineEdit * ed_ymin;
    QLineEdit * ed_ymax;
    
  private slots:
    virtual void accept();
    virtual void reject();
    void doc_browse();
    void navigator_browse();
    void template_browse();
    void editor_browse();
    void lang_browse();
};

extern int read_boumlrc();

#endif
