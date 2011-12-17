
#include "UmlArtifact.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

#include "UmlCom.h"
#include "Manifestation.h"
#include "UmlNcRelation.h"
void UmlArtifact::solveManifestation(QCString s, QCString idref) {
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end()) {
    if (!FileIn::isBypassedId(idref))
      UmlCom::trace("manifestation : unknown utilized element reference '" + idref + "'<br>");
    return;
  }
    
  UmlItem * target = *it;
    
  if (!FromBouml || (s != "dependency")) {    
    switch (target->kind()) {
    case aClass:
      if (s != "source")
	break;
      else if (stereotype().isEmpty())
	set_Stereotype("source");
      else if (stereotype() != "source")
	break;
      addAssociatedClass((UmlClass *) target);
      return;
    case anArtifact:
      if (!FromBouml)
	break;
      addAssociatedArtifact((UmlArtifact *) target);
      return;
    default:
      break;
    }
  }

  UmlNcRelation * rel = UmlNcRelation::create(aDependency, this, target);
    
  if (rel == 0)
    UmlCom::trace("cannot create manifestation from '" + name() +
		  "' to '" + target->name() + "'");
  else
    rel->set_Stereotype("manifest");
}

void UmlArtifact::init()
{
  declareFct("ownedmember", "uml:Artifact", &importIt);
  declareFct("packagedelement", "uml:Artifact", &importIt);
}

void UmlArtifact::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anArtifact, token, in);
  
  if (where == 0)
    return;
    
  QCString s = token.valueOf("name");
  
  if (s.isEmpty()) {
    static unsigned n = 0;
    
    s.sprintf("anonymous_artifact_%u", ++n);
  }
    
  UmlArtifact * artifact = create((UmlDeploymentView *) where, s);
  
  if (artifact == 0)
    in.error("cannot create artifact '" + s +
	     "' in '" + where->name() + "'");
  
  artifact->addItem(token.xmiId(), in);

  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      if (token.what() == "manifestation")
	Manifestation::import(in, token, artifact);
      else
	artifact->UmlItem::import(in, token);
    }
  }

  artifact->unload(TRUE, FALSE);
}

int UmlArtifact::NumberOf;

