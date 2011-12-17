
#include "UmlCom.h"
#include "UmlBaseActivityControlNodeClasses.h"
#include "UmlActivityControlNodeClasses.h"
#include "UmlItem.h"

UmlInitialActivityNode * UmlBaseInitialActivityNode::create(UmlItem * parent, const char * s)
{
    return (UmlInitialActivityNode *) parent->create_(anInitialActivityNode, s);
}

anItemKind UmlBaseInitialActivityNode::kind() {
    return anInitialActivityNode;
}

UmlFlowFinalActivityNode * UmlBaseFlowFinalActivityNode::create(UmlItem * parent, const char * s)
{
    return (UmlFlowFinalActivityNode *) parent->create_(aFlowFinalActivityNode, s);
}

anItemKind UmlBaseFlowFinalActivityNode::kind() {
    return aFlowFinalActivityNode;
}

UmlActivityFinalActivityNode * UmlBaseActivityFinalActivityNode::create(UmlItem * parent, const char * s)
{
    return (UmlActivityFinalActivityNode *) parent->create_(anActivityFinalActivityNode, s);
}

anItemKind UmlBaseActivityFinalActivityNode::kind() {
    return anActivityFinalActivityNode;
}

UmlDecisionActivityNode * UmlBaseDecisionActivityNode::create(UmlItem * parent, const char * s)
{
    return (UmlDecisionActivityNode *) parent->create_(aDecisionActivityNode, s);
}

anItemKind UmlBaseDecisionActivityNode::kind() {
    return aDecisionActivityNode;
}

UmlMergeActivityNode * UmlBaseMergeActivityNode::create(UmlItem * parent, const char * s)
{
    return (UmlMergeActivityNode *) parent->create_(aMergeActivityNode, s);
}

anItemKind UmlBaseMergeActivityNode::kind() {
    return aMergeActivityNode;
}

UmlForkActivityNode * UmlBaseForkActivityNode::create(UmlItem * parent, const char * s)
{
    return (UmlForkActivityNode *) parent->create_(aForkActivityNode, s);
}

anItemKind UmlBaseForkActivityNode::kind() {
    return aForkActivityNode;
}

UmlJoinActivityNode * UmlBaseJoinActivityNode::create(UmlItem * parent, const char * s)
{
    return (UmlJoinActivityNode *) parent->create_(aJoinActivityNode, s);
}

anItemKind UmlBaseJoinActivityNode::kind() {
    return aJoinActivityNode;
}

