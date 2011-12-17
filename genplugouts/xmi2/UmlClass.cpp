
#include "UmlClass.h"
#include "FileOut.h"

#include "UmlRelation.h"
#include "UmlCom.h"
void UmlClass::write(FileOut & out) {
  QCString st = stereotype();
  
  if (st == "metaclass")
    return;
  
  bool is_actor = (st == "actor");
  bool is_enum = (st == "enum");
  bool is_stereotype =
    (st == "stereotype") && 
    (parent()->parent()->kind() == aPackage) &&
    (parent()->parent()->stereotype() == "profile");
  
  if (!is_actor) {
    switch (_lang) {
    case Cpp:
      if (cppDecl().isEmpty())
	return;
      break;
    case Java:
      if (javaDecl().isEmpty())
	return;
    default:
      break;
    }
  }

  const char * k = (parent()->kind() == aClass)
    ? "nestedClassifier"
    : ((!_uml_20)
       ? "packagedElement"
       : ((is_stereotype) ? "ownedStereotype" : "ownedMember"));
  bool is_assoc_class = (_assoc != 0);
  
  out.indent();
  out << "<" << k << " xmi:type=\"uml:"
    << ((is_actor) 
	? "Actor"
	: ((is_assoc_class)
	   ? "AssociationClass"
	   : ((st == "interface")
	      ? "Interface" 
	      : ((is_enum)
		 ?"Enumeration"
		 : ((is_stereotype) ? "Stereotype" : "Class")))))
    << "\" name=\"";
  out.quote(name());
  out << '"';
  out.id(this);
  write_visibility(out);
  if (isAbstract())
    out << " isAbstract=\"true\"";
  if (isActive())
    out << " isActive=\"true\"";
  out << ">\n";
  
  if (is_assoc_class)
    _assoc->write_ends(out);
    
  out.indent(+1);
  
  write_constraint(out);
  write_annotation(out);
  write_description_properties(out);
  
  if (_gen_extension && (st == "typedef")) {
    const UmlTypeSpec & base = baseType();
    
    if ((base.type != 0) || !base.explicit_type.isEmpty()) {
      out.indent();
      out << "<xmi:Extension extender=\"Bouml\">\n";
      out.indent();
      out << "\t<typedef>\n";
      out.indent(+2);
      UmlItem::write_type(out, base, "base");
      out.indent(-2);
      out.indent();
      out << "\t</typedef>\n";
      out.indent();
      out << "</xmi:Extension>\n";
    }
  }
  
  write_formals(out);
  write_actuals(out);
  
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  unsigned i;
  
  for (i = 0; i != n; i += 1)
    ch[i]->write(out);
  
  if (is_stereotype) {
    QCString path;
    
    if (propertyValue("stereotypeIconPath", path) && !path.isEmpty()) {
      out.indent();
      out << "<icon xmi:type=\"uml:Image\"";
      out.id_prefix(this, "Icon_");
      out << " location=\"" << path << "\"/>\n";
    }
  }
  
  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n";
  
  if (is_stereotype)
    for (i = 0; i != n; i += 1)
      if (ch[i]->kind() == aRelation)
	((UmlRelation *) ch[i])->write_extension(out);
  
  unload();
}

void UmlClass::write_formals(FileOut & out) {
  QValueList<UmlFormalParameter> formal_params = formals();
    
  if (!formal_params.isEmpty()) {
    out.indent();
    out << "<ownedTemplateSignature xmi:type=\"uml:";
    if (!_uml_20)
      out << "Redefinable";
    out << "TemplateSignature\"";
    out.id_prefix(this, "FORMALS_");
    out << ">\n";
    out.indent(+1);

    int rank;
    QValueList<UmlFormalParameter>::ConstIterator iter;

    for (iter = formal_params.begin(), rank = 0;
	 iter != formal_params.end();
	 ++iter, rank += 1)
      (*iter).write(out, this, rank, _uml_20);

    out.indent(-1);
    out.indent();
    out << "</ownedTemplateSignature>\n";
  }
}

void UmlClass::write_actuals(FileOut & out) {
  QValueList<UmlActualParameter> actual_params = actuals();
  QValueList<UmlActualParameter>::ConstIterator iter;
  int rank;
  UmlClass * super = 0;

  for (iter = actual_params.begin(), rank = 0;
       iter != actual_params.end();
       ++iter, rank += 1) {
    if (super != (*iter).superClass()) {
      if (super != 0) {
	out.indent(-1);
	out.indent();
	out << "</templateBinding>\n";
      }
      
      super = (*iter).superClass();
      
      out.indent();
      out << "<templateBinding xmi:type=\"uml:TemplateBinding\"";
      out.id_prefix(this, "ACTUAL", rank);
      out << ">\n";
      out.indent(+1);
      
      out.indent();
      out << "<boundElement";
      out.idref(this);
      out << " />\n";
      
      out.indent();
      out << "<signature";
      out.idref_prefix(super, "FORMALS_");
      out << " />\n";
    }
      
    (*iter).write(out, this, rank);
  }

  if (super != 0) {
    out.indent(-1);
    out.indent();
    out << "</templateBinding>\n";
  }

}

void UmlClass::search_class_assoc() {
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->search_class_assoc();
}

UmlClass * UmlClass::set_assoc(UmlRelation * rel) {
  if (_assoc == 0) {
    _assoc = rel;
    return this;
  }
  else {
    QCString msg = "warning : class '" +  name() +
      "' is an association class associated with several relations<br>";
    
    UmlCom::trace(msg);
    return 0;
  }
}

void UmlClass::get_extended(QValueList<QCString> & r) {
  r.clear();
  
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  unsigned i;
  
  for (i = 0; i != n; i += 1) {
    UmlItem * x = ch[i];
    
    if ((x->kind() == aRelation) &&
	(((UmlRelation *) x)->relationKind() == aDirectionalAssociation) &&
	(((UmlRelation *) x)->roleType()->stereotype() == "metaclass"))
      r.append(((UmlRelation *) x)->roleType()->name());
  }
  
  if (r.isEmpty())
    r.append("Element");
}

