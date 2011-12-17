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

#ifndef ATTRIBUTEDATA_H
#define ATTRIBUTEDATA_H

#include "AType.h"
#include "ClassMemberData.h"

class BrowserAttribute;

class AttributeData : public ClassMemberData {
  Q_OBJECT
    
  friend class AttributeDialog;
  
  protected:
    AType type;
    SharedStr multiplicity;
    MyStr init_value;
    MyStr constraint;
    bool is_deleted : 1;
    bool isa_class_attribute : 1;
    bool isa_volatile_attribute : 1;
    bool isa_const_attribute : 1;
    bool is_derived : 1;
    bool is_derivedunion : 1;
    bool is_ordered : 1;
    bool is_unique : 1;
    bool cpp_mutable: 1;
    bool java_transient: 1;
    bool dummy : 6;
    UmlVisibility uml_visibility : 8;	// : 4 useless here, : 8 faster than : 3 ?
    
    // C++
    UmlVisibility cpp_visibility : 8;	// : 4 useless here, : 8 faster than : 3 ?
    SharedStr cpp_decl;
    
    // Java
    SharedStr java_decl;
    SharedStr java_annotation;
    
    // Php
    SharedStr php_decl;
    
    // Python
    SharedStr python_decl;
    
    // Idl
    BrowserAttribute * idl_case;	// exclusive with idl_explicit_case
    MyStr idl_explicit_case;
    SharedStr idl_decl;
    
    virtual void send_uml_def(ToolCom * com, BrowserNode * bn,
			      const QString & comment);
    virtual void send_cpp_def(ToolCom * com);
    virtual void send_java_def(ToolCom * com);
    virtual void send_php_def(ToolCom * com);
    virtual void send_python_def(ToolCom * com);
    virtual void send_idl_def(ToolCom * com);
    
  public:
    AttributeData();
    AttributeData(const AttributeData * model, BrowserNode *);
    virtual ~AttributeData();
    
    virtual bool deletedp() const;
    virtual void set_deletedp(bool y);
        
    void set_browser_node(BrowserAttribute *, bool update, bool enum_item);
  
    virtual QString definition(bool full, bool with_kind) const;
    QString definition(bool full, bool mult, bool init, bool modif,
		       DrawingLanguage language, ShowContextMode mode = noContext) const;
    
    virtual bool decldefbody_contain(const QString & s, bool cs, BrowserNode *);
    
    const char * get_constraint() const { return constraint; }
    
    const char * get_multiplicity() const { return multiplicity; }

    bool get_isa_class_attribute() const { return isa_class_attribute; }
    
    bool get_isa_const_attribute() const { return isa_const_attribute; }
    
    bool get_is_derived() const { return is_derived; }
    
    bool get_is_derivedunion() const { return is_derivedunion; }
    
    bool get_is_ordered() const { return is_ordered; }
    
    bool get_is_unique() const { return is_unique; }
    
    UmlVisibility get_uml_visibility() const { return uml_visibility; }
    virtual UmlVisibility get_visibility(BrowserNode *);
    UmlVisibility get_cpp_visibility() const { return cpp_visibility; }
    void set_visibility(UmlVisibility v);
    
    const AType & get_type() const { return type; }
    void set_type(const AType & t);
    
    const char * get_init_value() const { return init_value; }

    const char * get_cppdecl() const { return cpp_decl; }
        
    const char * get_javadecl() const { return java_decl; }
        
    const char * get_phpdecl() const { return php_decl; }
    
    const char * get_pythondecl() const { return python_decl; }
    
    const char * get_idldecl() const { return idl_decl; }
    const char * get_idlcase() const;
    void set_idlcase(BrowserAttribute * a, const char * e);
    
    void edit(bool new_st_attr);
    
    virtual bool tool_cmd(ToolCom * com, const char * args,
			  BrowserNode * bn, const QString & comment);
    
    void save(QTextStream &, QString & warning) const;
    void read(char * & st, char * & k);
    
    void replace(BrowserClass * old, BrowserClass * nw);
    
  protected slots:
    void on_delete();
    void on_delete_idlcase();
};

#endif
