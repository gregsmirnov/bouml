
#include "UmlClassMember.h"

void UmlClassMember::setVisibility(QCString v) {
  if ((v == "public") || (v == "vis_public"))
    set_Visibility(PublicVisibility);
  else if ((v == "protected") || (v == "vis_protected"))
    set_Visibility(ProtectedVisibility);
  else if ((v == "private") || (v == "vis_private"))
    set_Visibility(PrivateVisibility);
  else if ((v == "package") || (v == "vis_package"))
    set_Visibility(PackageVisibility);
}

