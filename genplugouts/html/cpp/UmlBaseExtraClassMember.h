#ifndef _UMLBASEEXTRACLASSMEMBER_H
#define _UMLBASEEXTRACLASSMEMBER_H


#include "UmlClassItem.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlExtraClassMember;
class UmlClass;

// Manage the 'extra' class members to produce any code, for instance a C++ pre-processing form or a Java initialization

class UmlBaseExtraClassMember : public UmlClassItem {
  public:
    // returns a new extra class member named 'name'  (the name may be empty)
    // created under the class 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlExtraClassMember * create(UmlClass * parent, const char * name);

    // returns the kind of the item
    virtual anItemKind kind();

#ifdef WITHCPP
    // returns TRUE if the extra member is managed as an inline operation in C++
    
    bool isCppInline();

    // to set if the extra member is managed as an inline operation in C++
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isCppInline(bool y);

    // returns the operation's definition in C++, notes that the declaration
    // is returned by the inherited ClassItemBase::CppDecl() operation
    const QCString & cppDef();

    // sets the operation's definition in C++, notes that the declaration
    // is set through the inherited ClassItemBase::set_CppDecl() operation
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppDef(const char * s);
#endif

    // to unload the object to free memory, it will be reloaded
    // automatically if needed. args unused
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
#ifdef WITHCPP
    bool _cpp_inline;
    

    QCString _cpp_def;
#endif

  protected:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseExtraClassMember(void * id, const QCString & n) : UmlClassItem(id, n) {};

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

};

#endif
