#ifndef _PHPSETTINGSCMD_H
#define _PHPSETTINGSCMD_H


enum PhpSettingsCmd {
  getPhpSettingsCmd,
  getPhpUseDefaultsCmd,
  setPhpUseDefaultsCmd,
  setPhpClassStereotypeCmd,
  setPhpRootdirCmd,
  setPhpSourceContentCmd,
  setPhpSourceExtensionCmd,
  setPhpClassDeclCmd,
  setPhpEnumDeclCmd,
  setPhpExternalClassDeclCmd,
  setPhpInterfaceDeclCmd,
  setPhpAttributeDeclCmd,
  setPhpEnumItemDeclCmd,
  setPhpRelationDeclCmd,
  setPhpOperationDefCmd,
  setPhpGetNameCmd,
  setPhpSetNameCmd,
  setPhpIsGetFinalCmd,
  setPhpIsSetFinalCmd,
  setPhpIsSetParamFinalCmd,
  setPhpJavadocStyleCmd,
  setPhpRequireOnceWithPathCmd,
  setPhpRelativePathCmd,
  setPhpRootRelativePathCmd,
  setPhpDirRevFilterCmd,
  setPhpFileRevFilterCmd,
  setPhpForceNamespaceGenCmd

};
#endif
