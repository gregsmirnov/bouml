
#include "UmlFormalParameter.h"
#include "FileOut.h"
#include "UmlClass.h"

void UmlFormalParameter::write(FileOut & out, UmlClass * cl, int rank, bool uml20) const {
  out.indent();
  out << "<parameter";
  out.idref_prefix(cl, "TEMPLPARAM", rank);
  out << "/>\n";
  out.indent();
  out << "<ownedParameter xmi:type=\"uml:ClassifierTemplateParameter\"";
  out.id_prefix(cl, "TEMPLPARAM", rank);
  out << ">\n";
  out.indent(+1);

  out.indent();
  if (uml20)
    out << "<ownedElement xmi:type=\"uml:Class\"";
  else
    out << "<ownedParameteredElement xmi:type=\"uml:Class\"";
  out.id_prefix(cl, "TEMPLELEM", rank);
  out << " name=\"";
  out.quote(name());
  out << '"';
  out.ref(cl, "templateParameter", "TEMPLPARAM", rank);
  out << "/>\n";

  if (defaultValue().type != 0)
    UmlItem::write_default_value(out, defaultValue().type->name(), cl, rank);
  else
    UmlItem::write_default_value(out, defaultValue().explicit_type, cl, rank);

  out.indent(-1);
  out.indent();
  out << "</ownedParameter>\n";
}

