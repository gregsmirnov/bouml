#ifndef _UMLITEM_H
#define _UMLITEM_H


#include "UmlBaseItem.h"
#include <qcstring.h>
#include "aLanguage.h"
#include "aVisibility.h"
#include "aDirection.h"
#include "aParameterEffectKind.h"
#include "anOrdering.h"
#include "Vector.h"
#include "FileWriter.h"

#include <qtextstream.h>
class UmlDiagram;
class UmlTypeSpec;

// This class is a mother class for all the other Uml* classes, this
// allows to generalize their management, declare virtual operations etc ...
//
// You can modify it as you want (except the constructor)
class UmlItem : public UmlBaseItem {
  public:
    UmlItem(void * id, const QCString & n) : UmlBaseItem(id, n), known(FALSE) { };

    virtual ~UmlItem();

    //returns a string indicating the king of the element
    virtual QCString sKind();

    //  Set the directory where the files will be generated
    //  initializing 'directory'.
    //
    //  May be remove already existing html files and css
    
    void set_dir(int argc, char ** argv);

    virtual void memo_ref();

    void define();

    static void start_file(QCString f, QCString s, bool withrefs);

    static void end_file();

    static void ref_indexes();

    static void generate_indexes();

    static void frame();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level) = 0;

    virtual bool chapterp();

    void html(QCString pfix, unsigned int rank, QCString what, unsigned int level, QCString kind);

    void html(const char * what, UmlDiagram * diagram);

    void write_children(QCString pfix, unsigned int rank, unsigned int level);

    void write_dependencies();

    void write_properties();

    void chapter(QCString k, QCString pfix, unsigned int rank, QCString kind, unsigned int level);

    //bypass the comments at the beginning of the form
    
    const char * bypass_comment(const char * s);

    void manage_alias(const char *& p);

    virtual void write();

    virtual void write(QCString target);

    static void writeq(QCString s);

    static void writeq(char c);

    static void write(const UmlTypeSpec & t, aLanguage lang);

    static void write(const UmlTypeSpec & t);

    static void write(aVisibility v, aLanguage lang);

    static void write(aVisibility v);

    static void write(aDirection d);

    static void write(aParameterEffectKind d);

    static void write(anOrdering d);

    static void generate_index(Vector & v, QCString k, QCString r);

    static void sort(Vector & v);


  private:
    static void sort(Vector & v, int low, int high);

    bool gt(UmlItem * other);


  public:
    virtual QCString pretty_name();


  protected:
    static Vector all;

    bool known;

    static FileWriter fw;

    static QCString directory;

    static unsigned int nrefs;

    static QCString letters;

    //true => use SVG picture rather than PNG
    static bool flat;

    //true => classes and tables are generated in index.html, else an own files are generated
    static bool svg;

};

#endif
