#ifndef _SGNL_H
#define _SGNL_H


#include <qcstring.h>
#include <qmap.h>

class FileIn;
class Token;
class UmlItem;

class Signal {
  public:
    static void init();

    //import the signal starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    //the result is null if not find
    
    static QCString get(QCString idref);

    static void add(FileIn & in, Token & token, QCString & name, QCString & idref);


  protected:
    static QMap<QCString, QCString> All;

};

#endif
