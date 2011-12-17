#ifndef _FILE_H
#define _FILE_H


#include <qstring.h>
#include <qcstring.h>
#include "aVisibility.h"
#include "Language.h"
#include <qdict.h>

#include <qfile.h>
enum { ATOM = 1, STRING = 2 };

class File : public QFile {
  public:
    File(QString s, QString here);

    bool open(int m);

    //// read a token in 's', returns its kind
    
    int read(QCString & s);

    //// e is expected, return TRUE if ok
    
    void read(const char * e);

    void unread(int k, const QCString & s);

    QCString context();


  protected:
    int readString(QCString & s);

    int readMLString(QCString & s);

    int readAtom(QCString & s);


  public:
    void syntaxError(const QCString s);

    void syntaxError(const QCString s, const QCString e);

    //// skip the end of the current block
    //// '(' already read
    void skipBlock();

    //// skip the next form
    
    void skipNextForm();

    void eof();

    void rewind();

    aVisibility readVisibility();

    bool readBool();

    Language readLanguage();

    int readDefinitionBeginning(QCString & s, QCString & id, QCString & ste, QCString & doc, QDict<QCString> & prop);

    void readProperties(QDict<QCString> & d);


  protected:
    int line_number;

    int unread_k;

    QCString unread_s;

};

#endif
