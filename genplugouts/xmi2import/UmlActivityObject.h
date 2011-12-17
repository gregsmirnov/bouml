#ifndef _UMLACTIVITYOBJECT_H
#define _UMLACTIVITYOBJECT_H


#include "UmlBaseActivityObject.h"
#include <qcstring.h>

class FileIn;
class Token;
class UmlItem;

class UmlActivityObject : public UmlBaseActivityObject {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityObject(void * id, const QCString & s) : UmlBaseActivityObject(id, s) {
    }

    static void init();

    //import the activity object starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);


  protected:
    void setType(QCString idref);

    void setType(Token & token);

    void setOrdering(QCString s, FileIn & in);

    void setSelection(QCString idref);

    void setInState(QCString s);

    //import the lower multiplicity starting by 'tk' inside 'where'
    void importMultiplicity(FileIn & in, Token & token, bool upper);

    //  finish to read activity object and sub classes definition
    
    void import_it(FileIn & in, Token & token);


  public:
    //  call at end of import : try to solve type or state
    virtual void solve(int context, QCString idref);

};

#endif
