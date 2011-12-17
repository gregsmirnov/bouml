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

#include <stdlib.h>

#include <qapplication.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qgrid.h>
#include <qlineedit.h>
#include <qpushbutton.h> 
#include <qmessagebox.h> 
#include <qcombobox.h>
#include <qtextcodec.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include <qdir.h>
#include <qfiledialog.h>

#include "EnvDialog.h"
#include "BrowserView.h"
#include "HelpDialog.h"
#include "UmlDesktop.h"
#include "UmlWindow.h"
#include "DialogUtil.h"
#include "strutil.h"
#include "mu.h"
#include "myio.h"
#include "translate.h"

void EnvDialog::edit(bool conv, bool noid)
{
  EnvDialog d(conv, noid);
  
  if ((d.exec() != QDialog::Accepted) && conv)
    // user close the window
    exit(-1);
}

EnvDialog::EnvDialog(bool conv, bool noid)
    : QDialog(0, "Environment dialog", TRUE), conversion(conv) {
  setCaption(TR("Environment dialog"));
  
  QVBoxLayout * vbox = new QVBoxLayout(this);
  QHBox * htab;
  QGrid * grid = new QGrid(2, this);
  QPushButton * button;
  QString s;
  
  vbox->addWidget(grid);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel(grid);  
  new QLabel(TR("MANDATORY, choose a value between 2 and 127 not used by an other person working at the same time on a project with you.\n"
	     "To be safe, if possible choose a value not used by an other person even not working on a given project with you"),
	     grid);
  
  new QLabel(TR("Own identifier "), grid);
  htab = new QHBox(grid);
  if (conv)
    s = getenv("BOUML_ID");	// yes !
  else if (! noid)
    s.setNum(user_id());
  ed_id = new QLineEdit(s, htab);
  if (BrowserView::get_project() != 0) {
    ed_id->setEnabled(FALSE);
    new QLabel(TR("   The identifier can't be modified while a project is load"), htab);
  }

  //
  
  new QLabel(grid);  
  new QLabel(TR("\nOptional, to indicate where are the HTML pages of the reference manual. Used by the help (called by the F1 key) to show the\n"
	     "chapter corresponding to the kind of the element selected in the browser"),
	     grid);

  new QLabel(TR("Manual path"), grid);
  htab = new QHBox(grid);
  ed_doc = new QLineEdit(htab);
  if (!conv)
    ed_doc->setText(manual_dir());











  new QLabel(" ", htab);
  button = new QPushButton(TR("Browse"), htab);
  connect(button, SIGNAL(clicked ()), this, SLOT(doc_browse()));

  //
  
  new QLabel(grid);  
  new QLabel(TR("\nOptional, to indicate a web navigator program. If it is not defined the reference manual will be shown with an internal simple viewer"),
	     grid);
  new QLabel(TR("Navigator"), grid);
  htab = new QHBox(grid);
  ed_navigator = new QLineEdit(htab);
  if (!conv)
    ed_navigator->setText(navigator_path());
  new QLabel(" ", htab);
  button = new QPushButton(TR("Browse"), htab);
  connect(button, SIGNAL(clicked ()), this, SLOT(navigator_browse()));

  //
  
  new QLabel(grid);  
  new QLabel(TR("\nOptional, to indicate a template project. This allows to create new projects getting all the template project settings"),
	     grid);
  new QLabel("Template project", grid);
  htab = new QHBox(grid);
  if (conv)
    s = getenv("BOUML_TEMPLATE");	// yes !
  else
    s = template_project();
  ed_template = new QLineEdit(s, htab);
  new QLabel(" ", htab);
  button = new QPushButton(TR("Browse"), htab);
  connect(button, SIGNAL(clicked ()), this, SLOT(template_browse()));

  //
  
  new QLabel(grid);  
  new QLabel(TR("\nOptional, to indicate a text editor (it must creates an own window). Else Bouml will use an internal editor"),
	     grid);
  new QLabel(TR("Editor path "), grid);
  htab = new QHBox(grid);
  if (conv)
    s = getenv("BOUML_EDITOR");	// yes !
  else
    s = editor();
  ed_editor = new QLineEdit(s, htab);
  new QLabel(" ", htab);
  button = new QPushButton(TR("Browse"), htab);
  connect(button, SIGNAL(clicked ()), this, SLOT(editor_browse()));

  //
  
  new QLabel(grid);  
  new QLabel(TR("\nOptional, to choose a language for menus and dialogs (default is English). You may have to select a corresponding character set"),
	     grid);
  new QLabel(TR("Translation file path "), grid);
  htab = new QHBox(grid);
  ed_lang = new QLineEdit(current_lang(), htab);
  new QLabel(" ", htab);
  button = new QPushButton(TR("Browse"), htab);
  connect(button, SIGNAL(clicked ()), this, SLOT(lang_browse()));

  //
  
  new QLabel(grid);  




  new QLabel(TR("\nOptional, to indicate a character set in case you use non ISO_8859-1/latin1 characters. For instance KOI8-R or KOI8-RU for Cyrillic"),
	     grid);

  new QLabel(TR("Character set "), grid);
  cb_charset = new QComboBox(FALSE, grid);
  cb_charset->setAutoCompletion(completion());
  
  QStringList l;
  QTextCodec * co;
  int i = 0;
  
  l.append("");
  while ((co = QTextCodec::codecForIndex(i++)) != 0) {
    QString na = co->name();
    int pos = 0;  
    
    while ((pos = na.find(' ', pos)) != -1)
      na.replace(pos, 1, "_");
    
    if (QTextCodec::codecForName(na) == co)
      l.append(na);
  }
  
  l.sort();
  cb_charset->insertStringList(l);
  if (conv)
    s = getenv("BOUML_CHARSET");	// yes !
  else
    s = codec();
  
  if (!s.isEmpty() && ((i = l.findIndex(s)) != -1))
    cb_charset->setCurrentItem(i);

  //
  
  new QLabel(grid);  
  new QLabel(TR("\nIn case you have a multiple screens configuration the best for you is to ask Bouml to place by default the dialogs in one of these\n"
	     "screens giving the area, else the dialogs will be shown on the center of the virtual screen."),
	     grid);
  new QLabel(TR("Default screen "), grid);
  
  QString x0, y0, x1, y1;
  int top, left, bottom, right;
  
  if (conv) {
    const char * limits = getenv("BOUML_LIMIT_DESKTOP"); // yes !
    
    if ((limits != 0) && 
	(sscanf(limits, "%d,%d,%d,%d", &left, &top, &right, &bottom) == 4)) {
      x0.setNum(left);
      y0.setNum(top);
      x1.setNum(right);
      y1.setNum(bottom);
    }
  }
  else {
    int top, left, bottom, right;
    
    UmlDesktop::limits(left, top, right, bottom);
    x0.setNum(left);
    y0.setNum(top);
    x1.setNum(right);
    y1.setNum(bottom);
  }
  
  htab = new QHBox(grid);
  new QLabel(TR("left: "), htab);
  ed_xmin = new QLineEdit(x0, htab);
  new QLabel(TR("      top: "), htab);
  ed_ymin = new QLineEdit(y0, htab);
  new QLabel(TR("      right: "), htab);
  ed_xmax = new QLineEdit(x1, htab);
  new QLabel(TR("      bottom: "), htab);
  ed_ymax = new QLineEdit(y1, htab);
  
  //
  
  new QLabel(grid);
  htab = new QHBox(grid);
  new QLabel(htab);
  connect(new QPushButton(TR("OK"), htab), SIGNAL(clicked()), this, SLOT(accept()));
  new QLabel(htab);
  if (! conv) {
    connect(new QPushButton(TR("Cancel"), htab), SIGNAL(clicked()), this, SLOT(reject()));
    new QLabel(htab);
  }
}

static void setCoord(int & v, BooL & ok, QLineEdit * ed)
{
  if (ed->text().isEmpty()) {
    v = 0;
    ok = TRUE;
  }
  else {
    bool yes = ok;
    
    v = ed->text().toInt(&yes);
    ok = yes;
  }
}

void EnvDialog::accept() {
  int id;
  
  if (ed_id->text().isEmpty() ||
      (sscanf((const char *) ed_id->text(), "%d", &id) != 1) ||
      (id < 2) ||
      (id > 127)) {
    QMessageBox::critical(this, "Bouml", TR("Invalid identifier, must be an integer between 2 and 127"));
    return;
  }
  
  int l, t, r, b;
  BooL ok_l, ok_t, ok_r, ok_b;
  
  setCoord(l, ok_l, ed_xmin);
  setCoord(t, ok_t, ed_ymin);
  setCoord(r, ok_r, ed_xmax);
  setCoord(b, ok_b, ed_ymax);
  
  if (ok_l && ok_t && ok_r && ok_b) {
    if ((l < 0) || (t < 0) || (r < 0) || (b < 0)) {
      QMessageBox::critical(this, "Bouml",
			    TR("Invalid DEFAULT SCREEN : coordinates can't be negative"));
      return;
    }
    else if ((l != 0) || (t != 0) || (r != 0) || (b != 0)) {
      if (r <= l) {
	QMessageBox::critical(this, "Bouml",
			      TR("Invalid DEFAULT SCREEN : the right must be greater than the left"));
	return;
      }
      
      if (b <= t) {
	QMessageBox::critical(this, "Bouml",
			      TR("Invalid DEFAULT SCREEN : the bottom must be greater than the top"));
	return;
      }
      
      if ((r - l) < 500)
	QMessageBox::warning(this, "Bouml",
			     TR("small DEFAULT SCREEN, width less than 500 points !"));
      
      if ((b - t) < 500)
	QMessageBox::warning(this, "Bouml",
			     TR("small DEFAULT SCREEN, height less than 500 points !"));
    }
  }
  else {
    QMessageBox::critical(this, "Bouml",
			  TR("Invalid DEFAULT SCREEN"
			     "To not specify the desktop all values must be empty or null."
			     "Else the values must be non negative, the right must be greater\n"
			     "than the left, and the bottom must be greater than the top"));
    return;
  }
  
  // note : QFile fp(QDir::home().absFilePath(".boumlrc")) doesn't work
  // if the path contains non latin1 characters, for instance cyrillic !
  QString s = homeDir().absFilePath(".boumlrc");
  FILE * fp = fopen((const char *) s, "w");
  
















  
  if (fp == 0) {
    QMessageBox::critical(this, "Bouml", "cannot write in '" + s + "'");
    if (conversion)
      exit(-1);
    else
      return;
  }
  
  fprintf(fp, "ID %d\n", id);
  
  if (! ed_doc->text().isEmpty())
    fprintf(fp, "MANUAL %s\n", (const char *) ed_doc->text());
  
  if (! ed_navigator->text().isEmpty())
    fprintf(fp, "NAVIGATOR %s\n", (const char *) ed_navigator->text());
  
  if (! ed_template->text().isEmpty())
    fprintf(fp, "TEMPLATE %s\n", (const char *) ed_template->text());
  
  if (! ed_editor->text().isEmpty())
    fprintf(fp, "EDITOR %s\n", (const char *) ed_editor->text());
  
  if (! ed_lang->text().isEmpty())
    fprintf(fp, "LANG %s\n", (const char *) ed_lang->text());
  else
    fputs("NOLANG\n", fp);
  
  if (! cb_charset->currentText().isEmpty())
    fprintf(fp, "CHARSET %s\n", (const char *) cb_charset->currentText());
  
  if (!ed_xmin->text().isEmpty() && 
      !ed_xmax->text().isEmpty() &&
      !ed_ymin->text().isEmpty() &&
      !ed_ymax->text().isEmpty()) {
    fprintf(fp, "DESKTOP %d %d %d %d\n", l, t, r, b);
  }
  
  fclose(fp);
  
  QDialog::accept();
}

void EnvDialog::reject() {
  if (!conversion)
    QDialog::reject();
}

void EnvDialog::doc_browse() {
  QString s =
    QFileDialog::getExistingDirectory(ed_doc->text(), this, 0,
				      TR("documentation directory"));
  
  if (! s.isNull())
    ed_doc->setText(s);
}

void EnvDialog::navigator_browse() {
  QString s =
    QFileDialog::getOpenFileName(ed_navigator->text(),



				 "",

				 this);
  
  if (! s.isNull())
    ed_navigator->setText(s);
}

void EnvDialog::template_browse() {
  QString s =
    QFileDialog::getOpenFileName(ed_template->text(), "*.prj", this);
  
  if (! s.isNull())
    ed_template->setText(s);
}

void EnvDialog::editor_browse() {
  QString s =
    QFileDialog::getOpenFileName(ed_editor->text(),



				 "",

				 this);
  
  if (! s.isNull())
    ed_editor->setText(s);
}

static QString lang_file()
{
  QString lang;
  




















  lang = getenv("LANG");
  if ((lang.length() > 3) && (lang[2] == '_')) {
    if (lang.lower() == "pt_br")
      lang = "pt_br";
    else
      lang = lang.left(2);
  }
  else {
    lang = "";
    
    FILE * fp = popen("locale", "r");
    
    if (fp != 0) {
      char l[64];
      
      while (fgets(l, sizeof(l), fp)) {
	if (!strncmp(l, "LC_", 3)) {
	  char * p = strstr(l+3, "=\"");
	  
	  if (p != 0)
	    lang = p + 2;
	  else if ((p = strchr(l+3, '=')) != 0)
	    lang = p + 1;
	}
	
	if ((lang.length() > 4) && (lang.left(5).lower() == "pt_br")) {
	  lang = lang.left(5);
	  break;
	}
	else if ((lang.length() > 3) && (lang[2] == '_')) {
	  lang = lang.left(2);
	  break;
	}
	else
	  lang = "";
      }
      
      pclose(fp);
    }    
  }

  
  if (! lang.isEmpty()) {
    lang = lang.lower() + ".lang";

    QString p = getenv("BOUML_LIB_DIR");
    
    if (!p.isEmpty())
      lang = p + "/" + lang;

  }
  
  return lang;
}
    
void EnvDialog::lang_browse() {
  QString s =
    QFileDialog::getOpenFileName((ed_lang->text().isEmpty())
				 ? lang_file() : ed_lang->text(),
				 "*.lang", this);
  
  if (! s.isNull())
    ed_lang->setText(s);
}

//

static void propose_lang()
{
  // note : QFile fp(QDir::home().absFilePath(".boumlrc")) doesn't work
  // if the path contains non latin1 characters, for instance cyrillic !
  QString s = homeDir().absFilePath(".boumlrc");
  FILE * fp = fopen((const char *) s, "a");
  
  if (fp != 0) {
    QString lang = lang_file();

    if (! lang.isEmpty()) {
      FILE * fp2 = fopen((const char *) lang, "r");
      
      if (fp2 != 0) {
	fclose(fp2);
	
	fprintf(fp, "LANG %s\n", (const char *) lang);
	fclose(fp);
	set_lang((const char *) lang);
	msg_warning(TR("language"),
		    TR("the used language for menus and dialogs was set automatically,\n"
		       "you can change it through the environment dialog"));
      }
    }
    else
      fclose(fp);
  }
}

int read_boumlrc()
{
  // note : QFile fp(QDir::home().absFilePath(".boumlrc")) doesn't work
  // if the path contains non latin1 characters, for instance cyrillic !
  QString s = QDir::home().absFilePath(".boumlrc");
  FILE * fp = fopen((const char *) s, "r");

















  
  if (fp == 0) {
    QMessageBox::critical(0, "Bouml", TR("cannot read '%1'", s));
    exit(-1);
  }

  set_manual_dir("");
  set_navigator_path("");
  set_template_project("");
  set_editor("");
  set_codec("");
  UmlDesktop::set_limits(0, 0, 0, 0);
        
  int id = -1;
  char line[512];
  bool lang_set = FALSE;
  bool nolang = FALSE;
      
  while (fgets(line, sizeof(line) - 1, fp) != 0) {
    remove_crlf(line);

    if (!strncmp(line, "ID ", 3))
      sscanf(line+3, "%d", &id);
    else if (!strncmp(line, "MANUAL ", 7))
      set_manual_dir(line+7);
    else if (!strncmp(line, "NAVIGATOR ", 10))
      set_navigator_path(line+10);
    else if (!strncmp(line, "TEMPLATE ",9 ))
      set_template_project(line+9);
    else if (!strncmp(line, "EDITOR ", 7))
      set_editor(line+7);
    else if (!strncmp(line, "CHARSET ", 8))
      set_codec(line+8);
    else if (!strncmp(line, "DESKTOP ", 8)) {
      int l, t, r, b;
      
      if (sscanf(line+8, "%d %d %d %d", &l, &t, &r, &b) == 4)
	UmlDesktop::set_limits(l, t, r, b);
    }
    else if (!strncmp(line, "LANG ", 5)) {
      set_lang(line+5);
      lang_set = TRUE;
    }
    else if (!strncmp(line, "NOLANG", 6)) {
      nolang = TRUE;
    }
  }
    
  fclose(fp);
  
  if (! lang_set) {
    set_lang("");
    
    if (! nolang)
      propose_lang();
  }
  
  if (id == -1) {
    QMessageBox::critical(0, "Bouml", TR("Own identifier missing or invalid"));
    EnvDialog::edit(FALSE, TRUE);
    return read_boumlrc();
  }
    
  return id;
}
