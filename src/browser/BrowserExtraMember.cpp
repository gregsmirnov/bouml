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





#include <qpopupmenu.h> 
#include <qpainter.h>
#include <qcursor.h>

#include "BrowserExtraMember.h"
#include "ExtraMemberData.h"
#include "BrowserClass.h"
#include "ClassData.h"
#include "UmlPixmap.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "ProfiledStereotypes.h"
#include "mu.h"
#include "translate.h"

IdDict<BrowserExtraMember> BrowserExtraMember::all(__FILE__);

BrowserExtraMember::BrowserExtraMember(QString s, BrowserNode * p, ExtraMemberData * d, int id)
    : BrowserNode(s, p), Labeled<BrowserExtraMember>(all, id), def(d) {
}

BrowserExtraMember::BrowserExtraMember(int id)
    : BrowserNode(), Labeled<BrowserExtraMember>(all, id),
      def(new ExtraMemberData) {
  // not yet read
  def->set_browser_node(this);
}

BrowserExtraMember::BrowserExtraMember(const BrowserExtraMember * model, BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserExtraMember>(all, 0) {
  def = new ExtraMemberData(model->def, this);
  comment = model->comment;
}

BrowserExtraMember * BrowserExtraMember::new_one(QString s, BrowserNode * p)
{
  ExtraMemberData * d = new ExtraMemberData();
  BrowserExtraMember * result = new BrowserExtraMember(s, p, d);
  
  d->set_browser_node(result);
  
  return result;
}

BrowserNode * BrowserExtraMember::duplicate(BrowserNode * p, QString name) {
  BrowserNode * result = new BrowserExtraMember(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  result->update_stereotype();
  
  return result;
}

BrowserExtraMember::~BrowserExtraMember() {
  all.remove(get_ident());
  delete def;
}

void BrowserExtraMember::clear(bool old)
{
  all.clear(old);
}

void BrowserExtraMember::update_idmax_for_root()
{
  all.update_idmax_for_root();
}

void BrowserExtraMember::prepare_update_lib() const {
  all.memo_id_oid(get_ident(), original_id);
}
    
void BrowserExtraMember::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

const QPixmap* BrowserExtraMember::pixmap(int) const {
  if (deletedp())
    return DeletedExtraMemberIcon;
  
  const QPixmap * px = ProfiledStereotypes::browserPixmap(def->get_stereotype());

  if (px != 0)
    return px;
  else if (is_marked && text(0).isEmpty())
    return ExtraMemberMarkedIcon;
  else
    return ExtraMemberIcon;
}

void BrowserExtraMember::menu() {
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(def->definition(FALSE, TRUE), m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_edited) {
      m.setWhatsThis(m.insertItem(TR("Edit"), 0),
		     TR("to edit the <i>extra member</i>, \
a double click with the left mouse button does the same thing"));
      if (!is_read_only) {
	m.setWhatsThis(m.insertItem(TR("Duplicate"), 3),
		       TR("to copy the <i>extra member</i> in a new one"));
	m.insertSeparator();
	if (edition_number == 0)
	  m.setWhatsThis(m.insertItem(TR("Delete"), 1),
			 TR("to delete the <i>extra member</i>. \
Note that you can undelete it after"));
      }
    }
    mark_menu(m, TR("the extra member"), 90);
    ProfiledStereotypes::menu(m, this, 99990);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem(TR("Tool"), &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0))
    m.setWhatsThis(m.insertItem(TR("Undelete"), 2),
		   TR("to undelete the <i>extra member</i>"));
  
  exec_menu_choice(m.exec(QCursor::pos()));
}

void BrowserExtraMember::exec_menu_choice(int rank) {
  switch (rank) {
  case 0:
    open(TRUE);
    return;
  case 1:
    delete_it();
    break;
  case 2:
    undelete(FALSE);
    break;
  case 3:
    ((BrowserClass *) parent())->add_extra_member(this);
    return;
  default:
    if (rank >= 99990)
      ProfiledStereotypes::choiceManagement(this, rank - 99990);
    else if (rank >= 100)
      ToolCom::run(Tool::command(rank - 100), this);
    else
      mark_management(rank - 90);
    return;
  }
  ((BrowserNode *) parent())->modified();
  package_modified();
}

void BrowserExtraMember::apply_shortcut(QString s) {
  int choice = -1;

  if (!deletedp()) {
    if (!is_edited) {
      if (s == "Edit")
	choice = 0;
      if (!is_read_only) {
	if (s == "Duplicate")
	  choice = 3;
	if (edition_number == 0)
	  if (s == "Delete")
	    choice = 1;
      }
    }
    mark_shortcut(s, choice, 90);
    if (edition_number == 0)
      Tool::shortcut(s, choice, get_type(), 100);
  }
  else if (!is_read_only && (edition_number == 0))
    if (s == "Undelete")
      choice = 2;
  
  exec_menu_choice(choice);
}

void BrowserExtraMember::open(bool) {
  if (!is_edited)
    def->edit();
}

void BrowserExtraMember::modified() {
  repaint();
  ((BrowserNode *) parent())->modified();
}

UmlCode BrowserExtraMember::get_type() const {
  return UmlExtraMember;
}

QString BrowserExtraMember::get_stype() const {
  return TR("extra member");
}

int BrowserExtraMember::get_identifier() const {
  return get_ident();
}

const char * BrowserExtraMember::help_topic() const  {
  return "extramember";
}

BasicData * BrowserExtraMember::get_data() const {
  return def;
}

void BrowserExtraMember::set_name(const char * s) {
  if (name != s) {    
    name = s;
    update_stereotype();
  }
}

bool BrowserExtraMember::same_name(const QString &, UmlCode) const {
  return FALSE;
}

bool BrowserExtraMember::allow_empty() const {
  return TRUE;
}

QString BrowserExtraMember::full_name(bool rev, bool) const {
  QString s = name;

  if (s.isEmpty())
    s = "<anonymous extra member>";

  return fullname(s, rev);
}

void BrowserExtraMember::member_cpp_def(const QString &, const QString &, 
					QString & s, bool) const {
  const char * df = def->get_cpp_def();
	  
  if (!def->get_cpp_inline() && df[0]) {
    s += df;
    s += "\n";
  }
}

bool BrowserExtraMember::tool_cmd(ToolCom * com, const char * args) {
  ClassData::ToolCmd dummy;
	
  return (def->tool_cmd(com, args, this, comment) ||
	  BrowserNode::tool_cmd(com, args));
}

void BrowserExtraMember::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  if (after == 0)
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

void BrowserExtraMember::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "extra_member_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "extra_member " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning);
    BrowserNode::save(st);
    indent(-1);
    nl_indent(st);
    st << "end";
    
    // for saveAs
    if (!is_from_lib() && !is_api_base())
      is_read_only = FALSE;
  }
}

BrowserExtraMember * BrowserExtraMember::read(char * & st, char * k,
					      BrowserNode * parent)
{
  BrowserExtraMember * result;
  int id;
  
  if (!strcmp(k, "extra_member")) {
    id = read_id(st);
    
    QString s = read_string(st);
    
    k = read_keyword(st);
    
    BrowserExtraMember * already_exist = all[id];

    result = new BrowserExtraMember(s, parent, new ExtraMemberData, id);
    result->def->read(st, k);	// updates k2

    if (already_exist != 0) {
      already_exist->must_change_id(all);
      already_exist->unconsistent_fixed("extra member", result);
    }

    result->is_defined = TRUE;
    
    result->is_read_only = !parent->is_writable() || 
      ((user_id() != 0) && result->is_api_base());
    
    result->def->set_browser_node(result);
    
    result->BrowserNode::read(st, k, id);
    
    if (strcmp(k, "end"))
      wrong_keyword(k, "end");
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserExtraMember::get_it(const char * k, int id)
{
  return (!strcmp(k, "extra_member_ref")) ? all[id] : 0;
}
