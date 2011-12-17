#ifndef _UMLACTIVITY_H
#define _UMLACTIVITY_H


#include "UmlBaseActivity.h"
#include "FlowContainer.h"
#include <qcstring.h>
#include <qmap.h>

class FileOut;
class UmlItem;

class UmlActivity : public UmlBaseActivity, public FlowContainer {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivity(void * id, const QCString & s) : UmlBaseActivity(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

    void write_condition(FileOut & out, QCString cond, bool pre);

    UmlItem * add_opaque_behavior(QCString beh, UmlItem * who, const char * k);

    UmlItem * add_opaque_expression(QCString val, UmlItem * who);


  private:
    struct Opaque {
        UmlItem * item;

        const char * kind;

        Opaque(){}

        Opaque(UmlItem * i, const char * k) : item(i), kind(k) {}

    };
    
    

  protected:
    QMap<QCString, Opaque> _opaque_behavior;

    QMap<QCString, UmlItem *> _opaque_expression;

};

#endif
