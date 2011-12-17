#ifndef _UMLFRAGMENT_H
#define _UMLFRAGMENT_H


#include "UmlBaseFragment.h"
#include <qlist.h>

class FileOut;
class UmlItem;
class UmlSequenceMessage;
class UmlClassInstanceReference;

// this class manages fragments, you can modify it
class UmlFragment : public UmlBaseFragment {
  public:
    //
    
    void write(FileOut & out, UmlItem * diagram, QList<UmlSequenceMessage> & msgs);

    void cover(UmlSequenceMessage * msg);


  private:
    //
    
    void write_ref(FileOut & out, UmlItem * diagram, QList< UmlSequenceMessage > & msgs);

    QList<UmlClassInstanceReference> covered;

};

#endif
