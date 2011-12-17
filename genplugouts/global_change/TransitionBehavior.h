#ifndef _TRANSITIONBEHAVIOR_H
#define _TRANSITIONBEHAVIOR_H


#include <qcstring.h>

class TransitionBehavior {
  public:
    QCString trigger;

    QCString guard;

    QCString activity;

    void read();

    void unload();

};

#endif
