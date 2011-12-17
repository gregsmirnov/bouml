#ifndef UMLCLASSDIAGRAM_H
#define UMLCLASSDIAGRAM_H

#include "UmlBaseClassDiagram.h"

// This class manages 'class diagrams', notes that the class 'UmlDiagram'
// is a mother class of all the diagrams, allowing to generalize their
// management
// You can modify it as you want (except the constructor)

class UmlClassDiagram : public UmlBaseClassDiagram {
  public:
    UmlClassDiagram(void * id, const QCString & n) 
      : UmlBaseClassDiagram(id, n) {};
};

#endif
