#ifdef WITHPYTHON


#include "PythonSettings.h"

#include "UmlCom.h"
#include "PythonSettingsCmd.h"
#include "UmlStereotype.h"
bool PythonSettings::isPython_2_2()
{
  read_if_needed_();
  return _2_2;
}

bool PythonSettings::set_IsPython_2_2(bool y)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPython22Cmd, (char) y);
  if (UmlCom::read_bool()) {
    _2_2 = y;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PythonSettings::indentStep()
{
  read_if_needed_();
  return _indent_step;
}

bool PythonSettings::set_IndentStep(char * v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonIndentStepCmd, v);
  if (UmlCom::read_bool()) {
    _indent_step = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool PythonSettings::useDefaults()
{
  UmlCom::send_cmd(pythonSettingsCmd, getPythonUseDefaultsCmd);
  return UmlCom::read_bool();
}

bool PythonSettings::set_UseDefaults(bool y)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonUseDefaultsCmd, (char) y);
  return UmlCom::read_bool();
}

QCString PythonSettings::relationAttributeStereotype(const QCString & s)
{
  read_if_needed_();
  
  UmlStereotype * b = UmlSettings::_map_relation_attribute_stereotypes.find(s);
  
  return (b) ? b->python : s;
}

bool PythonSettings::set_RelationAttributeStereotype(QCString s, QCString v)
{
  read_if_needed_();
  UmlCom::send_cmd(pythonSettingsCmd, setPythonRelationAttributeStereotypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlStereotype * st = UmlSettings::_map_relation_attribute_stereotypes.find(s);

    if (st == 0)
      st = UmlSettings::add_rel_attr_stereotype(s);
    st->python = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QCString PythonSettings::relationAttributeUmlStereotype(const QCString & s)
{
  read_if_needed_();
  
  return UmlSettings::uml_rel_attr_stereotype(s, &UmlStereotype::python);
}

QCString PythonSettings::classStereotype(const QCString & s)
{
  read_if_needed_();
  
  UmlStereotype * b = UmlSettings::_map_class_stereotypes.find(s);
  
  return (b) ? b->python : s;
}

bool PythonSettings::set_ClassStereotype(QCString s, QCString v)
{
  read_if_needed_();
  UmlCom::send_cmd(pythonSettingsCmd, setPythonClassStereotypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlStereotype * st = UmlSettings::_map_class_stereotypes.find(s);

    if (st == 0)
      st = UmlSettings::add_class_stereotype(s);
    st->python = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QCString PythonSettings::classUmlStereotype(const QCString & s)
{
  read_if_needed_();
  
  return UmlSettings::uml_class_stereotype(s, &UmlStereotype::python);
}

QCString PythonSettings::get_import(const QCString & s)
{
  read_if_needed_();
  
  QCString * r = _map_imports[s];
  
  return (r) ? *r : QCString(0);
}

bool PythonSettings::set_Import(QCString s, QCString v)
{
  read_if_needed_();
  UmlCom::send_cmd(pythonSettingsCmd, setPythonImportCmd, s, v);
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

const QCString & PythonSettings::rootDir()
{
  read_if_needed_();
  
  return _root;
}

bool PythonSettings::set_RootDir(QCString v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonRootdirCmd, v);
  if (UmlCom::read_bool()) {
    _root = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PythonSettings::sourceContent()
{
  read_if_needed_();
  
  return _src_content;
}

bool PythonSettings::set_SourceContent(QCString v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonSourceContentCmd, v);
  if (UmlCom::read_bool()) {
    _src_content = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PythonSettings::sourceExtension()
{
  read_if_needed_();
  
  return _ext; 
}

bool PythonSettings::set_SourceExtension(QCString v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonSourceExtensionCmd, v);
  if (UmlCom::read_bool()) {
    _ext = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PythonSettings::classDecl()
{
  read_if_needed_();
  
  return _class_decl;
}

bool PythonSettings::set_ClassDecl(QCString v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonClassDeclCmd, v);
  if (UmlCom::read_bool()) {
    _class_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PythonSettings::externalClassDecl()
{
  read_if_needed_();
  
  return _external_class_decl;
}

bool PythonSettings::set_ExternalClassDecl(QCString v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonExternalClassDeclCmd, v);
  if (UmlCom::read_bool()) {
    _external_class_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

QCString PythonSettings::enumDecl()
{
  read_if_needed_();
  
  return _enum_decl;
}

bool PythonSettings::set_EnumDecl(QCString v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonEnumDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PythonSettings::attributeDecl(const char * multiplicity)
{
  read_if_needed_();

  return _attr_decl[mult_column(multiplicity)];
}

bool PythonSettings::set_AttributeDecl(const char * multiplicity, QCString v)
{
  read_if_needed_();
  UmlCom::send_cmd(pythonSettingsCmd, setPythonAttributeDeclCmd, multiplicity, v);
  if (UmlCom::read_bool()) {
    _attr_decl[mult_column(multiplicity)] = v;
    return TRUE;
  }
  else
    return FALSE;
}

QCString PythonSettings::enumItemDecl()
{
  read_if_needed_();
  
  return _enum_item_decl;
}

bool PythonSettings::set_EnumItemDecl(QCString v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonEnumItemDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_item_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PythonSettings::relationDecl(bool by_value, const char * multiplicity)
{
  read_if_needed_();
  
  return _rel_decl[(by_value) ? 1 : 0][mult_column(multiplicity)];
}

bool PythonSettings::set_RelationDecl(bool by_value, const char * multiplicity, const char * v)
{
  read_if_needed_();
  UmlCom::send_cmd(pythonSettingsCmd, setPythonRelationDeclCmd, by_value, multiplicity, v);
  if (UmlCom::read_bool()) {
    _rel_decl[(by_value) ? 1 : 0][mult_column(multiplicity)] = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PythonSettings::operationDef()
{
  read_if_needed_();
  
  return _oper_def;
}

bool PythonSettings::set_OperationDef(QCString v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonOperationDefCmd, v);
  if (UmlCom::read_bool()) {
    _oper_def = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PythonSettings::getName()
{
  read_if_needed_();
  
  return _get_name;
}

bool PythonSettings::set_GetName(QCString v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonGetNameCmd, v);
  if (UmlCom::read_bool()) {
    _get_name = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & PythonSettings::setName()
{
  read_if_needed_();
  
  return _set_name;
}

bool PythonSettings::set_SetName(QCString v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonSetNameCmd, v);
  if (UmlCom::read_bool()) {
    _set_name = v;
    return TRUE;
  }
  else
    return FALSE;
}

unsigned PythonSettings::mult_column(const QCString & mult)
{
  return (mult.isEmpty() || (mult == "1")) ? 0 : 1;
}

bool PythonSettings::_defined;

bool PythonSettings::_2_2;

QCString PythonSettings::_indent_step;

QCString PythonSettings::_root;

QCString PythonSettings::_class_decl;

QCString PythonSettings::_external_class_decl;

QCString PythonSettings::_enum_decl;

QCString PythonSettings::_attr_decl[2/*multiplicity*/];

QCString PythonSettings::_enum_item_decl;

QCString PythonSettings::_rel_decl[2/*relation kind*/][2/*multiplicity*/];

QCString PythonSettings::_oper_def;

QCString PythonSettings::_get_name;

QCString PythonSettings::_set_name;

QCString PythonSettings::_src_content;

QCString PythonSettings::_ext;

QDict<QCString> PythonSettings::_map_imports;

void PythonSettings::read_()
{
  _2_2 = UmlCom::read_bool();
  
  _indent_step = UmlCom::read_string();
  
  _root = UmlCom::read_string();
  
  unsigned n;
  unsigned index;
  
  n = UmlCom::read_unsigned();
  
  for (index = 0; index != n; index += 1)
    UmlSettings::_relation_attribute_stereotypes[index].python = UmlCom::read_string();
  
  n = UmlCom::read_unsigned();
  
  for (index = 0; index != n; index += 1)
    UmlSettings::_class_stereotypes[index].python = UmlCom::read_string();
  
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
  _attr_decl[0] = UmlCom::read_string();
  _attr_decl[0] = UmlCom::read_string();
  _enum_item_decl = UmlCom::read_string();
  _rel_decl[0][0] = UmlCom::read_string();
  _rel_decl[0][1] = UmlCom::read_string();
  _rel_decl[1][0] = UmlCom::read_string();
  _rel_decl[1][1] = UmlCom::read_string();
  _oper_def = UmlCom::read_string();
  _get_name = UmlCom::read_string();
  _set_name = UmlCom::read_string();
}

void PythonSettings::read_if_needed_()
{
  UmlSettings::read_if_needed_();
  if (!_defined) {
    UmlCom::send_cmd(pythonSettingsCmd, getPythonSettingsCmd);
    read_();
    _defined = TRUE;
  }
}


#endif
