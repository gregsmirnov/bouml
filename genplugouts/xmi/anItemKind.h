#ifndef _ANITEMKIND_H
#define _ANITEMKIND_H


// the king of any item, returned by UmlBaseItem::Kind()
enum anItemKind {
  aRelation,
  anAttribute,
  anOperation,
  anExtraClassMember,
  aClass,
  anUseCase,
  aComponent,
  aNode,
  anArtifact,
  aNcRelation,
  aClassDiagram,
  anUseCaseDiagram,
  aSequenceDiagram,
  aCollaborationDiagram,
  aComponentDiagram,
  aDeploymentDiagram,
  aClassView,
  anUseCaseView,
  aComponentView,
  aDeploymentView,
  aPackage,
  aState,
  aTransition,
  aRegion,
  aStateDiagram,
  aStateAction,
  anInitialPseudoState,
  anEntryPointPseudoState,
  aFinalState,
  aTerminatePseudoState,
  anExitPointPseudoState,
  aDeepHistoryPseudoState,
  aShallowHistoryPseudoState,
  aJunctionPseudoState,
  aChoicePseudoState,
  aForkPseudoState,
  aJoinPseudoState

};
#endif
