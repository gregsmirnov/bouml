
#include "UmlComponent.h"

#include "UmlCom.h"
#include "UmlClass.h"
void UmlComponent::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlCom::message(name());

  UmlItem::html("Component", (UmlDiagram *) associatedDiagram());
  
  if (parent()->kind() == aComponent) {
    fw.write("<p>nested in ");
    parent()->write();
    fw.write("</p>\n");
  }

  const QVector<UmlClass> & provided = providedClasses();

  if (provided.size() != 0) {
    const char * sep = "<p>provided classes : ";
    
    for (unsigned i = 0; i != provided.size(); i += 1) {
      fw.write(sep);
      sep = ", ";
      provided[i]->write();
    }
    fw.write("</p>\n");
  }

  const QVector<UmlClass> & required = requiredClasses();

  if (required.size() != 0) {
    const char * sep = "<p>required classes : ";
    
    for (unsigned i = 0; i != required.size(); i += 1) {
      fw.write(sep);
      sep = ", ";
      required[i]->write();
    }
    fw.write("</p>\n");
  }

  const QVector<UmlClass> & realizing = realizingClasses();

  if (realizing.size() != 0) {
    const char * sep = "<p>realizing classes : ";
    
    for (unsigned i = 0; i != realizing.size(); i += 1) {
      fw.write(sep);
      sep = ", ";
      realizing[i]->write();
    }
    fw.write("</p>\n");
  }
  
  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlComponent::sKind() {
  return "component";
}

