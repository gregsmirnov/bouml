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

#ifndef BROWSERSEARCHDIALOG_H
#define BROWSERSEARCHDIALOG_H

#include <qdialog.h>

#include "BrowserNode.h"

class QComboBox;
class QCheckBox;
class QRadioButton;
class QPushButton;
class LineEdit;

class BrowserSearchDialog : public QDialog {
  Q_OBJECT
 
  protected:
    BrowserNodeList nodes;
    QComboBox * kind;
    QComboBox * results;
    LineEdit * ed;
    QCheckBox * case_sensitive;
    QCheckBox * even_deleted;
    QRadioButton * for_name;
    QRadioButton * for_stereotype;
    QRadioButton * for_comment;
    QRadioButton * for_decldefbody;
    QPushButton * select_b;
    QPushButton * mark_unmark_b;
    QPushButton * mark_them_b;
    QPushButton * unmark_all_b;
    
    static BrowserSearchDialog * the;
    static int saved_kind;
    static QString saved_ed;
    static bool saved_case_sensitive;
    static bool saved_even_deleted;
    static bool saved_name;
    static bool saved_stereotype;
    static bool saved_comment;
    static bool saved_decldefbody;
    
    static QSize previous_size;
        
  public:
    BrowserSearchDialog();
    virtual ~BrowserSearchDialog();

    void update();
    
    static BrowserSearchDialog * get() { return the; }
    
  protected slots:
    virtual void polish();
    void search();
    void select();
    void selected(int);
    void mark_unmark();
    void mark_them();
    void unmark_all();
};

#endif
