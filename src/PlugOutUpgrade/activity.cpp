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

#include "UmlClassView.h"
#include "UmlDeploymentView.h"
#include "UmlClass.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"
#include "UmlRelation.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "UmlArtifact.h"
#include "UmlExtraClassMember.h"

#include "util.h"
#include "activity.h"

//
// add all concerning activities
//

UmlClass * add_activity_diagram(UmlClassView * base_class_view, UmlClassView * user_class_view,
				UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
				UmlClass * base_item, UmlClass * user_activity,
				UmlClass * user_activityitem)
{
  UmlClass * user_diagram = UmlClass::get("UmlDiagram", 0);
  UmlClass * base_activitydiagram;
  UmlClass * user_activitydiagram;
    
  user_diagram->made(base_class_view, user_class_view,
		     base_depl_view, user_depl_view,
		     base_item, "ActivityDiagram",
		     base_activitydiagram, user_activitydiagram,
		     user_activityitem);
  base_activitydiagram->add_default_base_op(user_diagram, user_activitydiagram, user_activity,
					    "activity diagram", "anActivityDiagram");
  user_activitydiagram->add_constr(base_activitydiagram, PublicVisibility);

  user_activitydiagram->set_Description(" This class manages 'activity diagrams', notes that the class 'UmlDiagram'\n"
					" is a mother class of all the diagrams, allowing to generalize their\n"
					" management\n"
					"\n"
					" You can modify it as you want (except the constructor)");
  UmlClass * cl;
  
  if ((cl = UmlClass::get("UmlBaseStateDiagram", 0)) != 0)
    base_activitydiagram->moveAfter(cl);
  
  if ((cl = UmlClass::get("UmlStateDiagram", 0)) != 0)
    user_activitydiagram->moveAfter(cl);

  return user_activitydiagram;
}

void add_pre_post_conditions(UmlClass * base_class)
{
  // uid is already 0
  UmlOperation * op;

  defGet(base_class, _pre_condition, preCondition, "string", 0, 0,
	 "pre condition");
  defSet(base_class, _pre_condition, set_PreCondition, "str", setUmlEntryBehaviorCmd, 0, 0,
	 "pre condition");
  defGet(base_class, _post_condition, postCondition, "string", 0, 0,
	 "post condition");
  defSet(base_class, _post_condition, set_PostCondition, "str", setUmlExitBehaviorCmd, 0, 0,
	 "post condition");

  defGet(base_class, _cpp_pre_condition, cppPreCondition, "string", "WITHCPP", 0,
	 "pre condition in C++");
  defSet(base_class, _cpp_pre_condition, set_CppPreCondition, "str", setCppEntryBehaviorCmd, 0, 0,
	 "pre condition in C++");
  defGet(base_class, _cpp_post_condition, cppPostCondition, "string", 0, 0,
	 "post condition in C++");
  defSet(base_class, _cpp_post_condition, set_CppPostCondition, "str", setCppExitBehaviorCmd, 0, "#endif",
	 "post condition in C++");

  defGet(base_class, _java_pre_condition, javaPreCondition, "string", "WITHJAVA", 0,
	 "pre condition in Java");
  defSet(base_class, _java_pre_condition, set_JavaPreCondition, "str", setJavaEntryBehaviorCmd, 0, 0,
	 "pre condition in Java");
  defGet(base_class, _java_post_condition, javaPostCondition, "string", 0, 0,
	 "post condition in Java");
  defSet(base_class, _java_post_condition, set_JavaPostCondition, "str", setJavaExitBehaviorCmd, 0, "#endif",
	 "post condition in Java");
}

void add_activity(UmlClassView * base_class_view, UmlClassView * user_class_view,
		  UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		  UmlClass * base_item, UmlClass * user_item, UmlClass * user_activityitem)
{
  UmlClass * base_activity;
  UmlClass * user_activity;
  UmlOperation * op;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "Activity", base_activity, user_activity,
		  0);
  base_activity->add_default_base_op(user_item, user_activity,
				     UmlClass::get("UmlClassView", 0),
				     "activity", "anActivity");
  user_activity->add_constr(base_activity, PublicVisibility);  
    
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
    
  add_pre_post_conditions(base_activity);
  include_umlcom(base_activity);

  defGetBool(base_activity, _read_only, isReadOnly, 0, 0,
	     " return the isReadOnly attribute, if TRUE the activity must not made any changes to variables ouside the activity or to objects.");
  defSetBool(base_activity, _read_only, set_isReadOnly, setReadOnlyCmd, 0, 0,
	     " set the isReadOnly attribute");
  defGetBool(base_activity, _single_execution, isSingleExecution, 0, 0,
	     " return the isSingleExecution attribute, if TRUE all invocations of the activity are handle by the same execution");
  defSetBool(base_activity, _single_execution, set_isSingleExecution, setSingleExecutionCmd, 0, 0,
	     " set the isSingleExecution attribute");

  UmlCom::set_user_id(uid);
    
  UmlClass * user_activitydiagram = 
    add_activity_diagram(base_class_view, user_class_view,
			 base_depl_view, user_depl_view,
			 base_item, user_item, user_activityitem);
  
  UmlCom::set_user_id(0);
    
  add_assoc_diag_ops(base_activity, user_activitydiagram);

  op = base_activity->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _pre_condition = 0;\n"
	      "  _post_condition = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp_pre_condition = 0;\n"
	      "  _cpp_post_condition = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_pre_condition = 0;\n"
	      "  _java_post_condition = 0;\n"
	      "#endif\n"
	      "  UmlBaseItem::unload(rec, del);\n",
	      FALSE, 0, 0);
  QCString s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _pre_condition = null;\n"
	       "  _post_condition = null;\n"
	       "  _cpp_pre_condition = null;\n"
	       "  _cpp_post_condition = null;\n"
	       "  _java_pre_condition = null;\n"
	       "  _java_post_condition = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_activity->add_attribute("_pre_condition", PrivateVisibility, "string",
			       0, 0);
  base_activity->add_attribute("_post_condition", PrivateVisibility, "string",
			       0, 0);
  base_activity->add_attribute("_cpp_pre_condition", PrivateVisibility, "string",
			       "WITHCPP", 0);
  base_activity->add_attribute("_cpp_post_condition", PrivateVisibility, "string",
			       0, "endif");
  base_activity->add_attribute("_java_pre_condition", PrivateVisibility, "string",
				  "WITHJAVA", 0);
  base_activity->add_attribute("_java_post_condition", PrivateVisibility, "string",
			       0, "endif");
  base_activity->add_attribute("_read_only", PrivateVisibility, "bool",
			       0, 0);
  base_activity->add_attribute("_single_execution", PrivateVisibility, "bool",
			       0, 0);
  base_activity->add_relation(aDirectionalAssociation,
			      "_assoc_diagram", PrivateVisibility,
			      user_activitydiagram, 0, 0);  
  
  op = base_activity->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _assoc_diagram = (UmlActivityDiagram *) UmlBaseItem::read_();\n"
	      "  UmlBaseItem::read_uml_();\n"
	      "  _pre_condition = UmlCom::read_string();\n"
	      "  _post_condition = UmlCom::read_string();\n"
	      "  _read_only = UmlCom::read_bool();\n"
	      "  _single_execution = UmlCom::read_bool();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  _assoc_diagram = (UmlActivityDiagram) UmlBaseItem.read_();\n"
	       "  super.read_uml_();\n"
	       "  _pre_condition = UmlCom.read_string();\n"
	       "  _post_condition = UmlCom.read_string();\n"
	       "  _read_only = UmlCom.read_bool();\n"
	       "  _single_execution = UmlCom.read_bool();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_activity->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _cpp_pre_condition = UmlCom::read_string();\n"
	      "  _cpp_post_condition = UmlCom::read_string();\n",
	      FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "",
	       "  _cpp_pre_condition = UmlCom.read_string();\n"
	       "  _cpp_post_condition = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_activity->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	       "  _java_pre_condition = UmlCom::read_string();\n"
	       "  _java_post_condition = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "",
	       "  _java_pre_condition = UmlCom.read_string();\n"
	       "  _java_post_condition = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
}

UmlClass * add_activitynode(UmlClassView * base_class_view, UmlClassView * user_class_view,
			    UmlDeploymentView * base_depl_view,
			    UmlDeploymentView * user_depl_view,
			    UmlClass * base_item, UmlClass * user_item,
			    UmlClass * user_activityitem)
{
  UmlClass * base_activitynode;
  UmlClass * user_activitynode;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "ActivityNode", base_activitynode, user_activitynode,
		  user_activityitem);

  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
    
  base_activitynode->add_constr(user_item, ProtectedVisibility); 
  
  UmlCom::set_user_id(uid);
  
  user_activitynode->add_constr(base_activitynode, ProtectedVisibility);
  user_activitynode->set_isAbstract(TRUE);

  return user_activitynode;
}

UmlClass * add_flow_behavior(UmlClassView * base_class_view,
			     UmlDeploymentView * base_depl_view)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlClass * flow_behavior = 
    UmlClass::made(base_class_view, base_depl_view, "FlowBehavior");
  
  include_umlcom(flow_behavior);
  
  flow_behavior->add_attribute("weight", PublicVisibility, "string", 0, 0);
  flow_behavior->add_attribute("guard", PublicVisibility, "string", 0, 0);
  flow_behavior->add_attribute("selection", PublicVisibility, "string", 0, 0);
  flow_behavior->add_attribute("transformation", PublicVisibility, "string", 0, 0);
  
  UmlOperation * op;
  
  op = flow_behavior->add_op("read", PublicVisibility, "void");
  op->set_cpp("${type}", "", "\
  weight = UmlCom::read_string();\n\
  guard = UmlCom::read_string();\n\
  selection = UmlCom::read_string();\n\
  transformation = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "", "\
  weight = UmlCom.read_string();\n\
  guard = UmlCom.read_string();\n\
  selection = UmlCom.read_string();\n\
  transformation = UmlCom.read_string();\n",
	       FALSE);
  
  op = flow_behavior->add_op("unload", PublicVisibility, "void");
  op->set_cpp("${type}", "", "\
  weight = 0;\n\
  guard = 0;\n\
  selection = 0;\n\
  transformation = 0;\n",
	      FALSE, 0, 0);
  op->set_JavaDecl("");
  
  UmlCom::set_user_id(uid);
  
  return flow_behavior;
}

void add_flow(UmlClassView * base_class_view, UmlClassView * user_class_view,
	      UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
	      UmlClass * base_item, UmlClass * user_item,
	      UmlClass * user_activityitem, UmlClass * user_activitynode)
{
  UmlClass * base_flow;
  UmlClass * user_flow;
  UmlOperation * op;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "Flow", base_flow,
		  user_flow, user_activityitem);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  include_umlcom(base_flow);
  
  base_flow->add_kind("aFlow");
  
  op = base_flow->add_op("create", PublicVisibility, user_flow, TRUE);
  op->set_isClassMember(TRUE);
  op->add_param(0, InputOutputDirection, "start", user_activitynode);
  op->add_param(1, InputOutputDirection, "end", user_activitynode);
  op->set_cpp("${type} *", 
	      "${t0} * ${p0}, ${t1} * ${p1}",
	      "  UmlCom::send_cmd(start->_identifier, createCmd, aFlow,\n"
	      "\t\t   end->_identifier);\n"
	      "  UmlFlow * result = (UmlFlow *) UmlBaseItem::read_();\n"
	      "\n"
	      "  if (result != 0)\n"
	      "    start->reread_children_if_needed_();\n"
	      "  return result;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  UmlCom.send_cmd(start.identifier_(), OnInstanceCmd.createCmd, anItemKind.aFlow,\n"
	       "\t\t   end.identifier_());\n"
	       "  UmlFlow result = (UmlFlow) UmlBaseItem.read_();\n"
	       "  \n"
	       "  if (result != null)\n"
	       "    start.reread_children_if_needed_();\n"
	       "  else\n"
	       "    throw new RuntimeException(\"Cannot create the flow\");\n"
	       "  return result;\n",
	      FALSE);
  op->set_Description(" Returns a new flow from 'start' to 'end'\n\
\n\
 In case it cannot be created ('parent' cannot contain it etc ...) return 0\n\
  in C++ and produce a RuntimeException in Java\n");
  
  op = base_flow->add_op("target", PublicVisibility, user_activitynode);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" returns the 'end' object (the 'start' object is the parent of the flow) no set !");
  op->set_cpp("${type} *", "",
	      "  read_if_needed_();\n"
	      "  return _target;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "  return _target;\n", FALSE);
  
  defGetField(base_flow, _uml, weight, weight, "string", 0, 0,
	      "weight in OCL");
  defSetField(base_flow, _uml, set_Weight, weight, "str", setUmlActivityCmd, 0, 0,
	      "weight in OCL");
  defGetField(base_flow, _uml, guard, guard, "string", 0, 0,
	      "guard in OCL");
  defSetField(base_flow, _uml, set_Guard, guard, "str", setUmlGuardCmd, 0, 0,
	      "guard in OCL");
  defGetField(base_flow, _uml, selection, selection, "string", 0, 0, 
	      "selection in OCL");
  defSetField(base_flow, _uml, set_Selection, selection, "str", setUmlEntryBehaviorCmd, 0, 0,
	      "selection in OCL");
  defGetField(base_flow, _uml, transformation, transformation, "string", 0, 0, 
	      "transformation in OCL");
  defSetField(base_flow, _uml, set_Transformation, transformation, "str", setUmlExitBehaviorCmd, 0, 0,
	      "transformation in OCL");

  defGetField(base_flow, _cpp, cppWeight, weight, "string", "WITHCPP", 0,
	      "weight in C++");
  defSetField(base_flow, _cpp, set_CppWeight, weight, "str", setCppActivityCmd, 0, 0,
	      "weight in C++");
  defGetField(base_flow, _cpp, cppGuard, guard, "string", 0, 0,
	      "guard in C++");
  defSetField(base_flow, _cpp, set_CppGuard, guard, "str", setCppGuardCmd, 0, 0,
	      "guard in C++");
  defGetField(base_flow, _cpp, cppSelection, selection, "string", 0, 0,
	      "selection in C++");
  defSetField(base_flow, _cpp, set_CppSelection, selection, "str", setCppEntryBehaviorCmd, 0, 0,
	      "selection in C++");
  defGetField(base_flow, _cpp, cppTransformation, transformation, "string", 0, 0,
	      "transformation in C++");
  defSetField(base_flow, _cpp, set_CppTransformation, transformation, "str", setCppExitBehaviorCmd, 0, "endif",
	      "transformation in C++");

  defGetField(base_flow, _java, javaWeight, weight, "string", "WITHJAVA", 0,
	      "weight in Java");
  defSetField(base_flow, _java, set_JavaWeight, weight, "str", setJavaActivityCmd, 0, 0,
	      "weight in Java");
  defGetField(base_flow, _java, javaGuard, guard, "string", 0, 0,
	      "guard in Java");
  defSetField(base_flow, _java, set_JavaGuard, guard, "str", setJavaGuardCmd, 0, 0,
	      "guard in Java");
  defGetField(base_flow, _java, javaSelection, selection, "string", 0, 0,
	      "selection in Java");
  defSetField(base_flow, _java, set_JavaSelection, selection, "str", setJavaEntryBehaviorCmd, 0, 0,
	      "selection in Java");
  defGetField(base_flow, _java, javaTransformation, transformation, "string", 0, 0,
	      "transformation in Java");
  defSetField(base_flow, _java, set_JavaTransformation, transformation, "str", setJavaExitBehaviorCmd, 0, "endif",
	      "transformation in Java");
  
  op = base_flow->add_op("unload", PublicVisibility, "void");
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
  
  base_flow->add_relation(aDirectionalAssociation, "_target", PrivateVisibility,
			  user_activitynode, 0, 0);
			  
  UmlClass * flow_behavior =
    add_flow_behavior(base_class_view, base_depl_view);
  
  base_flow->add_relation(aDirectionalAggregationByValue, "_uml", PrivateVisibility,
			  flow_behavior, 0, 0);
  base_flow->add_relation(aDirectionalAggregationByValue, "_cpp", PrivateVisibility,
			  flow_behavior, "WITHCPP", "endif");
  base_flow->add_relation(aDirectionalAggregationByValue, "_java", PrivateVisibility,
			  flow_behavior, "WITHJAVA", "endif");
  
  op = base_flow->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseItem::read_uml_();\n"
	      "  _target = (UmlActivityNode *) UmlBaseItem::read_();\n"
	      "  _uml.read();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _target = (UmlActivityNode) UmlBaseItem.read_();\n"
	       "  _uml = new FlowBehavior();\n"
	       "  _uml.read();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_flow->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _cpp.read();\n", FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "", "  _cpp = new FlowBehavior();\n  _cpp.read();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_flow->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _java.read();\n", FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "", "  _java = new FlowBehavior();\n  _java.read();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  base_flow->add_constr(user_item, ProtectedVisibility);
  
  UmlCom::set_user_id(uid);
  
  op = user_flow->add_op("UmlFlow", PublicVisibility, (UmlClass *) 0, FALSE);
  op->add_param(0, InputDirection, "id", "item_id");
  op->add_param(1, InputDirection, "n", "string");
  op->set_cpp(": UmlBaseFlow(id, n)",
	      "${t0} ${p0}, const ${t1} & ${p1}",
	      "", TRUE, 0, 0);
  op->set_java("", "${t0} ${p0}, ${t1} ${p1}",
	       "  super(id, n);\n", TRUE);
}

void add_return_direction()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlClass * dir = UmlClass::get("aDirection", 0);
  
  dir->add_enum_item("ReturnDirection");
  
  UmlCom::set_user_id(uid);
}

void add_activityregion(UmlClassView * base_class_view, UmlClassView * user_class_view,
			UmlDeploymentView * base_depl_view,
			UmlDeploymentView * user_depl_view,
			UmlClass * base_item, UmlClass * user_item,
			UmlClass * user_activityitem, UmlClass * user_activitydiagram)
{
  // region
  
  UmlClass * base_activityregion;
  UmlClass * user_activityregion;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "ActivityRegion",
		  base_activityregion, user_activityregion,
		  user_activityitem);

  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  base_activityregion->add_constr(user_item, ProtectedVisibility);
  include_umlcom(base_activityregion);
  
  UmlCom::set_user_id(uid);
  
  user_activityregion->add_constr(base_activityregion, ProtectedVisibility);
  user_activityregion->set_isAbstract(TRUE);
  
  UmlCom::set_user_id(0);
  
  UmlOperation * op;
  
  add_assoc_diag_ops(base_activityregion, user_activitydiagram);
  
  base_activityregion->add_relation(aDirectionalAssociation,
				    "_assoc_diagram", PrivateVisibility,
				    user_activitydiagram, 0, 0); 
  
  op = base_activityregion->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _assoc_diagram = (UmlActivityDiagram *) UmlBaseItem::read_();\n"
	      "  UmlBaseItem::read_uml_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  _assoc_diagram = (UmlActivityDiagram) UmlBaseItem.read_();\n"
	       "  super.read_uml_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // expansion region
  
  UmlClass * base_expansionregion;
  UmlClass * user_expansionregion;
  
  user_activityregion->made(base_class_view, user_class_view,
			    base_depl_view, user_depl_view,
			    base_item, "ExpansionRegion",
			    base_expansionregion, user_expansionregion,
			    0);
  
  base_expansionregion->add_default_base_op(user_activityregion, user_expansionregion,
					    user_item, "expansion region", "anExpansionRegion");
  user_expansionregion->add_constr(base_expansionregion, PublicVisibility);
  
  UmlCom::set_user_id(0);
  
  include_umlcom(base_expansionregion);
  
  defGetBool(base_expansionregion, _must_isolate, isMustIsolate, 0, 0,
	     " return the isMustIsolate attribute, if TRUE the actions in the region execute in isolation from actions outside the region.");
  defSetBool(base_expansionregion, _must_isolate, set_isMustIsolate, setFlagCmd, 0, 0,
	     " set the isMustIsolate attribute");
  
  UmlClass * expansionkind = 
    UmlClass::made(base_class_view, base_depl_view, "anExpansionKind");

  expansionkind->set_Stereotype("enum_pattern");
  expansionkind->set_CppDecl(CppSettings::enumDecl());
  expansionkind->set_JavaDecl(JavaSettings::enumPatternDecl());
  expansionkind->add_enum_item("parallelExecution");
  expansionkind->add_enum_item("iterativeExecution");
  expansionkind->add_enum_item("streamExecution");
  
  defGetEnum(base_expansionregion, _mode, mode, expansionkind, 0, 0,
		   "mode attribute, the way in which the execution interact.");
  defSetEnum(base_expansionregion, _mode, set_Mode, expansionkind, setDefCmd, 0, 0,
		   "mode attribute");
  
  base_expansionregion->add_attribute("_must_isolate", PrivateVisibility, "bool", 0, 0);  
  base_expansionregion->add_relation(aDirectionalAggregationByValue,
				     "_mode", PrivateVisibility,
				     expansionkind, 0, 0);
  
  op = base_expansionregion->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityRegion::read_uml_();\n"
	      "  _must_isolate = UmlCom::read_bool();\n"
	      "  _mode = (anExpansionKind) UmlCom::read_char();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	      "  _must_isolate = UmlCom.read_bool();\n"
	      "  _mode = anExpansionKind.fromInt(UmlCom.read_char());\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // interruptible region
  
  UmlClass * base_interruptibleregion;
  UmlClass * user_interruptibleregion;
  
  user_activityregion->made(base_class_view, user_class_view,
			    base_depl_view, user_depl_view,
			    base_item, "InterruptibleActivityRegion",
			    base_interruptibleregion, user_interruptibleregion,
			    0);
  
  base_interruptibleregion->add_default_base_op(user_activityregion, user_interruptibleregion,
						user_item, "interruptible activity region", "anInterruptibleActivityRegion");
  user_interruptibleregion->add_constr(base_interruptibleregion, PublicVisibility);
}

void add_activityaction(UmlClassView * base_class_view, UmlClassView * user_class_view,
			UmlDeploymentView * base_depl_view,
			UmlDeploymentView * user_depl_view,
			UmlClass * base_item, UmlClass * user_item, 
			UmlClass * user_activitynode)
{
  UmlClass * user_diagram = UmlClass::get("UmlDiagram", 0);
  UmlClass * base_activityaction;
  UmlClass * user_activityaction;
  
  user_activitynode->made(base_class_view, user_class_view,
			  base_depl_view, user_depl_view,
			  base_item, "ActivityAction",
			  base_activityaction, user_activityaction,
			  0);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  base_activityaction->add_constr(user_activitynode, ProtectedVisibility);
  include_umlcom(base_activityaction);
  
  UmlCom::set_user_id(uid);
  
  user_activityaction->add_constr(base_activityaction, ProtectedVisibility);
  user_activityaction->set_isAbstract(TRUE);
  
  UmlCom::set_user_id(0);
  
  add_pre_post_conditions(base_activityaction);
  
  add_assoc_diag_ops(base_activityaction, user_diagram);

  UmlOperation * op;
  
  op = base_activityaction->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _pre_condition = 0;\n"
	      "  _post_condition = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp_pre_condition = 0;\n"
	      "  _cpp_post_condition = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_pre_condition = 0;\n"
	      "  _java_post_condition = 0;\n"
	      "#endif\n"
	      "  UmlBaseItem::unload(rec, del);\n",
	      FALSE, 0, 0);
  QCString s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _pre_condition = null;\n"
	       "  _post_condition = null;\n"
	       "  _cpp_pre_condition = null;\n"
	       "  _cpp_post_condition = null;\n"
	       "  _java_pre_condition = null;\n"
	       "  _java_post_condition = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_activityaction->add_attribute("_pre_condition", PrivateVisibility, "string",
				     0, 0);
  base_activityaction->add_attribute("_post_condition", PrivateVisibility, "string",
				     0, 0);
  base_activityaction->add_attribute("_cpp_pre_condition", PrivateVisibility, "string",
				     "WITHCPP", 0);
  base_activityaction->add_attribute("_cpp_post_condition", PrivateVisibility, "string",
				     0, "endif");
  base_activityaction->add_attribute("_java_pre_condition", PrivateVisibility, "string",
				     "WITHJAVA", 0);
  base_activityaction->add_attribute("_java_post_condition", PrivateVisibility, "string",
				     0, "endif");
  base_activityaction->add_relation(aDirectionalAssociation,
				    "_assoc_diagram", PrivateVisibility,
				    user_diagram, 0, 0);  
  
  op = base_activityaction->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _assoc_diagram = (UmlDiagram *) UmlBaseItem::read_();\n"
	      "  UmlBaseItem::read_uml_();\n"
	      "  _pre_condition = UmlCom::read_string();\n"
	      "  _post_condition = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  _assoc_diagram = (UmlDiagram) UmlBaseItem.read_();\n"
	       "  super.read_uml_();\n"
	       "  _pre_condition = UmlCom.read_string();\n"
	       "  _post_condition = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_activityaction->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _cpp_pre_condition = UmlCom::read_string();\n"
	      "  _cpp_post_condition = UmlCom::read_string();\n",
	      FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "",
	       "  _cpp_pre_condition = UmlCom.read_string();\n"
	       "  _cpp_post_condition = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_activityaction->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	       "  _java_pre_condition = UmlCom::read_string();\n"
	       "  _java_post_condition = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "",
	       "  _java_pre_condition = UmlCom.read_string();\n"
	       "  _java_post_condition = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // the artifacts containing all
  
  UmlCom::set_user_id(0);
  
  UmlArtifact * base_art = UmlArtifact::made(base_depl_view, "UmlBaseActivityActionClasses");
  
  UmlCom::set_user_id(uid);
  
  UmlArtifact * user_art = UmlArtifact::made(user_depl_view, "UmlActivityActionClasses");
  
  include_umlcom(base_art);
  
  // send object
  
  UmlClass * base_sendobject;
  UmlClass * user_sendobject;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "SendObjectAction",
			    base_sendobject, user_sendobject,
			    0);
  
  base_sendobject->add_default_base_op(user_activityaction, user_sendobject,
				       user_item,
				       "send object action", "aSendObjectAction");
  user_sendobject->add_constr(base_sendobject, PublicVisibility);

  // unmarshall action
  
  UmlClass * base_unmarshall;
  UmlClass * user_unmarshall;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "UnmarshallAction",
			    base_unmarshall, user_unmarshall,
			    0);
  
  base_unmarshall->add_default_base_op(user_activityaction, user_unmarshall,
				       user_item,
				       "unmarshall action", "anUnmarshallAction");
  user_unmarshall->add_constr(base_unmarshall, PublicVisibility);

  // on signal action
  
  UmlClass * base_onsignal;
  UmlClass * user_onsignal;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_depl_view, user_depl_view,
			    base_item, "OnSignalAction",
			    base_onsignal, user_onsignal,
			    0);
  
  UmlCom::set_user_id(0);
  
  base_onsignal->add_constr(user_activityaction, ProtectedVisibility);
  include_umlcom(base_onsignal);
  
  UmlCom::set_user_id(uid);
  
  user_onsignal->add_constr(base_onsignal, ProtectedVisibility);
  user_onsignal->set_isAbstract(TRUE);

  UmlCom::set_user_id(0);
  
  defGet(base_onsignal, _signal, signal, "string", 0, 0,
	 "signal");
  defSet(base_onsignal, _signal, set_Signal, "str", setUmlActivityCmd, 0, 0,
	 "signal");
  defGet(base_onsignal, _cpp_signal, cppSignal, "string", "WITHCPP", 0,
	 "signal in C++");
  defSet(base_onsignal, _cpp_signal, set_CppSignal, "str", setCppActivityCmd, 0, "#endif",
	 "signal in C++");
  defGet(base_onsignal, _java_signal, javaSignal, "string", "WITHJAVA", 0,
	 "signal in Java");
  defSet(base_onsignal, _java_signal, set_JavaSignal, "str", setJavaActivityCmd, 0, "#endif",
	 "signal in Java");

  op = base_onsignal->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _signal = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp_signal = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_signal = 0;\n"
	      "#endif\n"
	      "  UmlBaseActivityAction::unload(rec, del);\n",
	      FALSE, 0, 0);
  s = op->cppDecl();
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _signal = null;\n"
	       "  _cpp_signal = null;\n"
	       "  _java_signal = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_onsignal->add_attribute("_signal", PrivateVisibility, "string",
				 0, 0);
  base_onsignal->add_attribute("_cpp_signal", PrivateVisibility, "string",
				 "WITHCPP", "endif");
  base_onsignal->add_attribute("_java_signal", PrivateVisibility, "string",
				 "WITHJAVA", "endif");

  op = base_onsignal->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _signal = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _signal = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_onsignal->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_cpp_();\n"
	      "  _cpp_signal = UmlCom::read_string();\n",
	      FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "",
	       "  super.read_cpp_();\n"
	       "  _cpp_signal = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_onsignal->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_java_();\n"
	      "  _java_signal = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "",
	       "  super.read_java_();\n"
	       "  _java_signal = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // send signal action
  
  UmlClass * base_sendsignal;
  UmlClass * user_sendsignal;

  
  user_onsignal->made(base_class_view, user_class_view,
		      base_art, user_art,
		      base_item, "SendSignalAction",
		      base_sendsignal, user_sendsignal,
		      0);
  
  base_sendsignal->add_default_base_op(user_onsignal, user_sendsignal,
				       user_item,
				       "send signal action", "aSendSignalAction");
  user_sendsignal->add_constr(base_sendsignal, PublicVisibility);
  
  // broadcast signal action
  
  UmlClass * base_broadcastsignal;
  UmlClass * user_broadcastsignal;

  
  user_onsignal->made(base_class_view, user_class_view,
		      base_art, user_art,
		      base_item, "BroadcastSignalAction",
		      base_broadcastsignal, user_broadcastsignal,
		      0);
  
  base_broadcastsignal->add_default_base_op(user_onsignal, user_broadcastsignal,
					    user_item,
					    "broadcast signal action", "aBroadcastSignalAction");
  user_broadcastsignal->add_constr(base_broadcastsignal, PublicVisibility);
  
  // value specification action
  
  UmlClass * base_valuespecification;
  UmlClass * user_valuespecification;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "ValueSpecificationAction",
			    base_valuespecification, user_valuespecification,
			    0);
  
  base_valuespecification->add_default_base_op(user_activityaction, user_valuespecification,
					       user_item,
					       "value specification action", "aValueSpecificationAction");
  user_valuespecification->add_constr(base_valuespecification, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGet(base_valuespecification, _value, value, "string", 0, 0,
	 "value");
  defSet(base_valuespecification, _value, set_Value, "str", setUmlActivityCmd, 0, 0,
	 "value");
  defGet(base_valuespecification, _cpp_value, cppValue, "string", "WITHCPP", 0,
	 "value in C++");
  defSet(base_valuespecification, _cpp_value, set_CppValue, "str", setCppActivityCmd, 0, "#endif",
	 "value in C++");
  defGet(base_valuespecification, _java_value, javaValue, "string", "WITHJAVA", 0,
	 "value in Java");
  defSet(base_valuespecification, _java_value, set_JavaValue, "str", setJavaActivityCmd, 0, "#endif",
	 "value in Java");

  op = base_valuespecification->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _value = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp_value = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_value = 0;\n"
	      "#endif\n"
	      "  UmlBaseActivityAction::unload(rec, del);\n",
	      FALSE, 0, 0);
  s = op->cppDecl();
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _value = null;\n"
	       "  _cpp_value = null;\n"
	       "  _java_value = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_valuespecification->add_attribute("_value", PrivateVisibility, "string",
					 0, 0);
  base_valuespecification->add_attribute("_cpp_value", PrivateVisibility, "string",
					 "WITHCPP", "endif");
  base_valuespecification->add_attribute("_java_value", PrivateVisibility, "string",
					 "WITHJAVA", "endif");

  op = base_valuespecification->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _value = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _value = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_valuespecification->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_cpp_();\n"
	      "  _cpp_value = UmlCom::read_string();\n",
	      FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "",
	       "  super.read_cpp_();\n"
	       "  _cpp_value = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_valuespecification->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_java_();\n"
	      "  _java_value = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "",
	       "  super.read_java_();\n"
	       "  _java_value = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // opaque action
  
  UmlClass * base_opaque;
  UmlClass * user_opaque;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "OpaqueAction",
			    base_opaque, user_opaque,
			    0);
  
  base_opaque->add_default_base_op(user_activityaction, user_opaque,
				   user_item,
				   "opaque action", "anOpaqueAction");
  user_opaque->add_constr(base_opaque, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGet(base_opaque, _behavior, behavior, "string", 0, 0,
	 "behavior");
  defSet(base_opaque, _behavior, set_Behavior, "str", setUmlActivityCmd, 0, 0,
	 "behavior");
  defGet(base_opaque, _cpp_behavior, cppBehavior, "string", "WITHCPP", 0,
	 "behavior in C++");
  defSet(base_opaque, _cpp_behavior, set_CppBehavior, "str", setCppActivityCmd, 0, "#endif",
	 "behavior in C++");
  defGet(base_opaque, _java_behavior, javaBehavior, "string", "WITHJAVA", 0,
	 "behavior in Java");
  defSet(base_opaque, _java_behavior, set_JavaBehavior, "str", setJavaActivityCmd, 0, "#endif",
	 "behavior in Java");

  op = base_opaque->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _behavior = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp_behavior = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_behavior = 0;\n"
	      "#endif\n"
	      "  UmlBaseActivityAction::unload(rec, del);\n",
	      FALSE, 0, 0);
  s = op->cppDecl();
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _behavior = null;\n"
	       "  _cpp_behavior = null;\n"
	       "  _java_behavior = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_opaque->add_attribute("_behavior", PrivateVisibility, "string",
			     0, 0);
  base_opaque->add_attribute("_cpp_behavior", PrivateVisibility, "string",
			     "WITHCPP", "endif");
  base_opaque->add_attribute("_java_behavior", PrivateVisibility, "string",
			     "WITHJAVA", "endif");

  op = base_opaque->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _behavior = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _behavior = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_opaque->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_cpp_();\n"
	      "  _cpp_behavior = UmlCom::read_string();\n",
	      FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "",
	       "  super.read_cpp_();\n"
	       "  _cpp_behavior = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_opaque->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_java_();\n"
	      "  _java_behavior = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "",
	       "  super.read_java_();\n"
	       "  _java_behavior = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // accept event action
  
  UmlClass * base_acceptevent;
  UmlClass * user_acceptevent;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "AcceptEventAction",
			    base_acceptevent, user_acceptevent,
			    0);
  
  base_acceptevent->add_default_base_op(user_activityaction, user_acceptevent,
					user_item,
					"accept event action", "anAcceptEventAction");
  user_acceptevent->add_constr(base_acceptevent, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGetBool(base_acceptevent, _unmarshall, isUnmarshall, 0, 0,
	     " return the isUnmarshall attribute, if TRUE there are multiple output pins for attributes or the event.");
  defSetBool(base_acceptevent, _unmarshall, set_isUnmarshall, setUnmarshallCmd, 0, 0,
	     " set the isUnmarshall attribute");
  
  defGetBool(base_acceptevent, _timeevent, isTimeEvent, 0, 0,
	     " return the isTimeEvent attribute, if TRUE the event is a time event");
  defSetBool(base_acceptevent, _timeevent, set_isTimeEvent, setTimeEventCmd, 0, 0,
	     " set the isTimeEvent attribute");
  
  defGet(base_acceptevent, _trigger, trigger, "string", 0, 0,
	 "trigger");
  defSet(base_acceptevent, _trigger, set_Trigger, "str", setUmlTriggerCmd, 0, 0,
	 "trigger");
  defGet(base_acceptevent, _cpp_trigger, cppTrigger, "string", "WITHCPP", 0,
	 "trigger in C++");
  defSet(base_acceptevent, _cpp_trigger, set_CppTrigger, "str", setCppTriggerCmd, 0, "#endif",
	 "trigger in C++");
  defGet(base_acceptevent, _java_trigger, javaTrigger, "string", "WITHJAVA", 0,
	 "trigger in Java");
  defSet(base_acceptevent, _java_trigger, set_JavaTrigger, "str", setJavaTriggerCmd, 0, "#endif",
	 "trigger in Java");

  op = base_acceptevent->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _trigger = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp_trigger = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_trigger = 0;\n"
	      "#endif\n"
	      "  UmlBaseActivityAction::unload(rec, del);\n",
	      FALSE, 0, 0);
  s = op->cppDecl();
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _trigger = null;\n"
	       "  _cpp_trigger = null;\n"
	       "  _java_trigger = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_acceptevent->add_attribute("_unmarshall", PrivateVisibility, "bool",
				  0, 0);
  base_acceptevent->add_attribute("_timeevent", PrivateVisibility, "bool",
				  0, 0);
  
  base_acceptevent->add_attribute("_trigger", PrivateVisibility, "string",
				  0, 0);
  base_acceptevent->add_attribute("_cpp_trigger", PrivateVisibility, "string",
				  "WITHCPP", "endif");
  base_acceptevent->add_attribute("_java_trigger", PrivateVisibility, "string",
				  "WITHJAVA", "endif");

  op = base_acceptevent->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _unmarshall = UmlCom::read_bool();\n"
	      "  _timeevent = UmlCom::read_bool();\n"
	      "  _trigger = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _unmarshall = UmlCom.read_bool();\n"
	       "  _timeevent = UmlCom.read_bool();\n"
	       "  _trigger = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_acceptevent->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_cpp_();\n"
	      "  _cpp_trigger = UmlCom::read_string();\n",
	      FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "",
	       "  super.read_cpp_();\n"
	       "  _cpp_trigger = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_acceptevent->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_java_();\n"
	      "  _java_trigger = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "",
	       "  super.read_java_();\n"
	       "  _java_trigger = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // call operation action
  
  UmlClass * base_calloperation;
  UmlClass * user_calloperation;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "CallOperationAction",
			    base_calloperation, user_calloperation,
			    0);
  
  base_calloperation->add_default_base_op(user_activityaction, user_calloperation,
					  user_item,
					  "call operation action", "aCallOperationAction");
  user_calloperation->add_constr(base_calloperation, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGetBool(base_calloperation, _synchronous, isSynchronous, 0, 0,
	     " return the isSynchronous attribute, if TRUE the caller waits for the completion of the invoked behavior");
  defSetBool(base_calloperation, _synchronous, set_isSynchronous, setFlagCmd, 0, 0,
	     " set the isSynchronous attribute");
  
  UmlClass * user_operation = UmlClass::get("UmlOperation", 0);
  
  defGetPtr(base_calloperation, _operation, operation, user_operation, 0, 0,
	    "operation");
  defSetPtr(base_calloperation, _operation, set_Operation, user_operation, setDefCmd, 0, 0,
	    "operation");

  base_calloperation->add_attribute("_synchronous", PrivateVisibility, "bool",
				    0, 0);

  base_calloperation->add_relation(aDirectionalAssociation,
				   "_operation", PrivateVisibility,
				   user_operation, 0, 0);  
  
  op = base_calloperation->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _synchronous = UmlCom::read_bool();\n"
	      "  _operation = (UmlOperation *) UmlBaseItem::read_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _synchronous = UmlCom.read_bool();\n"
	       "  _operation = (UmlOperation) UmlBaseItem.read_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // call behavior action
  
  UmlClass * base_callbehavior;
  UmlClass * user_callbehavior;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "CallBehaviorAction",
			    base_callbehavior, user_callbehavior,
			    0);
  
  base_callbehavior->add_default_base_op(user_activityaction, user_callbehavior,
					 user_item,
					 "call behavior action", "aCallBehaviorAction");
  user_callbehavior->add_constr(base_callbehavior, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGetBool(base_callbehavior, _synchronous, isSynchronous, 0, 0,
	     " return the isSynchronous attribute, if TRUE the caller waits for the completion of the invoked behavior");
  defSetBool(base_callbehavior, _synchronous, set_isSynchronous, setFlagCmd, 0, 0,
	     " set the isSynchronous attribute");
  
  defGetPtr(base_callbehavior, _behavior, behavior, user_item, 0, 0,
	    "behavior, may be an activity or a state machine");
  defSetPtr(base_callbehavior, _behavior, set_Behavior, user_item, setDefCmd, 0, 0,
	    "behavior");

  base_callbehavior->add_attribute("_synchronous", PrivateVisibility, "bool",
				   0, 0);

  base_callbehavior->add_relation(aDirectionalAssociation,
				  "_behavior", PrivateVisibility,
				  user_item, 0, 0);  
  
  op = base_callbehavior->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _synchronous = UmlCom::read_bool();\n"
	      "  _behavior = UmlBaseItem::read_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _synchronous = UmlCom.read_bool();\n"
	       "  _behavior = UmlBaseItem.read_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // access variable value action
  
  UmlClass * base_accessvariablevalue;
  UmlClass * user_accessvariablevalue;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_depl_view, user_depl_view,
			    base_item, "AccessVariableValueAction",
			    base_accessvariablevalue, user_accessvariablevalue,
			    0);
  

  UmlCom::set_user_id(0);
  
  base_accessvariablevalue->add_constr(user_activityaction, ProtectedVisibility);
  include_umlcom(base_accessvariablevalue);

  UmlCom::set_user_id(uid);
  
  user_accessvariablevalue->add_constr(base_accessvariablevalue, ProtectedVisibility);
  user_accessvariablevalue->set_isAbstract(TRUE);

  UmlCom::set_user_id(0);
  
  defGetPtr(base_accessvariablevalue, _variable, variable, user_item, 0, 0,
	    "variable, may be an attribute or a relation");
  defSetPtr(base_accessvariablevalue, _variable, set_Variable, user_item, setDefCmd, 0, 0,
	    "variable");

  base_accessvariablevalue->add_relation(aDirectionalAssociation,
					 "_variable", PrivateVisibility,
					 user_item, 0, 0);  
  
  op = base_accessvariablevalue->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _variable = UmlBaseItem::read_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _variable = UmlBaseItem.read_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // clear variable value action
  
  UmlClass * base_clearvariablevalue;
  UmlClass * user_clearvariablevalue;
  
  user_accessvariablevalue->made(base_class_view, user_class_view,
				 base_art, user_art,
				 base_item, "ClearVariableValueAction",
				 base_clearvariablevalue, user_clearvariablevalue,
				 0);
  
  base_clearvariablevalue->add_default_base_op(user_accessvariablevalue, user_clearvariablevalue,
					       user_item,
					       "clear variable value action", "aClearVariableValueAction");
  user_clearvariablevalue->add_constr(base_clearvariablevalue, PublicVisibility);
  
  // read variable value action
  
  UmlClass * base_readvariablevalue;
  UmlClass * user_readvariablevalue;
  
  user_accessvariablevalue->made(base_class_view, user_class_view,
				 base_art, user_art,
				 base_item, "ReadVariableValueAction",
				 base_readvariablevalue, user_readvariablevalue,
				 0);
  
  base_readvariablevalue->add_default_base_op(user_accessvariablevalue, user_readvariablevalue,
					      user_item,
					      "read variable value action", "aReadVariableValueAction");
  user_readvariablevalue->add_constr(base_readvariablevalue, PublicVisibility);
  
  // write variable value action
  
  UmlClass * base_writevariablevalue;
  UmlClass * user_writevariablevalue;
  
  user_accessvariablevalue->made(base_class_view, user_class_view,
				 base_art, user_art,
				 base_item, "WriteVariableValueAction",
				 base_writevariablevalue, user_writevariablevalue,
				 0);
  
  base_writevariablevalue->add_default_base_op(user_accessvariablevalue, user_writevariablevalue,
					       user_item,
					       "write variable value action", "aWriteVariableValueAction");
  user_writevariablevalue->add_constr(base_writevariablevalue, PublicVisibility);
  
  // add variable value action
  
  UmlClass * base_addvariablevalue;
  UmlClass * user_addvariablevalue;
  
  user_accessvariablevalue->made(base_class_view, user_class_view,
				 base_art, user_art,
				 base_item, "AddVariableValueAction",
				 base_addvariablevalue, user_addvariablevalue,
				 0);
  
  base_addvariablevalue->add_default_base_op(user_accessvariablevalue, user_addvariablevalue,
					     user_item,
					     "add variable value action", "anAddVariableValueAction");
  user_addvariablevalue->add_constr(base_addvariablevalue, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGetBool(base_addvariablevalue, _replace_all, isReplaceAll, 0, 0,
	     " return the isReplaceAll attribute, if TRUE existing values of the variable must be removed before adding the new value");
  defSetBool(base_addvariablevalue, _replace_all, set_isReplaceAll, setFlagCmd, 0, 0,
	     " set the isReplaceAll attribute");
  
  base_addvariablevalue->add_attribute("_replace_all", PrivateVisibility, "bool",
				  0, 0);

  op = base_addvariablevalue->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _replace_all = UmlCom::read_bool();\n"
	      "  UmlBaseAccessVariableValueAction::read_uml_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	      "  _replace_all = UmlCom.read_bool();\n"
	       "  super.read_uml_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // remove variable value action
  
  UmlClass * base_removevariablevalue;
  UmlClass * user_removevariablevalue;
  
  user_accessvariablevalue->made(base_class_view, user_class_view,
				 base_art, user_art,
				 base_item, "RemoveVariableValueAction",
				 base_removevariablevalue, user_removevariablevalue,
				 0);
  
  base_removevariablevalue->add_default_base_op(user_accessvariablevalue, user_removevariablevalue,
						user_item,
						"remove variable value action", "aRemoveVariableValueAction");
  user_removevariablevalue->add_constr(base_removevariablevalue, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGetBool(base_removevariablevalue, _remove_duplicates, isRemoveDuplicates, 0, 0,
	     " return the isRemoveDuplicates attribute, if TRUE remove duplicates of the value if non-unique");
  defSetBool(base_removevariablevalue, _remove_duplicates, set_isRemoveDuplicates, setFlagCmd, 0, 0,
	     " set the isRemoveDuplicates attribute");
  
  base_removevariablevalue->add_attribute("_remove_duplicates", PrivateVisibility, "bool",
					  0, 0);

  op = base_removevariablevalue->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _remove_duplicates = UmlCom::read_bool();\n"
	      "  UmlBaseAccessVariableValueAction::read_uml_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	      "  _remove_duplicates = UmlCom.read_bool();\n"
	       "  super.read_uml_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
}

void add_activitycontrolnode(UmlClassView * base_class_view, UmlClassView * user_class_view,
			     UmlDeploymentView * base_depl_view,
			     UmlDeploymentView * user_depl_view,
			     UmlClass * base_item, UmlClass * user_item, 
			     UmlClass * user_activitynode)
{
  UmlClass * base_activitycontrolnode;
  UmlClass * user_activitycontrolnode;
  
  user_activitynode->made(base_class_view, user_class_view,
			  base_depl_view, user_depl_view,
			  base_item, "ActivityControlNode",
			  base_activitycontrolnode, user_activitycontrolnode,
			  0);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  base_activitycontrolnode->add_constr(user_activitynode, ProtectedVisibility);
  
  UmlCom::set_user_id(uid);
  
  user_activitycontrolnode->add_constr(base_activitycontrolnode, ProtectedVisibility);
  user_activitycontrolnode->set_isAbstract(TRUE);
  
  // the artifacts containing all
  UmlCom::set_user_id(0);
  
  UmlArtifact * base_art = UmlArtifact::made(base_depl_view, "UmlBaseActivityControlNodeClasses");
  
  UmlCom::set_user_id(uid);
  
  UmlArtifact * user_art = UmlArtifact::made(user_depl_view, "UmlActivityControlNodeClasses");
  
  include_umlcom(base_art);
  
#define DefControlNode(n, k, s)	\
  UmlClass * base_##n;	\
  UmlClass * user_##n;	\
  	\
  user_activitycontrolnode->made(base_class_view, user_class_view,	\
				 base_art, user_art,	\
				 base_item, #n,	\
				 base_##n, user_##n,	\
				 0);	\
  	\
  base_##n->add_default_base_op(user_activitycontrolnode, user_##n,	\
  			       user_item,	\
			       s " activity control node", #k);	\
  user_##n->add_constr(base_##n, PublicVisibility)

  DefControlNode(InitialActivityNode, anInitialActivityNode, "initial");
  DefControlNode(FlowFinalActivityNode, aFlowFinalActivityNode, "flow final");
  DefControlNode(ActivityFinalActivityNode, anActivityFinalActivityNode, "activity final");
  DefControlNode(DecisionActivityNode, aDecisionActivityNode, "decision");
  DefControlNode(MergeActivityNode, aMergeActivityNode, "merge");
  DefControlNode(ForkActivityNode, aForkActivityNode, "fork");
  DefControlNode(JoinActivityNode, aJoinActivityNode, "join");
}
  
UmlClass * add_activityobject(UmlClassView * base_class_view, UmlClassView * user_class_view,
			      UmlDeploymentView * base_depl_view,
			      UmlDeploymentView * user_depl_view,
			      UmlClass * base_item, UmlClass * user_item,
			      UmlClass * user_activitynode,
			      UmlClass * user_activitydiagram)
{
  UmlClass * base_activityobject;
  UmlClass * user_activityobject;
  
  user_activitynode->made(base_class_view, user_class_view,
			  base_depl_view, user_depl_view,
			  base_item, "ActivityObject",
			  base_activityobject, user_activityobject,
			  0);
  base_activityobject->add_default_base_op(user_activitynode, user_activityobject,
					   user_item, "activity object", "anActivityObject");
  user_activityobject->add_constr(base_activityobject, PublicVisibility);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  include_umlcom(base_activityobject);
  
  UmlClass * ordering = 
    UmlClass::made(base_class_view, base_depl_view, "anOrdering");

  ordering->set_Stereotype("enum_pattern");
  ordering->set_CppDecl(CppSettings::enumDecl());
  ordering->set_JavaDecl(JavaSettings::enumPatternDecl());
  ordering->add_enum_item("unordered");
  ordering->add_enum_item("ordered");
  ordering->add_enum_item("lifo");
  ordering->add_enum_item("fifo");
  
  UmlClass * typespec = UmlClass::get("UmlTypeSpec", 0);
  UmlOperation * op;
  
  defGet(base_activityobject, _type, type, typespec, 0, 0,
         "type");
  defSetRefType(base_activityobject, _type, set_Type, typespec, setTypeCmd, 0, 0,
		"type");
  
  defGet(base_activityobject, _multiplicity, multiplicity, "string", 0, 0,
	 "multiplicity");
  defSet(base_activityobject, _multiplicity, set_Multiplicity, "str", setMultiplicityCmd, 0, 0,
	 "multiplicity");

  defGet(base_activityobject, _in_state, inState, "string", 0, 0,
	 "inState attribute");
  defSet(base_activityobject, _in_state, set_InState, "str", setInStateCmd, 0, 0,
	 "inState attribute");

  defGet(base_activityobject, _selection, selection, "string", 0, 0,
	 "selection in OCL");
  defSet(base_activityobject, _selection, set_Selection, "str", setUmlActivityCmd, 0, 0,
	 "selection in OCL");
  defGet(base_activityobject, _cpp_selection, cppSelection, "string", "WITHCPP", 0,
	 "selection in C++");
  defSet(base_activityobject, _cpp_selection, set_CppSelection, "str", setCppActivityCmd, 0, "endif",
	 "selection in C++");
  defGet(base_activityobject, _java_selection, javaSelection, "string", "WITHJAVA", 0,
	 "selection in Java");
  defSet(base_activityobject, _java_selection, set_JavaSelection, "str", setJavaActivityCmd, 0, "endif",
	 "selection in Java");

  defGetEnum(base_activityobject, _ordering, ordering, ordering, 0, 0,
		   "ordering");
  defSetEnum(base_activityobject, _ordering, set_Ordering, ordering,
	     setOrderingCmd, 0, 0, "ordering");
  
  defGetBool(base_activityobject, _is_control, isControlType, 0, 0,
	     " return the isControlType attribute, tells whether the type of the object node is to be treated as control");
  defSetBoolBitField(base_activityobject, _is_control, set_IsControlType,
		     setFlagCmd, 0, 0, " set the isControlType attribute");
  
  add_assoc_diag_ops(base_activityobject, user_activitydiagram);

  op = base_activityobject->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _type.explicit_type = 0;\n"
	      "  _multiplicity = 0;\n"
	      "  _in_state = 0;\n"
	      "  _selection = 0;\n"
	      "#ifdef WINCPP\n"
	      "  _cpp_selection = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_selection = 0;\n"
	      "#endif\n"
	      "  UmlBaseActivityNode::unload(rec, del);\n",
	      FALSE, 0, 0);

  QCString s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _type = null;\n"
	       "  _multiplicity = null;\n"
	       "  _in_state = null;\n"
	       "  _selection = null;\n"
	       "  _cpp_selection = null;\n"
	       "  _java_selection = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
  
  base_activityobject->add_relation(aDirectionalAssociation,
				    "_assoc_diagram", PrivateVisibility,
				    user_activitydiagram, 0, 0);
  
  base_activityobject->add_relation(aDirectionalAggregationByValue,
				     "_type", PrivateVisibility,
				     typespec, 0, 0);
  base_activityobject->add_attribute("_multiplicity", PrivateVisibility, "string", 0, 0);  
  base_activityobject->add_attribute("_in_state", PrivateVisibility, "string", 0, 0);  
  base_activityobject->add_attribute("_selection", PrivateVisibility, "string", 0, 0);  
  base_activityobject->add_attribute("_cpp_selection", PrivateVisibility, "string", "WITHCPP", "endif");  
  base_activityobject->add_attribute("_java_selection", PrivateVisibility, "string", "WITHJAVA", "endif");  
  
  base_activityobject->add_relation(aDirectionalAggregationByValue,
				    "_ordering", PrivateVisibility,
				    ordering, 0, 0, " : 8");
  
  base_activityobject->add_attribute("_is_control", PrivateVisibility, "bool", 0, 0, " : 1");  
  
  op = base_activityobject->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _assoc_diagram = (kind() == anActivityObject) ? (UmlActivityDiagram *) UmlBaseItem::read_() : (UmlActivityDiagram *) 0;\n"
	      "  UmlBaseActivityNode::read_uml_();\n"
	      "  _type.type = (UmlClass *) UmlBaseItem::read_();\n"
	      "  if (_type.type == 0)\n"
	      "    _type.explicit_type = UmlCom::read_string();\n"
	      "  _multiplicity = UmlCom::read_string();\n"
	      "  _in_state = UmlCom::read_string();\n"
	      "  _selection = UmlCom::read_string();\n"
	      "  _ordering = (anOrdering) UmlCom::read_char();\n"
	      "  _is_control = UmlCom::read_bool();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  _assoc_diagram = (kind() == anItemKind.anActivityObject) ? (UmlActivityDiagram) UmlBaseItem.read_() : null;\n"
	       "  super.read_uml_();\n"
	       "  _type = new UmlTypeSpec();\n"
	       "  _type.type = (UmlClass) UmlBaseItem.read_();\n"
	       "  if (_type.type == null)\n"
	       "    _type.explicit_type = UmlCom.read_string();\n"
	       "  _multiplicity = UmlCom.read_string();\n"
	       "  _in_state = UmlCom.read_string();\n"
	       "  _selection = UmlCom.read_string();\n"
	       "  _ordering = anOrdering.fromInt(UmlCom.read_char());\n"
	       "  _is_control = UmlCom.read_bool();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);

  op = base_activityobject->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _cpp_selection = UmlCom::read_string();\n", FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "", "  _cpp_selection  = UmlCom.read_string();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_activityobject->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "", "  _java_selection = UmlCom::read_string();\n", FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "", "  _java_selection = UmlCom.read_string();\n", FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  return user_activityobject;
}

void add_expansionnode(UmlClassView * base_class_view, UmlClassView * user_class_view,
		       UmlDeploymentView * base_depl_view,
		       UmlDeploymentView * user_depl_view,
		       UmlClass * base_item, UmlClass * user_activityobject)
{
  UmlClass * base_expansionnode;
  UmlClass * user_expansionnode;
  
  user_activityobject->made(base_class_view, user_class_view,
			    base_depl_view, user_depl_view,
			    base_item, "ExpansionNode",
			    base_expansionnode, user_expansionnode,
			    0);
  base_expansionnode->add_default_base_op(user_activityobject, user_expansionnode,
					  UmlClass::get("UmlExpansionRegion", 0), 
					  "expansion node", "anExpansionNode");
  user_expansionnode->add_constr(base_expansionnode, PublicVisibility);
}

void add_pinparam(UmlClassView * base_class_view, UmlClassView * user_class_view,
		  UmlDeploymentView * base_depl_view,
		  UmlDeploymentView * user_depl_view,
		  UmlClass * base_item, UmlClass * user_item, 
		  UmlClass * user_activityobject)
{
  // UmlPinParameter
  
  UmlClass * base_pinparam;
  UmlClass * user_pinparam;
  
  user_activityobject->made(base_class_view, user_class_view,
			    base_depl_view, user_depl_view,
			    base_item, "PinParameter",
			    base_pinparam, user_pinparam,
			    0);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  base_pinparam->add_constr(user_activityobject, ProtectedVisibility);
  include_umlcom(base_pinparam);
  
  UmlCom::set_user_id(uid);
  
  user_pinparam->add_constr(base_pinparam, ProtectedVisibility);
  user_pinparam->set_isAbstract(TRUE);
  
  UmlCom::set_user_id(0);
  
  UmlClass * effect = 
    UmlClass::made(base_class_view, base_depl_view, "aParameterEffectKind");

  effect->set_Stereotype("enum_pattern");
  effect->set_CppDecl(CppSettings::enumDecl());
  effect->set_JavaDecl(JavaSettings::enumPatternDecl());
  effect->add_enum_item("noEffect");
  effect->add_enum_item("createEffect");
  effect->add_enum_item("readEffect");
  effect->add_enum_item("updateEffect");
  effect->add_enum_item("deleteEffect");
  
  UmlOperation * op;
  
  defGetBool(base_pinparam, _unique, isUnique, 0, 0,
	     " return the isUnique attribute");
  defSetBoolBitField(base_pinparam, _unique, set_IsUnique, 
		     setUniqueCmd, 0, 0, " set the isUnique attribute");

  defGetBool(base_pinparam, _exception, isException, 0, 0,
	     " return the isException attribute, exclusive with isStream");
  defSetBoolBitField(base_pinparam, _exception, set_IsException,
		     replaceExceptionCmd, 0, 0,
		     " set the isException attribute, exclusive with isStream");

  UmlClass * dir = UmlClass::get("aDirection", 0);
  
  defGetEnum(base_pinparam, _dir, direction, dir, 0, 0, "direction");
  defSetEnum(base_pinparam, _dir, set_Direction, dir, setIdlDirCmd, 0, 0, "direction");

  defGetEnum(base_pinparam, _effect, effect, effect, 0, 0,
		   "effect");
  defSetEnum(base_pinparam, _effect, set_Effect, effect, replaceParameterCmd, 0, 0, "effect");

  UmlAttribute * att;
  QCString s;
  
  base_pinparam->add_attribute("_unique", PrivateVisibility, "bool", 0, 0, " : 1");    
  base_pinparam->add_attribute("_exception", PrivateVisibility, "bool", 0, 0, " : 1");  
  base_pinparam->add_attribute("_stream", PrivateVisibility, "bool", 0, 0, " : 1");  
  att = base_pinparam->add_attribute("_dummy", PrivateVisibility, "int", 0, 0, " : 4");  
  att->set_JavaDecl("");
  
  base_pinparam->add_relation(aDirectionalAggregationByValue,
			      "_dir", PrivateVisibility, dir, 0, 0, " : 8");
  base_pinparam->add_relation(aDirectionalAggregationByValue,
			      "_effect", PrivateVisibility, effect, 0, 0, " : 8");
  
  op = base_pinparam->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityObject::read_uml_();\n"
	      "  _unique = UmlCom::read_bool();\n"
	      "  _exception = UmlCom::read_bool();\n"
	      "  _stream = UmlCom::read_bool();\n"
	      "  _dir = (aDirection) UmlCom::read_char();\n"
	      "  _effect = (aParameterEffectKind) UmlCom::read_char();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _unique = UmlCom.read_bool();\n"
	       "  _exception = UmlCom.read_bool();\n"
	       "  _stream = UmlCom.read_bool();\n"
	       "  _dir = aDirection.fromInt(UmlCom.read_char());\n"
	       "  _effect = aParameterEffectKind.fromInt(UmlCom.read_char());\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // pin
  
  UmlClass * base_activitypin;
  UmlClass * user_activitypin;
  UmlClass * user_activityaction = UmlClass::get("UmlActivityAction", 0);
  
  user_pinparam->made(base_class_view, user_class_view,
		      base_depl_view, user_depl_view,
		      base_item, "ActivityPin",
		      base_activitypin, user_activitypin,
		      0);
  
  base_activitypin->add_default_base_op(user_pinparam, user_activitypin,
					user_activityaction,
					"activity action pin", "anActivityPin");
  user_activitypin->add_constr(base_activitypin, PublicVisibility);
  
  // activity parameter
  
  UmlClass * base_activityparameter;
  UmlClass * user_activityparameter;
  
  user_pinparam->made(base_class_view, user_class_view,
		      base_depl_view, user_depl_view,
		      base_item, "ActivityParameter",
		      base_activityparameter, user_activityparameter,
		      0);
  
  base_activityparameter->add_default_base_op(user_pinparam, user_activityparameter,
					UmlClass::get("UmlActivity", 0),
					"activity parameter", "anActivityParameter");
  user_activityparameter->add_constr(base_activityparameter, PublicVisibility);
  
  UmlCom::set_user_id(0);
  
  include_umlcom(base_activityparameter);
  
  defGet(base_activityparameter, _default_value, defaultValue, "string", 0, 0,
	 "default value");
  defSet(base_activityparameter, _default_value, set_DefaultValue, "str",
	 setDefaultValueCmd, 0, 0, "default value");
  
  op = base_activityparameter->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _default_value = 0;\n"
	      "  UmlBaseActivityObject::unload(rec, del);\n",
	      FALSE, 0, 0);

  s = op->cppDecl();
  
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _default_value = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
  
  base_activityparameter->add_attribute("_default_value", PrivateVisibility, "string", 0, 0);  
  
  op = base_activityparameter->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBasePinParameter::read_uml_();\n"
	      "  _default_value = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _default_value = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  UmlCom::set_user_id(uid);
  
  // parameter set

  UmlClass * base_parameterset;
  UmlClass * user_parameterset;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "ParameterSet",
		  base_parameterset, user_parameterset,
		  0);
  
  base_parameterset->add_default_base_op(user_item, user_activityparameter,
					 user_activityaction,
					 "parameter set", "aParameterSet");
  user_parameterset->add_constr(base_parameterset, PublicVisibility);
  
  UmlCom::set_user_id(0);
  
  include_umlcom(base_parameterset);
  
  op = base_parameterset->add_op("pins", PublicVisibility, user_activitypin);
  op->set_Description(" return the pins part of the parameter set");
  op->set_cpp("const QVector<${type}> &", "",
	      "  read_if_needed_();\n"
	      "  return _pins;\n",
	      FALSE, 0, 0);
  op->set_java("${type}[]", "",
	       "  read_if_needed_();\n"
	       "  return (UmlActivityPin[]) _pins.clone();\n", FALSE);

  op = base_parameterset->add_op("set_Pins", PublicVisibility, "bool");
  op->set_Description(" set the pins");
  op->add_param(0, InputDirection, "v", user_activitypin);
  op->set_cpp("${type}", "const QVector<${t0}> & ${p0}",
	      "  UmlCom::send_cmd(_identifier, replaceParameterCmd, (const QVector<UmlItem> &) v);\n"
	      "  if (UmlCom::read_bool()) {\n"
	      "    if (_defined) _pins = v;\n"
	      "    return TRUE;\n"
	      "  }\n"
	      "  else\n"
	      "    return FALSE;\n",
	      FALSE, 0, 0);
  op->set_java("void", "${t0}[] ${p0}",
	      "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.replaceParameterCmd, v);\n"
	      "  UmlCom.check();\n"
	      "  _pins = (UmlActivityPin[]) v.clone();\n",
	      FALSE);
  
  op = base_parameterset->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _pins.resize(0);\n"
	      "  UmlBaseItem::unload(rec, del);\n",
	      FALSE, 0, 0);
  s = op->cppDecl();
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _pins = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
  
  UmlRelation * rel =
    base_parameterset->add_relation(aDirectionalAssociation,
				    "_pins", PrivateVisibility,
				    user_activitypin, 0, 0);  
  int index;
  
  rel->set_Stereotype("vector");
  rel->set_Multiplicity("*");
  rel->set_CppDecl(CppSettings::relationDecl(TRUE, "*"));

  s = rel->javaDecl();
  index = s.find("${type} ${name}");
  if (index != -1) {
    s.insert(index + 7,"[]");
    rel->set_JavaDecl(s);
  }

  op = base_parameterset->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseItem::read_uml_();\n"
	      "  UmlCom::read_item_list((QVector<UmlItem> &) _pins);\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _pins = (UmlActivityPin[]) UmlCom.read_item_list();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  // update an UmlCom::send_cmd

  const QVector<UmlItem> ch = UmlClass::get("UmlCom", 0)->children();
  UmlClass * cl = UmlClass::get("UmlClass", 0);
  
  UmlCom::trace("update UmlCom::send_cmd(...)<br>\n");
  
  for (index = 0; index != (int) ch.size(); index += 1) {
    if ((ch[index]->kind() == anOperation) &&
	(ch[index]->name() == "send_cmd")) {
      op = (UmlOperation *) ch[index];
      
      QValueList<UmlParameter> params = op->params();
      
      if (params.count() == 3) {
	UmlParameter p = params.last();
	
	if ((p.name == "l") && (p.type.type == cl)) {
	  p.type.type = user_item;
	  op->replaceParameter(2, p);
	  
	  s = op->cppBody();
	  if ((index = s.find("UmlClass")) != -1) {
	    s.replace(index+3, 5, "Item");
	    op->set_CppBody(s);
	  }
	  
	  s = op->javaBody();
	  if ((index = s.find("UmlClass")) != -1) {
	    s.replace(index+3, 5, "Item");
	    op->set_JavaBody(s);
	  }
	  break;
	}
      }
    }
  }
  
  // update UmlBaseArtifact::set_AssociatedClasses
  
  op = UmlClass::get("UmlBaseArtifact", 0)->get_operation("set_AssociatedClasses");
  if (op != 0) {
    UmlCom::trace("update UmlBaseArtifact::set_AssociatedClasses(...)<br>\n");
    s = op->cppBody();
    if ((index = s.find("l);")) != -1) {
      s.insert(index, "(const QVector<UmlItem> &) ");
      op->set_CppBody(s);
    }
  }
  
  UmlCom::set_user_id(uid);
}

void add_activities_item_kind()
{
  // warning : anActivityDiagram already exist
  // already root
  static const char * const kinds[] = {
    "anActivity", 
    "aFlow", 
    "anActivityParameter", 
    "aParameterSet", 
    "aPartition", 
    "anExpansionRegion", 
    "anInterruptibleActivityRegion", 
    "anOpaqueAction",
    "anAcceptEventAction",
    "aReadVariableValueAction",
    "aClearVariableValueAction",
    "aWriteVariableValueAction",
    "anAddVariableValueAction",
    "aRemoveVariableValueAction",
    "aCallBehaviorAction",
    "aCallOperationAction",
    "aSendObjectAction",
    "aSendSignalAction",
    "aBroadcastSignalAction",
    "anUnmarshallAction",
    "aValueSpecificationAction",
    "anActivityObject", 
    "anExpansionNode", 
    "anActivityPin", 
    "anInitialActivityNode", 
    "aFlowFinalActivityNode", 
    "anActivityFinalActivityNode", 
    "aDecisionActivityNode", 
    "aMergeActivityNode", 
    "aForkActivityNode", 
    "aJoinActivityNode", 
  };
  UmlClass * itkind = UmlClass::get("anItemKind", 0);
  
  for (int i = 0; i != sizeof(kinds)/sizeof(kinds[0]); i += 1)
    itkind->add_enum_item(kinds[i]);
}

void add_activity_on_instance_cmd()
{
  // already root
  UmlClass * itcmd = UmlClass::get("OnInstanceCmd", 0);
  
  itcmd->add_enum_item("setReadOnlyCmd");
  itcmd->add_enum_item("setSingleExecutionCmd");
  itcmd->add_enum_item("setUnmarshallCmd");
  itcmd->add_enum_item("setTimeEventCmd");
  itcmd->add_enum_item("setFlagCmd");
  itcmd->add_enum_item("setUniqueCmd");
  itcmd->add_enum_item("setStreamCmd");
  itcmd->add_enum_item("setInStateCmd");
  itcmd->add_enum_item("setOrderingCmd");
}    

void baseitem_read_activities(UmlClass * base_item)
{
  // update read_()'s body
  UmlOperation * op = base_item->get_operation("read_");
  
  if (op != 0) {
    QCString body;
    
    body = op->cppBody();
    body.remove(body.findRev("//return new UmlActivityDiagram(id, name);"), 2);
    body.insert(body.findRev("default:"),
		"case anActivity:\n"
		"      return new UmlActivity(id, name);\n"
		"    case aFlow:\n"
		"      return new UmlFlow(id, name);\n"
		"    case anActivityParameter:\n"
		"      return new UmlActivityParameter(id, name);\n"
		"    case aParameterSet:\n"
		"      return new UmlParameterSet(id, name);\n"
		"    case anExpansionRegion:\n"
		"      return new UmlExpansionRegion(id, name);\n"
		"    case anInterruptibleActivityRegion:\n"
		"      return new UmlInterruptibleActivityRegion(id, name);\n"
		"    case anOpaqueAction:\n"
		"      return new UmlOpaqueAction(id, name);\n"
		"    case anAcceptEventAction:\n"
		"      return new UmlAcceptEventAction(id, name);\n"
		"    case aReadVariableValueAction:\n"
		"      return new UmlReadVariableValueAction(id, name);\n"
		"    case aClearVariableValueAction:\n"
		"      return new UmlClearVariableValueAction(id, name);\n"
		"    case aWriteVariableValueAction:\n"
		"      return new UmlWriteVariableValueAction(id, name);\n"
		"    case anAddVariableValueAction:\n"
		"      return new UmlAddVariableValueAction(id, name);\n"
		"    case aRemoveVariableValueAction:\n"
		"      return new UmlRemoveVariableValueAction(id, name);\n"
		"    case aCallBehaviorAction:\n"
		"      return new UmlCallBehaviorAction(id, name);\n"
		"    case aCallOperationAction:\n"
		"      return new UmlCallOperationAction(id, name);\n"
		"    case aSendObjectAction:\n"
		"      return new UmlSendObjectAction(id, name);\n"
		"    case aSendSignalAction:\n"
		"      return new UmlSendSignalAction(id, name);\n"
		"    case aBroadcastSignalAction:\n"
		"      return new UmlBroadcastSignalAction(id, name);\n"
		"    case anUnmarshallAction:\n"
		"      return new UmlUnmarshallAction(id, name);\n"
		"    case aValueSpecificationAction:\n"
		"      return new UmlValueSpecificationAction(id, name);\n"
		"    case anActivityObject:\n"
		"      return new UmlActivityObject(id, name);\n"
		"    case anExpansionNode:\n"
		"      return new UmlExpansionNode(id, name);\n"
		"    case anActivityPin:\n"
		"      return new UmlActivityPin(id, name);\n"
		"    case anInitialActivityNode:\n"
		"      return new UmlInitialActivityNode(id, name);\n"
		"    case aFlowFinalActivityNode:\n"
		"      return new UmlFlowFinalActivityNode(id, name);\n"
		"    case anActivityFinalActivityNode:\n"
		"      return new UmlActivityFinalActivityNode(id, name);\n"
		"    case aDecisionActivityNode:\n"
		"      return new UmlDecisionActivityNode(id, name);\n"
		"    case aMergeActivityNode:\n"
		"      return new UmlMergeActivityNode(id, name);\n"
		"    case aForkActivityNode:\n"
		"      return new UmlForkActivityNode(id, name);\n"
		"    case aJoinActivityNode:\n"
		"      return new UmlJoinActivityNode(id, name);\n"
		"    case aPartition:\n"
		"      //return new UmlPartition(id, name);\n"
		"    ");
    op->set_CppBody(body);
      
    body = op->javaBody();
    body.remove(body.findRev("//return new UmlActivityDiagram(id, name);"), 2);
    body.insert(body.findRev("default:"),
		"case anItemKind._anActivity:\n"
		"      return new UmlActivity(id, name);\n"
		"    case anItemKind._aFlow:\n"
		"      return new UmlFlow(id, name);\n"
		"    case anItemKind._anActivityParameter:\n"
		"      return new UmlActivityParameter(id, name);\n"
		"    case anItemKind._aParameterSet:\n"
		"      return new UmlParameterSet(id, name);\n"
		"    case anItemKind._anExpansionRegion:\n"
		"      return new UmlExpansionRegion(id, name);\n"
		"    case anItemKind._anInterruptibleActivityRegion:\n"
		"      return new UmlInterruptibleActivityRegion(id, name);\n"
		"    case anItemKind._anOpaqueAction:\n"
		"      return new UmlOpaqueAction(id, name);\n"
		"    case anItemKind._anAcceptEventAction:\n"
		"      return new UmlAcceptEventAction(id, name);\n"
		"    case anItemKind._aReadVariableValueAction:\n"
		"      return new UmlReadVariableValueAction(id, name);\n"
		"    case anItemKind._aClearVariableValueAction:\n"
		"      return new UmlClearVariableValueAction(id, name);\n"
		"    case anItemKind._aWriteVariableValueAction:\n"
		"      return new UmlWriteVariableValueAction(id, name);\n"
		"    case anItemKind._anAddVariableValueAction:\n"
		"      return new UmlAddVariableValueAction(id, name);\n"
		"    case anItemKind._aRemoveVariableValueAction:\n"
		"      return new UmlRemoveVariableValueAction(id, name);\n"
		"    case anItemKind._aCallBehaviorAction:\n"
		"      return new UmlCallBehaviorAction(id, name);\n"
		"    case anItemKind._aCallOperationAction:\n"
		"      return new UmlCallOperationAction(id, name);\n"
		"    case anItemKind._aSendObjectAction:\n"
		"      return new UmlSendObjectAction(id, name);\n"
		"    case anItemKind._aSendSignalAction:\n"
		"      return new UmlSendSignalAction(id, name);\n"
		"    case anItemKind._aBroadcastSignalAction:\n"
		"      return new UmlBroadcastSignalAction(id, name);\n"
		"    case anItemKind._anUnmarshallAction:\n"
		"      return new UmlUnmarshallAction(id, name);\n"
		"    case anItemKind._aValueSpecificationAction:\n"
		"      return new UmlValueSpecificationAction(id, name);\n"
		"    case anItemKind._anActivityObject:\n"
		"      return new UmlActivityObject(id, name);\n"
		"    case anItemKind._anExpansionNode:\n"
		"      return new UmlExpansionNode(id, name);\n"
		"    case anItemKind._anActivityPin:\n"
		"      return new UmlActivityPin(id, name);\n"
		"    case anItemKind._anInitialActivityNode:\n"
		"      return new UmlInitialActivityNode(id, name);\n"
		"    case anItemKind._aFlowFinalActivityNode:\n"
		"      return new UmlFlowFinalActivityNode(id, name);\n"
		"    case anItemKind._anActivityFinalActivityNode:\n"
		"      return new UmlActivityFinalActivityNode(id, name);\n"
		"    case anItemKind._aDecisionActivityNode:\n"
		"      return new UmlDecisionActivityNode(id, name);\n"
		"    case anItemKind._aMergeActivityNode:\n"
		"      return new UmlMergeActivityNode(id, name);\n"
		"    case anItemKind._aForkActivityNode:\n"
		"      return new UmlForkActivityNode(id, name);\n"
		"    case anItemKind._aJoinActivityNode:\n"
		"      return new UmlJoinActivityNode(id, name);\n"
		"    case anItemKind._aPartition:\n"
		"      //return new UmlPartition(id, name);\n"
		"    ");
    op->set_JavaBody(body);
  }
  
  // update UmlBaseItem artifact
  UmlArtifact * art = base_item->associatedArtifact();
  QCString s;
  
  s = art->cppSource();
  s.insert(s.find("#include \"MiscGlobalCmd.h\""),
	   "#include \"UmlActivityDiagram.h\"\n"
	   "#include \"UmlActivity.h\"\n"
	   "#include \"UmlFlow.h\"\n"
	   "#include \"UmlActivityParameter.h\"\n"
	   "#include \"UmlParameterSet.h\"\n"
	   "#include \"UmlExpansionRegion.h\"\n"
	   "#include \"UmlInterruptibleActivityRegion.h\"\n"
	   "#include \"UmlActivityActionClasses.h\"\n"
	   "#include \"UmlActivityObject.h\"\n"
	   "#include \"UmlExpansionNode.h\"\n"
	   "#include \"UmlActivityPin.h\"\n"
	   "#include \"UmlActivityControlNodeClasses.h\"\n"
	   );
  art->set_CppSource(s);
}

void add_activity(UmlClass * base_item, UmlClass * user_item)
{
  UmlClassView * base_class_view = (UmlClassView *) base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) user_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    user_item->associatedArtifact()->parent();

  UmlCom::trace("<b>Activity</b><br>\n");
  
  UmlClass * user_activityitem = 
    UmlClass::made(user_class_view, user_depl_view, "UmlActivityItem", TRUE);

  add_activity(base_class_view, user_class_view, base_depl_view, user_depl_view,
	       base_item, user_item, user_activityitem);

  UmlClass * user_activitynode = 
    add_activitynode(base_class_view, user_class_view, base_depl_view, user_depl_view,
		     base_item, user_item, user_activityitem);

  add_flow(base_class_view, user_class_view, base_depl_view, user_depl_view,
	   base_item, user_item, user_activityitem, user_activitynode);
  
  UmlClass * user_diagram = UmlClass::get("UmlActivityDiagram", 0);
  
  add_activityregion(base_class_view, user_class_view, base_depl_view, user_depl_view,
		     base_item, user_item, user_activityitem, user_diagram);

  add_activityaction(base_class_view, user_class_view, base_depl_view, user_depl_view,
		     base_item, user_item, user_activitynode);
  
  add_activitycontrolnode(base_class_view, user_class_view, base_depl_view, user_depl_view,
			  base_item, user_item, user_activitynode);
  
  UmlClass * user_activityobject =
    add_activityobject(base_class_view, user_class_view, base_depl_view, user_depl_view,
		     base_item, user_item, user_activitynode, user_diagram);
  
  add_expansionnode(base_class_view, user_class_view, base_depl_view, user_depl_view,
		    base_item, user_activityobject);
  
  add_pinparam(base_class_view, user_class_view, base_depl_view, user_depl_view,
	       base_item, user_item, user_activityobject);
  
  //

  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update anItemKind<br>\n");
  
  add_activities_item_kind();
  
  UmlCom::trace("update cmd list<br>\n");
  
  add_activity_on_instance_cmd();
  
  UmlCom::trace("update item read<br>\n");
  
  baseitem_read_activities(base_item);
  
  UmlCom::trace("update aDirection<br>\n");
  
  add_return_direction();
  
  UmlCom::set_user_id(uid);
}

//
//
//

void fixe_activity(UmlClass * base_pinparam)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>fixes on activity</b><br>\n");
  
  UmlCom::trace("update UmlBasePinParameter<br>\n");
  
  UmlOperation * op;
  
  defGetBool(base_pinparam, _stream, isStream, 0, 0,
	     " return the isStream attribute");
  op->moveAfter(base_pinparam->get_operation("set_IsException"));
  
  UmlOperation * op1 = op;
  
  
  defSetBoolBitField(base_pinparam, _stream, set_IsStream, 
		     setStreamCmd, 0, 0, " set the isStream attribute");
  op->moveAfter(op1);
 
  // dummy must have 5 bits rather than 4
  UmlAttribute * dummy = base_pinparam->get_attribute("_dummy");
  QCString cppdecl = dummy->cppDecl();
  int index = cppdecl.find(": 4");
  
  if (index != -1) {
    cppdecl[index + 2] = '5';
    dummy->set_CppDecl(cppdecl);
  }
  
  //
  
  UmlCom::trace("fixe UmlBaseAddVariableValueAction::read_uml_()<br>\n");
  
  op = UmlClass::get("UmlBaseAddVariableValueAction", 0)->get_operation("read_uml_");
  
  op->set_CppBody("  UmlBaseAccessVariableValueAction::read_uml_();\n"
		  "  _replace_all = UmlCom::read_bool();\n");
  
  op->set_JavaBody("  super.read_uml_();\n"
		   "  _replace_all = UmlCom.read_bool();\n");
  
  //
  
  UmlCom::trace("fixe UmlBaseRemoveVariableValueAction::read_uml_()<br>\n");
  
  op = UmlClass::get("UmlBaseRemoveVariableValueAction", 0)->get_operation("read_uml_");
  
  op->set_CppBody("  UmlBaseAccessVariableValueAction::read_uml_();\n"
		  "  _remove_duplicates = UmlCom::read_bool();\n");
  
  op->set_JavaBody("  super.read_uml_();\n"
		   "  _remove_duplicates = UmlCom.read_bool();\n");
  
  //
  
  UmlCom::set_user_id(uid);
  
  //
  
  UmlRelation * rel = 
    UmlBaseRelation::create(aGeneralisation, 
			    UmlClass::get("UmlParameterSet", 0),
			    UmlClass::get("UmlActivityItem", 0));
  
  if (rel == 0) {
    QCString msg = "UmlParameterSet can't inherit UmlActivityItem<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
}

//
//
//

void add_partition(UmlClass * base_item, UmlClass * user_item)
{
  UmlClassView * base_class_view = (UmlClassView *) base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) user_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    user_item->associatedArtifact()->parent();
  
  UmlCom::trace("<b>add activity partition</b><br>\n");

  UmlClass * base_activitypartition;
  UmlClass * user_activitypartition;
  UmlClass * user_activityitem = UmlClass::get("UmlActivityItem", 0);
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "ActivityPartition",
		  base_activitypartition, user_activitypartition,
		  user_activityitem);
  base_activitypartition->add_default_base_op(user_item, user_activitypartition,
					      user_item, "activity partition", "aPartition");
  user_activitypartition->add_constr(base_activitypartition, PublicVisibility); 

  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  base_activitypartition->moveAfter(UmlClass::get("UmlBaseParameterSet", 0));
  include_umlcom(base_activitypartition);
  
  UmlOperation * op;
  UmlClass * user_activitydiagram = UmlClass::get("UmlActivityDiagram", 0);
  
  add_assoc_diag_ops(base_activitypartition, user_activitydiagram);
  defGetBool(base_activitypartition, _dimension, isDimension, 0, 0,
	     " return the isDimension attribute");
  defSetBool(base_activitypartition, _dimension, set_isDimension, setMultiplicityCmd, 0, 0,
	     " set the isDimension attribute");
  
  defGetBool(base_activitypartition, _external, isExternal, 0, 0,
	     " return the isExternal attribute");
  defSetBool(base_activitypartition, _external, set_isExternal, setIsCppExternalCmd, 0, 0,
	     " set the isExternal attribute");

  defGetPtr(base_activitypartition, _represents, represents, user_item, 0, 0,
	    "represents");
  defSetPtr(base_activitypartition, _represents, set_Represents, user_item, setDerivedCmd, 0, 0,
	    "represents");

  base_activitypartition->add_relation(aDirectionalAssociation,
				       "_assoc_diagram", PrivateVisibility,
				       user_activitydiagram, 0, 0); 
  base_activitypartition->add_attribute("_dimension", PrivateVisibility, "bool", 0, 0);
  base_activitypartition->add_attribute("_external", PrivateVisibility, "bool", 0, 0);
  base_activitypartition->add_relation(aDirectionalAssociation,
				       "_represents", PrivateVisibility,
				       user_item, 0, 0); 
  
  user_activitypartition->moveAfter(UmlClass::get("UmlParameterSet", 0));
  op = base_activitypartition->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _assoc_diagram = (UmlActivityDiagram *) UmlBaseItem::read_();\n"
	      "  UmlBaseItem::read_uml_();\n"
	      "  _dimension = UmlCom::read_bool();\n"
	      "  _external = UmlCom::read_bool();\n"
	      "  _represents = (UmlItem *) UmlBaseItem::read_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  _assoc_diagram = (UmlActivityDiagram) UmlBaseItem.read_();\n"
	       "  super.read_uml_();\n"
	       "  _dimension = UmlCom.read_bool();\n"
	       "  _external = UmlCom.read_bool();\n"
	       "  _represents = (UmlItem) UmlBaseItem.read_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  // update read_()'s body  
  QCString s;
  
  op = base_item->get_operation("read_");

  s = op->cppBody();
  s.replace(s.findRev("//return new UmlPartition"),
	    25, "return new UmlActivityPartition");
  op->set_CppBody(s);

  s = op->javaBody();
  s.replace(s.findRev("//return new UmlPartition"),
	    25, "return new UmlActivityPartition");
  op->set_JavaBody(s);
  
  // update UmlBaseItem artifact
  UmlArtifact * art = base_item->associatedArtifact();
  
  s = art->cppSource();
  s.insert(s.find("#include \"MiscGlobalCmd.h\""),
	   "#include \"UmlActivityPartition.h\"\n"
	   );
  art->set_CppSource(s);
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_additionalactions(UmlClass * base_item, UmlClass * user_item)
{
  UmlClassView * base_class_view = (UmlClassView *) base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) user_item->parent();
  UmlClass * user_activityaction = UmlClass::get("UmlActivityAction", 0);
  UmlArtifact * base_art = 
    UmlClass::get("UmlBaseAcceptEventAction", 0)->associatedArtifact();
  UmlArtifact * user_art = 
    UmlClass::get("UmlAcceptEventAction", 0)->associatedArtifact();
  UmlOperation * op;
  QCString s;
  unsigned uid = UmlCom::user_id();

  UmlCom::trace("<b>Add some activity actions</b><br>\n");
  
  // accept call action
  // doesn't inherit accept event action because of circular #include
  // problem and because I disagre with this inheritance because 
  // isUnmarshall and isTimerEvent are forced to false
  
  UmlClass * base_acceptcall;
  UmlClass * user_acceptcall;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "AcceptCallAction",
			    base_acceptcall, user_acceptcall,
			    0);
  
  base_acceptcall->add_default_base_op(user_activityaction, user_acceptcall,
					user_item,
					"accept call action", "anAcceptCallAction");
  user_acceptcall->add_constr(base_acceptcall, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGet(base_acceptcall, _trigger, trigger, "string", 0, 0,
	 "trigger");
  defSet(base_acceptcall, _trigger, set_Trigger, "str", setUmlTriggerCmd, 0, 0,
	 "trigger");
  defGet(base_acceptcall, _cpp_trigger, cppTrigger, "string", "WITHCPP", 0,
	 "trigger in C++");
  defSet(base_acceptcall, _cpp_trigger, set_CppTrigger, "str", setCppTriggerCmd, 0, "#endif",
	 "trigger in C++");
  defGet(base_acceptcall, _java_trigger, javaTrigger, "string", "WITHJAVA", 0,
	 "trigger in Java");
  defSet(base_acceptcall, _java_trigger, set_JavaTrigger, "str", setJavaTriggerCmd, 0, "#endif",
	 "trigger in Java");

  op = base_acceptcall->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _trigger = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp_trigger = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_trigger = 0;\n"
	      "#endif\n"
	      "  UmlBaseActivityAction::unload(rec, del);\n",
	      FALSE, 0, 0);
  s = op->cppDecl();
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _trigger = null;\n"
	       "  _cpp_trigger = null;\n"
	       "  _java_trigger = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_acceptcall->add_attribute("_trigger", PrivateVisibility, "string",
				  0, 0);
  base_acceptcall->add_attribute("_cpp_trigger", PrivateVisibility, "string",
				  "WITHCPP", "endif");
  base_acceptcall->add_attribute("_java_trigger", PrivateVisibility, "string",
				  "WITHJAVA", "endif");

  op = base_acceptcall->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _trigger = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _trigger = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_acceptcall->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_cpp_();\n"
	      "  _cpp_trigger = UmlCom::read_string();\n",
	      FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "",
	       "  super.read_cpp_();\n"
	       "  _cpp_trigger = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_acceptcall->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_java_();\n"
	      "  _java_trigger = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "",
	       "  super.read_java_();\n"
	       "  _java_trigger = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
    
  base_acceptcall->moveAfter(UmlClass::get("UmlBaseRemoveVariableValueAction", 0));
  user_acceptcall->moveAfter(UmlClass::get("UmlRemoveVariableValueAction", 0));
  
  UmlCom::set_user_id(uid);
  
  // reply action
  
  UmlClass * base_reply;
  UmlClass * user_reply;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "ReplyAction",
			    base_reply, user_reply,
			    0);
  
  base_reply->add_default_base_op(user_activityaction, user_reply,
				  user_item, "reply action", "aReplyAction");
  user_reply->add_constr(base_reply, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGet(base_reply, _trigger, replyToCall, "string", 0, 0,
	 "replyToCall trigger");
  defSet(base_reply, _trigger, set_ReplyToCall, "str", setUmlTriggerCmd, 0, 0,
	 "replyToCall trigger");
  defGet(base_reply, _cpp_trigger, cppReplyToCall, "string", "WITHCPP", 0,
	 "replyToCall trigger in C++");
  defSet(base_reply, _cpp_trigger, set_CppReplyToCall, "str", setCppTriggerCmd, 0, "#endif",
	 "replyToCall trigger in C++");
  defGet(base_reply, _java_trigger, javaReplyToCall, "string", "WITHJAVA", 0,
	 "replyToCall trigger in Java");
  defSet(base_reply, _java_trigger, set_JavaReplyToCall, "str", setJavaTriggerCmd, 0, "#endif",
	 "replyToCall trigger in Java");

  op = base_reply->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _trigger = 0;\n"
	      "#ifdef WITHCPP\n"
	      "  _cpp_trigger = 0;\n"
	      "#endif\n"
	      "#ifdef WITHJAVA\n"
	      "  _java_trigger = 0;\n"
	      "#endif\n"
	      "  UmlBaseActivityAction::unload(rec, del);\n",
	      FALSE, 0, 0);
  s = op->cppDecl();
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _trigger = null;\n"
	       "  _cpp_trigger = null;\n"
	       "  _java_trigger = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_reply->add_attribute("_trigger", PrivateVisibility, "string",
				  0, 0);
  base_reply->add_attribute("_cpp_trigger", PrivateVisibility, "string",
				  "WITHCPP", "endif");
  base_reply->add_attribute("_java_trigger", PrivateVisibility, "string",
				  "WITHJAVA", "endif");

  op = base_reply->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _trigger = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _trigger = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_reply->add_op("read_cpp_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_cpp_();\n"
	      "  _cpp_trigger = UmlCom::read_string();\n",
	      FALSE, "WITHCPP", "endif");
  op->set_java("${type}", "",
	       "  super.read_cpp_();\n"
	       "  _cpp_trigger = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  op = base_reply->add_op("read_java_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_java_();\n"
	      "  _java_trigger = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "endif");
  op->set_java("${type}", "",
	       "  super.read_java_();\n"
	       "  _java_trigger = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  base_reply->moveAfter(base_acceptcall);
  user_reply->moveAfter(user_acceptcall);
  
  UmlCom::set_user_id(uid);
  
  // create object action
  
  UmlClass * base_createobject;
  UmlClass * user_createobject;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "CreateObjectAction",
			    base_createobject, user_createobject,
			    0);
  
  base_createobject->add_default_base_op(user_activityaction, user_createobject,
					 user_item, "create object action", "aCreateObjectAction");
  user_createobject->add_constr(base_createobject, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGet(base_createobject, _classifier, classifier, "string", 0, 0,
	 "classifier");
  defSet(base_createobject, _classifier, set_Classifier, "str", setDefCmd, 0, 0,
	 "classifier");

  op = base_createobject->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  _classifier = 0;\n"
	      "  UmlBaseActivityAction::unload(rec, del);\n",
	      FALSE, 0, 0);
  s = op->cppDecl();
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _classifier = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. Recursively done for the sub items if 'rec' is TRUE.\n\
\n\
 if 'del' is true the sub items are deleted in C++, and removed from the\n\
 internal dictionnary in C++ and Java (to allow it to be garbaged),\n\
 you will have to call Children() to re-access to them");
    
  base_createobject->add_attribute("_classifier", PrivateVisibility, "string",
				  0, 0);

  op = base_createobject->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _classifier = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _classifier = UmlCom.read_string();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  base_createobject->moveAfter(base_reply);
  user_createobject->moveAfter(user_reply);
  
  UmlCom::set_user_id(uid);
  
  // destroy object Action
  
  UmlClass * base_destroyobject;
  UmlClass * user_destroyobject;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "DestroyObjectAction",
			    base_destroyobject, user_destroyobject,
			    0);
  
  base_destroyobject->add_default_base_op(user_activityaction, user_destroyobject,
					user_item,
					"destroy object action", "aDestroyObjectAction");
  user_destroyobject->add_constr(base_destroyobject, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGetBool(base_destroyobject, _links, isDestroyLinks, 0, 0,
	     " return the isDestroyLinks attribute.");
  defSetBool(base_destroyobject, _links, set_isDestroyLinks, setTypeCmd, 0, 0,
	     " set the isDestroyLinks attribute");
  
  defGetBool(base_destroyobject, _owned_objects, isDestroyOwnedObjects, 0, 0,
	     " return the isDestroyOwnedObjects attribute");
  defSetBool(base_destroyobject, _owned_objects, set_isDestroyOwnedObjects, setFlagCmd, 0, 0,
	     " set the isDestroyOwnedObjects attribute");
  
  base_destroyobject->add_attribute("_links", PrivateVisibility, "bool",
				  0, 0);
  base_destroyobject->add_attribute("_owned_objects", PrivateVisibility, "bool",
				  0, 0);
  
  op = base_destroyobject->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _links = UmlCom::read_bool();\n"
	      "  _owned_objects = UmlCom::read_bool();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _links = UmlCom.read_bool();\n"
	       "  _owned_objects = UmlCom.read_bool();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  base_destroyobject->moveAfter(base_createobject);
  user_destroyobject->moveAfter(user_createobject);
  
  UmlCom::set_user_id(uid);
  
  // test identity action
  
  UmlClass * base_testidentity;
  UmlClass * user_testidentity;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "TestIdentityAction",
			    base_testidentity, user_testidentity,
			    0);
  
  base_testidentity->add_default_base_op(user_activityaction, user_testidentity,
					user_item,
					"test identity action", "aTestIdentityAction");
  user_testidentity->add_constr(base_testidentity, PublicVisibility);
  
  UmlCom::set_user_id(0);
  
  base_testidentity->moveAfter(base_destroyobject);
  user_testidentity->moveAfter(user_destroyobject);
  
  UmlCom::set_user_id(uid);

  // raise exception action
  
  UmlClass * base_raiseexception;
  UmlClass * user_raiseexception;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "RaiseExceptionAction",
			    base_raiseexception, user_raiseexception,
			    0);
  
  base_raiseexception->add_default_base_op(user_activityaction, user_raiseexception,
					user_item,
					"raise exception action", "aRaiseExceptionAction");
  user_raiseexception->add_constr(base_raiseexception, PublicVisibility);
  
  UmlCom::set_user_id(0);
  
  base_raiseexception->moveAfter(base_testidentity);
  user_raiseexception->moveAfter(user_testidentity);
  
  UmlCom::set_user_id(uid);
  
  // reduce Action
  
  UmlClass * base_reduce;
  UmlClass * user_reduce;
  
  user_activityaction->made(base_class_view, user_class_view,
			    base_art, user_art,
			    base_item, "ReduceAction",
			    base_reduce, user_reduce,
			    0);
  
  base_reduce->add_default_base_op(user_activityaction, user_reduce,
				   user_item,
				   "reduce action", "aReduceAction");
  user_reduce->add_constr(base_reduce, PublicVisibility);

  UmlCom::set_user_id(0);
  
  defGetBool(base_reduce, _ordered, isOrdered, 0, 0,
	     " return the isOrdered attribute");
  defSetBool(base_reduce, _ordered, set_isOrdered, setFlagCmd, 0, 0,
	     " set the isOrdered attribute");
  
  defGetPtr(base_reduce, _reducer, reducer, user_item, 0, 0,
	    "reducer, may be an activity or a state machine");
  defSetPtr(base_reduce, _reducer, set_Reducer, user_item, setDefCmd, 0, 0,
	    "reducer");

  base_reduce->add_attribute("_ordered", PrivateVisibility, "bool",
				   0, 0);

  base_reduce->add_relation(aDirectionalAssociation,
				  "_reducer", PrivateVisibility,
				  user_item, 0, 0);  
  
  op = base_reduce->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  UmlBaseActivityAction::read_uml_();\n"
	      "  _ordered = UmlCom::read_bool();\n"
	      "  _reducer = UmlBaseItem::read_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  super.read_uml_();\n"
	       "  _ordered = UmlCom.read_bool();\n"
	       "  _reducer = UmlBaseItem.read_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  base_reduce->moveAfter(base_raiseexception);
  user_reduce->moveAfter(user_raiseexception);
  
  // stay in UmlCom::set_user_id(0);
  
  // update UmlItem::read_()'s body
  
  op = base_item->get_operation("read_");
  
  s = op->cppBody();
  s.insert(s.findRev("default:"),
	   "case anAcceptCallAction:\n"
	   "      return new UmlAcceptCallAction(id, name);\n"
	   "    case aReplyAction:\n"
	   "      return new UmlReplyAction(id, name);\n"
	   "    case aCreateObjectAction:\n"
	   "      return new UmlCreateObjectAction(id, name);\n"
	   "    case aDestroyObjectAction:\n"
	   "      return new UmlDestroyObjectAction(id, name);\n"
	   "    case aTestIdentityAction:\n"
	   "      return new UmlTestIdentityAction(id, name);\n"
	   "    case aRaiseExceptionAction:\n"
	   "      return new UmlRaiseExceptionAction(id, name);\n"
	   "    case aReduceAction:\n"
	   "      return new UmlReduceAction(id, name);\n"
	   "    ");
  op->set_CppBody(s);
  
  s = op->javaBody();
  s.insert(s.findRev("default:"),
	   "case anItemKind._anAcceptCallAction:\n"
	   "      return new UmlAcceptCallAction(id, name);\n"
	   "    case anItemKind._aReplyAction:\n"
	   "      return new UmlReplyAction(id, name);\n"
	   "    case anItemKind._aCreateObjectAction:\n"
	   "      return new UmlCreateObjectAction(id, name);\n"
	   "    case anItemKind._aDestroyObjectAction:\n"
	   "      return new UmlDestroyObjectAction(id, name);\n"
	   "    case anItemKind._aTestIdentityAction:\n"
	   "      return new UmlTestIdentityAction(id, name);\n"
	   "    case anItemKind._aRaiseExceptionAction:\n"
	   "      return new UmlRaiseExceptionAction(id, name);\n"
	   "    case anItemKind._aReduceAction:\n"
	   "      return new UmlReduceAction(id, name);\n"
	   "    ");
  op->set_JavaBody(s);
  
  // new uml item kinds
  
  static const char * const kinds[] = {
    "anAcceptCallAction", 
    "aReplyAction", 
    "aCreateObjectAction", 
    "aDestroyObjectAction", 
    "aTestIdentityAction", 
    "aRaiseExceptionAction", 
    "aReduceAction"
  };
  UmlClass * itkind = UmlClass::get("anItemKind", 0);
  UmlAttribute * a = itkind->get_attribute("aValueSpecificationAction");
  int i;

  for (i = 0; i != sizeof(kinds)/sizeof(kinds[0]); i += 1) {
    UmlAttribute * a2 = itkind->add_enum_item(kinds[i]);
    
    a2->moveAfter(a);
    a = a2;
  }
  
  // replace friends rels
  
  const QVector<UmlItem> ch = base_item->children();
  UmlExtraClassMember * ex = 0;
  
  i = ch.size();

  while (i--) {
    if ((ch[i]->kind() == anExtraClassMember) && (ch[i]->name() == "friend")) {
      ex = (UmlExtraClassMember *) ch[i];
      break;
    }
  }
  
  if (ex != 0) {
    // theo mandatory
    s = "";
    i = ch.size();
    
    while (i--) {
      if ((ch[i]->kind() == aRelation) &&
	  (((UmlRelation *) ch[i])->relationKind() == aDependency) &&
	  (ch[i]->stereotype() == "friend")) {
	s += "  friend class " + ((UmlRelation *) ch[i])->roleType()->name()
	  + ";\n";
	ch[i]->deleteIt();
      }
    }
    
    ex->set_CppDecl(ex->cppDecl() + s);
  }
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_activity_specification()
{
  unsigned uid = UmlCom::user_id();
  UmlClass * base_activity = UmlClass::get("UmlBaseActivity", 0);
  UmlClass * uml_oper = UmlClass::get("UmlOperation", 0);
  UmlCom::set_user_id(0);
  
  //
    
  UmlOperation * op;
  
  defGetPtr(base_activity, _specification, specification, uml_oper, 0, 0,
	    "specification");

  op->moveAfter(base_activity->get_operation("set_isSingleExecution"));
  
  UmlOperation * get = op;
  
  defSetPtr(base_activity, _specification, set_Specification, uml_oper,
	    setDefCmd, 0, 0, "specification");
  op->moveAfter(get);
  
  base_activity->add_relation(aDirectionalAssociation,
			      "_specification", PrivateVisibility,
			      uml_oper, 0, 0);  
  
  QCString s;
  
  op = base_activity->get_operation("read_uml_");
  
  s = op->cppBody();
  s += "  _specification = (UmlOperation *) UmlBaseItem::read_();\n";
  op->set_CppBody(s);
  
  s = op->javaBody();
  s += "  _specification = (UmlOperation) UmlBaseItem.read_();\n";
  op->set_JavaBody(s);
  
  //
  
  UmlCom::set_user_id(uid);
}
