#ifndef _TRIGGER_H
#define _TRIGGER_H


#include <qcstring.h>
#include <qmap.h>

class FileIn;
class Token;
class UmlItem;

class Trigger {
  public:
    static void init();

    //import the trigger starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    //the result is null if not find
    
    static QCString get(QCString idref);

    static void add(FileIn & in, Token & token, QCString & name, QCString & idref);


  protected:
    static QMap<QCString, QCString> All;

};

#endif
