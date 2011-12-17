
#include "UmlClassInstanceReference.h"
#include "FileOut.h"
#include "UmlItem.h"

#include "UmlClass.h"
const char * UmlClassInstanceReference::connector(UmlClassInstanceReference * other) {
  static char s[32];
  
  if (other->rank < rank)
    sprintf(s, "CONNECTOR%d_%d_", other->rank, rank);
  else
    sprintf(s, "CONNECTOR%d_%d_", rank, other->rank);
  
  return s;
}

const char * UmlClassInstanceReference::attribute() {
  static char s[32];
  
  sprintf(s, "COLLATTR%d_", rank);
  
  return s;
}

const char * UmlClassInstanceReference::lifeline() {
  static char s[32];
  
  sprintf(s, "LIFELINE%d_", rank);
  
  return s;
}

void UmlClassInstanceReference::write_lifelines(FileOut & out, UmlItem * diagram, const QVector<UmlClassInstanceReference> & insts)
{
  unsigned sup = insts.size();
  unsigned index;
  
  for (index = 0; index != sup; index += 1) {
    UmlClassInstanceReference * ci = insts.at(index);
    
    out.indent();
    out << "<lifeline xmi:type=\"uml:Lifeline\"";
    out.id_prefix(diagram, ci->lifeline());
    out << " name=\"";
    out.quote(ci->name());
    out << '"';
    out.ref(diagram, "represents", ci->attribute());
    out << "/>\n";
  }
}

void UmlClassInstanceReference::write_attributes(FileOut & out, UmlItem * diagram, const QVector<UmlClassInstanceReference> & insts)
{
  unsigned sup = insts.size();
  unsigned index;
  
  for (index = 0; index != sup; index += 1) {
    UmlClassInstanceReference * ci = insts.at(index);
    
    out.indent();
    out << "<ownedAttribute xmi:type=\"uml:Property\"";
    out.id_prefix(diagram, ci->attribute());
    out << " name=\"";
    out.quote(ci->name());
    out << '"';
    out.ref(ci->type(), "type");
    out << "/>\n";
  }
}

int UmlClassInstanceReference::next_rank;

