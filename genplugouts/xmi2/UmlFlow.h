#ifndef _UMLFLOW_H
#define _UMLFLOW_H


#include "UmlBaseFlow.h"
#include "UmlActivityItem.h"
#include <qcstring.h>
#include "ControlOrData.h"

class FileOut;

class UmlFlow : public UmlBaseFlow, public UmlActivityItem {
  public:
     UmlFlow(void * id, const QCString & n) : UmlBaseFlow(id, n), _control_or_data(Unset) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

    //goes among tree to memorize incoming flow :
    //the target memorise the current flow
    
    virtual void memo_incoming_flow();

    void write_it(FileOut & out);


  private:
    bool is_control_flow();

    ControlOrData _control_or_data;


  public:
    ControlOrData control_or_data() const { return _control_or_data; }

    void set_control_or_data(ControlOrData k);

};

#endif
