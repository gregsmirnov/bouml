#ifndef _TABDIALOG_H
#define _TABDIALOG_H


#include <qtabdialog.h>
#include <qstring.h>
#include <qcstring.h>

class QMultiLineEdit;
class UmlUseCase;

//Only for C++
//don't colapse the C++ & Java definition in only one class because this is too artificial
//note : the stereotype doesn't have special meaning for Bouml, I use it just to inform
class TabDialog : public QTabDialog {
  Q_OBJECT

  public:
    TabDialog(UmlUseCase * u);


  protected:
    UmlUseCase * uc;

    QTextCodec * Codec;


  public:
    QMultiLineEdit * summary;

    QMultiLineEdit * context;

    QMultiLineEdit * precond;

    QMultiLineEdit * description;

    QMultiLineEdit * postcond;

    QMultiLineEdit * exceptions;


  protected:
    QPoint desktopCenter;

protected slots:    virtual void polish();


  public:
    void accept();

    void reject();


  private:
    QString toUnicode(const char * str);

    void latinize(QString & s);

    QCString fromUnicode(const QString & s);

};

#endif
