
#include "UmlClassView.h"
#include "Context.h"

void UmlClassView::change(Context & ctx) {
  if (ctx.onClass() || ctx.onOperation() ||
      ctx.onAttribute() || ctx.onRelation()) {
    const QVector<UmlItem> ch = children();
    unsigned i;
    
    for (i = 0; i != ch.size(); i += 1)
      ch[i]->change(ctx);
    
    unload(TRUE, TRUE);
  }
}

