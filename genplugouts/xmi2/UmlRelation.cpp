
#include "UmlRelation.h"
#include "FileOut.h"

#include "UmlClass.h"
void UmlRelation::write(FileOut & out) {
  write(out, TRUE);
}

void UmlRelation::write(FileOut & out, bool inside) {
  switch (relationKind()) {
  case aGeneralisation:
    if (inside)
      write_generalization(out);
    break;
  case aRealization:
    if (inside)
      parent()->memo_relation(this);
    else
      write_realization(out);
    break;
  case aDependency:
    if (inside)
      parent()->memo_relation(this);
    else
      write_dependency(out);
    break;
  default:
    // don't generate them for actors
    {
      UmlItem * p = parent();
      
      if (p->stereotype() == "actor")
	return;
      do {
	p = p->parent();
      } while (p->kind() == aClass);
      
      UmlItem * op = roleType();
      
      if (op->stereotype() == "actor")
	return;
      do {
	op = op->parent();
      } while (op->kind() == aClass);
      
      if ((p->kind() == aClassView) && (op->kind() == aClassView)) {
	if (inside)
	  write_relation_as_attribute(out);
	else
	  // note : it is the first side
	  write_relation(out);
      }
    }
    break;
  }
}

void UmlRelation::write_generalization(FileOut & out) {
  out.indent();
  out << "<generalization xmi:type=\"uml:Generalization\"";
  out.id(this);
  out.ref(roleType(), "general");
  if (!constraint().isEmpty()) {
    out << ">\n";
    out.indent(+1);
    write_constraint(out);
    out.indent(-1);
    out.indent();
    out << "</generalization>\n";
  }
  else
    out << "/>\n";
}

void UmlRelation::write_dependency(FileOut & out) {
  const char * k = (_uml_20) ? "ownedElement" : "packagedElement";

  out.indent();
  out << '<' << k << " xmi:type=\"uml:Dependency\"";
  out.id(this);
  out.ref(parent(), "client");
  out.ref(roleType(), "supplier");
  out << ">\n";
  out.indent(+1); 
  write_constraint(out);
  write_description_properties(out);
  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n";
}

void UmlRelation::write_realization(FileOut & out) {
  const char * k = (_uml_20) ? "ownedElement" : "packagedElement";

  out.indent();
  out << '<' << k << " xmi:type=\"uml:Realization\"";
  out.id(this);
  out.ref(parent(), "client");
  out.ref(roleType(), "supplier");
  out.ref(roleType(), "realizingClassifier");
  out << ">\n";
  out.indent(+1); 
  write_constraint(out);
  write_description_properties(out);
  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n";
}

void UmlRelation::write_relation(FileOut & out) {
  // note : it is the first side
 
  if (_assoc_class != 0)
    // generated in the association class
    return;
    
  const char * k = (_uml_20) ? "ownedElement" : "packagedElement";

  out.indent();
  out << '<' << k << " xmi:type=\"uml:Association\"";
  out.id_prefix(this, "ASSOC_");
  
  QCString s = name();
  int i1 = s.find("(");
  int i2 = s.findRev(")");
  
  if ((i1 != -1) && (i2 != -1) && (i2 > i1) && (s[i1+1] != '<')  && (s[i2-1] != '>')) {
    s = s.mid(i1 + 1, i2 - i1 - 1);
    
    if (!s.isEmpty()) {
      out << " name=\"";
      out.quote(s);
      out << '"';
    }
  }
  write_visibility(out);
  out << ">\n";
  
  write_ends(out);
  
  out.indent();
  out << "</" << k << ">\n";

}

void UmlRelation::write_ends(FileOut & out) {
  // note : it is the first side
 
  out.indent();
  out << "\t<memberEnd";
  out.idref(this);
  out << "/>\n";
  
  UmlRelation * other = side(FALSE);
  
  out.indent();
  if (other != 0) {
    out << "\t<memberEnd";
    out.idref(other);
    out << "/>\n";
  }
  else {
    out << "\t<ownedEnd xmi:type=\"uml:Property\"";
    out.id_prefix(this, "REVERSE_");
    if (_assoc_class != 0)
      out.ref(_assoc_class, "association");
    else
      out.ref(this, "association", "ASSOC_");
    out << " visibility=\"" << ((_vis_prefix) ? "vis_private\"" : "private\"");
    out.ref(parent(), "type");
    out << " aggregation=\"";
    if (_gen_eclipse)
      out << "none";
    else {
      switch (relationKind()) {
      case anAggregation:
      case aDirectionalAggregation:
	out << "shared";
	break;
      case anAggregationByValue:
      case aDirectionalAggregationByValue:
	out << "composite";
	break;
      default:
	out << "none";
      }
    }
    out << "\" isNavigable=\"false\"/>\n";

    out.indent();
    out << "\t<memberEnd ";
    out.idref_prefix(this, "REVERSE_");
    out << "/>\n";
  }

}

void UmlRelation::write_relation_as_attribute(FileOut & out) {
  UmlRelation * first = side(TRUE);
  QCString s;  
  UmlClass * base;

  if ((first->parent()->stereotype() == "stereotype") &&
      (first->roleType()->stereotype() == "metaclass")) {
    if (this != first)
      return;
    
    base = first->roleType();
    s = "base_" + base->name();
  }
  else {
    base = 0;
      
    switch (_lang) {
    case Uml:
      s = roleName();
      break;
    case Cpp:
      if (cppDecl().isEmpty())
	return;
      s = true_name(roleName(), cppDecl());
      break;
    default: // Java
      if (javaDecl().isEmpty())
	return;
      s = true_name(roleName(), javaDecl());
    }
  }
  
  out.indent();
  out << "<ownedAttribute xmi:type=\"uml:Property\" name=\"" << s << '"';
  out.id(this);
  
  if (base != 0)
    out.ref(first, "association", "EXT_");
  else {
    write_visibility(out);
    write_scope(out);
    if (isReadOnly())
      out << " isReadOnly=\"true\"";
    if (isDerived()) {
      out << " isDerived=\"true\"";
      if (isDerivedUnion())
	out << " isDerivedUnion=\"true\"";
    }
    if (isOrdered())
      out << " isOrdered=\"true\"";
    if (isUnique())
      out << " isUnique=\"true\"";
  
    if (first->_assoc_class != 0)
      out.ref(first->_assoc_class, "association");
    else
      out.ref(first, "association", "ASSOC_");
  
    out << " aggregation=\"";
    if (this == first) {
      parent()->memo_relation(this);
      if (_gen_eclipse) {
	switch (relationKind()) {
	case anAggregation:
	case aDirectionalAggregation:
	  out << "shared";
	  break;
	case anAggregationByValue:
	case aDirectionalAggregationByValue:
	  out << "composite";
	  break;
	default:
	  out << "none";
	}
      }
      else
	out << "none";
    }
    else if (_gen_eclipse)
      out << "none";
    else {
      switch (relationKind()) {
      case anAggregation:
      case aDirectionalAggregation:
	out << "shared";
	break;
      case anAggregationByValue:
      case aDirectionalAggregationByValue:
	out << "composite";
	break;
      default:
	out << "none";
      }
    }
    out << '"';
  }
  
  out << ">\n";
  out.indent(+1);
  
  out.indent();
  out << "<type xmi:type=\"uml:Class\"";
  if (base != 0) {
    if (! base->propertyValue("metaclassPath", s))
      s = (_uml_20) ? "http://schema.omg.org/spec/UML/2.0/uml.xml"
		    : "http://schema.omg.org/spec/UML/2.1/uml.xml";
    out << " href=\"" << s << '#' << base->name() << '"';
  }
  else
    out.idref(roleType());
  out << "/>\n";
  write_multiplicity(out, multiplicity(), this);
  write_default_value(out, defaultValue(), this);
  write_constraint(out);
  write_annotation(out);
  write_description_properties(out);
  
  out.indent(-1);
  out.indent();
  out << "</ownedAttribute>\n";

  unload();
}

void UmlRelation::write_extension(FileOut & out) {
  if ((side(TRUE) == this) && 
      (parent()->stereotype() == "stereotype") &&
      (roleType()->stereotype() == "metaclass")) {
    const char * k = (_uml_20) ? "ownedMember" : "packagedElement";
    
    out.indent();
    out << "<" << k << " xmi:type=\"uml:Extension\" name=\"A_";
    out.quote(roleType()->name());
    out  << '_';
    out.quote(parent()->name());
    out << '"';
    out.id_prefix(this, "EXT_");
    out.ref(this, "memberEnd", "BASE_");
    out << ">\n";
    out.indent();
    out << "\t<ownedEnd xmi:type=\"uml:ExtensionEnd\" name=\"extension_";
    out.quote(parent()->name());
    out << '"';
    out.id_prefix(this, "EXTEND_");
    out.ref(this, "type");
    out << " aggregation=\"composite\"/>\n";
    out.indent();
    out << "</" << k << ">\n";
  }

  unload();
}

void UmlRelation::search_class_assoc() {
  if (side(TRUE) != this)
    return;
    
  switch (relationKind()) {
  case aGeneralisation:
  case aRealization:
  case aDependency:
    break;
  default:
    {
      UmlTypeSpec a = association();
      
      if (a.type != 0) {
	// not generated for actors
	UmlItem * p = parent();
	
	if (p->stereotype() != "actor") {
	  do {
	    p = p->parent();
	  } while (p->kind() == aClass);
	  if (p->kind() == aClassView)
	    _assoc_class = a.type->set_assoc(this);
	}
      }
    }
  }
}

