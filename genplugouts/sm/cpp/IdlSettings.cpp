#ifdef WITHIDL


#include "IdlSettings.h"

#include "UmlCom.h"
#include "UmlSettings.h"
#include "IdlSettingsCmd.h"
#include "UmlBuiltin.h"
#include "UmlStereotype.h"
bool IdlSettings::useDefaults()
{
  UmlCom::send_cmd(idlSettingsCmd, getIdlUseDefaultsCmd);
  return UmlCom::read_bool();
}

bool IdlSettings::set_UseDefaults(bool y)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlUseDefaultsCmd, (char) y);
  return UmlCom::read_bool();
}

QCString IdlSettings::type(QCString s)
{
  read_if_needed_();
  
  UmlBuiltin * b = UmlSettings::_map_builtins.find(s);
  
  return (b) ? b->idl : s;
}

bool IdlSettings::set_Type(QCString s, QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlTypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlBuiltin * b = UmlSettings::_map_builtins.find(s);

    if (b == 0)
      b = UmlSettings::add_type(s);
    b->idl = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QCString IdlSettings::umlType(QCString s)
{
  read_if_needed_();
  
  return UmlSettings::uml_type(s, &UmlBuiltin::idl);
}

QCString IdlSettings::relationAttributeStereotype(const QCString & s)
{
  read_if_needed_();
  
  UmlStereotype * b = UmlSettings::_map_relation_attribute_stereotypes.find(s);
  
  return (b) ? b->idl : s;
}

bool IdlSettings::set_RelationAttributeStereotype(QCString s, QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlRelationAttributeStereotypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlStereotype * st = UmlSettings::_map_relation_attribute_stereotypes.find(s);

    if (st == 0)
      st = UmlSettings::add_rel_attr_stereotype(s);
    st->idl = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QCString IdlSettings::relationAttributeUmlStereotype(QCString s)
{
  read_if_needed_();
  
  return UmlSettings::uml_rel_attr_stereotype(s, &UmlStereotype::idl);
}

QCString IdlSettings::classStereotype(QCString s)
{
  read_if_needed_();
  
  UmlStereotype * b = UmlSettings::_map_class_stereotypes.find(s);
  
  return (b) ? b->idl : s;
}

bool IdlSettings::set_ClassStereotype(QCString s, QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlClassStereotypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlStereotype * st = UmlSettings::_map_class_stereotypes.find(s);

    if (st == 0)
      st = UmlSettings::add_class_stereotype(s);
    st->idl = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QCString IdlSettings::classUmlStereotype(QCString s)
{
  read_if_needed_();
  
  return UmlSettings::uml_class_stereotype(s, &UmlStereotype::idl);
}

QCString IdlSettings::include(QCString s)
{
  read_if_needed_();
  
  QCString * r = _map_includes[s];
  
  return (r) ? *r : QCString(0);
}

bool IdlSettings::set_Include(QCString s, QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlIncludeCmd, s, v);
  if (UmlCom::read_bool()) {
    QCString * r = _map_includes.take(s);
    
    if (!v.isEmpty())
      _map_includes.insert(s, new QCString(v));
    if (r)
      delete r;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::rootDir()
{
  read_if_needed_();
  
  return _root;
}

bool IdlSettings::set_RootDir(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlRootDirCmd, v);
  if (UmlCom::read_bool()) {
    _root = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::sourceContent()
{
  read_if_needed_();
  
  return _src_content;
}

bool IdlSettings::set_SourceContent(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlSourceContentCmd, v);
  if (UmlCom::read_bool()) {
    _src_content = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::sourceExtension()
{
  read_if_needed_();
  
  return _ext; 
}

bool IdlSettings::set_SourceExtension(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlSourceExtensionCmd, v);
  if (UmlCom::read_bool()) {
    _ext = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::interfaceDecl()
{
  read_if_needed_();
  
  return _interface_decl;
}

bool IdlSettings::set_InterfaceDecl(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlInterfaceDeclCmd, v);
  if (UmlCom::read_bool()) {
    _interface_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::valuetypeDecl()
{
  read_if_needed_();
  
  return _valuetype_decl;
}

bool IdlSettings::set_ValuetypeDecl(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlValuetypeDeclCmd, v);
  if (UmlCom::read_bool()) {
    _valuetype_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::externalClassDecl()
{
  read_if_needed_();
  
  return _external_class_decl;
}

bool IdlSettings::set_ExternalClassDecl(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlExternalClassDeclCmd, v);
  if (UmlCom::read_bool()) {
    _external_class_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::structDecl()
{
  read_if_needed_();
  
  return _struct_decl;
}

bool IdlSettings::set_StructDecl(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlStructDeclCmd, v);
  if (UmlCom::read_bool()) {
    _struct_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::typedefDecl()
{
  read_if_needed_();
  
  return _typedef_decl;
}

bool IdlSettings::set_TypedefDecl(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlTypedefDeclCmd, v);
  if (UmlCom::read_bool()) {
    _typedef_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::exceptionDecl()
{
  read_if_needed_();
  
  return _exception_decl;
}

bool IdlSettings::set_ExceptionDecl(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlExceptionDeclCmd, v);
  if (UmlCom::read_bool()) {
    _exception_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::unionDecl()
{
  read_if_needed_();
  
  return _union_decl;
}

bool IdlSettings::set_UnionDecl(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlUnionDeclCmd, v);
  if (UmlCom::read_bool()) {
    _union_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::enumDecl()
{
  read_if_needed_();
  
  return _enum_decl;
}

bool IdlSettings::set_EnumDecl(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlEnumDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::attributeDecl(const char * multiplicity)
{
  read_if_needed_();

  return _attr_decl[UmlSettings::multiplicity_column(multiplicity)];
}

bool IdlSettings::set_AttributeDecl(const char * multiplicity, QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlAttributeDeclCmd, multiplicity, v);
  if (UmlCom::read_bool()) {
    _attr_decl[UmlSettings::multiplicity_column(multiplicity)] = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::valuetypeAttributeDecl(const char * multiplicity)
{
  read_if_needed_();

  return _valuetype_attr_decl[UmlSettings::multiplicity_column(multiplicity)];
}

bool IdlSettings::set_ValuetypeAttributeDecl(const char * multiplicity, QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlValuetypeAttributeDeclCmd, multiplicity, v);
  if (UmlCom::read_bool()) {
    _valuetype_attr_decl[UmlSettings::multiplicity_column(multiplicity)] = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::unionItemDecl(const char * multiplicity)
{
  read_if_needed_();

  return _union_item_decl[UmlSettings::multiplicity_column(multiplicity)];
}

bool IdlSettings::set_UnionItemDecl(const char * multiplicity, QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlUnionItemDeclCmd, multiplicity, v);
  if (UmlCom::read_bool()) {
    _union_item_decl[UmlSettings::multiplicity_column(multiplicity)] = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::enumItemDecl()
{
  read_if_needed_();
  
  return _enum_item_decl;
}

bool IdlSettings::set_EnumItemDecl(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlEnumItemDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_item_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::constDecl(const char * multiplicity)
{
  read_if_needed_();

  return _const_decl[UmlSettings::multiplicity_column(multiplicity)];
}

bool IdlSettings::set_ConstDecl(const char * multiplicity, QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlConstDeclCmd, multiplicity, v);
  if (UmlCom::read_bool()) {
    _const_decl[UmlSettings::multiplicity_column(multiplicity)] = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::relationDecl(const char * multiplicity)
{
  read_if_needed_();
  
  return _rel_decl[UmlSettings::multiplicity_column(multiplicity)];
}

bool IdlSettings::set_RelationDecl(const char * multiplicity, QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlRelationDeclCmd, multiplicity, v);
  if (UmlCom::read_bool()) {
    _rel_decl[UmlSettings::multiplicity_column(multiplicity)] = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::valuetypeRelationDecl(const char * multiplicity)
{
  read_if_needed_();
  
  return _valuetype_rel_decl[UmlSettings::multiplicity_column(multiplicity)];
}

bool IdlSettings::set_ValuetypeRelationDecl(const char * multiplicity, QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlValuetypeRelationDeclCmd, multiplicity, v);
  if (UmlCom::read_bool()) {
    _valuetype_rel_decl[UmlSettings::multiplicity_column(multiplicity)] = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::unionRelationDecl(const char * multiplicity)
{
  read_if_needed_();
  
  return _union_rel_decl[UmlSettings::multiplicity_column(multiplicity)];
}

bool IdlSettings::set_UnionRelationDecl(const char * multiplicity, QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlUnionRelationDeclCmd, multiplicity, v);
  if (UmlCom::read_bool()) {
    _union_rel_decl[UmlSettings::multiplicity_column(multiplicity)] = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::operationDecl()
{
  read_if_needed_();
  
  return _oper_decl;
}

bool IdlSettings::set_OperationDecl(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlOperationDeclCmd, v);
  if (UmlCom::read_bool()) {
    _oper_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::getName()
{
  read_if_needed_();
  
  return _get_name;
}

bool IdlSettings::set_GetName(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlGetNameCmd, v);
  if (UmlCom::read_bool()) {
    _get_name = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & IdlSettings::setName()
{
  read_if_needed_();
  
  return _set_name;
}

bool IdlSettings::set_SetName(QCString v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlSetNameCmd, v);
  if (UmlCom::read_bool()) {
    _set_name = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool IdlSettings::isSetOneway()
{
  read_if_needed_();
  
  return _is_set_oneway;
}

bool IdlSettings::set_IsSetOneway(bool v)
{
  UmlCom::send_cmd(idlSettingsCmd, setIdlIsSetOneWayCmd, v);
  if (UmlCom::read_bool()) {
    _is_set_oneway = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool IdlSettings::_defined;

QCString IdlSettings::_root;

QCString IdlSettings::_interface_decl;

QCString IdlSettings::_valuetype_decl;

QCString IdlSettings::_struct_decl;

QCString IdlSettings::_typedef_decl;

QCString IdlSettings::_exception_decl;

QCString IdlSettings::_union_decl;

QCString IdlSettings::_enum_decl;

QCString IdlSettings::_external_class_decl;

QCString IdlSettings::_attr_decl[3/*multiplicity*/];

QCString IdlSettings::_valuetype_attr_decl[3/*multiplicity*/];

QCString IdlSettings::_union_item_decl[3/*multiplicity*/];

QCString IdlSettings::_enum_item_decl;

QCString IdlSettings::_const_decl[3/*multiplicity*/];

QCString IdlSettings::_rel_decl[3/*multiplicity*/];

QCString IdlSettings::_valuetype_rel_decl[3/*multiplicity*/];

QCString IdlSettings::_union_rel_decl[3/*multiplicity*/];

QCString IdlSettings::_oper_decl;

QCString IdlSettings::_get_name;

QCString IdlSettings::_set_name;

bool IdlSettings::_is_set_oneway;

QCString IdlSettings::_src_content;

QCString IdlSettings::_ext;

QDict<QCString> IdlSettings::_map_includes;

void IdlSettings::read_()
{
  _root = UmlCom::read_string();
  
  unsigned n;
  unsigned index;
  
  n = UmlCom::read_unsigned();
  
  for (index = 0; index != n; index += 1) {
    UmlSettings::_builtins[index].idl = UmlCom::read_string();
  }
  
  n = UmlCom::read_unsigned();
  
  for (index = 0; index != n; index += 1)
    UmlSettings::_relation_attribute_stereotypes[index].idl = UmlCom::read_string();
  
  n = UmlCom::read_unsigned();
  
  for (index = 0; index != n; index += 1)
    UmlSettings::_class_stereotypes[index].idl = UmlCom::read_string();
  
  n = UmlCom::read_unsigned();
  _map_includes.clear();
  if (n > _map_includes.size())
    _map_includes.resize(n);
  
  for (index = 0; index != n; index += 1) {
    QCString t = UmlCom::read_string();
    QCString i = UmlCom::read_string();
    
    _map_includes.insert(t, new QCString(i));
  }
  
  _src_content = UmlCom::read_string();
  _ext = UmlCom::read_string();

  _interface_decl = UmlCom::read_string();
  _valuetype_decl = UmlCom::read_string();
  _struct_decl = UmlCom::read_string();
  _typedef_decl = UmlCom::read_string();
  _exception_decl = UmlCom::read_string();
  _union_decl = UmlCom::read_string();
  _enum_decl = UmlCom::read_string();
  _external_class_decl = UmlCom::read_string();
  for (index = 0; index != 3; index += 1)
    _attr_decl[index] = UmlCom::read_string();
  for (index = 0; index != 3; index += 1)
    _valuetype_attr_decl[index] = UmlCom::read_string();
  for (index = 0; index != 3; index += 1)
    _union_item_decl[index] = UmlCom::read_string();
  _enum_item_decl = UmlCom::read_string();
  for (index = 0; index != 3; index += 1)
    _const_decl[index] = UmlCom::read_string();
  for (index = 0; index != 3; index += 1) {
    _rel_decl[index] = UmlCom::read_string();
    _valuetype_rel_decl[index] = UmlCom::read_string();
    _union_rel_decl[index] = UmlCom::read_string();
  }
  _oper_decl = UmlCom::read_string();
  _get_name = UmlCom::read_string();
  _set_name = UmlCom::read_string();
  _is_set_oneway = UmlCom::read_bool();
}

void IdlSettings::read_if_needed_()
{
  UmlSettings::read_if_needed_();
  if (!_defined) {
    UmlCom::send_cmd(idlSettingsCmd, getIdlSettingsCmd);
    read_();
    _defined = TRUE;
  }
}


#endif
