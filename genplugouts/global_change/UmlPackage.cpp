
#include "UmlPackage.h"
#include "Context.h"

void UmlPackage::change(Context & ctx) {
const QVector<UmlItem> ch = children();
unsigned i;

for (i = 0; i != ch.size(); i += 1)
  ch[i]->change(ctx);
    
unload(TRUE, TRUE);

}

