#ifndef _UMLPACKAGE_H
#define _UMLPACKAGE_H


#include "UmlBasePackage.h"
#include <qcstring.h>
#include <qstring.h>
#include "anItemKind.h"

class FileIn;
class UmlItem;
class Token;
class UmlClassView;
class UmlComponentView;
class UmlDeploymentView;
class UmlUseCaseView;

// This class manages a 'package', notes that the project itself is a package
//
// You can modify it as you want (except the constructor)
class UmlPackage : public UmlBasePackage {
  public:
    UmlPackage(void * id, const QCString & n)
      : UmlBasePackage(id, n),
        _classview(0), _compview(0), _deplview(0), _usecaseview(0) {
     NumberOf += 1; 
    };

    //Import an xmi file under the current package
    virtual void import(QString path);


  private:
    void importHeader(FileIn & in);


  public:
    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, Token & token, FileIn & in);

    //import the package starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    static void appliedProfile(FileIn & in, Token & token, UmlItem * where);

    static void init();

    static int numberOf() { return NumberOf; };

    static int numberOfProfile() { return NumberOfProfile; };

    void applyStereotype(FileIn & in, Token & token);

    static UmlPackage * importProfile(FileIn & in, QCString href);


  private:
    virtual void packageImport(FileIn & in, Token & tk);

    void solveRefs();

    UmlPackage * findProfile(QCString xmiId);


  protected:
    static int NumberOf;

    static int NumberOfProfile;

    UmlClassView * _classview;

    UmlComponentView * _compview;

    UmlDeploymentView * _deplview;

    UmlUseCaseView * _usecaseview;

};

#endif
