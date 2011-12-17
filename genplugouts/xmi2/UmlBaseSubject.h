#ifndef _UMLBASESUBJECT_H
#define _UMLBASESUBJECT_H


#include <qcstring.h>
#include <qvector.h>

class UmlSubject;

// this class manages subjects
class UmlBaseSubject {
  public:
    // return the name
    const QCString & name() const {
        return _name;
    }


  private:
    QCString _name;

    int _x;

    int _y;

    int _w;

    int _h;

    // internal, don't call it
    void read_();


  public:
    // internal
    static UmlSubject * get_container_(int x, int y, int w, int h, const QVector<UmlSubject> & subjects);

  friend class UmlBaseUseCaseDiagramDefinition;
};

#endif
