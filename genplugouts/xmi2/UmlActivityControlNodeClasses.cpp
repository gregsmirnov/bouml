
#include "UmlActivityControlNodeClasses.h"

const char * UmlInitialActivityNode::sKind() const {
  return "InitialNode";
}

void UmlInitialActivityNode::solve_output_flows() {
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1) {
    UmlFlow * f = dynamic_cast<UmlFlow *>(ch[i]);

    if ((f != 0) && (f->control_or_data() == Unset))
      f->set_control_or_data(IsControl);
  }

}

const char * UmlFlowFinalActivityNode::sKind() const {
  return "FlowFinalNode";
}

void UmlFlowFinalActivityNode::solve_output_flows() {
  QListIterator<UmlFlow> it(_incoming_flows);
  
  while (it.current() != 0) {
    if (it.current()->control_or_data() == Unset)
      it.current()->set_control_or_data(IsControl);

    ++it;
  }
}

const char * UmlActivityFinalActivityNode::sKind() const {
  return "FinalNode";
}

void UmlActivityFinalActivityNode::solve_output_flows() {
  QListIterator<UmlFlow> it(_incoming_flows);
  
  while (it.current() != 0) {
    if (it.current()->control_or_data() == Unset)
      it.current()->set_control_or_data(IsControl);

    ++it;
  }

}

const char * UmlDecisionActivityNode::sKind() const {
  return "DecisionNode";
}

void UmlDecisionActivityNode::solve_output_flows() {
  // input and outputs flows must be control/data
  // except a possible decision input being data
  ControlOrData k = Unset;

  // look at output flows
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  unsigned i;
  
  for (i = 0; i != n; i += 1) {
    UmlFlow * f = dynamic_cast<UmlFlow *>(ch[i]);

    if ((f != 0) && ((k = f->control_or_data()) != Unset))
      break;
  }

  if (k == Unset) {
    bool already = FALSE;

    for (;;) {
      // look at input flows
      int ndata = 0;
      bool hascontrol = FALSE;
      bool hasunset = FALSE;
      QListIterator<UmlFlow> it(_incoming_flows);

      while (it.current() != 0) {
	k = it.current()->control_or_data();

	if (k == IsControl) {
	  hascontrol = TRUE;
	  break;
	}
	else if (k == IsData) {
	  if (++ndata == 2)
	    // more than a single decision input
	    break;
	}
	else
	  hasunset = TRUE;

	++it;
      }

      if (hascontrol) {
	// already has k = IsControl;
	break;
      }
      else if ((ndata == 2) || ((ndata == 1) && !hasunset)) {
	k = IsData;
	break;
      }
      else if (already || !hasunset) {
	// not possible to know, force control
	k = IsControl;
	break;
      }

      already = TRUE;

      // solve input flows
      it.toFirst();
      while (it.current() != 0) {
	((UmlActivityNode *) it.current()->parent())->solve_output_flows();
	++it;
      }
    }
  }

  // propagate
  for (i = 0; i != n; i += 1) {
    UmlFlow * f = dynamic_cast<UmlFlow *>(ch[i]);

    if ((f != 0) && (f->control_or_data() == Unset))
      f->set_control_or_data(k);
  }

  QListIterator<UmlFlow> it(_incoming_flows);

  while (it.current() != 0) {
    if (it.current()->control_or_data() == Unset)
      it.current()->set_control_or_data(k);

    ++it;
  }

}

const char * UmlMergeActivityNode::sKind() const {
  return "MergeNode";
}

void UmlMergeActivityNode::solve_output_flows() {
  // input and outputs flows must be control/data
  ControlOrData k = Unset;

  // look at output flows
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  unsigned i;
  
  for (i = 0; i != n; i += 1) {
    UmlFlow * f = dynamic_cast<UmlFlow *>(ch[i]);

    if ((f != 0) && ((k = f->control_or_data()) != Unset))
      break;
  }

  if (k == Unset) {
    bool already = FALSE;

    for (;;) {
      // look at input flows
      bool hasdata = FALSE;
      bool hascontrol = FALSE;
      bool hasunset = FALSE;
      QListIterator<UmlFlow> it(_incoming_flows);

      while (it.current() != 0) {
	k = it.current()->control_or_data();

	if (k == IsControl) {
	  hascontrol = TRUE;
	  break;
	}
	else if (k == IsData) {
	  hasdata = TRUE;
	  break;
	}
	else
	  hasunset = TRUE;

	++it;
      }

      if (hascontrol || hasdata)
	break;
      else if (already || !hasunset) {
	// not possible to know, force control
	k = IsControl;
	break;
      }

      already = TRUE;

      // solve input flows
      it.toFirst();
      while (it.current() != 0) {
	((UmlActivityNode *) it.current()->parent())->solve_output_flows();
	++it;
      }
    }
  }

  // propagate
  for (i = 0; i != n; i += 1) {
    UmlFlow * f = dynamic_cast<UmlFlow *>(ch[i]);

    if ((f != 0) && (f->control_or_data() == Unset))
      f->set_control_or_data(k);
  }

  QListIterator<UmlFlow> it(_incoming_flows);

  while (it.current() != 0) {
    if (it.current()->control_or_data() == Unset)
      it.current()->set_control_or_data(k);

    ++it;
  }

}

const char * UmlForkActivityNode::sKind() const {
  return "ForkNode";
}

void UmlForkActivityNode::solve_output_flows() {
  // input and outputs flows must be control/data

  UmlFlow * f;
  ControlOrData k;
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();

  if (!_incoming_flows.isEmpty()) {
    f = _incoming_flows.getFirst();
    k = f->control_or_data();
  }
  else {
    f = 0;
    k = Unset;
  }

  if (k == Unset) {
    // look at output flows
    for (unsigned i = 0; i != n; i += 1) {
      UmlFlow * f = dynamic_cast<UmlFlow *>(ch[i]);

      if ((f != 0) && ((k = f->control_or_data()) != Unset))
	break;
    }

    if (f != 0) {
      // f is unset
      if (k != Unset) {
	if (f->control_or_data() == Unset)
	  // propagate on input
	  f->set_control_or_data(k);
      }
      else {
	// solve input flow
	((UmlActivityNode *) f->parent())->solve_output_flows();
	k = f->control_or_data();
	if (k == Unset)
	  // not possible to know, force control
	  k = IsControl;
	// propagate on input
	f->set_control_or_data(k);
      }
    }
  }

  // propagate on output
  for (unsigned i = 0; i != n; i += 1) {
    UmlFlow * f = dynamic_cast<UmlFlow *>(ch[i]);

    if ((f != 0) && (f->control_or_data() == Unset))
      f->set_control_or_data(k);
  }
}

const char * UmlJoinActivityNode::sKind() const {
  return "JoinNode";
}

void UmlJoinActivityNode::solve_output_flows() {
  // output is data if at least one input is data
  ControlOrData k = Unset;
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size(); // theo 1

  if (n == 0)
    return;

  UmlFlow * f = (UmlFlow *) ch[0];

  // look at input flows
  QListIterator<UmlFlow> it(_incoming_flows);

  while (it.current() != 0) {
    k = it.current()->control_or_data();

    if (k == Unset) {
      ((UmlActivityNode *) it.current()->parent())->solve_output_flows();
      k = it.current()->control_or_data();
    }

    if (k == IsData)
      break;

    ++it;
  }

  f->set_control_or_data((k == IsData) ? IsData : IsControl);
}

