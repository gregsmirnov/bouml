
#ifdef WIN32
#include <stdlib.h>
#endif

#include "Dialog.h"

#include <qlayout.h> 
#include <qlineedit.h>
#include <qlabel.h>
#include <qhbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qdir.h>
Dialog::Dialog(bool ci, QCString & cmd, BooL & rec, BooL & reload)
  : QDialog(0, 0, TRUE), _cmd(cmd), _rec(rec), _reload(reload) {
    QVBoxLayout * vbox = new QVBoxLayout(this);
    QHBox * htab;
    
    vbox->setMargin(5);
    
    // add recursive check box
    
    htab = new QHBox(this);
    htab->setMargin(5);
    vbox->addWidget(htab);
    
    rec_cb = new QCheckBox((ci) ? "Check-in sub packages" : "Check-out sub-packages",
  			 htab);
    
    // add comment
    
    htab = new QHBox(this);
    htab->setMargin(5);
    vbox->addWidget(htab);
    new QLabel("In the command %file will be replaced by the file's name,"
  	     " %dir by the directory where is the file\n"
  	     "%dironly is replaced by the directory without the drive", htab);
    
    // add command edit
    
    htab = new QHBox(this);
    htab->setMargin(5);
    vbox->addWidget(htab);
  
    new QLabel("Command : ", htab);
    cmd_ed = new QLineEdit(htab);
    cmd_ed->setText(_cmd);
  
    // save & load
    
    htab = new QHBox(this);
    htab->setMargin(5);
    vbox->addWidget(htab);
    
    reload_cb = new QCheckBox("Save then reload the project to see the new write permission of the files",
  			    htab);  
    reload_cb->setChecked(TRUE);
    
    // ok & cancel
      htab = new QHBox(this);
    htab->setMargin(5);
    vbox->addWidget(htab);
    
    new QLabel(htab);
    QPushButton * ok = new QPushButton("&Ok", htab);
    new QLabel(htab);
    QPushButton * cancel = new QPushButton("&Cancel", htab);
    new QLabel(htab);
    QSize bs(cancel->sizeHint());
    
    ok->setFixedSize(bs);
    cancel->setFixedSize(bs);
    
    connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
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

void Dialog::accept() {
  if (cmd_ed->text().find("%file") == -1)
    QMessageBox::critical(this, "Error", "the command must contain %file");
  else {
    _cmd = cmd_ed->text();
    _rec = rec_cb->isChecked();
    _reload = reload_cb->isChecked();
    QDialog::accept();
  }
}

