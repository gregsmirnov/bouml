#ifndef _FILEWRITER_H
#define _FILEWRITER_H


#include <qcstring.h>

class QFile;
class QTextStream;

// The original html generator was written in Java to test the Java 
// version of the API, this class allows to have a source code similar
// in C++
class FileWriter {
  public:
    bool open(QCString s);

    void close();

    void write(QCString s);

    void write(const char * s);

    void write(char c);

    void write(unsigned n);


  protected:
    QTextStream * ts;

    QFile * f;

};

#endif
