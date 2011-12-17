// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// home   : http://bouml.free.fr
//
// *************************************************************************

#include "UmlClass.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"
#include "UmlRelation.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "UmlArtifact.h"

#include "util.h"
#include "state.h"

//
// add all concerning states
//

UmlClass * add_state_behavior(UmlClassView * base_class_view,
			      UmlDeploymentView * base_depl_view)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlClass * state_behavior = 
    UmlClass::made(base_class_view, base_depl_view, "StateBehavior");
  
  include_umlcom(state_behavior);
  
  state_behavior->add_attribute("on_entry", PublicVisibility, "string", 0, 0);
  state_behavior->add_attribute("on_exit", PublicVisibility, "string", 0, 0);
  state_behavior->add_attribute("do_activity", PublicVisibility, "string", 0, 0);
  
  UmlOperation * op;
  
  op = state_behavior->add_op("read", PublicVisibility, "void");
  op->set_cpp("${type}", "", "\
  on_entry = UmlCom::read_string();\n\
  on_exit = UmlCom::read_string();\n\
  do_activity = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "", "\
  on_entry = UmlCom.read_string();\n\
  on_exit = UmlCom.read_string();\n\
  do_activity = UmlCom.read_string();\n",
	       FALSE);
  
  op = state_behavior->add_op("unload", PublicVisibility, "void");
  op->set_cpp("${type}", "", "\
  on_entry = 0;\n\
  on_exit = 0;\n\
  do_activity = 0;\n",
	      FALSE, 0, 0);
  op->set_JavaDecl("");
  
  UmlCom::set_user_id(uid);
  
  return state_behavior;
}

UmlClass * add_state_diagram(UmlClassView * base_class_view, UmlClassView * user_class_view,
			     UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
			     UmlClass * base_item, UmlClass * user_stateitem, UmlClass * user_state)
{
  UmlClass * user_diagram = UmlClass::get("UmlDiagram", 0);
  UmlClass * base_statediagram;
  UmlClass * user_statediagram;
  
  user_diagram->made(base_class_view, user_class_view,
		     base_depl_view, user_depl_view,
		     base_item, "StateDiagram",
		     base_statediagram, user_statediagram,
		     user_stateitem);
  base_statediagram->add_default_base_op(user_diagram, user_statediagram, user_state,
					 "state diagram", "aStateDiagram");
  user_statediagram->add_constr(base_statediagram, PublicVisibility);
  
  user_statediagram->set_Description(" This class manages 'class diagrams', notes that the class 'UmlDiagram'\n"
				     " is a mother class of all the diagrams, allowing to generalize their\n"
				     " management\n"
				     "\n"
				     " You can modify it as you want (except the constructor)");
  return user_statediagram;
}

UmlClass * add_state(UmlClassView * base_class_view, UmlClassView * user_class_view,
		     UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		     UmlClass * base_item, UmlClass * user_item, UmlClass * user_stateitem)
{
  UmlClass * base_state;
  UmlClass * user_state;
  UmlOperation * op;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "State", base_state, user_state,
		  user_stateitem);
  base_state->add_default_base_op(user_item, user_state,
				  UmlClass::get("UmlClassView", 0),
				  "state", "aState");
  user_state->add_constr(base_state, PublicVisibility);
  
  UmlClass * user_statediagram = 
    add_state_diagram(base_class_view, user_class_view,
		      base_depl_view, user_depl_view,
		      base_item, user_stateitem, user_state);
  
    
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
    
  include_umlcom(base_state);
  
  defGetField(base_state, _uml, entryBehavior, on_entry, "string", 0, 0,
	      "entry behavior in OCL");
  defSetField(base_state, _uml, set_EntryBehavior, on_entry, "str", setUmlEntryBehaviorCmd, 0, 0,
	      "entry behavior in OCL");
  defGetField(base_state, _uml, exitBehavior, on_exit, "string", 0, 0,
	      "exit behavior in OCL");
  defSetField(base_state, _uml, set_ExitBehavior, on_exit, "str", setUmlExitBehaviorCmd, 0, 0,
	      "exit behavior in OCL");
  defGetField(base_state, _uml, doActivity, do_activity, "string", 0, 0, 
	      "activity in OCL");
  defSetField(base_state, _uml, set_DoActivity, do_activity, "str", setUmlActivityCmd, 0, 0,
	      "activity in OCL");

  defGetField(base_state, _cpp, cppEntryBehavior, on_entry, "string", "WITHCPP", 0,
	      "entry behavior in C++");
  defSetField(base_state, _cpp, set_CppEntryBehavior, on_entry, "str", setCppEntryBehaviorCmd, 0, 0,
	      "entry behavior in C++");
  defGetField(base_state, _cpp, cppExitBehavior, on_exit, "string", 0, 0,
	      "exit behavior in C++");
  defSetField(base_state, _cpp, set_CppExitBehavior, on_exit, "str", setCppExitBehaviorCmd, 0, 0,
	      "exit behavior in C++");
  defGetField(base_state, _cpp, cppDoActivity, do_activity, "string", 0, 0,
	      "activity in C++");
  defSetField(base_state, _cpp, set_CppDoActivity, do_activity, "str", setCppActivityCmd, 0, "endif",
	      "activity in C++");

  defGetField(base_state, _java, javaEntryBehavior, on_entry, "string", "WITHJAVA", 0,
	      "entry behavior in Java");
  defSetField(base_state, _java, set_JavaEntryBehavior, on_entry, "str", setJavaEntryBehaviorCmd, 0, 0,
	      "entry behavior in Java");
  defGetField(base_state, _java, javaExitBehavior, on_exit, "string", 0, 0,
	      "exit behavior in Java");
  defSetField(base_state, _java, set_JavaExitBehavior, on_exit, "str", setJavaExitBehaviorCmd, 0, 0,
	      "exit behavior in Java");
  defGetField(base_state, _java, javaDoActivity, do_activity, "string", 0, 0,
	      "activity in Java");
  defSetField(base_state, _java, set_JavaDoActivity, do_activity, "str", setJavaActivityCmd, 0, "endif",
	      "activity in Java");

  add_assoc_diag_ops(base_state, user_statediagram);
  include_umlcom(base_state);
  
  op = base_state->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _uml.unload();\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp.unload();\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java.unload();\n"
	      "#endif\n"
	      "  UmlBaseItem::unload(rec, del);\n",
	      FALSE, 0, 0);
  QCString s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _uml = null;\n"
	       "  _cpp = null;\n"
	       "  _java = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
  
  UmlClass * state_behavior =
    add_state_behavior(base_class_view, base_depl_view);
  
  base_state->add_relation(aDirectionalAggregationByValue,
			   "_uml", PrivateVisibility,
			   state_behavior, 0, 0);
  base_state->add_relation(aDirectionalAggregationByValue,
			   "_cpp", PrivateVisibility,
			   state_behavior, "WITHCPP", "endif");
  base_state->add_relation(aDirectionalAggregationByValue,
			   "_java", PrivateVisibility,
			   state_behavior, "WITHJAVA", "endif");
  base_state->add_relation(aDirectionalAssociation,
			   "_assoc_diagram", PrivateVisibility,
			   user_statediagram, 0, 0);
  
  op = base_state->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _assoc_diagram = (UmlStateDiagram *) UmlBaseItem::read_();\n"
	      "  UmlBaseItem::read_uml_();\n"
	      "  _uml.read();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  _assoc_diagram = (UmlStateDiagram) UmlBaseItem.read_();\n"
	       "  super.read_uml_();\n"
	       "  _uml = new StateBehavior();\n"
	       "  _uml.read();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_state->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _cpp.read();\n", FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "", "  _cpp = new StateBehavior();\n  _cpp.read();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_state->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _java.read();\n", FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "", "  _java = new StateBehavior();\n  _java.read();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  return user_state;
}

UmlClass * add_transition_behavior(UmlClassView * base_class_view,
				   UmlDeploymentView * base_depl_view)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlClass * transition_behavior = 
    UmlClass::made(base_class_view, base_depl_view, "TransitionBehavior");
  
  include_umlcom(transition_behavior);
  
  transition_behavior->add_attribute("trigger", PublicVisibility, "string", 0, 0);
  transition_behavior->add_attribute("guard", PublicVisibility, "string", 0, 0);
  transition_behavior->add_attribute("activity", PublicVisibility, "string", 0, 0);
  
  UmlOperation * op;
  
  op = transition_behavior->add_op("read", PublicVisibility, "void");
  op->set_cpp("${type}", "", "\
  trigger = UmlCom::read_string();\n\
  guard = UmlCom::read_string();\n\
  activity = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "", "\
  trigger = UmlCom.read_string();\n\
  guard = UmlCom.read_string();\n\
  activity = UmlCom.read_string();\n",
	       FALSE);
  
  op = transition_behavior->add_op("unload", PublicVisibility, "void");
  op->set_cpp("${type}", "", "\
  trigger = 0;\n\
  guard = 0;\n\
  activity = 0;\n",
	      FALSE, 0, 0);
  op->set_JavaDecl("");
  
  UmlCom::set_user_id(uid);
  
  return transition_behavior;
}

void add_transition(UmlClassView * base_class_view, UmlClassView * user_class_view,
		    UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		    UmlClass * base_item, UmlClass * user_item, UmlClass * user_stateitem)
{
  UmlClass * base_transition;
  UmlClass * user_transition;
  UmlOperation * op;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "Transition", base_transition,
		  user_transition, user_stateitem);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  include_umlcom(base_transition);
  
  base_transition->add_kind("aTransition");
  
  op = base_transition->add_op("create", PublicVisibility, user_transition, TRUE);
  op->set_isClassMember(TRUE);
  op->add_param(0, InputOutputDirection, "start", user_item);
  op->add_param(1, InputOutputDirection, "end", user_item);
  op->set_cpp("${type} *", 
	      "${t0} * ${p0}, ${t1} * ${p1}",
	      "  UmlCom::send_cmd(start->_identifier, createCmd, aTransition,\n"
	      "\t\t   end->_identifier);\n"
	      "  UmlTransition * result = (UmlTransition *) UmlBaseItem::read_();\n"
	      "\n"
	      "  if (result != 0)\n"
	      "    start->reread_children_if_needed_();\n"
	      "  return result;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  UmlCom.send_cmd(start.identifier_(), OnInstanceCmd.createCmd, anItemKind.aTransition,\n"
	       "\t\t   end.identifier_());\n"
	       "  UmlTransition result = (UmlTransition) UmlBaseItem.read_();\n"
	       "  \n"
	       "  if (result != null)\n"
	       "    start.reread_children_if_needed_();\n"
	       "  else\n"
	       "    throw new RuntimeException(\"Cannot create the Transition\");\n"
	       "  return result;\n",
	      FALSE);
  op->set_Description(" Returns a new Transition from 'start' to 'end'\n\
\n\
 In case it cannot be created ('parent' cannot contain it etc ...) return 0\n\
  in C++ and produce a RuntimeException in Java\n");
  
  op = base_transition->add_op("target", PublicVisibility, user_item);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" returns the 'end' object (the 'start' object is the parent of the transition) no set !");
  op->set_cpp("${type} *", "",
	      "  read_if_needed_();\n"
	      "  return _target;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "  return _target;\n", FALSE);
  
  defGetField(base_transition, _uml, trigger, trigger, "string", 0, 0,
	      "trigger in OCL");
  defSetField(base_transition, _uml, set_Trigger, trigger, "str", setUmlTriggerCmd, 0, 0,
	      "trigger in OCL");
  defGetField(base_transition, _uml, guard, guard, "string", 0, 0,
	      "guard in OCL");
  defSetField(base_transition, _uml, set_Guard, guard, "str", setUmlGuardCmd, 0, 0,
	      "guard in OCL");
  defGetField(base_transition, _uml, activity, activity, "string", 0, 0, 
	      "activity in OCL");
  defSetField(base_transition, _uml, set_Activity, activity, "str", setUmlActivityCmd, 0, 0,
	      "activity in OCL");

  defGetField(base_transition, _cpp, cppTrigger, trigger, "string", "WITHCPP", 0,
	      "trigger in C++");
  defSetField(base_transition, _cpp, set_CppTrigger, trigger, "str", setCppTriggerCmd, 0, 0,
         "trigger in C++");
  defGetField(base_transition, _cpp, cppGuard, guard, "string", 0, 0,
	      "guard in C++");
  defSetField(base_transition, _cpp, set_CppGuard, guard, "str", setCppGuardCmd, 0, 0,
	      "guard in C++");
  defGetField(base_transition, _cpp, cppActivity, activity, "string", 0, 0,
	      "activity in C++");
  defSetField(base_transition, _cpp, set_CppActivity, activity, "str", setCppActivityCmd, 0, "endif",
	      "activity in C++");

  defGetField(base_transition, _java, javaTrigger, trigger, "string", "WITHJAVA", 0,
	      "trigger in Java");
  defSetField(base_transition, _java, set_JavaTrigger, trigger, "str", setJavaTriggerCmd, 0, 0,
	      "trigger in Java");
  defGetField(base_transition, _java, javaGuard, guard, "string", 0, 0,
	      "guard in Java");
  defSetField(base_transition, _java, set_JavaGuard, guard, "str", setJavaGuardCmd, 0, 0,
	      "guard in Java");
  defGetField(base_transition, _java, javaActivity, activity, "string", 0, 0,
	      "activity in Java");
  defSetField(base_transition, _java, set_JavaActivity, activity, "str", setJavaActivityCmd, 0, "endif",
	      "activity in Java");
  
  op = base_transition->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _uml.unload();\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp.unload();\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java.unload();\n"
	      "#endif\n"
	      "  UmlBaseItem::unload(rec, del);\n",
	      FALSE, 0, 0);

  QCString s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _uml = null;\n"
	       "  _cpp = null;\n"
	       "  _java = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
  
  base_transition->add_relation(aDirectionalAssociation,
				"_target", PrivateVisibility,
				user_item, 0, 0);
			  
  UmlClass * transition_behavior =
    add_transition_behavior(base_class_view, base_depl_view);
  
  base_transition->add_relation(aDirectionalAggregationByValue,
				"_uml", PrivateVisibility,
				transition_behavior, 0, 0);
  base_transition->add_relation(aDirectionalAggregationByValue,
				"_cpp", PrivateVisibility,
				transition_behavior, "WITHCPP", "endif");
  base_transition->add_relation(aDirectionalAggregationByValue,
				"_java", PrivateVisibility,
				transition_behavior, "WITHJAVA", "endif");
  
  op = base_transition->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseItem::read_uml_();\n"
	      "  _target = UmlBaseItem::read_();\n"
	      "  _uml.read();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _target = UmlBaseItem.read_();\n"
	       "  _uml = new TransitionBehavior();\n"
	       "  _uml.read();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_transition->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _cpp.read();\n", FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "", "  _cpp = new TransitionBehavior();\n  _cpp.read();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_transition->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _java.read();\n", FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "", "  _java = new TransitionBehavior();\n  _java.read();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  base_transition->add_constr(user_item, ProtectedVisibility);
  
  UmlCom::set_user_id(uid);
  
  op = user_transition->add_op("UmlTransition", PublicVisibility,
			       (UmlClass *) 0, FALSE);
  op->add_param(0, InputDirection, "id", "item_id");
  op->add_param(1, InputDirection, "n", "string");
  op->set_cpp(": UmlBaseTransition(id, n)",
	      "${t0} ${p0}, const ${t1} & ${p1}",
	      "", TRUE, 0, 0);
  op->set_java("", "${t0} ${p0}, ${t1} ${p1}",
	       "  super(id, n);\n", TRUE);
}

void add_region(UmlClassView * base_class_view, UmlClassView * user_class_view,
		UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		UmlClass * base_item, UmlClass * user_item,
		UmlClass * user_stateitem, UmlClass * user_state)
{
  UmlClass * base_region;
  UmlClass * user_region;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "Region", base_region, user_region, user_stateitem);
  base_region->add_default_base_op(user_item, user_region, user_state, "region", "aRegion");
  user_region->add_constr(base_region, PublicVisibility);
}

void add_stateaction(UmlClassView * base_class_view, UmlClassView * user_class_view,
		     UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		     UmlClass * base_item, UmlClass * user_item,
		     UmlClass * user_stateitem, UmlClass * user_state)
{
  UmlClass * base_stateaction;
  UmlClass * user_stateaction;
  UmlOperation * op;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "StateAction", base_stateaction,
		  user_stateaction, user_stateitem);

  user_stateaction->add_constr(base_stateaction, PublicVisibility, TRUE);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  include_umlcom(base_stateaction);
  
  base_stateaction->add_default_base_op(user_item, user_stateaction, user_state,
					"state action", "aStateAction", TRUE);
    
  defGet(base_stateaction, _uml, expression, "string", 0, 0,
         "expression in OCL");
  defSet(base_stateaction, _uml, set_Expression, "str", setDefCmd, 0, 0,
         "expression in OCL");
  defGet(base_stateaction, _cpp, cppExpression, "string", "WITHCPP", 0,
         "expression in C++");
  defSet(base_stateaction, _cpp, set_CppExpression, "str", setCppDeclCmd, 0, "endif",
         "expression in C++");
  defGet(base_stateaction, _java, javaExpression, "string", "WITHJAVA", 0,
         "expression in Java");
  defSet(base_stateaction, _java, set_JavaExpression, "str", setJavaDeclCmd, 0, "endif",
         "expression in Java");

  op = base_stateaction->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _uml = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java = 0;\n"
	      "#endif\n"
	      "  UmlBaseItem::unload(rec, del);\n",
	      FALSE, 0, 0);
  
  QCString s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _uml = null;\n"
	       "  _cpp = null;\n"
	       "  _java = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
  
  base_stateaction->add_attribute("_uml", PrivateVisibility, "string",
				  0, 0);
  base_stateaction->add_attribute("_cpp", PrivateVisibility, "string",
				  "WITHCPP", "endif");
  base_stateaction->add_attribute("_java", PrivateVisibility, "string",
				  "WITHJAVA", "endif");
  
  op = base_stateaction->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseItem::read_uml_();\n"
	      "  _uml = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _uml = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_stateaction->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _cpp = UmlCom::read_string();\n", FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "", "  _cpp  = UmlCom.read_string();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_stateaction->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _java = UmlCom::read_string();\n", FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "", "  _java = UmlCom.read_string();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
}

  
void add_pseudostates(UmlClassView * base_class_view, UmlClassView * user_class_view,
		      UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		      UmlClass * base_item, UmlClass * user_item, UmlClass * user_stateitem)
{
  UmlClass * user_pseudostate =
    UmlClass::made(user_class_view, user_depl_view, "UmlPseudoState");
  
  user_pseudostate->set_isAbstract(TRUE);
  user_pseudostate->add_constr(user_item, PublicVisibility);
    
  UmlRelation * rel;
  
  if ((rel = UmlBaseRelation::create(aGeneralisation, user_pseudostate, user_stateitem)) == 0) {
    QCString msg = user_pseudostate->name() + " can't inherit " + user_stateitem->name() + "<br>\n";
    
    UmlCom::trace("<b>" + msg + "</b>");
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
  
  if ((rel = UmlBaseRelation::create(aGeneralisation, user_pseudostate, user_item)) == 0) {
    QCString msg = user_pseudostate->name() + " can't inherit " + user_item->name() + "<br>\n";
    
    UmlCom::trace("<b>" + msg + "</b>");
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
    
  const struct {
    const char * name;
    const char * what;
    const char * kind;
  } ps[] = {
    { "InitialPseudoState", "initial pseudo state", "anInitialPseudoState" },
    { "EntryPointPseudoState", "entry point pseudo state", "anEntryPointPseudoState" },
    { "FinalState", "final state", "aFinalState" },
    { "TerminatePseudoState", "terminate pseudo state", "aTerminatePseudoState" },
    { "ExitPointPseudoState", "exit point pseudo state", "anExitPointPseudoState" },
    { "DeepHistoryPseudoState", "deep history pseudo state", "aDeepHistoryPseudoState" },
    { "ShallowHistoryPseudoState", "shallow history pseudo state", "aShallowHistoryPseudoState" },
    { "JunctionPseudoState", "junction pseudo state", "aJunctionPseudoState" },
    { "ChoicePseudoState", "choice pseudo state", "aChoicePseudoState" },
    { "ForkPseudoState", "fork pseudo state", "aForkPseudoState" },
    { "JoinPseudoState", "join pseudo state", "aJoinPseudoState" }
  };
#define N_PS (sizeof(ps)/sizeof(ps[0]))
  UmlClass * base_ps[N_PS];
  UmlClass * user_ps[N_PS];
  int i;
  
  for (i = 0; i != N_PS; i += 1) {
    user_pseudostate->made(base_class_view, user_class_view,
			   base_depl_view, user_depl_view,
			   base_item, ps[i].name, base_ps[i], user_ps[i]);
    
    bool unnamed = 
      (strcmp(ps[i].kind, "anEntryPointPseudoState") != 0) &&
      (strcmp(ps[i].kind, "anExitPointPseudoState") != 0);
    
    base_ps[i]->add_default_base_op(user_pseudostate, user_ps[i], user_item,
				    ps[i].what, ps[i].kind, unnamed);
    user_ps[i]->add_constr(base_ps[i], PublicVisibility, unnamed);
  }
}

void add_state_item_kind()
{
  // already root
  static const char * const kinds[] = {
    "aState", 
    "aTransition", 
    "aRegion", 
    "aStateDiagram", 
    "aStateAction", 
    "anInitialPseudoState", 
    "anEntryPointPseudoState", 
    "aFinalState", 
    "aTerminatePseudoState", 
    "anExitPointPseudoState", 
    "aDeepHistoryPseudoState", 
    "aShallowHistoryPseudoState", 
    "aJunctionPseudoState", 
    "aChoicePseudoState", 
    "aForkPseudoState", 
    "aJoinPseudoState", 
  };
  UmlClass * itkind = UmlClass::get("anItemKind", 0);
  
  for (int i = 0; i != sizeof(kinds)/sizeof(kinds[0]); i += 1)
    itkind->add_enum_item(kinds[i]);
}

void add_state_on_instance_cmd()
{
  // already root
  static const char * const cmds[] = {
  "setMarkedCmd",
  
  "setUmlEntryBehaviorCmd",
  "setUmlExitBehaviorCmd",
  "setUmlActivityCmd",
  "setCppEntryBehaviorCmd",
  "setCppExitBehaviorCmd",
  "setCppActivityCmd",
  "setJavaEntryBehaviorCmd",
  "setJavaExitBehaviorCmd",
  "setJavaActivityCmd",
  "setUmlTriggerCmd",
  "setUmlGuardCmd",
  "setCppTriggerCmd",
  "setCppGuardCmd",
  "setJavaGuardCmd",
  "setJavaTriggerCmd",
  "setDefCmd"
  };
  UmlClass * itcmd = UmlClass::get("OnInstanceCmd", 0);
  QCString cpp = CppSettings::enumItemDecl();
  QCString java = JavaSettings::enumPatternItemDecl();
  QCString m = "add enum item OnInstanceCmd::";
  
  for (int i = 0; i != sizeof(cmds)/sizeof(cmds[0]); i += 1) {
    UmlAttribute * at;
    
    if ((at = UmlBaseAttribute::create(itcmd, cmds[i])) == 0) {
      // setMarkedCmd may alreadu exist
      if (i != 0) {
	QCString msg = "cannot add enum item '" + QCString(cmds[i]) +
	  "' in 'OnInstanceCmd'<br>\n";
	
	UmlCom::trace("<b>" + msg + "</b>");
	throw 0;
      }
    }
    else {
      UmlCom::trace(m + cmds[i] + "<br>\n");
      at->set_CppDecl(cpp);
      at->set_JavaDecl(java);
    }
  }
}

void baseitem_read_state(UmlClass * base_item) {
  // update read_()'s body
  UmlOperation * op = base_item->get_operation("read_");
  
  if (op != 0) {
    QCString body;
    
    body = op->cppBody();
    body.insert(body.findRev("default:"),
		"case aState:\n\
      return new UmlState(id, name);\n\
    case aTransition:\n\
      return new UmlTransition(id, name);\n\
    case aRegion:\n\
      return new UmlRegion(id, name);\n\
    case aStateDiagram:\n\
      return new UmlStateDiagram(id, name);\n\
    case aStateAction:\n\
      return new UmlStateAction(id);\n\
    case anInitialPseudoState:\n\
      return new UmlInitialPseudoState(id);\n\
    case anEntryPointPseudoState:\n\
      return new UmlEntryPointPseudoState(id, name);\n\
    case aFinalState:\n\
      return new UmlFinalState(id);\n\
    case aTerminatePseudoState:\n\
      return new UmlTerminatePseudoState(id);\n\
    case anExitPointPseudoState:\n\
      return new UmlExitPointPseudoState(id, name);\n\
    case aDeepHistoryPseudoState:\n\
      return new UmlDeepHistoryPseudoState(id);\n\
    case aShallowHistoryPseudoState:\n\
      return new UmlShallowHistoryPseudoState(id);\n\
    case aJunctionPseudoState:\n\
      return new UmlJunctionPseudoState(id);\n\
    case aChoicePseudoState:\n\
      return new UmlChoicePseudoState(id);\n\
    case aForkPseudoState:\n\
      return new UmlForkPseudoState(id);\n\
    case aJoinPseudoState:\n\
      return new UmlJoinPseudoState(id);\n\
    ");
    op->set_CppBody(body);
      
    body = op->javaBody();
    body.insert(body.findRev("default:"),
		"case anItemKind._aState:\n\
      return new UmlState(id, name);\n\
    case anItemKind._aTransition:\n\
      return new UmlTransition(id, name);\n\
    case anItemKind._aRegion:\n\
      return new UmlRegion(id, name);\n\
    case anItemKind._aStateDiagram:\n\
      return new UmlStateDiagram(id, name);\n\
    case anItemKind._aStateAction:\n\
      return new UmlStateAction(id);\n\
    case anItemKind._anInitialPseudoState:\n\
      return new UmlInitialPseudoState(id);\n\
    case anItemKind._anEntryPointPseudoState:\n\
      return new UmlEntryPointPseudoState(id, name);\n\
    case anItemKind._aFinalState:\n\
      return new UmlFinalState(id);\n\
    case anItemKind._aTerminatePseudoState:\n\
      return new UmlTerminatePseudoState(id);\n\
    case anItemKind._anExitPointPseudoState:\n\
      return new UmlExitPointPseudoState(id, name);\n\
    case anItemKind._aDeepHistoryPseudoState:\n\
      return new UmlDeepHistoryPseudoState(id);\n\
    case anItemKind._aShallowHistoryPseudoState:\n\
      return new UmlShallowHistoryPseudoState(id);\n\
    case anItemKind._aJunctionPseudoState:\n\
      return new UmlJunctionPseudoState(id);\n\
    case anItemKind._aChoicePseudoState:\n\
      return new UmlChoicePseudoState(id);\n\
    case anItemKind._aForkPseudoState:\n\
      return new UmlForkPseudoState(id);\n\
    case anItemKind._aJoinPseudoState:\n\
      return new UmlJoinPseudoState(id);\n\
    ");
    op->set_JavaBody(body);
  }
  
  // update artifact
  UmlArtifact * art = base_item->associatedArtifact();
  QCString s;
  
  s = art->cppSource();
  s.insert(s.find("#include \"MiscGlobalCmd.h\""),
	   "#include \"UmlState.h\"\n"
	   "#include \"UmlTransition.h\"\n"
	   "#include \"UmlRegion.h\"\n"
	   "#include \"UmlStateDiagram.h\"\n"
	   "#include \"UmlStateAction.h\"\n"
	   "#include \"UmlInitialPseudoState.h\"\n"
	   "#include \"UmlEntryPointPseudoState.h\"\n"
	   "#include \"UmlFinalState.h\"\n"
	   "#include \"UmlTerminatePseudoState.h\"\n"
	   "#include \"UmlExitPointPseudoState.h\"\n"
	   "#include \"UmlDeepHistoryPseudoState.h\"\n"
	   "#include \"UmlShallowHistoryPseudoState.h\"\n"
	   "#include \"UmlJunctionPseudoState.h\"\n"
	   "#include \"UmlChoicePseudoState.h\"\n"
	   "#include \"UmlForkPseudoState.h\"\n"
	   "#include \"UmlJoinPseudoState.h\"\n");
  art->set_CppSource(s);
}

void update_uml_com()
{
  // already root
  UmlClass * uml_com = UmlClass::get("UmlCom", 0);
  UmlOperation * op =
    uml_com->add_op("send_cmd", PublicVisibility, "void", FALSE);
  
  op->add_param(0, InputDirection, "id", "item_id");
  op->add_param(1, InputDirection, "cmd", UmlClass::get("OnInstanceCmd", 0));
  op->add_param(2, InputOutputDirection, "arg", UmlClass::get("anItemKind", 0));
  op->add_param(3, InputOutputDirection, "id2", "item_id");

  op->set_Description("internal, do NOT use it\n");
  op->set_isClassMember(TRUE);
  
  op->set_cpp("${type}", 
	      "const ${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, const ${t3} ${p3}",
	      "\
#ifdef TRACE\n\
  cout << \"UmlCom::send_cmd(id, \" << cmd << \", \" << arg << \", \" << id2 << \")\\n\";\n\
#endif\n\
  \n\
  write_char(onInstanceCmd);\n\
  write_id(id);\n\
  write_char(cmd);\n\
  write_char(arg);\n\
  write_id(id2);\n\
  flush();\n",
	      FALSE, 0, 0);

  op->set_java("${type}",
	       "${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, ${t3} ${p3}",
	       "\
  //cout << \"UmlCom.send_cmd(id, \" << cmd << \", \" << arg << \", \" << id2 << \")\\n\";\n\
  \n\
  write_char((byte) CmdFamily._onInstanceCmd);\n\
  write_id(id);\n\
  write_char((byte) cmd.value());\n\
  write_char((byte) arg.value());\n\
  write_id(id2);\n\
  flush();\n",
	       FALSE);
}

void upgrade_states(UmlClass * base_item,
		    UmlClass * user_item)
{
  UmlClassView * base_class_view = (UmlClassView *) base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) user_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    user_item->associatedArtifact()->parent();
  
  UmlCom::trace("<b>State</b><br>\n");
  
  UmlClass * user_stateitem = 
    UmlClass::made(user_class_view, user_depl_view, "UmlStateItem", TRUE);
  UmlClass * user_state =
    add_state(base_class_view, user_class_view,
	      base_depl_view, user_depl_view,
	      base_item, user_item, user_stateitem);
  
  UmlCom::trace("<b>Transition</b><br>\n");
  
  add_transition(base_class_view, user_class_view,
		 base_depl_view, user_depl_view,
		 base_item, user_item, user_stateitem);
  
  UmlCom::trace("<b>Region</b><br>\n");
  
  add_region(base_class_view, user_class_view,
	     base_depl_view, user_depl_view,
	     base_item, user_item, user_stateitem, user_state);
  
  UmlCom::trace("<b>Action</b><br>\n");
  
  add_stateaction(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, user_item, user_stateitem, user_state);

  UmlCom::trace("<b>Pseudo States</b><br>\n");
  
  add_pseudostates(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, user_item, user_stateitem);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>update anItemKind</b><br>\n");
  
  add_state_item_kind();
  
  UmlCom::trace("<b>update cmd list</b><br>\n");
  
  add_state_on_instance_cmd();
  
  UmlCom::trace("<b>update item read</b><br>\n");
  
  baseitem_read_state(base_item);
  
  UmlCom::trace("<b>update com</b><br>\n");
  
  update_uml_com();
  
  UmlCom::set_user_id(uid);
}

//
//
//

void base_state_include_umlcom()
{
  UmlArtifact * art = UmlClass::get("UmlBaseState", 0)->associatedArtifact();
  QCString s = art->cppSource();
  
  if (s.find("#include \"UmlCom.h\"\n") == -1) {
    s.insert(s.find("${includes}"), "#include \"UmlCom.h\"\n");
    art->set_CppSource(s);
  }
}


//
//
//

void add_state_specification()
{
  unsigned uid = UmlCom::user_id();
  UmlClass * base_state = UmlClass::get("UmlBaseState", 0);
  UmlClass * uml_oper = UmlClass::get("UmlOperation", 0);
  
  UmlCom::set_user_id(0);
  
  //
    
  UmlOperation * op;
  
  defGetPtr(base_state, _specification, specification, uml_oper, 0, 0,
	    "specification");

  op->moveAfter(base_state->get_operation("set_JavaDoActivity"));
  
  UmlOperation * get = op;
  
  defSetPtr(base_state, _specification, set_Specification, uml_oper,
	    setDefCmd, 0, 0, "specification");
  op->moveAfter(get);
  
  base_state->add_relation(aDirectionalAssociation,
			   "_specification", PrivateVisibility,
			   uml_oper, 0, 0);  
  
  QCString s;
  
  op = base_state->get_operation("read_uml_");
  
  s = op->cppBody();
  s += "  _specification = (UmlOperation *) UmlBaseItem::read_();\n";
  op->set_CppBody(s);
  
  s = op->javaBody();
  s += "  _specification = (UmlOperation) UmlBaseItem.read_();\n";
  op->set_JavaBody(s);
    
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

static void add_ref(const char * scl, const char * sucl, const char * aa, const char * ao)
{
  UmlClass * cl = UmlClass::get(scl, 0);
  UmlClass * ucl = UmlClass::get(sucl, 0);

  // att
  
  UmlRelation * rel =
    cl->add_relation(aDirectionalAssociation, "_reference", PrivateVisibility, ucl, 0, 0);
  
  if (aa != 0)
    rel->moveAfter(cl->get_attribute(aa));
  
  // get
  
  UmlOperation * op;
  
  defGetPtr(cl, _reference, reference, ucl, 0, 0,
	    "the referenced sub machine state or 0/null\n"
	    " if the state is not a sub machine state reference");

  op->moveAfter(cl->get_operation(ao));
  
  UmlOperation * get = op;
  
  defSetPtr(cl, _reference, set_Reference, ucl, setDerivedCmd, 0, 0, 
	    "referenced sub machine state (may be 0/null)");
  op->moveAfter(get);
  
  // read
    
  QCString s;
  
  op = cl->get_operation("read_uml_");
  if (op != 0) {
    s = op->cppBody() + "  _reference = (" + sucl +  " *) UmlBaseItem::read_();\n";
    op->set_CppBody(s);
    s = op->javaBody() + "  _reference = (" + sucl +  ") UmlBaseItem.read_();\n";
    op->set_JavaBody(s);
  }
  else {
    op = cl->add_op("read_uml_", ProtectedVisibility, "void");
    
    op->set_isCppVirtual(TRUE);
    s = QCString("  UmlBaseItem::read_uml_();\n  _reference = (") + sucl +  " *) UmlBaseItem::read_();\n";
    op->set_CppBody(s);
    s = QCString("  super.read_uml_();\n  _reference = (") + sucl +  ") UmlBaseItem.read_();\n";
    op->set_JavaBody(s);
    
    include_umlcom(cl);
  }
}

void add_stateref()
{
  UmlCom::trace("<b>Add sub state machine reference</b><br>\n");

  add_ref("UmlBaseState", "UmlState", "_active", "set_Specification");
  add_ref("UmlBaseEntryPointPseudoState", "UmlEntryPointPseudoState", 0, "kind");
  add_ref("UmlBaseExitPointPseudoState", "UmlExitPointPseudoState", 0, "kind");
}
