
#include "UmlActivityNode.h"
#include "FlowContainer.h"
#include "UmlActivity.h"
#include "UmlActivityObject.h"
#include "UmlFlow.h"
#include "FileOut.h"

void UmlFlow::write(FileOut & out) {
  out.indent();
  out << "<outgoing";
  out.idref(this);
  out << "/>\n";

  UmlItem * gp = parent()->parent();
  FlowContainer * fc = dynamic_cast<FlowContainer *>(gp);
  
  if (fc == 0)
    // pin
    fc = dynamic_cast<FlowContainer *>(gp->parent());
    
  fc->memo_flow(this);
}

void UmlFlow::memo_incoming_flow() {
  target()->add_incoming_flow(this);

}

void UmlFlow::write_it(FileOut & out) {
  const char * k;
    
  switch (parent()->parent()->kind()) {
  case anActivity:
    k = "edge";
    break;
  case anInterruptibleActivityRegion:
    k = (stereotype() == "interrupt")
      ? "interruptingEdge" : "containedEdge";
    break;
  default:
    // anExpansionRegion
    k = "containedEdge";
  }

  out.indent();
  out << '<' << k
    << ((is_control_flow()) ? " xmi:type=\"uml:ControlFlow\""
			    : " xmi:type=\"uml:ObjectFlow\"");
  out.id(this);
  if (!name().isEmpty() && (name() != "<flow>")) {
    out << " name=\"";
    out.quote(name());
    out << '"';
  }
  out.ref(parent(), "source");
  out.ref(target(), "target");
  
  QCString w;
  QCString g;
  QCString s;
  QCString t;
  
  switch (_lang) {
  case Uml:
    w = weight();
    g = guard();
    s = selection();
    t = transformation();
    break;
  case Cpp:
    w = cppWeight();
    g = cppGuard();
    s = cppSelection();
    t = cppTransformation();
    break;
  default:
    // java
    w = javaWeight();
    g = javaGuard();
    s = javaSelection();
    t = javaTransformation();
  }

  write_selection(out, s);

  if (! t.isEmpty())
    out.ref(activity()->add_opaque_behavior(t, this, "OPAQUE_TRANS_BEHAVIOR_"),
	    "transformation", "OPAQUE_TRANS_BEHAVIOR_");

  out << ">\n";
  out.indent(+1);
  
  write_description_properties(out);

  if (! w.isEmpty()) {
    out.indent();
    out << "<weight xmi:type=\"uml:LiteralString\"";
    out.id_prefix(this, "WEIGHT_");
    out << " value=\"";
    out.quote(w);
    out << "\"/>\n";
  }

  if (! g.isEmpty()) {
    out.indent();
    out << "<guard xmi:type=\"uml:OpaqueExpression\"";
    out.id_prefix(this, "GUARD_");
    out << ">\n";
    out.indent();
    out << "\t<body>";
    out.quote(g);
    out << "</body>\n";
    out.indent();
    out << "</guard>\n";
  }

  out.indent(-1);

  out.indent();
  out << "</" << k << ">\n";

  unload();
}

bool UmlFlow::is_control_flow() {
  if (_control_or_data == Unset)
    ((UmlActivityNode *) parent())->solve_output_flows(); // update _control_or_data
  
  return (_control_or_data == IsControl);
}

void UmlFlow::set_control_or_data(ControlOrData k) {
  _control_or_data = k;
}

