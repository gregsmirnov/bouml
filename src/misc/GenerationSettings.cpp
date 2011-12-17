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





#include <qcursor.h>
#include <qfiledialog.h>

#include "GenerationSettings.h"
#include "GenerationSettingsDialog.h"
#include "BrowserPackage.h"
#include "BrowserClass.h"
#include "BrowserView.h"
#include "ClassData.h"
#include "AType.h"
#include "myio.h"
#include "ToolCom.h"
#include "err.h"

int GenerationSettings::nbuiltins;
Builtin * GenerationSettings::builtins;
QStringList GenerationSettings::umltypes;

bool GenerationSettings::cpp_default_defs;
bool GenerationSettings::cpp_javadoc_comment;
SharedStr GenerationSettings::cpp_h_content;
SharedStr GenerationSettings::cpp_src_content;
QString GenerationSettings::cpp_in;
QString GenerationSettings::cpp_out;
QString GenerationSettings::cpp_inout;
QString GenerationSettings::cpp_return;
QString GenerationSettings::cpp_enum_in;
QString GenerationSettings::cpp_enum_out;
QString GenerationSettings::cpp_enum_inout;
QString GenerationSettings::cpp_enum_return;
SharedStr GenerationSettings::cpp_class_decl;
SharedStr GenerationSettings::cpp_external_class_decl;
SharedStr GenerationSettings::cpp_struct_decl;
SharedStr GenerationSettings::cpp_union_decl;
SharedStr GenerationSettings::cpp_enum_decl;
SharedStr GenerationSettings::cpp_typedef_decl;
SharedStr GenerationSettings::cpp_attr_decl[/*multiplicity*/3];
SharedStr GenerationSettings::cpp_enum_item_decl;
SharedStr GenerationSettings::cpp_rel_decl[/*relation kind*/2][/*multiplicity*/3];
SharedStr GenerationSettings::cpp_oper_decl;
SharedStr GenerationSettings::cpp_oper_def;
bool GenerationSettings::cpp_force_throw;
IncludesSpec GenerationSettings::cpp_includes;
UmlVisibility GenerationSettings::cpp_get_visibility;
SharedStr GenerationSettings::cpp_get_name;
bool GenerationSettings::cpp_get_inline;
bool GenerationSettings::cpp_get_const;
bool GenerationSettings::cpp_get_value_const;
UmlVisibility GenerationSettings::cpp_set_visibility;
SharedStr GenerationSettings::cpp_set_name;
bool GenerationSettings::cpp_set_inline;
bool GenerationSettings::cpp_set_param_const;
bool GenerationSettings::cpp_set_param_ref;
QCString GenerationSettings::cpp_indent_visibility;

bool GenerationSettings::java_default_defs;
bool GenerationSettings::java_javadoc_comment;
SharedStr GenerationSettings::java_src_content;
SharedStr GenerationSettings::java_class_decl;
SharedStr GenerationSettings::java_external_class_decl;
SharedStr GenerationSettings::java_interface_decl;
SharedStr GenerationSettings::java_enum_decl;
SharedStr GenerationSettings::java_enum_pattern_decl;
SharedStr GenerationSettings::java_attr_decl[/*multiplicity*/3];
SharedStr GenerationSettings::java_enum_item_decl;
SharedStr GenerationSettings::java_enum_pattern_item_decl;
SharedStr GenerationSettings::java_enum_pattern_item_case;
SharedStr GenerationSettings::java_rel_decl[/*multiplicity*/3];
SharedStr GenerationSettings::java_oper_def;
IncludesSpec GenerationSettings::java_imports;
UmlVisibility GenerationSettings::noncpp_get_visibility;
SharedStr GenerationSettings::java_get_name;
bool GenerationSettings::java_get_final;
UmlVisibility GenerationSettings::noncpp_set_visibility;
SharedStr GenerationSettings::java_set_name;
bool GenerationSettings::java_set_final;
bool GenerationSettings::java_set_param_final;

bool GenerationSettings::php_default_defs;
bool GenerationSettings::php_javadoc_comment;
SharedStr GenerationSettings::php_src_content;
SharedStr GenerationSettings::php_class_decl;
SharedStr GenerationSettings::php_external_class_decl;
SharedStr GenerationSettings::php_enum_decl;
SharedStr GenerationSettings::php_interface_decl;
SharedStr GenerationSettings::php_attr_decl;
SharedStr GenerationSettings::php_enum_item_decl;
SharedStr GenerationSettings::php_rel_decl;
SharedStr GenerationSettings::php_oper_def;
SharedStr GenerationSettings::php_get_name;
bool GenerationSettings::php_get_final;
SharedStr GenerationSettings::php_set_name;
bool GenerationSettings::php_set_final;

bool GenerationSettings::python_default_defs;
bool GenerationSettings::python_2_2;
bool GenerationSettings::python_3_operation;
QString GenerationSettings::python_indent_step;
SharedStr GenerationSettings::python_src_content;
SharedStr GenerationSettings::python_class_decl;
SharedStr GenerationSettings::python_enum_decl;
SharedStr GenerationSettings::python_external_class_decl;
SharedStr GenerationSettings::python_attr_decl[2/*multiplicity*/];
SharedStr GenerationSettings::python_enum_item_decl;
SharedStr GenerationSettings::python_rel_decl[2/*relation kind*/][2/*multiplicity*/];
SharedStr GenerationSettings::python_oper_def;
SharedStr GenerationSettings::python_initoper_def;
IncludesSpec GenerationSettings::python_imports;
SharedStr GenerationSettings::python_get_name;
SharedStr GenerationSettings::python_set_name;
    
bool GenerationSettings::idl_default_defs;
SharedStr GenerationSettings::idl_src_content;
SharedStr GenerationSettings::idl_interface_decl;
SharedStr GenerationSettings::idl_valuetype_decl;
SharedStr GenerationSettings::idl_struct_decl;
SharedStr GenerationSettings::idl_typedef_decl;
SharedStr GenerationSettings::idl_exception_decl;
SharedStr GenerationSettings::idl_union_decl;
SharedStr GenerationSettings::idl_external_class_decl;
IncludesSpec GenerationSettings::idl_includes;
SharedStr GenerationSettings::idl_enum_decl;
SharedStr GenerationSettings::idl_attr_decl[/*multiplicity*/3];
SharedStr GenerationSettings::idl_valuetype_attr_decl[/*multiplicity*/3];
SharedStr GenerationSettings::idl_union_item_decl[/*multiplicity*/3];
SharedStr GenerationSettings::idl_enum_item_decl;
SharedStr GenerationSettings::idl_const_decl[/*multiplicity*/3];
SharedStr GenerationSettings::idl_valuetype_rel_decl[/*multiplicity*/3];
SharedStr GenerationSettings::idl_rel_decl[/*multiplicity*/3];
SharedStr GenerationSettings::idl_union_rel_decl[/*multiplicity*/3];
SharedStr GenerationSettings::idl_oper_decl;
SharedStr GenerationSettings::idl_get_name;
SharedStr GenerationSettings::idl_set_name;
bool GenerationSettings::idl_set_oneway;

DrawingLanguage GenerationSettings::uml_get_name;
DrawingLanguage GenerationSettings::uml_set_name;

QString GenerationSettings::cpp_h_extension;
QString GenerationSettings::cpp_src_extension;
QString GenerationSettings::java_extension;
QString GenerationSettings::php_extension;
QString GenerationSettings::python_extension;
QString GenerationSettings::idl_extension;

bool GenerationSettings::cpp_include_with_path;
bool GenerationSettings::cpp_relative_path;
bool GenerationSettings::cpp_root_relative_path;

bool GenerationSettings::php_req_with_path;
bool GenerationSettings::php_relative_path;
bool GenerationSettings::php_root_relative_path;

bool GenerationSettings::cpp_force_namespace_gen;
bool GenerationSettings::cpp_inline_force_incl_in_h;
bool GenerationSettings::java_force_package_gen;
bool GenerationSettings::php_force_namespace_gen;
    
int GenerationSettings::nrelattrstereotypes;
Stereotype * GenerationSettings::relattr_stereotypes;

int GenerationSettings::nclassstereotypes;
Stereotype * GenerationSettings::class_stereotypes;

QString GenerationSettings::artifact_default_description;
QString GenerationSettings::class_default_description;
QString GenerationSettings::operation_default_description;
QString GenerationSettings::attribute_default_description;
QString GenerationSettings::relation_default_description;

QString GenerationSettings::cpp_root_dir;
QString GenerationSettings::java_root_dir;
QString GenerationSettings::idl_root_dir;
QString GenerationSettings::php_root_dir;
QString GenerationSettings::python_root_dir;

ReverseRoundtripFilter GenerationSettings::cpp_dir_filter;
ReverseRoundtripFilter GenerationSettings::cpp_file_filter;
ReverseRoundtripFilter GenerationSettings::java_dir_filter;
ReverseRoundtripFilter GenerationSettings::java_file_filter;
ReverseRoundtripFilter GenerationSettings::php_dir_filter;
ReverseRoundtripFilter GenerationSettings::php_file_filter;  

inline void Builtin::set(const char * u, const char * c,
			 const char * j, const char * i) {
  uml = u;
  cpp = c;
  java = j;
  idl = i;
  
  cpp_in = "${type}";
  cpp_out = cpp_inout = "${type} &";
  cpp_return = "${type}";
}

inline void Stereotype::set(const char * u, const char * c,
			    const char * j, const char * p, 
			    const char * y, const char * i) {
  uml = u;
  cpp = c;
  java = j;
  php = p;
  python = y;
  idl = i;
}

void GenerationSettings::init()
{
  if (builtins != 0)
    delete [] builtins;
  
  nbuiltins = 15;
  builtins = new Builtin[nbuiltins];
  
  builtins[0].set("void", "void", "void", "void");
  builtins[1].set("any", "void *", "Object", "any");
  builtins[1].cpp_in =  "const ${type}";
  builtins[1].cpp_out = builtins[0].cpp_inout = "${type}";
  builtins[2].set("bool", "bool", "boolean", "boolean");
  builtins[3].set("char", "char", "char", "char");
  builtins[4].set("uchar", "unsigned char", "char", "octet");
  builtins[5].set("byte", "unsigned char", "byte", "octet");
  builtins[6].set("short", "short", "short", "short");
  builtins[7].set("ushort", "unsigned short", "short", "unsigned short");
  builtins[8].set("int", "int", "int", "long");
  builtins[9].set("uint", "unsigned int", "int", "unsigned long");
  builtins[10].set("long", "long", "long", "long");
  builtins[11].set("ulong", "unsigned long", "long", "unsigned long");
  builtins[12].set("float", "float", "float", "float");
  builtins[13].set("double", "double", "double", "double");
  builtins[14].set("string", "string", "String", "string");
  
#define CPP_H_CONTENT "#ifndef ${NAMESPACE}_${NAME}_H\n\
#define ${NAMESPACE}_${NAME}_H\n\
\n\
${comment}\n\
${includes}\n\
${declarations}\n\
${namespace_start}\n\
${definition}\n\
${namespace_end}\n\
#endif\n"
  cpp_h_content = CPP_H_CONTENT;
#define CPP_SRC_CONTENT "${comment}\n\
${includes}\n\
${namespace_start}\n\
${members}\n\
${namespace_end}"
  cpp_src_content = CPP_SRC_CONTENT;
  cpp_h_extension = "h";
  cpp_src_extension = "cpp";
  
#define JAVA_SRC_CONTENT "${comment}\n\
${package}\n\
${imports}\n\
${definition}";
  java_src_content = JAVA_SRC_CONTENT;
  java_extension = "java";

#define PHP_SRC_CONTENT "<?php\n\
${comment}\n\
${namespace}\n\
${require_once}\n\
${definition}\n\
?>\n";
  php_src_content = PHP_SRC_CONTENT;
  php_extension = "php";
  
#define PYTHON_SRC_CONTENT "${comment}\n\
${import}\n\
${definition}";
  python_src_content = PYTHON_SRC_CONTENT;
  python_extension = "py";
  
#define IDL_SRC_CONTENT "#ifndef ${MODULE}_${NAME}_H\n\
#define ${MODULE}_${NAME}_H\n\
\n\
${comment}\n\
${includes}\n\
${module_start}\n\
${definition}\n\
${module_end}\n\
#endif\n"
  idl_src_content = IDL_SRC_CONTENT;
  idl_extension = "idl";
  
  if (relattr_stereotypes != 0)
    delete [] relattr_stereotypes;
  
  nrelattrstereotypes = 5;
  relattr_stereotypes = new Stereotype[nrelattrstereotypes];
  
  relattr_stereotypes[0].set("sequence", "vector", "Vector", "", "list", "sequence");
  relattr_stereotypes[1].set("vector", "vector", "Vector", "", "list", "sequence");
  relattr_stereotypes[2].set("list", "list", "List", "", "list", "sequence");
  relattr_stereotypes[3].set("set", "set", "Set", "", "set", "sequence");
  relattr_stereotypes[4].set("map", "map", "Map", "", "dict", "sequence");
  
  if (class_stereotypes != 0)
    delete [] class_stereotypes;
  
  nclassstereotypes = 14;
  class_stereotypes = new Stereotype[nclassstereotypes];
  
  class_stereotypes[0].set("class", "class", "class", "class", "class", "valuetype");
  class_stereotypes[1].set("interface", "class", "interface", "interface", "class", "interface");
  class_stereotypes[2].set("exception", "class", "class", "class", "class", "exception");
  class_stereotypes[3].set("enum", "enum", "enum", "enum", "enum", "enum");
  class_stereotypes[4].set("enum_pattern", "enum", "enum_pattern", "enum", "enum", "enum");
  class_stereotypes[5].set("struct", "struct", "class", "class", "class", "struct");
  class_stereotypes[6].set("union", "union", "class", "class", "class", "union");
  class_stereotypes[7].set("typedef", "typedef", "ignored", "ignored", "ignored", "typedef");
  class_stereotypes[8].set("boundary", "class", "class", "class", "class", "interface");
  class_stereotypes[9].set("control", "class", "class", "class", "class", "valuetype");
  class_stereotypes[10].set("entity", "class", "class", "class", "class", "valuetype");
  class_stereotypes[11].set("actor", "ignored", "ignored", "ignored", "ignored", "ignored");
  class_stereotypes[12].set("@interface", "ignored", "@interface", "ignored", "ignored", "ignored");
  class_stereotypes[13].set("stereotype", "ignored", "ignored", "ignored", "ignored", "ignored");
  
  cpp_enum_in = "${type}";
  cpp_enum_out = "${type} &";
  cpp_enum_inout = "${type} &";
  cpp_enum_return = "${type}";
  
  cpp_in = "const ${type} &";
  cpp_out = "${type} &";
  cpp_inout = "${type} &";
  cpp_return = "${type}";
  
  cpp_class_decl = "${comment}${template}class ${name}${inherit} {\n${members}};\n${inlines}\n";
  cpp_external_class_decl = "${name}\n#include <${name}.h>\n";
  cpp_struct_decl = "${comment}${template}struct ${name}${inherit} {\n${members}};\n${inlines}\n";
  cpp_union_decl = "${comment}${template}union ${name} {\n${members}};\n${inlines}\n";
  cpp_enum_decl = "${comment}enum ${name} {\n${items}\n};\n";
  cpp_typedef_decl = "${comment}typedef ${type} ${name};\n";
#define CPP_ATTR_DECL1	"    ${comment}${static}${mutable}${volatile}${const}${stereotype}<${type}> ${name}${value};\n"
#define CPP_ATTR_DECL2	"    ${comment}${static}${mutable}${volatile}${const}${type} ${name}${multiplicity}${value};\n"
#define CPP_ATTR_DESCR1	"    ${description}${static}${mutable}${volatile}${const}${stereotype}<${type}> ${name}${value};\n"
#define CPP_ATTR_DESCR2	"    ${description}${static}${mutable}${volatile}${const}${type} ${name}${multiplicity}${value};\n"
  cpp_attr_decl[0] = "    ${comment}${static}${mutable}${volatile}${const}${type} ${name}${value};\n";
  cpp_attr_decl[1] = CPP_ATTR_DECL1;
  cpp_attr_decl[2] = CPP_ATTR_DECL2;
  cpp_enum_item_decl = "  ${name}${value},${comment}";
  cpp_rel_decl[0][0] = "    ${comment}${static}${mutable}${volatile}${const}${type} * ${name}${value};\n";
  cpp_rel_decl[0][1] = "    ${comment}${static}${mutable}${volatile}${const}${stereotype}<${type} *> ${name}${value};\n";
  cpp_rel_decl[0][2] = "    ${comment}${static}${mutable}${volatile}${const}${type} * ${name}${multiplicity}${value};\n";
  cpp_rel_decl[1][0] = "    ${comment}${static}${mutable}${volatile}${const}${type} ${name}${value};\n";
  cpp_rel_decl[1][1] = "    ${comment}${static}${mutable}${volatile}${const}${stereotype}<${type}> ${name}${value};\n";
  cpp_rel_decl[1][2] = "    ${comment}${static}${mutable}${volatile}${const}${type} ${name}${multiplicity}${value};\n";
  cpp_oper_decl = "    ${comment}${friend}${static}${inline}${virtual}${type} ${name}${(}${)}${const}${volatile}${throw}${abstract};\n";
  cpp_oper_def = "${comment}${inline}${type} ${class}::${name}${(}${)}${const}${volatile}${throw}${staticnl}{\n  ${body}}\n";
  cpp_force_throw = FALSE;
  cpp_get_visibility = UmlPublic;
  cpp_get_name = "get_${name}";
  cpp_get_inline = TRUE;
  cpp_get_const = TRUE;
  cpp_get_value_const = TRUE;
  cpp_set_visibility = UmlPublic;
  cpp_set_name = "set_${name}";
  cpp_set_inline = FALSE;
  cpp_set_param_const = FALSE;
  cpp_set_param_ref = FALSE;
  cpp_include_with_path = FALSE;
  cpp_relative_path = FALSE;
  cpp_root_relative_path = FALSE;
  cpp_force_namespace_gen = FALSE;
  cpp_inline_force_incl_in_h = FALSE;
  cpp_javadoc_comment = FALSE;
#define CPP_INDENT_VISIBILITY "  "
  cpp_indent_visibility = CPP_INDENT_VISIBILITY;
  
  java_class_decl = "${comment}${@}${visibility}${final}${abstract}class ${name}${extends}${implements} {\n${members}}\n";
  java_external_class_decl = "${name}";
  java_interface_decl = "${comment}${@}${visibility}interface ${name}${extends} {\n${members}}\n";
  java_enum_decl = "${comment}${@}${visibility}${final}${abstract}enum ${name}${implements} {\n${items};\n${members}}\n";
  java_enum_pattern_decl = "${comment}${@}${visibility}final class ${name} {\n${members}\n\
  private final int value;\n\n\
  public int value() {\n\
    return value;\n\
  }\n\n\
  public static ${name} fromInt(int value) {\n\
    switch (value) {\n\
${cases}    default: throw new Error();\n\
    }\n\n\
  }\n\
  private ${name}(int v) { value = v; };\n}\n";
#define JAVA_ATTR_DECL1	"  ${comment}${@}${visibility}${static}${final}${transient}${volatile}${stereotype}<${type}> ${name}${value};\n"
#define JAVA_ATTR_DECL2	"  ${comment}${@}${visibility}${static}${final}${transient}${volatile}${type}${multiplicity} ${name}${value};\n"
#define JAVA_ATTR_DESCR1	"  ${description}${@}${visibility}${static}${final}${transient}${volatile}${stereotype}<${type}> ${name}${value};\n"
#define JAVA_ATTR_DESCR2	"  ${description}${@}${visibility}${static}${final}${transient}${volatile}${type}${multiplicity} ${name}${value};\n"
  java_attr_decl[0] = "  ${comment}${@}${visibility}${static}${final}${transient}${volatile}${type} ${name}${value};\n";
  java_attr_decl[1] = JAVA_ATTR_DECL1;
  java_attr_decl[2] = JAVA_ATTR_DECL2;
  java_enum_item_decl = "  ${@}${name}${value},${comment}";
  java_enum_pattern_item_decl = "  ${comment}${@}public static final int _${name}${value};\n\
public static final ${class} ${name} = new ${class}(_${name});\n";
  java_enum_pattern_item_case = "    case _${name}: return ${name};\n";
  java_rel_decl[0] = "  ${comment}${@}${visibility}${static}${final}${transient}${volatile}${type} ${name}${value};\n";
  java_rel_decl[1] = "  ${comment}${@}${visibility}${static}${final}${transient}${volatile}${stereotype}<${type}> ${name}${value};\n";
  java_rel_decl[2] = "  ${comment}${@}${visibility}${static}${final}${transient}${volatile}${type}${multiplicity} ${name}${value};\n";
  java_oper_def = "  ${comment}${@}${visibility}${final}${static}${abstract}${synchronized}${type} ${name}${(}${)}${throws}${staticnl}{\n  ${body}}\n";
  noncpp_get_visibility = UmlPublic;
  java_get_name = "get${Name}";
  java_get_final = TRUE;
  noncpp_set_visibility = UmlPublic;
  java_set_name = "set${Name}";
  java_set_final = FALSE;
  java_set_param_final = FALSE;
  java_javadoc_comment = TRUE;
  java_force_package_gen = TRUE;

#define PHP_CLASS "${comment}${final}${abstract}class ${name}${extends}${implements} {\n${members}}\n"
  php_class_decl = PHP_CLASS;
  php_external_class_decl = "${name}";
#define PHP_ENUM "${comment}${visibility}final class ${name} {\n${items}}\n"
  php_enum_decl = PHP_ENUM;
#define PHP_INTERFACE "${comment}interface ${name} {\n${members}}\n"
  php_interface_decl = PHP_INTERFACE;
#define PHP_ATTR "  ${comment}${visibility}${const}${static}${var}${name}${value};\n"
  php_attr_decl = PHP_ATTR;
#define PHP_ENUMITEM "  const ${name}${value};${comment}\n"
  php_enum_item_decl = PHP_ENUMITEM;
#define PHP_REL PHP_ATTR
  php_rel_decl = PHP_REL;
#define PHP_OPER "  ${comment}${final}${visibility}${abstract}${static}function ${name}${(}${)}\n{\n  ${body}}\n"
  // PHP_OPER known by php reverse
  php_oper_def = PHP_OPER;
  php_get_name = "get${Name}";
  php_get_final = TRUE;
  php_set_name = "set${Name}";
  php_set_final = FALSE;
  php_javadoc_comment = FALSE;
  php_req_with_path = FALSE;
  php_relative_path = FALSE;
  php_root_relative_path = FALSE;
  php_force_namespace_gen = FALSE;

  python_2_2 = TRUE;
  python_3_operation = FALSE;
#define PYTHON_INDENT_STEP "    "
  python_indent_step = PYTHON_INDENT_STEP;
#define PYTHON_CLASS "class ${name}${inherit}:\n${docstring}${members}\n"
  python_class_decl = PYTHON_CLASS;
  python_external_class_decl = "${name}";
#define PYTHON_ENUM "class ${name}:\n${docstring}${members}\n"
  python_enum_decl = PYTHON_ENUM;
#define PYTHON_ATTR1 "${comment}${self}${name} = ${value}\n"
#define PYTHON_ATTR2 "${comment}${self}${name} = ${stereotype}()\n"
  python_attr_decl[0] = PYTHON_ATTR1;
  python_attr_decl[1] = PYTHON_ATTR2;
#define PYTHON_ENUMITEM PYTHON_ATTR1
  python_enum_item_decl = PYTHON_ENUMITEM;
#define PYTHON_REL1 PYTHON_ATTR1
#define PYTHON_REL2 PYTHON_ATTR2
#define PYTHON_REL_COMP "${comment}${self}${name} = ${type}()\n"
  python_rel_decl[0][0] = PYTHON_REL1;
  python_rel_decl[0][1] = PYTHON_REL2;
  python_rel_decl[1][0] = PYTHON_REL_COMP;
  python_rel_decl[1][1] = PYTHON_REL2;
#define PYTHON_OPER "${@}${static}${abstract}def ${name}${(}${)}:\n${docstring}${body}\n"
  python_oper_def = PYTHON_OPER;
#define PYTHON_INITOPER "${@}${static}${abstract}def ${name}${(}${p0}${v0}${)}:\n${docstring}super(${class}, ${p0}).__init__()\n${body}\n"
  python_initoper_def = PYTHON_INITOPER;
  python_get_name = "get${Name}";
  python_set_name = "set${Name}";

#define  IDL_EXTERNAL_CLASS_DECL "${name}\n#include \"${name}.idl\"\n";
  idl_external_class_decl = IDL_EXTERNAL_CLASS_DECL;
  idl_interface_decl = "${comment}${abstract}${local}interface ${name}${inherit} {\n${members}};\n";
#define IDL_VALUETYPE_DECL "${comment}${abstract}${custom}valuetype ${name}${inherit} {\n${members}};\n";
  idl_valuetype_decl = IDL_VALUETYPE_DECL;
  idl_struct_decl = "${comment}struct ${name} {\n${members}};\n";
  idl_typedef_decl = "${comment}typedef ${type} ${name};\n";
  idl_exception_decl = "${comment}exception ${name} {\n${members}};\n";
  idl_union_decl = "${comment}union ${name} switch(${switch}) {\n${members}};\n";
  idl_enum_decl = "${comment}enum ${name} {\n${items}};\n";
#define IDL_ATTR_DECL1	"  ${comment}${readonly}${attribute}${stereotype}<${type}> ${name};\n"
#define IDL_ATTR_DECL2	"  ${comment}${readonly}${attribute}${stereotype}<${type},${multiplicity}> ${name};\n"
#define IDL_ATTR_DESCR1	"  ${description}${readonly}${attribute}${stereotype}<${type}> ${name};\n"
#define IDL_ATTR_DESCR2	"  ${description}${readonly}${attribute}${stereotype}<${type},${multiplicity}> ${name};\n"
  idl_attr_decl[0] = "  ${comment}${readonly}${attribute}${type} ${name};\n";
  idl_attr_decl[1] = IDL_ATTR_DECL1;
  idl_attr_decl[2] = IDL_ATTR_DECL2;
#define IDL_VALUETYPE_ATTRIBUTE_DECL0 "  ${comment}${visibility}${type} ${name};\n"
#define IDL_VALUETYPE_ATTRIBUTE_DECL1 "  ${comment}${visibility}${stereotype}<${type}> ${name};\n"
#define IDL_VALUETYPE_ATTRIBUTE_DECL2 "  ${comment}${visibility}${stereotype}<${type},${multiplicity}> ${name};\n"
#define IDL_VALUETYPE_ATTRIBUTE_DESCR1 "  ${description}${visibility}${stereotype}<${type}> ${name};\n"
#define IDL_VALUETYPE_ATTRIBUTE_DESCR2 "  ${description}${visibility}${stereotype}<${type},${multiplicity}> ${name};\n"
  idl_valuetype_attr_decl[0] = IDL_VALUETYPE_ATTRIBUTE_DECL0;
  idl_valuetype_attr_decl[1] = IDL_VALUETYPE_ATTRIBUTE_DECL1;
  idl_valuetype_attr_decl[2] = IDL_VALUETYPE_ATTRIBUTE_DECL2;
#define IDL_CONST_ATTR_DECL1  "  ${comment}const ${stereotype}<${type}> ${name}${value};\n"
#define IDL_CONST_ATTR_DECL2  "  ${comment}const ${stereotype}<${type},${multiplicity}> ${name}${value};\n"
#define IDL_CONST_ATTR_DESCR1  "  ${description}const ${stereotype}<${type}> ${name}${value};\n"
#define IDL_CONST_ATTR_DESCR2  "  ${description}const ${stereotype}<${type},${multiplicity}> ${name}${value};\n"
  idl_const_decl[0] = "  ${comment}const ${type} ${name}${value};\n";
  idl_const_decl[1] = IDL_CONST_ATTR_DECL1;
  idl_const_decl[2] = IDL_CONST_ATTR_DECL2;
#define IDL_UNION_ATTR_DECL1  "  ${comment}case ${case} : ${readonly}${stereotype}<${type}> ${name};";
#define IDL_UNION_ATTR_DECL2  "  ${comment}case ${case} : ${readonly}${stereotype}<${type},${multiplicity}> ${name};";
#define IDL_UNION_ATTR_DESCR1  "  ${description}case ${case} : ${readonly}${stereotype}<${type}> ${name};";
#define IDL_UNION_ATTR_DESCR2  "  ${description}case ${case} : ${readonly}${stereotype}<${type},${multiplicity}> ${name};";
  idl_union_item_decl[0] = "  ${comment}case ${case} : ${readonly}${type} ${name};";
  idl_union_item_decl[1] = IDL_UNION_ATTR_DECL1;
  idl_union_item_decl[2] = IDL_UNION_ATTR_DECL2;
  idl_enum_item_decl = "  ${name},${comment}";
  idl_rel_decl[0] = "  ${comment}${readonly}${attribute}${type} ${name};\n";
  idl_rel_decl[1] = "  ${comment}${readonly}${attribute}${stereotype}<${type}> ${name};\n";
  idl_rel_decl[2] = "  ${comment}${readonly}${attribute}${stereotype}<${type},${multiplicity}> ${name};\n";
#define IDL_VALUETYPE_REL_DECL0 "  ${comment}${visibility}${type} ${name};\n"
#define IDL_VALUETYPE_REL_DECL1 "  ${comment}${visibility}${stereotype}<${type}> ${name};\n"
#define IDL_VALUETYPE_REL_DECL2 "  ${comment}${visibility}${stereotype}<${type},${multiplicity}> ${name};\n"
#define IDL_VALUETYPE_REL_DESCR1 "  ${description}${visibility}${stereotype}<${type}> ${name};\n"
#define IDL_VALUETYPE_REL_DESCR2 "  ${description}${visibility}${stereotype}<${type},${multiplicity}> ${name};\n"
  idl_valuetype_rel_decl[0] = IDL_VALUETYPE_REL_DECL0;
  idl_valuetype_rel_decl[1] = IDL_VALUETYPE_REL_DECL1;
  idl_valuetype_rel_decl[2] = IDL_VALUETYPE_REL_DECL2;
  idl_union_rel_decl[0] = "  ${comment}case ${case} : ${readonly}${type} ${name};";
  idl_union_rel_decl[1] = "  ${comment}case ${case} : ${readonly}${stereotype}<${type}> ${name};";
  idl_union_rel_decl[2] = "  ${comment}case ${case} : ${readonly}${stereotype}<${type},${multiplicity}> ${name};";
  idl_oper_decl = "  ${comment}${oneway}${type} ${name}${(}${)}${raisesnl}${raises};\n";
  idl_get_name = "get_${name}";
  idl_set_name = "set_${name}";
  idl_set_oneway = FALSE;

  uml_get_name = UmlView;
  uml_set_name = UmlView;

  int i;
  
  umltypes.clear();
  
  for (i = 0; i != nbuiltins; i += 1)
    umltypes.append(builtins[i].uml);
  
  if (umltypes.findIndex("void") == -1)
    umltypes.append("void");
  
  cpp_includes.types.clear();
  cpp_includes.includes.clear();
  
  cpp_includes.types.append("vector");
  cpp_includes.includes.append("#include <vector>\nusing namespace std;");
  cpp_includes.types.append("list");
  cpp_includes.includes.append("#include <list>\nusing namespace std;");
  cpp_includes.types.append("map");
  cpp_includes.includes.append("#include <map>\nusing namespace std;");
  cpp_includes.types.append("string");
  cpp_includes.includes.append("#include <string>\nusing namespace std;");

  artifact_default_description = QString::null;
  class_default_description = QString::null;
  operation_default_description = QString::null;
  attribute_default_description = QString::null;
  relation_default_description = QString::null;
  
  cpp_root_dir = QString::null;
  java_root_dir = QString::null;
  php_root_dir = QString::null;
  python_root_dir = QString::null;
  idl_root_dir = QString::null;
  
  cpp_set_default_defs(FALSE);
  java_set_default_defs(FALSE);
  php_set_default_defs(FALSE);
  python_set_default_defs(FALSE);
  idl_set_default_defs(FALSE);
  
  cpp_dir_filter.case_sensitive = 
    cpp_file_filter.case_sensitive =
      java_dir_filter.case_sensitive = 
	java_file_filter.case_sensitive = 
	  php_dir_filter.case_sensitive =
	    php_file_filter.case_sensitive = FALSE;
}

int GenerationSettings::find_type(const QString & s) {
  int i;
  
  for (i = 0; i != nbuiltins; i += 1)
    if (builtins[i].uml == s)
      return i;
  
  return -1;
}

bool GenerationSettings::cpp_set_default_defs(bool y)
{ 
  BrowserPackage * p = BrowserView::get_project();
  
  if (p != 0)
    p->modified();
  
  return cpp_default_defs = y;
}

QString GenerationSettings::cpp_type(const QString & s)
{
  int index = find_type(s);
  
  return (index == -1) ? s : builtins[index].cpp;
}

bool GenerationSettings::java_set_default_defs(bool y)
{ 
  BrowserPackage * p = BrowserView::get_project();
  
  if (p != 0)
    p->modified();
  
  return java_default_defs = y;
}

QString GenerationSettings::java_type(const QString & s)
{
  int index = find_type(s);
  
  return (index == -1) ? s : builtins[index].java;
}

bool GenerationSettings::php_set_default_defs(bool y)
{ 
  BrowserPackage * p = BrowserView::get_project();
  
  if (p != 0)
    p->modified();
  
  return php_default_defs = y;
}

bool GenerationSettings::python_set_default_defs(bool y)
{ 
  BrowserPackage * p = BrowserView::get_project();
  
  if (p != 0)
    p->modified();
  
  return python_default_defs = y;
}

bool GenerationSettings::idl_set_default_defs(bool y)
{ 
  BrowserPackage * p = BrowserView::get_project();
  
  if (p != 0)
    p->modified();
  
  return idl_default_defs = y;
}

QString GenerationSettings::idl_type(const QString & s)
{
  int index = find_type(s);
  
  return (index == -1) ? s : builtins[index].idl;
}

static unsigned multiplicity_column(const QString & mult)
{
  if (mult.isEmpty() || (mult == "1"))
    return 0;

  if ((mult == "*") || (mult.find("..") != -1))
    return 1;

  return 2;
}

static unsigned python_multiplicity_column(const QString & mult)
{
  return (mult.isEmpty() || (mult == "1")) ? 0 : 1;
}

const QString GenerationSettings::cpp(const AType & type,
				      UmlParamDirection dir, int rank)
{
  QString s;
  int index;

  if ((type.type != 0) &&
      !strcmp(cpp_class_stereotype(((ClassData *) type.type->get_data())->get_stereotype()),
	      "enum")) {
    switch (dir) {
    case UmlIn:
      s = cpp_enum_in;
      break;
    case UmlOut:
      s = cpp_enum_out;
      break;
    default:
      // can't be return
      s = cpp_enum_inout;
      break;
    }
  }
  else {
    index = find_type(type.get_type());
    
    switch (dir) {
    case UmlIn:
      s = (index != -1) ? builtins[index].cpp_in : cpp_in;
      break;
    case UmlOut:
      s = (index != -1) ? builtins[index].cpp_out : cpp_out;
      break;
    default:
      // can't be return
      s = (index != -1) ? builtins[index].cpp_inout : cpp_inout;
      break;
    }
  }
  
  if ((index = s.find("${type}")) == -1)
    return s;
  
  QString t;
  
  t.sprintf("${t%d}", rank);
  return s.replace(index, 7, t) + " ${p" + QString::number(rank) + "}";
}

void GenerationSettings::set_cpp_return_type(const AType & type, QString & s)
{
  int index;
  
  if ((index = s.find("${type}")) == -1)
    return;
  
  if ((type.type != 0) &&
      !strcmp(cpp_class_stereotype(((ClassData *) type.type->get_data())->get_stereotype()),
	      "enum"))
    s.replace(index, 7, cpp_enum_return);
  else {
    int spec_index = find_type(type.get_type());    
    
    s.replace(index, 7,
	      (spec_index != -1) ? builtins[spec_index].cpp_return 
				 : cpp_return);
  }
}

const char *
  GenerationSettings::cpp_default_attr_decl(const QString & mult)
{
  return cpp_attr_decl[multiplicity_column(mult)];
}

const char *
  GenerationSettings::cpp_default_rel_decl(UmlCode rel,
					   const QString & mult)
{
  switch (rel) {
  default:
    return (const char *) 
      cpp_rel_decl[0][multiplicity_column((mult == "0..1") ? QString::null : mult)];
  case UmlAggregationByValue:
  case UmlDirectionalAggregationByValue:
    return (const char *) 
      cpp_rel_decl[1][multiplicity_column(mult)];
  }
}

const char *
  GenerationSettings::java_default_rel_decl(const QString & mult)
{
  return java_rel_decl[multiplicity_column(mult)];
}

const char *
  GenerationSettings::java_default_attr_decl(const QString & mult)
{
  return java_attr_decl[multiplicity_column(mult)];
}

const char *
  GenerationSettings::python_default_attr_decl(const QString & mult)
{
  return python_attr_decl[python_multiplicity_column(mult)];
}

const char *
  GenerationSettings::python_default_rel_decl(UmlCode rel,
					      const QString & mult)
{
  switch (rel) {
  default:
    return (const char *) 
      python_rel_decl[0][python_multiplicity_column((mult == "0..1") ? QString::null : mult)];
  case UmlAggregationByValue:
  case UmlDirectionalAggregationByValue:
    return (const char *) 
      python_rel_decl[1][python_multiplicity_column(mult)];
  }
}

const char *
  GenerationSettings::idl_default_attr_decl(const QString & mult)
{
  return idl_attr_decl[multiplicity_column(mult)];
}

const char *
  GenerationSettings::idl_default_valuetype_attr_decl(const QString & mult)
{
  return idl_valuetype_attr_decl[multiplicity_column(mult)];
}

const char *
  GenerationSettings::idl_default_const_decl(const QString & mult)
{
  return idl_const_decl[multiplicity_column(mult)];
}

const char *
  GenerationSettings::idl_default_union_item_decl(const QString & mult)
{
  return idl_union_item_decl[multiplicity_column(mult)];
}

const char *
  GenerationSettings::idl_default_rel_decl(const QString & mult)
{
  return idl_rel_decl[multiplicity_column(mult)];
}

const char *
  GenerationSettings::idl_default_valuetype_rel_decl(const QString & mult)
{
  return idl_valuetype_rel_decl[multiplicity_column(mult)];
}

const char *
  GenerationSettings::idl_default_union_rel_decl(const QString & mult)
{
  return idl_union_rel_decl[multiplicity_column(mult)];
}

int GenerationSettings::find_relationattribute_stereotype(const QString & s) {
  int i;
  
  for (i = 0; i != nrelattrstereotypes; i += 1)
    if (relattr_stereotypes[i].uml == s)
      return i;
  
  return -1;
}

QString GenerationSettings::cpp_relationattribute_stereotype(const QString & s) {
  int index = find_relationattribute_stereotype(s);
  
  return (index == -1) ? s : relattr_stereotypes[index].cpp;
}

QString GenerationSettings::java_relationattribute_stereotype(const QString & s) {
  int index = find_relationattribute_stereotype(s);
  
  return (index == -1) ? s : relattr_stereotypes[index].java;
}

QString GenerationSettings::python_relationattribute_stereotype(const QString & s) {
  int index = find_relationattribute_stereotype(s);
  
  return (index == -1) ? s : relattr_stereotypes[index].python;
}

QString GenerationSettings::idl_relationattribute_stereotype(const QString & s) {
  int index = find_relationattribute_stereotype(s);
  
  return (index == -1) ? s : relattr_stereotypes[index].idl;
}

int GenerationSettings::find_class_stereotype(const QString & s) {
  int i;
  
  for (i = 0; i != nclassstereotypes; i += 1)
    if (class_stereotypes[i].uml == s)
      return i;
  
  return -1;
}

QString GenerationSettings::cpp_class_stereotype(const QString & s) {
  int index = find_class_stereotype(s);
  
  return (index == -1) ? s : class_stereotypes[index].cpp;
}

QString GenerationSettings::java_class_stereotype(const QString & s) {
  int index = find_class_stereotype(s);
  
  return (index == -1) ? s : class_stereotypes[index].java;
}

QString GenerationSettings::php_class_stereotype(const QString & s) {
  int index = find_class_stereotype(s);
  
  return (index == -1) ? s : class_stereotypes[index].php;
}

QString GenerationSettings::python_class_stereotype(const QString & s) {
  int index = find_class_stereotype(s);
  
  return (index == -1) ? s : class_stereotypes[index].python;
}

QString GenerationSettings::idl_class_stereotype(const QString & s) {
  int index = find_class_stereotype(s);
  
  return (index == -1) ? s : class_stereotypes[index].idl;
}

bool GenerationSettings::edit() {
  GenerationSettingsDialog d;
  
  return (d.exec()== QDialog::Accepted);
}

void GenerationSettings::send_uml_def(ToolCom * com)
{
  int index;
  
  com->write_unsigned((unsigned) nbuiltins);
  
  for (index = 0; index != nbuiltins; index += 1)
    com->write_string(builtins[index].uml);
  
  com->write_unsigned((unsigned) nrelattrstereotypes);
  
  for (index = 0; index != nrelattrstereotypes; index += 1)
    com->write_string(relattr_stereotypes[index].uml);
  
  com->write_unsigned((unsigned) nclassstereotypes);
  
  for (index = 0; index != nclassstereotypes; index += 1)
    com->write_string(class_stereotypes[index].uml);
  
  int api_version = com->api_format();
  
  if (api_version >= 16) {
    com->write_string(artifact_default_description);
    com->write_string(class_default_description);
    com->write_string(operation_default_description);
    com->write_string(attribute_default_description);
    com->write_string(relation_default_description);
    
    if (api_version >= 28) {
      com->write_char(uml_get_name);
      com->write_char(uml_set_name);
    }
  }
}

void GenerationSettings::send_cpp_def(ToolCom * com)
{
  com->write_string(cpp_root_dir);
  
  int index;
  int api_version = com->api_format();
  
  com->write_unsigned((unsigned) nbuiltins);
  
  for (index = 0; index != nbuiltins; index += 1) {
    com->write_string(builtins[index].cpp);
    com->write_string(builtins[index].cpp_in);
    com->write_string(builtins[index].cpp_out);
    com->write_string(builtins[index].cpp_inout);
    if (api_version >= 16)
      com->write_string(builtins[index].cpp_return);
  }
  
  com->write_unsigned((unsigned) nrelattrstereotypes);
  
  for (index = 0; index != nrelattrstereotypes; index += 1)
    com->write_string(relattr_stereotypes[index].cpp);
  
  com->write_unsigned((unsigned) nclassstereotypes);
  
  for (index = 0; index != nclassstereotypes; index += 1)
    com->write_string(class_stereotypes[index].cpp);
  
  QStringList::Iterator it_t;
  QStringList::Iterator it_i;
  
  com->write_unsigned((unsigned) cpp_includes.types.count());
  
  for (it_t = cpp_includes.types.begin(), it_i = cpp_includes.includes.begin();
       it_t != cpp_includes.types.end();
       it_t++, it_i++) {
    com->write_string(*it_t);
    com->write_string(*it_i);
  }
  
  com->write_string(cpp_h_content);
  com->write_string(cpp_src_content);
  com->write_string(cpp_h_extension);
  com->write_string(cpp_src_extension);
  com->write_bool(cpp_include_with_path);
  
  com->write_string(cpp_in);
  com->write_string(cpp_out);
  com->write_string(cpp_inout);
  if (api_version >= 16)
    com->write_string(cpp_return);
  com->write_string(cpp_enum_in);
  com->write_string(cpp_enum_out);
  com->write_string(cpp_enum_inout);
  if (api_version >= 16)
    com->write_string(cpp_enum_return);
  com->write_string(cpp_class_decl);
  com->write_string(cpp_external_class_decl);
  com->write_string(cpp_struct_decl);
  com->write_string(cpp_union_decl);
  com->write_string(cpp_enum_decl);
  com->write_string(cpp_typedef_decl);
  com->write_string(cpp_attr_decl[0]);
  if (api_version >= 32) {
    com->write_string(cpp_attr_decl[1]);
    com->write_string(cpp_attr_decl[2]);
  }
  com->write_string(cpp_enum_item_decl);
  for (index = 0; index != 3; index += 1) {
    com->write_string(cpp_rel_decl[0][index]);
    com->write_string(cpp_rel_decl[1][index]);
  }
  com->write_string(cpp_oper_decl);
  com->write_string(cpp_oper_def);
  com->write_bool(cpp_force_throw);
  com->write_char(cpp_get_visibility);
  com->write_string(cpp_get_name);
  com->write_bool(cpp_get_inline);
  com->write_bool(cpp_get_const);
  com->write_bool(cpp_get_value_const);
  com->write_char(cpp_set_visibility);
  com->write_string(cpp_set_name);
  com->write_bool(cpp_set_inline);
  com->write_bool(cpp_set_param_const);
  if (api_version >= 26) {
    com->write_bool(cpp_set_param_ref);
    if (api_version >= 27) {
      com->write_bool(cpp_relative_path);
      com->write_bool(cpp_force_namespace_gen);
      if (api_version >= 29) {
	com->write_bool(cpp_root_relative_path);
	if (api_version >= 30) {
	  com->write_bool(cpp_javadoc_comment);
	  if (api_version >= 49) {
	    com->write_bool(cpp_inline_force_incl_in_h);
	    if (api_version >= 54) {
	      cpp_dir_filter.send_def(com);
	      cpp_file_filter.send_def(com);
	      if (api_version >= 55) {
		com->write_string(cpp_indent_visibility);
	      }
	    }
	  }
	}
      }
    }
  }
}

void GenerationSettings::send_java_def(ToolCom * com)
{
  int api_version = com->api_format();
  
  com->write_string(java_root_dir);
  
  int index;
  
  com->write_unsigned((unsigned) nbuiltins);
  
  for (index = 0; index != nbuiltins; index += 1) {
    com->write_string(builtins[index].java);
  }
  
  com->write_unsigned((unsigned) nrelattrstereotypes);
  
  for (index = 0; index != nrelattrstereotypes; index += 1)
    com->write_string(relattr_stereotypes[index].java);
  
  com->write_unsigned((unsigned) nclassstereotypes);
  
  for (index = 0; index != nclassstereotypes; index += 1)
    com->write_string(class_stereotypes[index].java);
  
  QStringList::Iterator it_t;
  QStringList::Iterator it_i;
  
  com->write_unsigned((unsigned) java_imports.types.count());
  
  for (it_t = java_imports.types.begin(), it_i = java_imports.includes.begin();
       it_t != java_imports.types.end();
       it_t++, it_i++) {
    com->write_string(*it_t);
    com->write_string(*it_i);
  }
  
  com->write_string(java_src_content);
  com->write_string(java_extension);
  
  com->write_string(java_class_decl);
  com->write_string(java_external_class_decl);
  if (api_version >= 18)
    com->write_string(java_enum_decl);
  com->write_string(java_enum_pattern_decl);
  com->write_string(java_interface_decl);
  com->write_string(java_attr_decl[0]);
  if (api_version >= 32) {
    com->write_string(java_attr_decl[1]);
    com->write_string(java_attr_decl[2]);
  }
  if (api_version >= 18)
    com->write_string(java_enum_item_decl);
  com->write_string(java_enum_pattern_item_decl);
  com->write_string(java_enum_pattern_item_case);
  for (index = 0; index != 3; index += 1)
    com->write_string(java_rel_decl[index]);
  com->write_string(java_oper_def);
  if (api_version >= 23)
    com->write_char(noncpp_get_visibility);
  else {
    switch (noncpp_get_visibility) {
    case UmlPackageVisibility:
      com->write_char(UmlPublic);
      break;
    case UmlDefaultVisibility:
      com->write_char(UmlDefaultVisibility - 1);
    default:
      com->write_char(noncpp_get_visibility);
    }
  }
  com->write_string(java_get_name);
  com->write_bool(java_get_final);
  if (api_version >= 23)
    com->write_char(noncpp_set_visibility);
  else {
    switch (noncpp_set_visibility) {
    case UmlPackageVisibility:
      com->write_char(UmlPublic);
      break;
    case UmlDefaultVisibility:
      com->write_char(UmlDefaultVisibility - 1);
    default:
      com->write_char(noncpp_set_visibility);
    }
  }
  com->write_string(java_set_name);
  com->write_bool(java_set_final);
  com->write_bool(java_set_param_final);
  if (api_version >= 30) {
    com->write_bool(java_javadoc_comment);
    if (api_version >= 40) {
      com->write_bool(java_force_package_gen);
      if (api_version >= 54) {
	java_dir_filter.send_def(com);
	java_file_filter.send_def(com);
      }
    }
  }
}

void GenerationSettings::send_php_def(ToolCom * com)
{
  int api_version = com->api_format();
  
  com->write_string(php_root_dir);
  
  int index;
  
  com->write_unsigned((unsigned) nclassstereotypes);
  
  for (index = 0; index != nclassstereotypes; index += 1)
    com->write_string(class_stereotypes[index].php);
  
  QStringList::Iterator it_t;
  QStringList::Iterator it_i;
  
  com->write_string(php_src_content);
  com->write_string(php_extension);
  
  com->write_string(php_class_decl);
  com->write_string(php_external_class_decl);
  com->write_string(php_enum_decl);
  com->write_string(php_interface_decl);
  com->write_string(php_attr_decl);
  com->write_string(php_enum_item_decl);
  com->write_string(php_rel_decl);
  com->write_string(php_oper_def);
  com->write_char(noncpp_get_visibility);
  com->write_string(php_get_name);
  com->write_bool(php_get_final);
  com->write_char(noncpp_get_visibility);
  com->write_string(php_set_name);
  com->write_bool(php_set_final);
  if (api_version >= 38) {
    com->write_bool(php_javadoc_comment);
    if (api_version >= 54) {      
      com->write_bool(php_req_with_path);    
      com->write_bool(php_relative_path);    
      com->write_bool(php_root_relative_path);
      php_dir_filter.send_def(com);
      php_file_filter.send_def(com);
      
      if (api_version >= 55) {
	com->write_bool(php_force_namespace_gen);
      }
    }
  }
}

void GenerationSettings::send_python_def(ToolCom * com)
{
  //int api_version = com->api_format();
  
  com->write_bool((python_2_2) ? 1 : 0);
  com->write_string(python_indent_step);
  com->write_string(python_root_dir);
  
  int index;
  
  com->write_unsigned((unsigned) nrelattrstereotypes);
  
  for (index = 0; index != nrelattrstereotypes; index += 1)
    com->write_string(relattr_stereotypes[index].python);
  
  com->write_unsigned((unsigned) nclassstereotypes);
  
  for (index = 0; index != nclassstereotypes; index += 1)
    com->write_string(class_stereotypes[index].python);
  
  QStringList::Iterator it_t;
  QStringList::Iterator it_i;
  
  com->write_unsigned((unsigned) python_imports.types.count());
  
  for (it_t = python_imports.types.begin(), it_i = python_imports.includes.begin();
       it_t != python_imports.types.end();
       it_t++, it_i++) {
    com->write_string(*it_t);
    com->write_string(*it_i);
  }
    
  com->write_string(python_src_content);
  com->write_string(python_extension);
  
  com->write_string(python_class_decl);
  com->write_string(python_external_class_decl);
  com->write_string(python_enum_decl);
  com->write_string(python_attr_decl[0]);
  com->write_string(python_attr_decl[1]);
  com->write_string(python_enum_item_decl);
  com->write_string(python_rel_decl[0][0]);
  com->write_string(python_rel_decl[0][1]);
  com->write_string(python_rel_decl[1][0]);
  com->write_string(python_rel_decl[1][1]);
  com->write_string(python_oper_def);
  com->write_string(python_get_name);
  com->write_string(python_set_name);
  
  int api = com->api_format();
  
  if (api > 43) {
    com->write_string(python_initoper_def);
    if (api > 47)
      com->write_bool((python_3_operation) ? 1 : 0);
  }
}

void GenerationSettings::send_idl_def(ToolCom * com)
{
  int api_version = com->api_format();

  com->write_string(idl_root_dir);
  
  int index;
  
  com->write_unsigned((unsigned) nbuiltins);
  
  for (index = 0; index != nbuiltins; index += 1) {
    com->write_string(builtins[index].idl);
  }
  
  com->write_unsigned((unsigned) nrelattrstereotypes);
  
  for (index = 0; index != nrelattrstereotypes; index += 1)
    com->write_string(relattr_stereotypes[index].idl);
  
  com->write_unsigned((unsigned) nclassstereotypes);
  
  for (index = 0; index != nclassstereotypes; index += 1)
    com->write_string(class_stereotypes[index].idl);
  
  QStringList::Iterator it_t;
  QStringList::Iterator it_i;
  
  com->write_unsigned((unsigned) idl_includes.types.count());
  
  for (it_t = idl_includes.types.begin(), it_i = idl_includes.includes.begin();
       it_t != idl_includes.types.end();
       it_t++, it_i++) {
    com->write_string(*it_t);
    com->write_string(*it_i);
  }
  
  com->write_string(idl_src_content);
  com->write_string(idl_extension);

  com->write_string(idl_interface_decl);
  com->write_string(idl_valuetype_decl);
  com->write_string(idl_struct_decl);
  com->write_string(idl_typedef_decl);
  com->write_string(idl_exception_decl);
  com->write_string(idl_union_decl);
  com->write_string(idl_enum_decl);
  com->write_string(idl_external_class_decl);
  if (api_version >= 32) {
    com->write_string(idl_enum_item_decl);
    for (index = 0; index != 3; index += 1) {
      com->write_string(idl_attr_decl[index]);
      com->write_string(idl_valuetype_attr_decl[index]);
      com->write_string(idl_union_item_decl[index]);
      com->write_string(idl_const_decl[index]);
      com->write_string(idl_rel_decl[index]);
      com->write_string(idl_valuetype_rel_decl[index]);
      com->write_string(idl_union_rel_decl[index]);
    }
  }
  else {
    com->write_string(idl_attr_decl[0]);
    com->write_string(idl_valuetype_attr_decl[0]);
    com->write_string(idl_union_item_decl[0]);
    com->write_string(idl_enum_item_decl);
    com->write_string(idl_const_decl[0]);

    for (index = 0; index != 3; index += 1) {
      com->write_string(idl_rel_decl[index]);
      com->write_string(idl_valuetype_rel_decl[index]);
      com->write_string(idl_union_rel_decl[index]);
    }
  }
  com->write_string(idl_oper_decl);
  com->write_string(idl_get_name);
  com->write_string(idl_set_name);
  com->write_bool(idl_set_oneway);
}

bool GenerationSettings::tool_global_uml_cmd(ToolCom * com, const char * args)
{
  switch ((unsigned char) args[-1]) {  
  case getUmlSettingsCmd:
    send_uml_def(com);
    break;
  default:
    // set cmds only
    if (!BrowserView::get_project()->is_writable())
      com->write_bool(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case setDefaultArtifactDescriptionCmd:
	artifact_default_description = com->get_string(args);
	break;
      case setDefaultClassDescriptionCmd:
	class_default_description = com->get_string(args);
	break;
      case setDefaultOperationDescriptionCmd:
	operation_default_description = com->get_string(args);
	break;
      case setDefaultAttributeDescriptionCmd:
	attribute_default_description = com->get_string(args);
	break;
      case setDefaultRelationDescriptionCmd:
	relation_default_description = com->get_string(args);
	break;
      case setUmlDefaultGetNameCmd:
	if (((unsigned char) *args) >= DefaultDrawingLanguage) {
	  com->write_bool(FALSE);
	  return TRUE;
	}
	uml_get_name = (DrawingLanguage) *args;
	break;
      case setUmlDefaultSetNameCmd:
	if (((unsigned char) *args) >= DefaultDrawingLanguage) {
	  com->write_bool(FALSE);
	  return TRUE;
	}
	uml_set_name = (DrawingLanguage) *args;
	break;
      default:
	com->write_bool(FALSE);
	return TRUE;
      }
      // ok case
      com->write_bool(TRUE);
      BrowserView::get_project()->package_modified();
    }
  }
  
  // ok case
  return TRUE;
}

Builtin & GenerationSettings::get_type(const char * u)
{
  int index;
  
  for (index = 0; index != nbuiltins; index += 1)
    if (builtins[index].uml == u)
      return builtins[index];
  
  // not find, add it
  
  Builtin * b = new Builtin[index + 1];
  
  for (index = 0; index != nbuiltins; index += 1)
    b[index] = builtins[index];
  
  b[index].set(u, u, u, u);
  b[index].cpp_in =  "const ${type}";

  if (builtins)
    delete [] builtins;
  
  builtins = b;
  nbuiltins += 1;
  
  return b[index];
}

Stereotype & GenerationSettings::get_stereotype(int & n, Stereotype * & st,
						const char * u)
{
  int index;
  
  for (index = 0; index != n; index += 1)
    if (st[index].uml == u)
      return st[index];
  
  // not find, add it
  
  Stereotype * s = new Stereotype[index + 1];
  
  for (index = 0; index != n; index += 1)
    s[index] = st[index];
  
  s[index].set(u, u, u, u, u, u);

  if (st)
    delete [] st;
  
  st = s;
  n += 1;
  
  return st[index];
}

void GenerationSettings::set_include(IncludesSpec & is, ToolCom * com,
				     const char * args)
{
  QString t = com->get_string(args);
  
  QStringList::Iterator it_t;
  QStringList::Iterator it_i;
  
  for (it_t = is.types.begin(), it_i = is.includes.begin();
       it_t != is.types.end();
       it_t++, it_i++) {
    if ((*it_t) == t) {
      *it_i = args;
      return;
    }
  }
  
  // does not exist, add it
  is.types.append(t);
  is.includes.append(args);
}

bool GenerationSettings::tool_global_cpp_cmd(ToolCom * com,
					     const char * args)
{
  switch ((unsigned char) args[-1]) {  
  case getCppSettingsCmd:
    send_cpp_def(com);
    break;
  case getCppUseDefaultsCmd:
    com->write_bool(cpp_get_default_defs());
    break;
  default:
    // set cmds only
    if (!BrowserView::get_project()->is_writable())
      com->write_bool(FALSE);
    else {
      int api_version = com->api_format();

      switch ((unsigned char) args[-1]) {
      case setCppUseDefaultsCmd:
	cpp_set_default_defs(*args);
	break;
      case setCppTypeCmd:
	{
	  const char * u = com->get_string(args);
	  
	  get_type(u).cpp = args;
	}
	break;
      case setCppRelationAttributeStereotypeCmd:
	{
	  const char * u = com->get_string(args);
	  
	  get_stereotype(nrelattrstereotypes, relattr_stereotypes, u).cpp = args;
	}
	break;
      case setCppClassStereotypeCmd:
	{
	  const char * u = com->get_string(args);
	  
	  get_stereotype(nclassstereotypes, class_stereotypes, u).cpp = args;
	}
	break;
      case setCppIncludeCmd:
	set_include(cpp_includes, com, args);
	break;
      case setCppHeaderContentCmd:
	cpp_h_content = args;
	break;
      case setCppRootDirCmd:
	cpp_root_dir = args;
	break;
      case setCppSourceContentCmd:
	cpp_src_content = args;
	break;
      case setCppHeaderExtensionCmd:
	cpp_h_extension = args;
	break;
      case setCppSourceExtensionCmd:
	cpp_src_extension = args;
	break;
      case setCppIncludeWithPathCmd:
	cpp_include_with_path = (*args != 0);
	break;
      case setCppRelativePathCmd:
	cpp_relative_path = (*args != 0);
	if (cpp_relative_path)
	  cpp_root_relative_path = FALSE;
	break;
      case setCppRootRelativePathCmd:
	cpp_root_relative_path = (*args != 0);
	if (cpp_root_relative_path)
	  cpp_relative_path = FALSE;
	break;
      case setCppForceNamespaceGenCmd:
	cpp_force_namespace_gen = (*args != 0);
	break;
      case setCppInlineOperForceInclInHeaderCmd:
	cpp_inline_force_incl_in_h = (*args != 0);
        break;
      case setCppEnumInCmd:
	cpp_enum_in = args;
	break;
      case setCppEnumOutCmd:
	cpp_enum_out = args;
	break;
      case setCppEnumInOutCmd:
	cpp_enum_inout = args;
	break;
      case setCppEnumReturnCmd:
	cpp_enum_return = args;
	break;
      case setCppInCmd:
	{
	  const char * s1 = com->get_string(args);
	  const char * s2 = (api_version < 41) ? "" : args;
	  
	  if (*s2)
	    get_type(s1).cpp_in = s2;
	  else
	    cpp_in = s1;
	}
	break;
      case setCppOutCmd:
	{
	  const char * s1 = com->get_string(args);
	  const char * s2 = (api_version < 41) ? "" : args;
	  
	  if (*s2)
	    get_type(s1).cpp_out = s2;
	  else
	    cpp_out = s1;
	}
	break;
      case setCppInOutCmd:
	{
	  const char * s1 = com->get_string(args);
	  const char * s2 = (api_version < 41) ? "" : args;
	  
	  if (*s2)
	    get_type(s1).cpp_inout = s2;
	  else
	    cpp_inout = s1;
	}
	break;
      case setCppReturnCmd:
	{
	  const char * s1 = com->get_string(args);
	  const char * s2 = (api_version < 41) ? "" : args;
	  
	  if (*s2)
	    get_type(s1).cpp_return = s2;
	  else
	    cpp_return = s1;
	}
	break;
      case setCppClassDeclCmd:
	cpp_class_decl = args;
	break;
      case setCppExternalClassDeclCmd:
	cpp_external_class_decl = args;
	break;
      case setCppStructDeclCmd:
	cpp_struct_decl = args;
	break;
      case setCppUnionDeclCmd:
	cpp_union_decl = args;
	break;
      case setCppEnumDeclCmd:
	cpp_enum_decl = args;
	break;
      case setCppTypedefDeclCmd:
	cpp_typedef_decl = args;
	break;
      case setCppAttributeDeclCmd:
	if (api_version >= 32) {
	  int i = multiplicity_column(com->get_string(args));

	  cpp_attr_decl[i] = args;
	}
	else
	  cpp_attr_decl[0] = args;
	break;
      case setCppEnumItemDeclCmd:
	cpp_enum_item_decl = args;
	break;
      case setCppRelationDeclCmd:
	{
	  int i1 = (*args++) ? 1 : 0;
	  int i2 = multiplicity_column(com->get_string(args));
	  
	  cpp_rel_decl[i1][i2] = args;
	}
	break;
      case setCppOperationDeclCmd:
	cpp_oper_decl = args;
	break;
      case setCppOperationDefCmd:
	cpp_oper_def = args;
	break;
      case setCppOperationForceThrowCmd:
	cpp_force_throw = (*args != 0);
	break;
      case setCppGetVisibilityCmd:
	{
	  UmlVisibility v;
	  
	  if (! com->get_visibility(v, args)) {
	    com->write_bool(FALSE);
	    return TRUE;
	  }
	  else
	    cpp_get_visibility = v;
	}
	break;
      case setCppGetNameCmd:
	cpp_get_name = args;
	break;
      case setCppIsGetInlineCmd:
	cpp_get_inline = (*args != 0);
	break;
      case setCppIsGetConstCmd:
	cpp_get_const = (*args != 0);
	break;
      case setCppIsGetValueConstCmd:
	cpp_get_value_const = (*args != 0);
	break;
      case setCppSetVisibilityCmd:
	{
	  UmlVisibility v;
	  
	  if (! com->get_visibility(v, args)) {
	    com->write_bool(FALSE);
	    return TRUE;
	  }
	  else
	    cpp_set_visibility = v;
	}
	break;
      case setCppSetNameCmd:
	cpp_set_name = args;
	break;
      case setCppIsSetInlineCmd:
	cpp_set_inline = (*args != 0);
	break;
      case setCppIsSetParamConstCmd:
	cpp_set_param_const = (*args != 0);
	break;
      case setCppIsSetParamRefCmd:
	cpp_set_param_ref = (*args != 0);
	break;
      case setCppJavadocStyleCmd:
	cpp_javadoc_comment = (*args != 0);
	break;
      case setCppDirRevFilterCmd:
	cpp_dir_filter.receive_def(args);
	break;
      case setCppFileRevFilterCmd:
	cpp_file_filter.receive_def(args);
	break;
      case setCppIndentVisibilityCmd:
	cpp_indent_visibility = args;
	break;
      default:
	return FALSE;
      }
      com->write_bool(TRUE);
      BrowserView::get_project()->package_modified();
    }
  }
  
  return TRUE;
}

bool GenerationSettings::tool_global_java_cmd(ToolCom * com,
					      const char * args)
{
  switch ((unsigned char) args[-1]) {  
  case getJavaSettingsCmd:
    send_java_def(com);
    break;
  case getJavaUseDefaultsCmd:
    com->write_bool(java_get_default_defs());
    break;
  default:
    // set cmds only
    if (!BrowserView::get_project()->is_writable())
      com->write_bool(FALSE);
    else {
      int api_version = com->api_format();

      switch ((unsigned char) args[-1]) {
      case setJavaUseDefaultsCmd:
	java_set_default_defs(*args);
	break;
      case setJavaTypeCmd:
	{
	  const char * u = com->get_string(args);
	  
	  get_type(u).java = args;
	}
	break;
      case setJavaRelationAttributeStereotypeCmd:
	{
	  const char * u = com->get_string(args);
	  
	  get_stereotype(nrelattrstereotypes, relattr_stereotypes, u).java = args;
	}
	break;
      case setJavaClassStereotypeCmd:
	{
	  const char * u = com->get_string(args);
	  
	  get_stereotype(nclassstereotypes, class_stereotypes, u).java = args;
	}
	break;
      case setJavaImportCmd:
	set_include(java_imports, com, args);
	break;
      case setJavaRootdirCmd:
	java_root_dir = args;
	break;
      case setJavaSourceContentCmd:
	break;
      case setJavaSourceExtensionCmd:
	java_extension = args;
	break;
      case setJavaClassDeclCmd:
	java_class_decl = args;
	break;
      case setJavaExternalClassDeclCmd:
	java_external_class_decl = args;
	break;
      case setJavaEnumDeclCmd:
	java_enum_decl = args;
	break;
      case setJavaEnumPatternDeclCmd:
	java_enum_pattern_decl = args;
	break;
      case setJavaInterfaceDeclCmd:
	java_interface_decl = args;
	break;
      case setJavaAttributeDeclCmd:
	if (api_version >= 32) {
	  int i = multiplicity_column(com->get_string(args));

	  java_attr_decl[i] = args;
	}
	else
	  java_attr_decl[0] = args;
	break;
      case setJavaEnumItemDeclCmd:
	java_enum_item_decl = args;
	break;
      case setJavaEnumPatternItemDeclCmd:
	java_enum_pattern_item_decl = args;
	break;
      case setJavaEnumPatternItemCaseCmd:
	java_enum_pattern_item_case = args;
	break;
      case setJavaRelationDeclCmd:
	{
	  int i = multiplicity_column(com->get_string(args));
	  
	  java_rel_decl[i] = args;
	    
	}
	break;
      case setJavaOperationDefCmd:
	java_oper_def = args;
	break;
      case setJavaGetvisibilityCmd:
	{
	  UmlVisibility v;
	  
	  if (! com->get_visibility(v, args)) {
	    com->write_bool(FALSE);
	    return TRUE;
	  }
	  else
	    noncpp_get_visibility = v;
	}
	break;
      case setJavaGetNameCmd:
	java_get_name = args;
	break;
      case setJavaSetNameCmd:
	java_set_name = args;
	break;
      case setJavaIsGetFinalCmd:
	java_get_final = (*args != 0);
	break;
      case setJavaIsSetFinalCmd:
	java_set_final = (*args != 0);
	break;
      case setJavaSetVisibilityCmd:
	{
	  UmlVisibility v;
	  
	  if (! com->get_visibility(v, args)) {
	    com->write_bool(FALSE);
	    return TRUE;
	  }
	  else
	    noncpp_set_visibility = v;
	}
	break;
      case setJavaIsSetParamFinalCmd:
	java_set_param_final = (*args != 0);
	break;
      case setJavaJavadocStyleCmd:
	java_javadoc_comment = (*args != 0);
	break;
      case setJavaForcePackageGenCmd:
	java_force_package_gen = (*args != 0);
	break;
      case setJavaDirRevFilterCmd:
	java_dir_filter.receive_def(args);
	break;
      case setJavaFileRevFilterCmd:
	java_file_filter.receive_def(args);
	break;
      default:
	return FALSE;
      }
      com->write_bool(TRUE);
      BrowserView::get_project()->package_modified();
    }
  }
  
  return TRUE;
}

bool GenerationSettings::tool_global_php_cmd(ToolCom * com,
					      const char * args)
{
  switch ((unsigned char) args[-1]) {  
  case getPhpSettingsCmd:
    send_php_def(com);
    break;
  case getPhpUseDefaultsCmd:
    com->write_bool(php_get_default_defs());
    break;
  default:
    // set cmds only
    if (!BrowserView::get_project()->is_writable())
      com->write_bool(FALSE);
    else {
      //int api_version = com->api_format();

      switch ((unsigned char) args[-1]) {
      case setPhpUseDefaultsCmd:
	php_set_default_defs(*args);
	break;
      case setPhpClassStereotypeCmd:
	{
	  const char * u = com->get_string(args);
	  
	  get_stereotype(nclassstereotypes, class_stereotypes, u).php = args;
	}
	break;
      case setPhpRootdirCmd:
	php_root_dir = args;
	break;
      case setPhpSourceContentCmd:
	break;
      case setPhpSourceExtensionCmd:
	php_extension = args;
	break;
      case setPhpClassDeclCmd:
	php_class_decl = args;
	break;
      case setPhpExternalClassDeclCmd:
	php_external_class_decl = args;
	break;
      case setPhpEnumDeclCmd:
	php_enum_decl = args;
	break;
      case setPhpInterfaceDeclCmd:
	php_interface_decl = args;
	break;
      case setPhpAttributeDeclCmd:
	php_attr_decl = args;
	break;
      case setPhpEnumItemDeclCmd:
	php_enum_item_decl = args;
	break;
      case setPhpRelationDeclCmd:
	php_rel_decl = args;
	break;
      case setPhpOperationDefCmd:
	php_oper_def = args;
	break;
      case setPhpGetNameCmd:
	php_get_name = args;
	break;
      case setPhpSetNameCmd:
	php_set_name = args;
	break;
      case setPhpIsGetFinalCmd:
	php_get_final = (*args != 0);
	break;
      case setPhpIsSetFinalCmd:
	php_set_final = (*args != 0);
	break;
      case setPhpJavadocStyleCmd:
	php_javadoc_comment = (*args != 0);
	break;
      case setPhpRequireOnceWithPathCmd:
	php_req_with_path = (*args != 0);
	break;
      case setPhpRelativePathCmd:
	php_relative_path = (*args != 0);
	if (php_relative_path)
	  php_root_relative_path = FALSE;
	break;
      case setPhpRootRelativePathCmd:
	php_root_relative_path = (*args != 0);
	if (php_root_relative_path)
	  php_relative_path = FALSE;
	break;
      case setPhpDirRevFilterCmd:
	php_dir_filter.receive_def(args);
	break;
      case setPhpFileRevFilterCmd:
	php_file_filter.receive_def(args);
	break;
      case setPhpForceNamespaceGenCmd:
	php_force_namespace_gen = (*args != 0);
	break;
      default:
	return FALSE;
      }
      com->write_bool(TRUE);
      BrowserView::get_project()->package_modified();
    }
  }
  
  return TRUE;
}

bool GenerationSettings::tool_global_python_cmd(ToolCom * com,
						const char * args)
{
  switch ((unsigned char) args[-1]) {  
  case getPythonSettingsCmd:
    send_python_def(com);
    break;
  case getPythonUseDefaultsCmd:
    com->write_bool(python_get_default_defs());
    break;
  default:
    // set cmds only
    if (!BrowserView::get_project()->is_writable())
      com->write_bool(FALSE);
    else {
      //int api_version = com->api_format();

      switch ((unsigned char) args[-1]) {
      case setPythonUseDefaultsCmd:
	python_set_default_defs(*args);
	break;
      case setPython22Cmd:
	python_2_2 = *args != 0;
	break;
      case setPythonIndentStepCmd:
	python_indent_step = args;
	break;
      case setPythonRelationAttributeStereotypeCmd:
	{
	  const char * u = com->get_string(args);
	  
	  get_stereotype(nrelattrstereotypes, relattr_stereotypes, u).python = args;
	}
	break;
      case setPythonClassStereotypeCmd:
	{
	  const char * u = com->get_string(args);
	  
	  get_stereotype(nclassstereotypes, class_stereotypes, u).python = args;
	}
	break;
      case setPythonImportCmd:
	set_include(python_imports, com, args);
	break;
      case setPythonRootdirCmd:
	python_root_dir = args;
	break;
      case setPythonSourceContentCmd:
	break;
      case setPythonSourceExtensionCmd:
	python_extension = args;
	break;
      case setPythonClassDeclCmd:
	python_class_decl = args;
	break;
      case setPythonExternalClassDeclCmd:
	python_external_class_decl = args;
	break;
      case setPythonEnumDeclCmd:
	python_enum_decl = args;
	break;
      case setPythonAttributeDeclCmd:
	{
	  int i = python_multiplicity_column(com->get_string(args));

	  python_attr_decl[i] = args;
	}
	break;
      case setPythonEnumItemDeclCmd:
	python_enum_item_decl = args;
	break;
      case setPythonRelationDeclCmd:
	{
	  int i1 = (*args++) ? 1 : 0;
	  int i2 = python_multiplicity_column(com->get_string(args));
	  
	  python_rel_decl[i1][i2] = args;
	}
	break;
      case setPythonOperationDefCmd:
	python_oper_def = args;
	break;
      case setPythonGetNameCmd:
	python_get_name = args;
	break;
      case setPythonSetNameCmd:
	python_set_name = args;
	break;
      case setPythonInitOperationDefCmd:
	python_initoper_def = args;
	break;
      case setPython3OperationCmd:
	python_3_operation = *args != 0;
	break;
      default:
	return FALSE;
      }
      com->write_bool(TRUE);
      BrowserView::get_project()->package_modified();
    }
  }
  
  return TRUE;
}

bool GenerationSettings::tool_global_idl_cmd(ToolCom * com,
					     const char * args)
{
  switch ((unsigned char) args[-1]) {  
  case getIdlSettingsCmd:
    send_idl_def(com);
    break;
  case getIdlUseDefaultsCmd:
    com->write_bool(idl_get_default_defs());
    break;
  default:
    // set cmds only
    if (!BrowserView::get_project()->is_writable())
      com->write_bool(FALSE);
    else {
      int api_version = com->api_format();

      switch ((unsigned char) args[-1]) {
      case setIdlUseDefaultsCmd:
	idl_set_default_defs(*args);
	break;
      case setIdlTypeCmd:
	{
	  const char * u = com->get_string(args);
	  
	  get_type(u).idl = args;
	}
	break;
      case setIdlRelationAttributeStereotypeCmd:
	{
	  const char * u = com->get_string(args);
	  
	  get_stereotype(nrelattrstereotypes, relattr_stereotypes, u).idl = args;
	}
	break;
      case setIdlClassStereotypeCmd:
	{
	  const char * u = com->get_string(args);
	  
	  get_stereotype(nclassstereotypes, class_stereotypes, u).idl = args;
	}
	break;
      case setIdlIncludeCmd:
	set_include(idl_includes, com, args);
	break;
      case setIdlRootDirCmd:
	idl_root_dir = args;
	break;
      case setIdlSourceContentCmd:
	idl_src_content = args;
	break;
      case setIdlSourceExtensionCmd:
	idl_extension = args;
	break;
      case setIdlInterfaceDeclCmd:
	idl_interface_decl = args;
	break;
      case setIdlValuetypeDeclCmd:
	idl_valuetype_decl = args;
	break;
      case setIdlStructDeclCmd:
	idl_struct_decl = args;
	break;
      case setIdlTypedefDeclCmd:
	idl_typedef_decl = args;
	break;
      case setIdlExceptionDeclCmd:
	idl_exception_decl = args;
	break;
      case setIdlUnionDeclCmd:
	idl_union_decl = args;
	break;
      case setIdlEnumDeclCmd:
	idl_enum_decl = args;
	break;
      case setIdlExternalClassDeclCmd:
	idl_external_class_decl = args;
	break;
      case setIdlAttributeDeclCmd:
	if (api_version >= 32) {
	  int i = multiplicity_column(com->get_string(args));

	  idl_attr_decl[i] = args;
	}
	else
	  idl_attr_decl[0] = args;
	break;
      case setIdlValuetypeAttributeDeclCmd:
	if (api_version >= 32) {
	  int i = multiplicity_column(com->get_string(args));

	  idl_valuetype_attr_decl[i] = args;
	}
	else
	  idl_valuetype_attr_decl[0] = args;
	break;
      case setIdlUnionItemDeclCmd:
	if (api_version >= 32) {
	  int i = multiplicity_column(com->get_string(args));

	  idl_union_item_decl[i] = args;
	}
	else
	  idl_union_item_decl[0] = args;
	break;
      case setIdlEnumItemDeclCmd:
	idl_enum_item_decl = args;
	break;
      case setIdlConstDeclCmd:
	if (api_version >= 32) {
	  int i = multiplicity_column(com->get_string(args));

	  idl_const_decl[i] = args;
	}
	else
	  idl_const_decl[0] = args;
	break;
      case setIdlRelationDeclCmd:
	{
	  int i = multiplicity_column(com->get_string(args));
	  
	  idl_rel_decl[i] = args;   
	}
	break;
      case setIdlValuetypeRelationDeclCmd:
	{
	  int i = multiplicity_column(com->get_string(args));
	  
	  idl_valuetype_rel_decl[i] = args;   
	}
	break;
      case setIdlUnionRelationDeclCmd:
	{
	  int i = multiplicity_column(com->get_string(args));
	  
	  idl_rel_decl[i] = args;   
	}
	break;
      case setIdlOperationDeclCmd:
	idl_oper_decl = args;
	break;
      case setIdlGetNameCmd:
	idl_get_name = args;
	break;
      case setIdlSetNameCmd:
	idl_set_name = args;
	break;
      case setIdlIsSetOneWayCmd:
	idl_set_oneway = (*args != 0);
	break;
      default:
	return FALSE;
      }
      com->write_bool(TRUE);
      BrowserView::get_project()->package_modified();
    }
  }
  
  return TRUE;
}

static void save_includes_imports(IncludesSpec & sp, const char * filename)
{
  QByteArray newdef;
  QTextOStream st(newdef);
	
  st.setEncoding(QTextStream::Latin1);

  st << "// \"a type\" \"needed " << filename << "\"\n";
    
  int sup = sp.types.count();
  QStringList::Iterator it_type = sp.types.begin();
  QStringList::Iterator it_incl = sp.includes.begin();
  
  for (int index = 0; index != sup; index += 1, it_type++, it_incl++) {
    save_string(*it_type, st);
    st << ' ';
    save_string(*it_incl, st);
    st << "\n\n";
  }

  st << '\000';
  save_if_needed(filename, newdef);
}

void GenerationSettings::save_dirs(QTextStream & st)
{
  if (!cpp_root_dir.isEmpty()) {
    nl_indent(st);
    st << "cpp_root_dir ";
    save_string(cpp_root_dir, st);
  }
  if (!java_root_dir.isEmpty()) {
    nl_indent(st);
    st << "java_root_dir ";
    save_string(java_root_dir, st);
  }
  if (!php_root_dir.isEmpty()) {
    nl_indent(st);
    st << "php_root_dir ";
    save_string(php_root_dir, st);
  }
  if (!python_root_dir.isEmpty()) {
    nl_indent(st);
    st << "python_root_dir ";
    save_string(python_root_dir, st);
  }
  if (!idl_root_dir.isEmpty()) {
    nl_indent(st);
    st << "idl_root_dir ";
    save_string(idl_root_dir, st);
  }

  st << '\n';
}

void GenerationSettings::save_descriptions(QTextStream & st)
{
  if (! artifact_default_description.isEmpty()) {
    nl_indent(st);
    st << "artifact_default_description ";
    save_string(artifact_default_description, st);
  }
  if (! class_default_description.isEmpty()) {
    nl_indent(st);
    st << "class_default_description ";
    save_string(class_default_description, st);
  }
  if (! operation_default_description.isEmpty()) {
    nl_indent(st);
    st << "operation_default_description ";
    save_string(operation_default_description, st);
  }
  if (! attribute_default_description.isEmpty()) {
    nl_indent(st);
    st << "attribute_default_description ";
    save_string(attribute_default_description, st);
  }
  if (! relation_default_description.isEmpty()) {
    nl_indent(st);
    st << "relation_default_description ";
    save_string(relation_default_description, st);
  }

  st << '\n';
}

void GenerationSettings::save()
{
  QByteArray newdef;
  QTextOStream st(newdef);
	
  st.setEncoding(QTextStream::Latin1);
  
  nl_indent(st);
  
  if (cpp_default_defs)
    st << "cpp_default_defs ";
  if (java_default_defs)
    st << "java_default_defs ";
  if (php_default_defs)
    st << "php_default_defs ";
  if (python_default_defs)
    st << "python_default_defs ";
  if (idl_default_defs)
    st << "idl_default_defs ";
  
  nl_indent(st);
  st << "cpp_h_extension ";
  save_string(cpp_h_extension, st);
  st << " cpp_src_extension ";
  save_string(cpp_src_extension, st);
  st << " java_extension ";
  save_string(java_extension, st);
  st << " php_extension ";
  save_string(php_extension, st);
  st << " python_extension ";
  save_string(python_extension, st);
  st << " idl_extension ";
  save_string(idl_extension, st);
  if (cpp_include_with_path) {
    nl_indent(st);
    st << "cpp_include_with_path";
  }
  if (cpp_relative_path) {
    nl_indent(st);
    st << "cpp_relative_path";
  }
  if (cpp_root_relative_path) {
    nl_indent(st);
    st << "cpp_root_relative_path";
  }
  if (cpp_force_namespace_gen) {
    nl_indent(st);
    st << "cpp_force_namespace_gen";
  }
  if (!cpp_inline_force_incl_in_h) {
    nl_indent(st);
    st << "cpp_inline_dont_force_incl_in_h";
  }
  
  if (cpp_javadoc_comment) {
    nl_indent(st);
    st << "cpp_javadoc_comment";
  }
  
  if (php_req_with_path) {
    nl_indent(st);
    st << "php_req_with_path";
  }
  if (php_relative_path) {
    nl_indent(st);
    st << "php_relative_path";
  }
  if (php_root_relative_path) {
    nl_indent(st);
    st << "php_root_relative_path";
  }
  if (php_force_namespace_gen) {
    nl_indent(st);
    st << "php_force_namespace_gen";
  }

  st << '\n';
  nl_indent(st);
  st << "type_forms " << nbuiltins << " // uml cpp java idl cpp_in cpp_out cpp_inout cpp_return";
  
  int index;
  
  for (index = 0; index != nbuiltins; index += 1) {
    Builtin & b = builtins[index];
    
    nl_indent(st);
    st << "  ";
    save_string(b.uml, st);
    st << ' ';
    save_string(b.cpp, st);
    st << ' ';
    save_string(b.java, st);
    st << ' ';
    save_string(b.idl, st);
    st << ' ';
    save_string(b.cpp_in, st);
    st << ' ';
    save_string(b.cpp_out, st);
    st << ' ';
    save_string(b.cpp_inout, st);
    st << ' ';
    save_string(b.cpp_return, st);
  }
  
  nl_indent(st);
  nl_indent(st);
  st << "relations_stereotypes " << nrelattrstereotypes << " // uml cpp java pythonidl";
  
  for (index = 0; index != nrelattrstereotypes; index += 1) {
    Stereotype & s = relattr_stereotypes[index];
    
    nl_indent(st);
    st << "  ";
    save_string(s.uml, st);
    st << ' ';
    save_string(s.cpp, st);
    st << ' ';
    save_string(s.java, st);
    st << ' ';
    save_string(s.python, st);
    st << ' ';
    save_string(s.idl, st);
  }
  
  nl_indent(st);
  nl_indent(st);
  st << "classes_stereotypes " << nclassstereotypes << " // uml cpp java php python idl";
  
  for (index = 0; index != nclassstereotypes; index += 1) {
    Stereotype & s = class_stereotypes[index];
    
    nl_indent(st);
    st << "  ";
    save_string(s.uml, st);
    st << ' ';
    save_string(s.cpp, st);
    st << ' ';
    save_string(s.java, st);
    st << ' ';
    save_string(s.php, st);
    st << ' ';
    save_string(s.python, st);
    st << ' ';
    save_string(s.idl, st);
  }
  
  nl_indent(st);
  nl_indent(st);
  st << "cpp_enum_default_type_forms ";
  save_string(cpp_enum_in, st);
  st << ' ';
  save_string(cpp_enum_out, st);
  st << ' ';
  save_string(cpp_enum_inout, st);
  st << ' ';
  save_string(cpp_enum_return, st);
  st << " // in out inout return";
  nl_indent(st);
  st << "other_cpp_types_default_type_forms ";
  save_string(cpp_in, st);
  st << ' ';
  save_string(cpp_out, st);
  st << ' ';
  save_string(cpp_inout, st);
  st << ' ';
  save_string(cpp_return, st);
  st << " // in out inout return";
  
  st << '\n';
  nl_indent(st);
  st << "cpp_default_h_content ";
  save_string(cpp_h_content, st);
  nl_indent(st);
  st << "cpp_default_src_content ";
  save_string(cpp_src_content, st);
  nl_indent(st);
  st << "cpp_default_class_decl ";
  save_string(cpp_class_decl, st);
  nl_indent(st);
  st << "cpp_default_external_class_decl ";
  save_string(cpp_external_class_decl, st);
  nl_indent(st);
  st << "cpp_default_struct_decl ";
  save_string(cpp_struct_decl, st);
  nl_indent(st);
  st << "cpp_default_union_decl ";
  save_string(cpp_union_decl, st);
  nl_indent(st);
  st << "cpp_default_enum_decl ";
  save_string(cpp_enum_decl, st);
  nl_indent(st);
  st << "cpp_default_typedef_decl ";
  save_string(cpp_typedef_decl, st);
  nl_indent(st);
  st << "cpp_default_attribute_declaration ";
  save_string(cpp_attr_decl[0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(cpp_attr_decl[1], st);
  st << " // multiplicity * a..b";
  nl_indent(st);
  save_string(cpp_attr_decl[2], st);
  st << " // multiplicity [..]";
  nl_indent(st);
  st << "cpp_default_enum_item_declaration ";
  save_string(cpp_enum_item_decl, st);
  nl_indent(st);
  st << "cpp_association_aggregation_declaration";
  indent(+1);
  nl_indent(st);
  save_string(cpp_rel_decl[0][0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(cpp_rel_decl[0][1], st);
  st << " // multiplicity * a..b";
  nl_indent(st);
  save_string(cpp_rel_decl[0][2], st);
  st << " // multiplicity [..]";
  indent(-1);
  nl_indent(st);
  st << "cpp_aggregation_by_value_declaration";
  indent(+1);
  nl_indent(st);
  save_string(cpp_rel_decl[1][0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(cpp_rel_decl[1][1], st);
  st << " // multiplicity * a..b";
  nl_indent(st);
  save_string(cpp_rel_decl[1][2], st);
  st << " // multiplicity [..]";
  indent(-1);
  nl_indent(st);
  st << "cpp_get ";
  save_string(cpp_get_name, st);
  if (cpp_get_inline) st << " inline";
  if (cpp_get_const) st << " const";
  if (cpp_get_value_const) st << " value_const";
  st << ' ' << stringify(cpp_get_visibility);
  nl_indent(st);
  st << "cpp_set ";
  save_string(cpp_set_name, st);
  if (cpp_set_inline) st << " inline";
  if (cpp_set_param_const) st << " param_const";
  if (cpp_set_param_ref) st << " param_ref";
  st << ' ' << stringify(cpp_set_visibility);
  nl_indent(st);
  st << "cpp_default_operation_declaration ";
  save_string(cpp_oper_decl, st);
  nl_indent(st);
  st << "cpp_default_operation_definition ";
  save_string(cpp_oper_def, st);
  nl_indent(st);
  if (cpp_force_throw) {
    st << "cpp_force_throw";
    nl_indent(st);
  }
  cpp_dir_filter.save("cpp_dir_filter", st);
  cpp_file_filter.save("cpp_file_filter", st);
  st << "cpp_indent_visibility ";
  save_string(cpp_indent_visibility, st);

  save_includes_imports(cpp_includes, "cpp_includes");
    
  if (!java_javadoc_comment) {
    st << "java_no_javadoc_comment";
    nl_indent(st);
  }
  
  if (!java_force_package_gen) {
    st << "java_no_force_package_gen";
    nl_indent(st);
  }
  
  st << "java_default_src_content ";
  save_string(java_src_content, st);
  nl_indent(st);
  st << "java_default_class_decl ";
  save_string(java_class_decl, st);
  nl_indent(st);
  st << "java_default_external_class_decl ";
  save_string(java_external_class_decl, st);
  nl_indent(st);
  st << "java_default_interface_decl ";
  save_string(java_interface_decl, st);
  nl_indent(st);
  st << "java5_default_enum_decl ";
  save_string(java_enum_decl, st);
  nl_indent(st);
  st << "java_default_enum_decl ";
  save_string(java_enum_pattern_decl, st);
  nl_indent(st);
  st << "java_default_attribute_declaration ";
  save_string(java_attr_decl[0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(java_attr_decl[1], st);
  st << " // multiplicity * a..b";
  nl_indent(st);
  save_string(java_attr_decl[2], st);
  st << " // multiplicity N";
  nl_indent(st);
  st << "java5_default_enum_item_declaration ";
  save_string(java_enum_item_decl, st);
  nl_indent(st);
  st << "java_default_enum_item_declaration ";
  save_string(java_enum_pattern_item_decl, st);
  nl_indent(st);
  st << "java_default_enum_case ";
  save_string(java_enum_pattern_item_case, st);
  nl_indent(st);
  st << "java_association_aggregation_declaration";
  indent(+1);
  nl_indent(st);
  save_string(java_rel_decl[0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(java_rel_decl[1], st);
  st << " // multiplicity * a..b";
  nl_indent(st);
  save_string(java_rel_decl[2], st);
  st << " // multiplicity N";
  indent(-1);
  nl_indent(st);
  st << "java_get ";
  save_string(java_get_name, st);
  if (java_get_final) st << " final";
  st << ' ' << stringify(noncpp_get_visibility);
  nl_indent(st);
  st << "java_set ";
  save_string(java_set_name, st);
  if (java_set_final) st << " final";
  if (java_set_param_final) st << " param_final";
  st << ' ' << stringify(noncpp_set_visibility);
  nl_indent(st);
  st << "java_default_operation_definition ";
  save_string(java_oper_def, st);
  nl_indent(st);
  java_dir_filter.save("java_dir_filter", st);
  java_file_filter.save("java_file_filter", st);
  
  save_includes_imports(java_imports, "java_imports");
  
  if (php_javadoc_comment) {
    st << "php_javadoc_comment";
    nl_indent(st);
  }
  st << "php_default_src_content ";
  save_string(php_src_content, st);
  nl_indent(st);
  st << "php_default_class_decl ";
  save_string(php_class_decl, st);
  nl_indent(st);
  st << "php_default_enum_decl ";
  save_string(php_enum_decl, st);
  nl_indent(st);
  st << "php_default_external_class_decl ";
  save_string(php_external_class_decl, st);
  nl_indent(st);
  st << "php_default_interface_decl ";
  save_string(php_interface_decl, st);
  nl_indent(st);
  st << "php_default_attribute_declaration ";
  save_string(php_attr_decl, st);
  nl_indent(st);
  st << "php_default_enum_item_decl ";
  save_string(php_enum_item_decl, st);
  nl_indent(st);
  st << "php_default_relation_declaration";
  save_string(php_rel_decl, st);
  nl_indent(st);
  st << "php_get ";
  save_string(php_get_name, st);
  if (php_get_final) st << " final";
  nl_indent(st);
  st << "php_set ";
  save_string(php_set_name, st);
  if (php_set_final) st << " final";
  nl_indent(st);
  st << "php_default_operation_definition ";
  save_string(php_oper_def, st);
  nl_indent(st);
  php_dir_filter.save("php_dir_filter", st);
  php_file_filter.save("php_file_filter", st);
  
  if (python_2_2) {
    st << "python_2_2";
    nl_indent(st);
  }
  if (python_3_operation) {
    st << "python_3_operation";
    nl_indent(st);
  }
  st << "python_indent_step ";
  save_string(python_indent_step, st);
  nl_indent(st);
  st << "python_default_src_content ";
  save_string(python_src_content, st);
  nl_indent(st);
  st << "python_default_class_decl ";
  save_string(python_class_decl, st);
  nl_indent(st);
  st << "python_default_enum_decl ";
  save_string(python_enum_decl, st);
  nl_indent(st);
  st << "python_default_external_class_decl ";
  save_string(python_external_class_decl, st);
  nl_indent(st);
  st << "python_default_attribute_declaration ";
  save_string(python_attr_decl[0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(python_attr_decl[1], st);
  st << " // multiplicity != 1";
  nl_indent(st);
  st << "python_default_enum_item_decl ";
  save_string(python_enum_item_decl, st);
  nl_indent(st);
  st << "python_default_relation_declaration";
  save_string(python_rel_decl[0][0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(python_rel_decl[0][1], st);
  st << " // multiplicity != 1";
  nl_indent(st);
  st << "python_default_composition_declaration";
  save_string(python_rel_decl[1][0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(python_rel_decl[1][1], st);
  st << " // multiplicity != 1";
  nl_indent(st);
  st << "python_default_operation_definition ";
  save_string(python_oper_def, st);
  nl_indent(st);
  st << "python_default_initoperation_definition ";
  save_string(python_initoper_def, st);
  nl_indent(st);
  st << "python_get ";
  save_string(python_get_name, st);
  nl_indent(st);
  st << "python_set ";
  save_string(python_set_name, st);
  nl_indent(st);
  
  save_includes_imports(python_imports, "python_imports");
  
  st << "idl_default_src_content ";
  save_string(idl_src_content, st);
  nl_indent(st);
  st << "idl_default_interface_decl ";
  save_string(idl_interface_decl, st);
  nl_indent(st);
  st << "idl_default_valuetype_decl ";
  save_string(idl_valuetype_decl, st);
  nl_indent(st);
  st << "idl_default_struct_decl ";
  save_string(idl_struct_decl, st);
  nl_indent(st);
  st << "idl_default_typedef_decl ";
  save_string(idl_typedef_decl, st);
  nl_indent(st);
  st << "idl_default_exception_decl ";
  save_string(idl_exception_decl, st);
  nl_indent(st);
  st << "idl_default_union_decl ";
  save_string(idl_union_decl, st);
  nl_indent(st);
  st << "idl_default_enum_decl ";
  save_string(idl_enum_decl, st);
  nl_indent(st);
  st << "idl_default_external_class_decl ";
  save_string(idl_external_class_decl, st);
  nl_indent(st);
  st << "idl_default_attribute_declaration ";
  save_string(idl_attr_decl[0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(idl_attr_decl[1], st);
  st << " // multiplicity * a..b";
  nl_indent(st);
  save_string(idl_attr_decl[2], st);
  st << " // multiplicity N";
  nl_indent(st);
  st << "idl_default_valuetype_attribute_declaration ";
  save_string(idl_valuetype_attr_decl[0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(idl_valuetype_attr_decl[1], st);
  st << " // multiplicity * a..b";
  nl_indent(st);
  save_string(idl_valuetype_attr_decl[2], st);
  st << " // multiplicity N";
  nl_indent(st);
  st << "idl_default_const_declaration ";
  save_string(idl_const_decl[0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(idl_const_decl[1], st);
  st << " // multiplicity * a..b";
  nl_indent(st);
  save_string(idl_const_decl[2], st);
  st << " // multiplicity N";
  nl_indent(st);
  st << "idl_default_enum_item_declaration ";
  save_string(idl_enum_item_decl, st);
  nl_indent(st);
  st << "idl_default_union_item_declaration ";
  save_string(idl_union_item_decl[0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(idl_union_item_decl[1], st);
  st << " // multiplicity * a..b";
  nl_indent(st);
  save_string(idl_union_item_decl[2], st);
  st << " // multiplicity N";
  nl_indent(st);
  st << "idl_association_aggregation_declaration";
  indent(+1);
  nl_indent(st);
  save_string(idl_rel_decl[0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(idl_rel_decl[1], st);
  st << " // multiplicity * a..b";
  nl_indent(st);
  save_string(idl_rel_decl[2], st);
  st << " // multiplicity N";
  indent(-1);
  nl_indent(st);
  st << "idl_valuetype_association_aggregation_declaration";
  indent(+1);
  nl_indent(st);
  save_string(idl_valuetype_rel_decl[0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(idl_valuetype_rel_decl[1], st);
  st << " // multiplicity * a..b";
  nl_indent(st);
  save_string(idl_valuetype_rel_decl[2], st);
  st << " // multiplicity N";
  indent(-1);
  nl_indent(st);
  st << "idl_union_association_aggregation_declaration";
  indent(+1);
  nl_indent(st);
  save_string(idl_union_rel_decl[0], st);
  st << " // multiplicity 1";
  nl_indent(st);
  save_string(idl_union_rel_decl[1], st);
  st << " // multiplicity * a..b";
  nl_indent(st);
  save_string(idl_union_rel_decl[2], st);
  st << " // multiplicity N";
  indent(-1);
  nl_indent(st);
  st << "idl_get ";
  save_string(idl_get_name, st);
  nl_indent(st);
  st << "idl_set ";
  save_string(idl_set_name, st);
  st << ((idl_set_oneway) ? " oneway" : "  twoways");
  nl_indent(st);
  st << "idl_default_operation_declaration ";
  save_string(idl_oper_decl, st);
  
  nl_indent(st);
  st << "uml_get_name " << stringify(uml_get_name)
    << " uml_set_name " << stringify(uml_set_name);
  
  st << "\nend\n";
  
  save_includes_imports(idl_includes, "idl_includes");
  
  st << '\000';
  save_if_needed("generation_settings", newdef);
}

void GenerationSettings::read_dirs(char * & st, char * & k)
{
  if (!strcmp(k, "root_dir")) {
    // old version
    cpp_root_dir =
      java_root_dir =
	php_root_dir =
	  python_root_dir =
	    idl_root_dir = read_string(st);
    k = read_keyword(st);
  }
  else {
    if (!strcmp(k, "cpp_root_dir")) {
      cpp_root_dir = read_string(st);
      k = read_keyword(st);
    }
    if (!strcmp(k, "java_root_dir")) {
      java_root_dir = read_string(st);
      k = read_keyword(st);
    }
    if (!strcmp(k, "php_root_dir")) {
      php_root_dir = read_string(st);
      k = read_keyword(st);
    }
    if (!strcmp(k, "python_root_dir")) {
      python_root_dir = read_string(st);
      k = read_keyword(st);
    }
    if (!strcmp(k, "idl_root_dir")) {
      idl_root_dir = read_string(st);
      k = read_keyword(st);
    }
  }
  
  if (!strcmp(k, "temp_dir")) {
    // old version
    (void) read_string(st);
    k = read_keyword(st);
  }
}
  
void GenerationSettings::read_descriptions(char * & st, char * & k)
{
  if (!strcmp(k, "artifact_default_description")) {
    artifact_default_description = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "class_default_description")) {
    class_default_description = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "operation_default_description")) {
    operation_default_description = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "attribute_default_description")) {
    attribute_default_description = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "relation_default_description")) {
    relation_default_description = read_string(st);
    k = read_keyword(st);
  }
}

static void read_rel_att_st(char * & st, Stereotype & s, unsigned fileformat)
{
  s.uml = read_string(st);
  s.cpp = read_string(st);
  s.java = read_string(st);
  if (fileformat >= 51)
    s.python = read_string(st);
  else if (s.uml == "set")
    s.python = "set";
  else if (s.uml == "map")
    s.python = "dict";
  else
    s.python = "list";
  s.idl = read_string(st);
}

static void read_class_st(char * & st, Stereotype & s, unsigned fileformat)
{
  s.uml = read_string(st);
  s.cpp = read_string(st);
  s.java = read_string(st);
  
  if (fileformat < 44) {
    if (s.uml == "interface") {
      s.php = "interface";
      s.python = "class";
    }
    else if ((s.uml == "enum") || (s.uml == "enum_pattern"))
      s.php = s.python = "enum";
    else if ((s.uml == "typedef") || (s.uml == "actor") || (s.uml == "@interface"))
      s.php = s.python = "ignored";
    else 
      s.php = s.python = "class";
    s.idl = read_string(st);
  }
  else if (fileformat < 51) {
    s.php = read_string(st);
    if ((s.uml == "enum") || (s.uml == "enum_pattern"))
      s.python = "enum";
    else if ((s.uml == "typedef") || (s.uml == "actor") || (s.uml == "@interface"))
      s.python = "ignored";
    else 
      s.python = "class";
    s.idl = read_string(st);
  }
  else {
    s.php = read_string(st);
    s.python = read_string(st);
    s.idl = read_string(st);
  }
}

void GenerationSettings::read(char * & st, char * & k)
{
  unsigned fileformat = read_file_format();

  if (!strcmp(k, "cpp_default_defs")) {
    cpp_default_defs = TRUE;
    k = read_keyword(st);
  }
  else
    cpp_default_defs = FALSE;
  
  if (!strcmp(k, "java_default_defs")) {
    java_default_defs = TRUE;
    k = read_keyword(st);
  }
  else
    java_default_defs = FALSE;
  
  if (!strcmp(k, "php_default_defs")) {
    php_default_defs = TRUE;
    k = read_keyword(st);
  }
  else
    php_default_defs = FALSE;
  
  if (!strcmp(k, "python_default_defs")) {
    python_default_defs = TRUE;
    k = read_keyword(st);
  }
  else
    python_default_defs = FALSE;
  
  if (!strcmp(k, "idl_default_defs")) {
    idl_default_defs = TRUE;
    k = read_keyword(st);
  }
  else
    idl_default_defs = FALSE;
  
  if (!strcmp(k, "cpp_h_extension")) {
    cpp_h_extension = read_string(st);
    k = read_keyword(st);
  }
  else
    cpp_h_extension = "h";
  
  if (!strcmp(k, "cpp_src_extension")) {
    cpp_src_extension = read_string(st);
    k = read_keyword(st);
  }
  else
    cpp_src_extension = "cpp";
  
  if (!strcmp(k, "java_extension")) {
    java_extension = read_string(st);
    k = read_keyword(st);
  }
  else
    java_extension = "java";

  if (!strcmp(k, "php_extension")) {
    php_extension = read_string(st);
    k = read_keyword(st);
  }
  else
    php_extension = "php";

  if (!strcmp(k, "python_extension")) {
    python_extension = read_string(st);
    k = read_keyword(st);
  }
  else
    python_extension = "py";

  if (!strcmp(k, "idl_extension")) {
    idl_extension = read_string(st);
    k = read_keyword(st);
  }
  else
    idl_extension = "idl";
  
  if (!strcmp(k, "cpp_include_with_path")) {
    cpp_include_with_path = TRUE;
    k = read_keyword(st);
  }
  else
    cpp_include_with_path = FALSE;
  if (!strcmp(k, "cpp_relative_path")) {
    cpp_relative_path = TRUE;
    k = read_keyword(st);
  }
  else
    cpp_relative_path = FALSE;
  if (!strcmp(k, "cpp_root_relative_path")) {
    cpp_root_relative_path = TRUE;
    k = read_keyword(st);
  }
  else
    cpp_root_relative_path = FALSE;
  if (!strcmp(k, "cpp_force_namespace_gen")) {
    cpp_force_namespace_gen = TRUE;
    k = read_keyword(st);
  }
  else
    cpp_force_namespace_gen = FALSE;
  if (!strcmp(k, "cpp_inline_dont_force_incl_in_h")) {
    cpp_inline_force_incl_in_h = FALSE;
    k = read_keyword(st);
  }
  else
    cpp_inline_force_incl_in_h = TRUE;
  if (!strcmp(k, "cpp_javadoc_comment")) {
    cpp_javadoc_comment = TRUE;
    k = read_keyword(st);
  }
  else
    cpp_javadoc_comment = FALSE;
  
  if (!strcmp(k, "php_include_with_path") ||
      !strcmp(k, "php_req_with_path")) {
    php_req_with_path = TRUE;
    k = read_keyword(st);
  }
  else
    php_req_with_path = FALSE;
  if (!strcmp(k, "php_relative_path")) {
    php_relative_path = TRUE;
    k = read_keyword(st);
  }
  else
    php_relative_path = FALSE;
  if (!strcmp(k, "php_root_relative_path")) {
    php_root_relative_path = TRUE;
    k = read_keyword(st);
  }
  else
    php_root_relative_path = FALSE;
  if (!strcmp(k, "php_force_namespace_gen")) {
    php_force_namespace_gen = TRUE;
    k = read_keyword(st);
  }
  else
    php_force_namespace_gen = FALSE;

  bool old_types = !strcmp(k, "types");
  bool new_types = !strcmp(k, "type_forms");
  
  if (old_types || new_types) {
    if (builtins != 0)
      delete [] builtins;

    nbuiltins = (int) read_unsigned(st);
    builtins = new Builtin[nbuiltins];
    
    umltypes.clear();
  
    int index;
    
    for (index = 0; index != nbuiltins; index += 1) {
      Builtin & b = builtins[index];
      
      b.uml = read_string(st);
      b.cpp = read_string(st);
      b.java = read_string(st);
      b.idl = read_string(st);
      b.cpp_in = read_string(st);
      b.cpp_out = read_string(st);
      b.cpp_inout = read_string(st);
      if (new_types)
	b.cpp_return = read_string(st);
      else
	b.cpp_return = "${type}";
      
      umltypes.append(b.uml);
    }
  
    if (umltypes.findIndex("void") == -1)
      umltypes.append("void");
    
    read_keyword(st, "relations_stereotypes");
    if (relattr_stereotypes != 0)
      delete [] relattr_stereotypes;

    nrelattrstereotypes = (int) read_unsigned(st);
    relattr_stereotypes = new Stereotype[nrelattrstereotypes];
    
    for (index = 0; index != nrelattrstereotypes; index += 1) {
      Stereotype & s = relattr_stereotypes[index];
      
      read_rel_att_st(st, s, fileformat);
    }
        
    read_keyword(st, "classes_stereotypes");
    if (class_stereotypes != 0)
      delete [] class_stereotypes;

    nclassstereotypes = (int) read_unsigned(st);
    class_stereotypes = new Stereotype[nclassstereotypes];

    for (index = 0; index != nclassstereotypes; index += 1) {
      Stereotype & s = class_stereotypes[index];
	
      read_class_st(st, s, fileformat);
    }
     
    if (new_types)
      read_keyword(st, "cpp_enum_default_type_forms");
    else
      read_keyword(st, "cpp_enum_default_param_passing");
    cpp_enum_in = read_string(st);
    cpp_enum_out = read_string(st);
    cpp_enum_inout = read_string(st);
    if (new_types)
      cpp_enum_return = read_string(st);
    else
      cpp_enum_return = "${type}";

    if (new_types)
      read_keyword(st, "other_cpp_types_default_type_forms");
    else
      read_keyword(st, "other_cpp_types_default_param_passing");
    cpp_in = read_string(st);
    cpp_out = read_string(st);
    cpp_inout = read_string(st);
    if (new_types)
      cpp_return = read_string(st);
    else
      cpp_return = "${type}";

    k = read_keyword(st);
    if (!strcmp(k, "cpp_default_h_content")) {
      cpp_h_content = read_string(st);
      k = read_keyword(st);
    }
    else
      cpp_h_content = CPP_H_CONTENT;
    if (!strcmp(k, "cpp_default_src_content")) {
      // old -> new version
      QCString s = read_string(st);
      int index;
      
      if ((index = s.find("${class_attributes}\n${operations}")) != -1)
	s.replace(index, 33, "${members}");
      else if ((index = s.find("${class_attributes}\r\n${operations}")) != -1)
	s.replace(index, 34, "${members}");
      cpp_src_content = s;
      k = read_keyword(st);
    }
    else
      cpp_src_content = CPP_SRC_CONTENT;
    if (strcmp(k, "cpp_default_class_decl"))
      wrong_keyword(k, "cpp_default_class_decl");
    cpp_class_decl = read_string(st);
    k = read_keyword(st);
    if (!strcmp(k, "cpp_default_external_class_decl")) {
      // new version
      cpp_external_class_decl = read_string(st);
      k = read_keyword(st);
    }
    if (strcmp(k, "cpp_default_struct_decl"))
      wrong_keyword(k, "cpp_default_struct_decl");
    cpp_struct_decl = read_string(st);
    read_keyword(st, "cpp_default_union_decl");
    cpp_union_decl = read_string(st);
    read_keyword(st, "cpp_default_enum_decl");
    cpp_enum_decl = read_string(st);
    read_keyword(st, "cpp_default_typedef_decl");
    cpp_typedef_decl = read_string(st);
    read_keyword(st, "cpp_default_attribute_declaration",
		 "cpp_default_attribut_declaration");
    cpp_attr_decl[0] = read_string(st);
    if (fileformat >= 41) {
      cpp_attr_decl[1] = read_string(st);
      cpp_attr_decl[2] = read_string(st);
    }
    else if (cpp_attr_decl[0].find("${description}") != -1) {
      cpp_attr_decl[1] = CPP_ATTR_DESCR1;
      cpp_attr_decl[2] = CPP_ATTR_DESCR2;
    }
    else {
      cpp_attr_decl[1] = CPP_ATTR_DECL1;
      cpp_attr_decl[2] = CPP_ATTR_DECL2;
    }
    read_keyword(st, "cpp_default_enum_item_declaration");
    cpp_enum_item_decl = read_string(st);
    read_keyword(st, "cpp_association_aggregation_declaration");
    cpp_rel_decl[0][0] = read_string(st);
    cpp_rel_decl[0][1] = read_string(st);
    cpp_rel_decl[0][2] = read_string(st);
    read_keyword(st, "cpp_aggregation_by_value_declaration");
    cpp_rel_decl[1][0] = read_string(st);
    cpp_rel_decl[1][1] = read_string(st);
    cpp_rel_decl[1][2] = read_string(st);
    
    read_keyword(st, "cpp_get");
    cpp_get_name = read_string(st);
    k = read_keyword(st);
    if (!strcmp(k, "inline")) {
      cpp_get_inline = TRUE;
      k = read_keyword(st);
    }
    else
      cpp_get_inline = FALSE;
    if (!strcmp(k, "const")) {
      cpp_get_const = TRUE;
      k = read_keyword(st);
    }
    else
      cpp_get_const = FALSE;
    if (!strcmp(k, "value_const")) {
      cpp_get_value_const = TRUE;
      k = read_keyword(st);
    }
    else
      cpp_get_value_const = FALSE;
    cpp_get_visibility = ::visibility(k);
    read_keyword(st, "cpp_set");
    cpp_set_name = read_string(st);
    k = read_keyword(st);
    if (!strcmp(k, "inline")) {
      cpp_set_inline = TRUE;
      k = read_keyword(st);
    }
    else
      cpp_set_inline = FALSE;
    if (!strcmp(k, "param_const")) {
      cpp_set_param_const = TRUE;
      k = read_keyword(st);
    }
    else
      cpp_set_param_const = FALSE;
    if (!strcmp(k, "param_ref")) {
      cpp_set_param_ref = TRUE;
      k = read_keyword(st);
    }
    else
      cpp_set_param_ref = FALSE;
    cpp_set_visibility = ::visibility(k);
    
    read_keyword(st, "cpp_default_operation_declaration");
    cpp_oper_decl = read_string(st);
    read_keyword(st, "cpp_default_operation_definition");
    cpp_oper_def = read_string(st);
    k = read_keyword(st);
    if (!strcmp(k, "cpp_force_throw")) {
      cpp_force_throw = TRUE;
      k = read_keyword(st);
    }
    else
      cpp_force_throw = FALSE;
    cpp_dir_filter.read("cpp_dir_filter", st, k);
    cpp_file_filter.read("cpp_file_filter", st, k);
    if (!strcmp(k, "cpp_indent_visibility")) {
      cpp_indent_visibility = read_string(st);
      k = read_keyword(st);
    }
    else
      cpp_indent_visibility = CPP_INDENT_VISIBILITY;
    
    if (!strcmp(k, "cpp_includes")) {
      // old version
      cpp_includes.types.clear();
      cpp_includes.includes.clear();
      
      for (index = (int) read_unsigned(st); index; index -= 1) {
	cpp_includes.types.append(read_string(st));
	cpp_includes.includes.append(read_string(st));
      }
      k = read_keyword(st);
    }
    
    if (!strcmp(k, "java_no_javadoc_comment")) {
      java_javadoc_comment = FALSE;
      k = read_keyword(st);
    }
    else
      java_javadoc_comment = TRUE;
    
    if (!strcmp(k, "java_no_force_package_gen")) {
      java_force_package_gen = FALSE;
      k = read_keyword(st);
    }
    else
      java_force_package_gen = TRUE;
    
    if (!strcmp(k, "java_default_src_content")) {
      java_src_content = read_string(st);
      k = read_keyword(st);
    }
    else
      java_src_content = JAVA_SRC_CONTENT;
    if (strcmp(k, "java_default_class_decl"))
      wrong_keyword(k, "java_default_class_decl");
    java_class_decl = read_string(st);
    k = read_keyword(st);
    if (!strcmp(k, "java_default_external_class_decl")) {
      // new version
      java_external_class_decl = read_string(st);
      k = read_keyword(st);
    }
    if (strcmp(k, "java_default_interface_decl"))
      wrong_keyword(k, "java_default_interface_decl");
    java_interface_decl = read_string(st);
    k = read_keyword(st);
    if (! strcmp(k, "java5_default_enum_decl")) {
      java_enum_decl = read_string(st);
      k = read_keyword(st);
    }
    else
      java_enum_decl = "${comment}${visibility}${final}${abstract}enum ${name}${implements} {\n${items};\n${members}}\n";      
    if (strcmp(k, "java_default_enum_decl"))
      wrong_keyword(k, "java_default_enum_decl");
    java_enum_pattern_decl = read_string(st);
    read_keyword(st, "java_default_attribute_declaration",
		 "java_default_attribut_declaration");
    java_attr_decl[0] = read_string(st);
    if (fileformat >= 41) {
      java_attr_decl[1] = read_string(st);
      java_attr_decl[2] = read_string(st);
    }
    else if (java_attr_decl[0].find("${description}") != -1) {
      java_attr_decl[1] = JAVA_ATTR_DESCR1;
      java_attr_decl[2] = JAVA_ATTR_DESCR2;
    }
    else {
      java_attr_decl[1] = JAVA_ATTR_DECL1;
      java_attr_decl[2] = JAVA_ATTR_DECL2;
    }
    k = read_keyword(st);
    if (! strcmp(k, "java5_default_enum_item_declaration")) {
      java_enum_item_decl = read_string(st);
      k = read_keyword(st);
    }
    else
      java_enum_item_decl = "  ${name}${value},${comment}";
    if (strcmp(k, "java_default_enum_item_declaration"))
      wrong_keyword(k, "java_default_enum_item_declaration");
    java_enum_pattern_item_decl = read_string(st);
    k = read_keyword(st);
    if (!strcmp(k, "java_default_enum_case")) {
      java_enum_pattern_item_case = read_string(st);
      k = read_keyword(st);
    }
    else {
      // old definitions
      java_enum_pattern_decl = "${comment}${visibility}final class ${name} {\n${members}\n\
  private final int value;\n\n\
  public int value() {\n\
    return value;\n\
  }\n\n\
  public static ${name} fromInt(int value) {\n\
    switch (value) {\n\
${cases}    default: throw new Error();\n\
    }\n\n\
  }\n\
  private ${name}(int v) { value = v; };\n}\n";
      java_enum_pattern_item_decl = "  ${comment}public static final int _${name}${value};\n\
public static final ${class} ${name} = new ${class}(_${name});\n";
      java_enum_pattern_item_case = "    case _${name}: return ${name};\n";
    }
    if (strcmp(k, "java_association_aggregation_declaration"))
      wrong_keyword(k, "java_association_aggregation_declaration");
    java_rel_decl[0] = read_string(st);
    java_rel_decl[1] = read_string(st);
    java_rel_decl[2] = read_string(st);
    
    read_keyword(st, "java_get");
    java_get_name = read_string(st);
    k = read_keyword(st);
    if (!strcmp(k, "final")) {
      java_get_final = TRUE;
      k = read_keyword(st);
    }
    else
      java_get_final = FALSE;
    noncpp_get_visibility = ::visibility(k);
    read_keyword(st, "java_set");
    java_set_name = read_string(st);
    k = read_keyword(st);
    if (!strcmp(k, "final")) {
      java_set_final = TRUE;
      k = read_keyword(st);
    }
    else
      java_set_final = FALSE;
    if (!strcmp(k, "param_final")) {
      java_set_param_final = TRUE;
      k = read_keyword(st);
    }
    else
      java_set_param_final = FALSE;
    cpp_set_visibility = ::visibility(k);
    
    read_keyword(st, "java_default_operation_definition");
    java_oper_def = read_string(st);
    k = read_keyword(st);
    
    java_dir_filter.read("java_dir_filter", st, k);
    java_file_filter.read("java_file_filter", st, k);
    
    if (!strcmp(k, "java_imports")) {
      // old version
      java_imports.types.clear();
      java_imports.includes.clear();
      
      for (index = (int) read_unsigned(st); index; index -= 1) {
	java_imports.types.append(read_string(st));
	java_imports.includes.append(read_string(st));
      }
      k = read_keyword(st);
    }

    if (fileformat < 44) {
      php_src_content = PHP_SRC_CONTENT;
      php_extension = "php";
      php_class_decl = PHP_CLASS;
      php_external_class_decl = "${name}";
      php_enum_decl = PHP_ENUM;
      php_interface_decl = PHP_INTERFACE;
      php_attr_decl = PHP_ATTR;
      php_enum_item_decl = PHP_ENUMITEM;
      php_rel_decl = PHP_REL;
      php_oper_def = PHP_OPER;
      php_get_name = "get${Name}";
      php_get_final = TRUE;
      php_set_name = "set${Name}";
      php_set_final = FALSE;
      php_javadoc_comment = FALSE;
    }
    else {
      if (!strcmp(k, "php_javadoc_comment")) {
	php_javadoc_comment = TRUE;
	k = read_keyword(st);
      }
      else
	php_javadoc_comment = FALSE;
      if (strcmp(k, "php_default_src_content"))
	wrong_keyword(k, "php_default_src_content");
      php_src_content = read_string(st);
      read_keyword(st, "php_default_class_decl");
      php_class_decl = read_string(st);
      read_keyword(st, "php_default_enum_decl");
      php_enum_decl = read_string(st);
      read_keyword(st, "php_default_external_class_decl");
      php_external_class_decl = read_string(st);
      read_keyword(st, "php_default_interface_decl");
      php_interface_decl = read_string(st);
      read_keyword(st, "php_default_attribute_declaration");
      php_attr_decl = read_string(st);
      read_keyword(st, "php_default_enum_item_decl");
      php_enum_item_decl = read_string(st);
      read_keyword(st, "php_default_relation_declaration");
      php_rel_decl = read_string(st);
      read_keyword(st, "php_get");
      php_get_name = read_string(st);
      k = read_keyword(st);
      if (!strcmp(k, "final")) {
	php_get_final = TRUE;
	k = read_keyword(st);
      }
      else
	php_get_final = FALSE;
      if (strcmp(k, "php_set"))
	wrong_keyword(k, "php_set");
      php_set_name = read_string(st);
      k = read_keyword(st);
      if (!strcmp(k, "final")) {
	php_set_final = TRUE;
	k = read_keyword(st);
      }
      else
	php_set_final = FALSE;
      if (strcmp(k, "php_default_operation_definition"))
	wrong_keyword(k, "php_default_operation_definition");
      php_oper_def = read_string(st);
      k = read_keyword(st);
      php_dir_filter.read("php_dir_filter", st, k);
      php_file_filter.read("php_file_filter", st, k);
    }
    
    if (fileformat < 51) {
      python_2_2 = TRUE;
      python_3_operation = FALSE;
      python_indent_step = PYTHON_INDENT_STEP;
      python_src_content = PYTHON_SRC_CONTENT;
      python_extension = "py";
      python_class_decl = PYTHON_CLASS;
      python_enum_decl = PYTHON_ENUM;
      python_external_class_decl = "${name}";
      python_attr_decl[0] = PYTHON_ATTR1;
      python_attr_decl[1] = PYTHON_ATTR2;
      python_enum_item_decl = PYTHON_ENUMITEM;
      python_rel_decl[0][0] = PYTHON_REL1;
      python_rel_decl[0][1] = PYTHON_REL2;
      python_rel_decl[1][0] = PYTHON_REL_COMP;
      python_rel_decl[1][1] = PYTHON_REL2;
      python_oper_def = PYTHON_OPER;
      python_initoper_def = PYTHON_INITOPER;
      python_get_name = "get${Name}";
      python_set_name = "set${Name}";
    }
    else {
      if (! strcmp(k, "python_2_2")) {
	python_2_2 = TRUE;
	k = read_keyword(st);
      }
      else
	python_2_2 = FALSE;
      if (! strcmp(k, "python_3_operation")) {
	python_3_operation = TRUE;
	k = read_keyword(st);
      }
      else
	python_3_operation = FALSE;
      if (strcmp(k, "python_indent_step"))
	wrong_keyword(k, "python_indent_step");
      python_indent_step = read_string(st);
      read_keyword(st, "python_default_src_content");
      python_src_content = read_string(st);
      read_keyword(st, "python_default_class_decl");
      python_class_decl = read_string(st);
      read_keyword(st, "python_default_enum_decl");
      python_enum_decl = read_string(st);
      read_keyword(st, "python_default_external_class_decl");
      python_external_class_decl = read_string(st);
      read_keyword(st, "python_default_attribute_declaration");
      python_attr_decl[0] = read_string(st);
      python_attr_decl[1] = read_string(st);
      read_keyword(st, "python_default_enum_item_decl");
      python_enum_item_decl = read_string(st);
      read_keyword(st, "python_default_relation_declaration");
      python_rel_decl[0][0] = read_string(st);
      python_rel_decl[0][1] = read_string(st);
      read_keyword(st, "python_default_composition_declaration");
      python_rel_decl[1][0] = read_string(st);
      python_rel_decl[1][1] = read_string(st);
      read_keyword(st, "python_default_operation_definition");
      python_oper_def = read_string(st);
      k = read_keyword(st);
      if (!strcmp(k, "python_default_initoperation_definition")) {
	python_initoper_def = read_string(st);
	k = read_keyword(st);
      }
      else
	python_initoper_def = PYTHON_INITOPER;
      if (strcmp(k, "python_get"))
	wrong_keyword(k, "python_get");
      python_get_name = read_string(st);
      read_keyword(st, "python_set");
      python_set_name = read_string(st);
      k = read_keyword(st);
    }
    
    if (!strcmp(k, "idl_default_src_content")) {
      idl_src_content = read_string(st);
      k = read_keyword(st);
    }
    else
      // old version
      idl_src_content = IDL_SRC_CONTENT;
    if (strcmp(k, "idl_default_class_decl") &&	// old
	strcmp(k, "idl_default_interface_decl"))
      wrong_keyword(k, "idl_default_interface_decl");
    idl_interface_decl = read_string(st);
    k = read_keyword(st);
    if (!strcmp(k, "idl_default_valuetype_decl")) {
      idl_valuetype_decl = read_string(st);
      k = read_keyword(st);
    }
    else
      // old version
      idl_valuetype_decl = IDL_VALUETYPE_DECL;
    if (strcmp(k, "idl_default_struct_decl"))
      wrong_keyword(k, "idl_default_struct_decl");
    idl_struct_decl = read_string(st);
    read_keyword(st, "idl_default_typedef_decl");
    idl_typedef_decl = read_string(st);
    read_keyword(st, "idl_default_exception_decl");
    idl_exception_decl = read_string(st);
    read_keyword(st, "idl_default_union_decl");
    idl_union_decl = read_string(st);
    read_keyword(st, "idl_default_enum_decl");
    idl_enum_decl = read_string(st);
    k = read_keyword(st);
    if (!strcmp(k, "idl_default_external_class_decl")) {
      idl_external_class_decl = read_string(st);
      k = read_keyword(st);
    }
    else
      // old version
      idl_external_class_decl = IDL_EXTERNAL_CLASS_DECL;
    if (strcmp(k, "idl_default_attribute_declaration") &&
	strcmp(k, "idl_default_attribut_declaration"))
      wrong_keyword(k, "idl_default_attribute_declaration");
    idl_attr_decl[0] = read_string(st);
    if (fileformat >= 41) {
      idl_attr_decl[1] = read_string(st);
      idl_attr_decl[2] = read_string(st);
    }
    else if (idl_attr_decl[0].find("${description}") != -1) {
      idl_attr_decl[1] = IDL_ATTR_DESCR1;
      idl_attr_decl[2] = IDL_ATTR_DESCR2;
    }
    else {
      idl_attr_decl[1] = IDL_ATTR_DECL1;
      idl_attr_decl[2] = IDL_ATTR_DECL2;
    }
     k = read_keyword(st);
    if (!strcmp(k, "idl_default_valuetype_attribute_declaration") ||
	!strcmp(k, "idl_default_valuetype_attribut_declaration")) {
      idl_valuetype_attr_decl[0] = read_string(st);
      if (fileformat >= 41) {
	idl_valuetype_attr_decl[1] = read_string(st);
	idl_valuetype_attr_decl[2] = read_string(st);
      }
      else if (idl_valuetype_attr_decl[0].find("${description}") != -1) {
	idl_valuetype_attr_decl[1] = IDL_VALUETYPE_ATTRIBUTE_DESCR1;
	idl_valuetype_attr_decl[2] = IDL_VALUETYPE_ATTRIBUTE_DESCR2;
      }
      else {
	idl_valuetype_attr_decl[1] = IDL_VALUETYPE_ATTRIBUTE_DECL1;
	idl_valuetype_attr_decl[2] = IDL_VALUETYPE_ATTRIBUTE_DECL2;
      }
      k = read_keyword(st);
    }
    else {
      // old version
      idl_valuetype_attr_decl[0] = IDL_VALUETYPE_ATTRIBUTE_DECL0;
      idl_valuetype_attr_decl[1] = IDL_VALUETYPE_ATTRIBUTE_DECL1;
      idl_valuetype_attr_decl[2] = IDL_VALUETYPE_ATTRIBUTE_DECL2;
    }
    if (strcmp(k, "idl_default_const_declaration"))
      wrong_keyword(k, "idl_default_const_declaration");
    idl_const_decl[0] = read_string(st);
    if (fileformat >= 41) {
      idl_const_decl[1] = read_string(st);
      idl_const_decl[2] = read_string(st);
    }
    else if (idl_const_decl[0].find("${description}") != -1) {
      idl_const_decl[1] = IDL_CONST_ATTR_DESCR1;
      idl_const_decl[2] = IDL_CONST_ATTR_DESCR2;
    }
    else {
      idl_const_decl[1] = IDL_CONST_ATTR_DECL1;
      idl_const_decl[2] = IDL_CONST_ATTR_DECL2;
    }
    read_keyword(st, "idl_default_enum_item_declaration");
    idl_enum_item_decl = read_string(st);
    read_keyword(st, "idl_default_union_item_declaration");
    idl_union_item_decl[0] = read_string(st);
    if (fileformat >= 41) {
      idl_union_item_decl[1] = read_string(st);
      idl_union_item_decl[2] = read_string(st);
    }
    else if (idl_union_item_decl[0].find("${description}") != -1) {
      idl_union_item_decl[1] = IDL_UNION_ATTR_DESCR1;
      idl_union_item_decl[2] = IDL_UNION_ATTR_DESCR2;
    }
    else {
      idl_union_item_decl[1] = IDL_UNION_ATTR_DECL1;
      idl_union_item_decl[2] = IDL_UNION_ATTR_DECL2;
    }
    read_keyword(st, "idl_association_aggregation_declaration");
    idl_rel_decl[0] = read_string(st);
    idl_rel_decl[1] = read_string(st);
    idl_rel_decl[2] = read_string(st);
    k = read_keyword(st);
    if (!strcmp(k, "idl_valuetype_association_aggregation_declaration")) {
      idl_valuetype_rel_decl[0] = read_string(st);
      idl_valuetype_rel_decl[1] = read_string(st);
      idl_valuetype_rel_decl[2] = read_string(st);
      k = read_keyword(st);
    }
    else {
      // old version
      idl_valuetype_rel_decl[0] = IDL_VALUETYPE_REL_DECL0;
      idl_valuetype_rel_decl[1] = IDL_VALUETYPE_REL_DECL1;
      idl_valuetype_rel_decl[2] = IDL_VALUETYPE_REL_DECL2;
    }
    if (strcmp(k, "idl_union_association_aggregation_declaration"))
      wrong_keyword(k, "idl_union_association_aggregation_declaration");
    idl_union_rel_decl[0] = read_string(st);
    idl_union_rel_decl[1] = read_string(st);
    idl_union_rel_decl[2] = read_string(st);

    read_keyword(st, "idl_get");
    idl_get_name = read_string(st);
    read_keyword(st, "idl_set");
    idl_set_name = read_string(st);
    k = read_keyword(st);
    if (!strcmp(k, "oneway"))
      idl_set_oneway = TRUE;
    else if (!strcmp(k, "twoways"))
      idl_set_oneway = FALSE;
    else
      wrong_keyword(k, "oneway/twoways");
    
    read_keyword(st, "idl_default_operation_declaration");
    idl_oper_decl = read_string(st);

    k = read_keyword(st);
    
    if (!strcmp(k, "uml_get_name")) {
      uml_get_name = drawing_language(read_keyword(st));
      k = read_keyword(st);
    }
    else
      uml_get_name = UmlView;
    
    if (!strcmp(k, "uml_set_name")) {
      uml_set_name = drawing_language(read_keyword(st));
      k = read_keyword(st);
    }
    else
      uml_set_name = UmlView;
  }
  else
    init();
}

void GenerationSettings::read()
{
  char * s = read_file("generation_settings");
    
  if (s != 0) {
    
    PRE_TRY;
    try {
      char * st = s;
      char * k = read_keyword(st);
    
      GenerationSettings::read(st, k);
    }
    catch (int) {
      ;
    }
    POST_TRY;
    delete [] s;
  }
}

static bool read_incl(IncludesSpec & sp, const char * filename)
{
  char * s = read_file(filename);
    
  if (s != 0) {
    sp.types.clear();
    sp.includes.clear();
    
    PRE_TRY;
    try {
      char * st = s;
      
      while (! at_end(st)) {
	sp.types.append(read_string(st));
	sp.includes.append(read_string(st));
      }
    }
    catch (int) {
      ;
    }
    POST_TRY;
    delete [] s;
    return TRUE;
  }
  else
    return FALSE;
}

bool GenerationSettings::import()
{
  QString fn = QFileDialog::getOpenFileName(last_used_directory(), "generation_settings");
      
  if (!fn.isEmpty()) {
    set_last_used_directory(fn);
    
    // format is missing in generation setting file
    // have to read associated project file format
    if (!BrowserPackage::load_version(fn))
      return FALSE;
    
    char * s = read_file(fn);
    
    if (s != 0) {
      PRE_TRY;
      try {
	char * st = s;
	char * k = read_keyword(st);
	
	GenerationSettings::read(st, k);
      }
      catch (int) {
	;
      }
      POST_TRY;
      delete [] s;
      
      read_incl(cpp_includes, fn.replace(fn.findRev("generation_settings"), 19, "cpp_includes"));
      read_incl(java_imports, fn.replace(fn.findRev("cpp_includes"), 12, "java_imports"));
      read_incl(python_imports, fn.replace(fn.findRev("java_imports"), 12, "python_imports"));
      read_incl(idl_includes, fn.replace(fn.findRev("python_imports"), 14, "idl_includes"));
   
      return TRUE;
    }
  }
  
  return FALSE;
}

bool GenerationSettings::add_class_rel_correspondences(unsigned fileformat)
{
  bool result = TRUE;
  char * s = read_file("generation_settings");
  
  if (s != 0) {
    PRE_TRY;
    try {
      char * st = s;
      int index;
      int n;
      
      skip_until(st, "relations_stereotypes");

      n = (int) read_unsigned(st);
      if (n != 0) {
	if (relattr_stereotypes == 0) {
	  nrelattrstereotypes = n;
	  relattr_stereotypes = new Stereotype[nrelattrstereotypes];
	  
	  for (index = 0; index != nrelattrstereotypes; index += 1) {
	    Stereotype & ste = relattr_stereotypes[index];
	    
	    read_rel_att_st(st, ste, fileformat);
	  }
	}
	else {
	  int oldn = nrelattrstereotypes;
	  Stereotype * oldst = relattr_stereotypes;
	  
	  relattr_stereotypes = new Stereotype[oldn + n /*worst case*/];
	  
	  for (index = 0; index != oldn; index += 1)
	    relattr_stereotypes[index] = oldst[index];
	  delete [] oldst;
	  
	  for (index = 0; index != n; index += 1) {
	    Stereotype & ste = relattr_stereotypes[nrelattrstereotypes];
	
	    read_rel_att_st(st, ste, fileformat);
	    
	    int index2;
	      
	    for (index2 = 0; index2 != oldn; index2 += 1)
	      if (relattr_stereotypes[index2].uml == ste.uml)
		break;
	    
	    if (index2 == oldn)
	      // add it
	      nrelattrstereotypes += 1;
	  }
	}
      }
      
      read_keyword(st, "classes_stereotypes");
      
      n = (int) read_unsigned(st);
      if (n != 0) {
	if (class_stereotypes == 0) {
	  nclassstereotypes = n;
	  class_stereotypes = new Stereotype[nclassstereotypes];
	  
	  for (index = 0; index != nclassstereotypes; index += 1) {
	    Stereotype & ste = class_stereotypes[index];
	    
	    read_class_st(st, ste, fileformat);
	  }
	}
	else {
	  int oldn = nclassstereotypes;
	  Stereotype * oldst = class_stereotypes;
	  
	  class_stereotypes = new Stereotype[oldn + n /*worst case*/];
	  
	  for (index = 0; index != oldn; index += 1)
	    class_stereotypes[index] = oldst[index];
	  delete [] oldst;
	  
	  for (index = 0; index != n; index += 1) {
	    Stereotype & ste = class_stereotypes[nclassstereotypes];
	
	    read_class_st(st, ste, fileformat);
	    
	    int index2;
	      
	    for (index2 = 0; index2 != oldn; index2 += 1)
	      if (class_stereotypes[index2].uml == ste.uml)
		break;
	    
	    if (index2 == oldn)
	      // add it
	      nclassstereotypes += 1;
	  }
	}
      }      
    }
    catch (int) {
      result = FALSE;
    }
    POST_TRY;
    delete [] s;
  }
  
  return result;
}

void GenerationSettings::read_includes_imports()
{
  if (!read_incl(cpp_includes, "cpp_includes") &&
      read_incl(cpp_includes, "include")) {
    // old version
    QDir d = BrowserView::get_dir();
    
    d.rename("include", "cpp_includes");  
  }
  
  if (!read_incl(java_imports, "java_imports") &&
      read_incl(cpp_includes, "import")) {
    // old version
    QDir d = BrowserView::get_dir();
    
    d.rename("import", "java_imports");  
  }
  
  read_incl(python_imports, "python_imports");
  read_incl(idl_includes, "idl_includes");
}

QString GenerationSettings::new_java_enums()
{
  QString new_st = "enum_pattern_";
  
  // search for an unused stereotype like enum_pattern_
  
  int index;
  
  index = 0;
  while (index != nclassstereotypes) {
    if (class_stereotypes[index].uml == new_st) {
      index = 0;
      new_st += '_';
    }
    else
      index += 1;
  }
  
  // search for "enum_pattern" to rename it new_st
  
  for (index = 0; index != nclassstereotypes; index += 1) {
    if (class_stereotypes[index].uml == "enum_pattern") {
      class_stereotypes[index].uml = new_st;
      break;
    }
  }
  if (index == nclassstereotypes)
    new_st = "";
  
  // search for java stereotype "enum" to setthem to "enum_pattern"
  // do not change uml stereotype "enum"
  
  for (index = 0; index != nclassstereotypes; index += 1) {
    if ((class_stereotypes[index].java == "enum") &&
	(class_stereotypes[index].uml != "enum"))
      class_stereotypes[index].java = "enum_pattern";
  }
  
  // search for "enum" to add "enum_pattern"
  
  for (index = 0; index != nclassstereotypes; index += 1) {
    if (class_stereotypes[index].uml == "enum") {
      Stereotype * s = new Stereotype[nclassstereotypes + 1];
      int index1;
  
      for (index1 = 0; index1 <= index; index1 += 1)
	s[index1] = class_stereotypes[index1];
      
      s[index1] = class_stereotypes[index];
      s[index1].uml = s[index1].java = "enum_pattern";
      
      while (++index != nclassstereotypes)
	s[index + 1] = class_stereotypes[index];
      
      delete [] class_stereotypes;
      class_stereotypes = s;
      nclassstereotypes += 1;
      return new_st;
    }
  }

  // no "enum", add "enum_pattern"
  
  Stereotype * s = new Stereotype[nclassstereotypes + 1];
  
  for (index = 0; index != nclassstereotypes; index += 1)
    s[index] = class_stereotypes[index];
  
  s[index].set("enum_pattern", "enum", "enum_pattern", "ignored", "ignored", "enum");
  
  if (class_stereotypes != 0)
    delete [] class_stereotypes;
  class_stereotypes = s;
  nclassstereotypes += 1;
  
  return new_st;
}

void ReverseRoundtripFilter::send_def(ToolCom * com) {
  com->write_string(regexp);
  com->write_bool(case_sensitive);
}

void ReverseRoundtripFilter::receive_def(const char * args) {
  case_sensitive = (*args != 0);
  regexp = args + 1;
}

void ReverseRoundtripFilter::save(const char * key, QTextOStream & st) {
  if (! regexp.isEmpty()) {
    st << key << ' ';
    save_string(regexp, st);
    if (case_sensitive)
      st << " case_sensitive";
    nl_indent(st);
  }
}

void ReverseRoundtripFilter::read(const char * key, char * & st, char * & k) {
  if (! strcmp(k, key)) {
    regexp = read_string(st);
    k = read_keyword(st);
    if (! strcmp(k, "case_sensitive")) {
      case_sensitive = TRUE;
      k = read_keyword(st);
    }
  }
}
