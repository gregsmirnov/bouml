#ifndef _UMLCLASSINSTANCEREFERENCE_H
#define _UMLCLASSINSTANCEREFERENCE_H


#include "UmlBaseClassInstanceReference.h"
#include <qvector.h>

class FileOut;
class UmlItem;

// this class manages class instance reference,
// you can modify it
class UmlClassInstanceReference : public UmlBaseClassInstanceReference {
  public:
    UmlClassInstanceReference() : rank(++next_rank) {}

    // return the connector id prefix between the instances
    // warning : a call modify result of a previous one
    
    const char * connector(UmlClassInstanceReference * other);

    // return the ownedAttribute property id prefix
    // warning : a call modify result of a previous one
    
    const char * attribute();

    // return the lifeline id prefix
    // warning : a call modify result of a previous one
    
    const char * lifeline();

    static void write_lifelines(FileOut & out, UmlItem * diagram, const QVector<UmlClassInstanceReference> & insts);

    static void write_attributes(FileOut & out, UmlItem * diagram, const QVector<UmlClassInstanceReference> & insts);


  private:
    int rank;

    static int next_rank;

};

#endif
