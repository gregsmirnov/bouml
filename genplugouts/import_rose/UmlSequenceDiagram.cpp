
#include "UmlSequenceDiagram.h"
#include "File.h"
#include "UmlItem.h"

#include "UmlCom.h"

void UmlSequenceDiagram::import(File & f, UmlItem * parent)
{
  QCString s;

  if (f.read(s) != STRING)
    f.syntaxError(s, "sequence diagram's name");

  QCString id;
  QCString ste;
  QCString doc;
  QDict<QCString> prop;
  QCString s2;
  int k;
  
  do {
    k = f.readDefinitionBeginning(s2, id, ste, doc, prop);
  } while (id.isEmpty());

  UmlSequenceDiagram * x;

  if (scanning) {
    if ((x = UmlBaseSequenceDiagram::create(parent, s)) == 0) {
      UmlCom::trace("<br>cannot create sequence diagram '" + s + "' in " +
		    parent->fullName());
      throw 0;
    }
    newItem(x, id);
    
    if (!doc.isEmpty())
      x->set_Description(doc);
    
    x->setProperties(prop);
  }
  else if ((x = (UmlSequenceDiagram *) findItem(id, aSequenceDiagram)) == 0) {
    UmlCom::trace("<br>unknown sequence diagram '" + s + "' in " +
                  parent->fullName());
    throw 0;
  }

  f.unread(k, s2);
  //!!!!!!!!!!!!
  f.skipBlock();
}

