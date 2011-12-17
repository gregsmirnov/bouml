
#include "UmlExitPointPseudoState.h"
#include "UmlClass.h"

#include "UmlCom.h"
#include "UmlTransition.h"
#include "UmlOperation.h"
#include "UmlState.h"

void UmlExitPointPseudoState::init(UmlClass *, QCString, QCString, UmlState *) {
  // check transition number
  const QVector<UmlItem> ch = children();
  
  switch (ch.count()) {
  case 0:
    return;
  case 1:
    break;
  default:
    UmlCom::trace("Error : exit point '" + name() + "' can't have several transitions<br>");
    throw 0;
  }

  // note : don't call setHasCompletion() in this case to not execute 
  // the output transition in _completion(), this is done when the
  // exit point is reached

}

void UmlExitPointPseudoState::generate(UmlClass * machine, UmlClass * anystate, UmlState * state) {
  // adds the operation _exit<n>() to share this pseudo state a priori shared
  if (_oper.isEmpty())
    _oper.sprintf("_exit%d", ++_rank);
  
  UmlClass * cl = state->assocClass();
  UmlOperation * ex;
  
  if (((ex = (UmlOperation *) cl->getChild(anOperation, _oper)) == 0) &&
      ((ex = UmlBaseOperation::create(cl, _oper)) == 0)) {
    UmlCom::trace("Error : cannot create operation '" + _oper
		  + "' in class '" + cl->name() + "'<br>");
    throw 0;
  }
  ex->defaultDef();
  ex->setComment("to manage the exit point '" + name() + "', defined because probably shared");
  ex->setType("void", "${type}");
  ex->addParam(0, InputOutputDirection, "stm", machine);
  ex->setParams("${t0} & ${p0}");
  
  QCString body;
  
  // the exit behavior is made entering in the exit pseudo state
  if (! state->cppExitBehavior().isEmpty())
    body = "  _doexit(stm);\n";
    
  // transition number <= 1 already checked by init()
  const QVector<UmlItem> ch = children();
  
  if (ch.count() != 0) {
    if (!((UmlTransition *) ch[0])->cppGuard().isEmpty()) {
      UmlCom::trace("Error : transition from an exit point can't have guard<br>");
      throw 0;
    }
    else
      ch[0]->generate(machine, anystate, state, body, "  ");
  }

  ex->set_CppBody(body);
}

void UmlExitPointPseudoState::generate(UmlClass *, UmlClass *, UmlState *, QCString & body, QCString indent) {
  // generate a call to _exit<n>() because it is a priori shared
  if (_oper.isEmpty())
    _oper.sprintf("_exit%d", ++_rank);
    
  // note : only a state may contain an exit point, a region can't
  body += indent + "stm" + ((UmlState *) parent())->path()
    + "." + _oper + "(stm);\n";
}

//  to have different name for the operation implementing the exit point

int UmlExitPointPseudoState::_rank;

