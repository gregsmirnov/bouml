
#include "UmlItem.h"
#include "FileOut.h"
#include "UmlUseCaseDiagram.h"
#include "UmlTypeSpec.h"

#include "UmlCom.h"
#include "UmlClass.h"

 UmlItem::~UmlItem() {
}

void UmlItem::xmi(int, char **) {
  UmlCom::trace("Error : must be applied on the project<br>");
}

void UmlItem::write(FileOut & out) {
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);

}

void UmlItem::write(FileOut &, bool) {
}

void UmlItem::write_description_properties(FileOut & out) {
  if (! description().isEmpty()) {
    static int rank = 0;
    
    out.indent();
    out << "<ownedComment xmi:type=\"uml:Comment\" xmi:id=\"COMMENT_"
      << ++rank << "\" body=\"";
    out.quote(description());
    out << "\"/>\n";
  }

  QCString ste = stereotype();
  
  if (_gen_extension) {
    const QDict<QCString> up = properties();    
    QDictIterator<QCString> it(up);
    
    if (it.current()) {
      out.indent();
      out << "<xmi:Extension extender=\"Bouml\">\n";
      
      if (! ste.isEmpty()) {
	out.indent();
	out << "\t<stereotype name=\"";
	out.quote(ste);
	out << "\"/>\n";
      }
      
      do {
	out.indent();
	out << "\t<taggedValue tag=\"";
	out.quote(it.currentKey());
	out << "\" value=\"";
	out.quote(*(it.current()));
	out << "\"/>\n";
	++it;
      } while (it.current());
      
      out.indent();
      out << "</xmi:Extension>\n";
    }
    else if (! ste.isEmpty()) {
      out.indent();
      out << "<xmi:Extension extender=\"Bouml\"><stereotype name=\"";
      out.quote(ste);
      out << "\"/></xmi:Extension>\n";
    }
  } 
  
  if (ste.contains(':') == 1)
    // probably a stereotype part of profile
    _stereotypes[ste].append(this);
}

void UmlItem::search_class_assoc() {
}

void UmlItem::memo_relation(UmlItem * r) {
  parent()->memo_relation(r);
}

void UmlItem::memo_ac_uc_assoc(UmlUseCaseDiagram * d) {
  parent()->memo_ac_uc_assoc(d);
}

void UmlItem::write_multiplicity(FileOut & out, QCString s, UmlItem * who)
{
  if (!s.isEmpty()) {
    QCString min;
    QCString max;
    int index = s.find("..");
    
    if (index != -1) {
      min = s.left(index).stripWhiteSpace();
      max = s.mid(index+2).stripWhiteSpace();
    }
    else
      min = max = s.stripWhiteSpace();
    
    out.indent();
    out << "<lowerValue xmi:type=\"uml:LiteralString\"";
    out.id_prefix(who, "MULTIPLICITY_L_");
    out << " value=\"" << min << "\"/>\n";
    
    out.indent();
    out << "<upperValue xmi:type=\"uml:LiteralString\"";
    out.id_prefix(who, "MULTIPLICITY_U_");
    out << " value=\"" << max << "\"/>\n";
  }
}

void UmlItem::write_type(FileOut & out, const UmlTypeSpec & t, const char * tk)
{
  if (t.type != 0) {
    out.indent();
    out << '<' << ((tk != 0) ? tk : "type") << " xmi:type=\"uml:Class\"";
    out.idref(t.type);
    out << "/>\n";
  }
  else if (!t.explicit_type.isEmpty()) {
    out.indent();
    out << '<' << ((tk != 0) ? tk : "type") << " xmi:type=\"uml:";
    
    if (t.explicit_type == "int")
      out << ((_uml_20)
	      ? "PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.0/uml.xml#Integer\"/>\n"
	      : "PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.1/uml.xml#Integer\"/>\n");
    else if (t.explicit_type == "bool")
      out << ((_uml_20)
	      ? "PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.0/uml.xml#Boolean\"/>\n"
	      : "PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.1/uml.xml#Boolean\"/>\n");
    else if (t.explicit_type == "string")
      out << ((_uml_20)
	      ? "PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.0/uml.xml#String\"/>\n"
	      : "PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.1/uml.xml#String\"/>\n");
    else if (t.explicit_type == "long")
      out << ((_uml_20)
	      ? "PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.0/uml.xml#UnlimitedNatural\"/>\n"
	      : "PrimitiveType\" href=\"http://schema.omg.org/spec/UML/2.1/uml.xml#UnlimitedNatural\"/>\n");
    else {
      out << "Class\"";
      out.idref_datatype(t.explicit_type);
      out << "/>\n";
    }
  }

}

void UmlItem::write_default_value(FileOut & out, QCString v, UmlItem * who, int rank)
{
  if (! v.isEmpty()) {
    if (v[0] == '=') {
      v = v.mid(1);
      if (v.isEmpty())
	return;
    }

    out.indent();
    out << "<defaultValue xmi:type=\"uml:LiteralString\"";
    if (rank == -1)
      out.id_prefix(who, "VALUE_");
    else
      out.id_prefix(who, "VALUE", rank);
    out << " value=\"";
    out.quote(v);
    out << "\"/>\n";
  }
}

void UmlItem::write_stereotyped(FileOut & out)
{
  QMap<QCString, QList<UmlItem> >::Iterator it;
  
  for (it = _stereotypes.begin(); it != _stereotypes.end(); ++it) {
    const char * st = it.key();
    UmlClass * cl = UmlClass::findStereotype(it.key(), TRUE);
		     
    if (cl != 0) {
      QValueList<QCString> extended;

      cl->get_extended(extended);
      
      QList<UmlItem> & l = it.data();
      UmlItem * elt;
      
      for (elt = l.first(); elt != 0; elt = l.next()) {
	out << "\t<" << st;
	out.id_prefix(elt, "STELT_");
	
	const QDict<QCString> props = elt->properties();
	QDictIterator<QCString> itp(props);
	
	while (itp.current()) {
	  QString k = itp.currentKey();
	  
	  if (k.contains(':') == 2) {
	    out << ' ';
	    out.quote(k.mid(k.findRev(':') + 1));
	    out << "=\"";
	    out.quote(*itp.current());
	    out << '"';
	  }
	  ++itp;
	}
	
	QValueList<QCString>::Iterator iter_extended;
	
	for (iter_extended = extended.begin(); 
	     iter_extended != extended.end();
	     ++iter_extended) {
	  QCString vr = "base_" + *iter_extended;
	  
	  out.ref(elt, vr);
	}
	
	out << "/>\n";
	
	elt->unload();
      }
    }
  }

}

bool UmlItem::gen_extension()
{
  return _gen_extension;
}

bool UmlItem::_gen_views;

Language UmlItem::_lang;

bool UmlItem::_uml_20;

bool UmlItem::_pk_prefix;

bool UmlItem::_vis_prefix;

bool UmlItem::_primitive_type;

bool UmlItem::_linefeed;

bool UmlItem::_gen_extension;

bool UmlItem::_gen_eclipse;

QMap<QCString, QList<UmlItem> > UmlItem::_stereotypes;

