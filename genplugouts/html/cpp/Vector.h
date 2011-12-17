#ifndef _VECTOR_H
#define _VECTOR_H


#include <qvector.h>

class UmlItem;

// The original html generator was written in Java to test the Java 
// version of the API, this class allows to have a source code similar
// in C++
class Vector {
  public:
    bool isEmpty() const {return v.isEmpty(); }

    unsigned int size() const { return v.count(); }

    void addElement(const UmlItem * x);

    UmlItem * elementAt(unsigned int i) const { return v[i]; }

    void setElementAt(const UmlItem * x, unsigned int i) { v.insert(i, x); }

    bool contains(const UmlItem * x) { return (!v.isEmpty() && (v.findRef(x) != -1)); }


  protected:
    QVector<UmlItem> v;

};

#endif
