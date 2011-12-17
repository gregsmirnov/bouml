
#include "UmlActivityObject.h"
#include "FileOut.h"

#include "UmlState.h"
#include "UmlTransition.h"
void UmlActivityObject::write(FileOut & out) {
  const char * k = (parent()->kind() == anActivity)
    ? "node" : "containedNode";
    
  out.indent();
  out << '<' << k << " xmi:type=\"uml:";
  
  QCString st = stereotype();
  
  if (st == "datastore")
    out << "DataStoreNode";
  else if (st == "centralBuffer")
    out << "CentralBufferNode";
  else
    out << "ObjectNode";
  
  out << "\" name=\"";
  out.quote(name());
  out << '"';
  out.id(this);
  if (isControlType())
    out << " isControlType=\"true\"";
  write_ordering(out);
  write_selection(out);
  write_in_state(out);
  out << ">\n";
  
  out.indent(+1);
  
  write_description_properties(out);
  write_multiplicity(out, multiplicity(), this);
  UmlItem::write_type(out, type());

  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);
  
  write_incoming_flows(out);
  
  out.indent(-1);

  out.indent();
  out << "</" << k << ">\n";

  unload();
}

void UmlActivityObject::solve_output_flows() {
  ControlOrData v = (isControlType()) ? IsControl : IsData;
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1) {
    UmlFlow * f = dynamic_cast<UmlFlow *>(ch[i]);

    if ((f != 0) && (f->control_or_data() == Unset))
      f->set_control_or_data(v);
  }

  QListIterator<UmlFlow> it(_incoming_flows);
  
  while (it.current() != 0) {
    if (it.current()->control_or_data() == Unset)
      it.current()->set_control_or_data(v);

    ++it;
  }
}

void UmlActivityObject::write_ordering(FileOut & out) {
  out << " ordering=\"";
  switch (ordering()) {
  case unordered:
    out << "unordered";
    break;
  case ordered:
    out << "ordered";
    break;
  case lifo:
    out << "LIFO";
    break;
  default:
    // fifo
    out << "FIFO";
    break;
  }
  out << '"';
}

void UmlActivityObject::write_in_state(FileOut & out) {
  QCString s = inState();
  
  if (!s.isEmpty()) {
    UmlState * st = UmlState::find(s);

    if (st != 0)
      out.ref(st, "inState");
    else {
      // not legal but ...
      out << " inState=\"";
      out.quote(s);
      out << '"';
    }
  }
}

void UmlActivityObject::write_selection(FileOut & out) {
  switch (_lang) {
  case Uml:
    UmlActivityItem::write_selection(out, selection());
    break;
  case Cpp:
    UmlActivityItem::write_selection(out, cppSelection());
  default:
    // java
    UmlActivityItem::write_selection(out, javaSelection());
  }
}

