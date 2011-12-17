#ifndef _UMLBASEFRAGMENT_H
#define _UMLBASEFRAGMENT_H


#include <qcstring.h>
#include <qvector.h>
#include <qptrdict.h>

class UmlFragmentCompartment;
class UmlDiagram;
class UmlClassInstanceReference;
class UmlFragment;

// this class manages fragments
class UmlBaseFragment {
  public:
    // return the name
    const QCString & name() const {
        return _name;
    }

    // return the compartments, at least one compartment exists
    const QVector<UmlFragmentCompartment> & compartments() const {
        return _compartments;
    }

    // return the fragment compartment containing the fragment,
    // or 0/null
    UmlFragmentCompartment * container() const {
        return _container;
    }

    // return the diagram optionally referenced by the fragment,
    // generally associated to an interaction use
    UmlDiagram * refer() const {
        return _refer;
    }

    // return the form corresponding to the arguments
    // and return value of the interaction use
    QCString arguments() const {
        return _arguments;
    }

    // return the the list of covered instances (life lines)
    const QVector<UmlClassInstanceReference> & covered() const {
        return _covered;
    }


  private:
    UmlFragmentCompartment * _container;

    QVector<UmlFragmentCompartment> _compartments;

    QCString _name;

    int _x;

    int _y;

    int _w;

    int _h;

    UmlDiagram * _refer;

    QCString _arguments;

    QVector<UmlClassInstanceReference> _covered;

    // internal, don't call it
    void read_();


  public:
    //internal, do NOT use it
    
    int vcenter_(int rank) const;

    //internal, do NOT use it
    
    void read_covered_(QPtrDict<UmlClassInstanceReference> & instances);

    // internal
    static UmlFragmentCompartment * get_container_(int x, int y, int w, int h, const QVector<UmlFragment> & fragments);


  private:
    // internal
    static void compute_container_(QVector<UmlFragment> & fragments);


  public:
    int w() const {
        return _w;
    }

    int h() const {
        return _h;
    }

    //  to avoid compiler warning, don't call it
     ~UmlBaseFragment();

  friend class UmlBaseUseCaseDiagramDefinition;
  friend class UmlBaseSequenceDiagramDefinition;
};

#endif
