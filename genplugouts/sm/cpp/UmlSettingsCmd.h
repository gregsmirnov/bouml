#ifndef _UMLSETTINGSCMD_H
#define _UMLSETTINGSCMD_H


// Internal enum
enum UmlSettingsCmd {
  getUmlSettingsCmd,
  firstSetUmlSettingsCmd= 50,
  setDefaultArtifactDescriptionCmd= firstSetUmlSettingsCmd,
  setDefaultClassDescriptionCmd,
  setDefaultOperationDescriptionCmd,
  setDefaultAttributeDescriptionCmd,
  setDefaultRelationDescriptionCmd,
  setUmlDefaultGetNameCmd,
  setUmlDefaultSetNameCmd

};
#endif
