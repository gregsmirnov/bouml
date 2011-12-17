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





#include <stdlib.h>

#include "ColMsg.h"
#include "OperationData.h"
#include "CodMsgSupport.h"
#include "CodObjCanvas.h"
#include "UmlCanvas.h"
#include "BrowserOperation.h"
#include "BrowserClass.h"
#include "CodSelfLinkCanvas.h"
#include "CodDirsCanvas.h"
#include "myio.h"
#include "ToolCom.h"

int ColMsgList::compareItems(QCollection::Item item1,
			     QCollection::Item item2) {
  unsigned r1 = ((ColMsg *) item1)->get_rank();
  unsigned r2 = ((ColMsg *) item2)->get_rank();
  
  if (r1 == r2)
    // in case sort compare an item with itself
    return 0;
  
  return (r1 > r2) ? 1 : -1;
}

//

ColMsg::ColMsg(bool forward, const OperationData * op,
	       const QString & expl_op, const QString & hr, CodMsgSupport * su)
    : is_forward(forward), hierarchical_rank(hr),
      operation(op), explicit_operation(expl_op), in(su) {
}

void ColMsg::delete_it(bool rec, ColMsgList & top) {
  // extract it from all msg
  extract_it(top);
  
  if (rec)
    delete_it_internal();
  else {
    // moves sub messages at toplevel
    while (msgs.first() != 0)
      top.append(msgs.take());
    
    // remove it from (self)link
    in->remove_it(this);
    
    delete this;
  }
}

void ColMsg::delete_it_internal() {
  // delete sub msgs
  while (!msgs.isEmpty())
    msgs.take()->delete_it_internal();
  
  // remove it from (self)link
  in->remove_it(this);
  
  delete this;
}

// warning, some items may have not already updated ranks
// search without considering ranks

bool ColMsg::extract_it(ColMsgList & top) {
  QListIterator<ColMsg> it(top);
  int index;
  
  for (index = 0; it.current(); ++it, index += 1) {
    if (it.current() == this) {
      top.remove(index);
      return TRUE;
    }
    
    if (extract_it(it.current()->msgs))
      return TRUE;

  }
  
  return FALSE;
}

QString ColMsg::def(bool hierarchical, bool full,
		    DrawingLanguage language,
		    ShowContextMode context) {
  QString op;

  if (operation == 0)
    op = explicit_operation;
  else if (!full)
    op = operation->definition(FALSE, FALSE);
  else {
    op = operation->definition(TRUE, language, TRUE, TRUE, context);
    
    if (op.isEmpty())
      return op;
    
    if (operation->get_browser_node()->deletedp())
      op.insert(0, "<deleted> ");
  }
  
  return (op.isEmpty())
    ? op
    : (((hierarchical) ? hierarchical_rank : QString::number(absolute_rank))
       + " " + op);
}

QString ColMsg::next_hierarchical_rank() const {
  int index = hierarchical_rank.findRev('.');
  
  return (index == -1)
    ? QString::number(hierarchical_rank.toUInt() + 1)
    : hierarchical_rank.left(index + 1) +
      QString::number(hierarchical_rank.mid(index + 1).toUInt() + 1);
}

void ColMsg::update_ranks(ColMsgList & msgs)
{
  unsigned rank = 0;
  unsigned hi_rank;
  QListIterator<ColMsg> it(msgs);
  
  for (hi_rank = 1; it.current() != 0; ++it, hi_rank += 1)
    it.current()->update_ranks(rank, QString::number(hi_rank));	// rank is updated
}

void ColMsg::update_ranks(unsigned & r, QString hr) {
  absolute_rank = ++r;
  hierarchical_rank = hr;
  
  hr += '.';
  
  unsigned hi_rank;
  QListIterator<ColMsg> it(msgs);
  
  for (hi_rank = 1; it.current() != 0; ++it, hi_rank += 1)
    it.current()->update_ranks(r, hr + QString::number(hi_rank));
}

void ColMsg::update_rank(unsigned & r) {
  absolute_rank = ++r;
  QListIterator<ColMsg> it(msgs);
  
  for (; it.current() != 0; ++it)
    it.current()->update_rank(r);
}

ColMsg * ColMsg::new_one(const OperationData * d, const QString & e, bool f,
			 const QString & hr, CodMsgSupport * in)
{
  ColMsg * result = new ColMsg(f, d, e, hr, in);
  
  result->place_in_its_support();
  
  return result;
}

void ColMsg::place_in_its_support() {
  unsigned index;
  QListIterator<ColMsg> it(in->get_msgs());
  const char * hr = hierarchical_rank;
  
  for (index = 0; it.current() != 0; ++it, index += 1) {
    if (!gt(hr, it.current()->hierarchical_rank)) {
      in->get_msgs().insert(index, this);
      return;
    }
  }
  
  in->get_msgs().append(this);
}

void ColMsg::place_in(ColMsgList & l) {
  // only the hierarchical_rank is set, not the absolute_rank
  place_in_internal(l);
  
  // lazy implementation where all the objects's rank is updated
  unsigned r = 0;
  QListIterator<ColMsg> it(l);
  
  for (; it.current() != 0; ++it)
    it.current()->update_rank(r);
}

void ColMsg::place_in_internal(ColMsgList & l) {
  ColMsgList * pl = &l;
  int ndots = hierarchical_rank.contains('.');
  
  for (;;) {
    QListIterator<ColMsg> it(*pl);
    ColMsg * m;
    unsigned index = pl->count();
    
    for (it.toLast(), index = pl->count(); (m = it.current()) != 0; --it, index -= 1)
      if (gt(hierarchical_rank, m->hierarchical_rank))
	break;

    if ((m == 0) || // must be the first one
	(m->hierarchical_rank.contains('.') == ndots)) {	// must be placed after m
      pl->insert(index, this);
      return;
    }
    
    // must be placed in a sub tree of m
    pl = &m->msgs;
  }
}

void ColMsg::get_all_in_all_out(ColMsgList & all_in, ColMsgList & all_out, 
				const ColMsgList & msgs)
{
  QListIterator<ColMsg> it(msgs);
  
  for (; it.current() != 0; ++it) {
    ColMsgList & l = (it.current()->is_forward) ? all_in : all_out;
    const QString & hi = it.current()->hierarchical_rank;
    
    unsigned index;
    QListIterator<ColMsg> it2(l);
    
    for (index = 0; it2.current() != 0; index += 1, ++it2)
      if (lt(hi, it2.current()->hierarchical_rank))
	break;

    l.insert(index, it.current());
  }
}

ColMsg * ColMsg::find(const QString & hi, ColMsgList & l)
{
  QListIterator<ColMsg> it(l);
  
  for (; it.current(); ++it)
    if ((*it)->hierarchical_rank == hi)
      return *it;
  
  return 0;
}

// warning hr may be invalid, and tree may be not sorted

ColMsg * ColMsg::find_rec(const QString & hr, ColMsgList & top)
{
  QListIterator<ColMsg> it(top);
  int index;
  
  for (index = 0; it.current(); ++it, index += 1) {
    if (it.current()->hierarchical_rank == hr)
      return it.current();
    
    ColMsg * result = find_rec(hr, it.current()->msgs);
    
    if (result != 0)
      return result;
  }
  
  return 0;
}

unsigned ColMsg::last_rank(const ColMsgList & l)
{
  unsigned result = 0;
  const ColMsgList * pl = &l;
  
  while (!pl->isEmpty()) {
    ColMsg * m = pl->getLast();
  
    result = m->absolute_rank;
    pl = &m->msgs;
  }
  
  return result;
}

bool ColMsg::lt(const char * h1, const char * h2)
{
  for (;;) {
    int i1 = atoi(h1);
    int i2 = atoi(h2);
    
    if (i1 != i2)
      return (i1 < i2);
    
    h1 = strchr(h1, '.');
    h2 = strchr(h2, '.');
    
    if (h1 == 0)
      return (h2 != 0);
    if (h2 == 0)
      return FALSE;
    
    h1 = h1 + 1;
    h2 = h2 + 1;  
  }
}

bool ColMsg::gt(const char * h1, const char * h2)
{
  for (;;) {
    int i1 = atoi(h1);
    int i2 = atoi(h2);
    
    if (i1 != i2)
      return (i1 > i2);
    
    h1 = strchr(h1, '.');
    h2 = strchr(h2, '.');
    
    if (h1 == 0)
      return FALSE;
    if (h2 == 0)
      return TRUE;
    
    h1 = h1 + 1;
    h2 = h2 + 1;  
  }
}


//

void ColMsg::save(QTextStream & st, const ColMsgList & l, bool copy,
		  QString & warning, const QString & diag_name)
{
  nl_indent(st);
  
  bool first_one = TRUE;
  QListIterator<ColMsg> it(l);
  ColMsg * msg;
  
  for (; (msg = it.current()) != 0; ++it) {
    if (copy && !msg->in->copyable())
      continue;
    
    if (first_one) {
      first_one = FALSE;
      st << "msgs";
      indent(+1);
    }
    
    nl_indent(st);
    if (msg->operation != 0) {
      if (msg->operation->deletedp()) {
	BrowserOperation * br_op = 
	  (BrowserOperation *) msg->operation->get_browser_node();
	
	warning += QString("<b>") + diag_name + "</b> reference the <b>" +
	  ((BrowserClass *) br_op->parent())->full_name() +
	    "</b> deleted operation <b>" + msg->operation->definition(TRUE, FALSE) +
	      "</b><br><br>\n";
	if (warning[0] == '!') {
	  st << "msg ";
	  msg->operation->save(st, TRUE, warning);
	}
	else {
	  st << "explicitmsg ";
	  save_string(br_op->get_name(), st);
	}
      }
      else {
	st << "msg ";
	msg->operation->save(st, TRUE, warning);
      }
    }
    else {
      st << "explicitmsg ";
      save_string(msg->explicit_operation, st);
    }
    
    indent(+1);      
    nl_indent(st);
    st << ((msg->is_forward) ? "forward" : "backward")
      << " ranks " << msg->absolute_rank << ' ';
    save_string(msg->hierarchical_rank, st);
    
    st << ' ';
    msg->in->save(st, TRUE, warning);
    
    save(st, msg->msgs, copy, warning, diag_name);
    indent(-1);
  }
  
  if (first_one)
    st << "no_msg";
  else {
    indent(-1);
    nl_indent(st);
    st << "msgsend";
  }
}

void ColMsg::read(char * & st, ColMsgList & l, UmlCanvas * canvas) {
  char * k = read_keyword(st);
    
  do {
    const OperationData * method = 0;
    QString expl;
    
    if (!strcmp(k, "msg")) {
      if ((method = OperationData::read_ref(st)) == 0)
	expl = "<deleted operation>";
    }
    else if (!strcmp(k, "explicitmsg"))
      expl = read_string(st);
    else
      return;
    
    bool forward = FALSE;
    
    k = read_keyword(st);
    
    if (!strcmp(k, "forward"))
      forward = TRUE;
    else if (!strcmp(k, "backward"))
      forward = FALSE;
    else
      wrong_keyword(k, "forward/backward");
    
    read_keyword(st, "ranks");
    unsigned abs = read_unsigned(st);
    QString hi = read_string(st);
    CodMsgSupport * in;
    
    k = read_keyword(st);
    if ((in = CodSelfLinkCanvas::read(st, canvas, k)) == 0)
      in = CodDirsCanvas::read(st, canvas, k);
    
    ColMsg * cm = new ColMsg(forward, method, expl, hi, in);		 
    
    cm->absolute_rank = abs;
    l.append(cm);
    in->get_msgs().append(cm);
    
    k = read_keyword(st);
    if (!strcmp(k, "msgs"))
      read(st, cm->msgs, canvas);
    else if (strcmp(k, "no_msg"))
      wrong_keyword(k, "msgs/no_msg");
    
    k = read_keyword(st);
  } while (strcmp(k, "msgsend"));
}

// for plug out

void ColMsg::get_all(const ColMsgList & l, ColMsgList & r)
{
  QListIterator<ColMsg> it(l);
  
  for (; it.current(); ++it) {
    r.append(it.current());
    get_all(it.current()->msgs, r);
  }
}

void ColMsg::send(ToolCom * com, const ColMsgList & l)
{
  ColMsgList lm;
  
  get_all(l, lm);
  lm.sort();
  
  com->write_unsigned(lm.count());
  
  ColMsg * msg;
  
  for (msg = lm.first(); msg != 0; msg = lm.next()) {
    if (msg->operation == 0) {
      com->write_id(0);
      com->write_string(msg->explicit_operation);
    }
    else if (msg->operation->deletedp()) {
      com->write_id(0);
      com->write_string(msg->operation->get_browser_node()->get_name());
    }
    else 
      msg->operation->get_browser_node()->write_id(com);
  
    CodObjCanvas * from;
    CodObjCanvas * to;
    
    msg->in->get_from_to(from, to, msg->is_forward);
    com->write_unsigned((unsigned) from->get_ident());
    com->write_unsigned((unsigned) to->get_ident());
    
    com->write_unsigned(msg->absolute_rank);
    com->write_string((const char *) msg->hierarchical_rank);
  }
}
