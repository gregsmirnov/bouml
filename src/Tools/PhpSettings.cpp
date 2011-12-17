#ifdef WITHPHP


#include "PhpSettings.h"

#include "UmlCom.h"
#include "PhpSettingsCmd.h"
#include "JavaSettingsCmd.h"
#include "UmlStereotype.h"
#ifdef WITHJAVA
#include "JavaSettings.h"
#endif
bool PhpSettings::useDefaults()
{
  UmlCom::send_cmd(phpSettingsCmd, getPhpUseDefaultsCmd);
  return UmlCom::read_bool();
}

bool PhpSettings::set_UseDefaults(bool y)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpUseDefaultsCmd, (char) y);
  return UmlCom::read_bool();
}

QCString PhpSettings::classStereotype(const QCString & s)
{
  read_if_needed_();
  
  UmlStereotype * b = UmlSettings::_map_class_stereotypes.find(s);
  
  return (b) ? b->php : s;
}

bool PhpSettings::set_ClassStereotype(QCString s, QCString v)
{
  read_if_needed_();
  UmlCom::send_cmd(phpSettingsCmd, setPhpClassStereotypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlStereotype * st = UmlSettings::_map_class_stereotypes.find(s);

    if (st == 0)
      st = UmlSettings::add_class_stereotype(s);
    st->php = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QCString PhpSettings::classUmlStereotype(const QCString & s)
{
  read_if_needed_();
  
  return UmlSettings::uml_class_stereotype(s, &UmlStereotype::php);
}

const QCString & PhpSettings::rootDir()
{
  read_if_needed_();
  
  return _root;
}

bool PhpSettings::set_RootDir(QCString v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpRootdirCmd, v);
  if (UmlCom::read_bool()) {
    _root = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PhpSettings::sourceContent()
{
  read_if_needed_();
  
  return _src_content;
}

bool PhpSettings::set_SourceContent(QCString v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpSourceContentCmd, v);
  if (UmlCom::read_bool()) {
    _src_content = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PhpSettings::sourceExtension()
{
  read_if_needed_();
  
  return _ext; 
}

bool PhpSettings::set_SourceExtension(QCString v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpSourceExtensionCmd, v);
  if (UmlCom::read_bool()) {
    _ext = v;
    return TRUE;
  }
  else
    return FALSE;
}

QCString PhpSettings::reverseRoundtripDirRegExp()
{
  read_if_needed_();

  return _dir_regexp;
}

bool PhpSettings::isReverseRoundtripDirRegExpCaseSensitive()
{
  read_if_needed_();

  return _dir_regexp_case_sensitive;
}

bool PhpSettings::set_ReverseRoundtripDirRegExp(QCString s, bool cs)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpDirRevFilterCmd, s, cs);
  if (UmlCom::read_bool()) {
    _dir_regexp = s;
    _dir_regexp_case_sensitive = cs;
    return TRUE;
  }
  else
    return FALSE;
}

QCString PhpSettings::reverseRoundtripFileRegExp()
{
  read_if_needed_();

  return _file_regexp;
}

bool PhpSettings::isReverseRoundtripFileRegExpCaseSensitive()
{
  read_if_needed_();

  return _file_regexp_case_sensitive;
}

bool PhpSettings::set_ReverseRoundtripFileRegExp(QCString s, bool cs)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpFileRevFilterCmd, s, cs);
  if (UmlCom::read_bool()) {
    _file_regexp = s;
    _file_regexp_case_sensitive = cs;
    return TRUE;
  }
  else
    return FALSE;
}

bool PhpSettings::requireOnceWithPath()
{
  read_if_needed_();

  return _req_with_path;
}

bool PhpSettings::set_RequireOnceWithPath(bool v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpRequireOnceWithPathCmd, v);
  if (UmlCom::read_bool()) {
    _req_with_path = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool PhpSettings::isRelativePath()
{
  read_if_needed_();

  return _is_relative_path;
}

bool PhpSettings::set_IsRelativePath(bool v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpRelativePathCmd, v);
  if (UmlCom::read_bool()) {
    _is_relative_path = v;
    if (v) _is_root_relative_path = FALSE;
    return TRUE;
  }
  else
    return FALSE;
}

bool PhpSettings::isRootRelativePath()
{
  read_if_needed_();

  return _is_root_relative_path;
}

bool PhpSettings::set_IsRootRelativePath(bool v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpRootRelativePathCmd, v);
  if (UmlCom::read_bool()) {
    _is_root_relative_path = v;
    if (v) _is_relative_path = FALSE;
    return TRUE;
  }
  else
    return FALSE;
}

bool PhpSettings::isForceNamespacePrefixGeneration()
{
  read_if_needed_();

  return _is_force_namespace_gen;
}

bool PhpSettings::set_IsForceNamespacePrefixGeneration(bool v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpForceNamespaceGenCmd, v);
  if (UmlCom::read_bool()) {
    _is_force_namespace_gen = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool PhpSettings::isGenerateJavadocStyleComment()
{
  read_if_needed_();

  return _is_generate_javadoc_comment;
}

bool PhpSettings::set_IsGenerateJavadocStyleComment(bool v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpJavadocStyleCmd, v);
  if (UmlCom::read_bool()) {
    _is_generate_javadoc_comment = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PhpSettings::classDecl()
{
  read_if_needed_();
  
  return _class_decl;
}

bool PhpSettings::set_ClassDecl(QCString v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpClassDeclCmd, v);
  if (UmlCom::read_bool()) {
    _class_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PhpSettings::externalClassDecl()
{
  read_if_needed_();
  
  return _external_class_decl;
}

bool PhpSettings::set_ExternalClassDecl(QCString v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpExternalClassDeclCmd, v);
  if (UmlCom::read_bool()) {
    _external_class_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

QCString PhpSettings::enumDecl()
{
  read_if_needed_();
  
  return _enum_decl;
}

bool PhpSettings::set_EnumDecl(QCString v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpEnumDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PhpSettings::interfaceDecl()
{
  read_if_needed_();
  
  return _interface_decl;
}

bool PhpSettings::set_InterfaceDecl(QCString v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpInterfaceDeclCmd, v);
  if (UmlCom::read_bool()) {
    _interface_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PhpSettings::attributeDecl()
{
  read_if_needed_();

  return _attr_decl;
}

bool PhpSettings::set_AttributeDecl(QCString v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpAttributeDeclCmd, v);
  if (UmlCom::read_bool()) {
    _attr_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

QCString PhpSettings::enumItemDecl()
{
  read_if_needed_();
  
  return _enum_item_decl;
}

bool PhpSettings::set_EnumItemDecl(QCString v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpEnumItemDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_item_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PhpSettings::relationDecl()
{
  read_if_needed_();

  return _rel_decl;
}

bool PhpSettings::set_RelationDecl(QCString v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpRelationDeclCmd, v);
  if (UmlCom::read_bool()) {
    _rel_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PhpSettings::operationDef()
{
  read_if_needed_();
  
  return _oper_def;
}

bool PhpSettings::set_OperationDef(QCString v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpOperationDefCmd, v);
  if (UmlCom::read_bool()) {
    _oper_def = v;
    return TRUE;
  }
  else
    return FALSE;
}

aVisibility PhpSettings::getVisibility()
{
#ifdef WITHJAVA
  return JavaSettings::getVisibility();
#else
  read_if_needed_();
  
  return _get_visibility;
#endif
}

bool PhpSettings::set_GetVisibility(aVisibility v)
{
#ifdef WITHJAVA
  return JavaSettings::set_GetVisibility(v);
#else
  UmlCom::send_cmd(phpSettingsCmd, setJavaGetvisibilityCmd, v);
  if (UmlCom::read_bool()) {
    _get_visibility = v;
    return TRUE;
  }
  else
    return FALSE;
#endif
}

const QCString & PhpSettings::getName()
{
  read_if_needed_();
  
  return _get_name;
}

bool PhpSettings::set_GetName(QCString v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpGetNameCmd, v);
  if (UmlCom::read_bool()) {
    _get_name = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PhpSettings::setName()
{
  read_if_needed_();
  
  return _set_name;
}

bool PhpSettings::set_SetName(QCString v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpSetNameCmd, v);
  if (UmlCom::read_bool()) {
    _set_name = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool PhpSettings::isGetFinal()
{
  read_if_needed_();
  
  return _is_get_final;
}

bool PhpSettings::set_IsGetFinal(bool v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpIsGetFinalCmd, v);
  if (UmlCom::read_bool()) {
    _is_get_final = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool PhpSettings::isSetFinal()
{
  read_if_needed_();
  
  return _is_set_final;
}

bool PhpSettings::set_IsSetFinal(bool v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpIsSetFinalCmd, v);
  if (UmlCom::read_bool()) {
    _is_set_final = v;
    return TRUE;
  }
  else
    return FALSE;
}

aVisibility PhpSettings::setVisibility()
{
#ifdef WITHJAVA
  return JavaSettings::setVisibility();
#else
  read_if_needed_();
  
  return _set_visibility;
#endif
}

bool PhpSettings::set_SetVisibility(aVisibility v)
{
#ifdef WITHJAVA
  return JavaSettings::set_SetVisibility(v);
#else
  UmlCom::send_cmd(phpSettingsCmd, setJavaSetVisibilityCmd, v);
  if (UmlCom::read_bool()) {
    _set_visibility = v;
    return TRUE;
  }
  else
    return FALSE;
#endif
}

bool PhpSettings::_defined;

QCString PhpSettings::_root;

QCString PhpSettings::_class_decl;

QCString PhpSettings::_external_class_decl;

QCString PhpSettings::_enum_decl;

QCString PhpSettings::_interface_decl;

QCString PhpSettings::_attr_decl;

QCString PhpSettings::_enum_item_decl;

QCString PhpSettings::_rel_decl;

QCString PhpSettings::_oper_def;

#ifndef WITHJAVA
    aVisibility PhpSettings::_get_visibility;
#endif

QCString PhpSettings::_get_name;

bool PhpSettings::_is_get_final;

#ifndef WITHJAVA
    aVisibility PhpSettings::_set_visibility;
#endif

QCString PhpSettings::_set_name;

bool PhpSettings::_is_set_final;

QCString PhpSettings::_src_content;

QCString PhpSettings::_ext;

QCString PhpSettings::_dir_regexp;

bool PhpSettings::_dir_regexp_case_sensitive;

QCString PhpSettings::_file_regexp;

bool PhpSettings::_file_regexp_case_sensitive;

bool PhpSettings::_is_generate_javadoc_comment;

bool PhpSettings::_req_with_path;

bool PhpSettings::_is_relative_path;

bool PhpSettings::_is_root_relative_path;

bool PhpSettings::_is_force_namespace_gen;

void PhpSettings::read_()
{
  _root = UmlCom::read_string();
  
  unsigned n;
  unsigned index;
  
  n = UmlCom::read_unsigned();
  
  for (index = 0; index != n; index += 1)
    UmlSettings::_class_stereotypes[index].php = UmlCom::read_string();
  
  _src_content = UmlCom::read_string();
  _ext = UmlCom::read_string();

  _class_decl = UmlCom::read_string();
  _external_class_decl = UmlCom::read_string();
  _enum_decl = UmlCom::read_string();
  _interface_decl = UmlCom::read_string();
  _attr_decl = UmlCom::read_string();
  _enum_item_decl = UmlCom::read_string();
  _rel_decl = UmlCom::read_string();
  _oper_def = UmlCom::read_string();
#ifndef WITHJAVA
  _get_visibility =
#endif
    (aVisibility) UmlCom::read_char();
  _get_name = UmlCom::read_string();
  _is_get_final = UmlCom::read_bool();
#ifndef WITHJAVA
  _set_visibility =
#endif
    (aVisibility) UmlCom::read_char();
  _set_name = UmlCom::read_string();
  _is_set_final = UmlCom::read_bool();
  _is_generate_javadoc_comment = UmlCom::read_bool();
  _req_with_path = UmlCom::read_bool();
  _is_relative_path = UmlCom::read_bool();
  _is_root_relative_path = UmlCom::read_bool();

  _dir_regexp = UmlCom::read_string();
  _dir_regexp_case_sensitive = UmlCom::read_bool();

  _file_regexp = UmlCom::read_string();
  _file_regexp_case_sensitive = UmlCom::read_bool();
  
  _is_force_namespace_gen = UmlCom::read_bool();
}

void PhpSettings::read_if_needed_()
{
  UmlSettings::read_if_needed_();
  if (!_defined) {
    UmlCom::send_cmd(phpSettingsCmd, getPhpSettingsCmd);
    read_();
    _defined = TRUE;
  }
}


#endif
