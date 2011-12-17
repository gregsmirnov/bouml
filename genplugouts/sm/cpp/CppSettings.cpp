#ifdef WITHCPP


#include "CppSettings.h"

#include "UmlCom.h"
#include "UmlSettings.h"
#include "CppSettingsCmd.h"
#include "UmlBuiltin.h"
#include "UmlStereotype.h"
bool CppSettings::useDefaults()
{
  UmlCom::send_cmd(cppSettingsCmd, getCppUseDefaultsCmd);
  return UmlCom::read_bool();
}

bool CppSettings::set_UseDefaults(bool y)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppUseDefaultsCmd, (char) y);
  return UmlCom::read_bool();
}

QCString CppSettings::type(QCString s)
{
  read_if_needed_();
  
  UmlBuiltin * b = UmlSettings::_map_builtins.find(s);
  
  return (b) ? b->cpp : s;
}

bool CppSettings::set_Type(QCString s, QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppTypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlBuiltin * b = UmlSettings::_map_builtins.find(s);

    if (b == 0)
      b = UmlSettings::add_type(s);
    b->cpp = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QCString CppSettings::umlType(QCString s)
{
  read_if_needed_();
  
  return UmlSettings::uml_type(s, &UmlBuiltin::cpp);
}

QCString CppSettings::relationAttributeStereotype(QCString s)
{
  read_if_needed_();
  
  UmlStereotype * b = UmlSettings::_map_relation_attribute_stereotypes.find(s);
  
  return (b) ? b->cpp : s;
}

bool CppSettings::set_RelationAttributeStereotype(QCString s, QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppRelationAttributeStereotypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlStereotype * st = UmlSettings::_map_relation_attribute_stereotypes.find(s);

    if (st == 0)
      st = UmlSettings::add_rel_attr_stereotype(s);
    st->cpp = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QCString CppSettings::relationAttributeUmlStereotype(QCString s)
{
  read_if_needed_();
  
  return UmlSettings::uml_rel_attr_stereotype(s, &UmlStereotype::cpp);
}

QCString CppSettings::classStereotype(QCString s)
{
  read_if_needed_();
  
  UmlStereotype * b = UmlSettings::_map_class_stereotypes.find(s);
  
  return (b) ? b->cpp : s;
}

bool CppSettings::set_ClassStereotype(QCString s, QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppClassStereotypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlStereotype * st = UmlSettings::_map_class_stereotypes.find(s);

    if (st == 0)
      st = UmlSettings::add_class_stereotype(s);
    st->cpp = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QCString CppSettings::classUmlStereotype(QCString s)
{
  read_if_needed_();
  
  return UmlSettings::uml_class_stereotype(s, &UmlStereotype::cpp);
}

QCString CppSettings::include(QCString s)
{
  read_if_needed_();
  
  QCString * r = _map_includes[s];
  
  return (r) ? *r : QCString(0);
}

bool CppSettings::set_Include(QCString s, QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppIncludeCmd, s, v);
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

QCString CppSettings::rootDir()
{
  read_if_needed_();
  
  return _root;
}

bool CppSettings::set_RootDir(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppRootDirCmd, v);
  if (UmlCom::read_bool()) {
    _root = v;
    return TRUE;
  }
  else
    return FALSE;
}

QCString CppSettings::headerContent()
{
  read_if_needed_();
  
  return _h_content; 
}

bool CppSettings::set_HeaderContent(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppHeaderContentCmd, v);
  if (UmlCom::read_bool()) {
    _h_content = v;
    return TRUE;
  }
  else
    return FALSE;
}

QCString CppSettings::sourceContent()
{
  read_if_needed_();
  
  return _src_content;
}

bool CppSettings::set_SourceContent(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppSourceContentCmd, v);
  if (UmlCom::read_bool()) {
    _src_content = v;
    return TRUE;
  }
  else
    return FALSE;
}

QCString CppSettings::headerExtension()
{
  read_if_needed_();
  
  return _h_ext; 
}

bool CppSettings::set_HeaderExtension(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppHeaderExtensionCmd, v);
  if (UmlCom::read_bool()) {
    _h_ext = v;
    return TRUE;
  }
  else
    return FALSE;
}

QCString CppSettings::sourceExtension()
{
  read_if_needed_();
  
  return _src_ext;
}

bool CppSettings::set_SourceExtension(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppSourceExtensionCmd, v);
  if (UmlCom::read_bool()) {
    _src_ext = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool CppSettings::includeWithPath()
{
  read_if_needed_();
  
  return _incl_with_path;
}

bool CppSettings::set_IncludeWithPath(bool v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppIncludeWithPathCmd, v);
  if (UmlCom::read_bool()) {
    _incl_with_path = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool CppSettings::isRelativePath()
{
  read_if_needed_();

  return _is_relative_path;
}

bool CppSettings::set_IsRelativePath(bool v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppRelativePathCmd, v);
  if (UmlCom::read_bool()) {
    _is_relative_path = v;
    if (v) _is_root_relative_path = FALSE;
    return TRUE;
  }
  else
    return FALSE;
}

bool CppSettings::isRootRelativePath()
{
  read_if_needed_();

  return _is_root_relative_path;
}

bool CppSettings::set_IsRootRelativePath(bool v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppRootRelativePathCmd, v);
  if (UmlCom::read_bool()) {
    _is_root_relative_path = v;
    if (v) _is_relative_path = FALSE;
    return TRUE;
  }
  else
    return FALSE;
}

bool CppSettings::isForceNamespacePrefixGeneration()
{
  read_if_needed_();

  return _is_force_namespace_gen;
}

bool CppSettings::set_IsForceNamespacePrefixGeneration(bool v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppForceNamespaceGenCmd, v);
  if (UmlCom::read_bool()) {
    _is_force_namespace_gen = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool CppSettings::isGenerateJavadocStyleComment()
{
  read_if_needed_();

  return _is_generate_javadoc_comment;
}

bool CppSettings::set_IsGenerateJavadocStyleComment(bool v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppJavadocStyleCmd, v);
  if (UmlCom::read_bool()) {
    _is_generate_javadoc_comment = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::enumIn()
{
  read_if_needed_();
  
  return _enum_in;
}

bool CppSettings::set_EnumIn(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppEnumInCmd, v);
  if (UmlCom::read_bool()) {
    _enum_in = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::enumOut()
{
  read_if_needed_();
  
  return _enum_out;
}

bool CppSettings::set_EnumOut(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppEnumOutCmd, v);
  if (UmlCom::read_bool()) {
    _enum_out = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::enumInout()
{
  read_if_needed_();
  
  return _enum_inout;
}

bool CppSettings::set_EnumInout(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppEnumInOutCmd, v);
  if (UmlCom::read_bool()) {
    _enum_inout = v;
    return TRUE;
  }
  else
    return FALSE;
}

QCString CppSettings::enumReturn()
{
  read_if_needed_();

  return _enum_return;
}

bool CppSettings::set_EnumReturn(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppEnumReturnCmd, v);
  if (UmlCom::read_bool()) {
    _enum_return = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::in()
{
  read_if_needed_();
  
  return _in;
}

bool CppSettings::set_In(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppInCmd, v);
  if (UmlCom::read_bool()) {
    _in = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::out()
{
  read_if_needed_();
  
  return _out;
}

bool CppSettings::set_Out(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppOutCmd, v);
  if (UmlCom::read_bool()) {
    _out = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::inout()
{
  read_if_needed_();
  
  return _inout;
}

bool CppSettings::set_Inout(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppInOutCmd, v);
  if (UmlCom::read_bool()) {
    _inout = v;
    return TRUE;
  }
  else
    return FALSE;
}

QCString CppSettings::Return()
{
  read_if_needed_();

  return _return;
}

bool CppSettings::set_Return(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppReturnCmd, v);
  if (UmlCom::read_bool()) {
    _return = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::classDecl()
{
  read_if_needed_();
  
  return _class_decl;
}

bool CppSettings::set_ClassDecl(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppClassDeclCmd, v);
  if (UmlCom::read_bool()) {
    _class_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::externalClassDecl()
{
  read_if_needed_();
  
  return _external_class_decl;
}

bool CppSettings::set_ExternalClassDecl(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppExternalClassDeclCmd, v);
  if (UmlCom::read_bool()) {
    _external_class_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::structDecl()
{
  read_if_needed_();
  
  return _struct_decl;
}

bool CppSettings::set_StructDecl(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppStructDeclCmd, v);
  if (UmlCom::read_bool()) {
    _struct_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::unionDecl()
{
  read_if_needed_();
  
  return _union_decl;
}

bool CppSettings::set_UnionDecl(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppUnionDeclCmd, v);
  if (UmlCom::read_bool()) {
    _union_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::enumDecl()
{
  read_if_needed_();
  
  return _enum_decl;
}

bool CppSettings::set_EnumDecl(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppEnumDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::typedefDecl()
{
  read_if_needed_();
  
  return _typedef_decl;
}

bool CppSettings::set_TypedefDecl(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppTypedefDeclCmd, v);
  if (UmlCom::read_bool()) {
    _typedef_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::attributeDecl(const char * multiplicity)
{
  read_if_needed_();

  return _attr_decl[UmlSettings::multiplicity_column(multiplicity)];
}

bool CppSettings::set_AttributeDecl(const char * multiplicity, QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppAttributeDeclCmd, multiplicity, v);
  if (UmlCom::read_bool()) {
    _attr_decl[UmlSettings::multiplicity_column(multiplicity)] = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::enumItemDecl()
{
  read_if_needed_();
  
  return _enum_item_decl;
}

bool CppSettings::set_EnumItemDecl(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppEnumItemDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_item_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::relationDecl(bool by_value, const char * multiplicity)
{
  read_if_needed_();
  
  return _rel_decl[(by_value) ? 1 : 0][UmlSettings::multiplicity_column(multiplicity)];
}

bool CppSettings::set_RelationDecl(bool by_value, const char * multiplicity, const char * v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppRelationDeclCmd, by_value, multiplicity, v);
  if (UmlCom::read_bool()) {
    _rel_decl[(by_value) ? 1 : 0][UmlSettings::multiplicity_column(multiplicity)] = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::operationDecl()
{
  read_if_needed_();
  
  return _oper_decl;
}

bool CppSettings::set_OperationDecl(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppOperationDeclCmd, v);
  if (UmlCom::read_bool()) {
    _oper_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::operationDef()
{
  read_if_needed_();
  
  return _oper_def;
}

bool CppSettings::set_OperationDef(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppOperationDefCmd, v);
  if (UmlCom::read_bool()) {
    _oper_def = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool CppSettings::operationForceThrow()
{
  read_if_needed_();
  
  return _force_oper_throw;
}

bool CppSettings::set_OperationForceThrow(bool v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppOperationForceThrowCmd, v);
  if (UmlCom::read_bool()) {
    _force_oper_throw = v;
    return TRUE;
  }
  else
    return FALSE;
}

aVisibility CppSettings::getVisibility()
{
  read_if_needed_();
  
  return _get_visibility;
}

bool CppSettings::set_GetVisibility(aVisibility v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppGetVisibilityCmd, v);
  if (UmlCom::read_bool()) {
    _get_visibility = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::getName()
{
  read_if_needed_();
  
  return _get_name;
}

bool CppSettings::set_GetName(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppGetNameCmd, v);
  if (UmlCom::read_bool()) {
    _get_name = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool CppSettings::isGetInline()
{
  read_if_needed_();
  
  return _is_get_inline;
}

bool CppSettings::set_IsGetInline(bool v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppIsGetInlineCmd, v);
  if (UmlCom::read_bool()) {
    _is_get_inline = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool CppSettings::isGetConst()
{
  read_if_needed_();
  
  return _is_get_const;
}

bool CppSettings::set_IsGetConst(bool v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppIsGetConstCmd, v);
  if (UmlCom::read_bool()) {
    _is_get_const = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool CppSettings::isGetValueConst()
{
  read_if_needed_();
  
  return _is_get_value_const;
}

bool CppSettings::set_IsGetValueConst(bool v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppIsGetValueConstCmd, v);
  if (UmlCom::read_bool()) {
    _is_get_value_const = v;
    return TRUE;
  }
  else
    return FALSE;
}

aVisibility CppSettings::setVisibility()
{
  read_if_needed_();
  
  return _set_visibility;
}

bool CppSettings::set_SetVisibility(aVisibility v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppSetVisibilityCmd, v);
  if (UmlCom::read_bool()) {
    _set_visibility = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & CppSettings::setName()
{
  read_if_needed_();
  
  return _set_name;
}

bool CppSettings::set_SetName(QCString v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppSetNameCmd, v);
  if (UmlCom::read_bool()) {
    _set_name = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool CppSettings::isSetInline()
{
  read_if_needed_();
  
  return _is_set_inline;
}

bool CppSettings::set_IsSetInline(bool v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppIsSetInlineCmd, v);
  if (UmlCom::read_bool()) {
    _is_set_inline = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool CppSettings::isSetParamConst()
{
  read_if_needed_();
  
  return _is_set_param_const;
}

bool CppSettings::set_IsSetParamConst(bool v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppIsSetParamConstCmd, v);
  if (UmlCom::read_bool()) {
    _is_set_param_const = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool CppSettings::isSetParamRef()
{
  read_if_needed_();

  return _is_set_param_ref;
}

bool CppSettings::set_IsSetParamRef(bool v)
{
  UmlCom::send_cmd(cppSettingsCmd, setCppIsSetParamRefCmd, v);
  if (UmlCom::read_bool()) {
    _is_set_param_ref = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool CppSettings::_defined;

QCString CppSettings::_root;

QCString CppSettings::_in;

QCString CppSettings::_out;

QCString CppSettings::_inout;

QCString CppSettings::_return;

QCString CppSettings::_enum_in;

QCString CppSettings::_enum_out;

QCString CppSettings::_enum_inout;

QCString CppSettings::_enum_return;

QCString CppSettings::_class_decl;

QCString CppSettings::_external_class_decl;

QCString CppSettings::_struct_decl;

QCString CppSettings::_union_decl;

QCString CppSettings::_enum_decl;

QCString CppSettings::_typedef_decl;

QCString CppSettings::_attr_decl[3/*multiplicity*/];

QCString CppSettings::_enum_item_decl;

QCString CppSettings::_rel_decl[2/*relation kind*/][3/*multiplicity*/];

QCString CppSettings::_oper_decl;

QCString CppSettings::_oper_def;

bool CppSettings::_force_oper_throw;

aVisibility CppSettings::_get_visibility;

QCString CppSettings::_get_name;

bool CppSettings::_is_get_inline;

bool CppSettings::_is_get_const;

bool CppSettings::_is_get_value_const;

aVisibility CppSettings::_set_visibility;

QCString CppSettings::_set_name;

bool CppSettings::_is_set_inline;

bool CppSettings::_is_set_param_const;

bool CppSettings::_is_set_param_ref;

QCString CppSettings::_h_content;

QCString CppSettings::_src_content;

QCString CppSettings::_h_ext;

QCString CppSettings::_src_ext;

bool CppSettings::_incl_with_path;

bool CppSettings::_is_relative_path;

bool CppSettings::_is_root_relative_path;

bool CppSettings::_is_force_namespace_gen;

bool CppSettings::_is_generate_javadoc_comment;

QDict<QCString> CppSettings::_map_includes;

void CppSettings::read_()
{
  _root = UmlCom::read_string();
  
  unsigned n;
  unsigned index;
  
  n = UmlCom::read_unsigned();
  
  for (index = 0; index != n; index += 1) {
    UmlSettings::_builtins[index].cpp = UmlCom::read_string();
    UmlSettings::_builtins[index].cpp_in = UmlCom::read_string();
    UmlSettings::_builtins[index].cpp_out = UmlCom::read_string();
    UmlSettings::_builtins[index].cpp_inout = UmlCom::read_string();
    UmlSettings::_builtins[index].cpp_return = UmlCom::read_string();
  }
  
  n = UmlCom::read_unsigned();
  
  for (index = 0; index != n; index += 1)
    UmlSettings::_relation_attribute_stereotypes[index].cpp = UmlCom::read_string();
  
  n = UmlCom::read_unsigned();
  
  for (index = 0; index != n; index += 1)
    UmlSettings::_class_stereotypes[index].cpp = UmlCom::read_string();
  
  n = UmlCom::read_unsigned();
  _map_includes.clear();
  if (n > _map_includes.size())
    _map_includes.resize(n);
  
  for (index = 0; index != n; index += 1) {
    QCString t = UmlCom::read_string();
    QCString i = UmlCom::read_string();
    
    _map_includes.insert(t, new QCString(i));
  }
  
  _h_content = UmlCom::read_string();
  _src_content = UmlCom::read_string();
  _h_ext = UmlCom::read_string();
  _src_ext = UmlCom::read_string();
  _incl_with_path = UmlCom::read_bool();

  _in = UmlCom::read_string();
  _out = UmlCom::read_string();
  _inout = UmlCom::read_string();
  _return = UmlCom::read_string();
  _enum_in = UmlCom::read_string();
  _enum_out = UmlCom::read_string();
  _enum_inout = UmlCom::read_string();
  _enum_return = UmlCom::read_string();
  _class_decl = UmlCom::read_string();
  _external_class_decl = UmlCom::read_string();
  _struct_decl = UmlCom::read_string();
  _union_decl = UmlCom::read_string();
  _enum_decl = UmlCom::read_string();
  _typedef_decl = UmlCom::read_string();
  for (index = 0; index != 3; index += 1)
    _attr_decl[index] = UmlCom::read_string();
  _enum_item_decl = UmlCom::read_string();
  for (index = 0; index != 3; index += 1) {
    _rel_decl[0][index] = UmlCom::read_string();
    _rel_decl[1][index] = UmlCom::read_string();
  }
  _oper_decl = UmlCom::read_string();
  _oper_def = UmlCom::read_string();
  _force_oper_throw = UmlCom::read_bool();
  _get_visibility = (aVisibility) UmlCom::read_char();
  _get_name = UmlCom::read_string();
  _is_get_inline = UmlCom::read_bool();
  _is_get_const = UmlCom::read_bool();
  _is_get_value_const = UmlCom::read_bool();
  _set_visibility = (aVisibility) UmlCom::read_char();
  _set_name = UmlCom::read_string();
  _is_set_inline = UmlCom::read_bool();
  _is_set_param_const = UmlCom::read_bool();

  _is_set_param_ref = UmlCom::read_bool();
  _is_relative_path = UmlCom::read_bool();
  _is_force_namespace_gen = UmlCom::read_bool();
  _is_root_relative_path = UmlCom::read_bool();
  _is_generate_javadoc_comment = UmlCom::read_bool();
}

void CppSettings::read_if_needed_()
{
  UmlSettings::read_if_needed_();
  if (!_defined) {
    UmlCom::send_cmd(cppSettingsCmd, getCppSettingsCmd);
    read_();
    _defined = TRUE;
  }
}


#endif
