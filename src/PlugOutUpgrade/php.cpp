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

#include "util.h"
#include "php.h"

//
// add php
//

UmlArtifact * add_php_settings()
{
  // already root
  UmlClass * umlsettings = UmlClass::get("UmlSettings", 0);
  UmlClass * javasettings = UmlClass::get("JavaSettings", 0);
  UmlArtifact * base_art =
    UmlArtifact::made((UmlDeploymentView *)
		      umlsettings->associatedArtifact()->parent(),
		      "PhpSettings");
  
  base_art->moveAfter(javasettings->associatedArtifact());
  
  base_art->set_CppHeader("#ifndef _${NAME}_H\n"
			  "#define _${NAME}_H\n"
			  "\n"
			  "#ifdef WITHPHP\n"
			  "\n"
			  "${comment}\n"
			  "${includes}\n"
			  "${declarations}\n"
			  "${namespace_start}\n"
			  "${definition}\n"
			  "${namespace_end}\n"
			  "\n"
			  "#endif\n"
			  "\n"
			  "#endif\n");

  base_art->set_CppSource("#ifdef WITHPHP\n"
			  "\n"
			  "${comment}\n"
			  "${includes}\n"
			  "#include \"UmlCom.h\"\n"
			  "#include \"PhpSettingsCmd.h\"\n"
			  "#include \"JavaSettingsCmd.h\"\n"
			  "#include \"UmlStereotype.h\"\n"
			  "#ifdef WITHJAVA\n"
			  "#include \"JavaSettings.h\"\n"
			  "#endif\n"
			  "${namespace_start}\n"
			  "${members}\n"
			  "${namespace_end}\n"
			  "\n"
			  "#endif\n");
  
  UmlClass * phpsettings = UmlClass::create(umlsettings->parent(), "PhpSettings");
  
  phpsettings->moveAfter(javasettings);
  
  base_art->addAssociatedClass(phpsettings);
  phpsettings->set_CppDecl(CppSettings::classDecl());
  phpsettings->set_JavaDecl(JavaSettings::classDecl());
  phpsettings->set_Description(" This class manages settings concerning PHP, configured through\n"
			       " the 'Generation settings' dialog.\n"
			       "\n"
			       " This class may be defined as a 'singleton', but I prefer to use static\n"
			       " members allowing to just write 'PhpSettings::member' rather than\n"
			       " 'PhpSettings::instance()->member' or other long sentence like this.");

  int index;

  {
    const char * opers[] = {
      "useDefaults", "set_UseDefaults",
      "classStereotype", "set_ClassStereotype",
      "rootDir", "set_RootDir",
      "sourceContent", "set_SourceContent",
      "sourceExtension", "set_SourceExtension",
      "classDecl", "set_ClassDecl",
      "externalClassDecl", "set_ExternalClassDecl",
      "enumDecl", "set_EnumDecl",
      "interfaceDecl", "set_InterfaceDecl"
    };
    
    for (index = 0; index != sizeof(opers)/sizeof(opers[0]); index += 1)
      UmlOperation::java2Php(phpsettings, javasettings, opers[index]);
  }
  
  UmlOperation * op;
  QCString s;
  
  op = UmlOperation::java2Php(phpsettings, javasettings, "attributeDecl");
  op->set_Description(" returns the default definition of an attribute");
  op->removeParameter(0);
  s = op->cppDecl();
  s.remove(s.find("const ${t0} ${p0}"), 17);
  op->set_CppDecl(s);
  s = op->cppDef();
  s.remove(s.find("const ${t0} ${p0}"), 17);
  op->set_CppDef(s);
  s = op->javaDecl();
  s.remove(s.find("${t0} ${p0}"), 11);
  op->set_JavaDecl(s);
  op->set_CppBody("  read_if_needed_();\n"
		  "\n"
		  "  return _attr_decl;\n");
  op->set_JavaBody("  read_if_needed_();\n"
		  "\n"
		  "  return _attr_decl;\n");

  op = UmlOperation::java2Php(phpsettings, javasettings, "set_AttributeDecl");
  op->set_Description(" set the default definition of an attribute\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  op->removeParameter(0);
  s = op->cppDecl();
  s.replace(s.find("const ${t0} ${p0}, ${t1} ${p1}"), 30, "${t0} ${p0}");
  op->set_CppDecl(s);
  s = op->cppDef();
  s.replace(s.find("const ${t0} ${p0}, ${t1} ${p1}"), 30, "${t0} ${p0}");
  op->set_CppDef(s);
  s = op->javaDecl();
  s.remove(s.find(", ${t1} ${p1}"), 13);
  op->set_JavaDecl(s);
  op->set_CppBody("  UmlCom::send_cmd(phpSettingsCmd, setPhpAttributeDeclCmd, v);\n"
		  "  if (UmlCom::read_bool()) {\n"
		  "    _attr_decl = v;\n"
		  "    return TRUE;\n"
		  "  }\n"
		  "  else\n"
		  "    return FALSE;\n");
  op->set_JavaBody("  UmlCom.send_cmd(CmdFamily.phpSettingsCmd, PhpSettingsCmd._setPhpAttributeDeclCmd, v);\n"
		   "  UmlCom.check();\n"
		   "\n"
		   "  _attr_decl = v;\n");

  {
    const char * opers[] = {
      "enumItemDecl", "set_EnumItemDecl"
    };
    
    for (index = 0; index != sizeof(opers)/sizeof(opers[0]); index += 1)
      UmlOperation::java2Php(phpsettings, javasettings, opers[index]);
  }

  op = UmlOperation::java2Php(phpsettings, javasettings, "relationDecl");
  op->set_Description(" returns the default definition of an relation");
  op->removeParameter(0);
  s = op->cppDecl();
  s.remove(s.find("const ${t0} ${p0}"), 17);
  op->set_CppDecl(s);
  s = op->cppDef();
  s.remove(s.find("const ${t0} ${p0}"), 17);
  op->set_CppDef(s);
  s = op->javaDecl();
  s.remove(s.find("${t0} ${p0}"), 11);
  op->set_JavaDecl(s);
  op->set_CppBody("  read_if_needed_();\n"
		  "\n"
		  "  return _rel_decl;\n");
  op->set_JavaBody("  read_if_needed_();\n"
		  "\n"
		  "  return _rel_decl;\n");

  op = UmlOperation::java2Php(phpsettings, javasettings, "set_RelationDecl");
  op->set_Description(" set the default definition of an relation\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  op->removeParameter(0);
  s = op->cppDecl();
  s.replace(s.find("const ${t0} ${p0}, ${t1} ${p1}"), 30, "${t0} ${p0}");
  op->set_CppDecl(s);
  s = op->cppDef();
  s.replace(s.find("const ${t0} ${p0}, ${t1} ${p1}"), 30, "${t0} ${p0}");
  op->set_CppDef(s);
  s = op->javaDecl();
  s.remove(s.find(", ${t1} ${p1}"), 13);
  op->set_JavaDecl(s);
  op->set_CppBody("  UmlCom::send_cmd(phpSettingsCmd, setPhpRelationDeclCmd, v);\n"
		  "  if (UmlCom::read_bool()) {\n"
		  "    _rel_decl = v;\n"
		  "    return TRUE;\n"
		  "  }\n"
		  "  else\n"
		  "    return FALSE;\n");
  op->set_JavaBody("  UmlCom.send_cmd(CmdFamily.phpSettingsCmd, PhpSettingsCmd._setPhpRelationDeclCmd, v);\n"
		   "  UmlCom.check();\n"
		   "\n"
		   "  _rel_decl = v;\n");

  {
    const char * opers[] = {
      "operationDef", "set_OperationDef"
    };
    
    for (index = 0; index != sizeof(opers)/sizeof(opers[0]); index += 1)
      UmlOperation::java2Php(phpsettings, javasettings, opers[index]);
  }

  op = UmlOperation::java2Php(phpsettings, javasettings, "getVisibility");
  op->set_CppBody("#ifdef WITHJAVA\n"
		  "  return JavaSettings::getVisibility();\n"
		  "#else\n"
		  "  read_if_needed_();\n"
		  "  \n"
		  "  return _get_visibility;\n"
		  "#endif\n");
  op->set_JavaBody("  return JavaSettings.getVisibility();\n");
  op->set_Description(op->description() + 
		      "\n\n note : visibility shared with Java");
  
  op = UmlOperation::java2Php(phpsettings, javasettings, "set_GetVisibility");
  op->set_CppBody("#ifdef WITHJAVA\n"
		  "  return JavaSettings::set_GetVisibility(v);\n"
		  "#else\n"
		  "  UmlCom::send_cmd(phpSettingsCmd, setJavaGetvisibilityCmd, v);\n"
		  "  if (UmlCom::read_bool()) {\n"
		  "    _get_visibility = v;\n"
		  "    return TRUE;\n"
		  "  }\n"
		  "  else\n"
		  "    return FALSE;\n"
		  "#endif\n");
  op->set_JavaBody("  JavaSettings.set_GetVisibility(v);\n");
  op->set_Description(op->description() + 
		      "\n\n note : visibility shared with Java");

  {
    const char * opers[] = {
      "getName", "set_GetName",
      "setName", "set_SetName",
      "isGetFinal", "set_IsGetFinal",
      "isSetFinal", "set_IsSetFinal"
    };
    
    for (index = 0; index != sizeof(opers)/sizeof(opers[0]); index += 1)
      UmlOperation::java2Php(phpsettings, javasettings, opers[index]);
  }

  op = UmlOperation::java2Php(phpsettings, javasettings, "setVisibility");
  op->set_CppBody("#ifdef WITHJAVA\n"
		  "  return JavaSettings::setVisibility();\n"
		  "#else\n"
		  "  read_if_needed_();\n"
		  "  \n"
		  "  return _set_visibility;\n"
		  "#endif\n");
  op->set_JavaBody("  return JavaSettings.setVisibility();\n");
  op->set_Description(op->description() + 
		      "\n\n   note : visibility shared with Java");
  
  op = UmlOperation::java2Php(phpsettings, javasettings, "set_SetVisibility");
  op->set_CppBody("#ifdef WITHJAVA\n"
		  "  return JavaSettings::set_SetVisibility(v);\n"
		  "#else\n"
		  "  UmlCom::send_cmd(phpSettingsCmd, setJavaSetVisibilityCmd, v);\n"
		  "  if (UmlCom::read_bool()) {\n"
		  "    _set_visibility = v;\n"
		  "    return TRUE;\n"
		  "  }\n"
		  "  else\n"
		  "    return FALSE;\n"
		  "#endif\n");
  op->set_JavaBody("  JavaSettings.set_SetVisibility(v);\n");
  op->set_Description(op->description() + 
		      "\n\n   note : visibility shared with Java");

  UmlOperation::java2Php(phpsettings, javasettings, "JavaSettings", "PhpSettings");

  UmlRelation * rel;
  
  if ((rel = UmlBaseRelation::create(aGeneralisation, phpsettings, umlsettings)) == 0) {
    QCString msg = "PhpSettings can't inherit UmlSettings<br>\n";
    
    UmlCom::trace("<b>" + msg + "</b>");
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
  
  {
    const char * attrs[] = {
      "_defined", 
      "_root", 
      "_class_decl", 
      "_external_class_decl", 
      "_enum_decl", 
      "_interface_decl"
    };
    
    for (index = 0; index != sizeof(attrs)/sizeof(attrs[0]); index += 1)
      UmlAttribute::java2Php(phpsettings, javasettings, attrs[index]);
  }

  phpsettings->add_attribute("_attr_decl", PrivateVisibility, "string", 0, 0)
    ->set_isClassMember(TRUE);
  
  UmlAttribute::java2Php(phpsettings, javasettings, "_enum_item_decl");
  
  phpsettings->add_attribute("_rel_decl", PrivateVisibility, "string", 0, 0)
    ->set_isClassMember(TRUE);
  
  UmlAttribute::java2Php(phpsettings, javasettings, "_oper_def");

  rel = UmlRelation::java2Php(phpsettings, javasettings, "_get_visibility");
  rel->set_CppDecl("#ifndef WITHJAVA\n" + rel->cppDecl() + "#endif\n");
  rel->set_JavaDecl("");

  UmlAttribute::java2Php(phpsettings, javasettings, "_get_name");
  UmlAttribute::java2Php(phpsettings, javasettings, "_is_get_final");

  rel = UmlRelation::java2Php(phpsettings, javasettings, "_set_visibility");
  rel->set_CppDecl("#ifndef WITHJAVA\n" + rel->cppDecl() + "#endif\n");
  rel->set_JavaDecl("");
  
  {
    const char * attrs[] = {
      "_set_name", 
      "_is_set_final", 
      "_src_content", 
      "_ext"
    };
    
    for (index = 0; index != sizeof(attrs)/sizeof(attrs[0]); index += 1)
      UmlAttribute::java2Php(phpsettings, javasettings, attrs[index]);
  }
  
  op = phpsettings->add_op("read_", ProtectedVisibility, "void");
  op->set_isClassMember(TRUE);
  op->set_cpp("${type}", "",
	      "  _root = UmlCom::read_string();\n"
	      "  \n"
	      "  unsigned n;\n"
	      "  unsigned index;\n"
	      "  \n"
	      "  n = UmlCom::read_unsigned();\n"
	      "  \n"
	      "  for (index = 0; index != n; index += 1)\n"
	      "    UmlSettings::_class_stereotypes[index].php = UmlCom::read_string();\n"
	      "  \n"
	      "  _src_content = UmlCom::read_string();\n"
	      "  _ext = UmlCom::read_string();\n"
	      "\n"
	      "  _class_decl = UmlCom::read_string();\n"
	      "  _external_class_decl = UmlCom::read_string();\n"
	      "  _enum_decl = UmlCom::read_string();\n"
	      "  _interface_decl = UmlCom::read_string();\n"
	      "  _attr_decl = UmlCom::read_string();\n"
	      "  _enum_item_decl = UmlCom::read_string();\n"
	      "  _rel_decl = UmlCom::read_string();\n"
	      "  _oper_def = UmlCom::read_string();\n"
	      "#ifndef WITHJAVA\n"
	      "  _get_visibility =\n"
	      "#endif\n"
	      "    (aVisibility) UmlCom::read_char();\n"
	      "  _get_name = UmlCom::read_string();\n"
	      "  _is_get_final = UmlCom::read_bool();\n"
	      "#ifndef WITHJAVA\n"
	      "  _set_visibility =\n"
	      "#endif\n"
	      "    (aVisibility) UmlCom::read_char();\n"
	      "  _set_name = UmlCom::read_string();\n"
	      "  _is_set_final = UmlCom::read_bool();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	      "  _root = UmlCom.read_string();\n"
	      "  \n"
	      "  int n;\n"
	      "  int index;\n"
	      "  \n"
	      "  n = UmlCom.read_unsigned();\n"
	      "  \n"
	      "  for (index = 0; index != n; index += 1)\n"
	      "    UmlSettings._class_stereotypes[index].php = UmlCom.read_string();\n"
	      "  \n"
	      "  _src_content = UmlCom.read_string();\n"
	      "  _ext = UmlCom.read_string();\n"
	      "\n"
	      "  _class_decl = UmlCom.read_string();\n"
	      "  _external_class_decl = UmlCom.read_string();\n"
	      "  _enum_decl = UmlCom.read_string();\n"
	      "  _interface_decl = UmlCom.read_string();\n"
	      "  _attr_decl = UmlCom.read_string();\n"
	      "  _enum_item_decl = UmlCom.read_string();\n"
	      "  _rel_decl = UmlCom.read_string();\n"
	      "  _oper_def = UmlCom.read_string();\n"
	      "  UmlCom.read_char(); // getter visibility\n"
	      "  _get_name = UmlCom.read_string();\n"
	      "  _is_get_final = UmlCom.read_bool();\n"
	      "  UmlCom.read_char(); // setter visibility\n"
	      "  _set_name = UmlCom.read_string();\n"
	      "  _is_set_final = UmlCom.read_bool();\n",
	       FALSE);
  
  op = phpsettings->add_op("read_if_needed_", ProtectedVisibility, "void");
  op->set_isClassMember(TRUE);
  op->set_cpp("${type}", "",
	      "  UmlSettings::read_if_needed_();\n"
	      "  if (!_defined) {\n"
	      "    UmlCom::send_cmd(phpSettingsCmd, getPhpSettingsCmd);\n"
	      "    read_();\n"
	      "    _defined = TRUE;\n"
	      "  }\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  UmlSettings.read_if_needed_();\n"
	       "  if (!_defined) {\n"
	       "    UmlCom.send_cmd(CmdFamily.phpSettingsCmd, PhpSettingsCmd._getPhpSettingsCmd);\n"
	       "    read_();\n"
	       "    _defined = true;\n"
	       "  }\n",
	       FALSE);
  
  return base_art;
}

UmlArtifact *  add_php_settings_cmd()
{
  // already root
  UmlClass * javasettingscmd = UmlClass::get("JavaSettingsCmd", 0);
  UmlArtifact * base_art =
    UmlArtifact::made((UmlDeploymentView *)
		      javasettingscmd->associatedArtifact()->parent(),
		      "PhpSettingsCmd");
  
  base_art->moveAfter(javasettingscmd->associatedArtifact());
  
  UmlClass * phpsettingscmd =
    UmlClass::create(javasettingscmd->parent(), "PhpSettingsCmd");
  
  phpsettingscmd->moveAfter(javasettingscmd);
  
  base_art->addAssociatedClass(phpsettingscmd);
  phpsettingscmd->set_Stereotype(javasettingscmd->stereotype());
  phpsettingscmd->set_CppDecl(javasettingscmd->cppDecl());
  phpsettingscmd->set_JavaDecl(javasettingscmd->javaDecl());

  unsigned index;
  const char * items[] = {
    "getPhpSettingsCmd",
    "getPhpUseDefaultsCmd",
  
    "setPhpUseDefaultsCmd",
    "setPhpClassStereotypeCmd",
    "setPhpRootdirCmd",
    "setPhpSourceContentCmd",
    "setPhpSourceExtensionCmd",
    "setPhpClassDeclCmd",
    "setPhpEnumDeclCmd",
    "setPhpExternalClassDeclCmd",
    "setPhpInterfaceDeclCmd",
    "setPhpAttributeDeclCmd",
    "setPhpEnumItemDeclCmd",
    "setPhpRelationDeclCmd",
    "setPhpOperationDefCmd",
    "setPhpGetNameCmd",
    "setPhpSetNameCmd",
    "setPhpIsGetFinalCmd",
    "setPhpIsSetFinalCmd",
    "setPhpIsSetParamFinalCmd",
  };
  
  for (index = 0; index != sizeof(items)/sizeof(items[0]); index += 1)
    phpsettingscmd->add_enum_item(items[index]);
  
  UmlClass::get("CmdFamily", 0)->add_enum_item("phpSettingsCmd");
  
  return base_art;
}

void add_php_on_enums()
{
  // already root
  UmlClass * itcmd = UmlClass::get("OnInstanceCmd", 0);
  
  unsigned index;
  const char * items[] = {
    "setPhpDeclCmd",
    "setIsPhpExternalCmd",
    "setIsPhpFinalCmd",
    "setPhpSrcCmd",
    "setPhpFinalCmd",
    "setPhpBodyCmd",
    "setPhpNameSpecCmd",
    "setPhpDirCmd",
    "setPhpFrozenCmd",
  };
  
  for (index = 0; index != sizeof(items)/sizeof(items[0]); index += 1)
    itcmd->add_enum_item(items[index]);
  
  UmlAttribute * at = itcmd->add_enum_item("getPhpDefCmd");
  
  at->moveAfter(itcmd->get_attribute("relationsCmd"));
  itcmd->add_enum_item("phpBodyCmd")->moveAfter(at);
  
  //
  
  UmlClass::get("aLanguage", 0)->add_enum_item("phpLanguage");
}

void artifact_add_php()
{
  // already root
  UmlClass * cl = UmlClass::get("UmlBaseArtifact", 0);
  
  UmlOperation * op =
    UmlOperation::java2Php(cl, cl, "javaSource", "phpSource");
  
  op->moveAfter(cl->get_operation("set_JavaSource"));
  UmlOperation::java2Php(cl, cl, "set_JavaSource", "set_PhpSource")
    ->moveAfter(op);
  
  UmlAttribute::java2Php(cl, cl, "_java_src", "_php_src")
    ->moveAfter(cl->get_attribute("_java_src"));
  
  UmlOperation::java2Php(cl, cl, "read_java_", "read_php_")
    ->moveAfter(cl->get_operation("read_java_"));
  
  QCString s;
  
  op = cl->get_operation("unload");
  
  s = op->cppBody();
  s.insert(s.find("#ifdef WITHIDL"),
	   "#ifdef WITHPHP\n"
	   "  _php_src = 0;\n"
	   "#endif\n");
  op->set_CppBody(s);
  
  s = op->javaBody();
  s.insert(s.find("  _idl_src = null;"),
	   "  _php_src = null;\n");
  op->set_JavaBody(s);
}

void attribute_add_php()
{
  // already root
  UmlClass * cl = UmlClass::get("UmlBaseAttribute", 0);
  UmlOperation * op = 
    UmlOperation::java2Php(cl, cl, "read_java_", "read_php_");
  
  op->moveAfter(cl->get_operation("read_java_"));
  
  op->set_CppBody("  UmlBaseClassMember::read_php_();\n");
  op->set_JavaBody("  super.read_php_();\n");
}

void class_add_php()
{
  // already root
  UmlClass * cl = UmlClass::get("UmlBaseClass", 0);
  UmlOperation * prev = cl->get_operation("set_isJavaFinal");
  int index;
  const struct {
    const char * java;
    const char * php;
  } opers[] = {
    { "isJavaExternal", "isPhpExternal" },
    { "set_isJavaExternal", "set_isPhpExternal" },
    { "isJavaFinal", "isPhpFinal" },
    { "set_isJavaFinal", "set_isPhpFinal" },
  };
  
  for (index = 0; index != sizeof(opers)/sizeof(opers[0]); index += 1) {
    UmlOperation * op = 
      UmlOperation::java2Php(cl, cl, opers[index].java, opers[index].php);
    
    op->moveAfter(prev);
    prev = op;
  }

  UmlAttribute * att = 
    UmlAttribute::java2Php(cl, cl, "_java_external", "_php_external");
  
  att->moveAfter(cl->get_attribute("_java_final"));
  
  UmlAttribute::java2Php(cl, cl, "_java_final", "_php_final")
    ->moveAfter(att);
  
  UmlOperation::java2Php(cl, cl, "read_java_", "read_php_")
    ->moveAfter(cl->get_operation("read_java_"));
}

void classitem_add_php()
{
  // already root
  UmlClass * cl = UmlClass::get("UmlBaseClassItem", 0);
  UmlOperation * op = 
    UmlOperation::java2Php(cl, cl, "javaDecl", "phpDecl");
    
  op->moveAfter(cl->get_operation("set_JavaDecl"));
  UmlOperation::java2Php(cl, cl, "set_JavaDecl", "set_PhpDecl")
    ->moveAfter(op);

  UmlAttribute::java2Php(cl, cl, "_java_decl", "_php_decl")
    ->moveAfter(cl->get_attribute("_java_decl"));
  
  UmlOperation::java2Php(cl, cl, "read_java_", "read_php_")
    ->moveAfter(cl->get_operation("read_java_"));

  QCString s;
  
  op = cl->get_operation("unload");
  
  s = op->cppBody();
  s.insert(s.find("#ifdef WITHIDL"),
	   "#ifdef WITHPHP\n"
	   "  _php_decl = 0;\n"
	   "#endif\n");
  op->set_CppBody(s);
  
  s = op->javaBody();
  s.insert(s.find("  _idl_decl = null;"),
	   "  _php_decl = null;\n");
  op->set_JavaBody(s);
}

void item_add_php()
{
  // already root
  UmlClass * cl = UmlClass::get("UmlBaseItem", 0);
  
  UmlOperation::java2Php(cl, cl, "read_java_", "read_php_")
    ->moveAfter(cl->get_operation("read_java_"));

  UmlOperation * op = cl->get_operation("read_if_needed_");
  op->set_CppBody("  if (!_defined) {\n"
		  "#if defined(WITHCPP) & defined(WITHJAVA) & defined(WITHPHP) & defined(WITHIDL)\n"
		  "    UmlCom::send_cmd(_identifier, getDefCmd);\n"
		  "    read_uml_();\n"
		  "    read_cpp_();\n"
		  "    read_java_();\n"
		  "    read_php_();\n"
		  "    read_idl_();\n"
		  "#else\n"
		  "# if defined(WITHCPP) & !defined(WITHJAVA) & !defined(WITHPHP) & !defined(WITHIDL)\n"
		  "    UmlCom::send_cmd(_identifier, getCppDefCmd);\n"
		  "    read_uml_();\n"
		  "    read_cpp_();\n"
		  "# else\n"
		  "#  if !defined(WITHCPP) & defined(WITHJAVA) & !defined(WITHPHP) & !defined(WITHIDL)\n"
		  "    UmlCom::send_cmd(_identifier, getJavaDefCmd);\n"
		  "    read_uml_();\n"
		  "    read_java_();\n"
		  "#  else\n"
		  "#   if !defined(WITHCPP) & !defined(WITHJAVA) & defined(WITHPHP) & !defined(WITHIDL)\n"
		  "    UmlCom::send_cmd(_identifier, getPhpDefCmd);\n"
		  "    read_uml_();\n"
		  "    read_php_();\n"
		  "#   else\n"
		  "#    if !defined(WITHCPP) & !defined(WITHJAVA) & !defined(WITHPHP) & defined(WITHIDL)\n"
		  "    UmlCom::send_cmd(_identifier, getIdlDefCmd);\n"
		  "    read_uml_();\n"
		  "    read_idl_();\n"
		  "#    else\n"
		  "#     if !defined(WITHCPP) & !defined(WITHJAVA) & !defined(WITHPHP) & !defined(WITHIDL)\n"
		  "    UmlCom::send_cmd(_identifier, getUmlDefCmd);\n"
		  "    read_uml_();\n"
		  "#     else\n"
		  "    ... WITHCPP and WITHJAVA and WITHPHP and WITHIDL must be both defined or undefined\n"
		  "    ... or only one of them must be defined\n"
		  "#     endif\n"
		  "#    endif\n"
		  "#   endif\n"
		  "#  endif\n"
		  "# endif\n"
		  "#endif\n"
		  "    \n"
		  "    _defined = TRUE;\n"
		  "  }\n");

  op->set_JavaBody("  if (!_defined) {\n"
		   "    UmlCom.send_cmd(identifier_(), OnInstanceCmd.getDefCmd);\n"
		   "    read_uml_();\n"
		   "    read_cpp_();\n"
		   "    read_java_();\n"
		   "    read_php_();\n"
		   "    read_idl_();\n"
		   "    \n"
		   "    _defined = true;\n"
		   "  }\n");
}

void operation_add_php()
{
  // already root
  UmlClass * cl = UmlClass::get("UmlBaseOperation", 0);
  UmlOperation * prev = cl->get_operation("set_JavaGetSetFrozen");
  UmlOperation * op;
  int index;
  const struct {
    const char * java;
    const char * php;
  } opers[] = {
    { "isJavaFinal", "isPhpFinal" },
    { "set_isJavaFinal", "set_isPhpFinal" },
    { "javaDef", "phpDef" },
    { "set_JavaDef", "set_PhpDef" },
    { "javaBody", "phpBody" },
    { "set_JavaBody", "set_PhpBody" },
    { "javaNameSpec", "phpNameSpec" },
    { "set_JavaNameSpec", "set_PhpNameSpec" },
    { "javaGetSetFrozen", "phpGetSetFrozen" },
    { "set_JavaGetSetFrozen", "set_PhpGetSetFrozen" }
  };
  
  for (index = 0; index != sizeof(opers)/sizeof(opers[0]); index += 1) {
    op = UmlOperation::java2Php(cl, cl, opers[index].java, opers[index].php);
    op->moveAfter(prev);
    prev = op;
  }

  UmlAttribute * att = 
    UmlAttribute::java2Php(cl, cl, "_java_final", "_php_final");
  
  att->moveAfter(cl->get_attribute("_java_synchronized"));
  att->set_CppDecl(att->cppDecl() + "#endif\n");
  
  UmlAttribute::java2Php(cl, cl, "_java_get_set_frozen", "_php_get_set_frozen")
    ->moveAfter(cl->get_attribute("_java_get_set_frozen"));
  
  UmlAttribute::java2Php(cl, cl, "_java_name_spec", "_php_name_spec")
    ->moveAfter(cl->get_attribute("_java_name_spec"));
  
  op = UmlOperation::java2Php(cl, cl, "read_java_", "read_php_");
  op->moveAfter(cl->get_operation("read_java_"));
  op->set_CppBody("  UmlBaseClassMember::read_php_();\n"
		  "  _php_final = UmlCom::read_bool();\n"
		  "  _php_name_spec = UmlCom::read_string();\n"
		  "  _php_get_set_frozen = UmlCom::read_bool();\n");
  op->set_JavaBody("  super.read_php_();\n"
		  "  _php_final = UmlCom.read_bool();\n"
		  "  _php_name_spec = UmlCom.read_string();\n"
		  "  _php_get_set_frozen = UmlCom.read_bool();\n");

  QCString s;
  
  op = cl->get_operation("unload");
  
  s = op->cppBody();
  s.insert(s.find("#ifdef WITHIDL"),
	   "#ifdef WITHPHP\n"
	   "  _php_name_spec = 0;\n"
	   "#endif\n");
  op->set_CppBody(s);
  
  s = op->javaBody();
  s.insert(s.find("  _idl_name_spec = null;"),
	   "  _php_name_spec = null;\n");
  op->set_JavaBody(s);  
}

void package_add_php()
{
  // already root
  UmlClass * cl = UmlClass::get("UmlBasePackage", 0);
  UmlOperation * op1 = 
    UmlOperation::java2Php(cl, cl, "javaDir", "phpDir");
    
  op1->moveAfter(cl->get_operation("findPackage"));
  
  UmlOperation * op2 = 
    UmlOperation::java2Php(cl, cl, "set_JavaDir", "set_PhpDir");
  
  op2->moveAfter(op1);
  op2->set_CppDecl(op2->cppDecl() + "#endif\n");
  op2->set_CppDef(op2->cppDef() + "#endif\n");

  UmlAttribute * att = 
    UmlAttribute::java2Php(cl, cl, "_java_dir", "_php_dir");
  
  att->moveAfter(cl->get_attribute("_java_package"));
  att->set_CppDecl(att->cppDecl() + "#endif\n");
  
  op1 = UmlOperation::java2Php(cl, cl, "read_java_", "read_php_");
  op1->moveAfter(cl->get_operation("read_java_"));
  op1->set_CppBody("  _php_dir = UmlCom::read_string();\n");
  op1->set_JavaBody("  _php_dir = UmlCom.read_string();\n");

  QCString s;
  
  op1 = cl->get_operation("unload");
  
  s = op1->cppBody();
  s.insert(s.find("#ifdef WITHIDL"),
	   "#ifdef WITHPHP\n"
	   "  _php_dir = 0;\n"
	   "#endif\n");
  op1->set_CppBody(s);
  
  s = op1->javaBody();
  s.insert(s.find("  _idl_dir = null;"),
	   "  _php_dir = null;\n");
  op1->set_JavaBody(s);
}

void relation_add_php()
{
  // already root
  UmlClass * cl = UmlClass::get("UmlBaseRelation", 0);
  UmlOperation * op = 
    UmlOperation::java2Php(cl, cl, "read_java_", "read_php_");
  
  op->moveAfter(cl->get_operation("read_java_"));
  
  op->set_CppBody("  UmlBaseClassMember::read_php_();\n");
  op->set_JavaBody("  super.read_php_();\n");
}

void stereotype_add_php()
{
  // already root
  UmlClass * cl = UmlClass::get("UmlStereotype", 0);
  
  UmlAttribute::java2Php(cl, cl, "java", "php")
    ->moveAfter(cl->get_attribute("java"));
}

void associate_php_artifacts(UmlArtifact * phpsettingsart,
			     UmlArtifact * phpsettingscmdart)
{
  UmlDeploymentView * dv = (UmlDeploymentView *)
    UmlClass::get("UmlArtifact", 0)->associatedArtifact()->parent();
  const QVector<UmlItem> ch = dv->children();
    
  for (unsigned i = 0; i != ch.size(); i += 1) {
    if ((ch[i]->kind() == anArtifact) && (ch[i]->name() == "executable")) {
      ((UmlArtifact *) ch[i])->addAssociatedArtifact(phpsettingsart);
      ((UmlArtifact *) ch[i])->addAssociatedArtifact(phpsettingscmdart);
      return;
    }
  }
}

void add_php()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  //

  UmlArtifact * phpsettingsart = add_php_settings();
  UmlArtifact * phpsettingscmdart = add_php_settings_cmd();
  
  add_php_on_enums();
  artifact_add_php();
  attribute_add_php();
  class_add_php();
  classitem_add_php();
  item_add_php();
  operation_add_php();
  package_add_php();
  relation_add_php();
  stereotype_add_php();
  associate_php_artifacts(phpsettingsart, phpsettingscmdart);
  
  //

  UmlCom::set_user_id(uid);
}

//
//
//

void php_javadocstylecomment()
{
  // already root

  UmlCom::trace("<b>update PhpSettings : add javadoc style</b><br>\n");
  
  UmlClass * php_settings = UmlClass::get("PhpSettings", 0);
  UmlAttribute * at =
    php_settings->add_attribute("_is_generate_javadoc_comment", PrivateVisibility,
				"bool",	0, 0);

  at->moveAfter(php_settings->get_attribute("_ext"));
  at->set_isClassMember(TRUE);

  // get
  
  UmlOperation * op = php_settings->add_op("isGenerateJavadocStyleComment", PublicVisibility, "bool");
   
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
  op->moveAfter(php_settings->get_operation("set_SourceExtension"));

  // set
  
  UmlOperation * op2 = 
    php_settings->add_op("set_IsGenerateJavadocStyleComment", PublicVisibility, "bool", TRUE);
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set if ${comment} generate Javadoc style comment\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(phpSettingsCmd, setPhpJavadocStyleCmd, v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _is_generate_javadoc_comment = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.phpSettingsCmd, PhpSettingsCmd._setPhpJavadocStyleCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_generate_javadoc_comment = v;\n",
		FALSE);
  op2->moveAfter(op);  
  
  //

  QCString s;
  
  op = php_settings->get_operation("read_");
  s = op->cppBody() + "  _is_generate_javadoc_comment = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _is_generate_javadoc_comment = UmlCom.read_bool();\n";
  op->set_JavaBody(s);

  //

  UmlClass * stcmd = UmlClass::get("PhpSettingsCmd", 0);
  
  stcmd->add_enum_item("setPhpJavadocStyleCmd");
  
  // stay root
}

//
//
//

void php_add_require_once()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>Add require_once generation mode</b><br>\n");
  
  // attributes
  
  UmlClass * php_settings = UmlClass::get("PhpSettings", 0);
  UmlAttribute * at;
  UmlAttribute * at2;
  UmlOperation * op;
  UmlOperation * op2;
  
  at = php_settings->add_attribute("_req_with_path", PrivateVisibility,
				   "bool", 0, 0);
  at->moveAfter(php_settings->get_attribute("_is_generate_javadoc_comment"));
  at->set_isClassMember(TRUE);
  at2 = at;
  
  at = php_settings->add_attribute("_is_relative_path", PrivateVisibility,
				   "bool", 0, 0);
  at->moveAfter(at2);
  at->set_isClassMember(TRUE);
  at2 = at;
  
  at = php_settings->add_attribute("_is_root_relative_path", PrivateVisibility,
				   "bool", 0, 0);
  at->moveAfter(at2);
  at->set_isClassMember(TRUE);

  // requireOnceWithPath
  
  op = php_settings->add_op("requireOnceWithPath", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" indicates to the code generator if the require_once may specify\n"
		      " the path of just the file's name");
  op->set_cpp("${type}", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _req_with_path;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _req_with_path;\n",
	       FALSE);
  op->moveAfter(php_settings->get_operation("set_SourceExtension"));

  // set_RequireOnceWithPath
  
  op2 = php_settings->add_op("set_RequireOnceWithPath", PublicVisibility, "bool", TRUE);
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" to indicates to the code generator if the require_once may specify\n"
		       " the path of just the file's name\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
	       "  UmlCom::send_cmd(phpSettingsCmd, setPhpRequireOnceWithPathCmd, v);\n"
	       "  if (UmlCom::read_bool()) {\n"
	       "    _req_with_path = v;\n"
	       "    return TRUE;\n"
	       "  }\n"
	       "  else\n"
	       "    return FALSE;\n",
	       FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.phpSettingsCmd, PhpSettingsCmd._setPhpRequireOnceWithPathCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _req_with_path = v;\n",
		FALSE);
  op2->moveAfter(op);  
  
  // isRelativePath
  
  op = php_settings->add_op("isRelativePath", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return if a relative path must be used when the path\n"
		      " must be generated in the produced require_once");
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
  op->moveAfter(op2);

  // set_IsRelativePath
  
  op2 = php_settings->add_op("set_IsRelativePath", PublicVisibility, "bool", TRUE);
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set if a relative path must be used when the path\n"
		       " must be generated in the produced require_once\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
	       "  UmlCom::send_cmd(phpSettingsCmd, setPhpRelativePathCmd, v);\n"
	       "  if (UmlCom::read_bool()) {\n"
	       "    _is_relative_path = v;\n"
	       "    if (v) _is_root_relative_path = FALSE;\n"
	       "    return TRUE;\n"
	       "  }\n"
	       "  else\n"
	       "    return FALSE;\n",
	       FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.phpSettingsCmd, PhpSettingsCmd._setPhpRelativePathCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_relative_path = v;\n"
	       "  if (v) _is_root_relative_path = false;\n",
		FALSE);
  op2->moveAfter(op);  
  
  // isRootRelativePath
  
  op = php_settings->add_op("isRootRelativePath", PublicVisibility, "bool");
   
  op->set_isClassMember(TRUE);
  op->set_Description(" return if a path relative to the project root must be used\n"
		      " when the path must be generated in the produced require_once");
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
  op->moveAfter(op2);

  // set_IsRootRelativePath
  
  op2 = php_settings->add_op("set_IsRootRelativePath", PublicVisibility, "bool", TRUE);
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set if a relative to the project root path must be used\n"
		       " when the path must be generated in the produced require_once\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
	       "  UmlCom::send_cmd(phpSettingsCmd, setPhpRootRelativePathCmd, v);\n"
	       "  if (UmlCom::read_bool()) {\n"
	       "    _is_root_relative_path = v;\n"
	       "    if (v) _is_relative_path = FALSE;\n"
	       "    return TRUE;\n"
	       "  }\n"
	       "  else\n"
	       "    return FALSE;\n",
	       FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.phpSettingsCmd, PhpSettingsCmd._setPhpRootRelativePathCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_root_relative_path = v;\n"
	       "  if (v) _is_relative_path = false;\n",
		FALSE);
  op2->moveAfter(op);  
  
  //

  QCString s;
  
  op = php_settings->get_operation("read_");
  s = op->cppBody() + 
    "  _req_with_path = UmlCom::read_bool();\n"
      "  _is_relative_path = UmlCom::read_bool();\n"
	"  _is_root_relative_path = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() +
    "  _req_with_path = UmlCom.read_bool();\n"
      "  _is_relative_path = UmlCom.read_bool();\n"
	"  _is_root_relative_path = UmlCom.read_bool();\n";
  op->set_JavaBody(s);

  //

  UmlClass * stcmd = UmlClass::get("PhpSettingsCmd", 0);
  
  stcmd->add_enum_item("setPhpRequireOnceWithPathCmd");
  stcmd->add_enum_item("setPhpRelativePathCmd");
  stcmd->add_enum_item("setPhpRootRelativePathCmd");
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_php_namespace(UmlClass * uml_base_package, UmlClass * phpsettings)
{
  UmlCom::trace("<b>Add Php namespace management</b><br>\n");

  //
  // changes on UmlBasePackage
  //
  
  // att
  
  UmlAttribute * att = 
    uml_base_package->add_attribute("_php_namespace", PrivateVisibility,
				    "string", 0, "#endif");
  UmlAttribute * att2 = 
    uml_base_package->get_attribute("_php_dir");
      
  att->moveAfter(att2);
  
  QCString s = att2->cppDecl();
  int index = s.find("\n#endif");
  
  if (index != -1)
    att2->set_CppDecl(s.left(index));
  
  // read

  UmlOperation * op = uml_base_package->get_operation("read_php_");
  
  s = op->cppBody() + "  _php_namespace = UmlCom::read_string();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "  _php_namespace = UmlCom.read_string();\n";
  op->set_JavaBody(s);
  
  // get

  op = uml_base_package->add_op("phpNamespace", PublicVisibility, "string");
  op->set_Description(" return the namespace name associed to the package\n");
  op->set_cpp("const ${type} &", "",
	      "  read_if_needed_();\n"
	      "\n"
	      "  return _php_namespace;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  read_if_needed_();\n"
	       "\n"
	       "  return _php_namespace;\n",
	       FALSE);
  
  UmlOperation * op2 = uml_base_package->get_operation("set_PhpDir");
  
  op->moveAfter(op2);
  
  s = op2->cppDecl();
  index = s.find("\n#endif");
  if (index != -1) op2->set_CppDecl(s.left(index));
  
  s = op2->cppDef();
  index = s.find("\n#endif");
  if (index != -1) op2->set_CppDef(s.left(index));
  
  // set
  
  op2 = uml_base_package->add_op("set_PhpNamespace", PublicVisibility, "bool", TRUE);
  
  op2->add_param(0, InputDirection, "v", "string");
  op2->set_Description(" the namespace name associed to the package.\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  
  op2->set_cpp("${type}", "const ${t0} & ${p0}",
	      "  return set_it_(_php_namespace, v, setPhpNamespaceCmd);\n",
	      FALSE, 0, 0);

  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(identifier_(), OnInstanceCmd.setPhpNamespaceCmd, v);\n"
		"  UmlCom.check();\n"
		"\n"
		"  _php_namespace = v;\n",
	      FALSE);
  
  op2->moveAfter(op);


  UmlClass::get("OnInstanceCmd", 0)->add_enum_item("setPhpNamespaceCmd");
  
  // find
  
  UmlClass * pack = UmlClass::get("UmlPackage", 0);
  
  op = uml_base_package->add_op("findPhpNamespace", PublicVisibility, pack);
  op->set_Description(" returns a sub package of the current one having the php namespace 'n'\n"
		      " (including the current one), else 0/null\n");
  op->add_param(0, InputDirection, "n", "string");
  op->set_cpp("${type} *", "const ${t0} & ${p0}",
	      "  UmlCom::send_cmd(packageGlobalCmd, findPhpNamespaceCmd, _identifier, n);\n"
	      "\n"
	      "  return (UmlPackage *) UmlBaseItem::read_();\n",
	      FALSE, 0, "#endif");
  op->set_java("${type}", "${t0} ${p0}",
	       "  UmlCom.send_cmd(CmdFamily.packageGlobalCmd, PackageGlobalCmd._findPhpNamespaceCmd, identifier_(), n);\n"
	       "  return (UmlPackage) UmlBaseItem.read_();\n",
	       FALSE);
  
  op->moveAfter(op2);  
  
  //
  
  UmlClass::get("PackageGlobalCmd", 0)->add_enum_item("findPhpNamespaceCmd");
  
  //
  // changes on PhpSettings
  //
  
  // read

  op = phpsettings->get_operation("read_");
  s = op->cppBody() + "\n  _is_force_namespace_gen = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  s = op->javaBody() + "\n  _is_force_namespace_gen = UmlCom.read_bool();\n";
  op->set_JavaBody(s);
  
  // att
  
  att = phpsettings->add_attribute("_is_force_namespace_gen", PrivateVisibility, "bool", 0, 0);

  att->set_isClassMember(TRUE);
  att->moveAfter(phpsettings->get_attribute("_is_root_relative_path"));

  // get
  
  op = phpsettings->add_op("isForceNamespacePrefixGeneration", PublicVisibility, "bool");
   
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
  op->moveAfter(phpsettings->get_operation("set_IsRootRelativePath"));

  // set
  
  op2 = phpsettings->add_op("set_IsForceNamespacePrefixGeneration", PublicVisibility, "bool", TRUE);
  
  op2->set_isClassMember(TRUE);
  op2->set_Description(" set if the namespace prefix must be always generated before class's names\n"
		       "\n"
		       " On error : return FALSE in C++, produce a RuntimeException in Java");
  op2->add_param(0, InputDirection, "v", "bool"); 
  op2->set_cpp("${type}", "${t0} ${p0}",
		"  UmlCom::send_cmd(phpSettingsCmd, setPhpForceNamespaceGenCmd, v);\n"
		"  if (UmlCom::read_bool()) {\n"
		"    _is_force_namespace_gen = v;\n"
		"    return TRUE;\n"
		"  }\n"
		"  else\n"
		"    return FALSE;\n",
		FALSE, 0, 0);
  op2->set_java("void", "${t0} ${p0}",
		"  UmlCom.send_cmd(CmdFamily.phpSettingsCmd, PhpSettingsCmd._setPhpForceNamespaceGenCmd,\n"
		"		   (v) ? (byte) 1 : (byte) 0);\n"
		"  UmlCom.check();\n"
		"  _is_force_namespace_gen = v;\n",
		FALSE);
  op2->moveAfter(op);
  
  //

  UmlClass * phpsettingscmd = UmlClass::get("PhpSettingsCmd", 0);
  
  phpsettingscmd->add_enum_item("setPhpForceNamespaceGenCmd");
}

