#ifndef _SLOTRELATION_H
#define _SLOTRELATION_H


class UmlRelation;
class UmlClassInstance;

struct SlotRelation {
    UmlRelation * relation;

    UmlClassInstance * value;

    // because of QValueList use
     SlotRelation() : relation(0), value(0) {
    }

     SlotRelation(UmlRelation * r, UmlClassInstance * v) : relation(r), value(v) {
    }

};

#endif
