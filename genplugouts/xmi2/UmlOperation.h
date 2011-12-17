#ifndef _UMLOPERATION_H
#define _UMLOPERATION_H


#include "UmlBaseOperation.h"
#include <qcstring.h>
#include <qptrdict.h>
#include <qasciidict.h>

class FileOut;

// This class manages 'operations', notes that the class 'UmlClassItem'
// is a mother class of all the class's children.
//
// You can modify it as you want (except the constructor)
class UmlOperation : public UmlBaseOperation {
  public:
    UmlOperation(void * id, const QCString & n) : UmlBaseOperation(id, n) {};

    virtual void write(FileOut & out);

    void write_exceptions(FileOut & out);

    void write_return_type(FileOut & out, QCString decl);

    void write_cpp_returntype(FileOut & out, QCString decl);

    void write_java_returntype(FileOut & out, QCString decl);

    void write_uml_params(FileOut & out);

    void write_cpp_java_params(FileOut & out, QCString decl);

    bool get_param(QCString s, int & index, QCString & r, QCString & kname, QCString & ktype, int & rank);

    // oper is sent/received in sequence/communication diagram, return event prefix
    // warning a class modify previous operation results
    const char * event(bool rec);

    // explicit operation/message sent in sequence/communication diagram, return event prefix
    static const char * event(const char * pfix, QCString msg);

    static void write_events(FileOut & out);


  protected:
    static int param_id;

    static QPtrDict<char> SentReceived;

    static QAsciiDict<char> Events;

};

#endif
