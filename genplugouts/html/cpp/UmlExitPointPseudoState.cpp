
#include "UmlExitPointPseudoState.h"

#include "UmlCom.h"
QCString UmlExitPointPseudoState::sKind() {
  return "exit point pseudo state";
}

void UmlExitPointPseudoState::html(QCString pfix, unsigned int rank, unsigned int level) {
  define();

  UmlCom::message(name());

  QCString s = sKind();
  
  s.replace(0, 1, s.left(1).upper());
  UmlItem::html(s, (UmlDiagram *) 0);
  
  fw.write("<p>Defined in ");
  if (parent()->kind() == aRegion)
    parent()->parent()->write();
  else
    parent()->write();
  fw.write("</p>");
  
  if (reference() != 0) {
    fw.write("<p>References ");
    reference()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

