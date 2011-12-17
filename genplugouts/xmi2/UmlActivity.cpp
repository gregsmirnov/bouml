
#include "UmlActivityItem.h"
#include "UmlFlow.h"
#include "UmlOperation.h"
#include "UmlActivity.h"
#include "FileOut.h"
#include "UmlItem.h"

void UmlActivity::write(FileOut & out) {
  const char * k = (_uml_20) ? "ownedMember" : "packagedElement";

  out.indent();
  out << '<' << k << " xmi:type=\"uml:Activity\"";
  out.id(this); 
  out << " name=\"";
  out.quote(name());
  out << '"';
  if (isReadOnly())
    out << " isReadOnly=\"true\"";
  if (isSingleExecution())
    out << " isSingleExecution=\"true\"";
  if (isActive())
    out << " isActive=\"true\"";
  if (specification() != 0)
    out.ref(specification(), "specification");
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
  
  switch  (_lang) {
  case Uml:
    write_condition(out, preCondition(), TRUE);
    write_condition(out, postCondition(), FALSE);
    break;
  case Cpp:
    write_condition(out, cppPreCondition(), TRUE);
    write_condition(out, cppPostCondition(), FALSE);
    break;
  default:
    // Java
    write_condition(out, javaPreCondition(), TRUE);
    write_condition(out, javaPostCondition(), FALSE);
    break;
  }
  
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size();
  unsigned i;
     
  for (i = 0; i != n; i += 1) {
    UmlActivityItem * x = dynamic_cast<UmlActivityItem *>(ch[i]);
    
    if (x != 0)
      x->memo_incoming_flow();
  }

  for (i = 0; i != n; i += 1)
    ch[i]->write(out);

  write_flows(out);
  
  QMap<QCString, Opaque>::ConstIterator ito;
  
  for (ito = _opaque_behavior.begin(); ito != _opaque_behavior.end(); ++ito) {
    out.indent();
    out << "<ownedBehavior xmi:type=\"uml:OpaqueBehavior\"";
    out.id_prefix(ito.data().item, ito.data().kind);
    out << ">\n";
    out.indent();
    out << "\t<body>";
    out.quote(ito.key());
    out << "</body>\n";
    out.indent();
    out << "</ownedBehavior>\n";
  }
       
  out.indent(-1); 
  out.indent(); 
  out << "</" << k << ">\n"; 

  QMap<QCString, UmlItem *>::ConstIterator ite;
  
  for (ite = _opaque_expression.begin(); ite != _opaque_expression.end(); ++ite) {
    out.indent();
    out << '<' << k << " xmi:type=\"uml:OpaqueExpression\"";
    out.id_prefix(ite.data(), "OPAQUE_EXPRESSION_");
    out << ">\n";
    out.indent();
    out << "\t<body>";
    out.quote(ite.key());
    out << "</body>\n";
    out.indent();
    out << "</" << k << ">\n"; 
  }

  unload();
}

void UmlActivity::write_condition(FileOut & out, QCString cond, bool pre) {
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

UmlItem * UmlActivity::add_opaque_behavior(QCString beh, UmlItem * who, const char * k) {
  QMap<QCString, Opaque>::Iterator iter = _opaque_behavior.find(beh);
  
  if (iter == _opaque_behavior.end()) {
    _opaque_behavior.insert(beh, Opaque(who, k));
    return who;
  }
  else
    return iter.data().item;
}

UmlItem * UmlActivity::add_opaque_expression(QCString val, UmlItem * who) {
  QMap<QCString, UmlItem *>::Iterator iter = _opaque_expression.find(val);
  
  if (iter == _opaque_expression.end()) {
    _opaque_expression.insert(val, who);
    return who;
  }
  else
    return iter.data();
}

