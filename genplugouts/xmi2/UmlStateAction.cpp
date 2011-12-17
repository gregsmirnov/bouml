
#include "UmlStateAction.h"
#include "FileOut.h"
#include "UmlTransition.h"

void UmlStateAction::memo_incoming_trans() {
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size();
  unsigned i;
     
  for (i = 0; i != n; i += 1) {
    UmlStateItem * x = dynamic_cast<UmlStateItem *>(ch[i]);

    if (x != 0)
      x->memo_incoming_trans();
  }
}

void UmlStateAction::write(FileOut & out) {
  out.indent(); 
  out << "<!-- how to export a state action ? -->\n"; 

  _incoming_trans.clear(); // !
  unload(); 
}

