#ifndef _BINDING_H
#define _BINDING_H


#include <qlist.h>
#include <qcstring.h>

class FileIn;
class Token;
class UmlClass;

//Manages template actuals

class Binding {
  public:
    static void import(FileIn & in, Token & token, UmlClass * where);

    static void solveThem();


  protected:
    static QList<Binding> All;

    //xmiId of the class having the binbing
    QCString boundId;

    //xmiId of the formal
    QCString formalId;

    //xmiId of the actual, may be a class, or an opaque expression
    QCString actualId;

    //the actual if actualId is empty
    QCString actual;

    //xmiId of the signature
    
    QCString signatureId;

};

#endif
