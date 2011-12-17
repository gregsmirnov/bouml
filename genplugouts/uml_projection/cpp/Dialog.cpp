
#include "Dialog.h"

#ifdef WIN32
#include <stdlib.h>
#endif

#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qhbox.h>
#include <qcheckbox.h>
#include <qdir.h>

Dialog::Dialog(BooL & rec, char & lang) : QDialog(0, 0, TRUE), _rec(rec), _lang(lang) {
  QVBoxLayout * vbox = new QVBoxLayout(this);
  QHBox * htab;
  
  vbox->setMargin(5);
  
  // recursive checkbox
  if (rec) {
    htab = new QHBox(this);
    htab->setMargin(5);
    vbox->addWidget(htab);
    
    rec_cb = new QCheckBox("Do recursively", htab);
  }
  else
    rec_cb = 0;

  // langs + cancel buttons
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  QPushButton * cpp = new QPushButton("&C++", htab);
  new QLabel(htab);
  QPushButton * java = new QPushButton("&Java", htab);
  new QLabel(htab);
  QPushButton * idl = new QPushButton("&Idl", htab);
  new QLabel(htab);
  QPushButton * php = new QPushButton("P&hp", htab);
  new QLabel(htab);
  QPushButton * python = new QPushButton("P&ython", htab);
  new QLabel(htab);
  QPushButton * cancel = new QPushButton("&Cancel", htab);
  new QLabel(htab);
  QSize bs(cancel->sizeHint());
  
  cpp->setFixedSize(bs);
  java->setFixedSize(bs);
  
  connect(cpp, SIGNAL(clicked()), this, SLOT(accept_cpp()));
  connect(java, SIGNAL(clicked()), this, SLOT(accept_java()));
  connect(idl, SIGNAL(clicked()), this, SLOT(accept_idl()));
  connect(php, SIGNAL(clicked()), this, SLOT(accept_php()));
  connect(python, SIGNAL(clicked()), this, SLOT(accept_python()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

  // help
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  new QLabel(htab);
  new QLabel("Warning : reset the declarations/definitions to\n"
	     "their default value from the 'generation settings'", htab);
  new QLabel(htab);
}

void Dialog::polish() {
  QDialog::polish();
  
  // try to read .boumlrc
  // note : QFile fp(QDir::home().absFilePath(".boumlrc")) doesn't work
  // if the path contains non latin1 characters, for instance cyrillic !
  QString s = QDir::home().absFilePath(".boumlrc");
  FILE * fp = fopen((const char *) s, "r");

#ifdef WIN32
  if (fp == 0) {
    QString hd = getenv("USERPROFILE");
    
    if (! hd.isEmpty()) {
      QDir d(hd);
      QString s2 = d.absFilePath(".boumlrc");
      
      fp = fopen((const char *) s2, "r");
    }
  }
#endif
  
  if (fp != 0) {
    char line[512];
      
    while (fgets(line, sizeof(line) - 1, fp) != 0) {
      if (!strncmp(line, "DESKTOP ", 8)) {
	int l, t, r, b;
	
	if (sscanf(line+8, "%d %d %d %d", &l, &t, &r, &b) == 4) {
	  if (!((r == 0) && (t == 0) && (r == 0) && (b == 0)) &&
	      !((r < 0) || (t < 0) || (r < 0) || (b < 0)) &&
	      !((r <= l) || (b <= t))) {
	    int cx = (r + l) /2;
	    int cy = (t + b) / 2;
	    
	    move(x() + cx - (x() + width() / 2), 
		 y() + cy - (y() + height() / 2));
	  }
	}
	  
	break;
      }
    }
    
    fclose(fp);
  }
}

void Dialog::accept_cpp() {
  _lang = 'c';
  
  accept();
}

void Dialog::accept_java() {
  _lang = 'j';
  accept();
}

void Dialog::accept_idl() {
  _lang = 'i';
  accept();
}

void Dialog::accept_php() {
  _lang = 'h';
  accept();
}

void Dialog::accept_python() {
  _lang = 'y';
  accept();
}

void Dialog::accept() {
  _rec = (rec_cb) ? rec_cb->isChecked() : FALSE;
  
  QDialog::accept();
}

