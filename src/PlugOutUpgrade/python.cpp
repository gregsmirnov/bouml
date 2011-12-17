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
#include "python.h"


void add_bypass_python()
{
  // already root
  UmlClass * cl;
  UmlAttribute * att;
  UmlOperation * op;
  QString s;
  
  //
  
  cl = UmlClass::get("UmlBaseArtifact", 0);
  
  att = cl->add_attribute("_python_src", PrivateVisibility, "string",
			  "WITHPYTHON", "endif");
  att->moveAfter(cl->get_attribute("_php_src"));
  
  op = cl->add_op("read_python_", ProtectedVisibility, "void");
  op->set_isCppVirtual(TRUE);
  op->set_cpp("${type}", "", "\
  _python_src = UmlCom::read_string();\n",
	      FALSE, "WITHPYTHON", "endif");
  op->set_java("${type}", "", "\
  _python_src = UmlCom.read_string();\n",
	       FALSE);
  op->moveAfter(cl->get_operation("read_php_"));
  op->set_Description("internal, do NOT use it\n");
  
  op = cl->get_operation("unload");
  s = op->cppBody();
  s.insert(s.find("#ifdef WITHIDL"),
	   "#ifdef WITHPYTHON\n"
	   "  _python_src = 0;\n"
	   "#endif\n");
  op->set_CppBody(s);
  s = op->javaBody();
  s.insert(s.find("  _idl_src = null;"),
	   "  _python_src = null;\n");
  op->set_JavaBody(s);
  
  //
  
  cl = UmlClass::get("UmlBaseAttribute", 0);

  op = cl->add_op("read_python_", ProtectedVisibility, "void");
  op->set_isCppVirtual(TRUE);
  op->set_cpp("${type}", "", "\
  UmlBaseClassMember::read_python_();\n",
	      FALSE, "WITHPYTHON", "endif");
  op->set_java("${type}", "", "\
  super.read_python_();\n",
	       FALSE);
  op->moveAfter(cl->get_operation("read_php_"));
  op->set_Description("internal, do NOT use it\n");
  
  //
  
  cl = UmlClass::get("UmlBaseClassItem", 0);
  
  att = cl->add_attribute("_python_decl", PrivateVisibility, "string",
			  "WITHPYTHON", "endif");
  att->moveAfter(cl->get_attribute("_php_decl"));
  
  op = cl->add_op("read_python_", ProtectedVisibility, "void");
  op->set_isCppVirtual(TRUE);
  op->set_cpp("${type}", "", "\
  _python_decl = UmlCom::read_string();\n",
	      FALSE, "WITHPYTHON", "endif");
  op->set_java("${type}", "", "\
  _python_decl = UmlCom.read_string();\n",
	       FALSE);
  op->moveAfter(cl->get_operation("read_php_"));
  op->set_Description("internal, do NOT use it\n");
  
  op = cl->get_operation("unload");
  s = op->cppBody();
  s.insert(s.find("#ifdef WITHIDL"),
	   "#ifdef WITHPYTHON\n"
	   "  _python_decl = 0;\n"
	   "#endif\n");
  op->set_CppBody(s);
  s = op->javaBody();
  s.insert(s.find("  _idl_decl = null;"),
	   "  _python_decl = null;\n");
  op->set_JavaBody(s);
  
  //
  
  cl = UmlClass::get("UmlBaseClass", 0);
  
  att = cl->add_attribute("_python_external", PrivateVisibility, "bool",
			  "WITHPYTHON", 0, " : 1");
  att->moveAfter(cl->get_attribute("_php_final"));
  
  UmlAttribute * att2;
  
  att2 = cl->add_attribute("_python_2_2", PrivateVisibility, "bool",
			   0, "endif", " : 1");
  att2->moveAfter(att);
  
  op = cl->add_op("read_python_", ProtectedVisibility, "void");
  op->set_isCppVirtual(TRUE);
  op->set_cpp("${type}", "", "\
  UmlBaseClassMember::read_python_();\n\
  _python_2_2 = UmlCom::read_bool();\n\
  _python_external = UmlCom::read_bool();\n",
	      FALSE, "WITHPYTHON", "endif");
  op->set_java("${type}", "", "\
  super.read_python_();\n\
  _python_2_2 = UmlCom.read_bool();\n\
  _python_external = UmlCom.read_bool();\n",
	       FALSE);
  op->moveAfter(cl->get_operation("read_php_"));
  op->set_Description("internal, do NOT use it\n");

  //
  
  int index;
  
  cl = UmlClass::get("UmlBaseOperation", 0);
  
  att = cl->get_attribute("_java_get_set_frozen");
  s = att->cppDecl();
  index = s.find("CPP");
  if (index != -1)
    att->set_CppDecl(s.replace(index, 3, "JAVA"));
  
  att = cl->get_attribute("_php_get_set_frozen");
  s = att->cppDecl();
  index = s.find("CPP");
  if (index != -1)
    att->set_CppDecl(s.replace(index, 3, "PHP"));
  
  att = cl->get_attribute("_idl_get_set_frozen");
  s = att->cppDecl();
  index = s.find("CPP");
  if (index != -1)
    att->set_CppDecl(s.replace(index, 3, "IDL"));
  
  att = cl->add_attribute("_python_get_set_frozen", PrivateVisibility, "bool",
			  "WITHPYTHON", "endif", " : 1");
  att->moveAfter(cl->get_attribute("_php_get_set_frozen"));
  
  att = cl->add_attribute("_python_contextual_body_indent", PrivateVisibility, "bool",
			  "WITHPYTHON", "endif", " : 1");
  att->moveAfter(cl->get_attribute("_php_contextual_body_indent"));
  
  att = cl->add_attribute("_python_name_spec", PrivateVisibility, "string",
			  "WITHPYTHON", 0);
  att->moveAfter(cl->get_attribute("_php_name_spec"));
  
  att2 = cl->add_attribute("_python_decorators", PrivateVisibility, "string",
			   0, "endif");
  att2->moveAfter(att);
  
  op = cl->get_operation("phpContextualBodyIndent");
  s = op->cppDef();
  index = s.find("IDL");
  if (index != -1)
    op->set_CppDef(s.replace(index, 3, "PHP"));
  s = op->cppDecl();
  index = s.find("IDL");
  if (index != -1)
    op->set_CppDecl(s.replace(index, 3, "PHP"));
  s = op->cppBody();
  index = s.find("idl");
  if (index != -1)
    op->set_CppBody(s.replace(index, 3, "php"));
  s = op->javaBody();
  index = s.find("idl");
  if (index != -1)
    op->set_JavaBody(s.replace(index, 3, "php"));

  
  op = cl->add_op("read_python_", ProtectedVisibility, "void");
  op->set_isCppVirtual(TRUE);
  op->set_cpp("${type}", "", "\
  UmlBaseClassMember::read_python_();\n\
  _python_decorators = UmlCom::read_string();\n\
  _python_name_spec = UmlCom::read_string();\n\
  _python_get_set_frozen = UmlCom::read_bool();\n\
  _python_contextual_body_indent = UmlCom::read_bool();\n",
	      FALSE, "WITHPYTHON", "endif");
  op->set_java("${type}", "", "\
  super.read_python_();\n\
  _python_decorators = UmlCom.read_string();\n\
  _python_name_spec = UmlCom.read_string();\n\
  _python_get_set_frozen = UmlCom.read_bool();\n\
  _python_contextual_body_indent = UmlCom.read_bool();\n",
	       FALSE);
  op->moveAfter(cl->get_operation("read_php_"));
  op->set_Description("internal, do NOT use it\n");
  
  op = cl->get_operation("unload");
  s = op->cppBody();
  s.insert(s.find("#ifdef WITHIDL"),
	   "#ifdef WITHPYTHON\n"
	   "  _python_name_spec = 0;\n"
	   "  _python_decorators = 0;\n"
	   "#endif\n");
  op->set_CppBody(s);
  s = op->javaBody();
  s.insert(s.find("  _idl_name_spec = null;"),
	   "  _python_name_spec = null;\n"
	   "  _python_decorators = null;\n");
  op->set_JavaBody(s);
  
  //
  
  cl = UmlClass::get("UmlBasePackage", 0);
  
  att = cl->add_attribute("_python_dir", PrivateVisibility, "string",
			  "WITHPYTHON", 0);
  att->moveAfter(cl->get_attribute("_php_dir"));
  
  att2 = cl->add_attribute("_python_package", PrivateVisibility, "string",
			   0, "endif");
  att2->moveAfter(att);
  
  op = cl->add_op("read_python_", ProtectedVisibility, "void");
  op->set_isCppVirtual(TRUE);
  op->set_cpp("${type}", "", "\
  _python_dir = UmlCom::read_string();\n\
  _python_package = UmlCom::read_string();\n",
	      FALSE, "WITHPYTHON", "endif");
  op->set_java("${type}", "", "\
  _python_dir = UmlCom.read_string();\n\
  _python_package = UmlCom.read_string();\n",
	       FALSE);
  op->moveAfter(cl->get_operation("read_php_"));
  op->set_Description("internal, do NOT use it\n");
  
  op = cl->get_operation("unload");
  s = op->cppBody();
  s.insert(s.find("#ifdef WITHIDL"),
	   "#ifdef WITHPYTHON\n"
	   "  _python_dir = 0;\n"
	   "  _python_package = 0;\n"
	   "#endif\n");
  op->set_CppBody(s);
  s = op->javaBody();
  s.insert(s.find("  _idl_dir = null;"),
	   "  _python_dir = null;\n"
	   "  _python_package = null;\n");
  op->set_JavaBody(s);
  
  //
  
  cl = UmlClass::get("UmlBaseRelation", 0);
  
  op = cl->add_op("read_python_", ProtectedVisibility, "void");
  op->set_isCppVirtual(TRUE);
  op->set_cpp("${type}", "", "\
  UmlBaseClassMember::read_python_();\n",
	      FALSE, "WITHPYTHON", "endif");
  op->set_java("${type}", "", "\
  super.read_python_();\n",
	       FALSE);
  op->moveAfter(cl->get_operation("read_php_"));
  op->set_Description("internal, do NOT use it\n");
  
  //
  
  cl = UmlClass::get("UmlBaseItem", 0);
  
  op = cl->add_op("read_python_", ProtectedVisibility, "void");
  op->set_isCppVirtual(TRUE);
  op->set_cpp("${type}", "", "",
	      FALSE, "WITHPYTHON", "endif");
  op->set_java("${type}", "", "",
	       FALSE);
  op->moveAfter(cl->get_operation("read_php_"));
  op->set_Description("internal, do NOT use it\n");
  
  op = cl->get_operation("read_if_needed_");
  op->set_CppBody("  if (!_defined) {\n"
		  "#if defined(WITHCPP) & defined(WITHJAVA) & defined(WITHPHP) & defined(WITHPYTHON) & defined(WITHIDL)\n"
		  "    UmlCom::send_cmd(_identifier, getDefCmd);\n"
		  "    read_uml_();\n"
		  "    read_cpp_();\n"
		  "    read_java_();\n"
		  "    read_php_();\n"
		  "    read_python_();\n"
		  "    read_idl_();\n"
		  "#else\n"
		  "# if defined(WITHCPP) & !defined(WITHJAVA) & !defined(WITHPHP) & !defined(WITHPYTHON) & !defined(WITHIDL)\n"
		  "    UmlCom::send_cmd(_identifier, getCppDefCmd);\n"
		  "    read_uml_();\n"
		  "    read_cpp_();\n"
		  "# else\n"
		  "#  if !defined(WITHCPP) & defined(WITHJAVA) & !defined(WITHPHP) & !defined(WITHPYTHON) & !defined(WITHIDL)\n"
		  "    UmlCom::send_cmd(_identifier, getJavaDefCmd);\n"
		  "    read_uml_();\n"
		  "    read_java_();\n"
		  "#  else\n"
		  "#   if !defined(WITHCPP) & !defined(WITHJAVA) & defined(WITHPHP) & !defined(WITHPYTHON) & !defined(WITHIDL)\n"
		  "    UmlCom::send_cmd(_identifier, getPhpDefCmd);\n"
		  "    read_uml_();\n"
		  "    read_php_();\n"
		  "#   else\n"
		  "#    if !defined(WITHCPP) & !defined(WITHJAVA) & !defined(WITHPHP) & defined(WITHPYTHON) & !defined(WITHIDL)\n"
		  "    UmlCom::send_cmd(_identifier, getPythonDefCmd);\n"
		  "    read_uml_();\n"
		  "    read_python_();\n"
		  "#    else\n"
		  "#     if !defined(WITHCPP) & !defined(WITHJAVA) & !defined(WITHPHP) & !defined(WITHPYTHON) & defined(WITHIDL)\n"
		  "    UmlCom::send_cmd(_identifier, getIdlDefCmd);\n"
		  "    read_uml_();\n"
		  "    read_idl_();\n"
		  "#     else\n"
		  "#      if !defined(WITHCPP) & !defined(WITHJAVA) & !defined(WITHPHP) & !defined(WITHPYTHON) & !defined(WITHIDL)\n"
		  "    UmlCom::send_cmd(_identifier, getUmlDefCmd);\n"
		  "    read_uml_();\n"
		  "#      else\n"
		  "    ... WITHCPP and WITHJAVA and WITHPHP and WITHPYTHON and WITHIDL must be both defined or undefined\n"
		  "    ... or only one of them must be defined\n"
		  "#      endif\n"
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
		   "    read_python_();\n"
		   "    read_idl_();\n"
		   "    \n"
		   "    _defined = true;\n"
		   "  }\n");

  // stay root
}

//
//
//

UmlArtifact * add_python_settings()
{
  // already root
  UmlClass * umlsettings = UmlClass::get("UmlSettings", 0);
  UmlClass * cppsettings = UmlClass::get("CppSettings", 0);
  UmlClass * javasettings = UmlClass::get("JavaSettings", 0);
  UmlClass * phpsettings = UmlClass::get("PhpSettings", 0);
  UmlArtifact * base_art =
    UmlArtifact::made((UmlDeploymentView *)
		      umlsettings->associatedArtifact()->parent(),
		      "PythonSettings");
  
  base_art->moveAfter(phpsettings->associatedArtifact());
  base_art->add_import("import java.util.*;\n");

  
  base_art->set_CppHeader("#ifndef _${NAME}_H\n"
			  "#define _${NAME}_H\n"
			  "\n"
			  "#ifdef WITHPYTHON\n"
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

  base_art->set_CppSource("#ifdef WITHPYTHON\n"
			  "\n"
			  "${comment}\n"
			  "${includes}\n"
			  "#include \"UmlCom.h\"\n"
			  "#include \"PythonSettingsCmd.h\"\n"
			  "#include \"UmlStereotype.h\"\n"
			  "${namespace_start}\n"
			  "${members}\n"
			  "${namespace_end}\n"
			  "\n"
			  "#endif\n");
  
  UmlClass * pythonsettings = UmlClass::create(umlsettings->parent(), "PythonSettings");
  
  pythonsettings->moveAfter(phpsettings);
  
  base_art->addAssociatedClass(pythonsettings);
  pythonsettings->set_CppDecl(CppSettings::classDecl());
  pythonsettings->set_JavaDecl(JavaSettings::classDecl());
  pythonsettings->set_Description(" This class manages settings concerning PYTHON, configured through\n"
			       " the 'Generation settings' dialog.\n"
			       "\n"
			       " This class may be defined as a 'singleton', but I prefer to use static\n"
			       " members allowing to just write 'PythonSettings::member' rather than\n"
			       " 'PythonSettings::instance()->member' or other long sentence like this.");

  UmlOperation * op;
  QCString s;
  
  defGetBool(pythonsettings, _2_2, isPython_2_2, 0, 0, "");
  op->set_Description(" return if classes follow Python 2.2 by default");
  op->set_isClassMember(TRUE);
  
  op = pythonsettings->add_op("set_IsPython_2_2", PublicVisibility, "bool", FALSE);
  op->set_Description(" set if classes follow Python 2.2 by default\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  op->set_isClassMember(TRUE);
  op->add_param(0, InputDirection, "y", "bool");
  op->set_cpp("${type}", 
	      "${t0} ${p0}",
	      "  UmlCom::send_cmd(pythonSettingsCmd, setPython22Cmd, (char) y);\n"
	      "  if (UmlCom::read_bool()) {\n"
	      "    _2_2 = y;\n"
	      "    return TRUE;\n"
	      "  }\n"
	      "  else\n"
	      "    return FALSE;\n"
	      , FALSE, 0, 0);
  op->set_java("void",
	       "${t0} ${p0}",
	       "  UmlCom.send_cmd(CmdFamily.pythonSettingsCmd, PythonSettingsCmd._setPython22Cmd,\n"
	       "		   (y) ? (byte) 1 : (byte) 0);\n"
	       "  UmlCom.check();\n"
	       "  _2_2 = y;\n"
	       , FALSE);
  
  defGet(pythonsettings, _indent_step, indentStep, "string", 0, 0,
	 " default indent step");
  op->set_isClassMember(TRUE);
  
  op = pythonsettings->add_op("set_IndentStep", PublicVisibility, "bool", FALSE);
  op->set_Description(" set default indent step\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  op->set_isClassMember(TRUE);
  op->add_param(0, InputDirection, "v", "str");
  op->set_cpp("${type}", 
	      "${t0} ${p0}",
	      "  UmlCom::send_cmd(pythonSettingsCmd, setPythonIndentStepCmd, v);\n"
	      "  if (UmlCom::read_bool()) {\n"
	      "    _indent_step = v;\n"
	      "    return TRUE;\n"
	      "  }\n"
	      "  else\n"
	      "    return FALSE;\n"
	      , FALSE, 0, 0);
  op->set_java("void",
	       "${t0} ${p0}",
	       "  UmlCom.send_cmd(CmdFamily.pythonSettingsCmd, PythonSettingsCmd._setPythonIndentStepCmd, v);\n"
	       "  UmlCom.check();\n"
	       "  _indent_step = v;\n"
	       , FALSE);
    
  int index;

  {
    const char * opers[] = {
      "useDefaults", "set_UseDefaults",
      "relationAttributeStereotype", "set_RelationAttributeStereotype",
      "relationAttributeUmlStereotype",
      "classStereotype", "set_ClassStereotype",
      "classUmlStereotype",
      "get_import", "set_Import",
      "rootDir", "set_RootDir",
      "sourceContent", "set_SourceContent",
      "sourceExtension", "set_SourceExtension",
      "classDecl", "set_ClassDecl",
      "externalClassDecl", "set_ExternalClassDecl",
      "enumDecl", "set_EnumDecl"
    };
    
    for (index = 0; index != sizeof(opers)/sizeof(opers[0]); index += 1)
      UmlOperation::java2Python(pythonsettings, javasettings, opers[index]);
  }
  
  op = UmlOperation::java2Python(pythonsettings, javasettings, "attributeDecl");
  s = op->cppBody();
  s.replace(s.find("UmlSettings::multiplicity"), 25, "mult");
  op->set_CppBody(s);
  s = op->javaBody();
  s.replace(s.find("UmlSettings.multiplicity"), 24, "mult");
  op->set_JavaBody(s);
  
  op = UmlOperation::java2Python(pythonsettings, javasettings, "set_AttributeDecl");
  s = op->cppBody();
  s.replace(s.find("UmlSettings::multiplicity"), 25, "mult");
  op->set_CppBody(s);
  s = op->javaBody();
  s.replace(s.find("UmlSettings.multiplicity"), 24, "mult");
  op->set_JavaBody(s);
  
  {
    const char * opers[] = {
      "enumItemDecl", "set_EnumItemDecl"
    };
    
    for (index = 0; index != sizeof(opers)/sizeof(opers[0]); index += 1)
      UmlOperation::java2Python(pythonsettings, javasettings, opers[index]);
  }

  op = UmlOperation::cpp2Python(pythonsettings, cppsettings, "relationDecl");
  s = op->cppBody();
  s.replace(s.find("UmlSettings::multiplicity"), 25, "mult");
  op->set_CppBody(s);
  s = op->javaBody();
  s.replace(s.find("UmlSettings.multiplicity"), 24, "mult");
  op->set_JavaBody(s);
  
  op = UmlOperation::cpp2Python(pythonsettings, cppsettings, "set_RelationDecl");
  s = op->cppBody();
  s.replace(s.find("UmlSettings::multiplicity"), 25, "mult");
  op->set_CppBody(s);
  s = op->javaBody();
  s.replace(s.find("UmlSettings.multiplicity"), 24, "mult");
  op->set_JavaBody(s);

  {
    const char * opers[] = {
      "operationDef", "set_OperationDef",
      "getName", "set_GetName",
      "setName", "set_SetName"
    };
    
    for (index = 0; index != sizeof(opers)/sizeof(opers[0]); index += 1)
      UmlOperation::java2Python(pythonsettings, javasettings, opers[index]);
  }
  
  UmlRelation * rel;
  
  if ((rel = UmlBaseRelation::create(aGeneralisation, pythonsettings, umlsettings)) == 0) {
    QCString msg = "PythonSettings can't inherit UmlSettings<br>\n";
    
    UmlCom::trace("<b>" + msg + "</b>");
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
  
  UmlOperation::java2Python(pythonsettings, javasettings, "JavaSettings", "PythonSettings");
  
  op = pythonsettings->add_op("mult_column", PrivateVisibility, "unsigned", FALSE);
  op->set_isClassMember(TRUE);
  op->add_param(0, InputDirection, "mult", "string");
  op->set_cpp("${type}", 
	      "const ${t0} & ${p0}",
	      "  return (mult.isEmpty() || (mult == \"1\")) ? 0 : 1;\n",
	      FALSE, 0, 0);

  op->set_java("${type}",
	       "${t0} ${p0}",
	       "  return ((mult == \"\") || (mult == \"1\")) ? 0 : 1;\n",
	       FALSE);
  
  //

  UmlAttribute::java2Python(pythonsettings, javasettings, "_defined");
  pythonsettings->add_attribute("_2_2", PrivateVisibility, "bool", 0, 0)
    ->set_isClassMember(TRUE);
  pythonsettings->add_attribute("_indent_step", PrivateVisibility, "string", 0, 0)
    ->set_isClassMember(TRUE);
  
  {
    const char * attrs[] = {
      "_root", 
      "_class_decl", 
      "_external_class_decl", 
      "_enum_decl"
    };
    
    for (index = 0; index != sizeof(attrs)/sizeof(attrs[0]); index += 1)
      UmlAttribute::cpp2Python(pythonsettings, cppsettings, attrs[index]);
  }

  UmlAttribute * at;
  
  at = UmlAttribute::cpp2Python(pythonsettings, cppsettings, "_attr_decl");
  s = at->cppDecl();
  s.replace(s.find("3"), 1, "2");
  at->set_CppDecl(s);
  
  UmlAttribute::cpp2Python(pythonsettings, cppsettings, "_enum_item_decl");
  
  at = UmlAttribute::cpp2Python(pythonsettings, cppsettings, "_rel_decl");
  s = at->cppDecl();
  s.replace(s.find("3"), 1, "2");
  at->set_CppDecl(s);
  
  UmlAttribute::cpp2Python(pythonsettings, cppsettings, "_oper_def");
  UmlAttribute::cpp2Python(pythonsettings, cppsettings, "_get_name");
  
  {
    const char * attrs[] = {
      "_set_name", 
      "_src_content", 
      "_ext"
    };
    
    for (index = 0; index != sizeof(attrs)/sizeof(attrs[0]); index += 1)
      UmlAttribute::java2Python(pythonsettings, javasettings, attrs[index]);
  }
  
  UmlAttribute::cpp2Python(pythonsettings, cppsettings, "_map_includes")
    ->set_Name("_map_imports");
    
  op = pythonsettings->add_op("read_", ProtectedVisibility, "void");
  op->set_isClassMember(TRUE);
  op->set_cpp("${type}", "",
	      "  _2_2 = UmlCom::read_bool();\n"
	      "  \n"
	      "  _indent_step = UmlCom::read_string();\n"
	      "  \n"
	      "  _root = UmlCom::read_string();\n"
	      "  \n"
	      "  unsigned n;\n"
	      "  unsigned index;\n"
	      "  \n"
	      "  n = UmlCom::read_unsigned();\n"
	      "  \n"
	      "  for (index = 0; index != n; index += 1)\n"
	      "    UmlSettings::_relation_attribute_stereotypes[index].python = UmlCom::read_string();\n"
	      "  \n"
	      "  n = UmlCom::read_unsigned();\n"
	      "  \n"
	      "  for (index = 0; index != n; index += 1)\n"
	      "    UmlSettings::_class_stereotypes[index].python = UmlCom::read_string();\n"
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
	      "  \n"
	      "  _src_content = UmlCom::read_string();\n"
	      "  _ext = UmlCom::read_string();\n"
	      "\n"
	      "  _class_decl = UmlCom::read_string();\n"
	      "  _external_class_decl = UmlCom::read_string();\n"
	      "  _enum_decl = UmlCom::read_string();\n"
	      "  _attr_decl[0] = UmlCom::read_string();\n"
	      "  _attr_decl[0] = UmlCom::read_string();\n"
	      "  _enum_item_decl = UmlCom::read_string();\n"
	      "  _rel_decl[0][0] = UmlCom::read_string();\n"
	      "  _rel_decl[0][1] = UmlCom::read_string();\n"
	      "  _rel_decl[1][0] = UmlCom::read_string();\n"
	      "  _rel_decl[1][1] = UmlCom::read_string();\n"
	      "  _oper_def = UmlCom::read_string();\n"
	      "  _get_name = UmlCom::read_string();\n"
	      "  _set_name = UmlCom::read_string();\n"
	      ,FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  _2_2 = UmlCom.read_bool();\n"
	       "  \n"
	       "  _indent_step = UmlCom.read_string();\n"
	       "  \n"
	       "  _root = UmlCom.read_string();\n"
	       "  \n"
	       "  int n;\n"
	       "  int index;\n"
	       "  \n"
	       "  n = UmlCom.read_unsigned();\n"
	       "  \n"
	       "  for (index = 0; index != n; index += 1)\n"
	       "    UmlSettings._relation_attribute_stereotypes[index].python = UmlCom.read_string();\n"
	       "  \n"
	       "  n = UmlCom.read_unsigned();\n"
	       "  \n"
	       "  for (index = 0; index != n; index += 1)\n"
	       "    UmlSettings._class_stereotypes[index].python = UmlCom.read_string();\n"
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
	       "  \n"
	       "  _src_content = UmlCom.read_string();\n"
	       "  _ext = UmlCom.read_string();\n"
	       "\n"
	       "  _class_decl = UmlCom.read_string();\n"
	       "  _external_class_decl = UmlCom.read_string();\n"
	       "  _enum_decl = UmlCom.read_string();\n"
	       "  _attr_decl = new String[2];\n"
	       "  _attr_decl[0] = UmlCom.read_string();\n"
	       "  _attr_decl[0] = UmlCom.read_string();\n"
	       "  _enum_item_decl = UmlCom.read_string();\n"
	       "  _rel_decl = new String[2][2];\n"
	       "  _rel_decl[0][0] = UmlCom.read_string();\n"
	       "  _rel_decl[0][1] = UmlCom.read_string();\n"
	       "  _rel_decl[1][0] = UmlCom.read_string();\n"
	       "  _rel_decl[1][1] = UmlCom.read_string();\n"
	       "  _oper_def = UmlCom.read_string();\n"
	       "  _get_name = UmlCom.read_string();\n"
	       "  _set_name = UmlCom.read_string();\n"
	       ,FALSE);
  
  op = pythonsettings->add_op("read_if_needed_", ProtectedVisibility, "void");
  op->set_isClassMember(TRUE);
  op->set_cpp("${type}", "",
	      "  UmlSettings::read_if_needed_();\n"
	      "  if (!_defined) {\n"
	      "    UmlCom::send_cmd(pythonSettingsCmd, getPythonSettingsCmd);\n"
	      "    read_();\n"
	      "    _defined = TRUE;\n"
	      "  }\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  UmlSettings.read_if_needed_();\n"
	       "  if (!_defined) {\n"
	       "    UmlCom.send_cmd(CmdFamily.pythonSettingsCmd, PythonSettingsCmd._getPythonSettingsCmd);\n"
	       "    read_();\n"
	       "    _defined = true;\n"
	       "  }\n",
	       FALSE);
  
  return base_art;
}

UmlArtifact *  add_python_settings_cmd()
{
  // already root
  UmlClass * phpsettingscmd = UmlClass::get("PhpSettingsCmd", 0);
  UmlArtifact * base_art =
    UmlArtifact::made((UmlDeploymentView *)
		      phpsettingscmd->associatedArtifact()->parent(),
		      "PythonSettingsCmd");
  
  base_art->moveAfter(phpsettingscmd->associatedArtifact());
  
  UmlClass * pythonsettingscmd =
    UmlClass::create(phpsettingscmd->parent(), "PythonSettingsCmd");
  
  pythonsettingscmd->moveAfter(phpsettingscmd);
  
  base_art->addAssociatedClass(pythonsettingscmd);
  pythonsettingscmd->set_Stereotype(phpsettingscmd->stereotype());
  pythonsettingscmd->set_CppDecl(phpsettingscmd->cppDecl());
  pythonsettingscmd->set_JavaDecl(phpsettingscmd->javaDecl());

  unsigned index;
  const char * items[] = {
    "getPythonSettingsCmd",
    "getPythonUseDefaultsCmd",
  
    "setPythonUseDefaultsCmd",
    "setPython22Cmd",
    "setPythonIndentStepCmd",
    "setPythonRelationAttributeStereotypeCmd",
    "setPythonClassStereotypeCmd",
    "setPythonImportCmd",
    "setPythonRootdirCmd",
    "setPythonSourceContentCmd",
    "setPythonSourceExtensionCmd",
    "setPythonClassDeclCmd",
    "setPythonEnumDeclCmd",
    "setPythonExternalClassDeclCmd",
    "setPythonInterfaceDeclCmd",
    "setPythonAttributeDeclCmd",
    "setPythonEnumItemDeclCmd",
    "setPythonRelationDeclCmd",
    "setPythonOperationDefCmd",
    "setPythonGetNameCmd",
    "setPythonSetNameCmd"
  };
  
  for (index = 0; index != sizeof(items)/sizeof(items[0]); index += 1)
    pythonsettingscmd->add_enum_item(items[index]);
  
  UmlClass::get("CmdFamily", 0)->add_enum_item("pythonSettingsCmd");
  
  return base_art;
}

void add_python_on_enums()
{
  // already root
  UmlClass * itcmd = UmlClass::get("OnInstanceCmd", 0);
  UmlAttribute * at = itcmd->add_enum_item("getPythonDefCmd");
  
  at->moveAfter(itcmd->get_attribute("phpBodyCmd"));
  itcmd->add_enum_item("pythonBodyCmd")->moveAfter(at);
  
  //
  
  UmlClass::get("aLanguage", 0)->add_enum_item("pythonLanguage");
}

void artifact_add_python()
{
  // already root
  UmlClass * cl = UmlClass::get("UmlBaseArtifact", 0);
  
  UmlOperation * op =
    UmlOperation::java2Python(cl, cl, "javaSource", "pythonSource");
  
  op->moveAfter(cl->get_operation("set_PhpSource"));
  UmlOperation::java2Python(cl, cl, "set_JavaSource", "set_PythonSource")
    ->moveAfter(op);
}

void class_add_python()
{
  // already root
  UmlClass * cl = UmlClass::get("UmlBaseClass", 0);
  UmlOperation * prev = cl->get_operation("set_isPhpFinal");
  int index;
  const struct {
    const char * java;
    const char * python;
  } opers[] = {
    { "isJavaExternal", "isPythonExternal" },
    { "set_isJavaExternal", "set_isPythonExternal" },
  };
  UmlOperation * op;
  
  for (index = 0; index != sizeof(opers)/sizeof(opers[0]); index += 1) {
    op = UmlOperation::java2Python(cl, cl, opers[index].java, opers[index].python);
    op->moveAfter(prev);
    prev = op;
  }

  defGetBool(cl, _python_2_2, isPython_2_2, 0, 0, "");
  op->set_Description(" returns TRUE is the class is a Python 2.2 class");
  op->moveAfter(prev);
  prev = op;
  defSetBoolBitField(cl, _python_2_2, set_isPython_2_2, setIsPython2_2Cmd, 0, "endif",
		     " set if the class is a Python 2.2 class");
  op->moveAfter(prev);
}

void classitem_add_python()
{
  // already root
  UmlClass * cl = UmlClass::get("UmlBaseClassItem", 0);
  UmlOperation * op = 
    UmlOperation::java2Python(cl, cl, "javaDecl", "pythonDecl");
    
  op->moveAfter(cl->get_operation("set_PhpDecl"));
  
  UmlOperation::java2Python(cl, cl, "set_JavaDecl", "set_PythonDecl")
    ->moveAfter(op);
}

void operation_add_python()
{
  // already root
  UmlClass * cl = UmlClass::get("UmlBaseOperation", 0);
  UmlOperation * prev = cl->get_operation("set_PhpContextualBodyIndent");
  UmlOperation * op;
  QCString s;
  int index;
  
  // fixing
  op = cl->get_operation("phpContextualBodyIndent");
  s = op->cppBody();
  index = s.find("_php_get_set_frozen");
  if (index != -1)
    op->set_CppBody(s.replace(index, 19, "_php_contextual_body_indent"));
  s = op->javaBody();
  index = s.find("_php_get_set_frozen");
  if (index != -1)
    op->set_JavaBody(s.replace(index, 19, "_php_contextual_body_indent"));
 
  const struct {
    const char * java;
    const char * python;
  } opers[] = {
    { "javaDef", "pythonDef" },
    { "set_JavaDef", "set_PythonDef" },
    { "javaBody", "pythonBody" },
    { "set_JavaBody", "set_PythonBody" },
    { "javaNameSpec", "pythonNameSpec" },
    { "set_JavaNameSpec", "set_PythonNameSpec" },
    { "javaGetSetFrozen", "pythonGetSetFrozen" },
    { "set_JavaGetSetFrozen", "set_PythonGetSetFrozen" },
    { "javaContextualBodyIndent", "pythonContextualBodyIndent" },
    { "set_JavaContextualBodyIndent", "set_PythonContextualBodyIndent" }
  };
  
  for (index = 0; index != sizeof(opers)/sizeof(opers[0]); index += 1) {
    op = UmlOperation::java2Python(cl, cl, opers[index].java, opers[index].python);
    op->moveAfter(prev);
    prev = op;
  }
    
  op = cl->get_operation("pythonDef");
  s = op->cppDecl();
  conditional(s, "WITHPYTHON", 0);
  op->set_CppDecl(s);
  s = op->cppDef();
  conditional(s, "WITHPYTHON", 0);
  op->set_CppDef(s);
  
  defGet(cl, _python_decorators, pythonDecorators, "string", "WITHPYTHON", 0,
	 " decorators");
  op->moveAfter(prev);
  prev = op;
  defSet(cl, _python_decorators, set_PythonDecorators, "str", setPythonDecoratorsCmd,
	 0, "endif", " decorators");
  op->moveAfter(prev);
}

void package_add_python()
{
  // already root
  // already root
  UmlClass * cl = UmlClass::get("UmlBasePackage", 0);
  UmlOperation * prev = cl->get_operation("set_PhpDir");
  UmlOperation * op;
  int index;
  const struct {
    const char * java;
    const char * python;
  } opers[] = {
    { "javaDir", "pythonDir" },
    { "set_JavaDir", "set_PythonDir" },
    { "javaPackage", "pythonPackage" },
    { "set_JavaPackage", "set_PythonPackage" },
    { "findJavaPackage", "findPythonPackage" }
  };
  
  for (index = 0; index != sizeof(opers)/sizeof(opers[0]); index += 1) {
    op = UmlOperation::java2Python(cl, cl, opers[index].java, opers[index].python);
    op->moveAfter(prev);
    prev = op;
  }
}

void stereotype_add_python()
{
  // already root
  UmlClass * cl = UmlClass::get("UmlStereotype", 0);
  
  UmlAttribute::java2Php(cl, cl, "java", "python")
    ->moveAfter(cl->get_attribute("php"));
}

void associate_python_artifacts(UmlArtifact * pythonsettingsart,
				UmlArtifact * pythonsettingscmdart)
{
  UmlDeploymentView * dv = (UmlDeploymentView *)
    UmlClass::get("UmlArtifact", 0)->associatedArtifact()->parent();
  const QVector<UmlItem> ch = dv->children();
    
  for (unsigned i = 0; i != ch.size(); i += 1) {
    if ((ch[i]->kind() == anArtifact) && (ch[i]->name() == "executable")) {
      ((UmlArtifact *) ch[i])->addAssociatedArtifact(pythonsettingsart);
      ((UmlArtifact *) ch[i])->addAssociatedArtifact(pythonsettingscmdart);
      return;
    }
  }
}

void add_python()
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  //

  UmlCom::trace("<b>add Python</b><br>");
  
  UmlArtifact * pythonsettingsart = add_python_settings();
  UmlArtifact * pythonsettingscmdart = add_python_settings_cmd();
  
  add_python_on_enums();
  artifact_add_python();
  // attribute already up to date  
  class_add_python();
  classitem_add_python();
  // item already up to date
  operation_add_python();
  package_add_python();
  // relation already up to date
  stereotype_add_python();
  associate_python_artifacts(pythonsettingsart, pythonsettingscmdart);

  //

  UmlCom::set_user_id(uid);
}

//

void add_default_initoper(UmlClass * pythonsettings)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  //

  UmlCom::trace("<b>add PythonSettings __init__ management</b><br>");
  
  UmlOperation * op = pythonsettings->get_operation("operationDef");
  UmlOperation * initop
    = UmlOperation::create(pythonsettings, "initOperationDef");
  
  initop->set_isClassMember(TRUE);
  initop->set_ReturnType(op->returnType());
  initop->set_CppDecl(op->cppDecl());
  initop->set_CppDef(op->cppDef());
  initop->set_JavaDef(op->javaDef());
  initop->set_CppBody("  read_if_needed_();\n"
		      "\n"
		      "  return _initoper_def;\n");

  initop->set_JavaBody("  read_if_needed_();\n"
		       "\n"
		       "  return _initoper_def;\n");
  initop->set_Description(" returns the default definition of __init__");

  op = pythonsettings->get_operation("set_OperationDef");
  initop->moveAfter(op);
    
  UmlOperation * setinitop
    = UmlOperation::create(pythonsettings, "set_InitOperationDef");
  
  setinitop->addParameter(0, op->params()[0]);
  setinitop->set_isClassMember(TRUE);
  setinitop->set_ReturnType(op->returnType());
  setinitop->set_CppDecl(op->cppDecl());
  setinitop->set_CppDef(op->cppDef());
  setinitop->set_JavaDef(op->javaDef());
  setinitop->set_CppBody("  UmlCom::send_cmd(pythonSettingsCmd, setPythonInitOperationDefCmd, v);\n"
			 "  if (UmlCom::read_bool()) {\n"
			 "    _initoper_def = v;\n"
			 "    return TRUE;\n"
			 "  }\n"
			 "  else\n"
			 "    return FALSE;\n");
  setinitop->set_JavaBody("  UmlCom.send_cmd(CmdFamily.pythonSettingsCmd, PythonSettingsCmd._setPythonInitOperationDefCmd, v);\n"
			  "  UmlCom.check();\n"
			  "  \n"
			  "  _initoper_def = v;\n");
  setinitop->set_Description(" set the default definition of __init__\n"
			     "\n"
			     " On error : return FALSE in C++, produce a RuntimeException in Java");
  setinitop->moveAfter(initop);
  
  //
  
  UmlAttribute * opdef = pythonsettings->get_attribute("_oper_def");
  UmlAttribute * initopdef = UmlAttribute::create(pythonsettings, "_initoper_def");

  initopdef->set_isClassMember(TRUE);
  initopdef->moveAfter(opdef);
  initopdef->set_CppDecl(opdef->cppDecl());
  initopdef->set_JavaDecl(opdef->javaDecl());
  initopdef->set_Type(opdef->type());
  
  //
  
  QString s;
  
  op = pythonsettings->get_operation("read_");
  
  s = op->cppBody();
  s += "  _initoper_def = UmlCom::read_string();\n";
  op->set_CppBody(s);
  
  s = op->javaBody();
  s += "  _initoper_def = UmlCom.read_string();\n";
  op->set_JavaBody(s);

  //

  UmlClass::get("PythonSettingsCmd", 0)
    ->add_enum_item("setPythonInitOperationDefCmd");
  
  //

  UmlCom::set_user_id(uid);
}

//

void add_operation3(UmlClass * pythonsettings)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  //

  UmlCom::trace("<b>add Python 3 operation management</b><br>");
  
  UmlOperation * op = pythonsettings->get_operation("isPython_2_2");
  UmlOperation * op3
    = UmlOperation::create(pythonsettings, "isPython_3_operation");
  
  op3->set_isClassMember(TRUE);
  op3->set_ReturnType(op->returnType());
  op3->set_CppDecl(op->cppDecl());
  op3->set_CppDef(op->cppDef());
  op3->set_JavaDef(op->javaDef());
  op3->set_CppBody("  read_if_needed_();\n"
		   "\n"
		   "  return _operation_3;\n");
  
  op3->set_JavaBody("  read_if_needed_();\n"
		    "\n"
		    "  return _operation_3;\n");
  op3->set_Description(" returns if operations follow Python 3 (pep-3107)");

  op = pythonsettings->get_operation("set_IsPython_2_2");
  op3->moveAfter(op);
    
  UmlOperation * setop3
    = UmlOperation::create(pythonsettings, "set_IsPython_3_operation");
  
  setop3->addParameter(0, op->params()[0]);
  setop3->set_isClassMember(TRUE);
  setop3->set_ReturnType(op->returnType());
  setop3->set_CppDecl(op->cppDecl());
  setop3->set_CppDef(op->cppDef());
  setop3->set_JavaDef(op->javaDef());
  setop3->set_CppBody("  UmlCom::send_cmd(pythonSettingsCmd, setPython3OperationCmd, y);\n"
		      "  if (UmlCom::read_bool()) {\n"
		      "    _operation_3 = y;\n"
		      "    return TRUE;\n"
		      "  }\n"
		      "  else\n"
		      "    return FALSE;\n");
  setop3->set_JavaBody("  UmlCom.send_cmd(CmdFamily.pythonSettingsCmd, PythonSettingsCmd._setPython3OperationCmd, (y) ? (byte) 1 : (byte) 0);\n"
		       "  UmlCom.check();\n"
		       "  \n"
		       "  _operation_3 = y;\n");
  setop3->set_Description(" set if operations follow Python 3 (pep-3107)\n"
			  "\n"
			  " On error : return FALSE in C++, produce a RuntimeException in Java");
  setop3->moveAfter(op3);
  
  //
  
  UmlAttribute * opdef = pythonsettings->get_attribute("_2_2");
  UmlAttribute * op3def = UmlAttribute::create(pythonsettings, "_operation_3");

  op3def->set_isClassMember(TRUE);
  op3def->moveAfter(opdef);
  op3def->set_CppDecl(opdef->cppDecl());
  op3def->set_JavaDecl(opdef->javaDecl());
  op3def->set_Type(opdef->type());
  
  //
  
  QString s;
  
  op = pythonsettings->get_operation("read_");
  
  s = op->cppBody();
  s += "  _operation_3 = UmlCom::read_bool();\n";
  op->set_CppBody(s);
  
  s = op->javaBody();
  s += "  _operation_3 = UmlCom.read_bool();\n";
  op->set_JavaBody(s);

  //

  UmlClass::get("PythonSettingsCmd", 0)
    ->add_enum_item("setPython3OperationCmd");
  
  //

  UmlCom::set_user_id(uid);
}
