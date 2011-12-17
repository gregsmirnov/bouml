#ifndef _UMLMESSAGE_H
#define _UMLMESSAGE_H


#include "UmlBaseMessage.h"
#include <qvector.h>

class FileOut;
class UmlItem;

// this class manages messages indenpendently of the diagram,
// you can modify it
class UmlMessage : public UmlBaseMessage {
  public:
    UmlMessage() : itsrank(++next_rank) {}

    static void write_connectors(FileOut & out, UmlItem * diagram, const QVector<UmlMessage> & msgs);


  protected:
    int itsrank;

    static int next_rank;

};

#endif
