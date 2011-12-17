#ifndef _UMLRELATION_H
#define _UMLRELATION_H


#include "UmlBaseRelation.h"
#include <qcstring.h>

class FileOut;
class UmlClass;

// This class manages 'relations' between classes, notes that the class 'UmlClassItem'
// is a mother class of all the class's children.
//
// You can modify it as you want (except the constructor)
class UmlRelation : public UmlBaseRelation {
  public:
    UmlRelation(void * id, const QCString & n) : UmlBaseRelation(id, n), _assoc_class(0) {};

    //called inside class definition
    
    virtual void write(FileOut & out);

    //inside is true when the generation is done inside the class definition
    virtual void write(FileOut & out, bool inside);

    //write definition inside the class
    virtual void write_generalization(FileOut & out);

    //write definition outside the class
    virtual void write_dependency(FileOut & out);

    //write definition outside the class
    virtual void write_realization(FileOut & out);

    //write definition outside the class
    
    virtual void write_relation(FileOut & out);

    //write member ends
    
    virtual void write_ends(FileOut & out);

    //write definition inside the class
    virtual void write_relation_as_attribute(FileOut & out);

    //write definition inside the class
    virtual void write_extension(FileOut & out);

    virtual void search_class_assoc();


  protected:
    //the association class,
    //to replace association() and manage unconsistencies when the same class is part of several association classes
    UmlClass * _assoc_class;

};

#endif
