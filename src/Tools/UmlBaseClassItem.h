#ifndef _UMLBASECLASSITEM_H
#define _UMLBASECLASSITEM_H


#include "UmlItem.h"
#include <qcstring.h>

//  Mother class of the all the class's items including the class themself
class UmlBaseClassItem : public UmlItem {
  public:
#ifdef WITHCPP
    //  return the C++ declaration
    
    const QCString & cppDecl();

    //  to set the C++ declaration
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppDecl(const char * s);
#endif

#ifdef WITHJAVA
    //  return the Java defininition
    
    const QCString & javaDecl();

    //  to set the Java definition
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaDecl(const char * s);
#endif

#ifdef WITHPHP
    //  return the Php defininition
    
    const QCString & phpDecl();

    //  to set the Php definition
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_PhpDecl(const char * s);
#endif

#ifdef WITHPYTHON
    //  return the Python defininition
    
    const QCString & pythonDecl();

    //  to set the Python definition
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_PythonDecl(const char * s);
#endif

#ifdef WITHIDL
    //  return the IDL declaration
    
    const QCString & idlDecl();

    //  set the IDL declaration
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IdlDecl(const char * s);
#endif

    virtual void unload(bool = FALSE, bool = FALSE);

  friend class UmlBaseAttribute;
  friend class UmlBaseOperation;
  friend class UmlBaseRelation;

  private:
#ifdef WITHCPP
    QCString _cpp_decl;
#endif

#ifdef WITHJAVA
    QCString _java_decl;
#endif

#ifdef WITHPHP
    QCString _php_decl;
#endif

#ifdef WITHPYTHON
    QCString _python_decl;
#endif

#ifdef WITHIDL
    QCString _idl_decl;
#endif


  protected:
    UmlBaseClassItem(void * id, const QCString & n) : UmlItem(id, n) {};

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

  friend class UmlBaseClassMember;
  friend class UmlBaseExtraClassMember;
  friend class UmlBaseClass;
};

#endif
