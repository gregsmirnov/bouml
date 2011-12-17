
#include "FileOut.h"
#include "UmlItem.h"

#include <qtextstream.h>
#include <qfile.h>
FileOut::FileOut(QFile * fp, bool lf, bool utf8) : QTextStream(fp), _lf(lf), _indent(0){
  if (utf8)
    setEncoding(QTextStream::UnicodeUTF8);
}

void FileOut::indent() {
  QTextStream & ts = *this;
  
  for (int n = _indent; n > 0; n -= 1)
    ts << '\t';
}

void FileOut::id(UmlItem * x) {
  ((QTextStream &) *this) << " xmi:id=\"BOUML_" << ((void *) x->getIdentifier()) << "_" << x->kind() << '"';

}

void FileOut::id_prefix(UmlItem * x, const char * pfix) {
  ((QTextStream &) *this) << " xmi:id=\"BOUML_" << pfix << ((void *) x->getIdentifier()) << "_" << x->kind() << '"';
}

void FileOut::id_prefix(UmlItem * x, const char * pfix, int n) {
  ((QTextStream &) *this) << " xmi:id=\"BOUML_" << pfix << n << "_" << ((void *) x->getIdentifier()) << "_" << x->kind() << '"';
}

void FileOut::idref(UmlItem * x) {
  ((QTextStream &) *this) << " xmi:idref=\"BOUML_" << ((void *) x->getIdentifier()) << "_" << x->kind() << '"';

}

void FileOut::idref(QCString s, UmlItem * x) {
  QString keys;
  {
    QTextStream keyst(&keys, IO_WriteOnly);
    
    keyst << ((void *) x) << "_" << s;
  }

  QMap<QCString, int>::ConstIterator it =
    _modifiedtypes.find((const char *) keys);
  
  if (it == _modifiedtypes.end())
    it = _modifiedtypes.insert((const char *) keys, _modifiedtypes.count());
    
  ((QTextStream &) *this) << " xmi:idref=\"BOUML_basedontype_"
    << it.data() << '"';

}

void FileOut::idref_prefix(UmlItem * x, const char * pfix) {
  ((QTextStream &) *this) << " xmi:idref=\"BOUML_"
	  << pfix << ((void *) x->getIdentifier()) << "_" << x->kind() << '"';

}

void FileOut::idref_prefix(UmlItem * x, const char * pfix, int n) {
  ((QTextStream &) *this) << " xmi:idref=\"BOUML_"
	  << pfix << n << "_" << ((void *) x->getIdentifier()) << "_" << x->kind() << '"';

}

void FileOut::idref_datatype(const QCString & t) {
  if (t.isEmpty())
    return;
    
  QMap<QCString, int>::ConstIterator it = _datatypes.find(t);
  
  if (it == _datatypes.end())
    it = _datatypes.insert(t, _datatypes.count());
    
  ((QTextStream &) *this) << " xmi:idref=\"BOUML_datatype_"
    << it.data() << '"';

}

void FileOut::ref(UmlItem * x, const char * pfix1, const char * pfix2) {
  ((QTextStream &) *this) << ' ' << pfix1 << "=\"BOUML_" 
	  << pfix2 << ((void *) x->getIdentifier()) << "_" << x->kind() << '"';
}

void FileOut::ref(UmlItem * x, const char * pfix1, const char * pfix2, int n) {
  ((QTextStream &) *this) << ' ' << pfix1 << "=\"BOUML_" 
	  << pfix2 << n << "_" << ((void *) x->getIdentifier()) << "_" << x->kind() << '"';
}

void FileOut::ref_only(UmlItem * x, const char * pfix) {
  ((QTextStream &) *this) << "BOUML_" << pfix << ((void *) x->getIdentifier())
    << "_" << x->kind();
}

void FileOut::define_datatypes(bool uml_20, bool primitive_type, bool gen_extension) {
  const char * pfix = (primitive_type) 
	? ((uml_20) ? "<ownedMember xmi:type=\"uml:PrimitiveType\""
		    : "<packagedElement xmi:type=\"uml:PrimitiveType\"")
	: ((uml_20) ? "<ownedMember xmi:type=\"uml:DataType\""
		    : "<packagedElement xmi:type=\"uml:DataType\"");

  QMap<QCString, int>::ConstIterator it;
  
  for (it = _datatypes.begin();
       it != _datatypes.end();
       ++it) {
    indent();
    (*this) << pfix << " xmi:id=\"BOUML_datatype_"
      << it.data() << "\" name=\"";
    quote(it.key());
    (*this) << "\"/>\n";
  }
  
  const char * postfix = 
    (uml_20) ? "</ownedMember>\n" : "</packagedElement>\n";

  for (it = _modifiedtypes.begin();
       it != _modifiedtypes.end();
       ++it) {
    QCString k = it.key();
    int index = k.find('_');
    
    indent();
    (*this) << pfix << " xmi:id=\"BOUML_basedontype_"
      << it.data() << "\" name = \"";
    quote(k.mid(index + 1));
    (*this) << '"';

    if (gen_extension) {
      (*this) << ">\n";
      indent();
      (*this) << "\t<xmi:Extension extender=\"Bouml\">\n";
      indent();
      (*this) << "\t\t<basedOn \"BOUML_" << k.left(index) << "\"/>\n";
      indent();
      (*this) << "\t</xmi:Extension>\n";
      indent();
      (*this) << postfix;
    }
    else
      (*this) << "/>\n";
  }
}

void FileOut::quote(const char * s) {
 for (;;) {
   switch (*s) {
   case 0: return;
   case '<': (*this) << "&lt;"; break;
   case '>': (*this) << "&gt;"; break;
   case '"': (*this) << "&quot;"; break;
   case '&': (*this) << "&amp;"; break;
   case '\n': if (_lf) (*this) << *s; else (*this) << "&#10;"; break;
   case '\r': if (_lf) (*this) << *s; else (*this) << "&#13;"; break;
   default: (*this) << *s; break;
   }
   s += 1;
 }
}

void FileOut::quote(char c) {
 switch (c) {
 case '<': (*this) << "&lt;"; break;
 case '>': (*this) << "&gt;"; break;
 case '"': (*this) << "&quot;"; break;
 case '&': (*this) << "&amp;"; break;
 case '\n': if (_lf) (*this) << c; else (*this) << "&#10;"; break;
 case '\r': if (_lf) (*this) << c; else (*this) << "&#13;"; break;
 default: (*this) << c; break;
 }
}

