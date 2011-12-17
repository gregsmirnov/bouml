#ifndef _UMLACTIVITYITEM_H
#define _UMLACTIVITYITEM_H


#include <qcstring.h>

class FileOut;
class UmlActivity;

class UmlActivityItem {
  public:
    //goes among tree to memorize incoming flow
    
    virtual void memo_incoming_flow() = 0;

    void write_selection(FileOut & out, QCString s);

    UmlActivity * activity();

};

#endif
