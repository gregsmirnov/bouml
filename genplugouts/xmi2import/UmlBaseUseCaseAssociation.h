#ifndef _UMLBASEUSECASEASSOCIATION_H
#define _UMLBASEUSECASEASSOCIATION_H


#include <qcstring.h>
#include <qptrdict.h>

class UmlUseCaseReference;
class UmlClass;

// this class manages association between use case and actor
class UmlBaseUseCaseAssociation {
  public:
    // return the name
    const QCString & name() const {
        return _name;
    }

    // return the stereotype
    const QCString & stereotype() const {
        return _stereotype;
    }

    // return the use case
    UmlUseCaseReference * useCase() const {
        return _use_case;
    }

    // return the actor
    UmlClass * actor() const {
        return _actor;
    }

    // return true if the actor is a primary actor for the use case 
    bool primary() const {
        return _primary;
    }


  private:
    UmlUseCaseReference * _use_case;

    UmlClass * _actor;

    QCString _name;

    QCString _stereotype;

    bool _primary;

    // internal, don't call it
    void read_(const QPtrDict<UmlUseCaseReference> & useCases);

  friend class UmlBaseUseCaseDiagramDefinition;
};

#endif
