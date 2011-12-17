#ifndef _UMLPACKAGE_H
#define _UMLPACKAGE_H


#include "UmlBasePackage.h"
#include <qcstring.h>
#include <qlist.h>

class FileOut;
class UmlItem;
class UmlUseCaseDiagram;

// This class manages a 'package', notes that the project itself is a package
//
// You can modify it as you want (except the constructor)
class UmlPackage : public UmlBasePackage {
  public:
    UmlPackage(void * id, const QCString & n) : UmlBasePackage(id, n) {};

    virtual void xmi(int argc, char ** argv);

    virtual void write(FileOut & out);


  private:
    void search_profiles(QList<UmlPackage> & l);


  public:
    virtual void search_class_assoc();

    //Memorize the relation in the package (or the view in case the views
    //are generated as package) containing the current element, to generate
    //the relation at the package/view level
    
    virtual void memo_relation(UmlItem * r);

    //Memorize the use case diagram in the package (or the view in case the views
    //are generated as package) containing the current element, to generate
    //the association use case / actor at the package/view level
    
    virtual void memo_ac_uc_assoc(UmlUseCaseDiagram * d);


  protected:
    QList<UmlItem> _relations;

    QList<UmlUseCaseDiagram> _assocs;

};

#endif
