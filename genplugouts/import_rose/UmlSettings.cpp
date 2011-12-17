
#include "UmlSettings.h"

#include "UmlCom.h"
#include "UmlSettingsCmd.h"
#include "UmlBuiltin.h"
#include "UmlStereotype.h"
bool UmlSettings::_defined;

QDict<UmlBuiltin> UmlSettings::_map_builtins;

UmlBuiltin * UmlSettings::_builtins;

QDict<UmlStereotype> UmlSettings::_map_relation_stereotypes;

UmlStereotype * UmlSettings::_relation_stereotypes;

QDict<UmlStereotype> UmlSettings::_map_class_stereotypes;

UmlStereotype * UmlSettings::_class_stereotypes;

void UmlSettings::read_()
{
  unsigned n;
  unsigned index;
  
  n = UmlCom::read_unsigned();
  
  _builtins = new UmlBuiltin[n];
  if (n/2 > _map_builtins.size())
    _map_builtins.resize(_map_builtins.size() * 2 - 1);
  
  for (index = 0; index != n; index += 1) {
    _builtins[index].uml = UmlCom::read_string();
    _map_builtins.insert(_builtins[index].uml, &_builtins[index]);
  }
  
  n = UmlCom::read_unsigned();
  
  _relation_stereotypes = new UmlStereotype[n];
  if (n/2 > _map_relation_stereotypes.size())
    _map_relation_stereotypes.resize(_map_relation_stereotypes.size() * 2 - 1);
  
  for (index = 0; index != n; index += 1) {
    _relation_stereotypes[index].uml = UmlCom::read_string();
    _map_relation_stereotypes.insert(_relation_stereotypes[index].uml,
				     &_relation_stereotypes[index]);
  }
  
  n = UmlCom::read_unsigned();
  
  _class_stereotypes = new UmlStereotype[n];
  if (n/2 > _map_class_stereotypes.size())
    _map_class_stereotypes.resize(_map_class_stereotypes.size() * 2 - 1);
  
  for (index = 0; index != n; index += 1) {
    _class_stereotypes[index].uml = UmlCom::read_string();
    _map_class_stereotypes.insert(_class_stereotypes[index].uml,
				  &_class_stereotypes[index]);
  }
}

void UmlSettings::read_if_needed_()
{
  if (!_defined) {
    UmlCom::send_cmd(umlSettingsCmd, getUmlSettingsCmd);
    read_();
    _defined = TRUE;
  }
}

unsigned UmlSettings::multiplicity_column(const QCString & mult)
{
  if (mult.isEmpty() || (mult == "1"))
    return 0;

  if ((mult == "*") || (mult.find("..") != -1))
    return 1;

  return 2;
}

QCString UmlSettings::uml_type(const QCString & t, QCString UmlBuiltin::* f)
{
  unsigned index = _map_builtins.count();
  
  while (index--)
    if ((_builtins[index]).*f == t)
      return _builtins[index].uml;
  
  return 0;
}

QCString UmlSettings::uml_rel_stereotype(const QCString & t, QCString UmlStereotype::* f)
{
  unsigned index = _map_relation_stereotypes.count();
  
  while (index--)
    if ((_relation_stereotypes[index]).*f == t)
      return _relation_stereotypes[index].uml;
  
  return 0;
}

QCString UmlSettings::uml_class_stereotype(const QCString & t, QCString UmlStereotype::* f)
{
  unsigned index = _map_class_stereotypes.count();
  
  while (index--)
    if ((_class_stereotypes[index]).*f == t)
      return _class_stereotypes[index].uml;
  
  return 0;
}

UmlBuiltin * UmlSettings::add_type(const QCString & s)
{
  unsigned n = _map_builtins.count();
  unsigned index;

  UmlBuiltin * builtins = new UmlBuiltin[n + 1];

  if (n/2 > _map_builtins.size())
    _map_builtins.resize(_map_builtins.size() * 2 - 1);
  
  for (index = 0; index != n; index += 1)
    builtins[index] = _builtins[index];
    
  builtins[index].uml = s;
#ifdef WITHCPP
  builtins[index].cpp = s;
  builtins[index].cpp_in = "const ${type}";
  builtins[index].cpp_out = "${type} &";
  builtins[index].cpp_inout = "${type} &";
#endif
#ifdef WITHJAVA
  builtins[index].java = s;
#endif
#ifdef WITHIDL
  builtins[index].idl = s;
#endif

  _map_builtins.insert(s, &_builtins[index]);

  delete [] _builtins;
  _builtins = builtins;
  
  return &_builtins[index];

}

UmlStereotype * UmlSettings::add_rel_stereotype(const QCString & s)
{
  unsigned n = _map_relation_stereotypes.count();
  unsigned index;

  UmlStereotype * relation_stereotypes = new UmlStereotype[n + 1];

  if (n/2 > _map_relation_stereotypes.size())
    _map_relation_stereotypes.resize(_map_relation_stereotypes.size() * 2 - 1);
  
  for (index = 0; index != n; index += 1)
    relation_stereotypes[index] = _relation_stereotypes[index];
    
  relation_stereotypes[index].uml = s;
#ifdef WITHCPP
  relation_stereotypes[index].cpp = s;
#endif
#ifdef WITHJAVA
  relation_stereotypes[index].java = s;
#endif
#ifdef WITGIDL
  relation_stereotypes[index].idl = s;
#endif

  _map_relation_stereotypes.insert(s, &_relation_stereotypes[index]);

  delete [] _relation_stereotypes;
  _relation_stereotypes = relation_stereotypes;

  return &_relation_stereotypes[index];

}

UmlStereotype * UmlSettings::add_class_stereotype(const QCString & s)
{
  unsigned n = _map_class_stereotypes.count();
  unsigned index;

  UmlStereotype * class_stereotypes = new UmlStereotype[n + 1];

  if (n/2 > _map_class_stereotypes.size())
    _map_class_stereotypes.resize(_map_class_stereotypes.size() * 2 - 1);
  
  for (index = 0; index != n; index += 1)
    class_stereotypes[index] = _class_stereotypes[index];
    
  class_stereotypes[index].uml = s;
#ifdef WITHCPP
  class_stereotypes[index].cpp = s;
#endif
#ifdef WITHJAVA
  class_stereotypes[index].java = s;
#endif
#ifdef WITHIDL
  class_stereotypes[index].idl = s;
#endif

  _map_class_stereotypes.insert(s, &_class_stereotypes[index]);

  delete [] _class_stereotypes;
  _class_stereotypes = class_stereotypes;

  return &_class_stereotypes[index];

}

