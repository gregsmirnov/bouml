#ifndef _DIALOG_H
#define _DIALOG_H


#include <qcstring.h>

#include "qdialog.h"

class UmlArtifact;
class SmallPushButton;

class QLineEdit;
class QMultiLineEdit;
class QPushButton;
class QComboBox;

class Dialog : public QDialog {
  Q_OBJECT
  public:
    Dialog(UmlArtifact * art, const QCString & path_exe, QCString & pro, QCString & target, QCString & tmplt, QCString & config, QCString & defines, QCString & includepath, QCString & dependpath, QCString & objectsdir, QCString & footer);

  protected:
    UmlArtifact * _art;

    QCString & _pro;

    QCString & _target;

    QCString & _tmplt;

    QCString & _config;

    QCString & _defines;

    QCString & _includepath;

    QCString & _dependpath;

    QCString & _objectsdir;

    QCString & _footer;

    QLineEdit * edpro;

    QPushButton * browsepro;

    QLineEdit * edtarget;

    QPushButton * browsetarget;

    QComboBox * cbtemplate;

    QComboBox * cbconf[5];

    QLineEdit * eddefines;

    QLineEdit * edincludepath;

    SmallPushButton * computeincludepath;

    QLineEdit * eddependpath;

    QLineEdit * edobjectsdir;

    QPushButton * browseobjectsdir;

    QMultiLineEdit * edfooter;

  protected slots:    virtual void polish();

    virtual void accept();
    void browse_pro();
    void browse_target();
    void compute_includepath();
    void browse_objectsdir();
};

#endif
