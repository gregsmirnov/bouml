#ifndef _UMLACTIVITY_H
#define _UMLACTIVITY_H


#include "UmlBaseActivity.h"
#include <qcstring.h>
#include "Vector.h"

class UmlActivity : public UmlBaseActivity {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivity(void * id, const QCString & s) : UmlBaseActivity(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //set the html ref
    //set the activity list
    virtual void memo_ref();

    static void ref_index();

    static void generate_index();

    virtual bool chapterp();


  protected:
    static Vector activities;

};

#endif
