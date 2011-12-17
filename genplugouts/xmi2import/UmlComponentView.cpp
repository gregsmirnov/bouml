
#include "UmlComponentView.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"

UmlItem * UmlComponentView::container(anItemKind kind, Token & token, FileIn & in) {
  switch (kind) {
  case aComponent:
    return this;
  default:
    return parent()->container(kind, token, in);
  }
}

