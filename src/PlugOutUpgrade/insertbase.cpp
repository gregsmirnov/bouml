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
#include "UmlArtifact.h"

#include "insertbase.h"

//
// add UmlBaseParameter, move UmlParameter members into it
// add add inheritance
//

void add_umlbaseparameter(UmlClass * uml_base_item, UmlClass * uml_item)
{
  UmlClassView * base_class_view = (UmlClassView *) uml_base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) uml_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    uml_base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    uml_item->associatedArtifact()->parent();
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlClass * uml_parameter = UmlClass::get("UmlParameter", 0);
  UmlClass * uml_base_parameter = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseParameter");

  uml_base_parameter->set_Stereotype("struct");
  uml_base_parameter->set_Description(uml_parameter->description());
  uml_base_parameter->set_CppDecl(uml_parameter->cppDecl());
  uml_base_parameter->set_JavaDecl(uml_parameter->javaDecl());

  UmlClass * oper;

  oper = UmlClass::get("UmlBaseOperation", 0);
  uml_base_parameter->moveAfter(oper);
  uml_base_parameter->associatedArtifact()->moveAfter(oper->associatedArtifact());

  uml_parameter->moveIn(user_class_view);
  oper = UmlClass::get("UmlOperation", 0);
  uml_parameter->moveAfter(oper);

  UmlArtifact * art = uml_parameter->associatedArtifact();
  
  art->moveIn(user_depl_view);
  art->moveAfter(oper->associatedArtifact());
  art->setUser(uid);

  //

  uml_parameter->get_attribute("default_value")->moveIn(uml_base_parameter);
  uml_parameter->get_attribute("name")->moveIn(uml_base_parameter);

  UmlRelation * rel;
  UmlRelation * old;

  old = uml_parameter->get_relation(aDirectionalAggregationByValue, "type");
  rel = UmlBaseRelation::create(aDirectionalAggregationByValue,
				uml_base_parameter, 
				UmlClass::get("UmlTypeSpec", 0));
  rel->set_CppDecl(old->cppDecl());
  rel->set_JavaDecl(old->javaDecl());
  rel->set_RoleName("type");
  rel->set_Visibility(PublicVisibility);
  old->deleteIt();

  old = uml_parameter->get_relation(aDirectionalAggregationByValue, "dir");
  rel = UmlBaseRelation::create(aDirectionalAggregationByValue,
				uml_base_parameter, 
				UmlClass::get("aDirection", 0));
  rel->set_CppDecl(old->cppDecl());
  rel->set_JavaDecl(old->javaDecl());
  rel->set_RoleName("dir");
  rel->set_Visibility(PublicVisibility);
  old->deleteIt();

  UmlOperation * op;
  QCString cpp_body;
  QCString java_body;
  
  op = uml_parameter->get_operation("clone_it");
  cpp_body = op->cppBody();
  java_body = op->javaBody();
  op->moveIn(uml_base_parameter);
  op->set_CppBody(cpp_body);
  op->set_JavaBody(java_body);

  op = uml_parameter->get_operation("UmlParameter");
  cpp_body = op->cppBody();
  java_body = op->javaBody();  
  op->moveIn(uml_base_parameter);
  op->set_CppBody(cpp_body);
  op->set_JavaBody(java_body);
  op->set_Name("UmlBaseParameter");

  // must not be done before else .body not find
  uml_parameter->setUser(uid);
  
  //

  UmlCom::set_user_id(uid);

  rel = UmlBaseRelation::create(aGeneralisation, uml_parameter, uml_base_parameter);

  if (rel == 0) {
    QCString msg = "UmlParameter can't inherit UmlBaseParameter<br>\n";
    
    UmlCom::trace("<b>" + msg + "</b>");
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
}

//
// add UmlBaseTypeSpec, move UmlTypeSpec members into it
// add add inheritance
//

void add_umlbasetypespec(UmlClass * uml_base_item, UmlClass * uml_item)
{
  UmlClassView * base_class_view = (UmlClassView *) uml_base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) uml_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    uml_base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    uml_item->associatedArtifact()->parent();
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlClass * uml_typespec = UmlClass::get("UmlTypeSpec", 0);
  UmlClass * uml_base_typespec = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseTypeSpec");

  uml_base_typespec->set_Description(uml_typespec->description());
  uml_base_typespec->set_CppDecl(uml_typespec->cppDecl());
  uml_base_typespec->set_JavaDecl(uml_typespec->javaDecl());

  uml_base_typespec->moveAfter(uml_typespec);
  uml_base_typespec->associatedArtifact()->moveAfter(uml_typespec->associatedArtifact());

  uml_typespec->moveIn(user_class_view);

  UmlArtifact * art = uml_typespec->associatedArtifact();
  
  art->moveIn(user_depl_view);
  art->setUser(uid);

  //

  uml_typespec->get_attribute("explicit_type")->moveIn(uml_base_typespec);

  UmlRelation * rel;
  UmlRelation * old;

  old = uml_typespec->get_relation(aDirectionalAssociation, "type");
  rel = UmlBaseRelation::create(aDirectionalAssociation,
				uml_base_typespec, 
				UmlClass::get("UmlClass", 0));
  rel->set_CppDecl(old->cppDecl());
  rel->set_JavaDecl(old->javaDecl());
  rel->set_RoleName("type");
  rel->set_Visibility(PublicVisibility);
  old->deleteIt();

  UmlOperation * op;
  QCString cpp_body;
  QCString java_body;
  
  op = uml_typespec->get_operation("clone_it");
  cpp_body = op->cppBody();
  java_body = op->javaBody();  
  op->moveIn(uml_base_typespec);
  op->set_CppBody(cpp_body);
  op->set_JavaBody(java_body);
  
  op = uml_typespec->get_operation("toString");
  cpp_body = op->cppBody();
  java_body = op->javaBody();  
  op->moveIn(uml_base_typespec);
  op->set_CppBody(cpp_body);
  op->set_JavaBody(java_body);

  op = uml_typespec->get_operation("UmlTypeSpec");
  cpp_body = op->cppBody();
  java_body = op->javaBody();  
  op->moveIn(uml_base_typespec);
  op->set_CppBody(cpp_body);
  op->set_JavaBody(java_body);
  op->set_Name("UmlBaseTypeSpec");

  // must not be done before else .body not find
  uml_typespec->setUser(uid);
  
  //

  UmlCom::set_user_id(uid);

  rel = UmlBaseRelation::create(aGeneralisation, uml_typespec, uml_base_typespec);

  if (rel == 0) {
    QCString msg = "UmlTypeSpec can't inherit UmlBaseTypeSpec<br>\n";
    
    UmlCom::trace("<b>" + msg + "</b>");
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
}

//
// add UmlBaseView and UmlView classes between base view
// classes and UmlItem
//

void add_umlbaseview_umlview(UmlClass * base_item, UmlClass * user_item)
{
  UmlClassView * base_class_view = (UmlClassView *) base_item->parent();
  UmlClassView * user_class_view = (UmlClassView *) user_item->parent();
  UmlDeploymentView * base_depl_view = (UmlDeploymentView *)
    base_item->associatedArtifact()->parent();
  UmlDeploymentView * user_depl_view = (UmlDeploymentView *)
    user_item->associatedArtifact()->parent();
  UmlClass * base_view;
  UmlClass * user_view;
  
  user_item->made(base_class_view, user_class_view,
		  base_depl_view, user_depl_view,
		  base_item, "View", base_view, user_view,
		  0);

  user_view->add_constr(base_view, ProtectedVisibility);
  user_view->set_isAbstract(TRUE);

  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
    
  base_view->add_constr(user_item, ProtectedVisibility); 
  
  const char * views[] = {
    "UmlBaseClassView",
    "UmlBaseComponentView",
    "UmlBaseDeploymentView",
    "UmlBaseUseCaseView"
  };
  
  for (unsigned i = 0; i != sizeof(views)/sizeof(views[0]); i += 1) {
    UmlClass * bview = UmlClass::get(views[i], 0);
    UmlRelation * rel =
      UmlBaseRelation::create(aGeneralisation, bview, user_view);
    
    if (rel == 0) {
      QCString msg = QCString(views[i]) + " can't inherit UmlView<br>\n";
      
      UmlCom::trace("<b>" + msg + "</b>");
      throw 0;
    }
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
    bview->get_relation(aGeneralisation, 0)->deleteIt(); // old gene
    rel->moveAfter(0);	// after old gene del
    
    UmlOperation * op = bview->get_operation(views[i]);
    QCString s = op->cppDecl();
    
    op->set_CppDecl(s.replace(s.find(": UmlItem"), 9, ": UmlView"));
  }
  
  //

  UmlCom::set_user_id(uid);
}
