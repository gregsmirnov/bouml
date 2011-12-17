#ifndef _UMLACTIVITYDIAGRAM_H
#define _UMLACTIVITYDIAGRAM_H


#include "UmlBaseActivityDiagram.h"
#include "UmlActivityItem.h"
#include <qcstring.h>

// This class manages 'activity diagrams', notes that the class 'UmlDiagram'
// is a mother class of all the diagrams, allowing to generalize their
// management
//
// You can modify it as you want (except the constructor)
class UmlActivityDiagram : public UmlBaseActivityDiagram, public UmlActivityItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityDiagram(void * id, const QCString & s) : UmlBaseActivityDiagram(id, s) {
    }

};

#endif
