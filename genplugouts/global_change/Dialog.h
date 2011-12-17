#ifndef _DIALOG_H
#define _DIALOG_H


#include <qcstring.h>
#include <qstring.h>

#include <qdialog.h>
#include <qlineedit.h>

class QLineEdit;
class QRadioButton;
class QCheckBox;

class LineEdit : public QLineEdit {
  public:
    LineEdit(QWidget * parent);


  protected:
    bool focusNextPrevChild(bool next);

    void keyPressEvent(QKeyEvent * e);

};

class Dialog : public QDialog {
Q_OBJECT
  public:
    Dialog();


  protected:
    QLineEdit * filter1_le;

    QLineEdit * filter2_le;

    QLineEdit * filter3_le;

    QRadioButton * with1_rb;

    QRadioButton * with2_rb;

    QRadioButton * with3_rb;

    QRadioButton * and12_rb;

    QRadioButton * and23_rb;

    QLineEdit * stereotype_le;

    QRadioButton * any_rb;

    QRadioButton * is_rb;

    QRadioButton * isnot_rb;

    QCheckBox * artifact_cb;

    QCheckBox * class_cb;

    QCheckBox * operation_cb;

    QCheckBox * attribute_cb;

    QCheckBox * relation_cb;

    QCheckBox * cpp_cb;

    QCheckBox * java_cb;

    QCheckBox * php_cb;

    QCheckBox * python_cb;

    QCheckBox * idl_cb;

    QLineEdit * current_le;

    QLineEdit * new_le;

  protected slots:
    void do_replace();

    QCString digest(const QString s);

    virtual void polish();

};

#endif
