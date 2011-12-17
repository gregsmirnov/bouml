#ifndef _UMLCOMPONENTDIAGRAM_H
#define _UMLCOMPONENTDIAGRAM_H


#include "UmlBaseComponentDiagram.h"
#include <qcstring.h>

// This class manages 'ccomponent diagrams', notes that the class 'UmlDiagram'
// is a mother class of all the diagrams, allowing to generalize their
// management
//
// You can modify it as you want (except the constructor)
class UmlComponentDiagram : public UmlBaseComponentDiagram {
  public:
    UmlComponentDiagram(void * id, const QCString & n) : UmlBaseComponentDiagram(id, n) {
      cpt[kind()] += 1;
    };

};

#endif
