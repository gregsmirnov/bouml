#ifndef _CLASSINSTANCE_H
#define _CLASSINSTANCE_H


#include <qlist.h>
#include <qcstring.h>
#include <qvaluelist.h>

class FileIn;
class Token;
class UmlItem;

class ClassInstance {
  public:
    static void init();

    //import the activity node starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    static void solveThem();


  protected:
    static QList<ClassInstance> All;

    struct Slot {
        //import the activity node starting by 'tk' inside 'where'
        void importIt(FileIn & in, Token & token);

        QCString featureId;

        QCString value;

        QCString valueId;

    };
    
    
    QValueList<Slot> bindings;

    QCString id;

    QCString name;

    QCString classifierId;

    UmlItem * where;

};

#endif
