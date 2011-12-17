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

#include "UmlClassView.h"
#include "UmlDeploymentView.h"
#include "UmlClass.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"
#include "UmlRelation.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "UmlArtifact.h"

#include "util.h"
#include "instance.h"

// 
// add class instance
//

void add_class_instance(UmlClassView * base_class_view, UmlClassView * user_class_view,
			UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
			UmlClass * base_item, UmlClass * user_item)
{
  UmlClass * base_class_instance;
  UmlClass * user_class_instance;
  
  user_item->made(base_class_view, user_class_view, base_depl_view, user_depl_view,
		  base_item, "ClassInstance", base_class_instance, user_class_instance,
		  0);
  
  user_class_instance->add_constr(base_class_instance, PublicVisibility);  
    
  //
  // SlotAttribute
  //
  
  UmlClass * user_class = UmlClass::get("UmlClass", 0);
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlClass * slotattr = UmlClass::create(base_class_view, "SlotAttribute");
  UmlClass * attr = UmlClass::get("UmlAttribute", 0);
  UmlArtifact * art;
  UmlOperation * op;
  
  slotattr->set_Stereotype("struct");
  slotattr->set_CppDecl(CppSettings::structDecl());
  slotattr->set_JavaDecl(JavaSettings::classDecl());
  slotattr->add_relation(aDirectionalAssociation, "attribute",
			 PublicVisibility, attr, 0, 0);
  slotattr->add_attribute("value", PublicVisibility, "string", 0, 0);
  op = slotattr->add_op("SlotAttribute", PublicVisibility, "", FALSE);
  op->set_Description(" because of QValueList use");
  op->set_cpp(": attribute(0)", "", "", TRUE, 0, 0);
  op->set_JavaDef("");
  op = slotattr->add_op("SlotAttribute", PublicVisibility, "", FALSE);
  op->add_param(0, InputOutputDirection, "a", attr);
  op->add_param(1, InputOutputDirection, "v", "string");
  op->set_cpp(": attribute(a), value(v)", "${t0} * ${p0}, ${t1} ${p1}",
	      "", TRUE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  attribute = a;\n"
	       "  value = v;\n",
	       FALSE);
  
  art = UmlArtifact::made(base_depl_view, "SlotAttribute");
  art->set_CppSource("");
  art->addAssociatedClass(slotattr);
  
  //
  // SlotRelation
  //
  
  UmlClass * slotrel = UmlClass::create(base_class_view, "SlotRelation");
  UmlClass * rel = UmlClass::get("UmlRelation", 0);
  
  slotrel->set_Stereotype("struct");
  slotrel->set_CppDecl(CppSettings::structDecl());
  slotrel->set_JavaDecl(JavaSettings::classDecl());
  slotrel->add_relation(aDirectionalAssociation, "relation",
			PublicVisibility, rel, 0, 0); 
  slotrel->add_relation(aDirectionalAssociation,
			"value", PublicVisibility, user_class_instance, 0, 0); 
  op = slotrel->add_op("SlotRelation", PublicVisibility, "", FALSE);
  op->set_Description(" because of QValueList use");
  op->set_cpp(": relation(0), value(0)",  "", "", TRUE, 0, 0);
  op->set_JavaDef("");
  op = slotrel->add_op("SlotRelation", PublicVisibility, "", FALSE);
  op->add_param(0, InputOutputDirection, "r", rel);
  op->add_param(1, InputOutputDirection, "v", user_class_instance);
  op->set_cpp(": relation(r), value(v)",  "${t0} * ${p0}, ${t1} * ${p1}",
	      "", TRUE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  relation = r;\n"
	       "  value = v;\n",
	       FALSE);
  
  art = UmlArtifact::made(base_depl_view, "SlotRelation");
  art->set_CppSource("");
  art->addAssociatedClass(slotrel);
  
  //
  // ClassInstance members
  //
  
  include_umlcom(base_class_instance);
  
  base_class_instance->add_kind("aClassInstance");
  
  // create()
  
  op = base_class_instance->add_op("create", PublicVisibility, user_class_instance, TRUE);
  op->set_isClassMember(TRUE);
  op->add_param(0, InputOutputDirection, "parent", user_item);
  op->add_param(1, InputOutputDirection, "name", "str");
  op->add_param(2, InputOutputDirection, "type", user_class);
  op->set_cpp("${type} *", 
	      "${t0} * ${p0}, const ${t1} ${p1}, ${t2} * ${p2}",
	      "  UmlCom::send_cmd(parent->_identifier, createCmd, aClassInstance,\n"
	      "\t\t   type->_identifier);\n"
	      "  UmlClassInstance * result = (UmlClassInstance *) UmlBaseItem::read_();\n"
	      "\n"
	      "  if (result != 0) {\n"
	      "    parent->reread_children_if_needed_();\n"
	      "    if (name != 0) result->set_Name(name);\n"
	      "  }\n"
	      "  return result;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}",
	       "  UmlCom.send_cmd(parent.identifier_(), OnInstanceCmd.createCmd, anItemKind.aClassInstance,\n"
	       "\t\t   type.identifier_());\n"
	       "  UmlClassInstance result = (UmlClassInstance) UmlBaseItem.read_();\n"
	       "  \n"
	       "  if (result != null) {\n"
	       "    parent.reread_children_if_needed_();\n"
	       "    if (name != null) result.set_Name(name);\n"
	      "  }\n"
	       "  else\n"
	       "    throw new RuntimeException(\"Cannot create the class instance\");\n"
	       "  return result;\n",
	      FALSE);
  op->set_Description(" Returns a new class instance\n\
\n\
 In case it cannot be created ('parent' cannot contain it etc ...) return 0\n\
  in C++ and produce a RuntimeException in Java\n");
  
  // type(), set_Type()
    
  defGetPtr(base_class_instance, _type, type, user_class, 0, 0, "type");
  defSetPtr(base_class_instance, _type, set_Type, user_class, setTypeCmd, 0, 0, "type");
    
  // attributesValue()
  
  op = base_class_instance->add_op("attributesValue", PublicVisibility, slotattr);
  op->add_param(0, OutputDirection, "result", slotattr);
  op->set_cpp("void", "QValueList<${t0}> & ${p0}",
	      "  UmlCom::send_cmd(_identifier, attributesCmd, (char) 0);\n"
	      "\n"
	      "  unsigned n = UmlCom::read_unsigned();\n"
	      "\n"
	      "  result.clear();\n"
	      "  while (n--) {\n"
	      "    UmlAttribute * at = (UmlAttribute *) UmlBaseItem::read_();\n"
	      "\n"
	      "    result.append(SlotAttribute(at, UmlCom::read_string()));\n"
	      "  }\n",
	      FALSE, 0, 0);
  op->set_java("${type}[]", "",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.attributesCmd, (byte) 0);\n"
	      "\n"
	       "  int n = UmlCom.read_unsigned();\n"
	       "  SlotAttribute[] v = new SlotAttribute[n];\n"
	       "\n"
	       "  for (int index = 0; index != n; index += 1) {\n"
	       "    UmlAttribute at = (UmlAttribute) UmlBaseItem.read_();\n"
	       "\n"
	       "    v[index] = new SlotAttribute(at, UmlCom.read_string());\n"
	       "  }\n"
	       "  return v;\n",
	      FALSE);
  op->set_Description(" Returns the attributes having a value");
  
  // relationsValue()
  
  op = base_class_instance->add_op("relationsValue", PublicVisibility, slotrel);
  op->add_param(0, OutputDirection, "result", slotrel);
  op->set_cpp("void", "QValueList<${t0}> & ${p0}",
	      "  UmlCom::send_cmd(_identifier, relationsCmd, (void *) 0);\n"
	      "\n"
	      "  unsigned n = UmlCom::read_unsigned();\n"
	      "\n"
	      "  result.clear();\n"
	      "  while (n--) {\n"
	      "    UmlRelation * r = (UmlRelation *) UmlBaseItem::read_();\n"
	      "\n"
	      "    result.append(SlotRelation(r, (UmlClassInstance *) UmlBaseItem::read_()));\n"
	      "  }\n",
	      FALSE, 0, 0);
  op->set_java("${type}[]", "",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.relationsCmd, (long) 0);\n"
	      "\n"
	       "  int n = UmlCom.read_unsigned();\n"
	       "  SlotRelation[] v = new SlotRelation[n];\n"
	       "\n"
	       "  for (int index = 0; index != n; index += 1) {\n"
	       "    UmlRelation rel = (UmlRelation) UmlBaseItem.read_();\n"
	       "\n"
	       "    v[index] = new SlotRelation(rel, (UmlClassInstance) UmlBaseItem.read_());\n"
	       "  }\n"
	       "  return v;\n",
	      FALSE);
  op->set_Description(" Returns the attributes having a value");
  
  // availableAttributes()
  
  op = base_class_instance->add_op("availableAttributes", PublicVisibility, attr);
  op->add_param(0, OutputDirection, "result", attr);
  op->set_cpp("void", "QVector<${t0}> & ${p0}",
	      "  UmlCom::send_cmd(_identifier, attributesCmd, (char) 1);\n"
	      "  UmlCom::read_item_list((QVector<UmlItem> &) result);\n",
	      FALSE, 0, 0);
  op->set_java("${type}[]", "",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.attributesCmd, (byte) 1);\n"
	      "\n"
	       "  int n = UmlCom.read_unsigned();\n"
	       "  UmlAttribute[] v = new UmlAttribute[n];\n"
	       "\n"
	       "  for (int index = 0; index != n; index += 1)\n"
	       "    v[index] = (UmlAttribute) UmlBaseItem.read_();\n"
	       "  return v;\n",
	      FALSE);
  op->set_Description(" Returns all the attributes of the class instance,\n"
		      " including the inherited");
  
  // availableRelations()
  
  op = base_class_instance->add_op("availableRelations", PublicVisibility, rel);
  op->add_param(0, OutputDirection, "other", user_class_instance);
  op->add_param(1, OutputDirection, "result", rel);
  op->set_cpp("void", 
	      "${t0} * ${p0}, QVector<${t1}> & ${p1}",
	      "  UmlCom::send_cmd(_identifier, relationsCmd, other->_identifier);\n"
	      "  UmlCom::read_item_list((QVector<UmlItem> &) result);\n",
	      FALSE, 0, 0);
  op->set_java("${type}[]", "${t0} ${p0}",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.relationsCmd, other.identifier_());\n"
	      "\n"
	       "  int n = UmlCom.read_unsigned();\n"
	       "  UmlRelation[] v = new UmlRelation[n];\n"
	       "\n"
	       "  for (int index = 0; index != n; index += 1)\n"
	       "    v[index] = (UmlRelation) UmlBaseItem.read_();\n"
	       "  return v;\n",
	      FALSE);
  op->set_Description(" Returns all the possible relations from the current instance"
		      " to 'other', including the inherited");
  
  // set_AttributeValue()
  
  op = base_class_instance->add_op("set_AttributeValue", PublicVisibility, "bool", TRUE);
  op->add_param(0, InputDirection, "attribute", attr);
  op->add_param(1, InputDirection, "value", "str");
  op->set_cpp("bool", "${t0} * ${p0}, const ${t1} ${p1}",
	      "  UmlCom::send_cmd(_identifier, setAttributeCmd, attribute->_identifier, value);\n"
	      "  return UmlCom::read_bool();\n",
	      FALSE, 0, 0);
  op->set_java("void", "${t0} ${p0}, ${t1} ${p1}",
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.setAttributeCmd, attribute.identifier_(), value);\n"
	       "  UmlCom.check();\n",
	      TRUE);
  op->set_Description(" Remove the slot if the value is an empty string.\n"
		      " Else set the value for the given attribute, replacing it\n"
		      " if the slot already exist."
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  
  // add_Relation()
  
  op = base_class_instance->add_op("add_Relation", PublicVisibility, "bool", TRUE);
  op->add_param(0, InputDirection, "relation", attr);
  op->add_param(1, InputDirection, "other", user_class_instance);
  op->set_cpp("bool", "${t0} * ${p0}, ${t1} * ${p1}",
	      "  QVector<UmlItem> v(2);\n"
	      "\n"
	      "  v.insert(0, relation);\n"
	      "  v.insert(1, other);\n"
	      "  UmlCom::send_cmd(_identifier, addRelationCmd, v);\n"
	      "  return UmlCom::read_bool();\n",
	      FALSE, 0, 0);
  op->set_java("void", "${t0} ${p0}, ${t1} ${p1}",
	       "  UmlItem[] v = new UmlItem[2];\n"
	       "\n"
	       "  v[0] = relation;\n"
	       "  v[1] = other;\n"
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.addRelationCmd, v);\n"
	       "  UmlCom.check();\n",
	      TRUE);
  op->set_Description(" Add the slot (does nothing if it already exist)\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  
  // remove_Relation()
  
  op = base_class_instance->add_op("remove_Relation", PublicVisibility, "bool", TRUE);
  op->add_param(0, InputDirection, "relation", attr);
  op->add_param(1, InputDirection, "other", user_class_instance);
  op->set_cpp("bool", "${t0} * ${p0}, ${t1} * ${p1}",
	      "  QVector<UmlItem> v(2);\n"
	      "\n"
	      "  v.insert(0, relation);\n"
	      "  v.insert(1, other);\n"
	      "  UmlCom::send_cmd(_identifier, removeRelationCmd, v);\n"
	      "  return UmlCom::read_bool();\n",
	      FALSE, 0, 0);
  op->set_java("void", "${t0} ${p0}, ${t1} ${p1}",
	       "  UmlItem[] v = new UmlItem[2];\n"
	       "\n"
	       "  v[0] = relation;\n"
	       "  v[1] = other;\n"
	       "  UmlCom.send_cmd(identifier_(), OnInstanceCmd.removeRelationCmd, v);\n"
	       "  UmlCom.check();\n",
	      TRUE);
  op->set_Description(" Remove the slot (does nothing if it doesn't exist)\n"
		      "\n"
		      " On error : return FALSE in C++, produce a RuntimeException in Java");
  
  // assoc diagram
  
  UmlClass * user_objectdiagram = UmlClass::get("UmlObjectDiagram", 0);
  
  add_assoc_diag_ops(base_class_instance, user_objectdiagram);

  // attributes and relations
  
  base_class_instance->add_relation(aDirectionalAssociation,
				    "_type", PrivateVisibility,
				    user_class, 0, 0);  
  
  base_class_instance->add_relation(aDirectionalAssociation,
				    "_assoc_diagram", PrivateVisibility,
				    user_objectdiagram, 0, 0);  
  
  // read_uml
  
  op = base_class_instance->add_op("read_uml_", ProtectedVisibility, "void");
  op->set_cpp("${type}", "",
	      "  _assoc_diagram = (UmlObjectDiagram *) UmlBaseItem::read_();\n"
	      "  UmlBaseItem::read_uml_();\n"
	      "  _type = (UmlClass *) UmlBaseItem::read_();\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  _assoc_diagram = (UmlObjectDiagram) UmlBaseItem.read_();\n"
	       "  super.read_uml_();\n"
	       "  _type = (UmlClass) UmlBaseItem.read_();\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  
  // constructor
  
  base_class_instance->add_constr(user_item, ProtectedVisibility);

  //
  
  QCString s;
  
  art = base_class_instance->associatedArtifact();
  s = art->cppHeader();
  s.insert(s.find("${includes}"),
	   "#include \"SlotAttribute.h\"\n"
	   "#include \"SlotRelation.h\"\n");
  art->set_CppHeader(s);
  UmlCom::set_user_id(uid);
}

void add_class_instance_on_instance_cmd()
{
  // already root
  UmlClass * itcmd = UmlClass::get("OnInstanceCmd", 0);
  
  itcmd->add_enum_item("setAttributeCmd");
  itcmd->add_enum_item("addRelationCmd");
  itcmd->add_enum_item("removeRelationCmd");
  
  UmlAttribute * at = itcmd->add_enum_item("attributesCmd");
  
  at->moveAfter(itcmd->get_attribute("sideCmd"));
  itcmd->add_enum_item("relationsCmd")->moveAfter(at);
}

void baseitem_read_class_instance(UmlClass * base_item)
{
  // update read_()'s body
  UmlOperation * op = base_item->get_operation("read_");
  
  if (op != 0) {
    QCString body;
    
    body = op->cppBody();
    body.insert(body.findRev("default:"),
		"case aClassInstance:\n\
      return new UmlClassInstance(id, name);\n\
    ");
    op->set_CppBody(body);
      
    body = op->javaBody();
    body.insert(body.findRev("default:"),
		"case anItemKind._aClassInstance:\n\
      return new UmlClassInstance(id, name);\n\
    ");
    op->set_JavaBody(body);
  }
  
  // update BaseUmlItem artifact
  UmlArtifact * art = base_item->associatedArtifact();
  QCString s;
  
  s = art->cppSource();
  s.insert(s.find("#include \"UmlUseCase.h\""),
	   "#include \"UmlClassInstance.h\"\n");
  art->set_CppSource(s);
}

void add_class_instance(UmlClass * base_item, UmlClass * user_item)
{
  UmlClassView * base_class_view = (UmlClassView *) base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) user_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    user_item->associatedArtifact()->parent();

  UmlCom::trace("<b>Class instance</b><br>\n");
  
  add_class_instance(base_class_view, user_class_view, base_depl_view, user_depl_view,
		     base_item, user_item);

  //

  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlCom::trace("<b>update anItemKind</b><br>\n");
  
  UmlClass * itkind = UmlClass::get("anItemKind", 0);
  
  itkind->add_enum_item("aClassInstance");
  
  UmlCom::trace("<b>update cmd list</b><br>\n");
  
  add_class_instance_on_instance_cmd();
  
  UmlCom::trace("<b>update item read</b><br>\n");
  
  baseitem_read_class_instance(base_item);
  
  UmlCom::set_user_id(uid);
}

//
//
//

void fixe_classinstance(UmlClass * bci)
{
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  //
  
  UmlCom::trace("<b>Fixe UmlBaseClassInstance operation profiles</b><br>\n");
  
  UmlClass * rel = UmlClass::get("UmlRelation", 0);
  UmlOperation * op = bci->get_operation("add_Relation");
  QValueList<UmlParameter> params = op->params();
  UmlParameter param = params.first();
    
  param.type.type = rel;
  op->replaceParameter(0, param);
  
  //
  
  op = bci->get_operation("remove_Relation");
  params = op->params();
  param = params.first();
    
  param.type.type = rel;
  op->replaceParameter(0, param);
  
  //

  UmlCom::set_user_id(uid);
}

