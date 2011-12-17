#ifndef _UMLBUILTIN_H
#define _UMLBUILTIN_H


#include <qcstring.h>

//  Internal class used to memorize the types defined through the first 'generation settings' tab

struct UmlBuiltin {
    QCString uml;

#ifdef WITHCPP
    QCString cpp;

    QCString cpp_in;

    QCString cpp_out;

    QCString cpp_inout;
#endif

    QCString cpp_return;

#ifdef WITHJAVA
    QCString java;
#endif

#ifdef WITHIDL
    QCString idl;
#endif

};

#endif
