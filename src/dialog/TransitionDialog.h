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

#ifndef TRANSITIONDIALOG_H
#define TRANSITIONDIALOG_H

#include <qwidgetlist.h>
#include <qstringlist.h>
#include <qtabdialog.h>

#include "BrowserNode.h"

class QPushButton;
class QComboBox;
class QCheckBox;

class LineEdit;
class MultiLineEdit;
class TransitionData;
class BrowserNode;
class KeyValuesTable;
struct TransDef;
class BodyDialog;

struct TransDialog {
  MultiLineEdit * edtrigger;
  MultiLineEdit * edguard;
  MultiLineEdit * edexpr;
  
  void accept(TransDef &);
};

class TransitionDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    bool visit;
    QWidget * umltab;
    TransitionData * rel;
    LineEdit * edname;
    QCheckBox * internal_cb;
    QComboBox * edstereotype;
    MultiLineEdit * comment;
    QList<BodyDialog> edits;
    
    QWidget * ocltab;
    QWidget * cpptab;
    QWidget * javatab;

    TransDialog uml;
    TransDialog cpp;
    TransDialog java;
        
    // User
    KeyValuesTable * kvtable;
    
    static QSize previous_size;
  
    static void post_edit_description(TransitionDialog * d, QString s);
    static void post_edit_uml_trigger(TransitionDialog * d, QString s);
    static void post_edit_uml_guard(TransitionDialog * d, QString s);
    static void post_edit_uml_expr(TransitionDialog * d, QString s);
    static void post_edit_cpp_trigger(TransitionDialog * d, QString s);
    static void post_edit_cpp_guard(TransitionDialog * d, QString s);
    static void post_edit_cpp_expr(TransitionDialog * d, QString s);
    static void post_edit_java_trigger(TransitionDialog * d, QString s);
    static void post_edit_java_guard(TransitionDialog * d, QString s);
    static void post_edit_java_expr(TransitionDialog * d, QString s);
  
  public:
    TransitionDialog(TransitionData * r);
    virtual ~TransitionDialog();
  
  protected:
    void init_tab(QWidget *&, TransDialog &, TransDef & td, const char * lbl,
		  const char * sl_trigger, const char * sl_guard,
		  const char * sl_expr, bool enabled);
  protected slots:
    virtual void polish();
    virtual void accept();
  
    void edit_description();
    void edit_uml_trigger();
    void edit_uml_guard();
    void edit_uml_expr();
    void edit_cpp_trigger();
    void edit_cpp_guard();
    void edit_cpp_expr();
    void edit_java_trigger();
    void edit_java_guard();
    void edit_java_expr();

    void change_tabs(QWidget *);
};

#endif
