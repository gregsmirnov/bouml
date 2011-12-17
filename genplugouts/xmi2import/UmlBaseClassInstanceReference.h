#ifndef _UMLBASECLASSINSTANCEREFERENCE_H
#define _UMLBASECLASSINSTANCEREFERENCE_H


#include <qcstring.h>

class UmlClass;
class UmlClassInstance;

// this class manages class instance reference
class UmlBaseClassInstanceReference {
  public:
    // return the type of the instance
    UmlClass * type() const;

    // return the corresponding instance in the model, or 0/null
    UmlClassInstance * instance() const {
        return _instance;
    }

    // return the name of the instance
    const QCString & name() const;


  private:
    UmlClass * _class;

    UmlClassInstance * _instance;

    QCString _name;

    // internal, don't call it
    void read_();

  friend class UmlBaseSequenceDiagramDefinition;
  friend class UmlBaseCollaborationDiagramDefinition;
};

#endif
