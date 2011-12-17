#ifndef _DIALOG_H
#define _DIALOG_H


#include <qcstring.h>
#include "Language.h"

#include <qdialog.h>

class QLineEdit;
class QPushButton;
class QCheckBox;
class QComboBox;
class QRadioButton;

class Dialog : public QDialog {
  Q_OBJECT
  public:
    Dialog(QCString & path, QCString & encoding, QCString & nomodel, QCString & genview, QCString & uml20, QCString & pk, QCString & vis, QCString & primitivetype, QCString & genextension, QCString & geneclipse, QCString & commentexporter, QCString & linefeed, Language & lang);


  protected:
    QCString & _path;

    QCString & _encoding;

    QCString & _nomodel;

    QCString & _genview;

    //yes => generate pk_ prefix
    QCString & _uml20;

    //yes => generate pk_ prefix
    QCString & _pk;

    //yes => generate vis_ prefix
    QCString & _vis;

    //yes => use primitiveType rather than dataType for int ...
    QCString & _primitivetype;

    QCString & _genextension;

    QCString & _geneclipse;

    QCString & _commentexporter;

    //yes => generate linefeed inside string, else &#10;
    QCString & _linefeed;

    Language & _lang;

    QLineEdit * ed;

    QRadioButton * uml20_rb;

    QRadioButton * uml21_rb;

    QComboBox * encoding_cb;

    QCheckBox * nomodel_cb;

    QCheckBox * genview_cb;

    QCheckBox * pk_cb;

    QCheckBox * vis_cb;

    QCheckBox * primitivetype_cb;

    QCheckBox * genextension_cb;

    QCheckBox * geneclipse_cb;

    QCheckBox * commentexporter_cb;

    QCheckBox * linefeed_cb;

  protected slots:
    virtual void polish();

    void browse();

    void accept_cpp();

    void accept_uml();

    void accept_java();

    void accept();

};

#endif
