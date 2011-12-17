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





#include <qapplication.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qtextview.h> 
#include <qpushbutton.h>
#include <qtimer.h>

#include "GreetingsDialog.h"
#include "UmlDesktop.h"

GreetingsDialog::GreetingsDialog() : QDialog(0, "", TRUE) {
  setCaption("Greetings");
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  QHBoxLayout * hbox; 
  
  vbox->setMargin(5);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);

  const char * msg = "\
<br>\n\
Greetings,<br>\n\
<br>\n\
Thank you for using <i>Bouml</i>. I develop and improve this tool for<br>\n\
several years and wished that it be useful and enjoyable to use.<br>\n\
<br>\n\
Used in a professional context Bouml allows you to improve and<br>\n\
accelerate your software developments, and thus reduce their<br>\n\
costs, without having to buy a UML tool whose licenses are often<br>\n\
very expensive.<br>\n\
<br>\n\
Bouml is a personal, non-sponsored work, thank you for to help me<br>\n\
to continue with your donations.<br>\n\
<br>\n\
Best regards and happy modeling<br>\n\
<br>\n\
Bruno Pages<i> <i>http://bouml.free.fr</i>\n\
<br>\n<br>\n<br>\n";

  QTextView * tx =
    new QTextView(msg, QString::null, this);
  QFont fnt = tx->font();
  
  fnt.setItalic(TRUE);
  
  QFontMetrics fm(fnt);
  
  tx->setVScrollBarMode(QScrollView::AlwaysOff);
  tx->setHScrollBarMode(QScrollView::AlwaysOff);
  tx->setMinimumSize(fm.size(0, msg));
  hbox->addWidget(tx);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  
  ok = new QPushButton("&OK", this);
  ok->setDefault(TRUE);
  ok->setEnabled(FALSE);
  
  hbox->addWidget(new QLabel(this));
  hbox->addWidget(ok);
  hbox->addWidget(new QLabel(this));
  
  if (UmlDesktop::fixed())
    UmlDesktop::tocenter(this);
  
  connect(ok, SIGNAL(clicked()), this, SLOT(reject()));
  
  QTimer::singleShot(5000, this, SLOT(valid_ok()));
  QApplication::setOverrideCursor(Qt::waitCursor);
}

void GreetingsDialog::valid_ok() {
  ok->setEnabled(TRUE);
  QApplication::restoreOverrideCursor();
}

void GreetingsDialog::closeEvent(QCloseEvent * e) {
  if (ok->isEnabled())
    QDialog::closeEvent(e);
}
