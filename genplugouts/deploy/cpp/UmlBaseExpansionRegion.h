#ifndef _UMLBASEEXPANSIONREGION_H
#define _UMLBASEEXPANSIONREGION_H


#include "UmlActivityRegion.h"
#include "anItemKind.h"
#include <qcstring.h>
#include "anExpansionKind.h"

class UmlExpansionRegion;
class UmlItem;

class UmlBaseExpansionRegion : public UmlActivityRegion {
  public:
    //  returns a new expansion region named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlExpansionRegion * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseExpansionRegion(void * id, const QCString & s) : UmlActivityRegion(id, s) {
    }


  public:
    // return the  return the isMustIsolate attribute, if TRUE the actions in the region execute in isolation from actions outside the region.
    bool isMustIsolate();

    // set the isMustIsolate attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isMustIsolate(bool v);

    // return the mode attribute, the way in which the execution interact.
    anExpansionKind mode();

    // set the mode attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Mode(anExpansionKind v);


  private:
    bool _must_isolate;

    anExpansionKind _mode;


  protected:
    virtual void read_uml_();

};

#endif
