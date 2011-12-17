#ifndef _UMLSTEREOTYPE_H
#define _UMLSTEREOTYPE_H


#include <qcstring.h>

//  Internal class used to know how what code is produced for a stereotype in each language

struct UmlStereotype {
    QCString uml;

#ifdef WITHCPP
    QCString cpp;
#endif

#ifdef WITHJAVA
    QCString java;
#endif

#ifdef WITHIDL
    QCString idl;
#endif

};

#endif
