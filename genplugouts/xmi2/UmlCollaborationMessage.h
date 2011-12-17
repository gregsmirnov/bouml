#ifndef _UMLCOLLABORATIONMESSAGE_H
#define _UMLCOLLABORATIONMESSAGE_H


#include "UmlBaseCollaborationMessage.h"
#include <qvector.h>

class FileOut;
class UmlItem;

// this class manages messages in a collaboration diagram,
// you can modify it
class UmlCollaborationMessage : public UmlBaseCollaborationMessage {
  public:
    static void write(FileOut & out, UmlItem * diagram, const QVector< UmlCollaborationMessage > & msgs, unsigned & index);

};

#endif
