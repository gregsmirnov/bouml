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
#include <qgrid.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qtextview.h>

#include "RevSettingsDialog.h"
#include "HelpRegexpDialog.h"
#include "GenerationSettingsDialog.h"
#include "DialogUtil.h"
#include "UmlDesktop.h"
#include "translate.h"
    

QSize RevSettingsDialog::previous_size;

RevSettingsDialog::RevSettingsDialog()
    : QDialog(0, "Reverse/Roundtrip Settings dialog", TRUE) {
  setCaption(TR("Reverse/Roundtrip Settings dialog"));

  QVBoxLayout * vbox = new QVBoxLayout(this);
  
  vbox->setMargin(5);
  
  QGrid * grid = new QGrid(3, this);
  QHBox * htab;
  
  vbox->addWidget(grid);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel(grid);
  new QLabel(TR("To specify through regular expression the directories\n"
		"and/or files to bypass during reverse and roundtrip"), grid);
  
  SmallPushButton * help = new SmallPushButton(TR("Help regexp"), grid);
  
  connect(help, SIGNAL(clicked()), this, SLOT(show_regexp_help()));
  
  new QLabel(grid);
  new QLabel(grid);
  new QLabel(grid);
  
  new QLabel(TR("C++ directory : "), grid);
  cpp_dir = new LineEdit(grid);
  cpp_dir->setText(GenerationSettings::cpp_dir_filter.regexp);
  cpp_dir_case_sensitive = new QCheckBox(TR("case sensitive"), grid);
  if (GenerationSettings::cpp_dir_filter.case_sensitive)
    cpp_dir_case_sensitive->setChecked(TRUE);
  
  new QLabel(TR("C++ file : "), grid);
  cpp_file = new LineEdit(grid);
  cpp_file->setText(GenerationSettings::cpp_file_filter.regexp);
  cpp_file_case_sensitive = new QCheckBox(TR("case sensitive"), grid);
  if (GenerationSettings::cpp_file_filter.case_sensitive)
    cpp_file_case_sensitive->setChecked(TRUE);
  
  new QLabel(grid);  
  htab = new QHBox(grid);
  new QLabel(grid);
  
  htab->setMargin(3);
  htab->setStretchFactor(new QLabel(TR("generated/reversed \nheader file extension : "), htab), 0);
  edcpp_h_extension = new QComboBox(TRUE, htab);
  htab->setStretchFactor(edcpp_h_extension, 100);
  edcpp_h_extension->insertItem(GenerationSettings::cpp_h_extension);
  edcpp_h_extension->setCurrentItem(0);
  edcpp_h_extension->insertItem("h");
  edcpp_h_extension->insertItem("hh");
  
  htab->setStretchFactor(new QLabel(TR("    generated/reversed \n    source file extension : "), htab), 0);
  edcpp_src_extension = new QComboBox(TRUE, htab);
  htab->setStretchFactor(edcpp_src_extension, 100);
  edcpp_src_extension->insertItem(GenerationSettings::cpp_src_extension);
  edcpp_src_extension->setCurrentItem(0);
  edcpp_src_extension->insertItem("cpp");
  edcpp_src_extension->insertItem("cc");
  
  new QLabel(grid);
  new QLabel(grid);
  new QLabel(grid);
  
  new QLabel(TR("Java directory : "), grid);
  java_dir = new LineEdit(grid);
  java_dir->setText(GenerationSettings::java_dir_filter.regexp);
  java_dir_case_sensitive = new QCheckBox(TR("case sensitive"), grid);
  if (GenerationSettings::java_dir_filter.case_sensitive)
    java_dir_case_sensitive->setChecked(TRUE);
  
  new QLabel(TR("Java file : "), grid);
  java_file = new LineEdit(grid);
  java_file->setText(GenerationSettings::java_file_filter.regexp);
  java_file_case_sensitive = new QCheckBox(TR("case sensitive"), grid);
  if (GenerationSettings::java_file_filter.case_sensitive)
    java_file_case_sensitive->setChecked(TRUE);
  
  new QLabel(grid);  
  htab = new QHBox(grid);
  new QLabel(grid);
  
  htab->setMargin(3);
  htab->setStretchFactor(new QLabel(TR("generated/reversed file extension : "), htab), 0);
  edjava_extension = new QComboBox(TRUE, htab);
  htab->setStretchFactor(edjava_extension, 100);
  edjava_extension->insertItem(GenerationSettings::java_extension);
  edjava_extension->setCurrentItem(0);
  edjava_extension->insertItem("java");
  
  new QLabel(grid);
  new QLabel(grid);
  new QLabel(grid);
  
  new QLabel(TR("Php directory : "), grid);
  php_dir = new LineEdit(grid);
  php_dir->setText(GenerationSettings::php_dir_filter.regexp);
  php_dir_case_sensitive = new QCheckBox(TR("case sensitive"), grid);
  if (GenerationSettings::php_dir_filter.case_sensitive)
    php_dir_case_sensitive->setChecked(TRUE);
  
  new QLabel(TR("Php file : "), grid);
  php_file = new LineEdit(grid);
  php_file->setText(GenerationSettings::php_file_filter.regexp);
  php_file_case_sensitive = new QCheckBox(TR("case sensitive"), grid);
  if (GenerationSettings::php_file_filter.case_sensitive)
    php_file_case_sensitive->setChecked(TRUE);
  
  new QLabel(grid);  
  htab = new QHBox(grid);
  new QLabel(grid);
  
  htab->setMargin(3);
  htab->setStretchFactor(new QLabel(TR("generated / reversed file extension : "), htab), 0);
  edphp_extension = new QComboBox(TRUE, htab);
  htab->setStretchFactor(edphp_extension, 100);
  edphp_extension->insertItem(GenerationSettings::php_extension);
  edphp_extension->setCurrentItem(0);
  edphp_extension->insertItem("php");
  
  new QLabel(grid);
  new QLabel(grid);
  new QLabel(grid);

  QPushButton * accept = new QPushButton(TR("&OK"), this);
  QPushButton * cancel = new QPushButton(TR("&Cancel"), this);
  QSize bs(cancel->sizeHint());
  
  accept->setDefault(TRUE);  
  accept->setFixedSize(bs);
  cancel->setFixedSize(bs);
  
  QHBoxLayout * hbox = new QHBoxLayout(vbox); 
  
  hbox->addWidget(accept);
  hbox->addWidget(cancel);
    
  connect(accept, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void RevSettingsDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

RevSettingsDialog::~RevSettingsDialog() {
  previous_size = size();
}

void RevSettingsDialog::accept() {
  GenerationSettings::cpp_dir_filter.regexp = cpp_dir->text();
  GenerationSettings::cpp_dir_filter.case_sensitive = cpp_dir_case_sensitive->isChecked();
  GenerationSettings::cpp_file_filter.regexp = cpp_file->text();
  GenerationSettings::cpp_file_filter.case_sensitive = cpp_file_case_sensitive->isChecked();
  GenerationSettings::cpp_h_extension = edcpp_h_extension->currentText().stripWhiteSpace();
  GenerationSettings::cpp_src_extension = edcpp_src_extension->currentText().stripWhiteSpace();
  
  GenerationSettings::java_dir_filter.regexp = java_dir->text();
  GenerationSettings::java_dir_filter.case_sensitive = java_dir_case_sensitive->isChecked();
  GenerationSettings::java_file_filter.regexp = java_file->text();
  GenerationSettings::java_file_filter.case_sensitive = java_file_case_sensitive->isChecked();
  GenerationSettings::java_extension = edjava_extension->currentText().stripWhiteSpace();
  
  GenerationSettings::php_dir_filter.regexp = php_dir->text();
  GenerationSettings::php_dir_filter.case_sensitive = php_dir_case_sensitive->isChecked();
  GenerationSettings::php_file_filter.regexp = php_file->text();
  GenerationSettings::php_file_filter.case_sensitive = php_file_case_sensitive->isChecked();  
  GenerationSettings::php_extension = edphp_extension->currentText().stripWhiteSpace();
  
  QDialog::accept();
}

void RevSettingsDialog::show_regexp_help() {
  HelpRegexpDialog d;
  
  d.raise();
  d.exec();
}

//

QSize HelpRegexpDialog::previous_size;

HelpRegexpDialog::HelpRegexpDialog()
    : QDialog(0, "Help, regular expression", TRUE) {
  setCaption(TR("Help regular expression"));
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  

  QTextView * txt = new QTextView(this);
  
  txt->setText("<p>Extracted from the QT documentation :</p><hr>"
	       "<ul>"
	       "<li><b>c</b> matches the character <i>c</i></li>"
	       "<li><b>?</b> matches any character </li>"
	       "<li><b>*</b> matches any sequence of characters</li>"
	       "<li><b>[]</b> matches a defined set of characters.</li>"
	       "</ul>"
	       "<p>A character set matches a defined set of characters. "
	       "For example, <b>[BSD]</b> matches any of <i>B</i>, <i>D</i> and <i>S</i>. "
	       "Within a character set, the special characters <i>*</i>, <i>?</i>, and <i>[</i> loose their special meanings. "
	       "The following special characters apply:</p>"
	       "<ul>"
	       "<li><b>^</b> When placed first in the list, changes the character set to match any character not in the list. To include the character <i>^</i> itself in the set, escape it or place it anywhere but first. </li>"
	       "<li><b>-</b> Defines a range of characters. To include the character <i>-</i> itself in the set, escape it or place it last. </li>"
	       "<li><b>]</b> Ends the character set definition. To include the character ']' itself in the set, escape it or place it first (but after the negation operator '^', if present)</li>"
	       "</ul>"
	       "<p>Thus, <i>[a-zA-Z0-9.]</i> matches upper and lower case ASCII letters, digits and dot.</p>");
  vbox->add(txt);
  
  QHBoxLayout * hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);

  QPushButton * close = new QPushButton(TR("Close"), this);
  
  close->setDefault(TRUE);
  
  hbox->addWidget(close);
  
  connect(close, SIGNAL(clicked()), this, SLOT(accept()));
  
  // not done in polish else the initial size is too small
  UmlDesktop::setsize_center(this, previous_size, 0.5, 0.5);
}

HelpRegexpDialog::~HelpRegexpDialog() {
  previous_size = size();
}
