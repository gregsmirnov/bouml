#ifndef _MANIFESTATION_H
#define _MANIFESTATION_H


#include <qcstring.h>
#include <qvaluelist.h>

class UmlArtifact;
class FileIn;
class Token;

class Manifestation {
  public:
    //for QValueList
    Manifestation(){}

    Manifestation(QCString s, UmlArtifact * art, QCString client) : name(s), artifact(art), utilized(client) {}

    static void import(FileIn & in, Token & token, UmlArtifact * artifact);

    static void solveThem();


  protected:
    QCString name;

    UmlArtifact * artifact;

    QCString utilized;

    static QValueList<Manifestation> All;

};

#endif
