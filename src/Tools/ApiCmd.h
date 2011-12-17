// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// home   : http://bouml.free.fr
//
// *************************************************************************

#ifndef APICMD_H
#define APICMD_H

enum CmdFamily {
  onInstanceCmd, classGlobalCmd, packageGlobalCmd, miscGlobalCmd,
  umlSettingsCmd, cppSettingsCmd, javaSettingsCmd, idlSettingsCmd,
  phpSettingsCmd, pythonSettingsCmd
};

enum MiscGlobalCmd {
  byeCmd,
  traceCmd,
  messageCmd,
  toolRunningCmd,
  targetCmd,
  allMarkedCmd,
  loadCmd,
  showTraceCmd,
  traceAutoRaiseCmd,
  
  fromIdCmd = 124,
  setRootPermissionCmd = 125,
  setUserIdCmd = 126,
  userIdCmd = 127
};

enum OnInstanceCmd {
  // get commands
  
  parentCmd,
  childrenCmd,
  getDefCmd,
  getUmlDefCmd,
  getCppDefCmd,
  getJavaDefCmd,
  getIdlDefCmd,
  isWritableCmd,
  supportFileCmd,
  
  isOpenCmd,
  setOpenCmd,
  
  assocComponentCmd,
  assocArtifactCmd,
  
  saveInCmd,
  
  formalsCmd,
  actualsCmd,
  cppBodyCmd,
  javaBodyCmd,
  
  applyCmd,
  
  referencedByCmd,
  
  sideCmd,
  
  // api 31
  attributesCmd,
  relationsCmd,
  
  // api 34
  getPhpDefCmd,
  phpBodyCmd,
  
  // api 39
  getPythonDefCmd,
  pythonBodyCmd,
  
  // set commands
  
  firstSetCmd = 50,	// just a mark
    
  createCmd,
  setCoupleValueCmd,
  setDescriptionCmd,
  setNameCmd,
  setStereotypeCmd,
  
  addAssocClassCmd,
  removeAssocClassCmd,
  setAssocClassesCmd,
  
  setAssocDiagramCmd,
  setAssocViewCmd,
  
  addGetOperCmd,
  addSetOperCmd,
  
  setIsClassMemberCmd,
  setVisibilityCmd,
  setCppDeclCmd,
  setCppDefCmd,
  setJavaDeclCmd,
  setIdlDeclCmd,
  setCppVisibilityCmd,
  setIsReadOnlyCmd,
  setDefaultValueCmd,
  setTypeCmd,
  setIsVolatileCmd,
  setIsCppMutableCmd,
  setIsJavaTransientCmd,
  setIdlCaseCmd,
  setIsCppInlineCmd,
  setIsAbstractCmd,
  setBaseTypeCmd,
  setIsCppExternalCmd,
  setIsJavaExternalCmd,
  setIsIdlExternalCmd,
  setIsJavaPublicCmd,
  setIsJavaFinalCmd,
  setIsIdlLocalCmd,
  setIsIdlCustomCmd,
  setSwitchTypeCmd,
  removeFormalCmd,
  addFormalCmd,
  replaceFormalCmd,
  replaceActualCmd,
  setCppHCmd,
  setCppSrcCmd,
  setJavaSrcCmd,
  setIdlSrcCmd,
  addAssocArtifactCmd,
  removeAssocArtifactCmd,
  removeAllAssocArtifactsCmd,
  setReturnTypeCmd,
  setIsCppConstCmd,
  setIsCppFriendCmd,
  setIsCppVirtualCmd,
  setIsIdlTruncatableCmd,
  setCppBodyCmd,
  setCppNameSpecCmd,
  setJavaFinalCmd,
  setJavaSynchronizedCmd,
  setJavaBodyCmd,
  setJavaNameSpecCmd,
  setIsIdlOnewayCmd,
  setIdlNameSpecCmd,
  addParameterCmd,
  removeParameterCmd,
  replaceParameterCmd,
  addExceptionCmd,
  removeExceptionCmd,
  replaceExceptionCmd,
  setCppSrcDirCmd,
  setCppHDirCmd,
  setCppNamespaceCmd,
  setJavaDirCmd,
  setJavaPackageCmd,
  setIdlDirCmd,
  setIdlModuleCmd,
  setRoleNameCmd,
  setMultiplicityCmd,
  setCppVirtualInheritanceCmd,
  setMarkedCmd,
  
  setUmlEntryBehaviorCmd,
  setUmlExitBehaviorCmd,
  setUmlActivityCmd,
  setCppEntryBehaviorCmd,
  setCppExitBehaviorCmd,
  setCppActivityCmd,
  setJavaEntryBehaviorCmd,
  setJavaExitBehaviorCmd,
  setJavaActivityCmd,
  setUmlTriggerCmd,
  setUmlGuardCmd,
  setCppTriggerCmd,
  setCppGuardCmd,
  setJavaGuardCmd,
  setJavaTriggerCmd,
  setDefCmd,
  
  // api 16
  moveAfterCmd,
  
  // api 19
  setJavaAnnotationCmd,
  
  // api 22
  setRelationAssocClassCmd,
  
  // api 25
  setReadOnlyCmd,
  setSingleExecutionCmd,
  setUnmarshallCmd,
  setTimeEventCmd,
  setFlagCmd,
  setUniqueCmd,
  setStreamCmd,
  setInStateCmd,
  setOrderingCmd,

  // api 26
  setCppFrozenCmd,
  setJavaFrozenCmd,
  setIdlFrozenCmd,
  
  // api 30
  setConstraintCmd,
  deleteCmd,
  
  // api 31
  setAttributeCmd,
  addRelationCmd,
  removeRelationCmd,
  
  // api 34
  setPhpDeclCmd,
  setIsPhpExternalCmd,
  setIsPhpFinalCmd,
  setPhpSrcCmd,
  setPhpFinalCmd,
  setPhpBodyCmd,
  setPhpNameSpecCmd,
  setPhpDirCmd,
  setPhpFrozenCmd,
  
  // api 35
  setIsForceBodyGenCmd,
  
  // api 37
  setCppContextualBodyIndentCmd,
  setJavaContextualBodyIndentCmd,
  setPhpContextualBodyIndentCmd,
  
  // api 39
  setPythonDeclCmd,
  setIsPythonExternalCmd,
  setPythonSrcCmd,
  setPythonBodyCmd,
  setPythonNameSpecCmd,
  setPythonDirCmd,
  setPythonFrozenCmd,
  setIsPython2_2Cmd,
  setPythonDecoratorsCmd,
  setPythonPackageCmd,
  setPythonContextualBodyIndentCmd,
  
  // api 40
  applyStereotypeCmd,
  
  // api 42
  setDerivedCmd,
  
  // api 46
  importProjectCmd,
  
  // api 48
  setActiveCmd,
  
  // api 55
  setPhpNamespaceCmd,
    
  setUserCmd = 252, // not in plug-outs managed through bouml
  moveInCmd = 253,  // not in plug-outs managed through bouml

  getIdCmd = 254, // not in old plug-outs managed through bouml
    
  // api 20
  old_deleteCmd = 255 // not in plug-outs managed through bouml
};

enum ClassGlobalCmd {
  findClassCmd
};

enum PackageGlobalCmd {
  findCppNamespaceCmd,
  findJavaPackageCmd,
  findIdlModuleCmd,
  getProjectCmd,
  isProjectModifiedCmd,
  saveProjectCmd,
  
  // Api 40 a faire
  findPythonPackageCmd,
  updateProfileCmd,
  findStereotypeCmd,
  
  // api 55
  findPhpNamespaceCmd
};

enum UmlSettingsCmd {
  getUmlSettingsCmd,
  
  firstSetUmlSettingsCmd = 50,	// just a mark
  // api 16
  setDefaultArtifactDescriptionCmd = firstSetUmlSettingsCmd,
  setDefaultClassDescriptionCmd,
  setDefaultOperationDescriptionCmd,
  setDefaultAttributeDescriptionCmd,
  setDefaultRelationDescriptionCmd,
  // api 28
  setUmlDefaultGetNameCmd,
  setUmlDefaultSetNameCmd
};

enum CppSettingsCmd {
  getCppSettingsCmd,
  getCppUseDefaultsCmd,
  
  setCppUseDefaultsCmd,
  setCppTypeCmd,
  setCppRelationAttributeStereotypeCmd,
  setCppClassStereotypeCmd,
  setCppIncludeCmd,
  setCppHeaderContentCmd,
  setCppRootDirCmd,
  setCppSourceContentCmd,
  setCppHeaderExtensionCmd,
  setCppSourceExtensionCmd,
  setCppIncludeWithPathCmd,
  setCppEnumInCmd,
  setCppEnumOutCmd,
  setCppEnumInOutCmd,
  setCppInCmd,
  setCppOutCmd,
  setCppInOutCmd,
  setCppClassDeclCmd,
  setCppExternalClassDeclCmd,
  setCppStructDeclCmd,
  setCppUnionDeclCmd,
  setCppEnumDeclCmd,
  setCppTypedefDeclCmd,
  setCppAttributeDeclCmd,
  setCppEnumItemDeclCmd,
  setCppRelationDeclCmd,
  setCppOperationDeclCmd,
  setCppOperationDefCmd,
  setCppOperationForceThrowCmd,
  setCppGetVisibilityCmd,
  setCppGetNameCmd,
  setCppIsGetInlineCmd,
  setCppIsGetConstCmd,
  setCppIsGetValueConstCmd,
  setCppSetVisibilityCmd,
  setCppSetNameCmd,
  setCppIsSetInlineCmd,
  setCppIsSetParamConstCmd,
  // api 16
  setCppEnumReturnCmd,
  setCppReturnCmd,
  // api 26
  setCppIsSetParamRefCmd,
  // api 27
  setCppRelativePathCmd,
  setCppForceNamespaceGenCmd,
  // api 29
  setCppRootRelativePathCmd,
  // api 30
  setCppJavadocStyleCmd,
  // api 49
  setCppInlineOperForceInclInHeaderCmd,
  
  // api 54
  setCppDirRevFilterCmd,
  setCppFileRevFilterCmd,
  
  //api 55
  setCppIndentVisibilityCmd
};

enum JavaSettingsCmd {
  getJavaSettingsCmd,
  getJavaUseDefaultsCmd,
  
  setJavaUseDefaultsCmd,
  setJavaTypeCmd,
  setJavaRelationAttributeStereotypeCmd,
  setJavaClassStereotypeCmd,
  setJavaImportCmd,
  setJavaRootdirCmd,
  setJavaSourceContentCmd,
  setJavaSourceExtensionCmd,
  setJavaClassDeclCmd,
  setJavaExternalClassDeclCmd,
  setJavaEnumPatternDeclCmd,
  setJavaInterfaceDeclCmd,
  setJavaAttributeDeclCmd,
  setJavaEnumPatternItemDeclCmd,
  setJavaEnumPatternItemCaseCmd,
  setJavaRelationDeclCmd,
  setJavaOperationDefCmd,
  setJavaGetvisibilityCmd,
  setJavaGetNameCmd,
  setJavaSetNameCmd,
  setJavaIsGetFinalCmd,
  setJavaIsSetFinalCmd,
  setJavaSetVisibilityCmd,
  setJavaIsSetParamFinalCmd,
  
  setJavaEnumDeclCmd,
  setJavaEnumItemDeclCmd,
  
  // api 30
  setJavaJavadocStyleCmd,
    
  // api 40
  setJavaForcePackageGenCmd,
  
  // api 54
  setJavaDirRevFilterCmd,
  setJavaFileRevFilterCmd
};

// api 34
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
  
  // api 38
  setPhpJavadocStyleCmd,
    
  // api 54
  setPhpRequireOnceWithPathCmd,
  setPhpRelativePathCmd,
  setPhpRootRelativePathCmd,
  
  // api 54
  setPhpDirRevFilterCmd,
  setPhpFileRevFilterCmd,
  
  // api 55
  setPhpForceNamespaceGenCmd
};

// api 39
enum PythonSettingsCmd {
  getPythonSettingsCmd,
  getPythonUseDefaultsCmd,
  
  setPythonUseDefaultsCmd,
  setPython22Cmd,
  setPythonIndentStepCmd,
  setPythonRelationAttributeStereotypeCmd,
  setPythonClassStereotypeCmd,
  setPythonImportCmd,
  setPythonRootdirCmd,
  setPythonSourceContentCmd,
  setPythonSourceExtensionCmd,
  setPythonClassDeclCmd,
  setPythonEnumDeclCmd,
  setPythonExternalClassDeclCmd,
  setPythonInterfaceDeclCmd,
  setPythonAttributeDeclCmd,
  setPythonEnumItemDeclCmd,
  setPythonRelationDeclCmd,
  setPythonOperationDefCmd,
  setPythonGetNameCmd,
  setPythonSetNameCmd,
  
  // api 44
  setPythonInitOperationDefCmd,
  
  // api 48
  setPython3OperationCmd,
};

enum IdlSettingsCmd {
  getIdlSettingsCmd,
  getIdlUseDefaultsCmd,
  
  setIdlUseDefaultsCmd,
  setIdlTypeCmd,
  setIdlRelationAttributeStereotypeCmd,
  setIdlClassStereotypeCmd,
  setIdlIncludeCmd,
  setIdlRootDirCmd,
  setIdlSourceContentCmd,
  setIdlSourceExtensionCmd,
  setIdlInterfaceDeclCmd,
  setIdlValuetypeDeclCmd,
  setIdlStructDeclCmd,
  setIdlTypedefDeclCmd,
  setIdlExceptionDeclCmd,
  setIdlUnionDeclCmd,
  setIdlEnumDeclCmd,
  setIdlExternalClassDeclCmd,
  setIdlAttributeDeclCmd,
  setIdlValuetypeAttributeDeclCmd,
  setIdlUnionItemDeclCmd,
  setIdlEnumItemDeclCmd,
  setIdlConstDeclCmd,
  setIdlRelationDeclCmd,
  setIdlValuetypeRelationDeclCmd,
  setIdlUnionRelationDeclCmd,
  setIdlOperationDeclCmd,
  setIdlGetNameCmd,
  setIdlSetNameCmd,
  setIdlIsSetOneWayCmd
};

#endif
