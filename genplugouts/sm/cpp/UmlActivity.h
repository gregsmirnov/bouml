#ifndef _UMLACTIVITY_H
#define _UMLACTIVITY_H


#include "UmlBaseActivity.h"
#include <qcstring.h>

class UmlActivity : public UmlBaseActivity {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivity(void * id, const QCString & s) : UmlBaseActivity(id, s) {
    }

};

#endif
