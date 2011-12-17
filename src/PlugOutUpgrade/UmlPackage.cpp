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

#include <qmessagebox.h>

#include "UmlPackage.h"
#include "UmlClassView.h"
#include "UmlDeploymentView.h"
#include "UmlClass.h"
#include "UmlArtifact.h"
#include "UmlRelation.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"
#include "UmlCom.h"
#include "UmlSettings.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "UmlExtraClassMember.h"
#include "util.h"
#include "state.h"
#include "activity.h"
#include "insertbase.h"
#include "instance.h"
#include "diagdef.h"
#include "python.h"
#include "php.h"

//
// define set_Name on UmlBaseClass to rename 
// constructors & destructor

bool class_set_name_defined(UmlClass * base_class)
{
  return (base_class->get_operation("set_Name") != 0);
}

void upgrade_rename_class(UmlClass * base_item, 
			  UmlClass * base_class)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlOperation * set_item_name
    = base_item->get_operation("set_Name");
      
  set_item_name->set_isCppVirtual(TRUE);
  
  UmlOperation * set_class_name = 
    base_class->add_op("set_Name", PublicVisibility, "bool", TRUE);
  
  set_class_name->set_isCppVirtual(TRUE);
  set_class_name->add_param(0, InputDirection, "s", "string");
  set_class_name->set_Description(set_item_name->description());
  
  set_class_name->set_CppDecl(set_item_name->cppDecl());
  set_class_name->set_CppDef(set_item_name->cppDef());
  set_class_name->set_CppBody("\
  if (!UmlBaseItem::set_Name(s))\n\
    return FALSE;\n\
\n\
  const QVector<UmlItem> ch = children();\n\
  QCString destr = \"~\" + name();\n\
\n\
  for (unsigned i = 0; i != ch.size(); i += 1) {\n\
    if (ch[i]->kind() == anOperation) {\n\
      if (ch[i]->name() == name())\n\
	ch[i]->set_Name(s);\n\
      else if (ch[i]->name() == destr)\n\
	ch[i]->set_Name(\"~\" + s);\n\
    }\n\
  }\n\
\n\
  return TRUE;\n");

  set_class_name->set_JavaDecl(set_item_name->javaDecl());
  set_class_name->set_JavaBody("\
  super.set_Name(s);\n\
\n\
  UmlItem[] ch = children();\n\
  String destr = \"~\" + name();\n\
\n\
  for (int i = 0; i != ch.length; i += 1) {\n\
    if (ch[i].kind() == anItemKind.anOperation) {\n\
      if (ch[i].name() == name())\n\
	ch[i].set_Name(s);\n\
      else if (ch[i].name() == destr)\n\
	ch[i].set_Name(\"~\" + s);\n\
    }\n\
  }\n");
  
  UmlCom::set_user_id(uid);
}


//
// add new settings linked to the default description and default C++ return forms
// add UmlBaseItem::moveAfter
//

bool item_move_after_defined(UmlClass * base_item)
{
  return (base_item->get_operation("moveAfter") != 0);
}

void add_move_after(UmlClass * base_item, UmlClass * uml_item)
{
  UmlOperation * move_after = 
    base_item->add_op("moveAfter", PublicVisibility, "bool", TRUE);
  
  move_after->add_param(0, InputDirection, "x", uml_item);
  move_after->set_Description(" If the parameter is null, move the current item to be\n"
			      " the first child of its parent. Else the current item and\n"
			      " the parameter must have the same parent, the current item\n"
			      " is moved to be just after the parameter.\n"
			      " On error return FALSE in C++, produce a RuntimeException in Java");
  move_after->set_cpp("${type}", "const ${t0} * ${p0}",
		      "  UmlCom::send_cmd(_identifier, moveAfterCmd, (x != 0) ? ((UmlBaseItem *) x)->_identifier : 0);\n"
		      "  if (UmlCom::read_bool()) {\n"
		      "    ((UmlBaseItem *) parent())->reread_children_if_needed_();\n"
		      "    return TRUE;\n"
		      "  }\n"
		      "  else\n"
		      "    return FALSE;\n",
		      FALSE, 0, 0);
  move_after->set_java("void", "${t0} ${p0}",
		       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.moveAfterCmd,\n"
		       "                  (x != null) ? x.identifier_() : 0);\n"
		       "  UmlCom.check();\n"
		       "  parent().reread_children_if_needed_();\n",
		       FALSE);
  
  // move it (!) after properties
  
  move_after->moveAfter(base_item->get_operation("properties"));
}

void upgrade_OnInstanceCmd_UmlSettingsCmd_CppSettingsCmd()
{
  // upgrade OnInstanceCmd
  
  UmlClass * cl = UmlClass::get("OnInstanceCmd", 0);
  
  cl->add_enum_item("moveAfterCmd");

  // update UmlSettingsCmd enum
  
  cl = UmlClass::get("UmlSettingsCmd", 0);
  
  cl->add_enum_item("setDefaultArtifactDescriptionCmd")->set_DefaultValue("= 50");
  cl->add_enum_item("setDefaultClassDescriptionCmd");
  cl->add_enum_item("setDefaultOperationDescriptionCmd");
  cl->add_enum_item("setDefaultAttributeDescriptionCmd");
  cl->add_enum_item("setDefaultRelationDescriptionCmd");
  
  // update CppSettingsCmd enum
  
  cl = UmlClass::get("CppSettingsCmd", 0);
  
  cl->add_enum_item("setCppEnumReturnCmd");
  cl->add_enum_item("setCppReturnCmd");
 }

void upgrade_UmlSettings()
{
    
  UmlClass * cl = UmlClass::get("UmlSettings", 0);
  
  cl->set_isAbstract(FALSE);
  cl->set_Description(" This class manages settings not linked with a language, configured through\n"
		      " the 'Generation settings' dialog.\n"
		      "\n"
		      " This class may be defined as a 'singleton', but I prefer to use static\n"
		      " members allowing to just write 'UmlSettings::member' rather than\n"
		      " 'UmlSettings::instance()->member' or other long sentence like this.\n");
  
  static const struct {
    const char * att;
    const char * after;
    const char * get;
    const char * set;
    const char * cmd;
  } s[] = {
    { "_artifact_default_description",
      "_defined",
      "artifactDescription",
      "set_ArtifactDescription",
      "setDefaultArtifactDescriptionCmd" },
    { "_class_default_description",
      "_artifact_default_description", 
      "classDescription",
      "set_ClassDescription",
      "setDefaultClassDescriptionCmd" },
    { "_operation_default_description",
      "_class_default_description",
      "operationDescription",
      "set_OperationDescription",
      "setDefaultOperationDescriptionCmd" },
    { "_attribute_default_description",
      "_operation_default_description",
      "attributeDescription",
      "set_AttributeDescription",
      "setDefaultAttributeDescriptionCmd" },
    { "_relation_default_description",
      "_attribute_default_description",
      "relationDescription",
      "set_RelationDescription",
      "setDefaultRelationDescriptionCmd" }
  };
  unsigned i;

  for (i = 0; i != sizeof(s)/sizeof(s[0]); i += 1) {
    // add attribute
    const QVector<UmlItem> ch = cl->children();
    UmlAttribute * at =
      cl->add_attribute(s[i].att, ProtectedVisibility, "string", 0, 0);
    at->set_isClassMember(TRUE);
    
    for (unsigned j = 0; j != ch.size(); j += 1) {
      if (ch[j]->name() == s[i].after) {
	at->moveAfter(ch[j]);
	break;
      }
    }
    
  }
  
  for (i = sizeof(s)/sizeof(s[0]) - 1; i != (unsigned) -1; i -= 1) {
    // add set
    UmlOperation * op = cl->add_op(s[i].set, PublicVisibility, "bool");
   
    op->moveAfter(0);
    op->set_isClassMember(TRUE);
    op->set_Description(" set the default description\n\
\n\
 On error : return FALSE in C++, produce a RuntimeException in Java");
    op->add_param(0, InputDirection, "v", "string"); 
    op->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(umlSettingsCmd, " + QCString(s[i].cmd) + ", v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    " + QCString(s[i].att) + " = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
    op->set_java("void", "${t0} ${p0}",
		 "  UmlCom.send_cmd(CmdFamily.umlSettingsCmd, UmlSettingsCmd._"
		 + QCString(s[i].cmd) + ", v);\n"
		 "  UmlCom.check();\n"
		 "  " + QCString(s[i].att) + " = v;\n",
		 FALSE);
    
    // add get
    op = 
      cl->add_op(s[i].get, PublicVisibility, "string");
   
    op->moveAfter(0);
    op->set_isClassMember(TRUE);
    op->set_Description(" return the default description");
    op->set_cpp("${type}", "",
		"  read_if_needed_();\n"
		"\n"
		"  return " + QCString(s[i].att) + ";\n",
		FALSE, 0, 0);
    op->set_java("${type}", "",
		 "  read_if_needed_();\n"
		"\n"
		"  return " + QCString(s[i].att) + ";\n",
		 FALSE);
  }
  
  // upgrade read_
  
  UmlOperation * op = cl->get_operation("read_");

  if (op != 0) {
    QCString body;
    
    body = op->cppBody();
    body.append("\n\
  _artifact_default_description = UmlCom::read_string();\n\
  _class_default_description = UmlCom::read_string();\n\
  _operation_default_description = UmlCom::read_string();\n\
  _attribute_default_description = UmlCom::read_string();\n\
  _relation_default_description = UmlCom::read_string();\n");
    op->set_CppBody(body);
      
    body = op->javaBody();    
    body.append("\n\
  _artifact_default_description = UmlCom.read_string();\n\
  _class_default_description = UmlCom.read_string();\n\
  _operation_default_description = UmlCom.read_string();\n\
  _attribute_default_description = UmlCom.read_string();\n\
  _relation_default_description = UmlCom.read_string();\n");
    op->set_JavaBody(body);  
  }
}

void upgrade_CppSettings()
{    
  static const struct {
    const char * att;
    const char * after_att;
    const char * get;
    const char * set;
    const char * after_oper;
    const char * cmd;
  } s[] = {
    { "_return", "_inout",
      "Return", "set_Return", "set_Inout", "setCppReturnCmd" },
    { "_enum_return", "_enum_inout",
      "enumReturn", "set_EnumReturn", "set_EnumInout", "setCppEnumReturnCmd" }
  };
  UmlClass * cl = UmlClass::get("CppSettings", 0);
  unsigned i;
  unsigned j;
  
  for (i = 0; i != sizeof(s)/sizeof(s[0]); i += 1) {
    UmlAttribute * at =
      cl->add_attribute(s[i].att, PrivateVisibility, "string", 0, 0);
    at->set_isClassMember(TRUE);
    
    const QVector<UmlItem> ch = cl->children();
  
    for (j = 0; j != ch.size(); j += 1) {
      if (ch[j]->name() == s[i].after_att) {
	at->moveAfter(ch[j]);
	break;
      }
    }
    
    // add set
    UmlOperation * op = cl->add_op(s[i].set, PublicVisibility, "bool", TRUE);
   
    op->set_isClassMember(TRUE);
    op->set_Description(" set the default operation value type form\n\
\n\
 On error : return FALSE in C++, produce a RuntimeException in Java");
    op->add_param(0, InputDirection, "v", "string"); 
    op->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(cppSettingsCmd, " + QCString(s[i].cmd) + ", v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    " + QCString(s[i].att) + " = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
    op->set_java("void", "${t0} ${p0}",
		 "  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._"
		 + QCString(s[i].cmd) + ", v);\n"
		 "  UmlCom.check();\n"
		 "  " + QCString(s[i].att) + " = v;\n",
		 FALSE);
    
    // add get
    UmlOperation * op2 = cl->add_op(s[i].get, PublicVisibility, "string");
   
    op2->set_isClassMember(TRUE);
    op2->set_Description(" return the default operation value type form");
    op2->set_cpp("${type}", "",
		 "  read_if_needed_();\n"
		 "\n"
		 "  return " + QCString(s[i].att) + ";\n",
		 FALSE, 0, 0);
    op2->set_java("${type}", "",
		  "  read_if_needed_();\n"
		  "\n"
		  "  return " + QCString(s[i].att) + ";\n",
		  FALSE);
    
    for (j = 0; j != ch.size(); j += 1) {
      if (ch[j]->name() == s[i].after_oper) {
	op->moveAfter(ch[j]);
	op2->moveAfter(ch[j]);
	break;
      }
    }
  }
  
  // upgrade read_
  
  UmlOperation * op = cl->get_operation("read_");
  
  if (op != 0) {
    QCString body;
    int index;
    
    body = op->cppBody();
    index = body.find("}");
    body.insert(index, "  UmlSettings::_builtins[index].cpp_return = UmlCom::read_string();\n  ");
    index = body.find("_enum_in", index);
    body.insert(index, "_return = UmlCom::read_string();\n  ");
    index = body.find("_class_decl", index);
    body.insert(index, "_enum_return = UmlCom::read_string();\n  ");
    op->set_CppBody(body);
    
    body = op->javaBody(); 
    index = body.find("}");
    body.insert(index, "  UmlSettings._builtins[index].cpp_return = UmlCom.read_string();\n  ");
    index = body.find("_enum_in", index);
    body.insert(index, "_return = UmlCom.read_string();\n  ");
    index = body.find("_class_decl", index);
    body.insert(index, "_enum_return = UmlCom.read_string();\n  ");
    op->set_JavaBody(body);
  }
}

    
void upgrade_UmlBuiltin()
{
  UmlClass * cl = UmlClass::get("UmlBuiltin", 0);
  UmlAttribute * at =
    cl->add_attribute("cpp_return", PublicVisibility, "string", 0, 0);
  
  at->move_after(anAttribute, "cpp_inout");
}

void upgrade_new_settings__move_after_item(UmlClass * base_item, UmlClass * uml_item)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  add_move_after(base_item, uml_item);
  upgrade_OnInstanceCmd_UmlSettingsCmd_CppSettingsCmd();
  upgrade_UmlSettings();
  upgrade_CppSettings();
  upgrade_UmlBuiltin();
  
  UmlCom::set_user_id(uid);
}

//
// update UmlBaseClass::associatedComponent
//

bool one_component_per_class(UmlClass * uml_base_class)
{
  return (uml_base_class->get_operation("associatedComponent") != 0);
}

void several_components_per_class(UmlClass * uml_base_class)
{
  UmlOperation * op = 
    uml_base_class->get_operation("associatedComponent");
  
  op->set_Name("associatedComponents");
      
  op->set_Description(" returns the components realizing or providing the class.\n"
		      " To set them refer to the UmlBaseComponent's operation"
		      " setAssociatedClasses()");
      
  QCString s;
  int index;
  
  s = op->cppDecl();
  index = s.find("${type} *");
  if (index != -1)
    op->set_CppDecl(s.replace(index, 9, "const QVector<${type}>"));
  
  s = op->cppDef();
  index = s.find("${type} *");
  if (index != -1)
    op->set_CppDef(s.replace(index, 9, "const QVector<${type}>"));
  
  op->set_CppBody("  UmlCom::send_cmd(_identifier, assocComponentCmd);\n"
		  "\n"
		  "  QVector<UmlComponent> result;\n"
		  "  unsigned n = UmlCom::read_unsigned();\n"
		  "\n"
		  "  result.resize(n);\n"
		  "\n"
		  "  for (unsigned index = 0; index != n; index += 1)\n"
		  "    result.insert(index, (UmlComponent *) UmlBaseItem::read_());\n"
		  "\n"
		  "  return result;\n");
  
  s = op->javaDecl();
  index = s.find("${type}");
  if (index != -1)
    op->set_JavaDecl(s.replace(index, 7, "${type}[]"));
  
  op->set_JavaBody("  UmlCom.send_cmd(identifier_(), OnInstanceCmd.assocComponentCmd);\n"
		   "\n"
		   "  int n = UmlCom.read_unsigned();\n"
		   "  UmlComponent[] result = new UmlComponent[n];\n"
		   "\n"
		   "  for (int index = 0; index != n; index += 1)\n"
		   "    result[index] = (UmlComponent) UmlBaseItem.read_();\n"
		   "\n"
		   "  return result;\n");
  
  UmlCom::trace("update operation UmlBaseClass::associatedComponent<br>\n");
}

//
// Java JDK 5 upgrade
//

bool java_jdk1_4(UmlClass * javasettings)
{
  return (javasettings->get_operation("enumPatternDecl") == 0);
}

void upgrade_jdk5(UmlClass * javasettings)
{
  UmlCom::trace("<b>upgrade to manage jdk5</b><br>\n");
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
    
  //
  // modify old enum petterns, add new enums
  //
  
  UmlPackage::getProject()->rename_jdk5();
  
  QVector<UmlItem> ch = javasettings->children();
  UmlOperation * set_EnumPatternDecl = 0;
  UmlOperation * set_EnumPatternItemCase = 0;
  UmlAttribute * _enum_pattern_decl = 0;
  UmlAttribute * _enum_pattern_item_case = 0;
  unsigned i;
  
  // rename JavaSettings operations and attributes
  // updates read_'s body
  
  for (i = 0; i != ch.size(); i += 1) {
    switch (ch[i]->kind()) {
    case anOperation:
      {
	QCString s = ch[i]->name();
	
	if (s == "enumDecl") {
	  UmlCom::trace("rename JavaSettings::enumDecl to enumPatternDecl<br>\n");
	  ch[i]->set_Name("enumPatternDecl");
	}
	else if (s == "set_EnumDecl") {
	  UmlCom::trace("rename JavaSettings::set_EnumDecl to set_EnumPatternDecl<br>\n");
	  ch[i]->set_Name("set_EnumPatternDecl");
	  set_EnumPatternDecl = (UmlOperation *) ch[i];
	}
	else if (s == "enumItemDecl") {
	  UmlCom::trace("rename JavaSettings::enumItemDecl to enumPatternItemDecl<br>\n");
	  ch[i]->set_Name("enumPatternItemDecl");
	}
	else if (s == "set_EnumItemDecl") {
	  UmlCom::trace("rename JavaSettings::set_EnumItemDecl to set_EnumPatternItemDecl<br>\n");
	  ch[i]->set_Name("set_EnumPatternItemDecl");
	}
	else if (s == "enumItemCase") {
	  UmlCom::trace("rename JavaSettings::enumItemCase to enumPatternItemCase<br>\n");
	  ch[i]->set_Name("enumPatternItemCase");
	}
	else if (s == "set_EnumItemCase") {
	  UmlCom::trace("rename JavaSettings::set_EnumItemCase to set_EnumPatternItemCase<br>\n");
	  ch[i]->set_Name("set_EnumPatternItemCase");
	  set_EnumPatternItemCase = (UmlOperation *) ch[i];
	}
	else if (s == "read_") {
	  UmlCom::trace("update JavaSettings::read_<br>\n");
	  ((UmlOperation *) ch[i])->
	    set_CppBody("  _root = UmlCom::read_string();\n"
			"  \n"
			"  unsigned n;\n"
			"  unsigned index;\n"
			"  \n"
			"  n = UmlCom::read_unsigned();\n"
			"  \n"
			"  for (index = 0; index != n; index += 1) {\n"
			"    UmlSettings::_builtins[index].java = UmlCom::read_string();\n"
			"  }\n"
			"  \n"
			"  n = UmlCom::read_unsigned();\n"
			"  \n"
			"  for (index = 0; index != n; index += 1)\n"
			"    UmlSettings::_relation_stereotypes[index].java = UmlCom::read_string();\n"
			"  \n"
			"  n = UmlCom::read_unsigned();\n"
			"  \n"
			"  for (index = 0; index != n; index += 1)\n"
			"    UmlSettings::_class_stereotypes[index].java = UmlCom::read_string();\n"
			"  \n"
			"  n = UmlCom::read_unsigned();\n"
			"  _map_imports.clear();\n"
			"  if (n > _map_imports.size())\n"
			"    _map_imports.resize(n);\n"
			"  \n"
			"  for (index = 0; index != n; index += 1) {\n"
			"    QCString t = UmlCom::read_string();\n"
			"    QCString i = UmlCom::read_string();\n"
			"    \n"
			"    _map_imports.insert(t, new QCString(i));\n"
			"  }\n"
			"    \n"
			"  _src_content = UmlCom::read_string();\n"
			"  _ext = UmlCom::read_string();\n"
			"\n"
			"  _class_decl = UmlCom::read_string();\n"
			"  _external_class_decl = UmlCom::read_string();\n"
			"  _enum_decl = UmlCom::read_string();\n"
			"  _enum_pattern_decl = UmlCom::read_string();\n"
			"  _interface_decl = UmlCom::read_string();\n"
			"  _attr_decl = UmlCom::read_string();\n"
			"  _enum_item_decl = UmlCom::read_string();\n"
			"  _enum_pattern_item_decl = UmlCom::read_string();\n"
			"  _enum_pattern_item_case = UmlCom::read_string();\n"
			"  for (index = 0; index != 3; index += 1)\n"
			"    _rel_decl[index] = UmlCom::read_string();\n"
			"  _oper_def = UmlCom::read_string();\n"
			"  _get_visibility = (aVisibility) UmlCom::read_char();\n"
			"  _get_name = UmlCom::read_string();\n"
			"  _is_get_final = UmlCom::read_bool();\n"
			"  _set_visibility = (aVisibility) UmlCom::read_char();\n"
			"  _set_name = UmlCom::read_string();\n"
			"  _is_set_final = UmlCom::read_bool();\n"
			"  _is_set_param_final = UmlCom::read_bool();\n"
			);
	  
	  ((UmlOperation *) ch[i])->
	    set_JavaBody("  _root = UmlCom.read_string();\n"
			 "  \n"
			 "  int n;\n"
			 "  int index;\n"
			 "  \n"
			 "  n = UmlCom.read_unsigned();\n"
			 "  \n"
			 "  for (index = 0; index != n; index += 1) {\n"
			 "    UmlSettings._builtins[index].java = UmlCom.read_string();\n"
			 "  }\n"
			 "  \n"
			 "  n = UmlCom.read_unsigned();\n"
			 "  \n"
			 "  for (index = 0; index != n; index += 1)\n"
			 "    UmlSettings._relation_stereotypes[index].java = UmlCom.read_string();\n"
			 "  \n"
			 "  n = UmlCom.read_unsigned();\n"
			 "  \n"
			 "  for (index = 0; index != n; index += 1)\n"
			 "    UmlSettings._class_stereotypes[index].java = UmlCom.read_string();\n"
			 "  \n"
			 "  n = UmlCom.read_unsigned();\n"
			 "  _map_imports = new Hashtable((n == 0) ? 1 : n);\n"
			 "  \n"
			 "  for (index = 0; index != n; index += 1) {\n"
			 "    String t = UmlCom.read_string();\n"
			 "    String i = UmlCom.read_string();\n"
			 "    \n"
			 "    _map_imports.put(t, i);\n"
			 "  }\n"
			 "    \n"
			 "  _src_content = UmlCom.read_string();\n"
			 "  _ext = UmlCom.read_string();\n"
			 "\n"
			 "  _class_decl = UmlCom.read_string();\n"
			 "  _external_class_decl = UmlCom.read_string();\n"
			 "  _enum_decl = UmlCom.read_string();\n"
			 "  _enum_pattern_decl = UmlCom.read_string();\n"
			 "  _interface_decl = UmlCom.read_string();\n"
			 "  _attr_decl = UmlCom.read_string();\n"
			 "  _enum_item_decl = UmlCom.read_string();\n"
			 "  _enum_pattern_item_decl = UmlCom.read_string();\n"
			 "  _enum_pattern_item_case = UmlCom.read_string();\n"
			 "  _rel_decl = new String[3];\n"
			 "  for (index = 0; index != 3; index += 1)\n"
			 "    _rel_decl[index] = UmlCom.read_string();\n"
			 "  _oper_def = UmlCom.read_string();\n"
			 "  _get_visibility = aVisibility.fromInt(UmlCom.read_char());\n"
			 "  _get_name = UmlCom.read_string();\n"
			 "  _is_get_final = UmlCom.read_bool();\n"
			 "  _set_visibility = aVisibility.fromInt(UmlCom.read_char());\n"
			 "  _set_name = UmlCom.read_string();\n"
			 "  _is_set_final = UmlCom.read_bool();\n"
			 "  _is_set_param_final = UmlCom.read_bool();\n"
			 );
	}
      }
      break;
    case anAttribute:
      {
	QCString s = ch[i]->name();
	
	if (s == "_enum_decl") {
	  UmlCom::trace("rename JavaSettings::_enum_decl to _enum_pattern_decl<br>\n");
	  ch[i]->set_Name("_enum_pattern_decl");
	  _enum_pattern_decl = (UmlAttribute *) ch[i];
	}
	else if (s == "_enum_item_decl") {
	  UmlCom::trace("rename JavaSettings::_enum_item_decl to _enum_pattern_item_decl<br>\n");
	  ch[i]->set_Name("_enum_pattern_item_decl");
	}
	else if (s == "_enum_item_case") {
	  UmlCom::trace("rename JavaSettings::_enum_item_case to _enum_pattern_item_case<br>\n");
	  ch[i]->set_Name("_enum_pattern_item_case");
	  _enum_pattern_item_case = (UmlAttribute *) ch[i];
	}
      }
      break;
    default:
      break;
    }
  }
  
  // add new JavaSettings operations
  
  UmlOperation * op1;
  
  op1 = javasettings->add_op("enumDecl", PublicVisibility, "string");
  op1->set_Description(" returns the default definition of an enumeration");
  op1->set_cpp("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _enum_decl;\n",
	      FALSE, 0, 0);
  op1->set_java("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _enum_decl;\n",
	       FALSE);
  op1->set_isClassMember(TRUE);
  op1->moveAfter(set_EnumPatternDecl);

  UmlOperation * op2;
  
  op2 = javasettings->add_op("set_EnumDecl", PublicVisibility, "bool", TRUE);
  op2->add_param(0, InputDirection, "v", "string");
  op2->set_Description(" set the default definition of an enumeration\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->set_cpp("${type}", "${t0} ${p0}", "\
  UmlCom::send_cmd(javaSettingsCmd, setJavaEnumDeclCmd, v);\n\
  if (UmlCom::read_bool()) {\n\
    _enum_decl = v;\n\
    return TRUE;\n\
  }\n\
  else\n\
    return FALSE;\n",
	      FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}", "\
  UmlCom.send_cmd(CmdFamily.javaSettingsCmd, JavaSettingsCmd._setJavaEnumDeclCmd, v);\n\
  UmlCom.check();\n\
  \n\
  _enum_decl = v;\n",
	       FALSE);
  op2->set_isClassMember(TRUE);
  op2->moveAfter(op1);

  op1 = javasettings->add_op("enumItemDecl", PublicVisibility, "string", FALSE);
  op1->set_Description(" returns the default definition of an enumeration item");
  op1->set_cpp("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _enum_item_decl;\n",
	      FALSE, 0, 0);
  op1->set_java("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _enum_item_decl;\n",
	       FALSE);
  op1->set_isClassMember(TRUE);
  op1->moveAfter(set_EnumPatternItemCase);

  op2 = javasettings->add_op("set_EnumItemDecl", PublicVisibility, "bool", TRUE);
  op2->set_Description(" set the default definition of an enumeration item\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "string");
  op2->set_cpp("${type}", "${t0} ${p0}", "\
  UmlCom::send_cmd(javaSettingsCmd, setJavaEnumItemDeclCmd, v);\n\
  if (UmlCom::read_bool()) {\n\
    _enum_item_decl = v;\n\
    return TRUE;\n\
  }\n\
  else\n\
    return FALSE;\n",
	      FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}", "\
  UmlCom.send_cmd(CmdFamily.javaSettingsCmd, JavaSettingsCmd._setJavaEnumItemDeclCmd, v);\n\
  UmlCom.check();\n\
    \n\
  _enum_item_decl = v;\n",
	       FALSE);
  op2->set_isClassMember(TRUE);
  op2->moveAfter(op1);
  
  // add new JavaSettings attributes
  
  UmlAttribute * att;
  
  att = javasettings->add_attribute("_enum_decl", PrivateVisibility, "string", 0, 0);
  att->set_isClassMember(TRUE);
  att->moveAfter(_enum_pattern_decl);
  
  att = javasettings->add_attribute("_enum_item_decl", PrivateVisibility, "string", 0, 0);
  att->set_isClassMember(TRUE);
  att->moveAfter(_enum_pattern_item_case);
  
  // rename JavaSettingsCmd items
  
  UmlClass * javacmd = UmlClass::get("JavaSettingsCmd", 0);
  
  ch = javacmd->children();
  
  for (i = 0; i != ch.size(); i += 1) {
    if (ch[i]->kind() == anAttribute) {
      if (ch[i]->name() == "setJavaEnumDeclCmd") {
	UmlCom::trace("rename JavaSettingsCmd::setJavaEnumDeclCmd to setJavaEnumPatternDeclCmd<br>\n");
	ch[i]->set_Name("setJavaEnumPatternDeclCmd");
      }
      else if (ch[i]->name() == "setJavaEnumItemDeclCmd") {
	UmlCom::trace("rename JavaSettingsCmd::setJavaEnumItemDeclCmd to setJavaEnumPatternItemDeclCmd<br>\n");
	ch[i]->set_Name("setJavaEnumPatternItemDeclCmd");
      }
      else if (ch[i]->name() == "setJavaEnumItemCaseCmd") {
	UmlCom::trace("rename JavaSettingsCmd::setJavaEnumItemCaseCmd to setJavaEnumPatternItemCaseCmd<br>\n");
	ch[i]->set_Name("setJavaEnumPatternItemCaseCmd");
      }
    }
  }
  
  // new JavaSettingsCmd items
  
  javacmd->add_enum_item("setJavaEnumDeclCmd");
  javacmd->add_enum_item("setJavaEnumItemDeclCmd");

  //
  // adds generics management
  //
  
  // add UmlBaseFormalParameter::_extends
  
  UmlClass * baseformalparam =
    UmlClass::get("UmlBaseFormalParameter", 0);
  
  ch = baseformalparam->children();
  
  for (i = 0; i != ch.size(); i += 1) {
    if (ch[i]->kind() == aRelation) {
      if (((UmlRelation *) ch[i])->roleName() == "_default_value") {
	UmlRelation * default_value = (UmlRelation *) ch[i];
	
	UmlCom::trace("add relation UmlBaseFormalParameter::_extends<br>\n");
	UmlRelation * extend = 
	  UmlRelation::create(aDirectionalAggregationByValue,
			      baseformalparam,
			      default_value->roleType());
	
	extend->set_CppDecl(default_value->cppDecl());
	extend->set_JavaDecl(default_value->javaDecl());
	extend->set_Visibility(ProtectedVisibility);
	extend->set_RoleName("_extends");
        extend->moveAfter(default_value);
	break;
      }
    }
  }
  
  // add UmlBaseFormalParameter::extend()
  
  UmlClass * typespec = UmlClass::get("UmlTypeSpec", 0);
  
  op1 = baseformalparam->add_op("extend", PublicVisibility,
				typespec, FALSE);
  op1->set_CppDecl("  ${comment}${static}${virtual}const ${type} & ${name}${(}${)}${const}${volatile}${abstract} { return _extends; };");
  op1->set_CppDef("");
  op1->set_JavaDecl("  ${comment}${visibility}${final}${static}${abstract}${synchronized}${type} ${name}${(}${)}${throws}${staticnl}{ return _extends; }");
  op1->set_Description("  return the optional extend (Java)");
  op1->moveAfter(baseformalparam->get_operation("defaultValue"));
  
  // update UmlBaseFormalParameter::read_()
  
  UmlCom::trace("update UmlBaseFormalParameter::read_()<br>\n");
  op1 = baseformalparam->get_operation("read_");
  op1->set_CppBody("  _name = UmlCom::read_string();\n"
		   "  _type = UmlCom::read_string();\n"
		   "  _default_value.type = (UmlClass *) UmlBaseItem::read_();\n"
		   "  if (_default_value.type == 0)\n"
		   "    _default_value.explicit_type = UmlCom::read_string();\n"
		   "  _extends.type = (UmlClass *) UmlBaseItem::read_();\n"
		   "  if (_extends.type == 0)\n"
		   "    _extends.explicit_type = UmlCom::read_string();\n");
  op1->set_JavaBody("  _name = UmlCom.read_string();\n"
		    "  _type = UmlCom.read_string();\n"
		    "  _default_value = new UmlTypeSpec();\n"
		    "  _default_value.type = (UmlClass) UmlBaseItem.read_();\n"
		    "  if (_default_value.type == null)\n"
		    "    _default_value.explicit_type = UmlCom.read_string();\n"
		    "  _extends = new UmlTypeSpec();\n"
		    "  _extends.type = (UmlClass) UmlBaseItem.read_();\n"
		    "  if (_extends.type == null)\n"
		    "    _extends.explicit_type = UmlCom.read_string();\n");
  
  // update an UmlCom::send_cmd

  ch = UmlClass::get("UmlCom", 0)->children();
  
  UmlCom::trace("update UmlCom::send_cmd(...)<br>\n");
  
  for (i = 0; i != ch.size(); i += 1) {
    if ((ch[i]->kind() == anOperation) &&
	(ch[i]->name() == "send_cmd")) {
      op1 = (UmlOperation *) ch[i];
      
      QValueList<UmlParameter> params = op1->params();
      
      if (params.count() == 6) {
	UmlParameter p = params.last();
	
	p.name = "arg5";
	op1->addParameter(6, p);
	op1->set_CppDecl("    ${comment}${friend}${static}${virtual}${type} ${name}${(}const ${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, const ${t3} ${p3}, const ${t4} ${p4}, const ${t5} & ${p5}, const ${t6} & ${p6}${)}${const}${volatile}${abstract};");
	op1->set_CppDef("${inline}${type} ${class}::${name}${(}const ${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, const ${t3} ${p3}, const ${t4} ${p4}, const ${t5} & ${p5}, const ${t6} & ${p6}${)}${const}${volatile}${staticnl}{\n${body}}");
	op1->set_JavaDef("  ${comment}${visibility}${final}${static}${abstract}${synchronized}${type} ${name}${(}${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, ${t3} ${p3}, ${t4} ${p4}, ${t5} ${p5}, ${t6} ${p6}${)}${throws}${staticnl}{\n${body}}");
	op1->set_CppBody("#ifdef DEBUGBOUML\n"
			 "  cout << \"UmlCom::send_cmd(id, \" << cmd << \", \" << arg1 << \\\", \\\"\" << arg2 << \"\\\", \\\"\" << arg3 << \"\\\", \" << \", UmlTypeSpec, UmlTypeSpec)\\n\";\n"
			 "#endif\n"
			 "  \n"
			 "  write_char(onInstanceCmd);\n"
			 "  write_id(id);\n"
			 "  write_char(cmd);\n"
			 "  write_unsigned(arg1);\n"
			 "  write_string(arg2);\n"
			 "  write_string(arg3);\n"
			 "  if (arg4.type) {\n"
			 "    write_id(arg4.type->_identifier);\n"
			 "    write_string(\"\");\n"
			 "  }\n"
			 "  else {\n"
			 "    write_id(0);\n"
			 "    write_string(arg4.explicit_type);\n"
			 "  }\n"
			 "  if (arg5.type) {\n"
			 "    write_id(arg5.type->_identifier);\n"
			 "    write_string(\"\");\n"
			 "  }\n"
			 "  else {\n"
			 "    write_id(0);\n"
			 "    write_string(arg5.explicit_type);\n"
			 "  }\n"
			 "  flush();\n");
	op1->set_JavaBody("  write_char((byte) CmdFamily._onInstanceCmd);\n"
			  "  write_id(id);\n"
			  "  write_char((byte) cmd.value());\n"
			  "  write_unsigned(arg1);\n"
			  "  write_string(arg2);\n"
			  "  write_string(arg3);\n"
			  "  if (arg4.type != null) {\n"
			  "    write_id(arg4.type.identifier_());\n"
			  "    write_string(null);\n"
			  "  }\n"
			  "  else {\n"
			  "    write_id(0);\n"
			  "    write_string(arg4.explicit_type);\n"
			  "  }\n"
			  "  if (arg5.type != null) {\n"
			  "    write_id(arg5.type.identifier_());\n"
			  "    write_string(null);\n"
			  "  }\n"
			  "  else {\n"
			  "    write_id(0);\n"
			  "    write_string(arg5.explicit_type);\n"
			  "  }\n"
			  "  flush();\n");
	break;
      }
    }
  }
  
  
  // upgrade addFormal() and replaceFormal on UmlBaseClass
  
  UmlClass * baseclass = UmlClass::get("UmlBaseClass", 0);
  
  UmlCom::trace("update UmlBaseClass::addFormal()<br>\n");
  op1 = baseclass->get_operation("addFormal");
  op1->set_CppBody("  UmlCom::send_cmd(_identifier, addFormalCmd, rank, formal._name,\n"
		   "		   formal._type, formal._default_value, formal._extends);\n"
		   "  return UmlCom::read_bool();\n");
  op1->set_JavaBody("  UmlCom.send_cmd(identifier_(), OnInstanceCmd.addFormalCmd, rank, formal._name,\n"
		    "		   formal._type, formal._default_value, formal._extends);\n"
		    "  UmlCom.check();\n");
  
  UmlCom::trace("update UmlBaseClass::replaceFormal()<br>\n");
  op1 = baseclass->get_operation("replaceFormal");
  op1->set_CppBody("  UmlCom::send_cmd(_identifier, replaceFormalCmd, rank, formal._name,\n"
		   "		   formal._type, formal._default_value, formal._extends);\n"
		   "  return UmlCom::read_bool();\n");
  op1->set_JavaBody("  UmlCom.send_cmd(identifier_(), OnInstanceCmd.replaceFormalCmd, rank, formal._name,\n"
		    "		   formal._type, formal._default_value, formal._extends);\n"
		    "  UmlCom.check();\n");
  
  //
  // adds annotation management
  //

  // add OnInstanceCmd::setJavaAnnotationCmd
      
  UmlClass::get("OnInstanceCmd", 0)->add_enum_item("setJavaAnnotationCmd");

  // add operations javaAnnotations and set_JavaAnnotations
  // on UmlBaseClassMember 
  
  UmlClass * baseclassmember = UmlClass::get("UmlBaseClassMember", 0);
  
  op1 = baseclassmember->add_op("javaAnnotations", PublicVisibility, "string");
  op1->set_Description(" returns the Java annotations");
  op1->set_cpp("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _java_annotation;\n",
	      FALSE, "WITHJAVA", 0);
  op1->set_java("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _java_annotation;\n",
	       FALSE);
  op1->moveAfter(baseclassmember->get_operation("set_CppVisibility"));

  op2 = baseclassmember->add_op("set_JavaAnnotations", PublicVisibility, "bool", TRUE);
  op2->add_param(0, InputDirection, "v", "str");
  op2->set_Description(" set the Java annotations\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->set_cpp("${type}", "const ${t0} ${p0}",
	       "  return set_it_(_java_annotation, v, setJavaAnnotationCmd);\n",
	       FALSE, 0, "#endif");
  op2->set_java("void", "${t0} ${p0}", "\
  UmlCom.send_cmd(identifier_(), OnInstanceCmd.setJavaAnnotationCmd, v);\n\
  UmlCom.check();\n\
  \n\
  _java_annotation = v;\n",
	       FALSE);
  op2->moveAfter(op1);
  
  // add _java_annotations on UmlBaseClassMember 
  
  ch = baseclassmember->children();
  att = baseclassmember->add_attribute("_java_annotation", PrivateVisibility,
				       "string", "WITHJAVA", "#endif");
  att->move_after(aRelation, "_cpp_visibility");
  
  // add read_java_ on UmlBaseClassMember
  
  op1 = baseclassmember->add_op("read_java_", ProtectedVisibility, "void");
  op1->set_isCppVirtual(TRUE);
  op1->set_cpp("${type}", "", "\
  UmlBaseClassItem::read_java_();\n\
  _java_annotation = UmlCom::read_string();\n",
	      FALSE, "WITHJAVA", "#endif");
  op1->set_java("${type}", "", "\
  super.read_java_();\n\
  _java_annotation = UmlCom.read_string();\n",
	       FALSE);
  op1->moveAfter(baseclassmember->get_operation("read_cpp_"));
  
  // update read_java_ on UmlBaseClass UmlBaseAttribute UmlBaseRelation
  // and UmlBaseOperation to call UmlBaseClassMember::read_java_
  
  UmlCom::trace("update UmlBaseClass::read_java_()<br>\n");
  baseclass->get_operation("read_java_")
    ->set_CppBody("  UmlBaseClassMember::read_java_();\n"
		  "  _java_public = UmlCom::read_bool();\n"
		  "  _java_final = UmlCom::read_bool();\n"
		  "  _java_external = UmlCom::read_bool();\n");
  
  UmlCom::trace("update UmlBaseAttribute::read_java_()<br>\n");
  UmlClass::get("UmlBaseAttribute", 0)->get_operation("read_java_")
    ->set_CppBody("  UmlBaseClassMember::read_java_();\n"
		  "  _java_transient = UmlCom::read_bool();\n");
  
  UmlCom::trace("update UmlBaseRelation::read_java_()<br>\n");
  UmlClass::get("UmlBaseRelation", 0)->get_operation("read_java_")
    ->set_CppBody("  UmlBaseClassMember::read_java_();\n"
		  "  _java_transient = UmlCom::read_bool();\n");
  
  UmlCom::trace("update UmlBaseOperation::read_java_()<br>\n");
  UmlClass::get("UmlBaseOperation", 0)->get_operation("read_java_")
    ->set_CppBody("  UmlBaseClassMember::read_java_();\n"
		  "  _java_final = UmlCom::read_bool();\n"
		  "  _java_synchronized = UmlCom::read_bool();\n"
		  "  _java_name_spec = UmlCom::read_string();\n");
  
  // done
  
  UmlCom::set_user_id(uid);
}
 
//
// fixe Package associated diagram management
//

void fixe_package_diagram()
{
  UmlCom::trace("<b>fixe operations on UmlBasePackage</b><br>\n");
  
  UmlClass * basepackage = UmlClass::get("UmlBasePackage", 0);
  UmlOperation * op;
  UmlTypeSpec t;
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  // fixe UmlBasePackage::associatedDiagram()
  
  op = basepackage->get_operation("associatedDiagram");
  t.type = UmlClass::get("UmlDiagram", 0);
  op->set_ReturnType(t);
  UmlCom::trace("operation UmlBasePackage::associatedDiagram corrected<br>\n");
  
  UmlParameter param;
  
  // fixe UmlBasePackage::set_AssociatedDiagram()
  
  param.dir = InputOutputDirection;
  param.type.type = t.type;
  param.name = "d";
  op = basepackage->get_operation("set_AssociatedDiagram");
  op->replaceParameter(0, param);
  UmlCom::trace("operation UmlBasePackage::set_AssociatedDiagram corrected<br>\n");
  
  // fixe UmlBasePackage::read_uml_()
  
  op = basepackage->get_operation("read_uml_");
  op->set_CppBody("  _assoc_diagram = (UmlDiagram *) UmlBaseItem::read_();\n"
		  "  UmlBaseItem::read_uml_();\n");
  op->set_JavaBody("  _assoc_diagram = (UmlDiagram) UmlBaseItem.read_();\n"
		   "  super.read_uml_();\n");
  UmlCom::trace("operation UmlBasePackage::read_ corrected<br>\n");
  
  // replace _assoc_diagram
  
  const QVector<UmlItem> ch = basepackage->children();
  unsigned i;
  
  for (i = 0; i != ch.size(); i += 1) {
    if (ch[i]->kind() == aRelation) {
      if (((UmlRelation *) ch[i])->roleName() == "_assoc_diagram") {
	UmlCom::trace("replace UmlBasePackage::_assoc_diagram<br>\n");
	UmlRelation * old = (UmlRelation *) ch[i];
	UmlRelation * rel = UmlRelation::create(aDirectionalAssociation,
						basepackage,
						UmlClass::get("UmlDiagram", 0));
	
	rel->set_CppDecl(old->cppDecl());
	rel->set_JavaDecl(old->javaDecl());
	rel->set_Visibility(old->visibility());
        rel->moveAfter(old);
	old->deleteIt();
	rel->set_RoleName("_assoc_diagram");
	break;
      }
    }
  }
    
  UmlCom::set_user_id(uid);
}

//
// fixe Java definition of UmlBaseActualParameter.read_
//

void fixe_umlbaseactualparameter_read()
{
  UmlClass::get("UmlBaseActualParameter", 0)->get_operation("read_")
    ->set_JavaBody("  _superClass = (UmlClass) UmlBaseItem.read_();	// cannot be 0\n"
		   "  _rank = UmlCom.read_unsigned();\n"
		   "  _value = new UmlTypeSpec();\n"
		   "  _value.type = (UmlClass) UmlBaseItem.read_();\n"
		   "  if (_value.type == null)\n"
		   "    _value.explicit_type = UmlCom.read_string();\n");
  UmlCom::trace("operation UmlBaseActualParameter.read_ corrected<br>\n");
}

//
// add class association management
//

void add_class_association(UmlClass * baserelation)
{
  UmlCom::trace("<b>add class association</b><br>\n");
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  // add OnInstanceCmd::setRelationAssocClassCmd
      
  UmlClass::get("OnInstanceCmd", 0)->add_enum_item("setRelationAssocClassCmd");
  
  // add relation _association
  
  UmlClass * typespec = UmlClass::get("UmlTypeSpec", 0);
  UmlRelation * rel =
    baserelation->add_relation(aDirectionalAggregationByValue, 
			       "_association", PrivateVisibility,
				typespec, 0, 0);
  
  rel->move_after(anAttribute, "_multiplicity");
  
  // add association on UmlBaseRelation
  
  UmlOperation * op1 =
    baserelation->add_op("association", PublicVisibility, typespec);
  
  op1->set_cpp("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _association;\n",
	       FALSE, 0, 0);
  op1->set_java("${type}", "", "\
  read_if_needed_();\n\
  \n\
  return _association;\n",
		FALSE);
  op1->set_Description(" return the associated class/type, may be an empty spec");
  op1->moveAfter(baserelation->get_operation("roleType"));
 
  // add set_Association on UmlBaseRelation
  
  UmlOperation * op2 =
    baserelation->add_op("set_Association", PublicVisibility, "bool", TRUE);
  
  op2->add_param(0, InputDirection, "t", typespec);
  op2->set_Description(" set the associated class/type, may be an empty spec\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->set_cpp("${type}", "const ${t0} & ${p0}", "\
  if (set_it_(_association, t, setRelationAssocClassCmd)) {\n\
    UmlBaseRelation * other = (UmlBaseRelation *) UmlBaseItem::read_();\n\
    \n\
    if (other != 0)\n\
      other->_association = t;\n\
    return TRUE;\n\
  }\n\
  else\n\
    return FALSE;\n",
	      FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}", "\
  UmlCom.send_cmd(identifier_(), OnInstanceCmd.setRelationAssocClassCmd, t);\n\
  UmlCom.check();\n\
  \n\
  _association = t;\n\
  UmlBaseRelation other = (UmlBaseRelation) UmlBaseItem.read_();\n\
  \n\
  if (other != null)\n\
    other._association = t;\n",
	       FALSE);
  op2->moveAfter(op1);
  
  // upgrade read_uml_
  
  op1 = baserelation->get_operation("read_uml_");
  
  op1->set_CppBody("  UmlBaseClassMember::read_uml_();\n"
		   "  _rel_kind = (aRelationKind) UmlCom::read_char();\n"
		   "  _role_type = (UmlClass *) UmlBaseItem::read_();\n"
		   "  _association.type = (UmlClass *) UmlBaseItem::read_();\n"
		   "  if (_association.type == 0)\n"
		   "    _association.explicit_type = UmlCom::read_string();\n"
		   "  _role_name = UmlCom::read_string();\n"
		   "  _multiplicity = UmlCom::read_string();\n"
		   "  _default_value = UmlCom::read_string();\n"
		   "  _read_only = UmlCom::read_bool();\n"
		   "  _get_oper = (UmlOperation *) UmlBaseItem::read_();\n"
		   "  _set_oper = (UmlOperation *) UmlBaseItem::read_();\n");
  op1->set_JavaBody("  super.read_uml_();\n"
		    "  _rel_kind = aRelationKind.fromInt(UmlCom.read_char());\n"
		    "  _role_type = (UmlClass) UmlBaseItem.read_();\n"
		    "  _association = new UmlTypeSpec();\n"
		    "  _association.type = (UmlClass) UmlBaseItem.read_();\n"
		    "  if (_association.type == null)\n"
		    "    _association.explicit_type = UmlCom.read_string();\n"
		    "  _role_name = UmlCom.read_string();\n"
		    "  _multiplicity = UmlCom.read_string();\n"
		    "  _default_value = UmlCom.read_string();\n"
		    "  _read_only = UmlCom.read_bool();\n"
		    "  _get_oper = (UmlOperation) UmlBaseItem.read_();\n"
		    "  _set_oper = (UmlOperation) UmlBaseItem.read_();\n");
  UmlCom::trace("operation UmlBaseRelation::read_uml_ upgraded<br>\n");
    
  UmlCom::set_user_id(uid);
}

//
// add visibility 'package' management
//

void add_package_visibility()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  // add aVisibility::setRelationAssocClassCmd
  UmlAttribute * at =
    UmlClass::get("aVisibility", 0)->add_enum_item("PackageVisibility");
  
  at->move_after(anAttribute, "PrivateVisibility");
    
  UmlCom::set_user_id(uid);
}

//
// add UmlBaseRelation::side()
//

void add_side(UmlClass * baserelation)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);  
  UmlOperation * op =
    baserelation->add_op("side", PublicVisibility,
			 UmlClass::get("UmlRelation", 0));
  op->add_param(0, InputDirection, "first", "bool");
  op->set_cpp("${type} *", "${t0} ${p0}", "\
  UmlCom::send_cmd(_identifier, sideCmd, (char) first);\n\
  \n\
  return (UmlRelation *) UmlBaseItem::read_();\n",
	       FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}", "\
  UmlCom.send_cmd(identifier_(), OnInstanceCmd.sideCmd,\n\
		  (first) ? (byte) 1 : (byte) 0);\n\
  \n\
  return (UmlRelation) UmlBaseItem.read_();\n",
		FALSE);
  op->set_Description(" if 'first' is true returns the relation associated to the\n"
		      " first role, else the relation associated to the second\n"
		      " role or 0/null if the relation is uni directional");
  op->move_after(anOperation, "relationKind");
  
  // upgrade OnInstanceCmd
  
  UmlClass * cl = UmlClass::get("OnInstanceCmd", 0);
  
  cl->add_enum_item("sideCmd")->move_after(anAttribute, "referencedByCmd");
    
  UmlCom::set_user_id(uid);
}

//
// add ObjectBaseDiagram & ObjectDiagram classes
//

void add_object_activity_diagram_item_kind()
{
  // already root
  UmlClass * itkind = UmlClass::get("anItemKind", 0);
  UmlAttribute * anObjectDiagram = itkind->add_enum_item("anObjectDiagram");
  UmlAttribute * anActivityDiagram = itkind->add_enum_item("anActivityDiagram");
  
  const QVector<UmlItem> ch = itkind->children();
    
  for (unsigned i = 0; i != ch.size(); i += 1) {
    if (ch[i]->name() == "aDeploymentDiagram") {
      anObjectDiagram->moveAfter(ch[i]);
      anActivityDiagram->moveAfter(anObjectDiagram);
      break;
    }
  }
}

void baseitem_read_objectdiagram(UmlClass * base_item) {
  // update read_()'s body
  UmlOperation * op = base_item->get_operation("read_");
  
  if (op != 0) {
    QCString body;
    
    body = op->cppBody();
    body.insert(body.findRev("default:"),
		"case anObjectDiagram:\n\
      return new UmlObjectDiagram(id, name);\n\
    case anActivityDiagram:\n\
      //return new UmlActivityDiagram(id, name);\n\
    ");
    op->set_CppBody(body);
      
    body = op->javaBody();
    body.insert(body.findRev("default:"),
		"case anItemKind._anObjectDiagram:\n\
      return new UmlObjectDiagram(id, name);\n\
    case anItemKind._anActivityDiagram:\n\
      //return new UmlActivityDiagram(id, name);\n\
    ");
    op->set_JavaBody(body);
  }
  
  // update BaseUmlItem artifact
  UmlArtifact * art = base_item->associatedArtifact();
  QCString s;
  
  s = art->cppSource();
  s.insert(s.find("#include \"UmlDeploymentDiagram.h\""),
	   "#include \"UmlObjectDiagram.h\"\n");
  art->set_CppSource(s);
}
				 
void add_objectdiagram(UmlClass * base_item, UmlClass * user_item)
{
  UmlClassView * base_class_view = (UmlClassView *) base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) user_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    user_item->associatedArtifact()->parent();
  UmlClass * user_diagram = UmlClass::get("UmlDiagram", 0);
  UmlClass * base_objectdiagram;
  UmlClass * user_objectdiagram;
  
  UmlCom::trace("<b>Object diagram</b><br>\n");
  
  user_diagram->made(base_class_view, user_class_view,
		     base_depl_view, user_depl_view,
		     base_item, "ObjectDiagram",
		     base_objectdiagram, user_objectdiagram,
		     0);
  base_objectdiagram->add_default_base_op(user_diagram, user_objectdiagram, user_item,
					  "object diagram", "anObjectDiagram");
  user_objectdiagram->add_constr(base_objectdiagram, PublicVisibility);

  user_objectdiagram->set_Description(" This class manages 'object diagrams', notes that the class 'UmlDiagram'\n"
				      " is a mother class of all the diagrams, allowing to generalize their\n"
				      " management\n"
				      "\n"
				      " You can modify it as you want (except the constructor)");
  UmlClass * cl;
  
  if ((cl = UmlClass::get("UmlBaseClassDiagram", 0)) != 0)
    base_objectdiagram->moveAfter(cl);
  
  if ((cl = UmlClass::get("UmlClassDiagram", 0)) != 0)
    user_objectdiagram->moveAfter(cl);
  
  //
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("update anItemKind<br>\n");
  
  add_object_activity_diagram_item_kind();
  
  UmlCom::trace("update item read<br>\n");
  
  baseitem_read_objectdiagram(base_item);
  
  UmlCom::set_user_id(uid);
}

//
// add void UmlCom::showTrace() and
// void UmlCom::traceAutoRaise(bool y)
//
// fixe write_bool definition in java
//

void add_new_trace_operations(UmlClass * uml_com)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>add trace operations on UmlCom</b><br>\n");
  
  UmlOperation * op1 =
    uml_com->add_op("showTrace", PublicVisibility, "void", FALSE);
  
  op1->set_Description("to show the trace window\n");
  op1->set_isClassMember(TRUE);
  
  op1->set_cpp("${type}", "",
	      "  send_cmd(miscGlobalCmd, showTraceCmd);\n",
	      FALSE, 0, 0);

  op1->set_java("${type}", "",
	       "  send_cmd(CmdFamily.miscGlobalCmd, MiscGlobalCmd._showTraceCmd);\n",
	       FALSE);
  op1->moveAfter(uml_com->get_operation("trace"));
  
  //
  
  UmlOperation * op2 =
    uml_com->add_op("traceAutoRaise", PublicVisibility, "void", FALSE);
  
  op2->add_param(0, InputDirection, "y", "bool");

  op2->set_Description("to automatically raise the trace window\n"
		      "each time trace() is called\n");
  op2->set_isClassMember(TRUE);
  
  op2->set_cpp("${type}", "${t0} ${p0}",
	      "  send_cmd(miscGlobalCmd, traceAutoRaiseCmd, (y == 0) ? 0 : 1);\n",
	      FALSE, 0, 0);

  op2->set_java("${type}", "${t0} ${p0}",
	       "  send_cmd(CmdFamily.miscGlobalCmd, MiscGlobalCmd._traceAutoRaiseCmd, (y) ? (byte) 1 : (byte) 0);\n",
	       FALSE);
  op2->moveAfter(op1);
  
  //
  
  UmlOperation * op3 = uml_com->get_operation("write_bool");
    
  if (op3 != 0)
    op3->set_JavaBody("  check_size_out(1);\n"
		      "\n"
		      "  buffer_out[p_buffer_out++] = (b) ? (byte) 1 : (byte) 0;\n");
  
  // add new global cmd

  UmlClass * cl = UmlClass::get("MiscGlobalCmd", 0);
  UmlAttribute * at1 = cl->add_enum_item("showTraceCmd");
  UmlAttribute * at2;
  
  if ((at2 = uml_com->get_attribute("loadCmd")) != 0)
    at1->moveAfter(at2);
  
  cl->add_enum_item("traceAutoRaiseCmd")->moveAfter(at1);
  
  // update UmlCom description
  uml_com->set_Description(
			   " This class manages the communications\n"
			   "\n"
			   " This class may be defined as a 'singleton', but I prefer to use static\n"
			   " members allowing to just write 'UmlCom::member' rather than\n"
			   " 'UmlCom::instance()->member' or other long sentence like this.\n"
			   "\n"
			   " The operation you can use yourself are :\n"
			   "\n"
			   " - connect()\n"
			   "\n"
			   " - targetItem()\n"
			   "\n"
			   " - trace()\n"
			   "\n"
			   " - showTrace()\n"
			   "\n"
			   " - traceAutoRaise()\n"
			   "\n"
			   " - message()\n"
			   "\n"
			   " - bye()\n"
			   "\n"
			   " - close()\n"
			   " \n"
			   " you must NOT call the others\n"
			   );
  //
  
  UmlCom::set_user_id(uid);
}
     
//
//
//

void add_cpp_set_param_ref(UmlClass * cppsetting)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>update CppSettings</b><br>\n");

  UmlAttribute * att = 
    cppsetting->add_attribute("_is_set_param_ref", PrivateVisibility, "bool", 0, 0);

  att->set_isClassMember(TRUE);
  att->moveAfter(cppsetting->get_attribute("_is_set_param_const"));

  //

  UmlOperation * op = cppsetting->get_operation("read_");
  QCString s;
  
  s = op->cppBody() + "  _is_set_param_ref = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _is_set_param_ref = UmlCom.read_bool();\n";
  op->set_JavaBody(s);
  
  //

  UmlOperation * op2 = cppsetting->get_operation("set_IsSetParamConst");

  op = cppsetting->add_op("set_IsSetParamRef", PublicVisibility, "bool", TRUE);
  op->set_isClassMember(TRUE);
  
  op->add_param(0, InputDirection, "v", "bool");
  op->set_Description(" set if the parameter of a 'set' operation generated through the\n"
		      " attribute and relation 'add set operation' menu is a reference by default\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  
  op->set_CppDecl(op2->cppDecl());
  op->set_CppDef(op2->cppDef());
  op->set_CppBody("\
  UmlCom::send_cmd(cppSettingsCmd, setCppIsSetParamRefCmd, v);\n\
  if (UmlCom::read_bool()) {\n\
    _is_set_param_ref = v;\n\
    return TRUE;\n\
  }\n\
  else\n\
    return FALSE;\n");

  op->set_JavaDecl(op2->javaDecl());
  op->set_JavaBody("\
  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppIsSetParamRefCmd,\n\
		  (v) ? (byte) 1 : (byte) 0);\n\
  UmlCom.check();\n\
  \n\
  _is_set_param_ref = v;\n");
  
  op->moveAfter(op2);

  //

  UmlClass::get("CppSettingsCmd", 0)->add_enum_item("setCppIsSetParamRefCmd");

  //

  UmlCom::set_user_id(uid);
}

void add_getter_setter_on_instance_cmd()
{
  // already root
  UmlClass * itcmd = UmlClass::get("OnInstanceCmd", 0);
  
  itcmd->add_enum_item("setCppFrozenCmd");
  itcmd->add_enum_item("setJavaFrozenCmd");
  itcmd->add_enum_item("setIdlFrozenCmd");
}    

void upgrade_setter_getter()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>update UmlBaseOperation setter and getter</b><br>\n");

  //

  add_getter_setter_on_instance_cmd();

  //

  UmlClass * baseoper = UmlClass::get("UmlBaseOperation", 0);
  UmlAttribute * att;
  UmlAttribute * att2;
  QCString s;
  
  att = baseoper->add_attribute("_cpp_get_set_frozen", PrivateVisibility, "bool", "WITHCPP", "endif", " : 1");  
  att->moveAfter(baseoper->get_attribute("_idl_oneway"));

  att2 = baseoper->add_attribute("_java_get_set_frozen", PrivateVisibility, "bool", "WITHJAVA", "endif", " : 1");  
  att2->moveAfter(att);

  att = baseoper->add_attribute("_idl_get_set_frozen", PrivateVisibility, "bool", "WITHIDL", "endif", " : 1");  
  att->moveAfter(att2);

  //

  UmlOperation * op;
  UmlOperation * pos;
  
  pos = baseoper->get_operation("set_CppNameSpec");
  defGetBool(baseoper, _cpp_get_set_frozen, cppGetSetFrozen, "WITHCPP", 0,
	     " indicate if the C++ definition is frozen, only for getter/setter operation");
  op->moveAfter(pos);
  pos = op;
  defSetBoolBitField(baseoper, _cpp_get_set_frozen, set_CppGetSetFrozen, setCppFrozenCmd, 0, "endif",
		     " set if the C++ definition is frozen, only for getter/setter operation");
  op->moveAfter(pos);

  pos = baseoper->get_operation("set_JavaNameSpec");
  defGetBool(baseoper, _java_get_set_frozen, javaGetSetFrozen, "WITHJAVA", 0,
	     " indicate if the Java definition is frozen, only for getter/setter operation");
  op->moveAfter(pos);
  pos = op;
  defSetBoolBitField(baseoper, _java_get_set_frozen, set_JavaGetSetFrozen, setJavaFrozenCmd, 0, "endif",
		     " set if the Java definition is frozen, only for getter/setter operation");
  op->moveAfter(pos);

  pos = baseoper->get_operation("set_IdlNameSpec");
  defGetBool(baseoper, _idl_get_set_frozen, idlGetSetFrozen, "WITHIDL", 0,
	     " indicate if the IDL definition is frozen, only for getter/setter operation");
  op->moveAfter(pos);
  pos = op;
  defSetBoolBitField(baseoper, _idl_get_set_frozen, set_IdlGetSetFrozen, setIdlFrozenCmd, 0, "endif",
		     " set if the IDL definition is frozen, only for getter/setter operation");
  op->moveAfter(pos);

  //

  op = baseoper->get_operation("read_cpp_");
  s = op->cppBody() + "  _cpp_get_set_frozen = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _cpp_get_set_frozen = UmlCom.read_bool();\n";
  op->set_JavaBody(s);

  op = baseoper->get_operation("read_java_");
  s = op->cppBody() + "  _java_get_set_frozen = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _java_get_set_frozen = UmlCom.read_bool();\n";
  op->set_JavaBody(s);

  op = baseoper->get_operation("read_idl_");
  s = op->cppBody() + "  _idl_get_set_frozen = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _idl_get_set_frozen = UmlCom.read_bool();\n";
  op->set_JavaBody(s);

  //

  UmlCom::set_user_id(uid);
}

//
//
//

void add_cpp_relative_path_force_namespace(UmlClass * cppsetting)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>update CppSettings</b><br>\n");

  //

  UmlOperation * op = cppsetting->get_operation("read_");
  QCString s;
  
  s = op->cppBody() + 
    "  _is_relative_path = UmlCom::read_bool();\n"
      "  _is_force_namespace_gen = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + 
    "  _is_relative_path = UmlCom.read_bool();\n"
      "  _is_force_namespace_gen = UmlCom.read_bool();\n";
  op->set_JavaBody(s);
  
  //
  // add missing operation isSetParamRef()
  // 
  
  op = cppsetting->add_op("isSetParamRef", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return if the parameter of a 'set' operation generated through the\n"
		      " attribute and relation 'add set operation' menu is a reference by default");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _is_set_param_ref;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _is_set_param_ref;\n",
	       FALSE);
  op->moveAfter(cppsetting->get_operation("set_IsSetParamConst"));
  
  //
  // relative path
  //
  
  UmlAttribute * att1 = 
    cppsetting->add_attribute("_is_relative_path", PrivateVisibility, "bool", 0, 0);

  att1->set_isClassMember(TRUE);
  att1->moveAfter(cppsetting->get_attribute("_incl_with_path"));

  // get
  
  op = cppsetting->add_op("isRelativePath", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return if a relative path must be used when the path\n"
		      " must be generated in the produced #includes");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _is_relative_path;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _is_relative_path;\n",
	       FALSE);
  op->moveAfter(cppsetting->get_operation("set_IncludeWithPath"));

  // set
  
  UmlOperation * op2 = cppsetting->add_op("set_IsRelativePath", PublicVisibility, "bool", TRUE);
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set if a relative path must be used when the path\n"
		       " must be generated in the produced #includes\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(cppSettingsCmd, setCppRelativePathCmd, v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _is_relative_path = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppRelativePathCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_relative_path = v;\n",
		FALSE);
  op2->moveAfter(op);

  //
  // force namespace prefix generation
  //
  
  UmlAttribute * att2 = 
    cppsetting->add_attribute("_is_force_namespace_gen", PrivateVisibility, "bool", 0, 0);

  att2->set_isClassMember(TRUE);
  att2->moveAfter(att1);

  // get
  
  op = cppsetting->add_op("isForceNamespacePrefixGeneration", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return if the namespace prefix must be\n"
		      " always generated before class's names");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _is_force_namespace_gen;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _is_force_namespace_gen;\n",
	       FALSE);
  op->moveAfter(op2);

  // set
  
  op2 = cppsetting->add_op("set_IsForceNamespacePrefixGeneration", PublicVisibility, "bool", TRUE);
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set if the namespace prefix must be always generated before class's names\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(cppSettingsCmd, setCppForceNamespaceGenCmd, v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _is_force_namespace_gen = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppForceNamespaceGenCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_force_namespace_gen = v;\n",
		FALSE);
  op2->moveAfter(op);
  
  //

  UmlClass * cppsettingcmd = UmlClass::get("CppSettingsCmd", 0);
  
  cppsettingcmd->add_enum_item("setCppRelativePathCmd");
  cppsettingcmd->add_enum_item("setCppForceNamespaceGenCmd");

  //

  UmlCom::set_user_id(uid);
}

//
//
//

void add_getter_setter_rules(UmlClass * umlsetting)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>update UmlSettings</b><br>\n");
  
  //
  
  UmlClassView * base_class_view = (UmlClassView *) umlsetting->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    umlsetting->associatedArtifact()->parent();
  UmlClass * lang = 
    UmlClass::made(base_class_view, base_depl_view, "aLanguage");

  lang->set_Stereotype("enum_pattern");
  lang->set_CppDecl(CppSettings::enumDecl());
  lang->set_JavaDecl(JavaSettings::enumPatternDecl());
  lang->add_enum_item("umlLanguage");
  lang->add_enum_item("cppLanguage");
  lang->add_enum_item("javaLanguage");
  lang->add_enum_item("idlLanguage");
  
  //
  
  UmlRelation * rel1 =
    umlsetting->add_relation(aDirectionalAggregationByValue, "_uml_get_name",
			     PrivateVisibility, lang, 0, 0);
  rel1->set_isClassMember(TRUE);
  rel1->moveAfter(umlsetting->get_attribute("_defined"));
  
  UmlRelation * rel2 =
    umlsetting->add_relation(aDirectionalAggregationByValue, "_uml_set_name",
			     PrivateVisibility,	lang, 0, 0); 
  rel2->set_isClassMember(TRUE);
  rel2->moveAfter(rel1);
  
  //

  UmlOperation * op = umlsetting->get_operation("read_");
  QCString s;
  
  s = op->cppBody() + 
    "  _uml_get_name = (aLanguage) UmlCom::read_char();\n"
      "  _uml_set_name = (aLanguage) UmlCom::read_char();\n";
  op->set_CppBody(s);
  s = op->javaBody() + 
    "  _uml_get_name = aLanguage.fromInt(UmlCom.read_char());\n"
      "  _uml_set_name = aLanguage.fromInt(UmlCom.read_char());\n";
  op->set_JavaBody(s);
  
  // get getter name
  
  op = umlsetting->add_op("umlGetName", PublicVisibility, lang);
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return the language from which the getter's name rule must be followed at Uml level");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _uml_get_name;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _uml_get_name;\n",
	       FALSE);
  op->moveAfter(umlsetting->get_operation("set_RelationDescription"));

  // set getter name
  
  UmlOperation * op2;
  
  op2 = umlsetting->add_op("set_UmlGetName", PublicVisibility, "bool", TRUE);
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set the language from which the getter's name rule must be followed at Uml level\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", lang); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(umlSettingsCmd, setUmlDefaultGetNameCmd, (char) v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _uml_get_name = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.umlSettingsCmd, UmlSettingsCmd._setUmlDefaultGetNameCmd,\n"
		"                  (byte) v.value());\n"
		"  UmlCom.check();\n"
		"  _uml_get_name = v;\n",
		FALSE);
  op2->moveAfter(op);
  
  // get setter name
  
  op = umlsetting->add_op("umlSetName", PublicVisibility, lang);
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return the language from which the setter's name rule must be followed at Uml level");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _uml_set_name;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _uml_set_name;\n",
	       FALSE);
  op->moveAfter(op2);

  // set setter name
  
  op2 = umlsetting->add_op("set_UmlSetName", PublicVisibility, "bool", TRUE);
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set the language from which the setter's name rule must be followed at Uml level\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", lang); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(umlSettingsCmd, setUmlDefaultSetNameCmd, (char) v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _uml_set_name = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.umlSettingsCmd, UmlSettingsCmd._setUmlDefaultSetNameCmd,\n"
		"                  (byte) v.value());\n"
		"  UmlCom.check();\n"
		"  _uml_set_name = v;\n",
		FALSE);
  op2->moveAfter(op);
  
  // update UmlSettingsCmd enum
  
  UmlClass * cl = UmlClass::get("UmlSettingsCmd", 0);
  
  cl->add_enum_item("setUmlDefaultGetNameCmd");
  cl->add_enum_item("setUmlDefaultSetNameCmd");

  //

  UmlCom::set_user_id(uid);
}


//
//
//

void add_extension_points()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>update use case</b><br>\n");
  
  //
  
  UmlClass * base_usecase = UmlClass::get("UmlBaseUseCase", 0);
  UmlOperation * op;
  QCString s;
  
  op = base_usecase->get_operation("read_uml_");
  s = op->cppBody() + "  _extension_points = UmlCom::read_string();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _extension_points = UmlCom.read_string();\n";
  op->set_JavaBody(s);

  //
  
  defGet(base_usecase, _extension_points, extensionPoints, "string", 0, 0,
         "extension points");
  op->moveAfter(base_usecase->get_operation("set_AssociatedDiagram"));
  
  UmlOperation * op2 = op;
  
  defSet(base_usecase, _extension_points, set_ExtensionPoints,
	 "str", replaceExceptionCmd, 0, 0,
	 "extension points");
  op->moveAfter(op2);
  
  //

  base_usecase->add_attribute("_extension_points", PrivateVisibility, "string", 0, 0)
    ->moveAfter(op);

  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void remove_java_public(UmlClass * uml_base_class)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>new class visibility</b><br>\n");
  
  //

  uml_base_class->get_attribute("_java_public")->deleteIt();
  
  UmlOperation * op;
  
  op = uml_base_class->get_operation("isJavaPublic");
  op->set_CppBody("  return visibility() == PublicVisibility;\n");
  op->set_JavaBody("  return visibility() == aVisibility.PublicVisibility;\n");
  
  op = uml_base_class->get_operation("set_isJavaPublic");
  op->set_CppBody("  return set_Visibility((y) ? PublicVisibility : PackageVisibility);\n");
  op->set_JavaBody("  set_Visibility((y) ? aVisibility.PublicVisibility : aVisibility.PackageVisibility);\n");
  
  QCString s;
  
  op = uml_base_class->get_operation("read_java_");
  
  s = op->cppBody();
  s.remove(s.find("_java_public = UmlCom::read_bool();"), 34);
  op->set_CppBody(s);
  
  s = op->javaBody();
  s.remove(s.find("_java_public = UmlCom.read_bool();"), 34);
  op->set_JavaBody(s);
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_cpp_root_relative_path(UmlClass * cppsetting)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>update UmlSettings</b><br>\n");
  
  //
  // add root relative
  //
  
  UmlAttribute * att1 = 
    cppsetting->add_attribute("_is_root_relative_path", PrivateVisibility, "bool", 0, 0);

  att1->set_isClassMember(TRUE);
  att1->moveAfter(cppsetting->get_attribute("_is_relative_path"));

  // get
  
  UmlOperation * op = cppsetting->add_op("isRootRelativePath", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return if a path relative to the project root must be used\n"
		      " when the path must be generated in the produced #includes");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _is_root_relative_path;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _is_root_relative_path;\n",
	       FALSE);
  op->moveAfter(cppsetting->get_operation("set_IsRelativePath"));

  // set
  
  UmlOperation * op2 = cppsetting->add_op("set_IsRootRelativePath", PublicVisibility, "bool", TRUE);
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set if a relative to the project root path must be used\n"
		       " when the path must be generated in the produced #includes\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(cppSettingsCmd, setCppRootRelativePathCmd, v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _is_root_relative_path = v;\n"
		"    if (v) _is_relative_path = FALSE;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppRootRelativePathCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_root_relative_path = v;\n"
		"  if (v) _is_relative_path = false;\n",
		FALSE);
  op2->moveAfter(op);  
  
  // upgrade set_IsRelativePath
  op2 = cppsetting->get_operation("set_IsRelativePath");
  
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(cppSettingsCmd, setCppRelativePathCmd, v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _is_relative_path = v;\n"
		"    if (v) _is_root_relative_path = FALSE;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppRelativePathCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_relative_path = v;\n"
		"  if (v) _is_root_relative_path = false;\n",
		FALSE);
  
  //
  
  UmlClass * cppsettingcmd = UmlClass::get("CppSettingsCmd", 0);
  
  cppsettingcmd->add_enum_item("setCppRootRelativePathCmd");

  //
  
  op = cppsetting->get_operation("read_");

  QCString s;
  
  s = op->cppBody() + "  _is_root_relative_path = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _is_root_relative_path = UmlCom.read_bool();\n";
  op->set_JavaBody(s);
  
  //
  
  UmlCom::set_user_id(uid);
}

///
// add generate javadoc comment settings
//

void add_cpp_generate_javadoc_comment(UmlClass * cppsetting)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>update CppSettings</b><br>\n");
  
  //
  // add _is_generate_javadoc_comment
  //
  
  UmlAttribute * att1 = 
    cppsetting->add_attribute("_is_generate_javadoc_comment", PrivateVisibility, "bool", 0, 0);

  att1->set_isClassMember(TRUE);
  att1->moveAfter(cppsetting->get_attribute("_is_force_namespace_gen"));

  // get
  
  UmlOperation * op = cppsetting->add_op("isGenerateJavadocStyleComment", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return if ${comment} generate Javadoc style comment");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _is_generate_javadoc_comment;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _is_generate_javadoc_comment;\n",
	       FALSE);
  op->moveAfter(cppsetting->get_operation("set_IsForceNamespacePrefixGeneration"));

  // set
  
  UmlOperation * op2 = cppsetting->add_op("set_IsGenerateJavadocStyleComment", PublicVisibility, "bool", TRUE);
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set if ${comment} generate Javadoc style comment\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(cppSettingsCmd, setCppJavadocStyleCmd, v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _is_generate_javadoc_comment = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppJavadocStyleCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_generate_javadoc_comment = v;\n",
		FALSE);
  op2->moveAfter(op);  
  
  //
  
  UmlClass * cppsettingcmd = UmlClass::get("CppSettingsCmd", 0);
  
  cppsettingcmd->add_enum_item("setCppJavadocStyleCmd");

  //
  
  op = cppsetting->get_operation("read_");

  QCString s;
  
  s = op->cppBody() + "  _is_generate_javadoc_comment = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _is_generate_javadoc_comment = UmlCom.read_bool();\n";
  op->set_JavaBody(s);
  
  //
  
  UmlCom::set_user_id(uid);
}

void add_java_generate_javadoc_comment(UmlClass * javasetting)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>update JavaSettings</b><br>\n");
  
  //
  // add _is_generate_javadoc_comment
  //
  
  UmlAttribute * att1 = 
    javasetting->add_attribute("_is_generate_javadoc_comment", PrivateVisibility, "bool", 0, 0);

  att1->set_isClassMember(TRUE);
  att1->moveAfter(javasetting->get_attribute("_ext"));

  // get
  
  UmlOperation * op = javasetting->add_op("isGenerateJavadocStyleComment", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return if ${comment} generate Javadoc style comment");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _is_generate_javadoc_comment;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _is_generate_javadoc_comment;\n",
	       FALSE);
  op->moveAfter(javasetting->get_operation("set_SourceExtension"));

  // set
  
  UmlOperation * op2 = javasetting->add_op("set_IsGenerateJavadocStyleComment", PublicVisibility, "bool", TRUE);
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set if ${comment} generate Javadoc style comment\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(javaSettingsCmd, setJavaJavadocStyleCmd, v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _is_generate_javadoc_comment = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.javaSettingsCmd, JavaSettingsCmd._setJavaJavadocStyleCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_generate_javadoc_comment = v;\n",
		FALSE);
  op2->moveAfter(op);  
  
  //
  
  UmlClass * javasettingcmd = UmlClass::get("JavaSettingsCmd", 0);
  
  javasettingcmd->add_enum_item("setJavaJavadocStyleCmd");

  //
  
  op = javasetting->get_operation("read_");

  QCString s;
  
  s = op->cppBody() + "  _is_generate_javadoc_comment = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _is_generate_javadoc_comment = UmlCom.read_bool();\n";
  op->set_JavaBody(s);
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_constraint(UmlClass * baseclassmember)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>Upgrade class members</b><br>\n");
  
  //
  
  baseclassmember->add_attribute("_constraint", PrivateVisibility, "string",
				 0, 0)
    ->moveAfter(baseclassmember->get_attribute("_java_annotation"));
  
  UmlOperation * op;
  UmlOperation * op1;
  
  defGet(baseclassmember, _constraint, constraint, "string", 0, 0,
         "constraint");
  op->moveAfter(baseclassmember->get_operation("set_JavaAnnotations"));
  op1 = op;
  defSet(baseclassmember, _constraint, set_Constraint, "str", setConstraintCmd, 0, 0,
         "constraint");
  op->moveAfter(op1);
  
  op = baseclassmember->get_operation("read_uml_");
  
  QCString s;
  
  s = op->cppBody() + "  _constraint = UmlCom::read_string();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _constraint = UmlCom.read_string();\n";
  op->set_JavaBody(s);
  
  //
  
  UmlClass::get("OnInstanceCmd", 0)->add_enum_item("setConstraintCmd");
  
  //
  
  UmlCom::set_user_id(uid);
}

//
// add UmlBaseItem::deleteIt
//

void add_deleteit(UmlClass * uml_base_item)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>Upgrade UmlBaseItem</b><br>\n");
 
  //
  
  UmlOperation * op = uml_base_item->add_op("deleteIt", PublicVisibility, "bool", TRUE);
  
  op->set_Description(" remove the element from the model, use it carefully because\n"
		      " after that the element can't be used anymore by the plug-out\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  op->set_CppBody("  UmlCom::send_cmd(_identifier, deleteCmd);\n"
		  "  if (UmlCom::read_bool() == 0) return FALSE;\n"
		  "  parent()->unload(TRUE);\n"
		  "  return TRUE;\n");
  op->set_java("void", "",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.deleteCmd);\n"
	       "  UmlCom.check();\n"
	       "  parent().unload(true, false);\n"
	       , FALSE);
  op->moveAfter(uml_base_item->get_operation("unload"));

  //
  
  UmlClass::get("OnInstanceCmd", 0)->add_enum_item("deleteCmd");
  
  //
  
  UmlCom::set_user_id(uid);
}

//
// add get_id()
//
void add_get_id(UmlClass * uml_base_item)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>Upgrade UmlBaseItem</b><br>\n");
 
  //
  
  UmlOperation * op = uml_base_item->add_op("getIdentifier", PublicVisibility, "int");
  
  op->set_Description(" return a constant identifier, it is unique within a given\n"
		      " kind of element (two classes can't have the same identifiers, but\n"
		      " a class and a component may have the same identifier)\n"
		      " except for the diagrams (a class diagram can't have the identifier\n"
		      " of a component diagram)");
  op->set_CppBody("  read_if_needed_();\n"
		  "\n"
		  "  return _modeler_id;\n");
  op->set_JavaBody("  read_if_needed_();\n"
		   "\n"
		   "  return _modeler_id;\n");
  op->moveAfter(uml_base_item->get_operation("referencedBy"));

  //
  
  UmlAttribute * att =
    uml_base_item->add_attribute("_modeler_id", ProtectedVisibility, "int", 0, 0);

  att->moveAfter(uml_base_item->get_attribute("_identifier"));
  
  //
  
  op = uml_base_item->get_operation("read_uml_");
  
  if (op != 0) {
    QCString body;
    
    body = op->cppBody();
    body += "  _modeler_id = (int) UmlCom::read_unsigned();\n";
    op->set_CppBody(body);
      
    body = op->javaBody();
    body += "  _modeler_id = UmlCom.read_unsigned();\n";
    op->set_JavaBody(body);
  }
  
  //
  
  UmlCom::set_user_id(uid);
}

//
// fixe UmlBaseParameterSet::read_uml_() in java
//

void fixe_parameterset_read_uml()
{
  UmlOperation * op =
    UmlClass::get("UmlBaseParameterSet", 0)->get_operation("read_uml_");
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  op->set_JavaBody("  super.read_uml_();\n"
		   "\n"
		   "  int n = UmlCom.read_unsigned();\n"
		   "\n"
		   "  _pins = new UmlActivityPin[n];\n"
		   "\n"
		   "  for (int index = 0; index != n; index += 1)\n"
		   "    _pins[index] = (UmlActivityPin) UmlBaseItem.read_();\n");
  
  UmlCom::set_user_id(uid);
}

//
// add multiplicity on attributes
//

void add_multiplicity(UmlClass * settings, QCString attr,
			     QCString get, QCString set,
			     QCString who, QCString Who, QCString what)
{
  
  UmlOperation * op;
  QCString s;
  
  // upgrade get
  
  op = settings->get_operation(get);
  op->add_param(0, InputDirection, "multiplicity", "str");
  s = op->cppDecl();
  op->set_CppDecl(s.insert(s.find("${)}"), "const ${t0} ${p0}"));
  s = op->cppDef();
  op->set_CppDef(s.insert(s.find("${)}"), "const ${t0} ${p0}"));
  op->set_CppBody("  read_if_needed_();\n"
		  "\n"
		  "  return " + attr + "[UmlSettings::multiplicity_column(multiplicity)];\n");
  s = op->javaDecl();
  op->set_JavaDef(s.insert(s.find("${)}"), "${t0} ${p0}"));
  op->set_JavaBody("  read_if_needed_();\n"
		   "\n"
		   "  return " + attr + "[UmlSettings.multiplicity_column(multiplicity)];\n");
  
  op->set_Description(op->description() + " depending on the multiplicity");
  
  // upgrade set
  
  op = settings->get_operation(set);
  op->add_param(0, InputDirection, "multiplicity", "str");
  s = op->cppDecl();
  op->set_CppDecl(s.replace(s.find("${t0}"), 11,
			    "const ${t0} ${p0}, ${t1} ${p1}"));
  s = op->cppDef();
  op->set_CppDef(s.replace(s.find("${t0}"), 11,
			   "const ${t0} ${p0}, ${t1} ${p1}"));
  op->set_CppBody("  UmlCom::send_cmd(" + who + "SettingsCmd, set" + Who + what + "DeclCmd, multiplicity, v);\n"
		  "  if (UmlCom::read_bool()) {\n"
		  "    " + attr + "[UmlSettings::multiplicity_column(multiplicity)] = v;\n"
		  "    return TRUE;\n"
		  "  }\n"
		  "  else\n"
		  "    return FALSE;\n");

  s = op->javaDecl();
  op->set_JavaDef(s.insert(s.find("${)}"), ", ${t1} ${p1}"));
  op->set_JavaBody("  UmlCom.send_cmd(CmdFamily." + who + "SettingsCmd, " + Who + "SettingsCmd._set" + Who + what + "DeclCmd, multiplicity, v);\n"
		   "  UmlCom.check();\n"
		   "\n"
		   "  " + attr + "[UmlSettings.multiplicity_column(multiplicity)] = v;\n");
  
  UmlAttribute * att = settings->get_attribute(attr);
  
  s = att->cppDecl();
  att->set_CppDecl(s.insert(s.find("${name}")+7, "[3/*multiplicity*/]"));
  s = att->javaDecl();
  att->set_JavaDecl(s.insert(s.find("${name}")+7, "[/*multiplicity*/]"));

  
  // upgrade read_
  
  op = settings->get_operation("read_");
  s = op->cppBody();
  op->set_CppBody(s.replace(s.find(attr + " ="), attr.length() + 2,
			    "for (index = 0; index != 3; index += 1)\n"
			    "    " + attr + "[index] ="));
  s = op->javaBody();
  op->set_JavaBody(s.replace(s.find(attr + " ="), attr.length() + 2,
			     "for (index = 0; index != 3; index += 1)\n"
			     "    " + attr + "[index] ="));
}

void add_attribute_multiplicity(UmlClass * settings, QCString who, QCString Who)
{  
  UmlCom::trace("<b>upgrade " + Who + "Settings</b><br>");
  
  settings->get_operation("relationStereotype")->set_Name("relationAttributeStereotype");
  settings->get_operation("set_RelationStereotype")->set_Name("set_RelationAttributeStereotype");
  settings->get_operation("relationUmlStereotype")->set_Name("relationAttributeUmlStereotype");

  add_multiplicity(settings, "_attr_decl", "attributeDecl", "set_AttributeDecl",
		   who, Who, "Attribute");
}

void rename_in(QCString & s, QCString from, QCString to)
{
  int index = 0;
  
  while ((index = s.find(from, index)) != -1) {
    s.replace(index, from.length(), to);
    index += to.length() - from.length();
  }
}

QCString rename_in(QCString s)
{
  rename_in(s, "elationStereotype", "elationAttributeStereotype");
  rename_in(s, "relationUmlStereotype", "relationAttributeUmlStereotype");
  rename_in(s, "_relation_stereotypes", "_relation_attribute_stereotypes");
  rename_in(s, "_rel_stereotype", "_rel_attr_stereotype");
  
  return s;
}

void rename_in(UmlOperation * op)
{
  op->set_CppBody(rename_in(op->cppBody()));
  op->set_JavaBody(rename_in(op->javaBody()));
  op->set_Description(rename_in(op->description()));
}

void add_attribute_multiplicity(UmlClass * umlsettings, UmlClass * cppsettings, 
				UmlClass * javasettings, UmlClass * idlsettings)
{
  UmlCom::trace("<b>Add multiplicity on attribute</b><br>\n");
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  // copy multiplicity() and set_Multiplicity() from UmlBaseRelation
  
  UmlClass * attribute = UmlClass::get("UmlBaseAttribute", 0);
  UmlClass * relation = UmlClass::get("UmlBaseRelation", 0);
  UmlOperation * op1, * op2;
  
  op1 = relation->get_operation("multiplicity");
  op2 = attribute->add_op("multiplicity", PublicVisibility,
			  op1->returnType().explicit_type, FALSE);
  op2->set_CppDecl(op1->cppDecl());
  op2->set_CppDef(op1->cppDef());
  op2->set_CppBody(op1->cppBody());
  op2->set_JavaDef(op1->javaDef());
  op2->set_JavaBody(op1->javaBody());
  op2->set_Description(op1->description());
  op2->moveAfter(attribute->get_operation("set_Type"));
  
  op1 = relation->get_operation("set_Multiplicity");
  op2 = attribute->add_op("set_Multiplicity", PublicVisibility,
			  op1->returnType().explicit_type, TRUE);
  op2->addParameter(0, op1->params()[0]);
  op2->addParameter(1, op1->params()[1]);
  op2->set_CppDecl(op1->cppDecl());
  op2->set_CppDef(op1->cppDef());
  op2->set_CppBody(op1->cppBody());
  op2->set_JavaDef(op1->javaDef());
  op2->set_JavaBody(op1->javaBody());
  op2->set_Description(op1->description());
  op2->moveAfter(attribute->get_operation("multiplicity"));
  
  // copy _multiplicity from UmlBaseRelation
  
  UmlAttribute * att1, * att2;
  
  att1 = relation->get_attribute("_multiplicity");
  att2 = attribute->add_attribute("_multiplicity", PublicVisibility,
				  att1->type().explicit_type, 0, 0);
  att2->set_CppDecl(att1->cppDecl());
  att2->set_JavaDecl(att1->javaDecl());
  att2->moveAfter(attribute->get_attribute("_read_only"));
  
  // modify read_uml_()
  
  QCString s;
  
  op1 = attribute->get_operation("read_uml_");
  s = op1->cppBody();
  op1->set_CppBody(s.insert(s.find("_default_value ="),
			    "_multiplicity = UmlCom::read_string();\n"
			    "  "));
  s = op1->javaBody();
  op1->set_JavaBody(s.insert(s.find("_default_value ="),
			     "_multiplicity = UmlCom.read_string();\n"
			     "  "));
  
  // modify unload

  op1 = attribute->get_operation("unload");
  s = op1->cppBody();
  op1->set_CppBody(s + "  _multiplicity = 0;\n");
  s = op1->javaBody();
  op1->set_JavaBody(s + "  _multiplicity = null;\n");
  
  // upgrade *Settings
  
  add_attribute_multiplicity(cppsettings, "cpp", "Cpp");
  add_attribute_multiplicity(javasettings, "java", "Java");
  add_attribute_multiplicity(idlsettings, "idl", "Idl");
  
  add_multiplicity(idlsettings, "_valuetype_attr_decl", "valuetypeAttributeDecl",
		   "set_ValuetypeAttributeDecl", "idl", "Idl", "ValuetypeAttribute");
  add_multiplicity(idlsettings, "_union_item_decl", "unionItemDecl",
		   "set_UnionItemDecl", "idl", "Idl", "UnionItem");
  add_multiplicity(idlsettings, "_const_decl", "constDecl",
		   "set_ConstDecl", "idl", "Idl", "Const");

  // rename cmds
  
  UmlCom::trace("<b>rename settings cmds</b><br>\n");
  
  UmlClass::get("CppSettingsCmd", 0)->get_attribute("setCppRelationStereotypeCmd")
    ->set_Name("setCppRelationAttributeStereotypeCmd");
  UmlClass::get("JavaSettingsCmd", 0)->get_attribute("setJavaRelationStereotypeCmd")
    ->set_Name("setJavaRelationAttributeStereotypeCmd");
  UmlClass::get("IdlSettingsCmd", 0)->get_attribute("setIdlRelationStereotypeCmd")
    ->set_Name("setIdlRelationAttributeStereotypeCmd");

  //
  
  UmlCom::trace("<b>upgrade UmlSettings</b><br>\n");
  
  umlsettings->get_attribute("_map_relation_stereotypes")
    ->set_Name("_map_relation_attribute_stereotypes");
  umlsettings->get_relation(aDirectionalAssociation, "_relation_stereotypes")
    ->set_RoleName("_relation_attribute_stereotypes");

  umlsettings->get_operation("uml_rel_stereotype")->set_Name("uml_rel_attr_stereotype");
  umlsettings->get_operation("add_rel_stereotype")->set_Name("add_rel_attr_stereotype");    

  //
  
  UmlCom::trace("<b>upgrade body to take into account renamings</b><br>\n");
  
  rename_in(cppsettings->get_operation("set_RelationAttributeStereotype"));
  rename_in(cppsettings->get_operation("relationAttributeStereotype"));
  rename_in(cppsettings->get_operation("relationAttributeUmlStereotype"));
  rename_in(cppsettings->get_operation("read_"));
  
  rename_in(javasettings->get_operation("set_RelationAttributeStereotype"));
  rename_in(javasettings->get_operation("relationAttributeStereotype"));
  rename_in(javasettings->get_operation("relationAttributeUmlStereotype"));
  rename_in(javasettings->get_operation("read_"));
  
  rename_in(idlsettings->get_operation("set_RelationAttributeStereotype"));  
  rename_in(idlsettings->get_operation("relationAttributeStereotype"));
  rename_in(idlsettings->get_operation("relationAttributeUmlStereotype"));
  rename_in(idlsettings->get_operation("read_"));
  
  rename_in(umlsettings->get_operation("read_"));
  rename_in(umlsettings->get_operation("uml_rel_attr_stereotype"));
  rename_in(umlsettings->get_operation("add_rel_attr_stereotype"));
  
  //

  UmlCom::set_user_id(uid);
}

//

void fixe_set_name(UmlClass * baseitem)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlOperation * op = baseitem->get_operation("set_Name");
  
  op->set_CppBody("  UmlCom::send_cmd(_identifier, setNameCmd, s);\n"
		  "  if (UmlCom::read_bool()) {\n"
		  "    _name = s;\n"
		  "    return TRUE;\n"
		  "  }\n"
		  "  else\n"
		  "    return FALSE;\n");
  
  op->set_JavaBody("    UmlCom.send_cmd(identifier_(), OnInstanceCmd.setNameCmd, s);\n"
		   "    UmlCom.check();\n"
		   "    _name = s;\n");
  
  //

  UmlCom::set_user_id(uid);
  
}

//

void add_external(UmlClass * transition)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlAttribute * att = 
    transition->add_attribute("_is_external", PublicVisibility, "bool", 0, 0);
  
  att->moveAfter(transition->get_relation(aDirectionalAssociation, "_target"));
  
  UmlOperation * op;
  UmlOperation * get;
  
  defGetBool(transition, _is_external, isExternal, 0, 0,
	     " return if the transition is internal or external,\n"
	     " only self transitions can't be external");
  op->moveAfter(transition->get_operation("target"));
  get = op;
  
  defSetBool(transition, _is_external, set_IsExternal, setIsCppExternalCmd, 0, 0,
	     " set if the transition is internal or external,\n"
	     " only a self transition may be set internal");
  op->moveAfter(get);
    
  QCString body;
    
  op = transition->get_operation("read_uml_");
  body = op->cppBody();
  body += "  _is_external = !UmlCom::read_bool();\n";
  op->set_CppBody(body);
      
  body = op->javaBody();
  body += "  _is_external = !UmlCom.read_bool();\n";
  op->set_JavaBody(body);
  
  //

  UmlCom::set_user_id(uid);
}

//
//
//

void add_force_body_gen()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  //
  
  UmlCom::trace("<b>add force operation body generation management</b><br>\n");
  UmlClass * base_oper = UmlClass::get("UmlBaseOperation", 0);
  UmlAttribute * att =
    base_oper->add_attribute("_force_body_generation", PrivateVisibility, "bool", 0, 0, " : 1");  
  
  att->moveAfter(base_oper->get_operation("unload"));

  UmlOperation * op;
  
  defGetBool(base_oper, _force_body_generation, isBodyGenerationForced, 0, 0,
	     " indicates if the body is generated even if preserve body is set, returns TRUE if yes");
  
  op->moveAfter(base_oper->get_operation("kind"));
  
  UmlOperation * get = op;
  
  defSetBoolBitField(base_oper, _force_body_generation, set_isBodyGenerationForced,
		     setIsForceBodyGenCmd, 0, 0,
		     " to set if the body is generated even if preserve body is set");
  op->moveAfter(get);
  
  QCString body;
  int index;
    
  op = base_oper->get_operation("read_uml_");
  body = op->cppBody();
  index = body.find("unsigned n;");
  body.insert(index,
	      "_force_body_generation = UmlCom::read_bool();\n"
	      "  \n"
	      "  ");
  op->set_CppBody(body);
      
  body = op->javaBody();
  index = body.find("int i;");
  body.insert(index,
	      "_force_body_generation = UmlCom.read_bool();\n"
	      "  \n"
	      "  ");
  op->set_JavaBody(body);

  //
  
  UmlClass * itcmd = UmlClass::get("OnInstanceCmd", 0);
  
  itcmd->add_enum_item("setIsForceBodyGenCmd");

  //

  UmlCom::set_user_id(uid);
}

//

void fixe_java_alloc()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  //
  
  UmlClass * cl;
  UmlOperation * op;
  QCString s;
  
  cl = UmlClass::get("CppSettings", 0);
  op = cl->get_operation("read_");
  s = op->javaBody();
  
  if (s.find("_attr_decl = new String[3];") == -1) {
    s.insert(s.find("_typedef_decl = UmlCom.read_string();") + 37,
	     "\n  _attr_decl = new String[3];");
    op->set_JavaBody(s);
  }
  
  cl = UmlClass::get("IdlSettings", 0);
  op = cl->get_operation("read_");
  s = op->javaBody();
  
  if (s.find("_attr_decl = new String[3];") == -1)
    s.insert(s.find("_external_class_decl = UmlCom.read_string();") + 44,
	     "\n  _attr_decl = new String[3];");
  if (s.find("_valuetype_attr_decl = new String[3];") == -1)
    s.insert(s.find("_attr_decl = new String[3];") + 27,
	     "\n  _valuetype_attr_decl = new String[3];");
  if (s.find("_union_item_decl = new String[3];") == -1)
    s.insert(s.find("_valuetype_attr_decl = new String[3];") + 37,
	     "\n  _union_item_decl = new String[3];");
  if (s.find("_const_decl = new String[3];") == -1)
    s.insert(s.find("_enum_item_decl = UmlCom.read_string();") + 39,
	     "\n  _const_decl = new String[3];");
  op->set_JavaBody(s);
  
  cl = UmlClass::get("JavaSettings", 0);
  op = cl->get_operation("read_");
  s = op->javaBody();
  
  if (s.find("_attr_decl = new String[3];") == -1) {
    s.insert(s.find("_interface_decl = UmlCom.read_string();") + 39,
	     "\n  _attr_decl = new String[3];");
    op->set_JavaBody(s);
  }

  //

  UmlCom::set_user_id(uid);
}

//

void realization(UmlClass * uml_base_component)
{
  UmlCom::trace("<b>replace 'realized' by 'realizing' in UmlBaseComponent</b><br>\n");
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  //
  
  UmlOperation * op;
  
  op = uml_base_component->get_operation("realizedClasses");
  if(op == 0)
    throw 0;
  
  op->set_Name("realizingClasses");
  op->set_Description(" returns (in Java a copy of) the optional realizing classes");
  op->set_CppBody("  read_if_needed_();\n"
		  "  \n"
		  "  return _realizing;\n");
  op->set_JavaBody("  read_if_needed_();\n"
		   "  \n"
		   "  return (UmlClass[]) _realizing.clone();\n");
  
  //
  
  op = uml_base_component->get_operation("unload");
  if (op != 0) {
    op->set_CppBody("  _realizing.clear();\n"
		    "  _provided.clear();\n"
		    "  _required.clear();\n"
		    "\n"
		    "  UmlBaseItem::unload(rec, del);\n");
    op->set_JavaBody("  _realizing = null;\n"
		     "  _provided = null;\n"
		     "  _required = null;\n"
		     "  super.unload(rec, del);\n");
  }
      
  op = uml_base_component->get_operation("read_uml_");
  op->set_CppBody("  _assoc_diagram = (UmlComponentDiagram *) UmlBaseItem::read_();\n"
		  "  UmlBaseItem::read_uml_();\n"
		  "  \n"
		  "  unsigned n;\n"
		  "  unsigned index;\n"
		  "  \n"
		  "  n = UmlCom::read_unsigned();\n"
		  "  _realizing.resize(n);\n"
		  "    \n"
		  "  for (index = 0; index != n; index += 1)\n"
		  "    _realizing.insert(index, (UmlClass *) UmlBaseItem::read_());\n"
		  "\n"
		  "  n = UmlCom::read_unsigned();\n"
		  "  _provided.resize(n);\n"
		  "    \n"
		  "  for (index = 0; index != n; index += 1)\n"
		  "    _provided.insert(index, (UmlClass *) UmlBaseItem::read_());\n"
		  "\n"
		  "  n = UmlCom::read_unsigned();\n"
		  "  _required.resize(n);\n"
		  "    \n"
		  "  for (index = 0; index != n; index += 1)\n"
		  "    _required.insert(index, (UmlClass *) UmlBaseItem::read_());\n");
  op->set_JavaBody("  _assoc_diagram = (UmlComponentDiagram) UmlBaseItem.read_();\n"
		   "  super.read_uml_();\n"
		   "  \n"
		   "  int n;\n"
		   "  int index;\n"
		   "  \n"
		   "  n = UmlCom.read_unsigned();\n"
		   "  _realizing = new UmlClass[n];\n"
		   "  \n"
		   "  for (index = 0; index != n; index += 1)\n"
		   "    _realizing[index] = (UmlClass) UmlBaseItem.read_();\n"
		   "  \n"
		   "  n = UmlCom.read_unsigned();\n"
		   "  _provided = new UmlClass[n];\n"
		   "  \n"
		   "  for (index = 0; index != n; index += 1)\n"
		   "    _provided[index] = (UmlClass) UmlBaseItem.read_();\n"
		   "  \n"
		   "  n = UmlCom.read_unsigned();\n"
		   "  _required = new UmlClass[n];\n"
		   "  \n"
		   "  for (index = 0; index != n; index += 1)\n"
		   "    _required[index] = (UmlClass) UmlBaseItem.read_();\n");
  
  op = uml_base_component->get_operation("set_AssociatedClasses");
  
  if (op != 0) {
    UmlParameter p = op->params()[0];
    
    p.name = "realizing";
    op->replaceParameter(0, p);
    op->set_CppBody("  UmlCom::send_cmd(_identifier, setAssocClassesCmd,\n"
		    "		   realizing, provided, required);\n"
		    "  if (UmlCom::read_bool()) {\n"
		    "    if (_defined) {\n"
		    "      // tests != to bypass Qt 2.3 bug\n"
		    "      if (&_realizing != &realizing) _realizing = realizing;\n"
		    "      if (&_provided != &provided) _provided = provided;\n"
		    "      if (&_required != &required) _required = required;\n"
		    "    }\n"
		    "    return TRUE;\n"
		    "  }\n"
		    "  else\n"
		    "    return FALSE;\n");
    op->set_JavaBody("  UmlCom.send_cmd(identifier_(), OnInstanceCmd.setAssocClassesCmd,\n"
		     "		  realizing, provided, required);\n"
		     "  UmlCom.check();\n"
		     "  \n"
		     "  _realizing = (UmlClass[]) realizing.clone();\n"
		     "  _provided = (UmlClass[]) provided.clone();\n"
		     "  _required = (UmlClass[]) required.clone();\n");
    op->set_Description(" set the realizing, provided and required classes lists\n"
			"\n"
			" On error return FALSE in C++, produce a RuntimeException in Java");
  }
  
  //
  
  uml_base_component->get_relation("_realized")
    ->set_RoleName("_realizing");

  //

  UmlCom::set_user_id(uid);
}

//

void add_missing_opers()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  //
  
  UmlClass * phpsettings = UmlClass::get("PhpSettings", 0);
  UmlClass * javasettings = UmlClass::get("JavaSettings", 0);
  
  if (phpsettings->get_operation("classUmlStereotype") == 0)
    UmlOperation::java2Php(phpsettings, javasettings, "classUmlStereotype")
      ->moveAfter(phpsettings->get_operation("set_ClassStereotype"));
  
  //
  
  UmlClass * baseitem = UmlClass::get("UmlBaseItem", 0);
  UmlOperation * op;
  UmlOperation * op1;
  
  if (baseitem->get_operation("isMarked") == 0) {
    defGetBool(baseitem, _marked, isMarked, 0, 0,
	       " return TRUE in case the item is marked");
    op1 = op;
    op1->moveAfter(baseitem->get_operation("apply"));
    
    defSetBool(baseitem, _marked, set_isMarked, setMarkedCmd, 0, 0,
	       " to mark/unmark the current item. The project cannot be marked");
    op->moveAfter(op1);
    
    op = baseitem->add_op("markedItems", PublicVisibility, UmlClass::get("UmlItem", 0));
    op->set_isClassMember(TRUE);
    op->set_cpp("const QVector<${type}>", "",
		"  UmlCom::send_cmd(miscGlobalCmd, allMarkedCmd);\n"
		"  \n"
		"  QVector<UmlItem> result;\n"
		"  \n"
		"  UmlCom::read_item_list(result);\n"
		"  return result;\n",
		FALSE, 0, 0);
    op->set_java("${type}[]", "", 
		 "  UmlCom.send_cmd(CmdFamily.miscGlobalCmd, MiscGlobalCmd._allMarkedCmd);\n"
		 "  return UmlCom.read_item_list();\n",
		 FALSE);
    op->moveAfter(baseitem->get_operation("isToolRunning"));
  }
  
  if (baseitem->get_operation("referencedBy") == 0) {
    op = baseitem->add_op("referencedBy", PublicVisibility, UmlClass::get("UmlItem", 0));
    op->set_Description("  Returns the items referencing the current one.\n"
			"  The result may contains UmlAttribute, UmlRelations, UmlNcRelations,\n"
			"  UmlOperation (their bodies are not taken into account) , UmlClass\n"
			"  and UmlComponents.");
    op->set_cpp("const QVector<${type}>", "",
		"  UmlCom::send_cmd(_identifier, referencedByCmd);\n"
		"  \n"
		"  QVector<UmlItem> result;\n"
		"  \n"
		"  UmlCom::read_item_list(result);\n"
		"  return result;\n",
		FALSE, 0, 0);
    op->set_java("${type}[]", "", 
		 "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.childrenCmd);\n"
		 "  return UmlCom.read_item_list();\n",
		 FALSE);
    op->moveAfter(baseitem->get_operation("set_isMarked"));
  }
  
  //
  
  UmlClass * basestate = UmlClass::get("UmlBaseState", 0);
  
  if (basestate->get_operation("set_AssociatedDiagram") == 0) {
    op = basestate->add_op("set_AssociatedDiagram", PublicVisibility, "bool", TRUE);
    op->add_param(0, InputDirection, "d", UmlClass::get("UmlStateDiagram", 0));
    op->set_cpp("${type}", "${t0} * ${p0}", 
		"  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, ((UmlBaseItem *) d)->_identifier);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _assoc_diagram = d;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
    op->set_java("void", "${t0} ${p0}",
		 "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.setAssocDiagramCmd, d.identifier_());\n"
		 "  UmlCom.check();\n"
		 "  _assoc_diagram = d;\n",
		 FALSE);
    op->set_Description(" sets the associated diagram, arg may be null to unset it\n"
			"\n"
			" On error return FALSE in C++, produce a RuntimeException in Java");
    op->moveAfter(basestate->get_operation("associatedDiagram"));
  }
    
  UmlClass * basepack = UmlClass::get("UmlBasePackage", 0);
  
  if ((op = basepack->get_operation("isProjectModified")) == 0) {
    op = basepack->add_op("isProjectModified", PublicVisibility, "bool");
    op->set_isClassMember(TRUE);
    op->set_cpp("${type}", "", 
		"  UmlCom::send_cmd(packageGlobalCmd, isProjectModifiedCmd);\n"
		"  \n"
		"  return UmlCom::read_bool();\n",
		FALSE, 0, 0);
    op->set_java("${type}", "",
		 "  UmlCom.send_cmd(CmdFamily.packageGlobalCmd, PackageGlobalCmd._isProjectModifiedCmd);\n"
		 "  \n"
		 "  return UmlCom.read_bool();\n",
		 FALSE);
    op->set_Description("return TRUE in case something is modified (i.e. the project must be saved)");
    op->moveAfter(basepack->get_operation("getProject"));
  }
  op1 = op;
  
  if ((op = basepack->get_operation("saveProject")) == 0) {
    op = basepack->add_op("saveProject", PublicVisibility, "void");
    op->set_isClassMember(TRUE);
    op->set_cpp("${type}", "", 
		"  UmlCom::send_cmd(packageGlobalCmd, saveProjectCmd);\n",
		FALSE, 0, 0);
    op->set_java("void", "",
		 "  UmlCom.send_cmd(CmdFamily.packageGlobalCmd, PackageGlobalCmd._saveProjectCmd);\n",
		 FALSE);
    op->set_Description("save the project if needed");
    op->moveAfter(op1);
  }
  op1 = op;
  
  if (basepack->get_operation("loadProject") == 0) {
    op = basepack->add_op("loadProject", PublicVisibility, "void");
    op->set_isClassMember(TRUE);
    op->add_param(0, InputDirection, "p", "string");
    op->set_cpp("${type}", "${t0} ${p0}", 
		"  UmlCom::send_cmd(miscGlobalCmd, loadCmd, (const char *) p);\n",
		FALSE, 0, 0);
    op->set_java("void", "${t0} ${p0}",
		 "  UmlCom.send_cmd(CmdFamily.miscGlobalCmd, MiscGlobalCmd._loadCmd, p);\n",
		 FALSE);
    op->set_Description("Does nothing in case an edition is on going under BOUML. Else :\n"
			"close the current project (in case it is not saved the last modifications are lost),\n"
			"load the specified one, and all the communications with the plug-outs including the\n"
			"current one are closed.\n");
    op->moveAfter(op1);
    
    UmlArtifact * art = basepack->associatedArtifact();
    QCString s;
  
    s = art->cppSource();
    if (s.find("#include \"MiscGlobalCmd.h\"") == -1) {
      s.insert(s.find("${namespace_start}"),
	       "#include \"MiscGlobalCmd.h\"\n");
      art->set_CppSource(s);
    }
  }

  //
  
  UmlClass * packageglobalcmd = UmlClass::get("PackageGlobalCmd", 0);
  
  if (packageglobalcmd->get_attribute("isProjectModifiedCmd") == 0) {
    packageglobalcmd->add_enum_item("isProjectModifiedCmd");
    packageglobalcmd->add_enum_item("saveProjectCmd");
  }
  
  //
  
  UmlClass * miscglobalcmd = UmlClass::get("MiscGlobalCmd", 0);
  UmlAttribute * allmarkedcmd = miscglobalcmd->get_attribute("allMarkedCmd");
  
  if (allmarkedcmd == 0) {
    allmarkedcmd = miscglobalcmd->add_enum_item("allMarkedCmd");
    allmarkedcmd->moveAfter(miscglobalcmd->get_attribute("targetCmd"));
  }
  
  UmlAttribute * loadcmd = miscglobalcmd->get_attribute("loadCmd");
  
  if (loadcmd == 0) {
    loadcmd = miscglobalcmd->add_enum_item("loadCmd");
    loadcmd->moveAfter(allmarkedcmd);
  }
      
  UmlAttribute * showtracecmd = miscglobalcmd->get_attribute("showTraceCmd");
  
  if (showtracecmd == 0) {
    showtracecmd = miscglobalcmd->add_enum_item("showTraceCmd");
    showtracecmd->moveAfter(loadcmd);
  }
      
  UmlAttribute * traceautoraisecmd = miscglobalcmd->get_attribute("traceAutoRaiseCmd");
  
  if (traceautoraisecmd == 0) {
    traceautoraisecmd = miscglobalcmd->add_enum_item("traceAutoRaiseCmd");
    traceautoraisecmd->moveAfter(showtracecmd);
  }
  
  //

  UmlCom::set_user_id(uid);
}

//
//
//

void replacefriend()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);  
  UmlClass::get("UmlBaseItem", 0)->parent()->parent()->replace_friend();
  UmlCom::set_user_id(uid);  
}

void UmlPackage::replace_friend() {
  const QVector<UmlItem> ch = children();
  unsigned i;
  
  for (i = 0; i != ch.size(); i += 1)
    ch[i]->replace_friend();
}


//
//
//

void update_pack_global(UmlClass * uml_base_package)
{
  // already root

  UmlCom::trace("<b>update UmlBasePackage : find namespace/module/namespace</b><br>\n");

  UmlOperation * op;
  QCString body;
  int index;
  
  if ((op = uml_base_package->get_operation("findNamespace")) != 0) {
    op->set_Name("findCppNamespace");
    body = op->cppBody();
    if ((index = body.find("findNamespaceCmd")) != -1)
      op->set_CppBody(body.replace(index, 16, "findCppNamespaceCmd"));
    body = op->javaBody();
    if ((index = body.find("findNamespaceCmd")) != -1)
      op->set_JavaBody(body.replace(index, 16, "findCppNamespaceCmd"));
  }
  
  if ((op = uml_base_package->get_operation("findPackage")) != 0) {
    op->set_Name("findJavaPackage");
    body = op->cppBody();
    if ((index = body.find("findPackageCmd")) != -1)
      op->set_CppBody(body.replace(index, 14, "findJavaPackageCmd"));
    body = op->javaBody();
    if ((index = body.find("findPackageCmd")) != -1)
      op->set_JavaBody(body.replace(index, 14, "findJavaPackageCmd"));
  }
  
  if ((op = uml_base_package->get_operation("findModule")) != 0) {
    op->set_Name("findIdlModule");
    body = op->cppBody();
    if ((index = body.find("findModuleCmd")) != -1)
      op->set_CppBody(body.replace(index, 13, "findIdlModuleCmd"));
    body = op->javaBody();
    if ((index = body.find("findModuleCmd")) != -1)
      op->set_JavaBody(body.replace(index, 13, "findIdlModuleCmd"));
  }
  
  UmlClass * packageglobalcmd = UmlClass::get("PackageGlobalCmd", 0);
  UmlAttribute * at;
  
  if ((at = packageglobalcmd->get_attribute("findNamespaceCmd")) != 0)
    at->set_Name("findCppNamespaceCmd");
  
  if ((at = packageglobalcmd->get_attribute("findPackageCmd")) != 0)
    at->set_Name("findJavaPackageCmd");
  
  if ((at = packageglobalcmd->get_attribute("findModuleCmd")) != 0)
    at->set_Name("findIdlModuleCmd");

  // stay root
}

//
//
//

void add_contextual_body_indent()
{
  // already root
  UmlCom::trace("<b>update UmlBaseOperation : contextual body indent</b><br>\n");

  UmlClass * baseoper = UmlClass::get("UmlBaseOperation", 0);
  
  UmlAttribute * at1 =
    baseoper->add_attribute("_cpp_contextual_body_indent", PrivateVisibility, "bool",
			    "WITHCPP", "endif", " : 1");
  at1->moveAfter(baseoper->get_attribute("_idl_get_set_frozen"));
  
  UmlAttribute * at2 =
    baseoper->add_attribute("_java_contextual_body_indent", PrivateVisibility, "bool",
			    "WITHJAVA", "endif", " : 1");
  at2->moveAfter(at1);
  
  UmlAttribute * at3 =
    baseoper->add_attribute("_php_contextual_body_indent", PrivateVisibility, "bool",
			    "WITHPHP", "endif", " : 1");
  at3->moveAfter(at2);
  
  //
  
  UmlOperation * op;
  UmlOperation * pos;
  
  pos = baseoper->get_operation("set_CppGetSetFrozen");
  defGetBool(baseoper, _cpp_contextual_body_indent, cppContextualBodyIndent, "WITHCPP", 0,
	     " indicate if the indent of the C++ body is contextual or absolute");
  op->moveAfter(pos);
  pos = op;
  defSetBoolBitField(baseoper, _cpp_contextual_body_indent,
		     set_CppContextualBodyIndent, setCppContextualBodyIndentCmd, 0, "endif",
		     " set if the indent of the C++ body is contextual or absolute");
  op->moveAfter(pos);

  pos = baseoper->get_operation("set_JavaGetSetFrozen");
  defGetBool(baseoper, _java_contextual_body_indent, javaContextualBodyIndent, "WITHJAVA", 0,
	     " indicate if the indent of the Java body is contextual or absolute");
  op->moveAfter(pos);
  pos = op;
  defSetBoolBitField(baseoper, _java_contextual_body_indent,
		     set_JavaContextualBodyIndent, setJavaContextualBodyIndentCmd, 0, "endif",
		     " set if the indent of the Java body is contextual or absolute");
  op->moveAfter(pos);

  pos = baseoper->get_operation("set_PhpGetSetFrozen");
  defGetBool(baseoper, _idl_get_set_frozen, phpContextualBodyIndent, "WITHIDL", 0,
	     " indicate if the indent of the PHP body is contextual or absolute");
  op->moveAfter(pos);
  pos = op;
  defSetBoolBitField(baseoper, _php_contextual_body_indent,
		     set_PhpContextualBodyIndent, setPhpContextualBodyIndentCmd, 0, "endif",
		     " set if the indent of the PHP body is contextual or absolute");
  op->moveAfter(pos);

  //

  QCString s;
  
  op = baseoper->get_operation("read_cpp_");
  s = op->cppBody() + "  _cpp_contextual_body_indent = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _cpp_contextual_body_indent = UmlCom.read_bool();\n";
  op->set_JavaBody(s);

  op = baseoper->get_operation("read_java_");
  s = op->cppBody() + "  _java_contextual_body_indent = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _java_contextual_body_indent = UmlCom.read_bool();\n";
  op->set_JavaBody(s);

  op = baseoper->get_operation("read_php_");
  s = op->cppBody() + "  _php_contextual_body_indent = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _php_contextual_body_indent = UmlCom.read_bool();\n";
  op->set_JavaBody(s);

  //
  
  UmlClass * itcmd = UmlClass::get("OnInstanceCmd", 0);
  
  itcmd->add_enum_item("setCppContextualBodyIndentCmd");
  itcmd->add_enum_item("setJavaContextualBodyIndentCmd");
  itcmd->add_enum_item("setPhpContextualBodyIndentCmd");
  
  // stay root
}

  
//
//
//

void add_profile()
{
  // already root
  UmlCom::trace("<b>Add profiles</b><br>\n");
  
  UmlClass * packageglobalcmd = UmlClass::get("PackageGlobalCmd", 0);

  packageglobalcmd->add_enum_item("findPythonPackageCmd");
  packageglobalcmd->add_enum_item("updateProfileCmd");
  packageglobalcmd->add_enum_item("findStereotypeCmd");
  
  //
  
  UmlClass * base_package = UmlClass::get("UmlBasePackage", 0);
  UmlOperation * op;
  
  op = base_package->add_op("updateProfiles", PublicVisibility, "void");
  op->set_Description(" Force consistency concerning the profiles and their stereotypes\n");
  op->set_cpp("${type}", "",
	      "  UmlCom::send_cmd(packageGlobalCmd, updateProfileCmd);\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  UmlCom.send_cmd(CmdFamily.packageGlobalCmd, PackageGlobalCmd._updateProfileCmd);\n",
	       FALSE);
  op->set_isClassMember(TRUE);
  op->moveAfter(base_package->get_operation("loadProject"));
  
  UmlClass * base_class = UmlClass::get("UmlBaseClass", 0);
  
  op = base_class->add_op("findStereotype", PublicVisibility, "UmlClass");
  op->add_param(0, InputDirection, "s", "string");
  op->add_param(1, InputDirection, "caseSensitive", "bool");
  op->set_Description(" Return the class supporting the stereotype corresponding to\n"
		       " the first parameter being 'profile_name:stereotype_name', or 0/null");
  op->set_cpp("${type} *", "${t0} ${p0}, ${t1} ${p1}",
	       "  UmlCom::send_cmd(packageGlobalCmd, findStereotypeCmd,\n"
	       "                   (caseSensitive) ? \"y\" : \"n\", (const char *) s);\n"
	       "  return (UmlClass *) UmlBaseItem::read_();\n",
	       FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
		"  UmlCom.send_cmd(CmdFamily.packageGlobalCmd, PackageGlobalCmd._findStereotypeCmd,\n"
		"                   (caseSensitive) ? \"y\" : \"n\", s);\n"
		"  return (UmlClass) UmlBaseItem.read_();\n",
		FALSE);
  op->set_isClassMember(TRUE);
  op->moveAfter(base_class->get_operation("get"));
  
  UmlArtifact * art = base_class->associatedArtifact();
  QCString s = art->cppSource();
  
  art->set_CppSource(s.insert(s.find("${namespace_start}"),
			      "#include \"PackageGlobalCmd.h\"\n"));
  
  //
  
  UmlClass * itcmd = UmlClass::get("OnInstanceCmd", 0);
  static const char * const cmds[] = {
    // used by api 39 but define them to add applyStereotypeCmd
    // before adding python management
    "setPythonDeclCmd",
    "setIsPythonExternalCmd",
    "setPythonSrcCmd",
    "setPythonBodyCmd",
    "setPythonNameSpecCmd",
    "setPythonDirCmd",
    "setPythonFrozenCmd",
    "setIsPython2_2Cmd",
    "setPythonDecoratorsCmd",
    "setPythonPackageCmd",
    "setPythonContextualBodyIndentCmd",
    
    "applyStereotypeCmd"
  };
  
  for (int i = 0; i != sizeof(cmds)/sizeof(cmds[0]); i += 1)
    itcmd->add_enum_item(cmds[i]);
  
  //
  
  UmlClass * base_item = UmlClass::get("UmlBaseItem", 0);
  
  op = base_item->add_op("applyStereotype", PublicVisibility, "bool", TRUE);
  op->set_Description(" If the current stereotype is part of a profile add needed properties.\n"
		      " In all cases remove extra properties whose keys contain two ':'.\n"
		      "\n"
		      " If the element is read-only, return FALSE in C++, produce a RuntimeException in Java");
  op->set_cpp("${type}", "",
	      "  UmlCom::send_cmd(_identifier, applyStereotypeCmd);\n"
	      "  if (UmlCom::read_bool() == 0) return FALSE;\n"
	      "  unload(false, false);\n"
	      "  return TRUE;\n",
	      FALSE, 0, 0);
  op->set_java("void", "",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.applyStereotypeCmd);\n"
	       "  UmlCom.check();\n"
	       "  unload(false, false);\n",
	       FALSE);
  op->moveAfter(base_item->get_operation("set_Stereotype"));
  
  // stay root
}

//
//
//

void modify_texts()
{
  // already root
  
  UmlCom::trace("<b>modify <i>UmlBaseFragmentCompartment::texts()</i> to bypass Microsoft Visual C++ bug</b><br>");
  
  UmlClass * base_frc = UmlClass::get("UmlBaseFragmentCompartment", 0);
  UmlOperation * op = base_frc->get_operation("texts");
  
  op->set_cpp("const QVector<char> &", "", "  return _texts;\n", TRUE, 0, 0);
  
  // stay root
}

//
//
//

void fixe_idlsetting_read()
{
  // already root
  
  UmlCom::trace("<b>fixe <i>IdlSetting::read_()</b><br>");
  
  UmlClass * idlsetting = UmlClass::get("IdlSettings", 0);
  UmlOperation * op = idlsetting->get_operation("read_");
  
  op->set_CppBody("\
  _root = UmlCom::read_string();\n\
  \n\
  unsigned n;\n\
  unsigned index;\n\
  \n\
  n = UmlCom::read_unsigned();\n\
  \n\
  for (index = 0; index != n; index += 1) {\n\
    UmlSettings::_builtins[index].idl = UmlCom::read_string();\n\
  }\n\
  \n\
  n = UmlCom::read_unsigned();\n\
  \n\
  for (index = 0; index != n; index += 1)\n\
    UmlSettings::_relation_attribute_stereotypes[index].idl = UmlCom::read_string();\n\
  \n\
  n = UmlCom::read_unsigned();\n\
  \n\
  for (index = 0; index != n; index += 1)\n\
    UmlSettings::_class_stereotypes[index].idl = UmlCom::read_string();\n\
  \n\
  n = UmlCom::read_unsigned();\n\
  _map_includes.clear();\n\
  if (n > _map_includes.size())\n\
    _map_includes.resize(n);\n\
  \n\
  for (index = 0; index != n; index += 1) {\n\
    QCString t = UmlCom::read_string();\n\
    QCString i = UmlCom::read_string();\n\
    \n\
    _map_includes.insert(t, new QCString(i));\n\
  }\n\
  \n\
  _src_content = UmlCom::read_string();\n\
  _ext = UmlCom::read_string();\n\
\n\
  _interface_decl = UmlCom::read_string();\n\
  _valuetype_decl = UmlCom::read_string();\n\
  _struct_decl = UmlCom::read_string();\n\
  _typedef_decl = UmlCom::read_string();\n\
  _exception_decl = UmlCom::read_string();\n\
  _union_decl = UmlCom::read_string();\n\
  _enum_decl = UmlCom::read_string();\n\
  _external_class_decl = UmlCom::read_string();\n\
  \n\
  _enum_item_decl = UmlCom::read_string();\n\
  for (index = 0; index != 3; index += 1) {\n\
    _attr_decl[index] = UmlCom::read_string();\n\
    _valuetype_attr_decl[index] = UmlCom::read_string();\n\
    _union_item_decl[index] = UmlCom::read_string();\n\
    _const_decl[index] = UmlCom::read_string();\n\
    _rel_decl[index] = UmlCom::read_string();\n\
    _valuetype_rel_decl[index] = UmlCom::read_string();\n\
    _union_rel_decl[index] = UmlCom::read_string();\n\
  }\n\
\n\
  _oper_decl = UmlCom::read_string();\n\
  _get_name = UmlCom::read_string();\n\
  _set_name = UmlCom::read_string();\n\
  _is_set_oneway = UmlCom::read_bool();\n");  
  
  op->set_JavaBody("\
  _root = UmlCom.read_string();\n\
  \n\
  int n;\n\
  int index;\n\
  \n\
  n = UmlCom.read_unsigned();\n\
  \n\
  for (index = 0; index != n; index += 1) {\n\
    UmlSettings._builtins[index].idl = UmlCom.read_string();\n\
  }\n\
  \n\
  n = UmlCom.read_unsigned();\n\
  \n\
  for (index = 0; index != n; index += 1)\n\
    UmlSettings._relation_attribute_stereotypes[index].idl = UmlCom.read_string();\n\
  \n\
  n = UmlCom.read_unsigned();\n\
  \n\
  for (index = 0; index != n; index += 1)\n\
    UmlSettings._class_stereotypes[index].idl = UmlCom.read_string();\n\
  \n\
  n = UmlCom.read_unsigned();\n\
  _map_includes = new Hashtable((n == 0) ? 1 : n);\n\
  \n\
  for (index = 0; index != n; index += 1) {\n\
    String t = UmlCom.read_string();\n\
    String i = UmlCom.read_string();\n\
    \n\
    _map_includes.put(t, i);\n\
  }\n\
  \n\
  _src_content = UmlCom.read_string();\n\
  _ext = UmlCom.read_string();\n\
  \n\
  _interface_decl = UmlCom.read_string();\n\
  _valuetype_decl = UmlCom.read_string();\n\
  _struct_decl = UmlCom.read_string();\n\
  _typedef_decl = UmlCom.read_string();\n\
  _exception_decl = UmlCom.read_string();\n\
  _union_decl = UmlCom.read_string();\n\
  _enum_decl = UmlCom.read_string();\n\
  _external_class_decl = UmlCom.read_string();\n\
  \n\
  _enum_item_decl = UmlCom.read_string();\n\
  _attr_decl = new String[3];\n\
  _valuetype_attr_decl = new String[3];\n\
  _union_item_decl = new String[3];\n\
  _const_decl = new String[3];\n\
  _rel_decl = new String[3];\n\
  _valuetype_rel_decl = new String[3];\n\
  _union_rel_decl = new String[3];\n\
  for (index = 0; index != 3; index += 1) {\n\
    _attr_decl[index] = UmlCom.read_string();\n\
    _valuetype_attr_decl[index] = UmlCom.read_string();\n\
    _union_item_decl[index] = UmlCom.read_string();\n\
    _const_decl[index] = UmlCom.read_string();\n\
    _rel_decl[index] = UmlCom.read_string();\n\
    _valuetype_rel_decl[index] = UmlCom.read_string();\n\
    _union_rel_decl[index] = UmlCom.read_string();\n\
  }\n\
  \n\
  _oper_decl = UmlCom.read_string();\n\
  _get_name = UmlCom.read_string();\n\
  _set_name = UmlCom.read_string();\n\
  _is_set_oneway = UmlCom.read_bool();\n");

  // stay root
}

//

UmlOperation * wrong_umlcom_send_cmd(UmlClass * uml_com)
{
  const QVector<UmlItem> ch = uml_com->children();
  UmlOperation * r = 0;

  for (unsigned i = 0; i != ch.size(); i += 1) {
    if ((ch[i]->kind() == anOperation)  && (ch[i]->name() == "send_cmd")) {
      UmlOperation * op = (UmlOperation *) ch[i];
      const QValueList<UmlParameter> p = op->params();
      
      if ((p.count() == 3) && 
	  ((p[0].type.type == UmlClass::get("CmdFamily", 0)) ||
	   (p[0].type.explicit_type == "CmdFamily")) &&
	  (CppSettings::type(p[2].type.explicit_type) == "char"))
	r = op;
      else if ((p.count() == 4) && 
	       ((p[0].type.type == UmlClass::get("CmdFamily", 0)) ||
		(p[0].type.explicit_type == "CmdFamily")) &&
	       (CppSettings::type(p[2].type.explicit_type) == "int"))
	return 0;
    }
  }
  
  return r;
}
   
void fixe_umlcom_send_cmd(UmlOperation * op0)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>fixe <i>UmlItem::isToolRunning</i> adding <i>UmlCom::send_cmd(,,int,dummy)</i></b><br>");
  
  UmlOperation * op =
    ((UmlClass *) op0->parent())->add_op("send_cmd", PublicVisibility, "void");
  
  op->add_param(0, InputDirection, "f", "CmdFamily");
  op->add_param(1, InputDirection, "cmd", "uint");
  op->add_param(2, InputDirection, "arg", "int");
  op->add_param(3, InputDirection, "dummy", "str");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, const ${t3} ${p3}",
	      "#ifdef TRACE\n"
	      "  cout << \"UmlCom::send_cmd((CmdFamily) \" << f << \", \" << cmd << \", \" << arg << \", dummy)\\n\";\n"
	      "#endif\n"
	      "  \n"
	      "  write_char(f);\n"
	      "  write_char(cmd);\n"
	      "  write_unsigned(arg);\n"
	      "  flush();\n",
	      FALSE, 0, 0);
  
  QCString s = op->cppDef();
  
  s.remove(s.find(" ${p3}"), 6);
  op->set_CppDef(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, ${t3} ${p3}",
	       "  //cout << \"UmlCom.send_cmd((CmdFamily) \" << f << \", \" << cmd << \", \" << arg << \", dummy)\\n\";\n"
	       "  \n"
	       "  write_char((byte) f.value());\n"
	       "  write_char((byte) cmd);\n"
	       "  write_unsigned(arg);\n"
	       "  flush();\n",
	       FALSE);

  op->set_Description("internal, do NOT use it\n");
  op->set_isClassMember(TRUE);
  op->moveAfter(op0);
  
  //
  op = UmlClass::get("UmlBaseItem", 0)->get_operation("isToolRunning");
  
  op->set_CppBody("  UmlCom::send_cmd(miscGlobalCmd, toolRunningCmd, id, \"\");\n"
		  "  return UmlCom::read_bool();\n");
  op->set_JavaBody("  UmlCom.send_cmd(CmdFamily.miscGlobalCmd, MiscGlobalCmd._toolRunningCmd, id, \"\");\n"
		   "  return UmlCom.read_bool();\n");

  UmlCom::set_user_id(uid);
}

//
//
//

void add_javasettings_forcepackageprefixgeneration(UmlClass * javasettings)
{
  // already root
  UmlCom::trace("<b>Upgrade JavaSettings</b><br>");
  UmlAttribute * att;
  UmlOperation * op;
  UmlOperation * op2;
  QCString s;
  
  //
  
  att = javasettings->add_attribute("_is_force_package_gen", PrivateVisibility, "bool",
				    0, 0);
  att->set_isClassMember(TRUE);
  att->moveAfter(javasettings->get_attribute("_is_generate_javadoc_comment"));
  
  op = javasettings->add_op("isForcePackagePrefixGeneration",
			    PublicVisibility, "bool");
  op->set_isClassMember(TRUE);
  op->set_cpp("${type}", "", "\
  read_if_needed_();\n\
\n\
  return _is_force_package_gen;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "", "\
  read_if_needed_();\n\
\n\
  return _is_force_package_gen;\n",
	       FALSE);
  op->moveAfter(javasettings->get_operation("set_IsGenerateJavadocStyleComment"));
  op->set_Description(" return if the package prefix must be\n"
		      " always generated before class's names\n");
  
  op2 = javasettings->add_op("set_IsForcePackagePrefixGeneration",
			     PublicVisibility, "bool", TRUE);
  op2->add_param(0, InputDirection, "v", "bool");   
  op2->set_isClassMember(TRUE);
  op2->set_cpp("${type}", "${t0} ${p0}", "\
  UmlCom::send_cmd(javaSettingsCmd, setJavaForcePackageGenCmd, v);\n\
  if (UmlCom::read_bool()) {\n\
    _is_force_package_gen = v;\n\
    return TRUE;\n\
  }\n\
  else\n\
    return FALSE;\n",
	      FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}", "\
  UmlCom.send_cmd(CmdFamily.javaSettingsCmd, JavaSettingsCmd._setJavaForcePackageGenCmd, (v) ? (byte) 1 : (byte) 0);\n\
  UmlCom.check();\n\
  _is_force_package_gen = v;\n",
	       FALSE);
  op2->moveAfter(op);
  op2->set_Description(" set if the package prefix must be always generated before class's names\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java\n");
  
  op = javasettings->get_operation("read_");
  s = op->cppBody() + "  _is_force_package_gen = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _is_force_package_gen = UmlCom.read_bool();\n";
  op->set_JavaBody(s);
    
  UmlClass * javasettingcmd = UmlClass::get("JavaSettingsCmd", 0);
  
  javasettingcmd->add_enum_item("setJavaForcePackageGenCmd");
  
  // stay root
}

//

void add_cppsettings_builtindir()
{
  // already root
  UmlCom::trace("<b>Upgrade CppSettings</b><br>");
    
  UmlClass * cppsettings = UmlClass::get("CppSettings", 0);
  UmlOperation * op, * op2;
  
  op = cppsettings->get_operation("builtinIn");
  if (op == 0) {
    op = cppsettings->add_op("builtinIn", PublicVisibility, "string");
    op->set_isClassMember(TRUE);
    op->add_param(0, InputDirection, "s", "string");   
    op->set_cpp("${type}", "${t0} ${p0}", "\
  read_if_needed_();\n\
\n\
  UmlBuiltin * b = UmlSettings::_map_builtins.find(s);\n\
\n\
  return (b) ? b->cpp_in : QCString("");\n",
	      FALSE, 0, 0);
    op->set_java("${type}", "${t0} ${p0}", "\
  read_if_needed_();\n\
\n\
  UmlBuiltin b = (UmlBuiltin) _map_builtins.get(s);\n\
    \n\
  return b.cpp_in;\n",
	       FALSE);
    op->set_Description(" returns the default operation 'in' parameter specification\n"
			" in case its type is specified in the first 'Generation\n"
			" settings' tab, else an empty string/null\n");
  }
  op->moveAfter(cppsettings->get_operation("set_EnumReturn"));

  op2 = cppsettings->add_op("set_BuiltinIn", PublicVisibility, "bool", TRUE);
  op2->set_isClassMember(TRUE);
  op2->add_param(0, InputDirection, "type", "string");  
  op2->add_param(1, InputDirection, "form", "string");   
  op2->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}", "\
  UmlCom::send_cmd(cppSettingsCmd, setCppInCmd, type, form);\n\
  if (UmlCom::read_bool()) {\n\
    UmlBuiltin * b = UmlSettings::_map_builtins.find(type);\n\
\n\
    if (b == 0)\n\
      b = UmlSettings::add_type(type);\n\
    b->cpp_in = form;\n\
    \n\
    return TRUE;\n\
  }\n\
  else\n\
    return FALSE;\n",
	       FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}, ${t1} ${p1}", "\
  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppInCmd, type, form);\n\
  UmlCom.check();\n\
    \n\
  UmlBuiltin b = (UmlBuiltin) _map_builtins.get(type);\n\
  \n\
  if (b == null)\n\
    b = UmlSettings.add_type(type);\n\
  b.cpp_in = form;\n",
		FALSE);
  op2->set_Description(" set the default operation 'in' parameter specification\n"
		       " in case its type is specified in the first 'Generation\n"
		       " settings' tab\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java\n");
  op2->moveAfter(op);

  //
  
  op = cppsettings->get_operation("builtinOut");
  if (op == 0) {
    op = cppsettings->add_op("builtinOut", PublicVisibility, "string");
    op->set_isClassMember(TRUE);
    op->add_param(0, InputDirection, "s", "string");   
    op->set_cpp("${type}", "${t0} ${p0}", "\
  read_if_needed_();\n\
\n\
  UmlBuiltin * b = UmlSettings::_map_builtins.find(s);\n\
\n\
  return (b) ? b->cpp_out : QCString("");\n",
	      FALSE, 0, 0);
    op->set_java("${type}", "${t0} ${p0}", "\
  read_if_needed_();\n\
\n\
  UmlBuiltin b = (UmlBuiltin) _map_builtins.get(s);\n\
    \n\
  return b.cpp_out;\n",
	       FALSE);
    op->set_Description(" returns the default operation 'out' parameter specification\n"
			" in case its type is specified in the first 'Generation\n"
			" settings' tab, else an empty string/null\n");
  }
  op->moveAfter(op2);

  op2 = cppsettings->add_op("set_BuiltinOut", PublicVisibility, "bool", TRUE);
  op2->set_isClassMember(TRUE);
  op2->add_param(0, InputDirection, "type", "string");  
  op2->add_param(1, InputDirection, "form", "string");   
  op2->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}", "\
  UmlCom::send_cmd(cppSettingsCmd, setCppOutCmd, type, form);\n\
  if (UmlCom::read_bool()) {\n\
    UmlBuiltin * b = UmlSettings::_map_builtins.find(type);\n\
\n\
    if (b == 0)\n\
      b = UmlSettings::add_type(type);\n\
    b->cpp_out = form;\n\
    \n\
    return TRUE;\n\
  }\n\
  else\n\
    return FALSE;\n",
	       FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}, ${t1} ${p1}", "\
  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppOutCmd, type, form);\n\
  UmlCom.check();\n\
    \n\
  UmlBuiltin b = (UmlBuiltin) _map_builtins.get(type);\n\
  \n\
  if (b == null)\n\
    b = UmlSettings.add_type(type);\n\
  b.cpp_out = form;\n",
		FALSE);
  op2->set_Description(" set the default operation 'out' parameter specification\n"
		       " in case its type is specified in the first 'Generation\n"
		       " settings' tab\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java\n");
  op2->moveAfter(op);
  
  //

  op = cppsettings->get_operation("builtinInOut");
  if (op == 0) {
    op = cppsettings->add_op("builtinInOut", PublicVisibility, "string");
    op->set_isClassMember(TRUE);
    op->add_param(0, InputDirection, "s", "string");   
    op->set_cpp("${type}", "${t0} ${p0}", "\
  read_if_needed_();\n\
\n\
  UmlBuiltin * b = UmlSettings::_map_builtins.find(s);\n\
\n\
  return (b) ? b->cpp_inout : QCString("");\n",
	      FALSE, 0, 0);
    op->set_java("${type}", "${t0} ${p0}", "\
  read_if_needed_();\n\
\n\
  UmlBuiltin b = (UmlBuiltin) _map_builtins.get(s);\n\
    \n\
  return b.cpp_inout;\n",
	       FALSE);
    op->set_Description(" returns the default operation 'inout' parameter specification\n"
			" in case its type is specified in the first 'Generation\n"
			" settings' tab, else an empty string/null\n");
  }
  op->moveAfter(op2);

  op2 = cppsettings->add_op("set_BuiltinInOut", PublicVisibility, "bool", TRUE);
  op2->set_isClassMember(TRUE);
  op2->add_param(0, InputDirection, "type", "string");  
  op2->add_param(1, InputDirection, "form", "string");   
  op2->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}", "\
  UmlCom::send_cmd(cppSettingsCmd, setCppInOutCmd, type, form);\n\
  if (UmlCom::read_bool()) {\n\
    UmlBuiltin * b = UmlSettings::_map_builtins.find(type);\n\
\n\
    if (b == 0)\n\
      b = UmlSettings::add_type(type);\n\
    b->cpp_inout = form;\n\
    \n\
    return TRUE;\n\
  }\n\
  else\n\
    return FALSE;\n",
	       FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}, ${t1} ${p1}", "\
  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppInOutCmd, type, form);\n\
  UmlCom.check();\n\
    \n\
  UmlBuiltin b = (UmlBuiltin) _map_builtins.get(type);\n\
  \n\
  if (b == null)\n\
    b = UmlSettings.add_type(type);\n\
  b.cpp_inout = form;\n",
	       FALSE);
  op2->set_Description(" set the default operation 'inout' parameter specification\n"
		       " in case its type is specified in the first 'Generation\n"
		       " settings' tab\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java\n");
  op2->moveAfter(op);

  //
  
  op = cppsettings->get_operation("builtinReturn");
  if (op == 0) {
    op = cppsettings->add_op("builtinReturn", PublicVisibility, "string");
    op->set_isClassMember(TRUE);
    op->add_param(0, InputDirection, "s", "string");   
    op->set_cpp("${type}", "${t0} ${p0}", "\
  read_if_needed_();\n\
\n\
  UmlBuiltin * b = UmlSettings::_map_builtins.find(s);\n\
\n\
  return (b) ? b->cpp_return : QCString("");\n",
	      FALSE, 0, 0);
    op->set_java("${type}", "${t0} ${p0}", "\
  read_if_needed_();\n\
\n\
  UmlBuiltin b = (UmlBuiltin) _map_builtins.get(s);\n\
    \n\
  return b.cpp_return;\n",
	       FALSE);
    op->set_Description(" returns the default operation 'return' parameter specification\n"
			" in case its type is specified in the first 'Generation\n"
			" settings' tab, else an empty string/null\n");
  }
  op->moveAfter(op2);

  op2 = cppsettings->add_op("set_BuiltinReturn", PublicVisibility, "bool", TRUE);
  op2->set_isClassMember(TRUE);
  op2->add_param(0, InputDirection, "type", "string");  
  op2->add_param(1, InputDirection, "form", "string");   
  op2->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}", "\
  UmlCom::send_cmd(cppSettingsCmd, setCppReturnCmd, type, form);\n\
  if (UmlCom::read_bool()) {\n\
    UmlBuiltin * b = UmlSettings::_map_builtins.find(type);\n\
\n\
    if (b == 0)\n\
      b = UmlSettings::add_type(type);\n\
    b->cpp_return = form;\n\
    \n\
    return TRUE;\n\
  }\n\
  else\n\
    return FALSE;\n",
	       FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}, ${t1} ${p1}", "\
  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppReturnCmd, type, form);\n\
  UmlCom.check();\n\
    \n\
  UmlBuiltin b = (UmlBuiltin) _map_builtins.get(type);\n\
  \n\
  if (b == null)\n\
    b = UmlSettings.add_type(type);\n\
  b.cpp_return = form;\n",
		FALSE);
  op2->set_Description(" set the default operation 'return' parameter specification\n"
		       " in case its type is specified in the first 'Generation\n"
		       " settings' tab\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java\n");
  op2->moveAfter(op);

  //
 
  QCString s;
  
  op = cppsettings->get_operation("set_In");
  s = op->cppBody();
  op->set_CppBody(s.insert(s.find(")"), ", \"\""));
  s = op->javaBody();
  op->set_JavaBody(s.insert(s.find(")"), ", \"\""));
  
  op = cppsettings->get_operation("set_Out");
  s = op->cppBody();
  op->set_CppBody(s.insert(s.find(")"), ", \"\""));
  s = op->javaBody();
  op->set_JavaBody(s.insert(s.find(")"), ", \"\""));
  
  op = cppsettings->get_operation("set_Inout");
  s = op->cppBody();
  op->set_CppBody(s.insert(s.find(")"), ", \"\""));
  s = op->javaBody();
  op->set_JavaBody(s.insert(s.find(")"), ", \"\""));
  
  op = cppsettings->get_operation("set_Return");
  s = op->cppBody();
  op->set_CppBody(s.insert(s.find(")"), ", \"\""));
  s = op->javaBody();
  op->set_JavaBody(s.insert(s.find(")"), ", \"\""));
  
  //stay root
}

//
  
void update_uml_com2()
{
  // already root

  UmlClass * uml_com = UmlClass::get("UmlCom", 0);
  UmlOperation * prev = uml_com->get_operation("send_cmd");
  UmlOperation * op =
    uml_com->add_op("send_cmd", PublicVisibility, "void", FALSE);
  
  op->add_param(0, InputDirection, "f", "CmdFamily");
  op->add_param(1, InputDirection, "cmd", "uint");
  op->add_param(2, InputDirection, "s", "str");
  op->add_param(3, InputDirection, "b", "bool");

  op->set_Description("internal, do NOT use it\n");
  op->set_isClassMember(TRUE);
  op->moveAfter(prev);
  
  op->set_cpp("${type}", 
	      "${t0} ${p0}, ${t1} ${p1}, const ${t2} ${p2}, ${t3} ${p3}",
	      "\
#ifdef TRACE\n\
  cout << \"UmlCom::send_cmd((CmdFamily) \" << f << \", \" << cmd << \", \" << ((s) ? s : \"\") << b << \")\\n\";\n\
#endif\n\
  \n\
  write_char(f);\n\
  write_char(cmd);\n\
  write_string(s);\n\
  write_bool(b);\n\
  flush();\n",
	      FALSE, 0, 0);

  op->set_java("${type}",
	       "${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, ${t3} ${p3}",
	       "\
  write_char((byte) f.value());\n\
  write_char((byte) cmd);\n\
  write_string(s);\n\
  write_bool(b);\n\
  flush();\n",
	       FALSE);
  
  // stay root
}

//
//
//

void oneBit(UmlClass * cl, QCString attname, const char * opname,
	    QCString cmd, const char * end_if)
{
  UmlAttribute * att = cl->get_attribute(attname);
  QCString s;
  int index;
  
  s = att->cppDecl();
  index = s.find("${type} ${name}");
  if (index != -1) {
    s.insert(index + 15, " : 1");
    att->set_CppDecl(s);
  }

  UmlOperation * op = cl->get_operation(opname);
  QCString v = op->params()[0].name;
  
  s = "  UmlCom::send_cmd(_identifier, " + cmd + ", (char) " + v + ");\n"
      "  if (UmlCom::read_bool()) {\n"
      "    " + attname + " = " + v + ";\n"
      "    return TRUE;\n"
      "  }\n"
      "  else\n"
      "    return FALSE;\n";
  
  op->set_cpp("${type}", "${t0} ${p0}", s, FALSE, 0, end_if);
}

void add_property_modifiers(UmlClass * cl)
{
  UmlAttribute * att, * att1;
  
  att = cl->add_attribute("_derived", PrivateVisibility, "bool", 0, 0, " : 1");
  att->moveAfter(cl->get_attribute("_read_only"));
  
  att1 = cl->add_attribute("_derived_union", PrivateVisibility, "bool", 0, 0, " : 1");
  att1->moveAfter(att);
  
  att = cl->add_attribute("_ordered", PrivateVisibility, "bool", 0, 0, " : 1");
  att->moveAfter(att1);
  
  att1 = cl->add_attribute("_unique", PrivateVisibility, "bool", 0, 0, " : 1");
  att1->moveAfter(att);
  
  //
  
  UmlOperation * op, * op1;
  
  defGetBool(cl, _derived, isDerived, 0, 0,
	     " return the property 'derived'");
  op->moveAfter(cl->get_operation("set_isReadOnly"));
  op1 = op;
  
  defGetBool(cl, _derived_union, isDerivedUnion, 0, 0,
	     " return the property 'derived union'");
  op->moveAfter(op1);
  
  op1 = cl->add_op("set_isDerived", PublicVisibility, "bool", TRUE);
  op1->set_Description(" Set the properties 'derived' and 'union'\n"
		       "\n"
		       " On error return FALSE in C++, produce a RuntimeException in Java");
  op1->add_param(0, InputDirection, "is_derived", "bool");
  op1->add_param(1, InputDirection, "is_union", "bool");
  op1->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  UmlCom::send_cmd(_identifier, setDerivedCmd,\n"
	       "                   (char) (((is_derived) ? 1 : 0) + ((is_union) ? 2 : 0)));\n"
	       "  if (UmlCom::read_bool()) {\n"
	       "    _derived = is_derived;\n"
	       "    _derived_union = is_union;\n"
	       "    return TRUE;\n"
	       "  }\n"
	       "  else\n"
	       "    return FALSE;\n",
	       FALSE, 0, 0);
  op1->set_java("void", "${t0} ${p0}, ${t1} ${p1}",
		"  UmlCom.send_cmd(identifier_(), OnInstanceCmd.setDerivedCmd,\n"
		"                  (byte) (((is_derived) ? 1 : 0) + ((is_union) ? 2 : 0)));\n"
		"  UmlCom.check();\n"
		"\n"
		"  _derived = is_derived;\n"
		"  _derived_union = is_union;\n",
		FALSE);
  op1->moveAfter(op);

  defGetBool(cl, _ordered, isOrdered, 0, 0,
	     " return the property 'ordered'");
  op->moveAfter(op1);
  op1 = op;
  defSetBoolBitField(cl, _ordered, set_isOrdered, setOrderingCmd, 0, 0,
		     " set the property 'ordered'");
  op->moveAfter(op1);
  op1 = op;
  
  defGetBool(cl, _unique, isUnique, 0, 0,
	     " return the property 'unique'");
  op->moveAfter(op1);
  op1 = op;
  defSetBoolBitField(cl, _unique, set_isUnique, setUniqueCmd, 0, 0,
		     " set the property 'unique'");
  op->moveAfter(op1);
  
  //
  
  op = cl->get_operation("read_uml_");
  
  QCString s;
  
  s = op->cppBody();
  s.insert(s.find("_get_oper = "),
	   "_derived = UmlCom::read_bool();\n"
	   "  _derived_union = UmlCom::read_bool();\n"
	   "  _ordered = UmlCom::read_bool();\n"
	   "  _unique = UmlCom::read_bool();\n  ");
  op->set_CppBody(s);
  
  s = op->javaBody();
  s.insert(s.find("_get_oper = "),
	   "_derived = UmlCom.read_bool();\n"
	   "  _derived_union = UmlCom.read_bool();\n"
	   "  _ordered = UmlCom.read_bool();\n"
	   "  _unique = UmlCom.read_bool();\n  ");
  op->set_JavaBody(s);
}

void add_property_modifiers()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  //
  
  UmlCom::trace("<b>Add property modifiers</b><br>");
    
  UmlClass * baseatt = UmlClass::get("UmlBaseAttribute", 0);
  UmlClass * baserel = UmlClass::get("UmlBaseRelation", 0);

  UmlAttribute * att = baseatt->get_attribute("_multiplicity");
  
  att->set_Visibility(PrivateVisibility);
  att->moveAfter(baseatt->get_attribute("_java_transient"));
  
  oneBit(baseatt, "_read_only", "set_isReadOnly", "setIsReadOnlyCmd", 0);
  oneBit(baseatt, "_cpp_mutable", "set_isCppMutable", "setIsCppMutableCmd", "endif");
  oneBit(baseatt, "_java_transient", "set_isJavaTransient", "setIsJavaTransientCmd", "endif");
  
  oneBit(baserel, "_read_only", "set_isReadOnly", "setIsReadOnlyCmd", 0);
  oneBit(baserel, "_cpp_mutable", "set_isCppMutable", "setIsCppMutableCmd", "endif");
  
  baserel->get_relation(aDirectionalAggregationByValue, "_rel_kind")
    ->moveAfter(baserel->get_operation("unload"));
  
  add_property_modifiers(baseatt);
  add_property_modifiers(baserel);
  
  //

  UmlClass::get("OnInstanceCmd", 0)->add_enum_item("setDerivedCmd");
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_read(UmlClass * cl, const char * opname)
{
  UmlOperation * op = cl->get_operation(opname);
  QCString s;
  
  s = "  read_if_needed_();\n" + op->cppBody();
  op->set_CppBody(s);
  
  s = "  read_if_needed_();\n" + op->javaBody();
  op->set_JavaBody(s);
}

void fixe_settings(UmlClass * umlsettings, UmlClass * cppsettings, UmlClass * javasettings) 
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  //
  
  UmlCom::trace("<b>fixe Uml/Cpp/Java/Idl/PhpSettings</b><br>");
    
  UmlClass * idlsettings = UmlClass::get("IdlSettings", 0);
  UmlClass * phpsettings = UmlClass::get("PhpSettings", 0);
  
  umlsettings->get_operation("add_type")->set_CppBody("\
  unsigned n = _map_builtins.count();\n\
  unsigned index;\n\
\n\
  UmlBuiltin * builtins = new UmlBuiltin[n + 1];\n\
\n\
  if (n/2 > _map_builtins.size())\n\
    _map_builtins.resize(_map_builtins.size() * 2 - 1);\n\
  \n\
  for (index = 0; index != n; index += 1) {\n\
    builtins[index] = _builtins[index];\n\
    _map_builtins.replace(builtins[index].uml, &builtins[index]);\n\
  }\n\
    \n\
  builtins[index].uml = s;\n\
#ifdef WITHCPP\n\
  builtins[index].cpp = s;\n\
  builtins[index].cpp_in = \"const ${type}\";\n\
  builtins[index].cpp_out = \"${type} &\";\n\
  builtins[index].cpp_inout = \"${type} &\";\n\
#endif\n\
#ifdef WITHJAVA\n\
  builtins[index].java = s;\n\
#endif\n\
#ifdef WITHIDL\n\
  builtins[index].idl = s;\n\
#endif\n\
\n\
  _map_builtins.insert(s, &_builtins[index]);\n\
\n\
  delete [] _builtins;\n\
  _builtins = builtins;\n\
  \n\
  return &_builtins[index];\n");
  
  umlsettings->get_operation("add_rel_attr_stereotype")->set_CppBody("\
  unsigned n = _map_relation_attribute_stereotypes.count();\n\
  unsigned index;\n\
\n\
  UmlStereotype * relation_attribute_stereotypes = new UmlStereotype[n + 1];\n\
\n\
  if (n/2 > _map_relation_attribute_stereotypes.size())\n\
    _map_relation_attribute_stereotypes.resize(_map_relation_attribute_stereotypes.size() * 2 - 1);\n\
  \n\
  for (index = 0; index != n; index += 1) {\n\
    relation_attribute_stereotypes[index] = _relation_attribute_stereotypes[index];\n\
    _map_relation_attribute_stereotypes.replace(relation_attribute_stereotypes[index].uml,\n\
						&relation_attribute_stereotypes[index]);\n\
  }\n\
    \n\
  relation_attribute_stereotypes[index].uml = s;\n\
#ifdef WITHCPP\n\
  relation_attribute_stereotypes[index].cpp = s;\n\
#endif\n\
#ifdef WITHJAVA\n\
  relation_attribute_stereotypes[index].java = s;\n\
#endif\n\
#ifdef WITGIDL\n\
  relation_attribute_stereotypes[index].idl = s;\n\
#endif\n\
\n\
  _map_relation_attribute_stereotypes.insert(s, &_relation_attribute_stereotypes[index]);\n\
\n\
  delete [] _relation_attribute_stereotypes;\n\
  _relation_attribute_stereotypes = relation_attribute_stereotypes;\n\
\n\
  return &_relation_attribute_stereotypes[index];\n");
  
  umlsettings->get_operation("add_class_stereotype")->set_CppBody("\
  unsigned n = _map_class_stereotypes.count();\n\
  unsigned index;\n\
\n\
  UmlStereotype * class_stereotypes = new UmlStereotype[n + 1];\n\
\n\
  if (n/2 > _map_class_stereotypes.size())\n\
    _map_class_stereotypes.resize(_map_class_stereotypes.size() * 2 - 1);\n\
  \n\
  for (index = 0; index != n; index += 1) {\n\
    class_stereotypes[index] = _class_stereotypes[index];\n\
    _map_class_stereotypes.replace(class_stereotypes[index].uml, &class_stereotypes[index]);\n\
  }\n\
    \n\
  class_stereotypes[index].uml = s;\n\
#ifdef WITHCPP\n\
  class_stereotypes[index].cpp = s;\n\
#endif\n\
#ifdef WITHJAVA\n\
  class_stereotypes[index].java = s;\n\
#endif\n\
#ifdef WITHIDL\n\
  class_stereotypes[index].idl = s;\n\
#endif\n\
\n\
  _map_class_stereotypes.insert(s, &_class_stereotypes[index]);\n\
\n\
  delete [] _class_stereotypes;\n\
  _class_stereotypes = class_stereotypes;\n\
\n\
  return &_class_stereotypes[index];\n");
  
  //
  
  add_read(cppsettings, "set_Type");
  add_read(cppsettings, "set_RelationAttributeStereotype");
  add_read(cppsettings, "set_ClassStereotype");
  add_read(cppsettings, "set_Include");
  add_read(cppsettings, "set_BuiltinIn");
  add_read(cppsettings, "set_BuiltinOut");
  add_read(cppsettings, "set_BuiltinInOut");
  add_read(cppsettings, "set_BuiltinReturn");
  add_read(cppsettings, "set_AttributeDecl");
  add_read(cppsettings, "set_RelationDecl");
  
  add_read(idlsettings, "set_Type");
  add_read(idlsettings, "set_RelationAttributeStereotype");
  add_read(idlsettings, "set_ClassStereotype");
  add_read(idlsettings, "set_Include");
  add_read(idlsettings, "set_AttributeDecl");
  add_read(idlsettings, "set_ValuetypeAttributeDecl");
  add_read(idlsettings, "set_UnionItemDecl");
  add_read(idlsettings, "set_ConstDecl");
  add_read(idlsettings, "set_RelationDecl");
  add_read(idlsettings, "set_ValuetypeRelationDecl");
  add_read(idlsettings, "set_UnionRelationDecl");
  
  add_read(javasettings, "set_Type");
  add_read(javasettings, "set_RelationAttributeStereotype");
  add_read(javasettings, "set_ClassStereotype");
  add_read(javasettings, "set_Import");
  add_read(javasettings, "set_AttributeDecl");
  add_read(javasettings, "set_RelationDecl");
  
  
  add_read(phpsettings, "set_ClassStereotype");
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void fixe_set_associateddiagram(UmlItem * v)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  //
  
  UmlCom::trace("<b>fixe operations <i>set_AssociatedDiagram</i></b><br>");
  
  const QVector<UmlItem> ch1 = v->children();

  for (unsigned i1 = 0; i1 != ch1.size(); i1 += 1) {
    if (ch1[i1]->kind() == aClass) {
      const QVector<UmlItem> ch2 = ch1[i1]->children();
      
      for (unsigned i2 = 0; i2 != ch2.size(); i2 += 1) {
	if ((ch2[i2]->kind() == anOperation) &&
	    (ch2[i2]->name() == "set_AssociatedDiagram")) {
	  UmlOperation * op = (UmlOperation *) ch2[i2];
	  QCString b;
	  
	  b = op->cppBody();
	  if (b.find("(d == 0) ? (void *) 0 : ") == -1) {
	    b.insert(b.find("((UmlBaseItem *) d)"), "(d == 0) ? (void *) 0 : ");
	    op->set_CppBody(b);
	  }
	  
	  b = op->javaBody();
	  if (b.find("(d == null) ? (long) 0 : ") == -1) {
	    b.insert(b.find("d.identifier_()"), "(d == null) ? (long) 0 : ");
	    op->set_JavaBody(b);
	  }
	}
      }
    }
  }
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void bypass_qvector_bug()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  //
  
  UmlCom::trace("<b>bypass Qt 2.3 bug concerning QVector assignment</b><br>");
  UmlOperation * op;
  
  op = UmlClass::get("UmlBaseComponent", 0)
    ->get_operation("set_AssociatedClasses");
  
  if (op != 0)
    op->set_CppBody("  UmlCom::send_cmd(_identifier, setAssocClassesCmd,\n"
		    "		   realizing, provided, required);\n"
		    "  if (UmlCom::read_bool()) {\n"
		    "    if (_defined) {\n"
		    "      // tests != to bypass Qt 2.3 bug\n"
		    "      if (&_realizing != &realizing) _realizing = realizing;\n"
		    "      if (&_provided != &provided) _provided = provided;\n"
		    "      if (&_required != &required) _required = required;\n"
		    "    }\n"
		    "    return TRUE;\n"
		    "  }\n"
		    "  else\n"
		    "    return FALSE;\n");
  
  UmlClass::get("UmlBaseArtifact", 0)
    ->get_operation("set_AssociatedClasses")
      ->set_CppBody("  UmlCom::send_cmd(_identifier, setAssocClassesCmd, (const QVector<UmlItem> &) l);\n"
		    "  if (UmlCom::read_bool()) {\n"
		  "      // tests != to bypass Qt 2.3 bug\n"
		    "    if (_defined && (&_assoc_classes != &l))\n"
		    "      _assoc_classes = l;\n"
		    "    return TRUE;\n"
		    "  }\n"
		    "  else\n"
		    "    return FALSE;\n");
  
  UmlClass::get("UmlBaseParameterSet", 0)
    ->get_operation("set_Pins")
      ->set_CppBody("  UmlCom::send_cmd(_identifier, replaceParameterCmd, (const QVector<UmlItem> &) v);\n"
		    "  if (UmlCom::read_bool()) {\n"
		  "      // tests != to bypass Qt 2.3 bug\n"
		    "    if (_defined && (&_pins != &v)) _pins = v;\n"
		    "    return TRUE;\n"
		    "  }\n"
		    "  else\n"
		    "    return FALSE;\n");
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_methods(UmlClass * opercl, UmlClass * uml_item)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  //
    
  UmlOperation * op = 
    opercl->add_op("methods", PublicVisibility, uml_item);
  
  op->set_isCppConst(TRUE);
  op->set_cpp("const QVector<${type}>", "",
	      "  QVector<UmlItem> l;\n"
	      "\n"
	      "  UmlCom::send_cmd(_identifier, sideCmd);\n"
	      "  UmlCom::read_item_list(l);\n"
	      "  return l;\n"
	      , FALSE, 0, 0);
  op->set_java("${type}[]", "",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.sideCmd);\n"
	       "  return UmlCom.read_item_list();\n"
	       , FALSE);
  op->set_Description(" return the behaviors (state and activities) implementing the operation");
  op->moveAfter(opercl->get_operation("replaceException"));
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_import_project(UmlClass * uml_base_package)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  //
    
  UmlOperation * op = 
    uml_base_package->add_op("importProject", PublicVisibility, 
			     UmlClass::get("UmlPackage", 0));
  
  op->add_param(0, InputDirection, "fn", "string");
  op->set_Description(" Import a project in the current package\n"
		      " fn is the pathname of the .prj file of the project to import\n"
		      " or an empty string (the project will be set through a dialog)\n"
		      " Return the new UmlPackage containing the imported project, or\n"
		      " 0/null on error");
  op->set_cpp("${type} *", "const ${t0} & ${p0}",
	      "  UmlCom::send_cmd(_identifier, importProjectCmd, fn);\n"
	      "\n"
	      "  return (UmlPackage *) UmlBaseItem::read_();\n"
	      , FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.importProjectCmd, fn);\n"
	       "  return (UmlPackage) UmlBaseItem.read_();\n"
	       , FALSE);
  op->moveAfter(uml_base_package->get_operation("set_AssociatedDiagram"));
  
  //

  UmlClass::get("OnInstanceCmd", 0)->add_enum_item("importProjectCmd");
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_is_active(UmlClass * uml_base_class)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  //
    
  UmlOperation * op;
  UmlOperation * pos;
  QCString body;
  
  uml_base_class->add_attribute("_active", PrivateVisibility, "bool", 0, 0, " : 1")
    ->moveAfter(uml_base_class->get_attribute("_abstract"));
  
  defGetBool(uml_base_class, _active, isActive, 0, 0,
	     " indicate if the class is active");
  op->moveAfter(uml_base_class->get_operation("set_isAbstract"));
  pos = op;
  
  defSetBoolBitField(uml_base_class, _active, set_isActive, setActiveCmd, 0, 0,
		     " set if the class is active");
  op->moveAfter(pos);
  
  op = uml_base_class->get_operation("read_uml_");
  body = op->cppBody() + "  _active = UmlCom::read_bool();\n";
  op->set_CppBody(body);
  body = op->javaBody() + "  _active = UmlCom.read_bool();\n";
  op->set_JavaBody(body);
  
  //
  
  UmlClass * uml_base_state = UmlClass::get("UmlBaseState", 0);
  UmlAttribute * at;
  
  at = uml_base_state->add_attribute("_active", PrivateVisibility, "bool", 0, 0);
  at->moveAfter(uml_base_state->get_relation("_java"));
  uml_base_state->get_relation("_specification")->moveAfter(at);
  
  defGetBool(uml_base_state, _active, isActive, 0, 0,
	     " indicate if the class is active");
  op->moveAfter(uml_base_state->get_operation("set_JavaDoActivity"));
  pos = op;
  
  defSetBoolBitField(uml_base_state, _active, set_isActive, setActiveCmd, 0, 0,
		     " set if the class is active");
  op->moveAfter(pos);
  
  op = uml_base_state->get_operation("read_uml_");
  body = op->cppBody() + "  _active = UmlCom::read_bool();\n";
  op->set_CppBody(body);
  body = op->javaBody() + "  _active = UmlCom.read_bool();\n";
  op->set_JavaBody(body);
  
  //
  
  UmlClass * uml_base_activity = UmlClass::get("UmlBaseActivity", 0);
  
  at = uml_base_activity->add_attribute("_active", PrivateVisibility, "bool", 0, 0);
  at->moveAfter(uml_base_activity->get_attribute("_single_execution"));
  uml_base_activity->get_relation("_specification")->moveAfter(at);
  
  defGetBool(uml_base_activity, _active, isActive, 0, 0,
	     " indicate if the class is active");
  op->moveAfter(uml_base_activity->get_operation("set_isSingleExecution"));
  pos = op;
  
  defSetBool(uml_base_activity, _active, set_isActive, setActiveCmd, 0, 0,
	     " set if the class is active");
  op->moveAfter(pos);
  
  op = uml_base_activity->get_operation("read_uml_");
  body = op->cppBody() + "  _active = UmlCom::read_bool();\n";
  op->set_CppBody(body);
  body = op->javaBody() + "  _active = UmlCom.read_bool();\n";
  op->set_JavaBody(body);
  
  //

  UmlClass::get("OnInstanceCmd", 0)->add_enum_item("setActiveCmd");
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_cpp_inline_oper_force_incl_in_h(UmlClass * cppsetting)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>update CppSettings</b><br>\n");

  UmlAttribute * att = 
    cppsetting->add_attribute("_is_inline_force_header_in_h", PrivateVisibility, "bool", 0, 0);

  att->set_isClassMember(TRUE);
  att->moveAfter(cppsetting->get_attribute("_is_generate_javadoc_comment"));

  //

  UmlOperation * op = cppsetting->get_operation("read_");
  QCString s;
  
  s = op->cppBody() + "  _is_inline_force_header_in_h = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _is_inline_force_header_in_h = UmlCom.read_bool();\n";
  op->set_JavaBody(s);
  
  // get

  UmlOperation * op2 = cppsetting->get_operation("set_IsForceNamespacePrefixGeneration");

  op = cppsetting->add_op("isInlineOperationForceIncludesInHeader", PublicVisibility, "bool");
  op->set_isClassMember(TRUE);
  op->set_Description(" return if the fact an operation is inline force the header of the\n"
		      " types referenced in the profile to be included in the header\n");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _is_inline_force_header_in_h;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _is_inline_force_header_in_h;\n",
	       FALSE);
  op->moveAfter(cppsetting->get_operation("set_IsForceNamespacePrefixGeneration"));
  
  // set
  
  UmlOperation * op3 = cppsetting->add_op("set_IsInlineOperationForceIncludesInHeader", PublicVisibility, "bool", TRUE);
  
  op3->set_isClassMember(TRUE);
  op3->add_param(0, InputDirection, "v", "bool");
  op3->set_Description(" set if the fact an operation is inline force the header of the\n"
		      " types referenced in the profile to be included in the header\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  
  op3->set_CppDecl(op2->cppDecl());
  op3->set_CppDef(op2->cppDef());
  op3->set_CppBody("\
  UmlCom::send_cmd(cppSettingsCmd, setCppInlineOperForceInclInHeaderCmd, v);\n\
  if (UmlCom::read_bool()) {\n\
    _is_inline_force_header_in_h = v;\n\
    return TRUE;\n\
  }\n\
  else\n\
    return FALSE;\n");

  op3->set_JavaDecl(op2->javaDecl());
  op3->set_JavaBody("\
  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppInlineOperForceInclInHeaderCmd,\n\
		  (v) ? (byte) 1 : (byte) 0);\n\
  UmlCom.check();\n\
  \n\
  _is_inline_force_header_in_h = v;\n");
  
  op3->moveAfter(op);

  //

  UmlClass::get("CppSettingsCmd", 0)->add_enum_item("setCppInlineOperForceInclInHeaderCmd");

  //

  UmlCom::set_user_id(uid);
}



//
//
//

void add_constraint2(UmlClass * basecl, const char *afterop, const char * afterat)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  QCString s = "<b>Add constraint on " + basecl->name() + "</b><br>\n";
  
  UmlCom::trace(s);
  
  //
  
  basecl->add_attribute("_constraint", PrivateVisibility, "string",
			0, 0)
    ->moveAfter(basecl->get_attribute(afterat));
  
  UmlOperation * op;
  UmlOperation * op1;
  
  defGet(basecl, _constraint, constraint, "string", 0, 0,
         "constraint");
  op->moveAfter(basecl->get_operation(afterop));
  op1 = op;
  defSet(basecl, _constraint, set_Constraint, "str", setConstraintCmd, 0, 0,
         "constraint");
  op->moveAfter(op1);
  
  op = basecl->get_operation("read_uml_");
  s = op->cppBody() + "  _constraint = UmlCom::read_string();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _constraint = UmlCom.read_string();\n";
  op->set_JavaBody(s);
  
  op = basecl->get_operation("unload");
  s = op->cppBody() + "  _constraint = 0;\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _constraint = null;\n";
  op->set_JavaBody(s);
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_rev_filter()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>Add reverse/roundtrip dir/file filtering</b><br>\n");
  
  //
  
  const char * const langs[] = { "Cpp", "Java", "Php" };
  unsigned i;
  
  for (i = 0; i != sizeof(langs) / sizeof(langs[0]); i += 1) {
    QCString pfix = langs[i];
    UmlClass * settings = UmlClass::get(pfix + "Settings", 0);
    UmlClass * settingscmd = UmlClass::get(settings->name() + "Cmd", 0);
    UmlAttribute * at2 = settings->get_attribute((langs[i][0] == 'C') ? "_src_ext" : "_ext");
    UmlOperation * op2 = settings->get_operation("set_SourceExtension");
    QCString what = "Dir";
    QCString cmd, rg, cs, s;
    UmlAttribute * at;
    UmlOperation * op;
    
    for (;;) {
      cmd = "set" + pfix + what + "RevFilterCmd";
      settingscmd->add_enum_item(cmd);
  
      rg = "_" + what.lower() + "_regexp";
      at = settings->add_attribute(rg, PrivateVisibility, "string", 0, 0);
      at->set_isClassMember(TRUE);
      at->moveAfter(at2);
      at2 = at;
  
      cs = rg + "_case_sensitive";
      at = settings->add_attribute(cs, PrivateVisibility, "bool", 0, 0);
      at->set_isClassMember(TRUE);
      at->moveAfter(at2);
      at2 = at;
      
      s = "reverseRoundtrip" + what + "RegExp";
      op = settings->add_op(s, PublicVisibility, "string");
      op->moveAfter(op2);
      op2 = op;
      op->set_isClassMember(TRUE);
      op->set_Description(" return the regular expression used to bypass\n"
			  " " + what.lower() + " s on reverse/roundtrip");
      op->set_cpp("${type}", "",
		"  read_if_needed_();\n"
		"\n"
		"  return " + rg + ";\n",
		  FALSE, 0, 0);
      op->set_java("${type}", "",
		   "  read_if_needed_();\n"
		   "\n"
		   "  return " + rg + ";\n",
		   FALSE);
      
      s = "isReverseRoundtrip" + what + "RegExpCaseSensitive";
      op = settings->add_op(s, PublicVisibility, "bool");
      op->moveAfter(op2);
      op2 = op;
      op->set_isClassMember(TRUE);
      op->set_Description(" return if the regular expression used to bypass\n"
			  " " + what.lower() + " s on reverse/roundtrip is case sensitive");
      op->set_cpp("${type}", "",
		"  read_if_needed_();\n"
		"\n"
		"  return " + cs + ";\n",
		  FALSE, 0, 0);
      op->set_java("${type}", "",
		   "  read_if_needed_();\n"
		   "\n"
		   "  return " + cs + ";\n",
		   FALSE);

      s = "set_ReverseRoundtrip" + what + "RegExp";
      op = settings->add_op(s, PublicVisibility, "bool", TRUE);
      op->moveAfter(op2);
      op2 = op;
      op->set_isClassMember(TRUE);
      op->set_Description(" set the regular expression used to bypass\n"
			  " " + what.lower() + " s on reverse/roundtrip\n"
			  " On error : return FALSE in C++, produce a RuntimeException in Java");
      op->add_param(0, InputDirection, "s", "string"); 
      op->add_param(1, InputDirection, "cs", "bool"); 
      op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
		  "  UmlCom::send_cmd(" + pfix.lower() + "SettingsCmd, " + cmd + ", s, cs);\n"
		  "  if (UmlCom::read_bool()) {\n"
		  "    " + rg + " = s;\n"
		  "    " + cs + " = cs;\n"
		  "    return TRUE;\n"
		  "  }\n"
		  "  else\n"
		  "    return FALSE;\n",
		  FALSE, 0, 0);
      op->set_java("void", "${t0} ${p0}, ${t1} ${p1}",
		   "  UmlCom.send_cmd(CmdFamily." + pfix.lower() + "SettingsCmd, " + pfix + "SettingsCmd._" + cmd + ", s, cs);\n"
		   "  UmlCom.check();\n"
		   "  " + rg + " = s;\n"
		   "  " + cs + " = cs;\n",
		   FALSE);
      
      op = settings->get_operation("read_");
      
      s = op->cppBody() + "\n\
  " + rg + " = UmlCom::read_string();\n\
  " + cs + " = UmlCom::read_bool();\n";
      op->set_CppBody(s);
      
      s = op->javaBody() + "\n\
  " + rg + " = UmlCom.read_string();\n\
  " + cs + " = UmlCom.read_bool();\n";
      op->set_JavaBody(s);
      
      if (what == "Dir")
	what = "File";
      else
	break;
    }
  }
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_cppvisi_indent(UmlClass * cppsetting)
{
  
  // already root
  
  UmlCom::trace("<b>update CppSettings to manage visibility indent</b><br>\n");

  UmlAttribute * att = 
    cppsetting->add_attribute("_visibility_indent", PrivateVisibility, "string", 0, 0);

  att->set_isClassMember(TRUE);
  att->moveAfter(cppsetting->get_attribute("_is_inline_force_header_in_h"));

  //

  UmlOperation * op = cppsetting->get_operation("read_");
  QCString s;

  s = op->cppBody() + "\n  _visibility_indent = UmlCom::read_string();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "\n  _visibility_indent = UmlCom.read_string();\n";
  op->set_JavaBody(s);
  
  // get

  op = cppsetting->add_op("visibilityIndent", PublicVisibility, "string");
  op->set_isClassMember(TRUE);
  op->set_Description(" return the indent of the visibility specifiers\n");
  op->set_cpp("const ${type} &", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _visibility_indent;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _visibility_indent;\n",
	       FALSE);
  op->moveAfter(cppsetting->get_operation("set_IsGenerateJavadocStyleComment"));
  
  // set
  
  UmlOperation * op2 = cppsetting->add_op("set_VisibilityIndent", PublicVisibility, "bool", TRUE);
  
  op2->set_isClassMember(TRUE);
  op2->add_param(0, InputDirection, "v", "string");
  op2->set_Description(" set visibility specifiers indent\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  
  op2->set_cpp("${type}", "${t0} ${p0}",
	      "  UmlCom::send_cmd(cppSettingsCmd, setCppIndentVisibilityCmd, v);\n"
	      "  if (UmlCom::read_bool()) {\n"
	      "    _visibility_indent = v;\n"
	      "    return TRUE;\n"
	      "  }\n"
	      "  else\n"
	      "    return FALSE;\n",
	      FALSE, 0, 0);

  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.cppSettingsCmd, CppSettingsCmd._setCppIndentVisibilityCmd, v);\n"
		"  UmlCom.check();\n"
		"\n"
		"  _visibility_indent = v;\n",
	      FALSE);
  
  op2->moveAfter(op);

  //

  UmlClass::get("CppSettingsCmd", 0)->add_enum_item("setCppIndentVisibilityCmd");
}

//
//
//

bool ask_for_upgrade()
{
  if (QMessageBox::warning(0, "Upgrade",
			   "VERY IMPORTANT : in case you valid the upgrade and\n"
			   "the message indicating that all is done never appears,\n"
			   "you MUST close the project WITHOUT saving it\n\n\n"
			   "Upgrade the plug-out ?",
			   "Yes", "No", QString::null, 1, 1)
	  != 0)
    return FALSE;
  
  if (! UmlCom::set_root_permission(TRUE)) {
    QMessageBox::critical(0, "Upgrade", 
			  "Can't be done until all the dialog are closed");
    return FALSE;
  }
   
  CppSettings::set_UseDefaults(TRUE);
  JavaSettings::set_UseDefaults(TRUE);
      
  return TRUE;
}

//

void update_api_version(const char * v)
{
  UmlClass * com = UmlClass::get("UmlCom", 0);
  const QVector<UmlItem> ch = com->children();
  
  for (unsigned i = 0; i != ch.size(); i += 1){
    if ((ch[i]->kind() == anOperation) &&
	(ch[i]->name() == "connect")) {
      unsigned uid = UmlCom::user_id();
      UmlOperation * op = (UmlOperation *) ch[i];
      QCString body;
      int index1;
      int index2;
      
      UmlCom::set_user_id(0);
  
      body = op->cppBody();
      index1 = body.find("write_unsigned(") + 15;
      index2 = body.find(")", index1);
      body.replace(index1, index2-index1, v);
      op->set_CppBody(body);
      
      body = op->javaBody();
      index1 = body.find("write_unsigned(") + 15;
      index2 = body.find(")", index1);
      body.replace(index1, index2-index1, v);
      op->set_JavaBody(body);
      
      UmlCom::set_user_id(uid);
      
      return;
    }
  }
}

//
// Entry point
//

bool UmlPackage::upgrade() {
  bool cpp_default = CppSettings::useDefaults();
  bool java_default = JavaSettings::useDefaults();
  
  UmlClass * uml_base_item = UmlClass::get("UmlBaseItem", 0);
  UmlClass * uml_item = UmlClass::get("UmlItem", 0);
  
  if ((uml_base_item == 0) || (uml_item == 0))
    UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font face=helvetica color=red>Not a plug-out</font><br>\n");
  else {
    bool work = FALSE;
    
    if (UmlClass::get("UmlBaseState", 0) == 0) {
      if (! ask_for_upgrade())
	return FALSE;
      
      set_childrenVisible(FALSE);
      upgrade_states(uml_base_item, uml_item);
      work = TRUE;
    }
    
    UmlClass * uml_base_class = UmlClass::get("UmlBaseClass", 0);
    
    if (! class_set_name_defined(uml_base_class)) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      upgrade_rename_class(uml_base_item, uml_base_class);
      work = TRUE;
    }
    
    if (! item_move_after_defined(uml_base_item)) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      upgrade_new_settings__move_after_item(uml_base_item, uml_item);
      work = TRUE;
    }
    
    if (one_component_per_class(uml_base_class)) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      several_components_per_class(uml_base_class);
      work = TRUE;
    }
    
    UmlClass * javasettings = UmlClass::get("JavaSettings", 0);
    
    if (java_jdk1_4(javasettings)) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      upgrade_jdk5(javasettings);
      fixe_package_diagram();
      fixe_umlbaseactualparameter_read();
      work = TRUE;
    }
    
    UmlClass * baserelation = UmlClass::get("UmlBaseRelation", 0);
    
    if (baserelation->get_operation("association") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_class_association(baserelation);
      add_package_visibility();
      work = TRUE;
    }
    
    if (baserelation->get_operation("side") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_side(baserelation);
      work = TRUE;
    }
    
    if (UmlClass::get("UmlBaseObjectDiagram", 0) == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_objectdiagram(uml_base_item, uml_item);
      work = TRUE;
    }
    
    if (UmlClass::get("UmlBaseActivity", 0) == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_activity(uml_base_item, uml_item);
      base_state_include_umlcom();
      work = TRUE;
    }
    
    UmlClass * uml_com = UmlClass::get("UmlCom", 0);
    
    if (uml_com->get_operation("showTrace") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_new_trace_operations(uml_com);
      work = TRUE;
    }
    
    UmlClass * basepinparam = UmlClass::get("UmlBasePinParameter", 0);
    
    if (basepinparam->get_operation("isStream") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      fixe_activity(basepinparam);
      work = TRUE;
    }

    UmlClass * cppsettings = UmlClass::get("CppSettings", 0);

    if (cppsettings->get_attribute("_is_set_param_ref") == 0)  {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_cpp_set_param_ref(cppsettings);
      upgrade_setter_getter();

      work = TRUE;
    }

    if (cppsettings->get_attribute("_is_relative_path") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_cpp_relative_path_force_namespace(cppsettings);

      work = TRUE;
    }

    UmlClass * umlsettings = UmlClass::get("UmlSettings", 0);

    if (umlsettings->get_operation("umlGetName") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_getter_setter_rules(umlsettings);
      add_extension_points();

      work = TRUE;
    }
    
    if (uml_base_class->get_attribute("_java_public") != 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      remove_java_public(uml_base_class);
      add_cpp_root_relative_path(cppsettings);

      work = TRUE;
    }
    
    if (cppsettings->get_attribute("_is_generate_javadoc_comment") == 0)  {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_cpp_generate_javadoc_comment(cppsettings);
      add_java_generate_javadoc_comment(javasettings);

      work = TRUE;
    }    
    
    UmlClass * baseclassmember = UmlClass::get("UmlBaseClassMember", 0);
    
    if (baseclassmember->get_attribute("_constraint") == 0)  {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_constraint(baseclassmember);

      work = TRUE;
    }    
    
    if (uml_base_item->get_operation("deleteIt") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_deleteit(uml_base_item);

      work = TRUE;
    }   
    
    if (uml_base_item->get_attribute("_modeler_id") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_get_id(uml_base_item);
      add_class_instance(uml_base_item, uml_item);
      fixe_parameterset_read_uml();
      
      work = TRUE;
    }

    if (UmlClass::get("UmlBaseParameter", 0) == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_umlbaseparameter(uml_base_item, uml_item);
      add_umlbasetypespec(uml_base_item, uml_item);
      add_umlbaseview_umlview(uml_base_item, uml_item);
      
      work = TRUE;
    }
    
    if (cppsettings->get_operation("relationAttributeStereotype") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      fixe_set_name(uml_base_item);
      add_attribute_multiplicity(umlsettings, cppsettings, javasettings,
				 UmlClass::get("IdlSettings", 0));

      work = TRUE;
    }
    
    UmlClass * transition = UmlClass::get("UmlBaseTransition", 0);
    
    if (transition->get_operation("isExternal") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_external(transition);

      work = TRUE;
    }
    
    UmlClass * bci = UmlClass::get("UmlBaseClassInstance", 0);
    UmlOperation * op = bci->get_operation("add_Relation");
    const QValueList<UmlParameter> params = op->params();

    if (params.first().type.type->name() == "UmlAttribute") {
      if (!work && !ask_for_upgrade())
	return FALSE;
      fixe_classinstance(bci);

      work = TRUE;
    }
    
    if (UmlClass::get("PhpSettings", 0) == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      add_php();
      add_force_body_gen();
      fixe_java_alloc();

      work = TRUE;
    }
    
    UmlClass * uml_base_component = UmlClass::get("UmlBaseComponent", 0);

    if (uml_base_component->get_operation("realizingClasses") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      realization(uml_base_component);
      add_missing_opers();
      work = TRUE;
    }
    
    if (UmlClass::get("UmlUseCaseDiagramDefinition", 0) == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      replacefriend();
      add_diagram_def();
      work = TRUE;
    }
    
    UmlClass * uml_base_package = UmlClass::get("UmlBasePackage", 0);
      
    if (uml_base_package->get_operation("findCppNamespace") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      unsigned uid = UmlCom::user_id();
      
      UmlCom::set_user_id(0);
      
      update_pack_global(uml_base_package);
      add_contextual_body_indent();
      php_javadocstylecomment();
      add_profile();	// theo Api 40 but compatible api 38
      modify_texts();
      fixe_idlsetting_read();
      
      UmlCom::set_user_id(uid);
      
      work = TRUE;
    }
    
    op = wrong_umlcom_send_cmd(uml_com);
    
    if (op != 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      fixe_umlcom_send_cmd(op);
      work = TRUE;
    }
    
    // fixe UmlBaseFragment::read_()
    
    UmlClass * base_frg = UmlClass::get("UmlBaseFragment", 0);
    
    op = base_frg->get_operation("read_");
    
    QCString s = op->cppBody();
    
    if (s.find("_container = 0;") == -1) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      unsigned uid = UmlCom::user_id();
      
      UmlCom::set_user_id(0);
      
      UmlCom::trace("<b>fixe <i>UmlBaseFragment::read_()</i></b><br>\n");
      s += "  _container = 0;\n";
      op->set_CppBody(s);
      
      UmlCom::set_user_id(uid);
      
      work = TRUE;
    }

    if (javasettings->get_operation("isForcePackagePrefixGeneration") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      unsigned uid = UmlCom::user_id();
  
      UmlCom::set_user_id(0);
  
      add_bypass_python();
      add_javasettings_forcepackageprefixgeneration(javasettings);
      add_cppsettings_builtindir();
      update_uml_com2();
      
      UmlCom::set_user_id(uid);
      
      upgrade_interaction();
      
      work = TRUE;
    }
    
    {
      UmlOperation * createstateaction = 
	UmlClass::get("UmlBaseStateAction", 0)->get_operation("create");
      UmlParameter p = createstateaction->params().first();
    
      if (p.type.type != uml_item) {
	if (!work && !ask_for_upgrade())
	  return FALSE;
	
	UmlCom::trace("<b>fixe <i>UmlBaseStateAction::create()</i> first parameter type</b><br>\n");
	
	unsigned uid = UmlCom::user_id();
	
	UmlCom::set_user_id(0);
	p.type.type = uml_item;
	createstateaction->replaceParameter(0, p);
	UmlCom::set_user_id(uid);
      
	work = TRUE;
      }
    }
    
    if (UmlClass::get("UmlBaseAttribute", 0)->get_operation("isDerived") == 0) {
      if (!work && !ask_for_upgrade())
	  return FALSE;
	
      add_property_modifiers();
      
      work = TRUE;
    }
    
    if (umlsettings->get_operation("add_type")->cppBody().find("replace")
	== -1) {
      if (!work && !ask_for_upgrade())
	  return FALSE;
	
      fixe_settings(umlsettings, cppsettings, javasettings);
      
      work = TRUE;
    }
    
    UmlClass * pythonsettings = UmlClass::get("PythonSettings", 0);
    
    if (pythonsettings == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      add_python();
      pythonsettings = UmlClass::get("PythonSettings", 0);

      work = TRUE;
    }
    
    if (UmlClass::get("UmlBaseActivityPartition", 0) == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      fixe_set_associateddiagram(uml_base_item->parent());
      add_partition(uml_base_item, uml_item);

      work = TRUE;
    }
    
    if (UmlClass::get("UmlBaseAcceptCallAction", 0) == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      bypass_qvector_bug();
      add_additionalactions(uml_base_item, uml_item);

      work = TRUE;
    }

    if (pythonsettings->get_operation("initOperationDef") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_default_initoper(pythonsettings);
      work = TRUE;
    }
    
    UmlClass * baseoper = UmlClass::get("UmlBaseOperation", 0);
    
    if (baseoper->get_operation("methods") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_methods(baseoper, uml_item);
      add_state_specification();
      add_activity_specification();
      work = TRUE;
    }
    
    if (uml_base_package->get_operation("importProject") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
    
      add_import_project(uml_base_package);
      work = TRUE;
    }
    
    if (uml_base_class->get_operation("isActive") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
    
      add_is_active(uml_base_class);
      work = TRUE;
    }

    if (pythonsettings->get_operation("isPython_3_operation") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_operation3(pythonsettings);
      work = TRUE;
    }

    if (cppsettings->get_operation("isInlineOperationForceIncludesInHeader") == 0)  {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_cpp_inline_oper_force_incl_in_h(cppsettings);

      work = TRUE;
    }
    
    UmlClass * baseseqmsg = UmlClass::get("UmlBaseSequenceMessage", 0);
    
    if (baseseqmsg->get_operation("stereotype") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_stereotype_on_seq_msg(baseseqmsg);

      work = TRUE;
    }
    
    op = UmlClass::get("UmlBaseClassDiagram", 0)->get_operation("create");
    UmlParameter pa = op->params()[0];

    if (pa.type.type != UmlClass::get("UmlItem", 0)) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      UmlCom::trace("update UmlBaseClassDiagram::create<br>\n");
      pa.type.type = UmlClass::get("UmlItem", 0);
      op->replaceParameter(0, pa);
      work = TRUE;
    }
    
    op = uml_com->get_operation("bye");
    if (op->params().isEmpty()) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      UmlCom::trace("update UmlCom::bye()<br>\n");

      pa.name = "v";
      pa.default_value = "";
      pa.type.explicit_type = "int";
      pa.type.type = 0;
      pa.dir = InputDirection;
      op->addParameter(0, pa);
      s = op->cppDecl();
      op->set_CppDecl(s.insert(s.find("${)}"), "${t0} ${p0}"));
      s = op->cppDef();
      op->set_CppDef(s.insert(s.find("${)}"), "${t0} ${p0}"));
      op->set_CppBody("  send_cmd(miscGlobalCmd, byeCmd, v, \"\");");
      s = op->javaDecl();
      op->set_JavaDecl(s.insert(s.find("${)}"), "${t0} ${p0}"));
      op->set_JavaBody("  send_cmd(CmdFamily.miscGlobalCmd, MiscGlobalCmd._byeCmd, v, \"\");");
      work = TRUE;
    }
    
    op = uml_base_item->get_operation("read_");
    s = op->cppBody();
    
    if (s.find("UmlCom::bye();") != -1) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      UmlCom::trace("fixe UmlBaseItem::read_()<br>\n");
      
      s = s.insert(s.find("UmlCom::bye();") + 12, "-1");
      op->set_CppBody(s);
      
      s = op->javaBody();
      s = s.insert(s.find("UmlCom.bye();") + 11, "-1");
      op->set_JavaBody(s);
      work = TRUE;
    }
    
    UmlClass * umlbaseactivity = UmlClass::get("UmlBaseActivity", 0);
    
    if (umlbaseactivity->get_attribute("_constraint") == 0)  {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_constraint2(umlbaseactivity, 
		      "set_JavaPostCondition", "_java_post_condition");
      add_constraint2(UmlClass::get("UmlBaseActivityAction", 0), 
		      "set_JavaPostCondition", "_java_post_condition");

      work = TRUE;
    }
    
    UmlClass * phpsettings = UmlClass::get("PhpSettings", 0);

    if (phpsettings->get_attribute("_req_with_path") == 0)  {
      if (!work && !ask_for_upgrade())
	return FALSE;
      php_add_require_once();

      work = TRUE;
    }
    
    UmlAttribute * attpython = 
      UmlClass::get("UmlStereotype", 0)->get_attribute("python");
    int index = (s = attpython->cppDecl()).find("#ifdef WITHPHP");
    
    if (index != -1) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      UmlCom::trace("fixe UmlStereotype::python<br>\n");
      s.replace(index + 12, 2, "YTHON");
      attpython->set_CppDecl(s);
      work = TRUE;
    }
    
    if (cppsettings->get_attribute("_dir_regexp") == 0)  {
      if (!work && !ask_for_upgrade())
	return FALSE;
      add_rev_filter();

      work = TRUE;
    }
    
    if (uml_base_package->get_operation("phpNamespace") == 0) {
      if (!work && !ask_for_upgrade())
	return FALSE;
      
      unsigned uid = UmlCom::user_id();
      
      UmlCom::set_user_id(0);
  
      add_php_namespace(uml_base_package, phpsettings);
      add_cppvisi_indent(cppsettings);
      add_stateref();
      
      UmlCom::set_user_id(uid);
      
      work = TRUE;
    }
    
    if (work) {
      CppSettings::set_UseDefaults(cpp_default);
      JavaSettings::set_UseDefaults(java_default);

      UmlCom::trace("update api version<br>\n");
      update_api_version("55");
      UmlCom::message("ask for save-as");
      QMessageBox::information(0, "Upgrade", 
			       "Upgrade done\n\n"
			       "a 'save as' will be called, thus the project will be closed");  
      UmlCom::set_root_permission(FALSE);
      return TRUE;
    }
    else
      UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font face=helvetica>Plug-out already up to date</font><br>\n");
  }
  
  return FALSE;
}
