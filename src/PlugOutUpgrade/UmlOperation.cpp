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

#include "UmlOperation.h"
#include "UmlClass.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "util.h"

static QCString remove_throw(QCString d)
{
  int index;
  
  index = d.find("${throw}");
  if (index != -1)
    d.remove(index, 8);
  
  return d;
}


void UmlOperation::remove_cpp_throw() {
  set_CppDecl(remove_throw(CppSettings::operationDecl()));
  set_CppDef(remove_throw(CppSettings::operationDef()));
}

void UmlOperation::add_param(int rank, aDirection dir,
			     const char * name, UmlTypeSpec type) {
  UmlParameter p;
  
  p.dir = dir;
  p.type = type;
  p.name = name;
  
  if (!addParameter(rank, p)) {
    QCString msg = QCString("can't add parameter '") + name +
      "' to " + parent()->name() + "::" + this->name() + "<br>\n";

    UmlCom::trace(msg);
    throw 0;
  }
}

void UmlOperation::set_cpp(const char * return_form_or_inherit,
			   const char * params, QCString body,
			   bool inlinep, const char * if_def,
			   const char * end_if) {
  if (*return_form_or_inherit == ':') {
    // inherit
    if (inlinep) {
      QCString s = remove_throw(CppSettings::operationDecl());
      int index = s.find("${)}");
      
      s.resize(index + 5);
      s.insert(index, params);
      s.append(" ");
      s.append(return_form_or_inherit);
      if (!body.isEmpty()) {
	s.append(" {\n  ");
	s.append(body);
	s.append("}\n");
      }
      else
	s.append(" {\n}\n");
      conditional(s, if_def, end_if);
      set_CppDecl(s);
      
      set_CppDef("");
    }
    else {
      QCString s = remove_throw(CppSettings::operationDecl());
      int index = s.find("${)}");
      
      s.resize(index + 5);
      s.insert(index, params);
      s.append(";");
      conditional(s, if_def, end_if);
      set_CppDecl(s);
      
      s = remove_throw(CppSettings::operationDef());
      index = s.find("${)}");
      s.resize(index + 5);
      s.insert(index, params);
      s.append(" ");
      s.append(return_form_or_inherit);
      if (!body.isEmpty()) {
	s.append(" {\n  ");
	s.append(body);
	s.append("}\n");
      }
      else
	s.append(" {\n}\n");
      conditional(s, if_def, end_if);
      set_CppDef(s);
    }
  }
  else {
    // return
    if (inlinep) {
      QCString s = remove_throw(CppSettings::operationDecl());
      int index = s.find("${type}");
      
      s.replace(index, 7, return_form_or_inherit);
      s.insert(s.find("${)}", index), params);
      s.resize(s.findRev(";") + 1);
      if (!body.isEmpty()) {
	s.append(" {\n  ");
	s.append(body);
	s.append("}\n");
      }
      else
	s.append(" {\n}\n");
      conditional(s, if_def, end_if);
      set_CppDecl(s);
      
      set_CppDef("");
    }
    else {
      QCString s = remove_throw(CppSettings::operationDecl());
      int index = s.find("${type}");
      
      s.replace(index, 7, return_form_or_inherit);
      s.insert(s.find("${)}", index), params);
      conditional(s, if_def, end_if);
      set_CppDecl(s);
      
      s = remove_throw(CppSettings::operationDef());
      index = s.find("${type}");
      s.replace(index, 7, return_form_or_inherit);
      s.insert(s.find("${)}", index), params);
      conditional(s, if_def, end_if);
      set_CppDef(s);
      
      set_CppBody(body);
    }
  }
}


void UmlOperation::set_java(const char * return_form,
			    const char * params, QCString body,
			    bool inlinep) {
  QCString s = JavaSettings::operationDef();
  int index = s.find("${type}");
  
  s.replace(index, 7, return_form);
  s.insert(s.find("${)}", index), params);
  
  if (inlinep) {
    s.replace(s.findRev("${body}"), 7, body);
    set_JavaDef(s);
  }
  else {
    set_JavaDef(s);
    set_JavaBody(body);
  }
}

// rename call to operation of java enum settings
// rename cmd enum item usage
// do the rename in C++ decl, def and body, in Java def and body

static bool is_sep(char c)
{
  return !((c == '_') ||
	   ((c >= 'a') && (c <= 'z')) ||
	   ((c >= 'A') && (c <= 'Z')) ||
	   ((c >= '0') && (c <= '9')));
}

static bool rename(QCString & s, bool java, bool javasettings)
{
  static const struct {
    const char * o;
    const char * n;
    bool java_only;
  } T[] = {
    { "enumDecl", "enumPatternDecl", FALSE },
    { "set_EnumDecl", "set_EnumPatternDecl", FALSE },
    { "enumItemDecl", "enumPatternItemDecl", FALSE },
    { "set_EnumItemDecl", "set_EnumPatternItemDecl", FALSE },
    { "enumItemCase", "enumPatternItemCase", FALSE },
    { "set_EnumItemCase", "set_EnumPatternItemCase", FALSE },
    
    { "setJavaEnumDeclCmd", "setJavaEnumPatternDeclCmd", FALSE },
    { "setJavaEnumItemDeclCmd", "setJavaEnumPatternItemDeclCmd", FALSE },
    { "setJavaEnumItemCaseCmd", "setJavaEnumPatternItemCaseCmd", FALSE },
    
    { "_setJavaEnumDeclCmd", "_setJavaEnumPatternDeclCmd", TRUE },
    { "_setJavaEnumItemDeclCmd", "_setJavaEnumPatternItemDeclCmd", TRUE },
    { "_setJavaEnumItemCaseCmd", "_setJavaEnumPatternItemCaseCmd", TRUE },
    
    { "_enum_decl", "_enum_pattern_decl", FALSE },
    { "_enum_item_decl", "_enum_pattern_item_decl", FALSE },
    { "_enum_item_case", "_enum_pattern_item_case", FALSE }
  };
  int t_index;
  bool changed = FALSE;
  
  for (t_index = 0; t_index != sizeof(T)/sizeof(T[0]); t_index += 1) {
    if (!T[t_index].java_only || java) {
      QCString o = T[t_index].o;
      
      if (!javasettings)
	o = ((java) ? "JavaSettings." : "JavaSettings::") + o;
      
      int o_len = o.length();
      QCString n = T[t_index].n;
      
      if (!javasettings)
	n = ((java) ? "JavaSettings." : "JavaSettings::") + n;
      
      int n_len = n.length();
      int index = 0;
      
      while ((index = s.find(o, index)) != -1) {
	if (((index == 0) || is_sep(s[index - 1])) &&
	    is_sep(s[index + o_len])) {
	  s.replace(index, o_len, n);
	  index += n_len;
	  changed = TRUE;
	}
	else
	  index += 1;
      }
    }
  }

  return changed;
}

void UmlOperation::rename_jdk5() {
  QCString s;
  bool changed = FALSE;
  bool javasettings = (parent()->name() == "JavaSettings");
  
  s = cppDecl();
  if (rename(s, FALSE, javasettings)) {
    changed = TRUE;
    set_CppDecl(s);
  }
  
  s = cppDef();
  if (rename(s, FALSE, javasettings)) {
    changed = TRUE;
    set_CppDef(s);
  }
  
  s = cppBody();
  if (rename(s, FALSE, javasettings)) {
    changed = TRUE;
    set_CppBody(s);
  }
  
  s = javaDef();
  if (rename(s, TRUE, javasettings)) {
    changed = TRUE;
    set_JavaDef(s);
  }
  
  s = javaBody();
  if (rename(s, TRUE, javasettings)) {
    changed = TRUE;
    set_JavaBody(s);
  }
  
  if (changed)
    UmlCom::trace(parent()->name() + "::" + name() +
		  " upgraded to take into account the rename of 'enum' to 'enum pattern'<br>\n");
  
}


UmlOperation * UmlOperation::java2Php(UmlClass * php, UmlClass * java,
				      const char * javaname,
				      const char * phpname)
{
  if (phpname == 0)
    phpname = javaname;
  
  UmlOperation * from = java->get_operation(javaname);
  
  if (from == 0) {
    QCString err = QCString("cannot find operation '") + 
      javaname + QCString("' in class '") + java->name()
	+ QCString("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlOperation * to = UmlBaseOperation::create(php, phpname);
  
  if (to == 0) {
    QCString err = QCString("cannot create operation '") + 
      phpname + QCString("' in class '") + php->name()
	+ QCString("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlCom::trace("add operation " + php->name() + "::" + phpname + "<br>\n");

  to->set_Description(::java2Php(from->description()));
  to->set_ReturnType(from->returnType());
  to->set_isClassMember(from->isClassMember());
  to->set_Visibility(from->visibility());
  to->set_CppVisibility(from->cppVisibility());
  
  const QValueList<UmlParameter> params = from->params();
  unsigned index;
  
  for (index = 0; index != params.count(); index += 1)
    to->addParameter(index, params[index]);
  
  const QValueList<UmlTypeSpec> exceptions = from->exceptions();
  
  for (index = 0; index != exceptions.count(); index += 1)
    to->addException(index, exceptions[index]);
  
  to->set_isCppVirtual(from->isCppVirtual());
  to->set_isCppConst(from->isCppConst());
  to->set_isCppInline(from->isCppInline());
  to->set_CppDecl(::java2Php(from->cppDecl()));
  to->set_CppDef(::java2Php(from->cppDef()));
  to->set_CppBody(::java2Php(from->cppBody()));
  
  to->set_isJavaFinal(from->isJavaFinal());
  to->set_JavaDef(from->javaDef());
  to->set_JavaBody(::java2Php(from->javaBody()));
  
  return to;
}

UmlOperation * UmlOperation::java2Python(UmlClass * python, UmlClass * java,
					 const char * javaname,
					 const char * pythonname)
{
  if (pythonname == 0)
    pythonname = javaname;
  
  UmlOperation * from = java->get_operation(javaname);
  
  if (from == 0) {
    QCString err = QCString("cannot find operation '") + 
      javaname + QCString("' in class '") + java->name()
	+ QCString("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlOperation * to = UmlBaseOperation::create(python, pythonname);
  
  if (to == 0) {
    QCString err = QCString("cannot create operation '") + 
      pythonname + QCString("' in class '") + python->name()
	+ QCString("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlCom::trace("add operation " + python->name() + "::" + pythonname + "<br>\n");

  to->set_Description(::java2Python(from->description()));
  to->set_ReturnType(from->returnType());
  to->set_isClassMember(from->isClassMember());
  to->set_Visibility(from->visibility());
  to->set_CppVisibility(from->cppVisibility());
  
  const QValueList<UmlParameter> params = from->params();
  unsigned index;
  
  for (index = 0; index != params.count(); index += 1)
    to->addParameter(index, params[index]);
  
  const QValueList<UmlTypeSpec> exceptions = from->exceptions();
  
  for (index = 0; index != exceptions.count(); index += 1)
    to->addException(index, exceptions[index]);
  
  to->set_isCppVirtual(from->isCppVirtual());
  to->set_isCppConst(from->isCppConst());
  to->set_isCppInline(from->isCppInline());
  to->set_CppDecl(::java2Python(from->cppDecl()));
  to->set_CppDef(::java2Python(from->cppDef()));
  to->set_CppBody(::java2Python(from->cppBody()));
  
  to->set_isJavaFinal(from->isJavaFinal());
  to->set_JavaDef(from->javaDef());
  to->set_JavaBody(::java2Python(from->javaBody()));
  
  return to;
}

UmlOperation * UmlOperation::cpp2Python(UmlClass * python, UmlClass * cpp,
					 const char * cppname,
					 const char * pythonname)
{
  if (pythonname == 0)
    pythonname = cppname;
  
  UmlOperation * from = cpp->get_operation(cppname);
  
  if (from == 0) {
    QCString err = QCString("cannot find operation '") + 
      cppname + QCString("' in class '") + cpp->name()
	+ QCString("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlOperation * to = UmlBaseOperation::create(python, pythonname);
  
  if (to == 0) {
    QCString err = QCString("cannot create operation '") + 
      pythonname + QCString("' in class '") + python->name()
	+ QCString("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlCom::trace("add operation " + python->name() + "::" + pythonname + "<br>\n");

  to->set_Description(::cpp2Python(from->description()));
  to->set_ReturnType(from->returnType());
  to->set_isClassMember(from->isClassMember());
  to->set_Visibility(from->visibility());
  to->set_CppVisibility(from->cppVisibility());
  
  const QValueList<UmlParameter> params = from->params();
  unsigned index;
  
  for (index = 0; index != params.count(); index += 1)
    to->addParameter(index, params[index]);
  
  const QValueList<UmlTypeSpec> exceptions = from->exceptions();
  
  for (index = 0; index != exceptions.count(); index += 1)
    to->addException(index, exceptions[index]);
  
  to->set_isCppVirtual(from->isCppVirtual());
  to->set_isCppConst(from->isCppConst());
  to->set_isCppInline(from->isCppInline());
  to->set_CppDecl(::cpp2Python(from->cppDecl()));
  to->set_CppDef(::cpp2Python(from->cppDef()));
  to->set_CppBody(::cpp2Python(from->cppBody()));
  
  to->set_isJavaFinal(from->isJavaFinal());
  to->set_JavaDef(from->javaDef());
  to->set_JavaBody(::cpp2Python(from->javaBody()));
  
  return to;
}
