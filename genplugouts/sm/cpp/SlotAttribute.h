#ifndef _SLOTATTRIBUTE_H
#define _SLOTATTRIBUTE_H


#include <qcstring.h>

class UmlAttribute;

struct SlotAttribute {
    UmlAttribute * attribute;

    QCString value;

    // because of QValueList use
     SlotAttribute() : attribute(0) {
    }

     SlotAttribute(UmlAttribute * a, QCString v) : attribute(a), value(v) {
    }

};

#endif
