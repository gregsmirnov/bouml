TEMPLATE	= app
CONFIG		+= qt warn_on release
TARGET		= gpro
DEFINES		= WITHCPP
HEADERS		= Dialog.h
SOURCES		= ClassGlobalCmd.cpp CmdFamily.cpp CppSettings.cpp CppSettingsCmd.cpp Dialog.cpp IdlSettings.cpp IdlSettingsCmd.cpp JavaSettings.cpp JavaSettingsCmd.cpp Main.cpp MiscGlobalCmd.cpp OnInstanceCmd.cpp PackageGlobalCmd.cpp SmallPushButton.cpp UmlAttribute.cpp UmlBaseActualParameter.cpp UmlBaseAttribute.cpp \
		  UmlBaseClass.cpp UmlBaseClassInstance.cpp \
		  UmlBaseClassDiagram.cpp UmlBaseClassItem.cpp UmlBaseClassMember.cpp UmlBaseClassView.cpp UmlBaseCollaborationDiagram.cpp UmlBaseComponent.cpp UmlBaseArtifact.cpp UmlBaseComponentDiagram.cpp UmlBaseComponentView.cpp UmlBaseDeploymentDiagram.cpp UmlBaseDeploymentView.cpp UmlBaseDiagram.cpp UmlBaseExtraClassMember.cpp UmlBaseFormalParameter.cpp UmlBaseItem.cpp UmlBaseNode.cpp UmlBaseOperation.cpp UmlBasePackage.cpp UmlBaseRelation.cpp UmlBaseNcRelation.cpp UmlBaseSequenceDiagram.cpp UmlBaseUseCase.cpp UmlBaseUseCaseDiagram.cpp UmlBaseUseCaseView.cpp UmlBuiltin.cpp \
		UmlClass.cpp UmlClassInstance.cpp \
		UmlClassMember.cpp UmlClassView.cpp UmlCollaborationDiagram.cpp UmlCom.cpp UmlComponent.cpp UmlArtifact.cpp UmlComponentView.cpp UmlExtraClassMember.cpp UmlItem.cpp UmlParameter.cpp UmlSettings.cpp UmlSettingsCmd.cpp UmlStereotype.cpp UmlTypeSpec.cpp aDirection.cpp aRelationKind.cpp aVisibility.cpp anItemKind.cpp
