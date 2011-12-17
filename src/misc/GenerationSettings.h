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

#ifndef GENERATIONSETTINGS_H
#define GENERATIONSETTINGS_H

#include <qtextstream.h>
#include <qstringlist.h>

#include "UmlEnum.h"
#include "mystr.h"

class QDir;
class AType;
class ToolCom;

class Builtin {
  public:
    QString uml;
    QString cpp;
    QString java;
    QString idl;
    QString cpp_in;
    QString cpp_out;
    QString cpp_inout;
    QString cpp_return;
    
    void set(const char * u, const char * c, const char * j, const char * i);
};

class Stereotype {
  public:
    QString uml;
    QString cpp;
    QString java;
    QString php;
    QString python;
    QString idl;
    
    void set(const char * u, const char * c, const char * j,
	     const char * p, const char * y, const char * i);
};

// on the tool API one use a QDict, it is useless here because
// there is no search, list usage allow to save the order
struct IncludesSpec {
  QStringList types;
  QStringList includes;
};

struct ReverseRoundtripFilter {
  QString regexp;
  bool case_sensitive;
  
  void send_def(ToolCom * com);
  void receive_def(const char * args);
  void save(const char * key, QTextOStream & st);
  void read(const char * key, char * & st, char * & k);
};

class GenerationSettings {
  friend class GenerationSettingsDialog;
  friend class RevSettingsDialog;
  friend class TypesTable;
  friend class StereotypesTable;
  
  protected:
    static int nbuiltins;
    static Builtin * builtins;
    static QStringList umltypes;
    
    static bool cpp_default_defs;
    static bool cpp_javadoc_comment;
    static SharedStr cpp_h_content;
    static SharedStr cpp_src_content;
    static QString cpp_in;
    static QString cpp_out;
    static QString cpp_inout;
    static QString cpp_return;
    static QString cpp_enum_in;
    static QString cpp_enum_out;
    static QString cpp_enum_inout;
    static QString cpp_enum_return;
    static SharedStr cpp_class_decl;
    static SharedStr cpp_external_class_decl;
    static SharedStr cpp_struct_decl;
    static SharedStr cpp_union_decl;
    static SharedStr cpp_enum_decl;
    static SharedStr cpp_typedef_decl;
    static SharedStr cpp_attr_decl[3/*multiplicity*/];
    static SharedStr cpp_enum_item_decl;
    static SharedStr cpp_rel_decl[2/*relation kind*/][3/*multiplicity*/];
    static SharedStr cpp_oper_decl;
    static SharedStr cpp_oper_def;
    static bool cpp_force_throw;
    static UmlVisibility cpp_get_visibility;
    static SharedStr cpp_get_name;
    static bool cpp_get_inline;
    static bool cpp_get_const;
    static bool cpp_get_value_const;
    static UmlVisibility cpp_set_visibility;
    static SharedStr cpp_set_name;
    static bool cpp_set_inline;
    static bool cpp_set_param_const;
    static bool cpp_set_param_ref;
    static QCString cpp_indent_visibility;
    
    static bool java_default_defs;
    static bool java_javadoc_comment;
    static SharedStr java_src_content;
    static SharedStr java_class_decl;
    static SharedStr java_external_class_decl;
    static SharedStr java_enum_decl;
    static SharedStr java_enum_pattern_decl;
    static SharedStr java_interface_decl;
    static SharedStr java_attr_decl[3/*multiplicity*/];
    static SharedStr java_enum_item_decl;
    static SharedStr java_enum_pattern_item_decl;
    static SharedStr java_enum_pattern_item_case;
    static SharedStr java_rel_decl[3/*multiplicity*/];
    static SharedStr java_oper_def;
    static UmlVisibility noncpp_get_visibility;
    static SharedStr java_get_name;
    static bool java_get_final;
    static UmlVisibility noncpp_set_visibility;
    static SharedStr java_set_name;
    static bool java_set_final;
    static bool java_set_param_final;
    
    static bool php_default_defs;
    static bool php_javadoc_comment;
    static SharedStr php_src_content;
    static SharedStr php_class_decl;
    static SharedStr php_external_class_decl;
    static SharedStr php_enum_decl;
    static SharedStr php_interface_decl;
    static SharedStr php_attr_decl;
    static SharedStr php_enum_item_decl;
    static SharedStr php_rel_decl;
    static SharedStr php_oper_def;
    static SharedStr php_get_name;
    static bool php_get_final;
    static SharedStr php_set_name;
    static bool php_set_final;
    
    static bool python_default_defs;
    static bool python_2_2;
    static bool python_3_operation;
    static QString python_indent_step;
    static SharedStr python_src_content;
    static SharedStr python_class_decl;
    static SharedStr python_external_class_decl;
    static SharedStr python_enum_decl;
    static SharedStr python_attr_decl[2/*multiplicity*/];
    static SharedStr python_enum_item_decl;
    static SharedStr python_rel_decl[2/*relation kind*/][2/*multiplicity*/];
    static SharedStr python_oper_def;
    static SharedStr python_initoper_def;
    static SharedStr python_get_name;
    static SharedStr python_set_name;
    
    static bool idl_default_defs;
    static SharedStr idl_src_content;
    static SharedStr idl_external_class_decl;
    static SharedStr idl_interface_decl;
    static SharedStr idl_valuetype_decl;
    static SharedStr idl_struct_decl;
    static SharedStr idl_typedef_decl;
    static SharedStr idl_exception_decl;
    static SharedStr idl_union_decl;
    static SharedStr idl_enum_decl;
    static SharedStr idl_attr_decl[3/*multiplicity*/];
    static SharedStr idl_valuetype_attr_decl[3/*multiplicity*/];
    static SharedStr idl_union_item_decl[3/*multiplicity*/];
    static SharedStr idl_enum_item_decl;
    static SharedStr idl_const_decl[3/*multiplicity*/];
    static SharedStr idl_rel_decl[3/*multiplicity*/];
    static SharedStr idl_valuetype_rel_decl[3/*multiplicity*/];
    static SharedStr idl_union_rel_decl[3/*multiplicity*/];
    static SharedStr idl_oper_decl;
    static SharedStr idl_get_name;
    static SharedStr idl_set_name;
    static bool idl_set_oneway;
    
    static DrawingLanguage uml_get_name;
    static DrawingLanguage uml_set_name;

    static QString cpp_h_extension;
    static QString cpp_src_extension;
    static bool cpp_include_with_path;
    static bool cpp_relative_path;
    static bool cpp_root_relative_path;
    static bool cpp_force_namespace_gen;
    static bool cpp_inline_force_incl_in_h;
    static IncludesSpec cpp_includes;
    static QString java_extension;
    static bool java_force_package_gen;
    static IncludesSpec java_imports;
    static IncludesSpec python_imports;
    static QString php_extension;
    static bool php_req_with_path;
    static bool php_relative_path;
    static bool php_root_relative_path;
    static bool php_force_namespace_gen;
    static QString python_extension;
    static QString idl_extension;
    static IncludesSpec idl_includes;
    
    static int nrelattrstereotypes;
    static Stereotype * relattr_stereotypes;
    
    static int nclassstereotypes;
    static Stereotype * class_stereotypes;
    
    static QString artifact_default_description;
    static QString class_default_description;
    static QString operation_default_description;
    static QString attribute_default_description;
    static QString relation_default_description;
    
    static QString cpp_root_dir;
    static QString java_root_dir;
    static QString php_root_dir;
    static QString python_root_dir;
    static QString idl_root_dir;
    
    static ReverseRoundtripFilter cpp_dir_filter;
    static ReverseRoundtripFilter cpp_file_filter;
    static ReverseRoundtripFilter java_dir_filter;
    static ReverseRoundtripFilter java_file_filter;
    static ReverseRoundtripFilter php_dir_filter;
    static ReverseRoundtripFilter php_file_filter;
  
    static int find_type(const QString &);
    static int find_relationattribute_stereotype(const QString &);
    static int find_class_stereotype(const QString &);
    static Builtin & get_type(const char * u);
    static Stereotype & get_stereotype(int & n, Stereotype * & st, const char * u);
    
    static void set_include(IncludesSpec & is, ToolCom * com, const char * args);
    
    static void send_uml_def(ToolCom * com);
    static void send_cpp_def(ToolCom * com);
    static void send_java_def(ToolCom * com);
    static void send_php_def(ToolCom * com);
    static void send_python_def(ToolCom * com);
    static void send_idl_def(ToolCom * com);
    
  public:
    static void init();
  
    static const QStringList & basic_types() { return umltypes; };
    static QString cpp_type(const QString &);
    static QString java_type(const QString &);
    static QString idl_type(const QString &);
    
    static bool cpp_get_default_defs() { return cpp_default_defs; };
    static bool cpp_set_default_defs(bool y);
    static const char * cpp_default_header_content() { return cpp_h_content; };
    static const char * cpp_default_source_content() { return cpp_src_content; };
    static const char * cpp_default_class_decl() { return cpp_class_decl; };
    static const char * cpp_default_external_class_decl() { return cpp_external_class_decl; };
    static const char * cpp_default_struct_decl() { return cpp_struct_decl; };
    static const char * cpp_default_union_decl() { return cpp_union_decl; };
    static const char * cpp_default_enum_decl() { return cpp_enum_decl; };
    static const char * cpp_default_typedef_decl() { return cpp_typedef_decl; };
    static const char * cpp_default_attr_decl(const QString & mult);
    static const char * cpp_default_enum_item_decl() { return cpp_enum_item_decl; };
    static const char * cpp_default_rel_decl(UmlCode rel, const QString & mult);
    static const char * cpp_default_oper_decl() { return cpp_oper_decl; };
    static const char * cpp_default_oper_def() { return cpp_oper_def; };
    static bool cpp_oper_force_throw() { return cpp_force_throw; };
    static UmlVisibility cpp_default_get_visibility() { return cpp_get_visibility; };
    static const char * cpp_default_get_name() { return cpp_get_name; };
    static bool cpp_default_get_inline() { return cpp_get_inline; };
    static bool cpp_default_get_const() { return cpp_get_const; };
    static bool cpp_default_get_value_const() { return cpp_get_value_const; };
    static UmlVisibility cpp_default_set_visibility() { return cpp_set_visibility; };
    static const char * cpp_default_set_name() { return cpp_set_name; };
    static bool cpp_default_set_inline() { return cpp_set_inline; };
    static bool cpp_default_set_param_const() { return cpp_set_param_const; };
    static bool cpp_default_set_param_ref() { return cpp_set_param_ref; };
    static void set_cpp_return_type(const AType & type, QString & s);
    static const QString cpp(const AType & type, UmlParamDirection, int);
    static bool cpp_javadoc_style() { return cpp_javadoc_comment; }
    
    static bool java_get_default_defs() { return java_default_defs; };
    static bool java_set_default_defs(bool y);
    static const char * java_default_source_content() { return java_src_content; };
    static const char * java_default_class_decl() { return java_class_decl; };
    static const char * java_default_external_class_decl() { return java_external_class_decl; };
    static const char * java_default_interface_decl() { return java_interface_decl; };
    static const char * java_default_enum_decl() { return java_enum_decl; };
    static const char * java_default_enum_pattern_decl() { return java_enum_pattern_decl; };
    static const char * java_default_attr_decl(const QString & mult);
    static const char * java_default_enum_item_decl() { return java_enum_item_decl; };
    static const char * java_default_enum_pattern_item_decl() { return java_enum_pattern_item_decl; };
    static const char * java_default_enum_pattern_item_case() { return java_enum_pattern_item_case; };
    static const char * java_default_rel_decl(const QString & mult);
    static const char * java_default_oper_def() { return java_oper_def; };
    static UmlVisibility noncpp_default_get_visibility() { return noncpp_get_visibility; };
    static const char * java_default_get_name() { return java_get_name; };
    static bool java_default_get_final() { return java_get_final; };
    static UmlVisibility noncpp_default_set_visibility() { return noncpp_set_visibility; };
    static const char * java_default_set_name() { return java_set_name; };
    static bool java_default_set_final() { return java_set_final; };
    static bool java_default_set_param_final() { return java_set_param_final; };
    static bool java_javadoc_style() { return java_javadoc_comment; }

    static bool php_get_default_defs() { return php_default_defs; };
    static bool php_set_default_defs(bool y);
    static const char * php_default_source_content() { return php_src_content; };
    static const char * php_default_class_decl() { return php_class_decl; };
    static const char * php_default_external_class_decl() { return php_external_class_decl; };
    static const char * php_default_enum_decl() { return php_enum_decl; };
    static const char * php_default_interface_decl() { return php_interface_decl; };
    static const char * php_default_attr_decl() { return php_attr_decl; }
    static const char * php_default_enum_item_decl() { return php_enum_item_decl; };
    static const char * php_default_rel_decl() { return php_rel_decl; }
    static const char * php_default_oper_def() { return php_oper_def; };
    static const char * php_default_get_name() { return php_get_name; };
    static bool php_default_get_final() { return php_get_final; };
    static const char * php_default_set_name() { return php_set_name; };
    static bool php_default_set_final() { return php_set_final; };
    static bool php_javadoc_style() { return php_javadoc_comment; }

    static bool python_get_default_defs() { return python_default_defs; };
    static bool python_set_default_defs(bool y);
    static bool python_default_2_2() { return python_2_2; };
    static bool python_default_operation3() { return python_3_operation; };
    static const QString & python_get_indent_step() { return python_indent_step; }
    static const char * python_default_source_content() { return python_src_content; };
    static const char * python_default_class_decl() { return python_class_decl; };
    static const char * python_default_external_class_decl() { return python_external_class_decl; };
    static const char * python_default_enum_decl() { return python_enum_decl; };
    static const char * python_default_attr_decl(const QString & mult);
    static const char * python_default_enum_item_decl() { return python_enum_item_decl; };
    static const char * python_default_rel_decl(UmlCode rel, const QString & mult);
    static const char * python_default_oper_def() { return python_oper_def; };
    static const char * python_default_initoper_def() { return python_initoper_def; };
    static const char * python_default_get_name() { return python_get_name; };
    static const char * python_default_set_name() { return python_set_name; };

    static bool idl_get_default_defs() { return idl_default_defs; };
    static bool idl_set_default_defs(bool y);
    static const char * idl_default_source_content() { return idl_src_content; };
    static const char * idl_default_external_class_decl() { return idl_external_class_decl; };
    static const char * idl_default_interface_decl() { return idl_interface_decl; };
    static const char * idl_default_valuetype_decl() { return idl_valuetype_decl; };
    static const char * idl_default_struct_decl() { return idl_struct_decl; };
    static const char * idl_default_typedef_decl() { return idl_typedef_decl; };
    static const char * idl_default_exception_decl() { return idl_exception_decl; };
    static const char * idl_default_union_decl() { return idl_union_decl; };
    static const char * idl_default_enum_decl() { return idl_enum_decl; };
    static const char * idl_default_attr_decl(const QString & mult);
    static const char * idl_default_valuetype_attr_decl(const QString & mult);
    static const char * idl_default_const_decl(const QString & mult);
    static const char * idl_default_union_item_decl(const QString & mult);
    static const char * idl_default_enum_item_decl() { return idl_enum_item_decl; };
    static const char * idl_default_rel_decl(const QString & mult);
    static const char * idl_default_valuetype_rel_decl(const QString & mult);
    static const char * idl_default_union_rel_decl(const QString & mult);
    static const char * idl_default_oper_decl() { return idl_oper_decl; };
    static const char * idl_default_get_name() { return idl_get_name; };
    static const char * idl_default_set_name() { return idl_set_name; };
    static bool idl_default_set_oneway() { return idl_set_oneway; };
    
    static DrawingLanguage uml_default_get_name() { return uml_get_name; }
    static DrawingLanguage uml_default_set_name() { return uml_set_name; }
    
    static QString cpp_relationattribute_stereotype(const QString &);
    static QString java_relationattribute_stereotype(const QString &);
    static QString python_relationattribute_stereotype(const QString &);
    static QString idl_relationattribute_stereotype(const QString &);
    
    static QString cpp_class_stereotype(const QString &);
    static QString java_class_stereotype(const QString &);
    static QString php_class_stereotype(const QString &);
    static QString python_class_stereotype(const QString &);
    static QString idl_class_stereotype(const QString &);
    
    static QString default_artifact_description() { return artifact_default_description; }
    static QString default_class_description() { return class_default_description; }
    static QString default_operation_description() { return operation_default_description; }
    static QString default_attribute_description() { return attribute_default_description; }
    static QString default_relation_description() { return relation_default_description; }
    
    static const QString & get_cpp_root_dir() { return cpp_root_dir; };
    static const QString & get_java_root_dir() { return java_root_dir; };
    static const QString & get_php_root_dir() { return php_root_dir; };
    static const QString & get_python_root_dir() { return python_root_dir; };
    static const QString & get_idl_root_dir() { return idl_root_dir; };
    
    static const QString & get_cpp_h_extension() { return cpp_h_extension; };
    static const QString & get_cpp_src_extension() { return cpp_src_extension; };
    static const QString & get_java_extension() { return java_extension; };
    static const QString & get_php_extension() { return php_extension; };
    static const QString & get_python_extension() { return python_extension; };
    static const QString & get_idl_extension() { return idl_extension; };

    static bool edit();
    
    static bool tool_global_uml_cmd(ToolCom * com, const char * args);
    static bool tool_global_cpp_cmd(ToolCom * com, const char * args);
    static bool tool_global_java_cmd(ToolCom * com, const char * args);
    static bool tool_global_php_cmd(ToolCom * com, const char * args);
    static bool tool_global_python_cmd(ToolCom * com, const char * args);
    static bool tool_global_idl_cmd(ToolCom * com, const char * args);

    static void save_dirs(QTextStream & st);
    static void save_descriptions(QTextStream & st);
    static void save();
    static void read_dirs(char * & st, char * & k);
    static void read_descriptions(char * & st, char * & k);
    static void read(char * & st, char * & k);
    static void read();
    static bool import();
    static void read_includes_imports();
    static bool add_class_rel_correspondences(unsigned fileformat);
    
    static QString new_java_enums();
};

#endif
