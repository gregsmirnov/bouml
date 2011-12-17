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
#include <qtextview.h> 
#include <qlayout.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qtextstream.h>
#include <myio.h>

#include "TraceDialog.h"
#include "UmlDesktop.h"
#include "DialogUtil.h"
#include "translate.h"

TraceDialog * TraceDialog::the;
QTextView * TraceDialog::txt;
bool TraceDialog::AutoRaise;
QString TraceDialog::content;

QSize TraceDialog::previous_size;

TraceDialog::TraceDialog() : QDialog(0, "", FALSE, WDestructiveClose) {
  setCaption(TR("Trace"));
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  

  txt = new QTextView(this);
  txt->setText(content);
  vbox->add(txt);
  
  QHBoxLayout * hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  QPushButton * cl = new QPushButton(TR("Clear"), this);
  QPushButton * save = new QPushButton(TR("Save"), this);
  QPushButton * close = new QPushButton(TR("Close"), this);
  QSize bs(cl->sizeHint());
  
  close->setDefault(TRUE);
  close->setFixedSize(bs);
  save->setFixedSize(bs);
  cl->setFixedSize(bs);
  
  hbox->addWidget(close);
  hbox->addWidget(save);
  hbox->addWidget(cl);
  
  connect(close, SIGNAL(clicked()), this, SLOT(accept()));
  connect(save, SIGNAL(clicked()), this, SLOT(save()));
  connect(cl, SIGNAL(clicked()), this, SLOT(clr()));
  
  // not done in polish else the initial size is too small
  UmlDesktop::setsize_center(this, previous_size, 0.5, 0.5);
  
  open_dialog(this);
}

TraceDialog::~TraceDialog() {
  delete txt;
  txt = 0;
  previous_size = size();
  
  close_dialog(this);
}

void TraceDialog::clr() {
  clear();
}

void TraceDialog::save() {
  QString filename =
    QFileDialog::getSaveFileName(last_used_directory(), "*.html", this);

  if (!filename.isNull()) {
    if (filename.right(5).lower() != ".html")
      filename += ".html";
    
    set_last_used_directory(filename);

    QFile file(filename);
    
    if (file.open(IO_WriteOnly)) {
      QTextStream stream(&file);
      
      stream << "<html>\n" << txt->text() << "\n</html>\n";
      file.close();
    }
  }
}

void TraceDialog::clear()
{
  content = "";
  
  if (AutoRaise && (txt == 0))
    show_it();
  
  if (txt != 0)
    txt->setText("");
}

void TraceDialog::add(const char * s) {
  if (UmlDesktop::nogui())
    fputs(s, stdout);
  else {
    if (AutoRaise && (txt == 0))
      show_it();
    
    if (txt != 0) {
      the->show();
      the->raise();
      txt->append(s);
      txt->update();
    }
    content.append(s);
  }
}

void TraceDialog::show_it()
{
  if (UmlDesktop::nogui())
    return;

  if (txt == 0) {
    the = new TraceDialog();
  }
  else
    the->hide();
  the->show();
  the->raise();
}

void TraceDialog::trace_auto_raise(bool y)
{
  if (!UmlDesktop::nogui())
    AutoRaise = y;
}
