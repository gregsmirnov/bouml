#ifndef _UMLITEM_H
#define _UMLITEM_H


#include <qmap.h>
#include "UmlBaseItem.h"
#include <qcstring.h>
#include "Language.h"
#include <qlist.h>

class FileOut;
class FileOut;
class UmlUseCaseDiagram;
class UmlTypeSpec;

// This class is a mother class for all the other Uml* classes, this
// allows to generalize their management, declare virtual operations etc ...
//
// You can modify it as you want (except the constructor)
class UmlItem : public UmlBaseItem {
  public:
    UmlItem(void * id, const QCString & n)
      : UmlBaseItem(id, n), _written(FALSE) {};

    virtual ~UmlItem();

    virtual void xmi(int argc, char ** argv);

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

    //inside is true when the generation is done inside the parent definition
    //does nothing at this level
    virtual void write(FileOut & out, bool inside);

    void write_description_properties(FileOut & out);

    virtual void search_class_assoc();

    //Memorize the relation in the package (or the view in case the views
    //are generated as package) containing the current element, to generate
    //the relation at the package/view level
    
    virtual void memo_relation(UmlItem * r);

    //Memorize the use case diagram in the package (or the view in case the views
    //are generated as package) containing the current element, to generate
    //the association use case / actor at the package/view level
    
    virtual void memo_ac_uc_assoc(UmlUseCaseDiagram * d);

    static void write_multiplicity(FileOut & out, QCString s, UmlItem * who);

    //if the type is specified write it in a form "<type xmi:type=... />"
    //except if tk is given and replace "type"
    static void write_type(FileOut & out, const UmlTypeSpec & t, const char * tk = 0);

    static void write_default_value(FileOut & out, QCString v, UmlItem * who, int rank = -1);

    static void write_stereotyped(FileOut & out);

    static bool gen_extension();


  protected:
    bool _written;

    static bool _gen_views;

    static Language _lang;

    static bool _uml_20;

    static bool _pk_prefix;

    static bool _vis_prefix;

    static bool _primitive_type;

    static bool _linefeed;

    static bool _gen_extension;

    static bool _gen_eclipse;

    static QMap<QCString, QList<UmlItem> > _stereotypes;

};

#endif
