#ifndef _UMLBASECLASS_H
#define _UMLBASECLASS_H


#include "UmlClassMember.h"
#include "anItemKind.h"
#include "UmlTypeSpec.h"
#include <qvaluelist.h>
#include <qvector.h>
#include <qcstring.h>
#include <qdict.h>

#include "UmlFormalParameter.h"
#include "UmlActualParameter.h"
class UmlClass;
class UmlItem;
class UmlFormalParameter;
class UmlActualParameter;
class UmlClassDiagram;
class UmlArtifact;
class UmlComponent;
class UmlPackage;

//  Manage the classes
class UmlBaseClass : public UmlClassMember {
  public:
    // returns a new class named 's' created under 'parent' 
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlClass * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // returns TRUE in case the class is abstract
    bool isAbstract();

    // the set if the class is abstract
    //
    // On error return FALSE in C++, produce a RuntimeException in Java,
    // for instance you cannot set it to FALSE in case the class have
    // abstract operations
    bool set_isAbstract(bool y);

    // returns the type on which the class (in fact a typedef) is based
    const UmlTypeSpec & baseType();

    // to set the type on which the class (in fact a typedef) is based
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java, does not check that the class is (already) a typedef
    bool set_BaseType(const UmlTypeSpec & t);

    // returns (a copy of) the formals list
    QValueList<UmlFormalParameter> formals();

    // remove the formal of the given rank (0...), returns 0 on error
    //
    // On error return FALSE in C++, produce a RuntimeException in Java,
    // does not check that the class is (already) a typedef
    bool removeFormal(unsigned int rank);

    // adds a formal at the given rank (0...), returns 0 on error
    //
    //On error return FALSE in C++, produce a RuntimeException in Java,
    //does not check that the class is (already) a typedef
    
    bool addFormal(unsigned int rank, const UmlFormalParameter & formal);

    // replace the formal at the given rank (0...)
    //
    // On error return FALSE in C++, produce a RuntimeException in Java,
    // does not check that the class is (already) a typedef
    
    bool replaceFormal(unsigned int rank, const UmlFormalParameter & formal);

    // returns (a copy of) the actuals list
    QValueList<UmlActualParameter> actuals();

    // replace the actual value at the given rank (0...)
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool replaceActual(unsigned int rank, const UmlTypeSpec & type);

    // returns the optional associated diagram
    UmlClassDiagram * associatedDiagram();

    // sets the associated diagram, arg may be null to unset it
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_AssociatedDiagram(UmlClassDiagram * d);

    // returns the optional associated artifact,
    // to set it refer to the UmlBaseArtifact's operations
    // addAssociatedClass(), removeAssociatedClass() and
    // setAssociatedClasses()
    UmlArtifact * associatedArtifact();

    // returns the components realizing or providing the class.
    // To set them refer to the UmlBaseComponent's operation setAssociatedClasses()
    const QVector<UmlComponent> associatedComponents();

#ifdef WITHCPP
    // returns TRUE if the class is external, its definition
    // must contains how the name is made on the first line
    // (isCppExternal by default), the other lines if they exist
    // must contains the #include and may be using forms
    bool isCppExternal();

    // set if the class is external
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isCppExternal(bool y);
#endif

#ifdef WITHJAVA
    // returns TRUE if the class is external, its definition
    // must contain how the name is made on the first line
    // (isJavaExternal by default), the other lines are ignored
    bool isJavaExternal();

    // set if the class is external
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isJavaExternal(bool y);

    // returns TRUE is the class is public
    bool isJavaPublic();

    // set if the class is public
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isJavaPublic(bool y);

    // returns TRUE is the class is final   
    bool isJavaFinal();

    // set if the class is final
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isJavaFinal(bool y);
#endif

#ifdef WITHPHP
    // returns TRUE if the class is external, its definition
    // must contain how the name is made on the first line
    // (isPhpExternal by default), the other lines are ignored
    bool isPhpExternal();

    // set if the class is external
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isPhpExternal(bool y);

    // returns TRUE is the class is final   
    bool isPhpFinal();

    // set if the class is final
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isPhpFinal(bool y);
#endif

#ifdef WITHPYTHON
    // returns TRUE if the class is external, its definition
    // must contain how the name is made on the first line
    // (isPythonExternal by default), the other lines are ignored
    bool isPythonExternal();

    // set if the class is external
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isPythonExternal(bool y);

    // returns TRUE is the class is a Python 2.2 class   
    bool isPython_2_2();

    // set if the class is a Python 2.2 class 
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isPython_2_2(bool y);
#endif

#ifdef WITHIDL
    // returns the switch's type, significant in case the class
    // is an union in IDL
    const UmlTypeSpec & switchType();

    bool set_SwitchType(const UmlTypeSpec & t);

    // returns TRUE if the class is external, its definition
    // must contain how the name is made on the first line
    // (isIdlExternal by default), the other lines are ignored
    bool isIdlExternal();

    // set if the class is external
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isIdlExternal(bool y);

    // returns TRUE if the class is local, have sense for an interface
    bool isIdlLocal();

    // set if the class is local, have sense for an interface
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isIdlLocal(bool y);

    // returns TRUE is the class is custom, have sense for a valuetype
    bool isIdlCustom();

    // set if the class is custom have sense for aa valuetype
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isIdlCustom(bool y);
#endif

    //returns the class having the name given in argument in case it
    //exist, else 0/null. In case the package is specified (not 0/null), 
    //the returned class will be defined in a sub-level of the package.
    
    static UmlClass * get(const QCString & n, const UmlPackage * p);
    
    //returns all the classes
    static void getAll(QVector<UmlClass> & v);

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE. 
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool rec = FALSE, bool del = FALSE);

    // to set the name
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    virtual bool set_Name(const QCString & s);

  friend class UmlBaseRelation;
  friend class UmlBaseArtifact;

  private:
    static QDict<UmlClass> _classes;

    bool _abstract;

#ifdef WITHCPP
    bool _cpp_external : 1;
#endif

#ifdef WITHJAVA
    bool _java_external : 1;

    bool _java_final : 1;
#endif

#ifdef WITHPHP
    bool _php_external : 1;

    bool _php_final : 1;
#endif

#ifdef WITHPYTHON
    bool _python_external : 1;
    bool _python_2_2 : 1;
#endif

#ifdef WITHIDL
    bool _idl_external : 1;

    bool _idl_local : 1;

    bool _idl_custom : 1;
#endif
    

    UmlTypeSpec _base_type;

    UmlClassDiagram * _assoc_diagram;

#ifdef WITHIDL
    // union
    UmlTypeSpec _switch_type;
#endif


  protected:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseClass(void * id, const QCString & n);

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
    
    void reread_if_needed_();

};

#endif
