#ifndef _UMLCLASSDIAGRAM_H
#define _UMLCLASSDIAGRAM_H


#include "UmlBaseClassDiagram.h"
#include <qcstring.h>

class File;
class UmlClassView;

// This class manages 'class diagrams', notes that the class 'UmlDiagram'
// is a mother class of all the diagrams, allowing to generalize their
// management
//
// You can modify it as you want (except the constructor)
class UmlClassDiagram : public UmlBaseClassDiagram {
  public:
    UmlClassDiagram(void * id, const QCString & n) : UmlBaseClassDiagram(id, n) {
      cpt[kind()] += 1;
    };

    static void import(File & f, UmlClassView * p);


  private:
    void import(File & f);

};

#endif
