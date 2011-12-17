#ifndef _UMLACTIVITYPARAMETER_H
#define _UMLACTIVITYPARAMETER_H


#include "UmlBaseActivityParameter.h"
#include <qcstring.h>

class FileIn;
class Token;

class UmlActivityParameter : public UmlBaseActivityParameter {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityParameter(void * id, const QCString & s) : UmlBaseActivityParameter(id, s) {
    }

    void readParameter(FileIn & in, Token & token);

    void readParameterNode(FileIn & in, Token & token);


  private:
    void setEffect(QCString s, FileIn & in);

};

#endif
