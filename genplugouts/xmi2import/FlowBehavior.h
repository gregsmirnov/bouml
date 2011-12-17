#ifndef _FLOWBEHAVIOR_H
#define _FLOWBEHAVIOR_H


#include <qcstring.h>

class FlowBehavior {
  public:
    QCString weight;

    QCString guard;

    QCString selection;

    QCString transformation;

    void read();

    void unload();

};

#endif
