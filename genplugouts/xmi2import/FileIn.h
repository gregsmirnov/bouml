#ifndef _FILEIN_H
#define _FILEIN_H


#include <qstring.h>
#include <qcstring.h>
#include <qdict.h>

#include <stdio.h>
#include <qmap.h>
class Token;

//to read in the file and extract 'Token'
class FileIn {
  public:
    FileIn(const QString & path, FILE * fp);

    ~FileIn();

    Token & read(bool any = FALSE);

    //return content and the next token which must be a 'what'
    QCString body(QCString what);

    //return the next work, may be :
    //<
    ///
    //>
    //=
    //!
    //-
    //?
    //a string without "
    //an identifier (forced to lower case)
    //
    //doesn't return on eof
    //
    //Warning : returned value will be changed by next call !
    const char * readWord(bool any, BooL & str);

    void finish(QCString what);

    void bypass(Token & tk);

    void bypassedId(Token & tk);

    //doesn't return
    void error(QCString s);

    void warning(QCString s);

    const QCString & path() const { return _path; }

    static bool isBypassedId(QCString id) {
      return BypassedIds[QString(id)] != 0;
    }


  protected:
    QCString _path;

    FILE * _fp;

    bool _utf8;

    int _linenum;

    int _length;

    char * _buffer;

    QMap<QCString, char> _special_chars;

    static QDict<char> BypassedIds;


  private:
    const char * read_word(int c, bool any);

    const char * read_string(int marker);

    char read_special_char();


  public:
    void setEncoding(QCString s);

};

#endif
