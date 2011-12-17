
#include "UmlActivityAction.h"
#include "FileOut.h"

#include "UmlActivityObject.h"
void UmlActivityAction::write_begin(FileOut & out, QCString k) {
  out.indent();
  out << ((parent()->kind() == anActivity) ? "<node" : "<containedNode")
    << " xmi:type=\"uml:" << k << '"';
  out.id(this);
  if (!name().isEmpty()){
    out << " name=\"";
    out.quote(name());
    out << '"';
  }
}

void UmlActivityAction::write_end(FileOut & out, bool dontclose) {
  out << ">\n";
  out.indent(+1);
  
  QCString s = constraint();
  
  if (! s.isEmpty()) {
    out.indent();
    out << "<ownedRule xmi:type=\"uml:Constraint\"";
    out.id_prefix(this, "CONSTRAINT_");
    out.ref(this, "constrainedElement");
    out << ">\n";
    out.indent();
    out << "\t<specification xmi:type=\"uml:OpaqueExpression\"";
    out.id_prefix(this, "CSPEC_");
    out << ">\n";
    out.indent();
    out << "\t\t<body>";
    out.quote(s);
    out << "</body>\n";
    out.indent();
    out << "\t</specification>\n";
    out.indent();
    out << "</ownedRule>\n";
  }
  
  write_description_properties(out);

  switch (_lang) {
  case Uml:
    write_condition(out, preCondition(), TRUE);
    write_condition(out, postCondition(), FALSE);
    break;
  case Cpp:
    write_condition(out, cppPreCondition(), TRUE);
    write_condition(out, cppPostCondition(), FALSE);
    break;
  default:
    // java
    write_condition(out, javaPreCondition(), TRUE);
    write_condition(out, javaPostCondition(), FALSE);
  }

  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);

  write_incoming_flows(out);
  
  if (!dontclose)
    write_close(out);
}

void UmlActivityAction::write_close(FileOut & out) {
  out.indent(-1);
  out.indent();
  out << ((parent()->kind() == anActivity) ? "</node>\n" : "</containedNode>\n");
    
  unload();
}

void UmlActivityAction::write_condition(FileOut & out, QCString cond, bool pre) {
  if (! cond.isEmpty()) {
    const char * k;
    const char * K;
    const char * body;
    
    if (pre) {
      k = "pre";
      K = "PRE_";
      body = "PRE_BODY_";
    }
    else {
      k = "post";
      K = "POST_";
      body = "POST_BODY_";
    }
    
    out.indent();
    out << '<' << k << "condition xmi:type=\"uml:Constraint\"";
    out.id_prefix(this, K);
    out << ">\n";
    out.indent();
    out << "\t<specification xmi:type=\"uml:OpaqueExpression\"";
    out.id_prefix(this, body);
    out << " body=\"";
    out.quote(cond);
    out << "\"/>\n";
    out.indent();
    out << "</" << k << "condition>\n";
  }
}

void UmlActivityAction::solve_output_flows() {
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1) {
    UmlFlow * f = dynamic_cast<UmlFlow *>(ch[i]);

    if ((f != 0) && (f->control_or_data() == Unset)) {
      UmlActivityObject * o = dynamic_cast<UmlActivityObject *>(f->target());

      f->set_control_or_data(((o == 0) || o->isControlType()) ? IsControl : IsData);
    }
  }

}

