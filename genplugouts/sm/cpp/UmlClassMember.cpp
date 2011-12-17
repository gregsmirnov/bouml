
#include "UmlClassMember.h"

void UmlClassMember::setComment(QCString s) {
  set_Description(((cppDecl().find("${description}") != -1) ? "// " : " ")
		  + s);

}

