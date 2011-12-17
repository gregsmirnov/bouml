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

#ifndef COMPONENTDIALOG_H
#define COMPONENTDIALOG_H





#include <qptrdict.h>
#include <qmap.h>
#include <qtabdialog.h>

#include "BrowserComponent.h"

class QComboBox;
class QListBox;
class QVBox;
class LineEdit;
class MultiLineEdit;
class SimpleData;
class KeyValuesTable;
class FormalParamsTable;
class ActualParamsTable;
class BodyDialog;

class ComponentDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    QWidget * umltab;
    SimpleData * data;
    LineEdit * edname;
    QComboBox * edstereotype;
    MultiLineEdit * comment;
    QList<BodyDialog> edits;
        
    // required classes
    QVBox * rq_page;
    QComboBox * rq_stereotypefilter;
    QListBox * lb_rq_available;
    QListBox * lb_rq;
    QValueList<BrowserClass *> rqs;
        
    // provided classes
    QVBox * pr_page;
    QComboBox * pr_stereotypefilter;
    QListBox * lb_pr_available;
    QListBox * lb_pr;
    QValueList<BrowserClass *> prs;
        
    // realization classes
    QVBox * rz_page;
    QComboBox * rz_stereotypefilter;
    QListBox * lb_rz_available;
    QListBox * lb_rz;
    QValueList<BrowserClass *> rzs;
    
    // user
    KeyValuesTable * kvtable;
  
    static QSize previous_size;
    
    void init_uml_tab();
    void init_l_tab(QVBox *& page, QComboBox *& stereotypefilter,
		    void (ComponentDialog::* filteractivated)(const QString & st),
		    const char * filter_slt,
		    const char * add_slt, const char * remove_slt,
		    QListBox *& lb_available, QListBox *& lb,
		    const QValueList<BrowserClass *> & cls,
		    const char * lbl);
    
    static void post_edit_description(ComponentDialog * d, QString s);
    
  public:
    ComponentDialog(SimpleData * nd);
    virtual ~ComponentDialog();
  
  protected slots:
    virtual void polish();
    virtual void accept();
    void edit_description();
    void rq_stereotypeFilterActivated(const QString & st);
    void pr_stereotypeFilterActivated(const QString & st);
    void rz_stereotypeFilterActivated(const QString & st);
    void require_cls();
    void unrequire_cls();
    void provide_cls();
    void unprovide_cls();
    void realize_cls();
    void unrealize_cls();

    void change_tabs(QWidget *);
};

#endif
