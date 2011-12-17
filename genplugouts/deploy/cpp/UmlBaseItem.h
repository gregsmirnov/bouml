#ifndef _UMLBASEITEM_H
#define _UMLBASEITEM_H


#include "anItemKind.h"
#include <qcstring.h>
#include <qvector.h>
#include <qdict.h>
#include <qptrdict.h>
#include "OnInstanceCmd.h"
#include "aVisibility.h"
#include "aDirection.h"

class UmlItem;
class UmlTypeSpec;

// Base class of all the classes representing browser's objects.
//
// In the API all for each 'system' class exists a 'user' class
// under your responsability which inherits the 'system' class.
//
// You do not have to change the 'system' classes, place your
// attributes and operations in the corresponding 'user' classes :
// all the instances made by the 'system' classes are instance of
// the corresponding 'user' classes.
//
// You must never create or delete yourself a class of the API,
// use the Create operation defined in the 'system' classes.
//
// To not allow you to access to the 'system' attributes or to
// call 'system' internal operations even by error, the 'system' internal
// parts are declared 'private' rather than 'public' or 'protected'.
class UmlBaseItem {
  public:
    // returns the kind of the item
    virtual anItemKind kind() = 0;

    // returns the name
    const QCString & name() { return _name; };

    // to set the name
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    virtual bool set_Name(const QCString & s);

    // returns the stereotype
    const QCString & stereotype();

    // to set the stereotype
    //  On error return FALSE in C++, produce a RuntimeException in Java
    virtual bool set_Stereotype(const QCString & s);

    // If the current stereotype is part of a profile add needed properties.
    // In all cases remove extra properties whose keys contain two ':'.
    //
    // If the element is read-only, return FALSE in C++, produce a RuntimeException in Java
    bool applyStereotype();

    // returns the description
    const QCString & description();

    // to set the description
    //  On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Description(const QCString & s);

    // returns the parent (0/null if it is applied on the project itself),
    // to go all over the browser tree
    UmlItem * parent();

    // returns (in Java : a copy of) the children list, to go all over the browser tree
    const QVector<UmlItem> children();

    // return TRUE if the children exist and are visible (independently
    // of the scroll bar) in the browser, else FALSE
    bool childrenVisible();

    // open the item in the browser (its children are visible) if y is TRUE,
    // else close it (its children are not visible). May be applied on items
    // without children. The item is also selected in the browser, so 
    // set_ChildrenVisible(childrenVisible()) is the right way to select the item
    // in the browser.
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_childrenVisible(bool y);

    // In C++, in case the key specified by 'k' exist for the current
    // object, the string 'v' is modified which the associated value
    // and TRUE is returned, else returns FALSE.
    //
    // In Java return the value associated with the key specified by 'k'
    // for the current object, null in case the property doesn't
    // exist.
    //
    // These properties may be edited through the object dialog last tab
    // named 'user'
    bool propertyValue(const QCString & k, QCString & v);

    // to set (may be insert a new) the value 'v' associated to the key 'k'
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_PropertyValue(const QCString & k, const QCString & v);

    // returns all the properties of the object through (in Java a copy of) a dictionnary
    
    const QDict<QCString> properties();

    // If the parameter is null, move the current item to be
    // the first child of its parent. Else the current item and
    // the parameter must have the same parent, the current item
    // is moved to be just after the parameter.
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool moveAfter(const UmlItem * x);

    //  Returns the absolute path name of the file memoring the Item.
    //  In general an item is saved in a file associated to its package,
    //  the exceptions are :
    //
    //  - the body of the operations of a class which definition contains the
    //     keyword '{$body}'. This path name is returned when you apply
    //     supportFile on any operation of the class.
    //
    //  - the drawing of the diagrams, this path name is returned when
    //     you apply supportFile on the diagram.
    //
    //  - the configuration of the tools edited throw the entry 'Tools Settings' of
    //     the menu Tools is saved in the file 'tools'
    //
    //  - the configuration of the '#include' and 'using' forms associated to the
    //    external type and edited through the last C++ tab of the
    //    'Generation Settings' dialog is saved in the file 'cpp_includes'
    //
    //  - the configuration of the 'imports' forms associated to the external
    //    types and edited through the last Java tab of the 'Generation Settings'
    //    dialog is saved in the file 'java_imports'
    //
    //  - the configuration of the '#include' forms associated to the
    //    external type and edited through the last Idl tab of the
    //    'Generation Settings' dialog is saved in the file 'idl_includes'
    // 
    QCString supportFile();

    // return TRUE in case the item may be modified, i.e. it is not
    // an API base 'plug-out' class and the corresponding file(s) are not
    // read-only.
    bool isWritable();

    //  Apply asynchronously the tool on the item, returns an identifier to call isToolDone()
    int apply(QCString cmd);

    //return TRUE in case the item is marked
    bool isMarked();

    //  to mark/unmark the current item. The project cannot marked
    //  On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isMarked(bool y);

    //  Returns the items referencing the current one.
    //  The result may contains UmlAttribute, UmlRelations, UmlNcRelations,
    //  UmlOperation (their bodies are not taken into account) , UmlClass
    //  and UmlComponents.
    const QVector<UmlItem> referencedBy();

    // return a constant identifier, it is unique within a given
    // kind of element (two classes can't have the same identifiers, but
    // a class and a component may have the same identifier)
    // except for the diagrams (a class diagram can't have the identifier
    // of a component diagram)
    int getIdentifier();

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE. 
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool rec = FALSE, bool del = FALSE);

    // remove the element from the model, use it carefully because
    // after that the element can't be used anymore by the plug-out
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    bool deleteIt();

    //  'id' is an identifier returned by apply(), indicates if the tool is still running
    static bool isToolRunning(int id);

    static const QVector<UmlItem> markedItems();


  private:
    bool _defined;

    bool _marked;

    void * _identifier;


  protected:
    int _modeler_id;


  private:
    QCString _name;

    //  Note : protected in Java for internal reason, but do NOT
    //  access to this field yourself !
    QCString _stereotype;

    QCString _description;

    UmlItem * _parent;

    QVector<UmlItem> * _children;

    QDict<QCString> _dict;

    static QPtrDict<UmlItem> _all;


  protected:
    //internal, do NOT use it
    
    void read_if_needed_();

    //internal, do NOT use it
    
    UmlItem * create_(anItemKind k, const char * s);

    //internal, do NOT use it
    
    virtual void read_uml_();

#ifdef WITHCPP
    //internal, do NOT use it
    
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    //internal, do NOT use it
    
    virtual void read_java_();
#endif

#ifdef WITHPHP
    //internal, do NOT use it
    
    virtual void read_php_();
#endif

#ifdef WITHPYTHON
    //internal, do NOT use it
    
    virtual void read_python_();
#endif

#ifdef WITHIDL
    //internal, do NOT use it
    
    virtual void read_idl_();
#endif

    //internal, do NOT use it
    
    void read_children_();

    //internal, do NOT use it
    
    void reread_children_if_needed_();

    //internal, do NOT use it
    
    bool set_it_(bool & r, bool v, OnInstanceCmd cmd);

    //internal, do NOT use it
    
    bool set_it_(aVisibility & r, aVisibility v, OnInstanceCmd cmd);

    //internal, do NOT use it
    
    bool set_it_(aDirection & r, aDirection v, OnInstanceCmd cmd);

    //internal, do NOT use it
    
    bool set_it_(QCString & r, const char * v, OnInstanceCmd cmd);

    //internal, do NOT use it
    
    bool set_it_(UmlTypeSpec & r, const UmlTypeSpec & t, OnInstanceCmd cmd);

    //internal, do NOT use it
    
    static UmlItem * read_();


  public:
    // the constructor, do not call it yourself !
    UmlBaseItem(void * id, const QCString & n);

    // the destructor, do not delete objects yourself !!!!!!!!!!
    virtual ~UmlBaseItem();

  friend class UmlBaseView;
  friend class UmlBaseClassInstance;
  friend class UmlBaseParameterSet;
  friend class UmlBaseActivityParameter;
  friend class UmlBaseActivityPin;
  friend class UmlBasePinParameter;
  friend class UmlBaseExpansionNode;
  friend class UmlBaseActivityObject;
  friend class UmlBaseJoinActivityNode;
  friend class UmlBaseForkActivityNode;
  friend class UmlBaseMergeActivityNode;
  friend class UmlBaseDecisionActivityNode;
  friend class UmlBaseActivityFinalActivityNode;
  friend class UmlBaseFlowFinalActivityNode;
  friend class UmlBaseInitialActivityNode;
  friend class UmlBaseActivityControlNode;
  friend class UmlBaseRemoveVariableValueAction;
  friend class UmlBaseAddVariableValueAction;
  friend class UmlBaseWriteVariableValueAction;
  friend class UmlBaseReadVariableValueAction;
  friend class UmlBaseClearVariableValueAction;
  friend class UmlBaseAccessVariableValueAction;
  friend class UmlBaseCallBehaviorAction;
  friend class UmlBaseCallOperationAction;
  friend class UmlBaseAcceptEventAction;
  friend class UmlBaseOpaqueAction;
  friend class UmlBaseValueSpecificationAction;
  friend class UmlBaseBroadcastSignalAction;
  friend class UmlBaseSendSignalAction;
  friend class UmlBaseOnSignalAction;
  friend class UmlBaseUnmarshallAction;
  friend class UmlBaseSendObjectAction;
  friend class UmlBaseActivityAction;
  friend class UmlBaseInterruptibleActivityRegion;
  friend class UmlBaseExpansionRegion;
  friend class UmlBaseActivityRegion;
  friend class UmlBaseFlow;
  friend class UmlBaseActivityNode;
  friend class UmlBaseActivityDiagram;
  friend class UmlBaseActivity;
  friend class UmlBaseObjectDiagram;
  friend class UmlBaseJoinPseudoState;
  friend class UmlBaseForkPseudoState;
  friend class UmlBaseChoicePseudoState;
  friend class UmlBaseJunctionPseudoState;
  friend class UmlBaseShallowHistoryPseudoState;
  friend class UmlBaseDeepHistoryPseudoState;
  friend class UmlBaseExitPointPseudoState;
  friend class UmlBaseTerminatePseudoState;
  friend class UmlBaseFinalState;
  friend class UmlBaseEntryPointPseudoState;
  friend class UmlBaseInitialPseudoState;
  friend class UmlBaseStateAction;
  friend class UmlBaseRegion;
  friend class UmlBaseTransition;
  friend class UmlBaseStateDiagram;
  friend class UmlBaseState;
  friend class UmlBaseComponent;
  friend class UmlBaseExtraClassMember;
  friend class UmlBaseClassMember;
  friend class UmlCom;
  friend class UmlBaseActualParameter;
  friend class UmlBaseFormalParameter;
  friend class UmlBaseDiagram;
  friend class UmlBaseUseCaseView;
  friend class UmlBaseUseCaseDiagram;
  friend class UmlBaseUseCase;
  friend class UmlBaseSequenceDiagram;
  friend class UmlBaseNcRelation;
  friend class UmlBaseRelation;
  friend class UmlBasePackage;
  friend class UmlBaseOperation;
  friend class UmlBaseNode;
  friend class UmlBaseDeploymentView;
  friend class UmlBaseDeploymentDiagram;
  friend class UmlBaseDeployment;
  friend class UmlBaseComponentView;
  friend class UmlBaseComponentDiagram;
  friend class UmlBaseArtifact;
  friend class UmlBaseCollaborationDiagram;
  friend class UmlBaseClassView;
  friend class UmlBaseClassDiagram;
  friend class UmlBaseClass;
  friend class UmlBaseAttribute;
  friend class UmlBaseClassItem;
  friend class UmlBaseUseCaseReference;
  friend class UmlBaseUseCaseAssociation;
  friend class UmlBaseClassInstanceReference;
  friend class UmlBaseMessage;
  friend class UmlBaseUseCaseDiagramDefinition;
  friend class UmlBaseSequenceDiagramDefinition;
  friend class UmlBaseCollaborationDiagramDefinition;
  friend class UmlBaseFragment;
};

#endif
