#ifndef _UMLACTIVITYOBJECT_H
#define _UMLACTIVITYOBJECT_H


#include "UmlBaseActivityObject.h"
#include <qcstring.h>

class FileOut;

class UmlActivityObject : public UmlBaseActivityObject {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityObject(void * id, const QCString & s) : UmlBaseActivityObject(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

    virtual void solve_output_flows();


  protected:
    void write_ordering(FileOut & out);

    void write_in_state(FileOut & out);

    void write_selection(FileOut & out);

};

#endif
