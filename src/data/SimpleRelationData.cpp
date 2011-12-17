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





#include <qtextstream.h>
#include <qcursor.h>

#include "SimpleRelationData.h"
#include "BrowserSimpleRelation.h"
#include "SimpleRelationDialog.h"
#include "BrowserComponent.h"
#include "BrowserUseCase.h"
#include "BrowserDeploymentNode.h"
#include "BrowserArtifact.h"
#include "BrowserPackage.h"
#include "BrowserActivity.h"
#include "BrowserActivityAction.h"
#include "BrowserActivityObject.h"
#include "BrowserClass.h"
#include "myio.h"
#include "ToolCom.h"

SimpleRelationData::SimpleRelationData(UmlCode e) : type(e), end(0) {
}

SimpleRelationData::SimpleRelationData(const BrowserSimpleRelation * model,
				       BrowserSimpleRelation * r)
    : SimpleData(model->get_data()) {
  SimpleRelationData * md = (SimpleRelationData *) model->get_data();
  
  type = md->type;
  set_start_end(r, md->end);
}

SimpleRelationData::~SimpleRelationData() {
}

void SimpleRelationData::delete_it() {
  if (!deletedp()) {
    if (!browser_node->deletedp())
      browser_node->delete_it();
    BasicData::delete_it();
  }
}

void SimpleRelationData::set_start_end(BrowserSimpleRelation * s, BrowserNode * e) {
  browser_node = s;
  end = e;
  connect(e->get_data(), SIGNAL(deleted()), this, SLOT(end_deleted()));
}

BrowserNode * SimpleRelationData::get_start_node() const {
  return (BrowserNode *) browser_node->parent();
}

QString SimpleRelationData::definition(bool, bool with_kind) const {
  QString r;
  
  if (with_kind)
    r = "[" + browser_node->get_stype() + "] ";

  switch (type) {
  case UmlDependOn:
    return r + "<dependency>";
  case UmlInherit:
    return r + "<generalization>";
  default:
    return "SimpleRelationData::definition error !";
  }
}

void SimpleRelationData::edit() {
  (new SimpleRelationDialog(this))->show();
}

void SimpleRelationData::end_deleted() {
  browser_node->package_modified();
  delete_it();
}

//

void SimpleRelationData::send_uml_def(ToolCom * com,
				      BrowserNode * bn,
				      const QString & comment) {
  BasicData::send_uml_def(com, bn, comment);
  switch (type) {
  case UmlDependOn:
    com->write_char(UmlDependency);
    break;
  default:	// UmlInherit:
    com->write_char(UmlGeneralisation);
    break;
  }
    
  get_end_node()->write_id(com);
}

void SimpleRelationData::save(QTextStream & st, QString & warning) const {
  st << stringify(type);
  indent(+1);
  
  BasicData::save(st, warning);
  nl_indent(st);
  st << "on ";
  end->save(st, TRUE, warning);
  indent(-1);
}

SimpleRelationData * SimpleRelationData::read(char * & st)
{
  SimpleRelationData * result =
    new SimpleRelationData(relation_type(read_keyword(st), TRUE));
  char * k = read_keyword(st);
  
  result->BasicData::read(st, k);	// updates k

  if (strcmp(k, "on"))
    wrong_keyword(k, "on");
  
  k = read_keyword(st);
  
  // note : BrowserArtifact::read get Component def on component -> artifact
  if (((result->end = BrowserArtifact::read(st, k, 0)) == 0) &&
      ((result->end = BrowserComponent::read(st, k, 0)) == 0) &&
      ((result->end = BrowserUseCase::read(st, k, 0)) == 0) &&
      ((result->end = BrowserPackage::read(st, k, 0, FALSE)) == 0) &&
      ((result->end = BrowserDeploymentNode::read(st, k, 0)) == 0) &&
      ((result->end = BrowserActivity::read(st, k, 0)) == 0) &&
      ((result->end = BrowserActivityAction::read(st, k, 0)) == 0) &&
      ((result->end = BrowserActivityObject::read(st, k, 0)) == 0) &&
      ((result->end = BrowserClass::read(st, k, 0)) == 0))
    wrong_keyword(k, "a ref to a class, component, usecase, package, deployment node, activity, activity action or activity object");
  
  connect(result->end->get_data(), SIGNAL(deleted()),
	  result, SLOT(end_deleted()));
  
  return result;
}
