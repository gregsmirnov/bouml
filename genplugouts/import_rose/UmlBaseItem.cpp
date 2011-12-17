
#include "UmlBaseItem.h"
#include "UmlItem.h"
#include "UmlTypeSpec.h"

#include "UmlCom.h"
#include "UmlRelation.h"
#include "UmlNcRelation.h"
#include "UmlAttribute.h"
#include "UmlOperation.h"
#include "UmlClass.h"
#include "UmlUseCase.h"
#include "UmlNode.h"
#include "UmlArtifact.h"
#include "UmlComponent.h"
#include "UmlClassDiagram.h"
#include "UmlUseCaseDiagram.h"
#include "UmlSequenceDiagram.h"
#include "UmlCollaborationDiagram.h"
#include "UmlComponentDiagram.h"
#include "UmlDeploymentDiagram.h"
#include "UmlClassView.h"
#include "UmlUseCaseView.h"
#include "UmlComponentView.h"
#include "UmlDeploymentView.h"
#include "UmlPackage.h"
#include "UmlExtraClassMember.h"
#include "MiscGlobalCmd.h"

bool UmlBaseItem::set_Name(const QCString & s) {
  return set_it_(_name, s, setNameCmd);
}

const QCString & UmlBaseItem::stereotype() {
  read_if_needed_();
  
  return _stereotype;
}

bool UmlBaseItem::set_Stereotype(const QCString & s) {
  return set_it_(_stereotype, s, setStereotypeCmd);
}

const QCString & UmlBaseItem::description() {
  read_if_needed_();
  
  return _description;
}

bool UmlBaseItem::set_Description(const QCString & s) {
  return set_it_(_description, s, setDescriptionCmd);
}

UmlItem * UmlBaseItem::parent() {
  if (_parent == 0) {
    UmlCom::send_cmd(_identifier, parentCmd);
    
    _parent = UmlBaseItem::read_();
  }
  
  return _parent;
}

const QVector<UmlItem> UmlBaseItem::children() {
  if (_children == 0)
    read_children_();
  
  return *_children;
}

bool UmlBaseItem::childrenVisible() {
  UmlCom::send_cmd(_identifier, isOpenCmd);
  return UmlCom::read_bool();
}

bool UmlBaseItem::set_childrenVisible(bool y) {
  UmlCom::send_cmd(_identifier, setOpenCmd, (char) y);
  return UmlCom::read_bool();
}

bool UmlBaseItem::propertyValue(const QCString & k, QCString & v) {
  read_if_needed_();
  
  QCString * s = _dict[k];
  
  if (s == 0)
    return FALSE;
  
  v = *s;
  return TRUE;

}

bool UmlBaseItem::set_PropertyValue(const QCString & k, const QCString & v) {
  read_if_needed_();
  
  UmlCom::send_cmd(_identifier, setCoupleValueCmd, k, v);
  if (UmlCom::read_bool()) {
    if (_defined) {
      QCString * s = _dict[k];
      
      if (s == 0)
	_dict.insert(k, new QCString(v));
      else
	*s = v;
    }
    
    return TRUE;
  }
  else
    return FALSE;
}

const QDict<QCString> UmlBaseItem::properties() {
  read_if_needed_();

  return _dict;
}

QCString UmlBaseItem::supportFile() {
  UmlCom::send_cmd(_identifier, supportFileCmd); 
  return UmlCom::read_string();
}

bool UmlBaseItem::isWritable() {
  UmlCom::send_cmd(_identifier, isWritableCmd); 
  return UmlCom::read_bool();
}

int UmlBaseItem::apply(QCString cmd) {
  UmlCom::send_cmd(_identifier, applyCmd, cmd); 
  return (int) UmlCom::read_unsigned();
}

void UmlBaseItem::unload(bool rec, bool del) {
  _defined = FALSE;
  _stereotype = 0;
  _dict.clear();
  _description = 0;

  if (_children != 0) {
    if (rec) {
      for (unsigned chindex = 0; chindex != _children->size(); chindex += 1) {
	_children->at(chindex)->unload(TRUE, del);
	if (del)
	  delete _children->at(chindex);
      }
    }
    
    delete _children;
    _children = 0;
  }
}

bool UmlBaseItem::isToolRunning(int id)
{
  UmlCom::send_cmd(miscGlobalCmd, toolRunningCmd, id);
  return UmlCom::read_bool();
}

QPtrDict<UmlItem> UmlBaseItem::_all(997);

void UmlBaseItem::read_if_needed_() {
  if (!_defined) {
#ifdef WITHCPP
# ifdef WITHJAVA
#  ifdef WITHIDL
    UmlCom::send_cmd(_identifier, getDefCmd);
    read_uml_();
    read_cpp_();
    read_java_();
    read_idl_();
#  else
    ... WITHIDL must be defined when WITHCPP and WITHJAVA are both defined
#  endif
# else
#  ifdef WITHIDL
    ... WITHJAVA must be defined when WITHCPP and WITHIDL are both defined
#  else
    UmlCom::send_cmd(_identifier, getCppDefCmd);
    read_uml_();
    read_cpp_();
#  endif
# endif
#else
# ifdef WITHJAVA
#  ifdef WITHIDL
    ... WITHCPP must be defined when WITHIDL and WITHJAVA are both defined
#  else
    UmlCom::send_cmd(_identifier, getJavaDefCmd);
    read_uml_();
    read_java_();
#  endif
# else
#  ifdef WITHIDL
    UmlCom::send_cmd(_identifier, getIdlDefCmd);
    read_uml_();
    read_idl_();
#  else
    UmlCom::send_cmd(_identifier, getUmlDefCmd);
    read_uml_();
#  endif
# endif
#endif
    
    _defined = TRUE;
  }
}

UmlItem * UmlBaseItem::create_(anItemKind k, const char * s) {
  UmlCom::send_cmd(_identifier, createCmd, k, s);

  UmlItem * result = UmlBaseItem::read_();
  
  if (result != 0) {
    if (_children != 0) {
      unsigned n = _children->count();
      
      _children->resize(n + 1);
      _children->insert(n, result);
    }
    ((UmlBaseItem *) result)->_parent = (UmlItem *) this;
  }
  
  return result;
}

void UmlBaseItem::read_uml_() {
  _stereotype = UmlCom::read_string();

  unsigned n = UmlCom::read_unsigned();
  
  while (n--) {
    QCString k = UmlCom::read_string();
    
    _dict.insert(k, new QCString(UmlCom::read_string()));
  }
  
  _description = UmlCom::read_string();
  
  _marked = UmlCom::read_bool();
}

#ifdef WITHCPP
void UmlBaseItem::read_cpp_() {
}
#endif

#ifdef WITHJAVA
void UmlBaseItem::read_java_() {
}
#endif

#ifdef WITHIDL
void UmlBaseItem::read_idl_() {
}
#endif

void UmlBaseItem::read_children_() {
  UmlCom::send_cmd(_identifier, childrenCmd);
  _children = new QVector<UmlItem>;
  
  UmlCom::read_item_list(*_children);
  
  unsigned n = _children->size();
  
  while (n--)
    ((UmlBaseItem *) _children->at(n))->_parent = (UmlItem *) this;
}

void UmlBaseItem::reread_children_if_needed_() {
  if (_children != 0) {
    delete _children;
    read_children_();
  }
}

bool UmlBaseItem::set_it_(bool & r, bool v, OnInstanceCmd cmd) {
  UmlCom::send_cmd(_identifier, cmd, (char) v);
  if (UmlCom::read_bool()) {
    r = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseItem::set_it_(aVisibility & r, aVisibility v, OnInstanceCmd cmd) {
  UmlCom::send_cmd(_identifier, cmd, (char) v);
  if (UmlCom::read_bool()) {
    r = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseItem::set_it_(aDirection & r, aDirection v, OnInstanceCmd cmd) {
  UmlCom::send_cmd(_identifier, cmd, (char) v);
  if (UmlCom::read_bool()) {
    r = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseItem::set_it_(QCString & r, const char * v, OnInstanceCmd cmd) {
  UmlCom::send_cmd(_identifier, cmd, v);
  if (UmlCom::read_bool()) {
    if (_defined) r = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseItem::set_it_(UmlTypeSpec & r, const UmlTypeSpec & t, OnInstanceCmd cmd) {
  UmlCom::send_cmd(_identifier, cmd, t);
  if (UmlCom::read_bool()) {
    if (_defined) r = t;
    return TRUE;
  }
 else
   return FALSE;
}

UmlItem * UmlBaseItem::read_()
{
  void * id = UmlCom::read_id();
  
  if (id == 0)
    return 0;
  
  anItemKind kind = (anItemKind) UmlCom::read_char();
  const char * name = UmlCom::read_string();
  
#ifndef WIN32
  //cout << "UmlBaseItem::read id " << id << " kind " << kind << " name " << name << '\n';
#endif
  
  UmlItem * result = _all[id];
  
  if (result == 0) {
    switch (kind) {
    case aRelation:
      result = new UmlRelation(id, name);
      break;
    case anAttribute:
      result = new UmlAttribute(id, name);
      break;
    case anOperation:
      result = new UmlOperation(id, name);
      break;
    case anExtraClassMember:
      result = new UmlExtraClassMember(id, name);
      break;
    case aClass:
      result = new UmlClass(id, name);
      break;
    case anUseCase:
      result = new UmlUseCase(id, name);
      break;
    case aComponent:
      result = new UmlComponent(id, name);
      break;
    case anArtifact:
      result = new UmlArtifact(id, name);
      break;
    case aNode:
      result = new UmlNode(id, name);
      break;
    case aNcRelation:
      result = new UmlNcRelation(id, name);
      break;
    case aClassDiagram:
      result = new UmlClassDiagram(id, name);
      break;
    case anUseCaseDiagram:
      result = new UmlUseCaseDiagram(id, name);
      break;
    case aSequenceDiagram:
      result = new UmlSequenceDiagram(id, name);
      break;
    case aCollaborationDiagram:
      result = new UmlCollaborationDiagram(id, name);
      break;
    case aComponentDiagram:
      result = new UmlComponentDiagram(id, name);
      break;
    case aDeploymentDiagram:
      result = new UmlDeploymentDiagram(id, name);
      break;
    case aClassView:
      result = new UmlClassView(id, name);
      break;
    case anUseCaseView:
      result = new UmlUseCaseView(id, name);
      break;
    case aComponentView:
      result = new UmlComponentView(id, name);
      break;
    case aDeploymentView:
      result = new UmlDeploymentView(id, name);
      break;
    case aPackage:
      result = new UmlPackage(id, name);
      break;
    default:
      UmlCom::bye();
      UmlCom::fatal_error(QCString("unknown item type ") + QCString().setNum(kind));
    }
  }
  
  return result;
}

 UmlBaseItem::UmlBaseItem(void * id, const QCString & n)
    : _defined(FALSE), _identifier(id), _name(n), _parent(0), _children(0)  {
  _all.insert(id, (UmlItem *) this);
  if (_all.count()/_all.size() > 10)
    _all.resize(_all.size() * 2 - 1);
}

 UmlBaseItem::~UmlBaseItem() {
  if (_children != 0)
    delete _children;
  _all.remove(_identifier);
}

