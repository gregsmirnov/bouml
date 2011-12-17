#ifndef _SMALLPUSHBUTTON_H
#define _SMALLPUSHBUTTON_H


#include <qstring.h>

#include <qpushbutton.h>
class SmallPushButton : public QPushButton {
  public:
    SmallPushButton(const QString & text, QWidget * parent) : QPushButton(text, parent) {}

    virtual QSize sizeHint() const;

};

#endif
