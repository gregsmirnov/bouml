#ifndef _AMESSAGEKIND_H
#define _AMESSAGEKIND_H


// indicate the kind of a message
enum aMessageKind {
  aSynchronousCall,
  anAsynchronousCall,
  anExplicitReturn,
  anImplicitReturn,//  message added to indicate the end of a duration bar
  aDestruction,
  anInteractionUse

};
#endif
