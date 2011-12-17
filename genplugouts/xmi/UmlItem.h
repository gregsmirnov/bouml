#ifndef _UMLITEM_H
#define _UMLITEM_H


#include "UmlBaseItem.h"
#include <qcstring.h>
#include "Language.h"

class FileOut;
class FileOut;

// This class is a mother class for all the other Uml* classes, this
// allows to generalize their management, declare virtual operations etc ...
//
// You can modify it as you want (except the constructor)
class UmlItem : public UmlBaseItem {
  public:
    UmlItem(void * id, const QCString & n)
      : UmlBaseItem(id, n), _written(FALSE) {};

    virtual ~UmlItem();

    virtual void xmi(int argc, char ** argv);

    virtual bool write_if_needed(FileOut & out);

    virtual void write(FileOut & out);

    void write_stereotype(FileOut & out);

    void write_description_properties(FileOut & out);

    virtual void ref(FileOut & out);


  protected:
    bool _written;

    static bool _gen_views;

    static Language _lang;

    static int _taggedvalue_mode;

};

#endif
