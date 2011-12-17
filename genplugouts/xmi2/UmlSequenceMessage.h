#ifndef _UMLSEQUENCEMESSAGE_H
#define _UMLSEQUENCEMESSAGE_H


#include "UmlBaseSequenceMessage.h"
#include <qvector.h>
#include <qlist.h>

class FileOut;
class UmlItem;

// this class manages messages in a sequence diagram,
// you can modify it
class UmlSequenceMessage : public UmlBaseSequenceMessage {
  public:
    UmlSequenceMessage();

    static void write(FileOut & out, UmlItem * diagram, const QVector< UmlSequenceMessage > & msgs);

    void write_fragment(FileOut & out, UmlItem * diagram, QList< UmlSequenceMessage > & msgs);

    static void write_them(FileOut & out, UmlItem * diagram, const QVector< UmlSequenceMessage > & msgs);


  private:
    static void set_reverses(const QVector<UmlSequenceMessage> & msgs);

    UmlSequenceMessage * reverse;

    bool used;

};

#endif
