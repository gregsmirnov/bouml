
#include "UmlDeploymentView.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"

UmlItem * UmlDeploymentView::container(anItemKind kind, Token & token, FileIn & in) {
  switch (kind) {
  case aNode:
  case anArtifact:
    return this;
  default:
    return parent()->container(kind, token, in);
  }

}

