
#include "UmlRegion.h"
#include "UmlTransition.h"
#include "FileOut.h"

void UmlRegion::memo_incoming_trans() {
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size();
  unsigned i;
     
  for (i = 0; i != n; i += 1) {
    UmlStateItem * x = dynamic_cast<UmlStateItem *>(ch[i]);

    if (x != 0)
      x->memo_incoming_trans();
  }
}

void UmlRegion::memo_trans(UmlTransition * tr) {
#if 0
  _trans.append(tr);
#else
  // to bypass Eclipse's bug
  dynamic_cast<UmlStateItem *>(parent())->memo_trans(tr);
#endif
}

void UmlRegion::write(FileOut & out) {
  out.indent(); 
  out << "<region xmi:type=\"uml:Region\"";
  out.id(this); 
  out << " name=\"";
  out.quote(name());
  out << "\">\n";
  out.indent(+1); 
  
  write_description_properties(out); 
     
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size();
  unsigned i;
  
  for (i = 0; i != n; i += 1)
    ch[i]->write(out);
    
#if 0
  // to bypass Eclipse's bug this is done by the parent state
  while (! _trans.isEmpty())
    _trans.take(0)->write_it(out);
#endif

  out.indent(-1); 
  out.indent(); 
  out << "</region>\n"; 
    
  unload(); 
}

