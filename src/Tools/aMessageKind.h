#ifndef aMessageKind_H
#define aMessageKind_H

enum UmlMessageKind {
  aSynchronousCall,
  anAsynchronousCall,
  anExplicitReturn,
  anImplicitReturn,
  aDestruction,
  anInteractionUse
};

#endif
