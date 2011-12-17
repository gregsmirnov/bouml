
#include "UmlActivityControlNodeClasses.h"

QCString UmlInitialActivityNode::sKind() {
  return "initial activity node";
}

QCString UmlFlowFinalActivityNode::sKind() {
  return "flow final";
}

QCString UmlActivityFinalActivityNode::sKind() {
  return "activity final";
}

QCString UmlDecisionActivityNode::sKind() {
  return "decision activity node";
}

QCString UmlMergeActivityNode::sKind() {
  return "merge activity node";
}

QCString UmlForkActivityNode::sKind() {
  return "fork activity node";
}

QCString UmlJoinActivityNode::sKind() {
  return "join activity node";
}

