#ifndef _UMLBASECLASSMEMBER_H
#define _UMLBASECLASSMEMBER_H


#include "UmlClassItem.h"
#include "aVisibility.h"
#include <qcstring.h>
#include "UmlBaseClassItem.h"

// This class manages 'true' class's items : attributes, relation,
// operation and sub-classes
class UmlBaseClassMember : public UmlClassItem {
  public:
    // Indicate if the member is a 'class member' (static)
    // Always false in case of a class
    bool isClassMember();

    // Set if the member is a 'class member' (static), not significant for a class
    //
    // On error return FALSE in C++, produce a RuntimeException in Java,
    // does not check that the class is (already) a typedef
    bool set_isClassMember(bool y);

    // The member's visibility
    aVisibility visibility();

    // Set the member visibility,  DefaultVisibility is not legal
    //
    // On error return FALSE in C++, produce a RuntimeException in Java,
    // does not check that the class is (already) a typedef
    bool set_Visibility(aVisibility v);

#ifdef WITHCPP
    // Special case for C++, thank to the 'friend class X' it may be
    // usefull to have a C++ visibility != other visibility
    // In case the C++ visibility must follow the other, return
    // DefaultVisibility
    aVisibility cppVisibility();

    // Set the visibility for C++, DefaultVisibility is legal
    //
    // On error return FALSE in C++, produce a RuntimeException in Java,
    // does not check that the class is (already) a typedef
    bool set_CppVisibility(aVisibility v);
    // Indicate if the member is 'volatile'
    // Always false in case of a class
    bool isCppVolatile();

    // Set if the member is 'volatile', not significant for a class
    //
    // On error return FALSE in C++, produce a RuntimeException in Java,
    // does not check that the class is (already) a typedef
    bool set_isCppVolatile(bool y);
#endif

  private:
    bool _class_member;

    aVisibility _visibility : 8;

#ifdef WITHCPP
    aVisibility _cpp_visibility : 8;

    bool _cpp_volatile;
#endif


  protected:
    UmlBaseClassMember(void * id, const QCString & n) : UmlClassItem(id, n) {};

    void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

  friend class UmlBaseOperation;
  friend class UmlBaseAttribute;
  friend class UmlBaseRelation;
  friend class UmlBaseClass;
};

#endif
