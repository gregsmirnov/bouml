#ifndef _UMLBASEACTIVITYREGION_H
#define _UMLBASEACTIVITYREGION_H


#include "UmlItem.h"
#include <qcstring.h>

class UmlActivityDiagram;

class UmlBaseActivityRegion : public UmlItem {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseActivityRegion(void * id, const QCString & s) : UmlItem(id, s) {
    }


  public:
    // returns the optional associated diagram
    UmlActivityDiagram * associatedDiagram();

    // sets the associated diagram, arg may be null to unset it
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_AssociatedDiagram(UmlActivityDiagram * d);


  private:
    UmlActivityDiagram * _assoc_diagram;


  protected:
    virtual void read_uml_();

};

#endif
