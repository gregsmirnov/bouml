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
    FileOut(QFile * fp);

    void indent();

    void indent(int dir) { _indent += dir; }

    void id(const UmlItem * x);

    void idref(const UmlItem * x);

    void idref(QCString s, const UmlItem * x);

    void ref(const UmlItem * x);

    void idref_datatype(const QCString & t);

    void define_datatypes(int taggedvalue_mode);

    void quote(const char * s);


  protected:
    int _indent;

    QMap<QCString,int> _datatypes;

    QMap<QCString,int> _modifiedtypes;

};

#endif
