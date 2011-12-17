#ifdef WITHJAVA


#include "JavaSettings.h"

#include "UmlCom.h"
#include "UmlSettings.h"
#include "JavaSettingsCmd.h"
#include "UmlBuiltin.h"
#include "UmlStereotype.h"
bool JavaSettings::useDefaults()
{
  UmlCom::send_cmd(javaSettingsCmd, getJavaUseDefaultsCmd);
  return UmlCom::read_bool();
}

bool JavaSettings::set_UseDefaults(bool y)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaUseDefaultsCmd, (char) y);
  return UmlCom::read_bool();
}

QCString JavaSettings::type(const QCString & s)
{
  read_if_needed_();
  
  UmlBuiltin * b = UmlSettings::_map_builtins.find(s);
  
  return (b) ? b->java : s;
}

bool JavaSettings::set_Type(QCString s, QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaTypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlBuiltin * b = UmlSettings::_map_builtins.find(s);

    if (b == 0)
      b = UmlSettings::add_type(s);
    b->java = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QCString JavaSettings::umlType(const QCString & s)
{
  read_if_needed_();
  
  return UmlSettings::uml_type(s, &UmlBuiltin::java);
}

QCString JavaSettings::relationStereotype(const QCString & s)
{
  read_if_needed_();
  
  UmlStereotype * b = UmlSettings::_map_relation_stereotypes.find(s);
  
  return (b) ? b->java : s;
}

bool JavaSettings::set_RelationStereotype(QCString s, QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaRelationStereotypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlStereotype * st = UmlSettings::_map_relation_stereotypes.find(s);

    if (st == 0)
      st = UmlSettings::add_rel_stereotype(s);
    st->java = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QCString JavaSettings::relationUmlStereotype(const QCString & s)
{
  read_if_needed_();
  
  return UmlSettings::uml_rel_stereotype(s, &UmlStereotype::java);
}

QCString JavaSettings::classStereotype(const QCString & s)
{
  read_if_needed_();
  
  UmlStereotype * b = UmlSettings::_map_class_stereotypes.find(s);
  
  return (b) ? b->java : s;
}

bool JavaSettings::set_ClassStereotype(QCString s, QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaClassStereotypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlStereotype * st = UmlSettings::_map_class_stereotypes.find(s);

    if (st == 0)
      st = UmlSettings::add_class_stereotype(s);
    st->java = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QCString JavaSettings::classUmlStereotype(const QCString & s)
{
  read_if_needed_();
  
  return UmlSettings::uml_class_stereotype(s, &UmlStereotype::java);
}

QCString JavaSettings::get_import(const QCString & s)
{
  read_if_needed_();
  
  QCString * r = _map_imports[s];
  
  return (r) ? *r : QCString(0);
}

bool JavaSettings::set_Import(QCString s, QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaImportCmd, s, v);
  if (UmlCom::read_bool()) {
    QCString * r = _map_imports.take(s);
    
    if (!v.isEmpty())
      _map_imports.insert(s, new QCString(v));
    if (r)
      delete r;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & JavaSettings::rootDir()
{
  read_if_needed_();
  
  return _root;
}

bool JavaSettings::set_RootDir(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaRootdirCmd, v);
  if (UmlCom::read_bool()) {
    _root = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & JavaSettings::sourceContent()
{
  read_if_needed_();
  
  return _src_content;
}

bool JavaSettings::set_SourceContent(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaSourceContentCmd, v);
  if (UmlCom::read_bool()) {
    _src_content = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & JavaSettings::sourceExtension()
{
  read_if_needed_();
  
  return _ext; 
}

bool JavaSettings::set_SourceExtension(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaSourceExtensionCmd, v);
  if (UmlCom::read_bool()) {
    _ext = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & JavaSettings::classDecl()
{
  read_if_needed_();
  
  return _class_decl;
}

bool JavaSettings::set_ClassDecl(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaClassDeclCmd, v);
  if (UmlCom::read_bool()) {
    _class_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & JavaSettings::externalClassDecl()
{
  read_if_needed_();
  
  return _external_class_decl;
}

bool JavaSettings::set_ExternalClassDecl(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaExternalClassDeclCmd, v);
  if (UmlCom::read_bool()) {
    _external_class_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & JavaSettings::enumPatternDecl()
{
  read_if_needed_();
  
  return _enum_pattern_decl;
}

bool JavaSettings::set_EnumPatternDecl(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaEnumPatternDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_pattern_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

QCString JavaSettings::enumDecl()
{
  read_if_needed_();
  
  return _enum_decl;
}

bool JavaSettings::set_EnumDecl(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaEnumDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & JavaSettings::interfaceDecl()
{
  read_if_needed_();
  
  return _interface_decl;
}

bool JavaSettings::set_InterfaceDecl(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaInterfaceDeclCmd, v);
  if (UmlCom::read_bool()) {
    _interface_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & JavaSettings::attributeDecl()
{
  read_if_needed_();
  
  return _attr_decl;
}

bool JavaSettings::set_AttributeDecl(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaAttributeDeclCmd, v);
  if (UmlCom::read_bool()) {
    _attr_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & JavaSettings::enumPatternItemDecl()
{
  read_if_needed_();
  
  return _enum_pattern_item_decl;
}

bool JavaSettings::set_EnumPatternItemDecl(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaEnumPatternItemDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_pattern_item_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & JavaSettings::enumPatternItemCase()
{
  read_if_needed_();
  
  return _enum_pattern_item_case;
}

bool JavaSettings::set_EnumPatternItemCase(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaEnumPatternItemCaseCmd, v);
  if (UmlCom::read_bool()) {
    _enum_pattern_item_case = v;
    return TRUE;
  }
  else
    return FALSE;
}

QCString JavaSettings::enumItemDecl()
{
  read_if_needed_();
  
  return _enum_item_decl;
}

bool JavaSettings::set_EnumItemDecl(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaEnumItemDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_item_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & JavaSettings::relationDecl(const char * multiplicity)
{
  read_if_needed_();
  
  return _rel_decl[UmlSettings::multiplicity_column(multiplicity)];
}

bool JavaSettings::set_RelationDecl(const char * multiplicity, QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaRelationDeclCmd, multiplicity, v);
  if (UmlCom::read_bool()) {
    _rel_decl[UmlSettings::multiplicity_column(multiplicity)] = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & JavaSettings::operationDef()
{
  read_if_needed_();
  
  return _oper_def;
}

bool JavaSettings::set_OperationDef(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaOperationDefCmd, v);
  if (UmlCom::read_bool()) {
    _oper_def = v;
    return TRUE;
  }
  else
    return FALSE;
}

aVisibility JavaSettings::getVisibility()
{
  read_if_needed_();
  
  return _get_visibility;
}

bool JavaSettings::set_GetVisibility(aVisibility v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaGetvisibilityCmd, v);
  if (UmlCom::read_bool()) {
    _get_visibility = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & JavaSettings::getName()
{
  read_if_needed_();
  
  return _get_name;
}

bool JavaSettings::set_GetName(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaGetNameCmd, v);
  if (UmlCom::read_bool()) {
    _get_name = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & JavaSettings::setName()
{
  read_if_needed_();
  
  return _set_name;
}

bool JavaSettings::set_SetName(QCString v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaSetNameCmd, v);
  if (UmlCom::read_bool()) {
    _set_name = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool JavaSettings::isGetFinal()
{
  read_if_needed_();
  
  return _is_get_final;
}

bool JavaSettings::set_IsGetFinal(bool v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaIsGetFinalCmd, v);
  if (UmlCom::read_bool()) {
    _is_get_final = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool JavaSettings::isSetFinal()
{
  read_if_needed_();
  
  return _is_set_final;
}

bool JavaSettings::set_IsSetFinal(bool v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaIsSetFinalCmd, v);
  if (UmlCom::read_bool()) {
    _is_set_final = v;
    return TRUE;
  }
  else
    return FALSE;
}

aVisibility JavaSettings::setVisibility()
{
  read_if_needed_();
  
  return _set_visibility;
}

bool JavaSettings::set_SetVisibility(aVisibility v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaSetVisibilityCmd, v);
  if (UmlCom::read_bool()) {
    _set_visibility = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool JavaSettings::isSetParamFinal()
{
  read_if_needed_();
  
  return _is_set_param_final;
}

bool JavaSettings::set_IsSetParamFinal(bool v)
{
  UmlCom::send_cmd(javaSettingsCmd, setJavaIsSetParamFinalCmd, v);
  if (UmlCom::read_bool()) {
    _is_set_param_final = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool JavaSettings::_defined;

QCString JavaSettings::_root;

QCString JavaSettings::_class_decl;

QCString JavaSettings::_external_class_decl;

QCString JavaSettings::_enum_pattern_decl;

QCString JavaSettings::_enum_decl;

QCString JavaSettings::_interface_decl;

QCString JavaSettings::_attr_decl;

QCString JavaSettings::_enum_pattern_item_decl;

QCString JavaSettings::_enum_pattern_item_case;

QCString JavaSettings::_enum_item_decl;

QCString JavaSettings::_rel_decl[3/*multiplicity*/];

QCString JavaSettings::_oper_def;

aVisibility JavaSettings::_get_visibility;

QCString JavaSettings::_get_name;

bool JavaSettings::_is_get_final;

aVisibility JavaSettings::_set_visibility;

QCString JavaSettings::_set_name;

bool JavaSettings::_is_set_final;

bool JavaSettings::_is_set_param_final;

QCString JavaSettings::_src_content;

QCString JavaSettings::_ext;

QDict<QCString> JavaSettings::_map_imports;

void JavaSettings::read_()
{
  _root = UmlCom::read_string();
  
  unsigned n;
  unsigned index;
  
  n = UmlCom::read_unsigned();
  
  for (index = 0; index != n; index += 1) {
    UmlSettings::_builtins[index].java = UmlCom::read_string();
  }
  
  n = UmlCom::read_unsigned();
  
  for (index = 0; index != n; index += 1)
    UmlSettings::_relation_stereotypes[index].java = UmlCom::read_string();
  
  n = UmlCom::read_unsigned();
  
  for (index = 0; index != n; index += 1)
    UmlSettings::_class_stereotypes[index].java = UmlCom::read_string();
  
  n = UmlCom::read_unsigned();
  _map_imports.clear();
  if (n > _map_imports.size())
    _map_imports.resize(n);
  
  for (index = 0; index != n; index += 1) {
    QCString t = UmlCom::read_string();
    QCString i = UmlCom::read_string();
    
    _map_imports.insert(t, new QCString(i));
  }
    
  _src_content = UmlCom::read_string();
  _ext = UmlCom::read_string();

  _class_decl = UmlCom::read_string();
  _external_class_decl = UmlCom::read_string();
  _enum_decl = UmlCom::read_string();
  _enum_pattern_decl = UmlCom::read_string();
  _interface_decl = UmlCom::read_string();
  _attr_decl = UmlCom::read_string();
  _enum_item_decl = UmlCom::read_string();
  _enum_pattern_item_decl = UmlCom::read_string();
  _enum_pattern_item_case = UmlCom::read_string();
  for (index = 0; index != 3; index += 1)
    _rel_decl[index] = UmlCom::read_string();
  _oper_def = UmlCom::read_string();
  _get_visibility = (aVisibility) UmlCom::read_char();
  _get_name = UmlCom::read_string();
  _is_get_final = UmlCom::read_bool();
  _set_visibility = (aVisibility) UmlCom::read_char();
  _set_name = UmlCom::read_string();
  _is_set_final = UmlCom::read_bool();
  _is_set_param_final = UmlCom::read_bool();
}

void JavaSettings::read_if_needed_()
{
  UmlSettings::read_if_needed_();
  if (!_defined) {
    UmlCom::send_cmd(javaSettingsCmd, getJavaSettingsCmd);
    read_();
    _defined = TRUE;
  }
}


#endif
