#ifndef _CONSTRDESTRCOPYDIALOG_H
#define _CONSTRDESTRCOPYDIALOG_H


#include <qcheckbox.h>

#include "qdialog.h"
class UmlClass;

//The dialog to choose desired operations

class ConstrDestrCopyDialog : public QDialog {
Q_OBJECT
  public:
    ConstrDestrCopyDialog(UmlClass * cl, bool have_constructor, bool have_destructor,
                          bool have_copy, bool have_const_copy,
                          bool have_assignment, bool have_const_assignment);

public slots:

  protected:
    virtual void polish();


  public:
    void accept();


  protected:
    //the class on which the plug-out is applied
    UmlClass * target;

    //checkbox to add constructor
    QCheckBox * add_constr;

    //checkbox to add constructor
    QCheckBox * constr_explicit;

    //checkbox to add destructor
    QCheckBox * add_destr;

    //checkbox to specify that the destructor is virtual
    QCheckBox * virtual_destr;

    //checkbox to add copy constructor (non const param)
    
    QCheckBox * add_copy;

    //checkbox to add copy contructor (const param)
    
    QCheckBox * add_const_copy;

    //checkbox to add operator = (non const param)
    
    QCheckBox * add_assign;

    //checkbox to add operator = (const param)
    QCheckBox * add_const_assign;

};

#endif
