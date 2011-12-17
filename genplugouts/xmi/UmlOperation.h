#ifndef _UMLOPERATION_H
#define _UMLOPERATION_H


#include "UmlBaseOperation.h"
#include <qcstring.h>

class FileOut;

// This class manages 'operations', notes that the class 'UmlClassItem'
// is a mother class of all the class's children.
//
// You can modify it as you want (except the constructor)
class UmlOperation : public UmlBaseOperation {
  public:
    UmlOperation(void * id, const QCString & n) : UmlBaseOperation(id, n) {};

    virtual bool write_if_needed(FileOut & out);

    void write_return_type(FileOut & out, QCString decl);

    void write_cpp_returntype(FileOut & out, QCString decl);

    void write_java_returntype(FileOut & out, QCString decl);

    void write_uml_params(FileOut & out);

    void write_cpp_java_params(FileOut & out, QCString decl);

    bool get_param(QCString s, int & index, QCString & r, QCString & kname, QCString & ktype, int & rank);


  protected:
    static int param_id;

};

#endif
