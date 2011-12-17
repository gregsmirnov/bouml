#ifndef _STATEBEHAVIOR_H
#define _STATEBEHAVIOR_H


#include <qcstring.h>

class StateBehavior {
  public:
    QCString on_entry;

    QCString on_exit;

    QCString do_activity;

    void read();

    void unload();

};

#endif
