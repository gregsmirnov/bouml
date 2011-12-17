#ifndef _UMLBASEUSECASEDIAGRAMDEFINITION_H
#define _UMLBASEUSECASEDIAGRAMDEFINITION_H


// to avoid compiler warning
#include "UmlClass.h"
#include "UmlSubject.h"
#include "UmlFragment.h"
#include "UmlFragmentCompartment.h"
#include "UmlUseCaseReference.h"
#include "UmlUseCaseAssociation.h"

#include <qvector.h>

class UmlUseCaseReference;
class UmlClass;
class UmlUseCaseAssociation;
class UmlFragment;
class UmlSubject;

// this class manages use case diagram definition
class UmlBaseUseCaseDiagramDefinition {
  public:
    // return the use cases present in the diagram
    const QVector<UmlUseCaseReference> & useCases() const {
        return _use_cases;
    }

    // return the actors present in the diagram
    const QVector<UmlClass> & actors() const {
        return _actors;
    }

    // return the associations between actor and use case present in the diagram
    const QVector<UmlUseCaseAssociation> & associations() const {
        return _rels;
    }

    // return the fragments present in the diagram
    const QVector<UmlFragment> & fragments() const {
        return _fragments;
    }

    // return the subjects present in the diagram
    const QVector<UmlSubject> & subjects() const {
        return _subjects;
    }


  private:
    QVector<UmlUseCaseReference> _use_cases;

    QVector<UmlClass> _actors;

    QVector<UmlUseCaseAssociation> _rels;

    QVector<UmlFragment> _fragments;

    QVector<UmlSubject> _subjects;

    // internal, don't call it
    void read_();


  public:
    //  to avoid compiler warning, don't call it
     ~UmlBaseUseCaseDiagramDefinition();

  friend class UmlBaseUseCaseDiagram;
};

#endif
