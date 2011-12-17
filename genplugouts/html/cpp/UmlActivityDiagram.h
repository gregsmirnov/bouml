#ifndef _UMLACTIVITYDIAGRAM_H
#define _UMLACTIVITYDIAGRAM_H


#include <qcstring.h>
#include "Vector.h"
#include "UmlBaseActivityDiagram.h"
#include "UmlActivityItem.h"

// This class manages 'activity diagrams', notes that the class 'UmlDiagram'
// is a mother class of all the diagrams, allowing to generalize their
// management
//
// You can modify it as you want (except the constructor)
class UmlActivityDiagram : public UmlBaseActivityDiagram, public UmlActivityItem {
  public:
    //set he html ref
    //set the diagrams list
    virtual void memo_ref();

    //returns a string indicating the king of the element
    virtual QCString sKind();

    static void ref_index();

    static void generate_index();


  protected:
    static Vector diagrams;


  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityDiagram(void * id, const QCString & s) : UmlBaseActivityDiagram(id, s) {
    }

};

#endif
