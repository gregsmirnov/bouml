
#include "UmlChoicePseudoState.h"
#include "UmlClass.h"
#include "UmlState.h"

#include "UmlTransition.h"

void UmlChoicePseudoState::generate(UmlClass * machine, UmlClass * anystate, UmlState * state, QCString & body, QCString indent) {
  const QVector<UmlItem> ch = children();
  QList<UmlTransition> trs;
  unsigned index;
  
  for (index = 0; index != ch.count(); index += 1)
    if (ch[index]->kind() == aTransition)
      // theo mandatory
      trs.append((UmlTransition *) ch[index]);
    
  UmlTransition::generate(trs, machine, anystate, state, body, indent, FALSE);
}

