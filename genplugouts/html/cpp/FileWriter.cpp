
#include "FileWriter.h"

#include <qtextstream.h>
#include <qfile.h>

bool FileWriter::open(QCString s) {
  f = new QFile(s);
  
  if (! f->open(IO_WriteOnly)) {
    delete f;
    return FALSE;
  }
    
  ts = new QTextStream(f);
  return TRUE;
}

void FileWriter::close() {
  f->flush();
  f->close();
  delete ts;
  delete f;
  f = 0;
  ts = 0;
}

void FileWriter::write(QCString s) {
  ts->writeRawBytes(s, s.length());
}

void FileWriter::write(const char * s) {
  ts->writeRawBytes(s, strlen(s));
}

void FileWriter::write(char c) {
 ts->writeRawBytes(&c, 1);
}

void FileWriter::write(unsigned n) {
  QCString s;
  
  s.setNum(n);
  
  ts->writeRawBytes(s, s.length());

}

