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

#ifndef REVSETTINGSDIALOG_H
#define REVSETTINGSDIALOG_H

#include <qdialog.h>

class QCheckBox;
class QComboBox;
class LineEdit;

class RevSettingsDialog : public QDialog {
  Q_OBJECT
 
  protected:
    LineEdit * cpp_dir;
    LineEdit * cpp_file;
    QCheckBox * cpp_dir_case_sensitive;
    QCheckBox * cpp_file_case_sensitive;
    QComboBox * edcpp_h_extension;
    QComboBox * edcpp_src_extension;
    
    LineEdit * java_dir;
    LineEdit * java_file;
    QCheckBox * java_dir_case_sensitive;
    QCheckBox * java_file_case_sensitive;
    QComboBox * edjava_extension;
    
    LineEdit * php_dir;
    LineEdit * php_file;
    QCheckBox * php_dir_case_sensitive;
    QCheckBox * php_file_case_sensitive;
    QComboBox * edphp_extension;
 
    static QSize previous_size;
  
 public:
    RevSettingsDialog();
    virtual ~RevSettingsDialog();
 
  protected slots:
    virtual void polish();
    void accept();
    void show_regexp_help();
};

#endif
