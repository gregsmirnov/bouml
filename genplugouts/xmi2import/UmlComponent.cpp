
#include "UmlComponent.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

#include "UmlCom.h"
void UmlComponent::init()
{
  declareFct("ownedmember", "uml:Component", &importIt);
  declareFct("packagedelement", "uml:Component", &importIt);
}

void UmlComponent::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aComponent, token, in);
  
  if (where == 0)
    return;
    
  QCString s = token.valueOf("name");
  
  if (s.isEmpty()) {
    static unsigned n = 0;
    
    s.sprintf("anonymous_component_%u", ++n);
  }
    
  UmlComponent * component = create((UmlComponentView *) where, s);
  
  if (component == 0)
    in.error("cannot create component '" + s +
	     "' in '" + where->name() + "'");
  
  component->addItem(token.xmiId(), in);

  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      QCString ks = token.what();
      
      if ((ks == "interfacerealization") || (ks == "realization"))
	component->manageInterface(token, in);
      else
	component->UmlItem::import(in, token);
    }
  }

  component->unload(TRUE, FALSE);
}

void UmlComponent::manageInterface(Token & token, FileIn & in) {
  QCString idref = token.valueOf("supplier");
  
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  int c = ((const char *) token.what())[0];
  
  if (it != All.end()) {
    if ((*it)->kind() == aClass) {
      if (c == 'i') {
	// provided
	QVector<UmlClass> provided = providedClasses();
	unsigned sz = provided.size();
	
	provided.resize(sz + 1);
	provided.insert(sz, (UmlClass *) *it);
	set_AssociatedClasses(realizingClasses(), provided, requiredClasses());
      }
      else {
	// realization
	QVector<UmlClass> realizing = realizingClasses();
	unsigned sz = realizing.size();
	
	realizing.resize(sz + 1);
	realizing.insert(sz, (UmlClass *) *it);
	set_AssociatedClasses(realizing, providedClasses(), requiredClasses());
      }
    }
  }
  else
    UnresolvedWithContext::add(this, idref, c);
    
  if (! token.closed())
    in.finish(token.what());
}

UmlItem * UmlComponent::container(anItemKind kind, Token & token, FileIn & in) {
  switch (kind) {
  case aComponent:
    return this;
  default:
    return parent()->container(kind, token, in);
  }
}

void UmlComponent::solve(int context, QCString idref) {
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it != All.end()) {
    if ((*it)->kind() == aClass) {
      if (context == 'i') {
	// provided
	QVector<UmlClass> provided = providedClasses();
	unsigned sz = provided.size();
	
	provided.resize(sz + 1);
	provided.insert(sz, (UmlClass *) *it);
	set_AssociatedClasses(realizingClasses(), provided, requiredClasses());
      }
      else {
	// realization
	QVector<UmlClass> realizing = realizingClasses();
	unsigned sz = realizing.size();
	
	realizing.resize(sz + 1);
	realizing.insert(sz, (UmlClass *) *it);
	set_AssociatedClasses(realizing, providedClasses(), requiredClasses());
      }
    }
  }
  else if (!FileIn::isBypassedId(idref))
    UmlCom::trace("component : unknown reference '" + idref + "'<br>");
}

void UmlComponent::generalizeDependRealize(UmlItem * target, FileIn & in, int context, QCString label, QCString constraint) {
  if ((context == 3) && (target->kind() == aClass)) {
    // usage indicate a required interface
    QVector<UmlClass> required = requiredClasses();
    unsigned sz = required.size();
    
    required.resize(sz + 1);
    required.insert(sz, (UmlClass *) target);
    set_AssociatedClasses(realizingClasses(), providedClasses(), required);
  }
  else
    UmlItem::generalizeDependRealize(target, in, context, label, constraint);
}

void UmlComponent::solveGeneralizationDependencyRealization(int context, QCString idref, QCString label, QCString constraint) {
  QMap<QCString, UmlItem *>::Iterator it;
  
  if ((context == 3) &&
      ((it = All.find(idref)) != All.end()) &&
      ((*it)->kind() == aClass)) {
    // usage indicate resuired interface
    QVector<UmlClass> required = requiredClasses();
    unsigned sz = required.size();
    
    required.resize(sz + 1);
    required.insert(sz, (UmlClass *) *it);
    set_AssociatedClasses(realizingClasses(), providedClasses(), required);
  }
  else
    UmlItem::solveGeneralizationDependencyRealization(context, idref, label, constraint);
}

int UmlComponent::NumberOf;

