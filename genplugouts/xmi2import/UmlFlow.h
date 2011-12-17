#ifndef _UMLFLOW_H
#define _UMLFLOW_H


#include "UmlBaseFlow.h"
#include "UmlActivityItem.h"
#include <qcstring.h>
#include <qvaluelist.h>

class FileIn;
class Token;
class UmlItem;

class UmlFlow : public UmlBaseFlow, public UmlActivityItem {
  public:
     UmlFlow(void * id, const QCString & n) : UmlBaseFlow(id, n) {
    }

    static void init();

    static void solveThem();

    //import the flow starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    struct Flow {
        QCString id;

        QCString name;

        bool interrupt;

        QCString source;

        QCString target;

        QCString selection;

        QCString transformation;

        QCString weight;

        QCString guard;

    };
    
    

  protected:
    static QValueList<Flow> All;

};

#endif
