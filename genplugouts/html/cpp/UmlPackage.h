#ifndef _UMLPACKAGE_H
#define _UMLPACKAGE_H


#include "UmlBasePackage.h"
#include <qcstring.h>
#include "Vector.h"

// This class manages a 'package', notes that the project itself is a package
//
// You can modify it as you want (except the constructor)
class UmlPackage : public UmlBasePackage {
  public:
    UmlPackage(void * id, const QCString & n) : UmlBasePackage(id, n) {};

    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //set the html ref
    //set the package list
    virtual void memo_ref();

    static void ref_index();

    static void generate_index();

    virtual bool chapterp();


  protected:
    static Vector packages;

};

#endif
