
#include "UmlActivityNode.h"
#include "FileOut.h"

void UmlActivityNode::memo_incoming_flow() {
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size();
  unsigned i;
     
  for (i = 0; i != n; i += 1) {
    UmlActivityItem * x = dynamic_cast<UmlActivityItem *>(ch[i]);
								
    if (x != 0)
      x->memo_incoming_flow();
  }
}

void UmlActivityNode::add_incoming_flow(UmlFlow * flow) {
  _incoming_flows.append(flow);
}

void UmlActivityNode::write_incoming_flows(FileOut & out) {
  QListIterator<UmlFlow> it(_incoming_flows);
  
  while (it.current() != 0) {
    out.indent();
    out << "<incoming";
    out.idref(it.current());
    out << "/>\n";
    ++it;
  }

}

