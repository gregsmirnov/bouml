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





#include <stdio.h>

#include <qgrid.h> 
#include <qvbox.h>
#include <qlabel.h>

#include "StereotypesDialog.h"
#include "BrowserPackage.h"
#include "BrowserClass.h"
#include "BrowserUseCase.h"
#include "BrowserAttribute.h"
#include "BrowserOperation.h"
#include "BrowserState.h"
#include "BrowserActivity.h"
#include "BrowserInterruptibleActivityRegion.h"
#include "BrowserExpansionRegion.h"
#include "BrowserExpansionNode.h"
#include "BrowserActivityNode.h"
#include "BrowserActivityAction.h"
#include "BrowserActivityObject.h"
#include "BrowserActivityPartition.h"
#include "BrowserPin.h"
#include "BrowserFlow.h"
#include "BrowserParameter.h"
#include "BrowserParameterSet.h"
#include "BrowserPseudoState.h"
#include "BrowserStateAction.h"
#include "BrowserComponent.h"
#include "BrowserArtifact.h"
#include "BrowserDeploymentNode.h"
#include "BrowserClassView.h"
#include "BrowserUseCaseView.h"
#include "BrowserComponentView.h"
#include "BrowserDeploymentView.h"
#include "BrowserClassDiagram.h"
#include "BrowserObjectDiagram.h"
#include "BrowserColDiagram.h"
#include "BrowserSeqDiagram.h"
#include "BrowserUseCaseDiagram.h"
#include "BrowserStateDiagram.h"
#include "BrowserActivityDiagram.h"
#include "BrowserComponentDiagram.h"
#include "BrowserDeploymentDiagram.h"
#include "UmlDesktop.h"
#include "DialogUtil.h"
#include "MyTable.h"
#include "UmlPixmap.h"
#include "strutil.h"
#include "translate.h"

QSize StereotypesDialog::previous_size;

StereotypesDialog::StereotypesDialog()
    : QTabDialog(0, "Default stereotypes dialog", TRUE, 0) {
  setCaption(TR("Default stereotypes dialog"));
  
  setOkButton(TR("OK"));
  setCancelButton(TR("Cancel"));
  
  const QString space(" ");
  QGrid * grid;
  
  // package tab
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel(TR("package\nstereotypes : "), grid);
  package_list = new LineEdit(grid);
  package_list->setTheText(BrowserPackage::its_default_stereotypes.join(space));

  new QLabel(TR("relations\nstereotypes : "), grid);
  packagetable = new MyTable(1, 1, grid);
  packagetable->horizontalHeader()->setLabel(0, TR("Stereotypes"));
  packagetable->verticalHeader()->setLabel(0, QIconSet(*dependencyButton), "");
  packagetable->setText(0, 0, BrowserPackage::relation_default_stereotypes.join(space));
  packagetable->setColumnStretchable (0, TRUE);
  packagetable->setRowStretchable (0, TRUE);
  
  addTab(grid, TR("Package"));
  
  // class tab
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel(TR("class\nstereotypes : "), grid);
  class_list = new LineEdit(grid);
  class_list->setTheText(BrowserClass::its_default_stereotypes.join(space));

  new QLabel(TR("attribute\nstereotypes : "), grid);
  attribute_list = new LineEdit(grid);
  attribute_list->setTheText(BrowserAttribute::its_default_stereotypes.join(space));

  new QLabel(TR("operation\nstereotypes : "), grid);
  operation_list = new LineEdit(grid);
  operation_list->setTheText(BrowserOperation::its_default_stereotypes.join(space));

  new QLabel(TR("relations\nstereotypes : "), grid);
  classtable = new MyTable(9, 1, grid);
  classtable->horizontalHeader()->setLabel(0, TR("Stereotypes"));
  classtable->verticalHeader()->setLabel(0, QIconSet(*generalisationButton), "");
  classtable->setText(0, 0, BrowserClass::relations_default_stereotypes[UmlGeneralisation].join(space));
  classtable->verticalHeader()->setLabel(1, QIconSet(*directionalAssociationButton), "");
  classtable->setText(1, 0, BrowserClass::relations_default_stereotypes[UmlDirectionalAssociation].join(space));
  classtable->verticalHeader()->setLabel(2, QIconSet(*associationButton), "");
  classtable->setText(2, 0, BrowserClass::relations_default_stereotypes[UmlAssociation].join(space));
  classtable->verticalHeader()->setLabel(3, QIconSet(*aggregationButton), "");
  classtable->setText(3, 0, BrowserClass::relations_default_stereotypes[UmlAggregation].join(space));
  classtable->verticalHeader()->setLabel(4, QIconSet(*aggregationByValueButton), "");
  classtable->setText(4, 0, BrowserClass::relations_default_stereotypes[UmlAggregationByValue].join(space));
  classtable->verticalHeader()->setLabel(5, QIconSet(*directionalAggregationButton), "");
  classtable->setText(5, 0, BrowserClass::relations_default_stereotypes[UmlDirectionalAggregation].join(space));
  classtable->verticalHeader()->setLabel(6, QIconSet(*directionalAggregationByValueButton), "");
  classtable->setText(6, 0, BrowserClass::relations_default_stereotypes[UmlDirectionalAggregationByValue].join(space));
  classtable->verticalHeader()->setLabel(7, QIconSet(*dependencyButton), "");
  classtable->setText(7, 0, BrowserClass::relations_default_stereotypes[UmlDependency].join(space));
  classtable->verticalHeader()->setLabel(8, QIconSet(*realizeButton), "");
  classtable->setText(8, 0, BrowserClass::relations_default_stereotypes[UmlRealize].join(space));
  classtable->setColumnStretchable (0, TRUE);
  
  addTab(grid, TR("Class"));
  
  // Use case
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel(TR("use case\nstereotypes : "), grid);
  usecase_list = new LineEdit(grid);
  usecase_list->setTheText(BrowserUseCase::its_default_stereotypes.join(space));

  (void) new QLabel(TR("relations\nstereotypes : "), grid);
  usecasetable = new MyTable(3, 1, grid);
  usecasetable->horizontalHeader()->setLabel(0, TR("Stereotypes"));
  usecasetable->verticalHeader()->setLabel(0, QIconSet(*generalisationButton), "");
  usecasetable->setText(0, 0, BrowserUseCase::relations_default_stereotypes[UmlGeneralisation].join(space));
  usecasetable->verticalHeader()->setLabel(1, QIconSet(*associationButton), "");
  usecasetable->setText(1, 0, BrowserUseCase::relations_default_stereotypes[UmlAssociation].join(space));
  usecasetable->verticalHeader()->setLabel(2, QIconSet(*dependencyButton), "");
  usecasetable->setText(2, 0, BrowserUseCase::relations_default_stereotypes[UmlDependency].join(space));
  usecasetable->setColumnStretchable (0, TRUE);
  usecasetable->setRowStretchable (0, TRUE);
  usecasetable->setRowStretchable (1, TRUE);
  usecasetable->setRowStretchable (2, TRUE);
  
  addTab(grid, TR("Use Case"));
  
  // artifact tab
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel(TR("artifact\nstereotypes : "), grid);
  artifact_list = new LineEdit(grid);
  artifact_list->setTheText(BrowserArtifact::its_default_stereotypes.join(space));

  new QLabel(TR("relations\nstereotypes : "), grid);
  artifacttable = new MyTable(1, 1, grid);
  artifacttable->horizontalHeader()->setLabel(0, TR("Stereotypes"));
  artifacttable->verticalHeader()->setLabel(0, QIconSet(*dependencyButton), "");
  artifacttable->setText(0, 0, BrowserArtifact::relation_default_stereotypes.join(space));
  artifacttable->setColumnStretchable (0, TRUE);
  artifacttable->setRowStretchable (0, TRUE);
  
  addTab(grid, TR("Artifact"));
  
  // view
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel(TR("class view\nstereotypes : "), grid);
  classview_list = new LineEdit(grid);
  classview_list->setTheText(BrowserClassView::its_default_stereotypes.join(space));

  new QLabel(TR("use case view\nstereotypes : "), grid);
  usecaseview_list = new LineEdit(grid);
  usecaseview_list->setTheText(BrowserUseCaseView::its_default_stereotypes.join(space));

  new QLabel(TR("component view\nstereotypes : "), grid);
  componentview_list = new LineEdit(grid);
  componentview_list->setTheText(BrowserComponentView::its_default_stereotypes.join(space));

  new QLabel(TR("deployment view\nstereotypes : "), grid);
  deploymentview_list = new LineEdit(grid);
  deploymentview_list->setTheText(BrowserDeploymentView::its_default_stereotypes.join(space));

  addTab(grid, TR("Views"));
  
  // diagrams
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel(TR("class diagram\nstereotypes : "), grid);
  classdiagram_list = new LineEdit(grid);
  classdiagram_list->setTheText(BrowserClassDiagram::its_default_stereotypes.join(space));

  new QLabel(TR("object diagram\nstereotypes : "), grid);
  objectdiagram_list = new LineEdit(grid);
  objectdiagram_list->setTheText(BrowserObjectDiagram::its_default_stereotypes.join(space));

  new QLabel(TR("communication diagram\nstereotypes : "), grid);
  coldiagram_list = new LineEdit(grid);
  coldiagram_list->setTheText(BrowserColDiagram::its_default_stereotypes.join(space));

  new QLabel(TR("sequence diagram\nstereotypes : "), grid);
  seqdiagram_list = new LineEdit(grid);
  seqdiagram_list->setTheText(BrowserSeqDiagram::its_default_stereotypes.join(space));

  new QLabel(TR("use case diagram\nstereotypes : "), grid);
  usecasediagram_list = new LineEdit(grid);
  usecasediagram_list->setTheText(BrowserUseCaseDiagram::its_default_stereotypes.join(space));

  new QLabel(TR("state diagram\nstereotypes : "), grid);
  statediagram_list = new LineEdit(grid);
  statediagram_list->setTheText(BrowserStateDiagram::its_default_stereotypes.join(space));

  new QLabel(TR("activity diagram\nstereotypes : "), grid);
  activitydiagram_list = new LineEdit(grid);
  activitydiagram_list->setTheText(BrowserActivityDiagram::its_default_stereotypes.join(space));

  new QLabel(TR("component diagram\nstereotypes : "), grid);
  componentdiagram_list = new LineEdit(grid);
  componentdiagram_list->setTheText(BrowserComponentDiagram::its_default_stereotypes.join(space));

  new QLabel(TR("deployment diagram\nstereotypes : "), grid);
  deploymentdiagram_list = new LineEdit(grid);
  deploymentdiagram_list->setTheText(BrowserDeploymentDiagram::its_default_stereotypes.join(space));

  addTab(grid, TR("Diagrams"));
  
  // activities
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel(TR("activity\nstereotypes : "), grid);
  activity_list = new LineEdit(grid);
  activity_list->setTheText(BrowserActivity::its_default_stereotypes.join(space));

  new QLabel(TR("interruptible activity\nregion stereotypes : "), grid);
  interruptibleactivityregion_list = new LineEdit(grid);
  interruptibleactivityregion_list->setTheText(BrowserInterruptibleActivityRegion::its_default_stereotypes.join(space));

  new QLabel(TR("expansion region\nstereotypes : "), grid);
  expansionregion_list = new LineEdit(grid);
  expansionregion_list->setTheText(BrowserExpansionRegion::its_default_stereotypes.join(space));

  new QLabel(TR("partition\nstereotypes : "), grid);
  activitypartition_list = new LineEdit(grid);
  activitypartition_list->setTheText(BrowserActivityPartition::its_default_stereotypes.join(space));

  new QLabel(TR("parameter\nstereotypes : "), grid);
  parameter_list = new LineEdit(grid);
  parameter_list->setTheText(BrowserParameter::its_default_stereotypes.join(space));

  new QLabel(TR("parameter set\nstereotypes : "), grid);
  parameterset_list = new LineEdit(grid);
  parameterset_list->setTheText(BrowserParameterSet::its_default_stereotypes.join(space));

  new QLabel(TR("activity control node\nstereotypes : "), grid);
  activitynode_list = new LineEdit(grid);
  activitynode_list->setTheText(BrowserActivityNode::its_default_stereotypes.join(space));

  new QLabel(TR("activity action\nstereotypes : "), grid);
  activityaction_list = new LineEdit(grid);
  activityaction_list->setTheText(BrowserActivityAction::its_default_stereotypes.join(space));

  new QLabel(TR("activity object node\nstereotypes : "), grid);
  activityobject_list = new LineEdit(grid);
  activityobject_list->setTheText(BrowserActivityObject::its_default_stereotypes.join(space));

  new QLabel(TR("pin\nstereotypes : "), grid);
  pin_list = new LineEdit(grid);
  pin_list->setTheText(BrowserPin::its_default_stereotypes.join(space));

  new QLabel(TR("expansion node\nstereotypes : "), grid);
  expansionnode_list = new LineEdit(grid);
  expansionnode_list->setTheText(BrowserExpansionNode::its_default_stereotypes.join(space));

  new QLabel(TR("flow\nstereotypes : "), grid);
  flow_list = new LineEdit(grid);
  flow_list->setTheText(BrowserFlow::its_default_stereotypes.join(space));

  addTab(grid, TR("Activities"));
  
  // others
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel(TR("state\nstereotypes : "), grid);
  state_list = new LineEdit(grid);
  state_list->setTheText(BrowserState::its_default_stereotypes.join(space));

  new QLabel(TR("pseudostate\nstereotypes : "), grid);
  pseudostate_list = new LineEdit(grid);
  pseudostate_list->setTheText(BrowserPseudoState::its_default_stereotypes.join(space));

  new QLabel(TR("state action\nstereotypes : "), grid);
  stateaction_list = new LineEdit(grid);
  stateaction_list->setTheText(BrowserStateAction::its_default_stereotypes.join(space));

  new QLabel(TR("component\nstereotypes : "), grid);
  component_list = new LineEdit(grid);
  component_list->setTheText(BrowserComponent::its_default_stereotypes.join(space));

  new QLabel(TR("node\nstereotypes : "), grid);
  node_list = new LineEdit(grid);
  node_list->setTheText(BrowserDeploymentNode::its_default_stereotypes.join(space));

  new QLabel(TR("message\nstereotypes : "), grid);
  msg_list = new LineEdit(grid);
  msg_list->setTheText(BrowserSeqDiagram::message_default_stereotypes.join(space));

  addTab(grid, TR("Others"));
}

StereotypesDialog::~StereotypesDialog() {
  previous_size = size();
}

void StereotypesDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

void StereotypesDialog::accept() {
  packagetable->forceUpdateCells();
  classtable->forceUpdateCells();
  usecasetable->forceUpdateCells();
  artifacttable->forceUpdateCells();
  
  const QString space(" ");
  
  BrowserPackage::its_default_stereotypes =
    QStringList::split(space, package_list->theText());
  
  BrowserPackage::relation_default_stereotypes =
    QStringList::split(space, packagetable->text(0, 0));

  BrowserClass::its_default_stereotypes =
    QStringList::split(space, class_list->theText());
  
  BrowserClass::relations_default_stereotypes[UmlGeneralisation] =
    QStringList::split(space, classtable->text(0, 0));
  BrowserClass::relations_default_stereotypes[UmlDirectionalAssociation] =
    QStringList::split(space, classtable->text(1, 0));
  BrowserClass::relations_default_stereotypes[UmlAssociation] =
    QStringList::split(space, classtable->text(2, 0));
  BrowserClass::relations_default_stereotypes[UmlAggregation] =
    QStringList::split(space, classtable->text(3, 0));
  BrowserClass::relations_default_stereotypes[UmlAggregationByValue] =
    QStringList::split(space, classtable->text(4, 0));
  BrowserClass::relations_default_stereotypes[UmlDirectionalAggregation] =
    QStringList::split(space, classtable->text(5, 0));
  BrowserClass::relations_default_stereotypes[UmlDirectionalAggregationByValue] =
    QStringList::split(space, classtable->text(6, 0));
  BrowserClass::relations_default_stereotypes[UmlDependency] =
    QStringList::split(space, classtable->text(7, 0));
  BrowserClass::relations_default_stereotypes[UmlRealize] =
    QStringList::split(space, classtable->text(8, 0));

  BrowserUseCase::its_default_stereotypes =
    QStringList::split(space, usecase_list->theText());
  
  BrowserUseCase::relations_default_stereotypes[UmlGeneralisation] =
    QStringList::split(space, usecasetable->text(0, 0));
  BrowserUseCase::relations_default_stereotypes[UmlAssociation] =
    QStringList::split(space, usecasetable->text(1, 0));
  BrowserUseCase::relations_default_stereotypes[UmlDependency] =
    QStringList::split(space, usecasetable->text(2, 0));
  
  BrowserArtifact::its_default_stereotypes =
    QStringList::split(space, artifact_list->theText());
  
  BrowserArtifact::relation_default_stereotypes =
    QStringList::split(space, artifacttable->text(0, 0));

  BrowserAttribute::its_default_stereotypes =
    QStringList::split(space, attribute_list->theText());
  
  BrowserOperation::its_default_stereotypes =
    QStringList::split(space, operation_list->theText());
  
  BrowserState::its_default_stereotypes =
    QStringList::split(space, state_list->theText());
  
  BrowserPseudoState::its_default_stereotypes =
    QStringList::split(space, pseudostate_list->theText());
  
  BrowserStateAction::its_default_stereotypes =
    QStringList::split(space, stateaction_list->theText());
  
  BrowserActivity::its_default_stereotypes =
    QStringList::split(space, activity_list->theText());
  
  BrowserInterruptibleActivityRegion::its_default_stereotypes =
    QStringList::split(space, interruptibleactivityregion_list->theText());
  
  BrowserExpansionRegion::its_default_stereotypes =
    QStringList::split(space, expansionregion_list->theText());
  
  BrowserActivityPartition::its_default_stereotypes =
    QStringList::split(space, activitypartition_list->theText());
  
  BrowserParameter::its_default_stereotypes =
    QStringList::split(space, parameter_list->theText());
  
  BrowserParameterSet::its_default_stereotypes =
    QStringList::split(space, parameterset_list->theText());
  
  BrowserActivityNode::its_default_stereotypes =
    QStringList::split(space, activitynode_list->theText());
  
  BrowserActivityAction::its_default_stereotypes =
    QStringList::split(space, activityaction_list->theText());
  
 BrowserActivityObject::its_default_stereotypes =
    QStringList::split(space, activityobject_list->theText());
  
  BrowserPin::its_default_stereotypes =
    QStringList::split(space, pin_list->theText());
  
  BrowserExpansionNode::its_default_stereotypes =
    QStringList::split(space, expansionnode_list->theText());
  
  BrowserFlow::its_default_stereotypes =
    QStringList::split(space, flow_list->theText());
  
  BrowserComponent::its_default_stereotypes =
    QStringList::split(space, component_list->theText());
  
  BrowserDeploymentNode::its_default_stereotypes =
    QStringList::split(space, node_list->theText());
  
  BrowserSeqDiagram::message_default_stereotypes =
    QStringList::split(space, msg_list->theText());
  
  BrowserClassView::its_default_stereotypes =
    QStringList::split(space, classview_list->theText());
  
  BrowserUseCaseView::its_default_stereotypes =
    QStringList::split(space, usecaseview_list->theText());
  
  BrowserComponentView::its_default_stereotypes =
    QStringList::split(space, componentview_list->theText());
  
  BrowserDeploymentView::its_default_stereotypes =
    QStringList::split(space, deploymentview_list->theText());
  
  BrowserClassDiagram::its_default_stereotypes =
    QStringList::split(space, classdiagram_list->theText());
  
  BrowserObjectDiagram::its_default_stereotypes =
    QStringList::split(space, objectdiagram_list->theText());
  
  BrowserColDiagram::its_default_stereotypes =
    QStringList::split(space, coldiagram_list->theText());
  
  BrowserSeqDiagram::its_default_stereotypes =
    QStringList::split(space, seqdiagram_list->theText());
  
  BrowserUseCaseDiagram::its_default_stereotypes =
    QStringList::split(space, usecasediagram_list->theText());
  
  BrowserStateDiagram::its_default_stereotypes =
    QStringList::split(space, statediagram_list->theText());
  
  BrowserActivityDiagram::its_default_stereotypes =
    QStringList::split(space, activitydiagram_list->theText());
  
  BrowserComponentDiagram::its_default_stereotypes =
    QStringList::split(space, componentdiagram_list->theText());
  
  BrowserDeploymentDiagram::its_default_stereotypes =
    QStringList::split(space, deploymentdiagram_list->theText());
  
  QTabDialog::accept();
}
