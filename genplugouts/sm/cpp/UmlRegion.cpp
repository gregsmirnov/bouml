
#include "UmlRegion.h"
#include "UmlClass.h"

#include "UmlState.h"

void UmlRegion::init(UmlClass * mother, QCString path, QCString pretty_path, UmlState *) {
  const QVector<UmlItem> ch = children();
  unsigned index;
  
  pretty_path = pretty_path + "." + name();
  
  for (index = 0; index != ch.count(); index += 1)
    if (ch[index]->kind() == aState)
      ((UmlState *) ch[index])->init(mother, path, pretty_path, 0);

}

