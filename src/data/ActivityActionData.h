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

#ifndef ACTIVITYACTIONDATA_H
#define ACTIVITYACTIONDATA_H

#include "SimpleData.h"
#include "InfoData.h"
#include "PinParamData.h"
#include "UmlEnum.h"

class QTextStream;

class BrowserActivityAction;
class ActivityActionDialog;
class BrowserOperation;
class BrowserPin;

struct PinDescr :public PinParamData {
  const char * name;

  PinDescr() : name(0) {}
  
  virtual void do_connect(BrowserClass *);
  virtual void do_disconnect(BrowserClass *);
};

struct AnyAction {
  AnyAction();
  virtual ~AnyAction();
  virtual AnyAction * duplicate() const = 0;
  virtual UmlActionKind kind() const = 0;
  virtual QValueList<PinDescr> pins() const;	// no pins by default
  virtual bool may_add_pin() const; // true by default
  virtual QString str(DrawingLanguage lang, QString name) const;
  virtual BasicData * depend_on();
  virtual void on_delete();
  virtual BrowserNode * referenced(const char *&) const;

  virtual void save(QTextStream & st, QString & warning) const = 0;
  virtual void read(char * & st, char * & k) = 0;
  virtual void send_def(ToolCom * com, DrawingLanguage) = 0;
  virtual bool tool_cmd(ToolCom * com, const char * args);
};

struct OpaqueAction : public AnyAction {
  friend class OpaqueDialog;
    
  MyStr uml_behavior;
  MyStr cpp_behavior;
  MyStr java_behavior;

  OpaqueAction();
  virtual ~OpaqueAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QString str(DrawingLanguage lang, QString name) const;
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
  virtual bool tool_cmd(ToolCom * com, const char * args);
};

struct AcceptEventAction : public AnyAction {
  friend class AcceptEventDialog;
    
  bool unmarshall;
  bool timeevent;
  MyStr uml_trigger;
  MyStr cpp_trigger;
  MyStr java_trigger;

  AcceptEventAction();
  virtual ~AcceptEventAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
  virtual bool tool_cmd(ToolCom * com, const char * args);
};

struct AccessVariableValueAction : public AnyAction {
  friend class AccessVariableValueDialog;
    
  BrowserNode * variable;	// attribute or relation

  AccessVariableValueAction();
  virtual ~AccessVariableValueAction();
  AnyAction * duplicate(AccessVariableValueAction * r) const;
  QValueList<PinDescr> pins(UmlParamDirection, const char * str) const;
  	// [in/out] <str> : le type de la variable
  virtual BasicData * depend_on();
  virtual void on_delete();
  virtual BrowserNode * referenced(const char *&) const;
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
  virtual bool tool_cmd(ToolCom * com, const char * args);

  void set_variable(BrowserNode * v) { variable = v; }
};

struct ClearVariableValueAction : public AccessVariableValueAction {
  friend class AccessVariableValueDialog;
    
  ClearVariableValueAction();
  virtual ~ClearVariableValueAction();
  AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
};

struct ReadVariableValueAction : public AccessVariableValueAction {
  friend class AccessVariableValueDialog;
    
  ReadVariableValueAction();
  virtual ~ReadVariableValueAction();
  AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  QValueList<PinDescr> pins() const;
  	// [out] "result" : le type de la variable
};

struct WriteVariableValueAction : public AccessVariableValueAction {
  friend class AccessVariableValueDialog;
    
  WriteVariableValueAction();
  virtual ~WriteVariableValueAction();
  AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  QValueList<PinDescr> pins() const;
  	// [in] "value" : le type de la variable
};

struct ChangeVariableValueAction : public AccessVariableValueAction {
  friend class ChangeVariableValueDialog;
    
  bool flag;

  ChangeVariableValueAction();
  virtual ~ChangeVariableValueAction();
  AnyAction * duplicate(ChangeVariableValueAction * r) const;
  QValueList<PinDescr> pins(const char * str) const;
  	// [in] "value" : le type de la variable
  	// [in] <str>: UnlimitedNatural / uint
  
  void save(QTextStream & st, QString & warning, const char * str) const;
  void read(char * & st, char * & k, const char * str);
  void send_def(ToolCom * com, DrawingLanguage);
  virtual bool tool_cmd(ToolCom * com, const char * args);
};

struct AddVariableValueAction : public ChangeVariableValueAction {
  // flag is replace_all

  AddVariableValueAction();
  virtual ~AddVariableValueAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [in] "value" : le type de la variable
  	// [in] "insertAt" : UnlimitedNatural / uint
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
};

struct RemoveVariableValueAction : public ChangeVariableValueAction {
  // flag is remove_duplicates

  RemoveVariableValueAction();
  virtual ~RemoveVariableValueAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [in] "value" : le type de la variable
  	// [in] "removeAt" : UnlimitedNatural / uint
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
};

struct CallBehaviorAction : public AnyAction {
  friend class CallBehaviorDialog;
    
  bool synchronous;
  BrowserNode * behavior;	// activity or state machine
  
  CallBehaviorAction();
  virtual ~CallBehaviorAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [any]* : depend on activity's parameter
  virtual BasicData * depend_on();
  virtual void on_delete();
  virtual BrowserNode * referenced(const char *&) const;

  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
  virtual bool tool_cmd(ToolCom * com, const char * args);

  void set_behavior(BrowserNode * beh) { behavior = beh; }
};

struct CallOperationAction : public AnyAction {
  friend class CallOperationDialog;
    
  bool synchronous;
  BrowserOperation * operation;
  
  CallOperationAction();
  virtual ~CallOperationAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [in] "target" : instance,
  	// [any]* : other depend on operation's params&result&exception
  virtual QString str(DrawingLanguage lang, QString name) const;
  virtual BasicData * depend_on();
  virtual void on_delete();
  virtual BrowserNode * referenced(const char *&) const;
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
  virtual bool tool_cmd(ToolCom * com, const char * args);

  void set_operation(BrowserOperation * oper) { operation = oper; }
};

struct SendObjectAction : public AnyAction { 
  friend class SendObjectDialog;
    
  SendObjectAction();
  virtual ~SendObjectAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [in] "request" : the sent object
  	// [in] "target" : target object,
  	// [in]* : arguments (pin addable et removables)
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
};

struct SendSignalAction : public AnyAction { 
  friend class SendSignalDialog;
    
  MyStr ocl_signal;
  MyStr cpp_signal;
  MyStr java_signal;
  
  SendSignalAction();
  virtual ~SendSignalAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [in] "target"
  	// [*] depend on signal parameters
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
  virtual bool tool_cmd(ToolCom * com, const char * args);
};

struct BroadcastSignalAction : public SendSignalAction { 
  friend class SendSignalDialog;
    
  BroadcastSignalAction();
  virtual ~BroadcastSignalAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
};

struct UnmarshallAction : public AnyAction { 
  friend class UnmarshallDialog;
    
  UnmarshallAction();
  virtual ~UnmarshallAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [in] "object" : the unmashalled object
  	// [out]* : the objects (pin addable et removables)
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
};

struct ValueSpecificationAction : public AnyAction { 
  friend class ValueSpecificationDialog;
    
  MyStr uml_value;
  MyStr cpp_value;
  MyStr java_value;
  
  ValueSpecificationAction();
  virtual ~ValueSpecificationAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [out] : the value
  virtual bool may_add_pin() const;
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
  virtual bool tool_cmd(ToolCom * com, const char * args);
};

struct AcceptCallAction : public AnyAction {
  friend class AcceptCallDialog;
    
  MyStr uml_trigger;
  MyStr cpp_trigger;
  MyStr java_trigger;

  AcceptCallAction();
  virtual ~AcceptCallAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [out] : returnInformation
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
  virtual bool tool_cmd(ToolCom * com, const char * args);
};

struct ReplyAction : public AnyAction {
  friend class ReplyDialog;
    
  MyStr uml_trigger;
  MyStr cpp_trigger;
  MyStr java_trigger;

  ReplyAction();
  virtual ~ReplyAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [in] : returnInformation
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
  virtual bool tool_cmd(ToolCom * com, const char * args);
};

struct CreateObjectAction : public AnyAction {
  friend class CreateObjectDialog;
    
  MyStr classifier;

  CreateObjectAction();
  virtual ~CreateObjectAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [out] : result
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
  virtual bool tool_cmd(ToolCom * com, const char * args);
};

struct DestroyObjectAction : public AnyAction {
  friend class DestroyObjectDialog;
    
  bool is_destroy_links;
  bool is_destroy_owned_objects;

  DestroyObjectAction();
  virtual ~DestroyObjectAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [in] : result
  virtual bool may_add_pin() const;
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
  virtual bool tool_cmd(ToolCom * com, const char * args);
};

struct TestIdentityAction : public AnyAction {
  friend class TestIdentityDialog;

  TestIdentityAction();
  virtual ~TestIdentityAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [in] : first
  	// [in] : second
  	// [out] : result
  virtual bool may_add_pin() const;
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
};

struct RaiseExceptionAction : public AnyAction {
  friend class RaiseExceptionDialog;

  RaiseExceptionAction();
  virtual ~RaiseExceptionAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [in] : exception
  virtual bool may_add_pin() const;
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
};

struct ReduceAction : public AnyAction {
  friend class ReduceDialog;
  
  bool is_ordered;
  BrowserNode * reducer;	// activity or state machine

  ReduceAction();
  virtual ~ReduceAction();
  virtual AnyAction * duplicate() const;
  virtual UmlActionKind kind() const;
  virtual QValueList<PinDescr> pins() const;
  	// [in] : collection
  	// [out] : result
  virtual bool may_add_pin() const;
  virtual BasicData * depend_on();
  virtual void on_delete();
  virtual BrowserNode * referenced(const char *&) const;
  
  void save(QTextStream & st, QString & warning) const;
  void read(char * & st, char * & k);
  void send_def(ToolCom * com, DrawingLanguage);
  virtual bool tool_cmd(ToolCom * com, const char * args);
};

class ActivityActionData : public SimpleData {
  Q_OBJECT

  friend class ActivityActionDialog;
    
  protected:
    InfoData uml_condition;
    InfoData cpp_condition;
    InfoData java_condition;
    MyStr constraint;
    AnyAction * action;
    
    virtual void send_uml_def(ToolCom * com, BrowserNode * bn,
			      const QString & comment);
    virtual void send_cpp_def(ToolCom * com);
    virtual void send_java_def(ToolCom * com);
    
  public:
    ActivityActionData();
    ActivityActionData(UmlActionKind k);
    ActivityActionData(ActivityActionData * model, BrowserNode * br);
    virtual ~ActivityActionData();
    
    QString get_precond(DrawingLanguage) const;
    QString get_postcond(DrawingLanguage) const;
    QString str(DrawingLanguage lang) const;
    const char * get_constraint() const { return constraint; }        
    UmlActionKind get_action_kind() const {
      return (action) ? action->kind() : UmlOpaqueAction;
    }
    void set_action_kind(UmlActionKind);
    bool may_add_pin() const { return action->may_add_pin(); }
    
    AnyAction * get_action() const { return action; }
    void undepend();
    void update_depend();

    void edit();
    
    virtual bool tool_cmd(ToolCom * com, const char * args,
			  BrowserNode * bn, const QString & comment);
    
    void save(QTextStream &, QString & warning) const;
    void read(char * & st, char * & k);

  private slots:
    void depend_deleted();
};
  
#endif
