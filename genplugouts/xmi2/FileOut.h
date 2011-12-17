#ifndef _FILEOUT_H
#define _FILEOUT_H


#include <qtextstream.h>
#include <qmap.h>
#include <qcstring.h>

class QFile;
class UmlItem;

//QTextOStream + usefull operations
class FileOut : public QTextStream {
  public:
    FileOut(QFile * fp, bool lf, bool utf8);

    void indent();

    void indent(int dir) { _indent += dir; }

    void id(UmlItem * x);

    void id_prefix(UmlItem * x, const char * pfix);

    void id_prefix(UmlItem * x, const char * pfix, int n);

    void idref(UmlItem * x);

    //for C++/Java types
    void idref(QCString s, UmlItem * x);

    void idref_prefix(UmlItem * x, const char * pfix);

    void idref_prefix(UmlItem * x, const char * pfix, int n);

    void idref_datatype(const QCString & t);

    //output <space><pfix1>="BOUML_<pfix2><x>"
    void ref(UmlItem * x, const char * pfix1, const char * pfix2 = "");

    //output <space><pfix1>="BOUML_<pfix2><n><x>"
    void ref(UmlItem * x, const char * pfix1, const char * pfix2, int n);

    //output BOUML_<pfix><x>
    void ref_only(UmlItem * x, const char * pfix);

    void define_datatypes(bool uml_20, bool primitive_type, bool gen_extension);

    void quote(const char * s);

    void quote(char c);


  protected:
    bool _lf;

    int _indent;

    QMap<QCString,int> _datatypes;

    QMap<QCString,int> _modifiedtypes;

};

#endif
