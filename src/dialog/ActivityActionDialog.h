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

#ifndef ACTIVITYACTIONDIALOG_H
#define ACTIVITYACTIONDIALOG_H

#include <qwidgetlist.h>
#include <qstringlist.h>
#include <qtabdialog.h>

#include "BrowserNode.h"
#include "BodyDialog.h"
#include "ActivityActionData.h"

class QComboBox;
class QGroupBox;
class QGrid;
class QCheckBox;

class ActivityActionData;
class LineEdit;
class MultiLineEdit;
class KeyValuesTable;

struct ActionCondDialog {
  MultiLineEdit * edpre;
  MultiLineEdit * edpost;
  
  void init(QGrid *, ActivityActionData *, DrawingLanguage, bool visit);
  void get(QString & pre, QString & post) const;
  void set(QString pre, QString post);
};

class AnyActionDialog {
  protected:
    QTabDialog * td;
    QGrid * ocl_grid;
    QGrid * cpp_grid;
    QGrid * java_grid;
    ActionCondDialog ocl_cond;
    ActionCondDialog cpp_cond;
    ActionCondDialog java_cond;

  public:
    AnyActionDialog();
    void init(QTabDialog *, ActivityActionData *, void * d, bool visit);
    void init_cpp(QTabDialog *, ActivityActionData *, void * d, bool visit);
    void init_java(QTabDialog *, ActivityActionData *, void * d, bool visit);
    bool update(void *);
    void goes(AnyActionDialog & previous);
    void get_cond(QString & ocl_pre, QString & ocl_post, 
		  QString & cpp_pre, QString & cpp_post, 
		  QString & java_pre, QString & java_post) const;
};

class OpaqueDialog : public QObject, public AnyActionDialog {
  Q_OBJECT

  private:
    QList<BodyDialog> * edits;
    MultiLineEdit * ocl_beh;
    MultiLineEdit * cpp_beh;
    MultiLineEdit * java_beh;

  public:
    void init(QTabDialog *, ActivityActionData *, OpaqueAction *,
	      QList<BodyDialog> & e, bool visit);
    bool update(OpaqueAction *);

    static void post_edit_ocl(ActivityActionDialog * d, QString s);
    static void post_edit_cpp(ActivityActionDialog * d, QString s);
    static void post_edit_java(ActivityActionDialog * d, QString s);

  protected slots:
    void edit_ocl();
    void edit_cpp();
    void edit_java();
};

class AcceptEventDialog : public AnyActionDialog {
  private:
    QCheckBox * unmarshall_cb;
    QCheckBox * timeevent_cb;
    LineEdit * uml_trigger;
    LineEdit * cpp_trigger;
    LineEdit * java_trigger;

  public:
    void init(QTabDialog *, ActivityActionData *, AcceptEventAction *, bool visit);
    bool update(AcceptEventAction *);
};

class AccessVariableValueDialog : public QObject, public AnyActionDialog {
  Q_OBJECT

  private:
    BrowserNodeList * classes;
    QStringList * class_names;
    BrowserNodeList vars;	// attribute or relation
    QStringList var_names;
    QCheckBox * flag_cb;
    QComboBox * class_co;
    QComboBox * var_co;
    bool visit;

    void insert_vars(BrowserClass * c);
    void set(BrowserNode * bn);

  public:
    void init(QTabDialog *, ActivityActionData *,
	      AccessVariableValueAction *, BrowserNodeList & nodes,
	      QStringList &, bool visit);
    bool update(AccessVariableValueAction *);

  protected slots:
    void classChanged(int);
    void menu_var();
};

class ClearVariableValueDialog : public AccessVariableValueDialog {
};

class ReadVariableValueDialog : public AccessVariableValueDialog {
};

class WriteVariableValueDialog : public AccessVariableValueDialog {
};

class ChangeVariableValueDialog : public AccessVariableValueDialog {
  private:
    QCheckBox * flag_cb;

  public:
    void init(QTabDialog *, ActivityActionData *,
	      ChangeVariableValueAction *, const char * flg_name, 
	      BrowserNodeList & nodes, QStringList &, bool visit);
    bool update(ChangeVariableValueAction *);
};

class AddVariableValueDialog : public ChangeVariableValueDialog {
  public:
    void init(QTabDialog *, ActivityActionData *, AddVariableValueAction *,
	      BrowserNodeList & nodes, QStringList &, bool visit);
};

class RemoveVariableValueDialog : public ChangeVariableValueDialog {
  public:
    void init(QTabDialog *, ActivityActionData *, RemoveVariableValueAction *,
	      BrowserNodeList & nodes, QStringList &, bool visit);
};

class WithBehaviorDialog : public AnyActionDialog {
  protected:
    BrowserNodeList * nodes;
    QStringList * node_names;
    QComboBox * behavior_co;
    BrowserNode * view;
    bool visit;
    
    void menu_behavior();

  public:
    void init(BrowserNode * beh);
};

class CallBehaviorDialog : public QObject, public WithBehaviorDialog {
  Q_OBJECT
    
  private:
    QCheckBox * synchronous_cb;

  public:
    void init(QTabDialog *, ActivityActionData *, CallBehaviorAction *, 
	      BrowserNodeList &, QStringList &, BrowserNode * v, bool visit);
    bool update(CallBehaviorAction *);
    
  public slots:
    void menu_beh();
};

class CallOperationDialog :  public QObject, public AnyActionDialog {
  Q_OBJECT

  private:
    BrowserNodeList * classes;
    QStringList * class_names;
    BrowserNodeList opers;	// attribute or relation
    QStringList oper_names;
    QCheckBox * synchronous_cb;
    QComboBox * class_co;
    QComboBox * oper_co;
    bool visit;

    void insert_opers(BrowserClass * c);
    void set(BrowserNode * bn);

  public:
    void init(QTabDialog *, ActivityActionData *, CallOperationAction *, 
	      BrowserNodeList &, QStringList &, bool visit);
    bool update(CallOperationAction *);

  protected slots:
    void classChanged(int);
    void menu_oper();
};

class SendObjectDialog : public AnyActionDialog {
};

class SendSignalDialog : public AnyActionDialog {
  private:
    LineEdit * ocl_signal;
    LineEdit * cpp_signal;
    LineEdit * java_signal;

  public:
    void init(QTabDialog *, ActivityActionData *, SendSignalAction *, bool visit);
    bool update(SendSignalAction *);
};

class BroadcastSignalDialog : public SendSignalDialog {
};

class UnmarshallDialog : public AnyActionDialog {
};

class ValueSpecificationDialog : public QObject, public AnyActionDialog {
  Q_OBJECT

  private:
    QList<BodyDialog> * edits;
    MultiLineEdit * ocl_val;
    MultiLineEdit * cpp_val;
    MultiLineEdit * java_val;

  public:
    void init(QTabDialog *, ActivityActionData *,
	      ValueSpecificationAction *, QList<BodyDialog> & e, 
	      bool visit);
    bool update(ValueSpecificationAction *);

    static void post_edit_ocl(ActivityActionDialog * d, QString s);
    static void post_edit_cpp(ActivityActionDialog * d, QString s);
    static void post_edit_java(ActivityActionDialog * d, QString s);

  protected slots:
    void edit_ocl();
    void edit_cpp();
    void edit_java();
};

class AcceptCallDialog : public AnyActionDialog {
  private:
    LineEdit * uml_trigger;
    LineEdit * cpp_trigger;
    LineEdit * java_trigger;

  public:
    void init(QTabDialog *, ActivityActionData *, AcceptCallAction *, bool visit);
    bool update(AcceptCallAction *);
};

class ReplyDialog : public AnyActionDialog {
  private:
    LineEdit * uml_trigger;
    LineEdit * cpp_trigger;
    LineEdit * java_trigger;

  public:
    void init(QTabDialog *, ActivityActionData *, ReplyAction *, bool visit);
    bool update(ReplyAction *);
};

class CreateObjectDialog : public AnyActionDialog {
  private:
    LineEdit * classifier;

  public:
    void init(QTabDialog *, ActivityActionData *, CreateObjectAction *, bool visit);
    bool update(CreateObjectAction *);
};

class DestroyObjectDialog : public AnyActionDialog {
  private:
    QCheckBox * is_destroy_links_cb;
    QCheckBox * is_destroy_owned_objects_cb;

  public:
    void init(QTabDialog *, ActivityActionData *, DestroyObjectAction *, bool visit);
    bool update(DestroyObjectAction *);
};

class TestIdentityDialog : public AnyActionDialog {
  public:
    void init(QTabDialog *, ActivityActionData *, TestIdentityAction *, bool visit);
    bool update(TestIdentityAction *);
};

class RaiseExceptionDialog : public AnyActionDialog {
  public:
    void init(QTabDialog *, ActivityActionData *, RaiseExceptionAction *, bool visit);
    bool update(RaiseExceptionAction *);
};

class ReduceDialog : public QObject, public WithBehaviorDialog {
  Q_OBJECT
    
  private:
    QCheckBox * is_ordered_cb;

  public:
    void init(QTabDialog *, ActivityActionData *, ReduceAction *, 
	      BrowserNodeList &, QStringList &, BrowserNode * v, bool visit);
    bool update(ReduceAction *);
    
  public slots:
    void menu_beh();
};

class ActivityActionDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    // Uml
    bool visit;
    QWidget * umltab;
    ActivityActionData * act;
    LineEdit * edname;
    QComboBox * edstereotype;
    QComboBox * edtype;
    UmlActionKind current_type;
    MultiLineEdit * comment;
    MultiLineEdit * constraint;
    BrowserNodeList classes;
    QStringList class_names;
    BrowserNodeList behaviors;
    QStringList behavior_names;
    QList<BodyDialog> edits;
        
    OpaqueDialog opaque;
    AcceptEventDialog acceptevent;
    ReadVariableValueDialog readvariablevalue;
    ClearVariableValueDialog clearvariablevalue;
    WriteVariableValueDialog writevariablevalue;
    AddVariableValueDialog addvariablevalue;
    RemoveVariableValueDialog removevariablevalue;
    CallBehaviorDialog callbehavior;
    CallOperationDialog calloperation;
    SendObjectDialog sendobject;
    SendSignalDialog sendsignal;
    BroadcastSignalDialog broadcastsignal;
    UnmarshallDialog unmarshall;
    ValueSpecificationDialog valuespecification;
    AcceptCallDialog acceptcall;
    ReplyDialog reply;
    CreateObjectDialog createobject;
    DestroyObjectDialog destroyobject;
    TestIdentityDialog testidentity;
    RaiseExceptionDialog raiseexception;
    ReduceDialog reduce;

    // User
    KeyValuesTable * kvtable;
        
    static QSize previous_size;
  
  protected:
    static void post_edit_description(ActivityActionDialog * d, QString s);
    static void post_edit_constraint(ActivityActionDialog * d, QString s);
    AnyActionDialog & get_dialog(int);
  
  public:
    ActivityActionDialog(ActivityActionData * r);
    virtual ~ActivityActionDialog();

    OpaqueDialog & opaque_dialog() { return opaque; }
    ValueSpecificationDialog & valuespecification_dialog() { return valuespecification; }
  
  protected slots:
    virtual void polish();
    virtual void accept();
  
    void edTypeActivated(int);  
    void edit_description();
    void edit_constraint();

    void change_tabs(QWidget *);
};

#endif
