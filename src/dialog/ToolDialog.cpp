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





#include "ToolDialog.h"
#include "Tool.h"
#include "UmlPixmap.h"
#include "UmlDesktop.h"
#include "translate.h"

static const ToolColumnDef Classes[] = {
  { UmlClass, &classButton },
  { UmlOperation, &PublicOperationIcon },
  { UmlAttribute, &PublicAttributeIcon },
  { UmlGeneralisation, &generalisationButton },
  { UmlRealize, &realizeButton },
  { UmlDependency, &dependencyButton },
  { UmlAssociation, &associationButton },
  { UmlDirectionalAssociation, &directionalAssociationButton },
  { UmlAggregation, &aggregationButton },
  { UmlAggregationByValue, &aggregationByValueButton },
  { UmlDirectionalAggregation, &directionalAggregationButton },
  { UmlDirectionalAggregationByValue, &directionalAggregationByValueButton },
  { UmlExtraMember, &ExtraMemberIcon },
  { UmlClassInstance, &classinstanceButton },
};

static const ToolColumnDef States[] = {
  { UmlState, &StateIcon },
  { UmlRegion, &RegionIcon },
  { UmlStateAction, &ActionIcon },
  { InitialPS, &InitialIcon },
  { EntryPointPS, &EntryPointIcon },
  { FinalPS, &FinalIcon },
  { TerminatePS, &TerminateIcon },
  { ExitPointPS, &ExitPointIcon },
  { DeepHistoryPS, &DeepHistoryIcon },
  { ShallowHistoryPS, &ShallowHistoryIcon }, 
  { JunctionPS, &JunctionIcon },
  { ChoicePS, &ChoiceIcon },
  { ForkPS, &ForkIcon },
  { JoinPS, &JoinIcon },
  { UmlTransition, &directionalAssociationButton }
};

static const ToolColumnDef Activities[] = {
  { UmlActivity, &ActivityIcon },
  { UmlInterruptibleActivityRegion, &InterruptibleActivityRegionIcon },
  { UmlExpansionRegion, &ExpansionRegionIcon },
  { UmlActivityObject, &ActionIcon },
  { UmlActivityAction, &ActivityActionIcon },
  { UmlParameter, &ParameterIcon },
  { UmlParameterSet, &ParameterSetIcon },
  { UmlActivityPin, &PinIcon },
  { UmlExpansionNode, &ExpansionNodeIcon },
  { InitialAN, &InitialIcon },
  { ActivityFinalAN, &FinalIcon },
  { FlowFinalAN, &ExitPointIcon },
  { DecisionAN, &DecisionIcon },
  { MergeAN, &MergeIcon },
  { ForkAN, &ForkIcon },
  { JoinAN, &JoinIcon },
  { UmlFlow, &directionalAssociationButton },
};

static const ToolColumnDef Others[] = {
  { UmlProject, 0 }, // Prj
  { UmlPackage, &PackageIcon },
  { UmlUseCaseView, &UseCaseViewIcon },
  { UmlClassView, &ClassViewIcon },
  { UmlComponentView,&ComponentViewIcon },
  { UmlDeploymentView,&DeploymentViewIcon },
  { UmlUseCaseDiagram, &UseCaseDiagramIcon },
  { UmlSeqDiagram, &SeqDiagramIcon },
  { UmlColDiagram, &ColDiagramIcon },
  { UmlClassDiagram, &ClassDiagramIcon },
  { UmlObjectDiagram, &ObjectDiagramIcon },
  { UmlStateDiagram, &StateDiagramIcon },
  { UmlActivityDiagram, &ActivityDiagramIcon },
  { UmlComponentDiagram,&ComponentDiagramIcon },
  { UmlDeploymentDiagram,&DeploymentDiagramIcon },
  { UmlUseCase, &usecaseButton },
  { UmlComponent, &ComponentIcon },
  { UmlDeploymentNode, &DeploymentNodeIcon },
  { UmlArtifact, &ArtifactIcon },
  { UmlInherit, &generalisationButton },
  { UmlDependOn, &dependencyButton },
};

struct Tbl {
  const ToolColumnDef * cd;

  const

  unsigned ncol;
  const char * label;
  ToolTable * tbl;
};

static Tbl Tables[] = {
  { &Classes[0], sizeof(Classes) / sizeof(ToolColumnDef), "Class", 0},
  { &States[0], sizeof(States) / sizeof(ToolColumnDef), "State", 0 },
  { &Activities[0], sizeof(Activities) / sizeof(ToolColumnDef), "Activity", 0 },
  { &Others[0], sizeof(Others) / sizeof(ToolColumnDef), "Others", 0 },
};

//

QSize ToolDialog::previous_size;

ToolDialog::ToolDialog() : QTabDialog(0, "ToolDialog", TRUE, 0) {
  setCaption(TR("Tools dialog"));
  setOkButton(TR("OK"));
  setCancelButton(TR("Cancel"));
  
  for (unsigned i = 0; i != sizeof(Tables)/sizeof(Tbl); i += 1) {
    Tables[i].tbl = new ToolTable(this, Tables[i].cd, Tables[i].ncol);
    addTab(Tables[i].tbl, TR(Tables[i].label));
  }
}

ToolDialog::~ToolDialog() {
  previous_size = size();
}

void ToolDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.9, 0.9);
}

void ToolDialog::accept() {
  unsigned n = 0;
  unsigned i;
  
  for (i = 0; i != sizeof(Tables)/sizeof(Tbl); i += 1)
    n += Tables[i].tbl->ntools();
  
  Tool::set_ntools(n);
  
  n = 0;
  
  for (i = 0; i != sizeof(Tables)/sizeof(Tbl); i += 1)
    n = Tables[i].tbl->update(n);
  
  QTabDialog::accept();
}

// ToolTable

ToolTable::ToolTable(QWidget * parent,
		     const ToolColumnDef * tcd, unsigned ncols)
    : StringTable(1, ncols + 3, parent, FALSE), cd(tcd) {
  unsigned nrows = 1;
  unsigned rank;
  unsigned col;
  
  for (rank = 0; rank != Tool::ntools; rank += 1) {
    ATool & tool = Tool::tools[rank];
    
    for (col = 0; col != ncols; col += 1) {
      if (tool.applicable[cd[col].kind]) {
	nrows += 1;
	break;
      }
    }
  }
  
  setNumRows(nrows);

  horizontalHeader()->setLabel(0, TR("executable"));
  horizontalHeader()->setLabel(1, TR("display"));
  setColumnStretchable(0, FALSE);
  setColumnStretchable(1, FALSE);

  for (col = 0; col != ncols; col += 1) {
    if (cd[col].pixmap == 0) {
      horizontalHeader()->setLabel(col + 2, "Prj");
      adjustColumn(col + 2);
    }
    else
      horizontalHeader()->setLabel(col + 2, QIconSet(**(cd[col].pixmap)), "", 24);
    setColumnStretchable(col, FALSE);
  }
  
  horizontalHeader()->setLabel(ncols + 2, TR("do"));
  adjustColumn(ncols + 2);
  setColumnStretchable(ncols + 2, FALSE);
  
  nrows = 0;
  init_row(0);
  
  for (rank = 0; rank != Tool::ntools; rank += 1) {
    ATool & tool = Tool::tools[rank];
    
    for (col = 0; col != ncols; col += 1) {
      if (tool.applicable[cd[col].kind]) {
	setText(nrows, 0, tool.cmd);
	setText(nrows, 1, tool.display);
	setText(nrows, col + 2, " X");
	while (++col != ncols)
	  if (tool.applicable[cd[col].kind])
	    setText(nrows, col + 2, " X");
	nrows += 1;
	init_row(nrows);
	break;
      }
    }
  }
  
  adjustColumn(0);
  adjustColumn(1);
}
  
void ToolTable::init_row(int row) {
  int index;
  int n = numCols();
  
  setText(row, 0, QString::null);
  setText(row, 1, QString::null);
  
  for (index = 2; index != n; index += 1)
    setItem(row, index,
	    new TableItem(this, QTableItem::Never, ""));
}

void ToolTable::button_pressed(int row, int col, int b, const QPoint & p) {
  if ((col >= 2) && (col != (numCols()-1))) {
    setText(row, col, text(row, col).isEmpty() ? " X" : "");
  }
  else
    StringTable::button_pressed(row,  col, b, p);
}

unsigned ToolTable::ntools() {
  forceUpdateCells();
  
  int n = numRows();
  
  return (text(n - 1, 0).isEmpty() || text(n - 1, 1).isEmpty())
    ? (unsigned) n - 1 : (unsigned) n;
}

unsigned ToolTable::update(unsigned rank) {
  int n = numRows();
  int ncol = numCols();
  
  if (text(n - 1, 0).isEmpty() || text(n - 1, 1).isEmpty())
    n -= 1;
  
  for (int row = 0; row != n; row += 1) {
    ATool & tool = Tool::tools[rank];
    
    tool.cmd = text(row, 0).stripWhiteSpace();
    tool.display = text(row, 1).stripWhiteSpace();
    
    for (int index = 2; index != ncol-1; index += 1)
      tool.applicable[cd[index - 2].kind] =
	!text(row, index).stripWhiteSpace().isEmpty();
    
    rank += 1;
  }  
  
  return rank;
}
