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

#ifndef GENERATIONSETTINGSDIALOG_H
#define GENERATIONSETTINGSDIALOG_H

#include <qtabdialog.h>

#include "VisibilityGroup.h"
#include "StringTable.h"
#include "GenerationSettings.h"

class QComboBox;
class QCheckBox;
class QRadioButton;

class LineEdit;
class MultiLineEdit;
class TypesTable;
class StereotypesTable;
class RelationTable;
class IncludeTable;

class GenerationSettingsDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    // all
    TypesTable * types_table;
  
    // stereotypes
    StereotypesTable * relation_stereotypes_table; 
    StereotypesTable * class_stereotypes_table; 
  
    // C++ specific
    MultiLineEdit * edcpp_h_content;
    MultiLineEdit * edcpp_src_content;
    QComboBox * edcpp_h_extension;
    QComboBox * edcpp_src_extension;
    QComboBox * cpp_include_with_path_cb;
    QCheckBox * cpp_force_namespace_gen_cb;
    QCheckBox * cpp_inline_force_incl_in_h_cb;
    QCheckBox * cpp_javadoc_cb;
    LineEdit * cpp_enum_in;
    LineEdit * cpp_enum_out;
    LineEdit * cpp_enum_inout;
    LineEdit * cpp_enum_return;
    LineEdit * cpp_in;
    LineEdit * cpp_out;
    LineEdit * cpp_inout;
    LineEdit * cpp_return;
    MultiLineEdit * edcpp_class_decl;
    MultiLineEdit * edcpp_external_class_decl;
    MultiLineEdit * edcpp_struct_decl;
    MultiLineEdit * edcpp_union_decl;
    MultiLineEdit * edcpp_enum_decl;
    MultiLineEdit * edcpp_typedef_decl;
    MultiLineEdit * edcpp_attr_decl[3];
    MultiLineEdit * edcpp_enum_item_decl;
    MultiLineEdit * edcpp_rel_decl[2][3];
    VisibilityGroup cpp_get_visibility;
    LineEdit * edcpp_get_name;
    QCheckBox * uml_follow_cpp_get_name;
    QCheckBox * cpp_get_inline_cb;
    QCheckBox * cpp_get_const_cb;
    QCheckBox * cpp_get_value_const_cb;
    VisibilityGroup cpp_set_visibility;
    LineEdit * edcpp_set_name;
    QCheckBox * uml_follow_cpp_set_name;
    QCheckBox * cpp_set_inline_cb;
    QCheckBox * cpp_set_param_const_cb;
    QCheckBox * cpp_set_param_ref_cb;
    MultiLineEdit * edcpp_oper_decl;
    MultiLineEdit * edcpp_oper_def;
    QCheckBox * cpp_force_throw_cb;
    IncludeTable * cpp_include_table;
    QComboBox * indentvisi_cb;
  
    // Java specific
    MultiLineEdit * edjava_src_content;
    QComboBox * edjava_extension;
    QCheckBox * java_javadoc_cb;
    QCheckBox * java_force_package_gen_cb;
    MultiLineEdit * edjava_class_decl;
    LineEdit * edjava_external_class_decl;
    MultiLineEdit * edjava_interface_decl;
    MultiLineEdit * edjava_enum_decl;
    MultiLineEdit * edjava_enum_pattern_decl;
    MultiLineEdit * edjava_attr_decl[3];
    MultiLineEdit * edjava_enum_item_decl;
    MultiLineEdit * edjava_enum_pattern_item_decl;
    MultiLineEdit * edjava_enum_pattern_item_case;
    MultiLineEdit * edjava_rel_decl[3];
    MultiLineEdit * edjava_oper_def;
    VisibilityGroup java_get_visibility;
    LineEdit * edjava_get_name;
    QCheckBox * uml_follow_java_get_name;
    QCheckBox * java_get_final_cb;
    VisibilityGroup java_set_visibility;
    LineEdit * edjava_set_name;
    QCheckBox * uml_follow_java_set_name;
    QCheckBox * java_set_final_cb;
    QCheckBox * java_set_param_final_cb;
    IncludeTable * java_import_table;
  
    // Php specific
    MultiLineEdit * edphp_src_content;
    QComboBox * edphp_extension;
    QCheckBox * php_javadoc_cb;
    QComboBox * php_require_with_path_cb;
    QCheckBox * php_force_namespace_gen_cb;
    MultiLineEdit * edphp_class_decl;
    LineEdit * edphp_external_class_decl;
    MultiLineEdit * edphp_interface_decl;
    MultiLineEdit * edphp_enum_decl;
    MultiLineEdit * edphp_attr_decl;
    MultiLineEdit * edphp_enum_item_decl;
    MultiLineEdit * edphp_rel_decl;
    MultiLineEdit * edphp_oper_def;
    VisibilityGroup php_get_visibility;
    LineEdit * edphp_get_name;
    QCheckBox * uml_follow_php_get_name;
    QCheckBox * php_get_final_cb;
    VisibilityGroup php_set_visibility;
    LineEdit * edphp_set_name;
    QCheckBox * uml_follow_php_set_name;
    QCheckBox * php_set_final_cb;
  
    // Python specific
    QComboBox * indentstep_cb;
    MultiLineEdit * edpython_src_content;
    QComboBox * edpython_extension;
    QCheckBox * python_2_2_cb;
    QCheckBox * python_3_operation_cb;
    MultiLineEdit * edpython_class_decl;
    MultiLineEdit * edpython_enum_decl;
    MultiLineEdit * edpython_external_class_decl;
    MultiLineEdit * edpython_attr_decl[2];
    MultiLineEdit * edpython_enum_item_decl;
    MultiLineEdit * edpython_rel_decl[2][2];
    MultiLineEdit * edpython_oper_def;
    MultiLineEdit * edpython_initoper_def;
    VisibilityGroup python_get_visibility;
    LineEdit * edpython_get_name;
    QCheckBox * uml_follow_python_get_name;
    VisibilityGroup python_set_visibility;
    LineEdit * edpython_set_name;
    QCheckBox * uml_follow_python_set_name;
    IncludeTable * python_import_table;
  
    // Idl specific
    MultiLineEdit * edidl_src_content;
    QComboBox * edidl_extension;
    MultiLineEdit * edidl_external_class_decl;
    MultiLineEdit * edidl_interface_decl;
    MultiLineEdit * edidl_valuetype_decl;
    MultiLineEdit * edidl_struct_decl;
    MultiLineEdit * edidl_union_decl;
    MultiLineEdit * edidl_typedef_decl;
    MultiLineEdit * edidl_exception_decl;
    MultiLineEdit * edidl_enum_decl;
    MultiLineEdit * edidl_attr_decl[3];
    MultiLineEdit * edidl_valuetype_attr_decl[3];
    MultiLineEdit * edidl_const_decl[3];
    MultiLineEdit * edidl_union_item_decl[3];
    MultiLineEdit * edidl_enum_item_decl;
    MultiLineEdit * edidl_rel_decl[3];
    MultiLineEdit * edidl_valuetype_rel_decl[3];
    MultiLineEdit * edidl_union_rel_decl[3];
    MultiLineEdit * edidl_oper_decl;
    LineEdit * edidl_get_name;
    LineEdit * edidl_set_name;
    QCheckBox * uml_follow_idl_get_name;
    QCheckBox * uml_follow_idl_set_name;
    QCheckBox * idl_set_oneway_cb;
    IncludeTable * idl_include_table;
    
    // descriptions
    MultiLineEdit * edartifact_default_description;
    MultiLineEdit * edclass_default_description;
    MultiLineEdit * edoperation_default_description;
    MultiLineEdit * edattribute_default_description;
    MultiLineEdit * edrelation_default_description;
    
    // directories
    LineEdit * edcpproot;
    LineEdit * edjavaroot; 
    LineEdit * edphproot;  
    LineEdit * edpythonroot;  
    LineEdit * edidlroot;
    QPushButton * cpprelbutton;
    QPushButton * javarelbutton;
    QPushButton * phprelbutton;
    QPushButton * pythonrelbutton;
    QPushButton * idlrelbutton;
    
    static QSize previous_size;
  
  public:
    GenerationSettingsDialog();
    virtual ~GenerationSettingsDialog();
  
  protected:
    void init_types();
    void init_stereotypes();
    void init_cpp1();
    void init_cpp2();
    void init_cpp3();
    void init_cpp4();
    void init_cpp5();
    void init_java1();
    void init_java2();
    void init_java3();
    void init_java4();
    void init_php1();
    void init_php2();
    void init_python1();
    void init_python2();
    void init_python3();
    void init_python4();
    void init_idl1();
    void init_idl2();
    void init_idl3();
    void init_idl4();
    void init_idl5();
    void init_descriptions();
    void init_dirs();
    void relative(LineEdit * ed, QPushButton * button);
    
  protected slots:
    virtual void polish();
    virtual void accept();
    void cpproot_browse();
    void javaroot_browse();
    void phproot_browse();
    void pythonroot_browse();
    void idlroot_browse();
    void cpp_relative();
    void java_relative();
    void php_relative();
    void python_relative();
    void idl_relative();
    void java_get_visi_changed(int);
    void java_set_visi_changed(int);
    void php_get_visi_changed(int);
    void php_set_visi_changed(int);
    void follow_cpp_get_name();
    void follow_cpp_set_name();
    void follow_java_get_name();
    void follow_java_set_name();
    void follow_php_get_name();
    void follow_php_set_name();
    void follow_python_get_name();
    void follow_python_set_name();
    void follow_idl_get_name();
    void follow_idl_set_name();
};

class TypesTable : public StringTable {
  public:
    TypesTable(QWidget * parent);
  
    virtual void init_row(int r);

    void update();
    bool check();
};

class StereotypesTable : public StringTable {
  protected:
    bool with_php;
  public:
    StereotypesTable(QWidget * parent, int nst, Stereotype * st, bool php);
  
    virtual void init_row(int r);
  
    void update(int & nst, Stereotype *& st);
};

class IncludeTable : public StringTable {
  protected:
    IncludesSpec & spec;
    const char * dflt;
  
  public:
    IncludeTable(QWidget * parent, IncludesSpec & spc,
		 const char * title, const char * df);
  
  public:
    virtual void init_row(int index);
							   
    void update();
};

#endif
