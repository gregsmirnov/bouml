
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
#include "UmlState.h"
#include "UmlTransition.h"
#include "UmlRegion.h"
#include "UmlStateDiagram.h"
#include "UmlStateAction.h"
#include "UmlInitialPseudoState.h"
#include "UmlEntryPointPseudoState.h"
#include "UmlFinalState.h"
#include "UmlTerminatePseudoState.h"
#include "UmlExitPointPseudoState.h"
#include "UmlDeepHistoryPseudoState.h"
#include "UmlShallowHistoryPseudoState.h"
#include "UmlJunctionPseudoState.h"
#include "UmlChoicePseudoState.h"
#include "UmlForkPseudoState.h"
#include "UmlJoinPseudoState.h"
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

bool UmlBaseItem::isMarked() {
  read_if_needed_();

  return _marked;
}

bool UmlBaseItem::set_isMarked(bool y) {
  return set_it_(_marked, y, setMarkedCmd);
}

const QVector<UmlItem> UmlBaseItem::referencedBy() {
  UmlCom::send_cmd(_identifier, referencedByCmd);
  
  QVector<UmlItem> result;
  
  UmlCom::read_item_list(result);
  return result;
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

const QVector<UmlItem> UmlBaseItem::markedItems()
{
  UmlCom::send_cmd(miscGlobalCmd, allMarkedCmd);
  
  QVector<UmlItem> result;
  
  UmlCom::read_item_list(result);
  return result;
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
      return new UmlRelation(id, name);
    case anAttribute:
      return new UmlAttribute(id, name);
    case anOperation:
      return new UmlOperation(id, name);
    case anExtraClassMember:
      return new UmlExtraClassMember(id, name);
    case aClass:
      return new UmlClass(id, name);
    case anUseCase:
      return new UmlUseCase(id, name);
    case aComponent:
      return new UmlComponent(id, name);
    case anArtifact:
      return new UmlArtifact(id, name);
    case aNode:
      return new UmlNode(id, name);
    case aNcRelation:
      return new UmlNcRelation(id, name);
    case aClassDiagram:
      return new UmlClassDiagram(id, name);
    case anUseCaseDiagram:
      return new UmlUseCaseDiagram(id, name);
    case aSequenceDiagram:
      return new UmlSequenceDiagram(id, name);
    case aCollaborationDiagram:
      return new UmlCollaborationDiagram(id, name);
    case aComponentDiagram:
      return new UmlComponentDiagram(id, name);
    case aDeploymentDiagram:
      return new UmlDeploymentDiagram(id, name);
    case aClassView:
      return new UmlClassView(id, name);
    case anUseCaseView:
      return new UmlUseCaseView(id, name);
    case aComponentView:
      return new UmlComponentView(id, name);
    case aDeploymentView:
      return new UmlDeploymentView(id, name);
    case aPackage:
      return new UmlPackage(id, name);
    case aState:
      return new UmlState(id, name);
    case aTransition:
      return new UmlTransition(id);
    case aRegion:
      return new UmlRegion(id, name);
    case aStateDiagram:
      return new UmlStateDiagram(id, name);
    case aStateAction:
      return new UmlStateAction(id);
    case anInitialPseudoState:
      return new UmlInitialPseudoState(id);
    case anEntryPointPseudoState:
      return new UmlEntryPointPseudoState(id, name);
    case aFinalState:
      return new UmlFinalState(id);
    case aTerminatePseudoState:
      return new UmlTerminatePseudoState(id);
    case anExitPointPseudoState:
      return new UmlExitPointPseudoState(id, name);
    case aDeepHistoryPseudoState:
      return new UmlDeepHistoryPseudoState(id);
    case aShallowHistoryPseudoState:
      return new UmlShallowHistoryPseudoState(id);
    case aJunctionPseudoState:
      return new UmlJunctionPseudoState(id);
    case aChoicePseudoState:
      return new UmlChoicePseudoState(id);
    case aForkPseudoState:
      return new UmlForkPseudoState(id);
    case aJoinPseudoState:
      return new UmlJoinPseudoState(id);
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

