
#include "File.h"

#include <stdlib.h>
#include <qinputdialog.h>
#include <qdict.h>
#include <qfileinfo.h> 
#include <qdir.h>
#include "UmlCom.h"

File::File(QString s, QString here) {
  for (;;)  {
    int index = 0;
    
    while ((index = s.find('\\', index)) != -1)
      s.replace(index++, 1, "/");

    int index0;
    
    if ((index0 = s.find('$')) == -1)
      break;
    
    if ((index = s.find('/', index0 + 1)) == -1)
      index = s.length();

    static QDict<char> alias;

    QString var = s.left(index);
    const char * val = alias[var];

    if (val == 0) {
      if ((val = getenv(var.mid(1))) == 0) {
	QString v = QInputDialog::getText("Unknown environment variable",
					  var.mid(1) + " : ");
      
	if (v.isEmpty())
	  throw 0;
	
	val = strdup((const char *) v);
      }
      else
	val = strdup(val);
      
      alias.insert(var, val);
    }

    if (*val == '&') {
      QFileInfo fi(here);
      
      s.replace(index0, index - index0, fi.dirPath(TRUE));
    }
    else
      s.replace(index0, index - index0, val);
  }

  setName(QDir::cleanDirPath(s));
  
  unread_k = -1;

}

bool File::open(int m) {
  line_number = 1;

  return QFile::open(m);
}

int File::read(QCString & s) {
  if (unread_k != -1) {
    s = unread_s;
    
    int r = unread_k;
    
    unread_k = -1;
    return r;
  }

  s = "";

  for (;;) {
    int c = getch();

    switch (c) {
    case '(':
    case ')':
      s += c;	// s = c does not work
      return c;
    case -1:
      s = "<eof>";
      return c;
    case '\n':
      line_number += 1;
      // no break
    case ' ':
    case '\r':
    case '\t':
      break;
    case '"':
      return readString(s);
    case '|':
      return readMLString(s);
    default:
      s += c;	// s = c does not work
      return readAtom(s);
    }
  }
}

void File::read(const char * e) {
  QCString s;

  if (read(s) == -1)
    eof();

  if (s != e)
    syntaxError(s, e);
}

void File::unread(int k, const QCString & s) {
 unread_k = k;
 unread_s = s;
}

QCString File::context() {
  QCString s;

  return QCString(name()) + " line " + s.setNum(line_number);
}

int File::readString(QCString & s) {
  int c;

  while ((c = getch()) != '"') {
    if (c == -1) {
      UmlCom::trace("<br>end of file reading a string in " + context());
      throw 0;
    }

    s += ((c == '\\') ? getch() : c);
  }

  return STRING;
}

int File::readMLString(QCString & s) {
  for (;;) {
    int c = getch();

    switch (c) {
    case -1:
      return STRING;
    case '\n':
      line_number += 1;
      c = getch();

      if (c == '\r')
        c = getch();

      switch (c) {
      case -1:
        return STRING;
      case '|':
        s += '\n';
        break;
      default:
        ungetch(c);
        return STRING;
      }
    case '\r':
      break;
    default:
      s += ((c == '\\') ? getch() : c);
    }
  }
}

int File::readAtom(QCString & s) {
  for (;;) {
    int c = getch();

    switch (c) {
    case '(':
    case ')':
    case '"':
    case '\n':
      ungetch(c);
      // no break !
    case -1:
    case ' ':
    case '\r':
    case '\t':
      return ATOM;
    default:
      s += c;
    }
  }
}

void File::syntaxError(const QCString s) {
  UmlCom::trace("<br>syntax error near '" + s + "' in " + context());
  throw 0;
}

void File::syntaxError(const QCString s, const QCString e) {
  QCString msg = "<br>'" + e + "' expected rather than '" + s + "' in " + context();
  
  UmlCom::trace(msg);
  throw 0;
}

void File::skipBlock() {
  int lvl = 1;
  QCString s;

  for(;;) {
    switch (read(s)) {
    case -1:
      return;
    case '(':
      lvl += 1;
      break;
    case ')': 
      if (--lvl == 0)
        return;
    default:
      break;
    }
  }

}

void File::skipNextForm() {
  QCString s;

  switch (read(s)) {
  case ')':
    ungetch(')');
    break;
  case '(':
    skipBlock();
    break;
  default:
    break;
  }
    
}

void File::eof() {
  UmlCom::trace("<br>premature end of file in " + name());
}

void File::rewind() {
  line_number = 1;
  at(0);
  unread_k = -1;
}

aVisibility File::readVisibility() {
  QCString s;
 
  if (read(s) == STRING) {
    if (s == "Private")
      return PrivateVisibility;
    if (s == "Protected")
      return ProtectedVisibility;
    if (s == "Public")
      return PublicVisibility;
  }

  //syntaxError(s, "a visibility");
  return PublicVisibility;
}

bool File::readBool() {
  QCString s;
 
  if (read(s) == ATOM) {
    if (s == "TRUE")
      return TRUE;
    if (s == "FALSE")
      return FALSE;
  }

  syntaxError(s, "TRUE or FALSE");
  return FALSE;	// to avoid warning
}

Language File::readLanguage() {
  QCString s;
  
  if (read(s) != STRING)
    syntaxError(s, "a language");
    
  if (s == "C++")
    return Cplusplus;
  if (s == "ANSI C++")
    return AnsiCplusplus;
  if (s == "VC++")
    return VCplusplus;
  if (s == "CORBA")
    return Corba;
  if (s == "Oracle8")
    return Oracle8;
  if (s == "Java")
    return Java;
  
  return None;
}

int File::readDefinitionBeginning(QCString & s, QCString & id, QCString & ste, QCString & doc, QDict<QCString> & prop) {
  for (;;) {
    int k = read(s);
    
    switch(k) {
    case -1:
      eof();
      throw 0;
    case ')':
      return k;
    case '(':
      skipBlock();
      // no break !
    default:
      continue;
    case ATOM:
      break;
    }

    if (s == "quid") {
      if (read(id) != STRING)
	syntaxError(id, "quid value");
    }
    else if (s == "attributes") {
      readProperties(prop);
    }
    else if (s == "documentation") {
      if (read(doc) != STRING)
	syntaxError(doc, "the documentation");
    }
    else if (s == "stereotype") {
      if (read(ste) != STRING)
	syntaxError(ste, "the stereotype");
    }
    else
      return k;
  }
}

void File::readProperties(QDict<QCString> & d) {
  d.setAutoDelete(TRUE);
  
  read("(");
  read("list");
  read("Attribute_Set");
  
  for (;;) {
    QCString s;
  
    switch (read(s)) {
    case ')':
      return;
    case '(':
      read("object");
      read("Attribute");
      read("tool");
      
      if (read(s) != STRING)
	syntaxError(s, "the tool");
      
      read("name");
      {
	QCString s2;
	
	if (read(s2) != STRING)
	  syntaxError(s2, "the name");
	
	s += "/" + s2;
	
	read("value");
	
	switch (read(s2)) {
	case '(':
	  if ((read(s2) == ATOM) && (s2 == "value")) {
	    read("Text");
	    if (read(s2) != STRING)
	      syntaxError(s2, "the value");
	    read(")");
	  }
	  else
	    skipBlock();
	  // no break !
	case STRING:
	case ATOM:
	  d.insert(s, new QCString(s2));
	  break;
	default:
	  syntaxError(s, "the value");
	}
      }
      
      read(")");
      break;
    default:
      syntaxError(s);
    }
  }

}

