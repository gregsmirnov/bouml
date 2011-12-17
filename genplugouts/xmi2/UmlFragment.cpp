
#include "UmlFragment.h"
#include "FileOut.h"
#include "UmlItem.h"
#include "UmlSequenceMessage.h"
#include "UmlClassInstanceReference.h"

#include "UmlFragmentCompartment.h"
#include "UmlDiagram.h"
void UmlFragment::write(FileOut & out, UmlItem * diagram, QList<UmlSequenceMessage> & msgs) {
  QCString oper = name();
  
  if (oper == "ref")
    write_ref(out, diagram, msgs);
  else {
    if ((oper != "alt") &&
	(oper != "opt") &&
	(oper != "loop") &&
	(oper != "break") &&
	(oper != "par") &&
	(oper != "seq") &&
	(oper != "strict") &&
	(oper != "neg") &&
	(oper != "critical") &&
	(oper != "assert") &&
	(oper != "ignore") &&
	(oper != "consider"))
      oper = "";
    else {
      static int rank = 0;
      
      out.indent();
      out << "<fragment xmi:type=\"uml:CombinedFragment\"";
      out.id_prefix(diagram, "COMBINEDFRAGMENT", ++rank);
      
      if (! covered.isEmpty()) {
	out << " covered=\"";
	
	UmlClassInstanceReference * lifeline = covered.first();
	
	for (;;) {
	  out.ref_only(diagram, lifeline->lifeline());
	  lifeline = covered.next();
	  if (lifeline == 0)
	    break;
	  out << ' ';
	}
	
	out << '"';
      }
      
      out << " interactionOperator=\"" << oper << "\">\n";
      out.indent(+1);
    }
    
    const QVector<UmlFragmentCompartment> & subs = compartments();
    unsigned n = subs.size();
    
    for (unsigned i = 0; i != n; i += 1)
      subs.at(i)->write(out, diagram, msgs, oper);
    
    if (!oper.isEmpty()) {
      out.indent(-1);
      out.indent();
      out << "</fragment>\n";
    }
  }
}

void UmlFragment::cover(UmlSequenceMessage * msg) {
  if (covered.findRef(msg->from()) == -1)
    covered.append(msg->from());
  if (covered.findRef(msg->to()) == -1)
    covered.append(msg->to());

  if (container() != 0)
    container()->fragment()->cover(msg);
}

void UmlFragment::write_ref(FileOut & out, UmlItem * diagram, QList< UmlSequenceMessage > & msgs) {
  static int rank = 0;
  
  out.indent();
  out << "<fragment xmi:type=\"uml:InteractionUse\"";
  out.id_prefix(diagram, "INTERACTIONUSE", ++rank);
  
  const QVector<UmlClassInstanceReference> & v = UmlBaseFragment::covered();
  unsigned n;
  unsigned index;
  
  n = v.count();
  
  if (n != 0) {
    out << " covered=\"";
    
    index = 0;
    for (;;) {
      out.ref_only(diagram, v.at(index)->lifeline());
      if (++index == n)
	break;
      out << ' ';
    }
    
    out << '"';
  }
  
  UmlDiagram * d = refer();
  
  if (d != 0) {
    switch (d->kind()) {
    case aSequenceDiagram:
    case aCollaborationDiagram:
      out.ref(d, "refersTo", "INTERACTION_");
      
      if (arguments().isEmpty())
	out << "/>\n";
      else {
	static int rank = 0;
	
	out << ">\n";
	out.indent();
	out << "\t<argument xmi:type=\"uml:OpaqueExpression\"";
	out.id_prefix(d, "INTER_ARG_EXPR_", ++rank);
	out << ">\n";
	out.indent();
	out << "\t\t<body>";
	out.quote(arguments());
	out << "</body>\n";
	out.indent();
	out << "\t</argument>\n";
	out.indent();
	out << "</fragment>\n";
      }
      break;
    default:
      out << "/>\n";
      break;
    }
  }
  else
    out << "/>\n";

  // remove internal messages and compartment
  
  const QVector<UmlFragmentCompartment> & subs = compartments();

  n = subs.size();
  
  for (index = 0; index != n; index += 1)
    subs.at(index)->bypass(msgs);
}

