#ifndef _UC_H
#define _UC_H


#include <qcstring.h>

class UmlItem;
class File;

class Uc {
  public:
    //return the UmlItem implementing Uc
    
    virtual UmlItem * item() = 0;

    virtual void readObject(File & f, QCString s) = 0;

    void import(File & f);

    void readObjects(File & f);

    //Just to not have warning
    virtual ~Uc();

};

#endif
