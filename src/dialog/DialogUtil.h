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

#ifndef DIALOGUTIL_H
#define DIALOGUTIL_H

#include <qpushbutton.h>
#include <qlineedit.h>
#include <qmultilineedit.h>
#include <qmessagebox.h>

#include "AType.h"

class QWidget;
class QPopupMenu;
class QTabDialog;
class BrowserNodeList;
class UmlCanvas;
class BodyDialog;
class BrowserNode;
class KeyValuesTable;

// a push button without magin
class SmallPushButton : public QPushButton {
  public:
    SmallPushButton(const QString & text, QWidget * parent)
      : QPushButton(text, parent) {};
  
    virtual QSize sizeHint() const;
};

// redefine text() to remove non latin1 characters
class MultiLineEdit : public QMultiLineEdit {
  public:
    MultiLineEdit(QWidget * w, const char * name=0) : QMultiLineEdit(w, name) { };
    
    virtual void setText(const QString &);
    virtual QString text() const;
    
    QString stripWhiteSpaceText() const;
    
    void setTheText(const QString & s) {
      // no toUnicode
      QMultiLineEdit::setText(s);
    }
    QString theText() const {
      // no fromUnicode
      return QMultiLineEdit::text();
    }
};

// redefine text() to remove non latin1 characters
class LineEdit : public QLineEdit {
  public:
    LineEdit(QWidget * w) : QLineEdit(w) { };
    LineEdit(const QString & s, QWidget * parent, const char * name=0) : QLineEdit(s, parent, name) {}
    
    virtual void setText(const QString &);
    virtual QString text() const;
    
    void setTheText(const QString & s) {
      // no toUnicode
      QLineEdit::setText(s);
    }
    QString theText() const {
      // no fromUnicode
      return QLineEdit::text();
    }
};

extern void init_font_menu(QPopupMenu & fontsubm, UmlCanvas * the_canvas, 
			   int index);
extern void same_width(QWidget *, QWidget *);
extern void same_width(QWidget *, QWidget *, QWidget *);
extern void same_width(QWidget *, QWidget *, QWidget *, QWidget *);
extern void same_width(QWidget *, QWidget *, QWidget *, QWidget *, QWidget *);

enum EditType { CppEdit, JavaEdit, PhpEdit, PythonEdit, TxtEdit };

typedef void (* post_edit)(QTabDialog *, QString);

extern void edit(const QString &, QString name, void * id, EditType k,
		 QTabDialog * tbl, post_edit pf, QList<BodyDialog> & edits);
extern bool check_edits(QList<BodyDialog> & edits);

extern AType the_type(const QString & t, const QStringList & types,
		      BrowserNodeList & nodes);

extern QString type(const QString & t, const QStringList & types,
		    BrowserNodeList & nodes);

// 'true' name or external name
extern QString get_cpp_name(const BrowserClass * cl,
			    ShowContextMode mode = noContext);
extern QString get_java_name(const BrowserClass * cl,
			    ShowContextMode mode = noContext);
extern QString get_php_name(const BrowserClass * cl,
			    ShowContextMode mode = noContext);
extern QString get_python_name(const BrowserClass * cl,
			    ShowContextMode mode = noContext);
extern QString get_idl_name(const BrowserClass * cl,
			    ShowContextMode mode = noContext);
extern QString get_cpp_name(const AType,
			    ShowContextMode mode = noContext);
extern QString get_java_name(const AType,
			    ShowContextMode mode = noContext);
extern QString get_php_name(const AType,
			    ShowContextMode mode = noContext);
extern QString get_python_name(const AType,
			    ShowContextMode mode = noContext);
extern QString get_idl_name(const AType,
			    ShowContextMode mode = noContext);
    
extern void manage_alias(const BrowserNode * node, const char *& p,
			 QString & s, KeyValuesTable *);

extern int msg_warning(QString caption, QString text,
		       int button0=QMessageBox::Ok,
		       int button1=0, int button2=0);
extern int msg_critical(QString caption, QString text, 
			int button0=QMessageBox::Ok,
			int button1=0, int button2=0);
extern int msg_information(QString caption, QString text,
			   int button0=QMessageBox::Ok,
			   int button1=0, int button2=0);

extern QString editor();
extern void set_editor(QString);

extern void set_completion(bool y);
extern bool completion();

extern void open_dialog(QWidget *);
extern void close_dialog(QWidget *);
extern QWidgetList dialogs();

#endif
