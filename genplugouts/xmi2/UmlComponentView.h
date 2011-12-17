#ifndef _UMLCOMPONENTVIEW_H
#define _UMLCOMPONENTVIEW_H


#include "UmlBaseComponentView.h"
#include <qcstring.h>
#include <qlist.h>

class FileOut;
class UmlItem;

// This class manages 'component view'
//
// You can modify it as you want (except the constructor)
class UmlComponentView : public UmlBaseComponentView {
  public:
    UmlComponentView(void * id, const QCString & n) : UmlBaseComponentView(id, n) {};

    virtual void write(FileOut & out);

    //Memorize the relation in the package (or the view in case the views
    //are generated as package) containing the current element, to generate
    //the relation at the package/view level
    
    virtual void memo_relation(UmlItem * r);


  protected:
    QList<UmlItem> _relations;

};

#endif
