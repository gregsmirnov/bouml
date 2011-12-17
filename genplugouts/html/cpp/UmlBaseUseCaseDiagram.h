#ifndef _UMLBASEUSECASEDIAGRAM_H
#define _UMLBASEUSECASEDIAGRAM_H


#include "UmlDiagram.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlUseCaseDiagram;
class UmlItem;
class UmlUseCaseDiagramDefinition;

//  Manage the use case diagrams
class UmlBaseUseCaseDiagram : public UmlDiagram {
  public:
    // returns a new use case diagram named 'name' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlUseCaseDiagram * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseUseCaseDiagram(void * id, const QCString & n) : UmlDiagram(id, n), _def(0) {};


  public:
    // return the semantic part of the diagram not present in the model
    UmlUseCaseDiagramDefinition * definition();

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. args unused
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    UmlUseCaseDiagramDefinition * _def;

};

#endif
