
#include "UmlJunctionPseudoState.h"
#include "UmlClass.h"
#include "UmlState.h"

#include "UmlCom.h"
#include "UmlTransition.h"
#include "UmlOperation.h"

void UmlJunctionPseudoState::generate(UmlClass * machine, UmlClass * anystate, UmlState * state) {
  // create an operation because a priori shared
  if (_oper.isEmpty())
    _oper.sprintf("_junction%d", ++_rank);

  UmlClass * cl = state->assocClass();
  UmlOperation * junction;
  
  if (((junction = (UmlOperation *) cl->getChild(anOperation, _oper)) == 0) &&
      ((junction = UmlBaseOperation::create(cl, _oper)) == 0)) {
    UmlCom::trace("Error : cannot create operation '" + _oper 
		  + "' in class '" + cl->name() + "'<br>");
    throw 0;
  }

  junction->defaultDef();
  junction->setComment("implement a junction, through an operation because shared, internal");
  junction->setType("void", "${type}");
  junction->addParam(0, InputOutputDirection, "stm", machine);
  junction->setParams("${t0} & ${p0}");
  
  QCString body;
  const QVector<UmlItem> ch = children();
  QList<UmlTransition> trs;
  unsigned index;
  
  for (index = 0; index != ch.count(); index += 1)
    if (ch[index]->kind() == aTransition)
      // theo mandatory
      trs.append((UmlTransition *) ch[index]);
    
  UmlTransition::generate(trs, machine, anystate, state, body, "  ", FALSE);
  
  junction->set_CppBody(body);
}

void UmlJunctionPseudoState::generate(UmlClass *, UmlClass *, UmlState * state, QCString & body, QCString indent) {
  if (_oper.isEmpty())
    _oper.sprintf("_junction%d", ++_rank);

  body += indent + "stm" + state->path() + "." + _oper + "(stm);\n";
}

//  to have different name for the operation implementing the junction

int UmlJunctionPseudoState::_rank;

