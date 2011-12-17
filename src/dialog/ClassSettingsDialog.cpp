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





#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h>

#include "ClassSettingsDialog.h"
#include "Settings.h"
#include "UmlDesktop.h"
#include "translate.h"
    

class ComboVisibility : public QComboBox {
  public:
    ComboVisibility(QWidget * parent, UmlVisibility v, bool nodefault);
};

ComboVisibility::ComboVisibility(QWidget * parent, UmlVisibility v, bool nodefault) 
    : QComboBox(FALSE, parent) {
  // the last value MUST be default
  for (int i = 0; i != (int) UmlDefaultVisibility; i += 1)
    insertItem(stringify((UmlVisibility) i));

  if (!nodefault)
    insertItem(stringify(UmlDefaultVisibility));
  setCurrentItem(v);
}

QSize ClassSettingsDialog::previous_size;

ClassSettingsDialog::ClassSettingsDialog(ClassSettings * se, bool nodefault)
    : QDialog(0, "Class Settings dialog", TRUE), settings(se) {
  setCaption(TR("Class Settings dialog"));
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  QHBoxLayout * hbox;
  QString s;
  
  vbox->setMargin(5);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(new QLabel(TR("default attributes visibility : "), this), 1000);
  cbattribute = new ComboVisibility(this, settings->attribute_visibility, nodefault);
  hbox->addWidget(cbattribute);

  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(new QLabel(TR("default relations visibility : "), this), 1000);
  cbrelation = new ComboVisibility(this, settings->relation_visibility, nodefault);
  hbox->addWidget(cbrelation);

  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(new QLabel(TR("default operations visibility : "), this), 1000);
  cboperation = new ComboVisibility(this, settings->operation_visibility, nodefault);
  hbox->addWidget(cboperation);

  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  QPushButton * accept = new QPushButton(TR("&OK"), this);
  QPushButton * cancel = new QPushButton(TR("&Cancel"), this);
  QSize bs(cancel->sizeHint());
  
  accept->setDefault(TRUE);  
  accept->setFixedSize(bs);
  cancel->setFixedSize(bs);
  
  hbox->addWidget(accept);
  hbox->addWidget(cancel);
    
  connect(accept, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void ClassSettingsDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

ClassSettingsDialog::~ClassSettingsDialog() {
  previous_size = size();
}

void ClassSettingsDialog::accept() {
  settings->attribute_visibility = (UmlVisibility) cbattribute->currentItem();
  settings->relation_visibility = (UmlVisibility) cbrelation->currentItem();
  settings->operation_visibility = (UmlVisibility) cboperation->currentItem();
  
  QDialog::accept();
}
