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

#ifndef BROWSER_NODE_H
#define BROWSER_NODE_H

#include <qtextstream.h> 
#include <qlist.h> 
#include <qlistview.h> 
#include <qstringlist.h>
#include <qdict.h>

#include "UmlEnum.h"
#include "HaveKeyValueData.h"
#include "mystr.h"
#include "Editable.h"
#include "AType.h"

class QDragMoveEvent;
class QPopupMenu;
template <class K> class QPtrDict;

class BrowserView;
class BasicData;
class ClassDiagramSettings;
class UseCaseDiagramSettings;
class SequenceDiagramSettings;
class CollaborationDiagramSettings;
class StateDiagramSettings;
class ActivityDiagramSettings;
class ComponentDiagramSettings;
class DeploymentDiagramSettings;
class ComponentDrawingSettings;
class StateDrawingSettings;
class ActivityDrawingSettings;
class ObjectDiagramSettings;
class SimpleClassDiagramSettings;
class BrowserNodeList;
class ToolCom;
class SaveProgress;

class BrowserNode : public QListViewItem,
  		    public HaveKeyValueData,
  		    public Editable {
  protected:
    MyStr name;
    MyStr comment;
    int original_id;	// from project library
    BooL is_new;	// backup file useless
    BooL is_deleted;
    bool is_modified : 1;
    bool is_read_only : 1;
    bool is_edited : 1;
    bool is_marked : 1;
    bool is_defined : 1;	// to indicate unconsistency due to projectSynchro
    				// pre condition not followed
    
    static bool show_stereotypes;
    static unsigned edition_number;
    static QList<BrowserNode> marked_list;
    static bool popup_menu_active;
    
    static SaveProgress * save_progress;
    static int must_be_saved_counter;
    static int already_saved;
    
    static QString FullPathPrefix;
    static QString FullPathPostfix;
    static QString FullPathDotDot;

    BrowserNode();
    
    void set_parent(QListViewItem * parent);
    virtual bool delete_internal(QString & warning);
    
  public:
    BrowserNode(QString s, BrowserView * parent);
    BrowserNode(QString s, BrowserNode * parent);
    virtual ~BrowserNode();
    
    virtual bool is_undefined() const;
    
    const char * get_name() const { return name; };
    virtual void set_name(const char * s);
    virtual void update_stereotype(bool rec = FALSE);
    
    static bool edition_active() { return edition_number != 0; };
    virtual void edit_start();
    virtual void edit_end();
    virtual bool in_edition() const;
    
    void mark_menu(QPopupMenu & m, const char *, int bias) const;
    void mark_shortcut(QString s, int & index, int bias);
    void mark_management(int choice);
    void toggle_mark();
    bool markedp() const { return is_marked; }
    static const QList<BrowserNode> & marked_nodes() { return marked_list; }
    static void unmark_all();
    
    bool is_from_lib() const { return original_id != 0; }
    virtual void prepare_update_lib() const = 0;
    virtual void support_file(QDict<char> & files, bool add) const;
    
    virtual bool is_writable() const;	// file writable & not api base
    virtual void delete_it();
    bool deletedp() const { return is_deleted; };
    void undelete(bool rec);
    virtual bool undelete(bool rec, QString & warning, QString & renamed);
    void must_be_deleted(); // deleted after load time
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null) = 0;
  
    bool nestedp() const { return ((BrowserNode *) parent())->get_type() == get_type(); };
    virtual const char * get_comment() const;
    virtual void set_comment(const char * c);
    const char * get_stereotype() const;
    virtual QString stereotypes_properties() const;
    bool may_contains(BrowserNode *, bool rec) const;
    virtual bool may_contains_them(const QList<BrowserNode> &,
				   BooL & duplicable) const;
    bool may_contains_it(BrowserNode * bn) const;
    virtual void move(BrowserNode *, BrowserNode * after);
    bool enter_child_name(QString & r, const QString & msg, UmlCode type,
			  bool allow_spaces, bool allow_empty);
    bool enter_child_name(QString & r, const QString & msg, UmlCode type,
			  BrowserNodeList & nodes, BrowserNode ** old,
			  bool allow_spaces, bool allow_empty, 
			  bool exiting = FALSE);
    bool wrong_child_name(const QString & s, UmlCode type,
			  bool allow_spaces, bool allow_empty) const;
    virtual bool allow_spaces() const;
    virtual bool allow_empty() const;
    virtual bool same_name(const QString & s, UmlCode type) const;
    void select_in_browser();
    void edit(const char *, const QStringList & default_stereotypes);
    void children(BrowserNodeList & nodes,
		  UmlCode kind1, UmlCode kind2 = UmlRelations) const;
    
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    QString fullname(bool rev) const;
    QString fullname(QString & s, bool rev) const;
    virtual void menu() = 0;
    virtual void apply_shortcut(QString s) = 0;
    virtual void open(bool force_edit);
    virtual void on_close();
    virtual UmlCode get_type() const = 0;
    virtual QString get_stype() const = 0;
    virtual int get_identifier() const = 0;
    virtual const char * help_topic() const;
    virtual void modified();
    bool modifiedp() { return is_modified; }
    void unmodified() { is_modified = FALSE; }
    bool modifiedp() const { return is_modified; };
    virtual void on_delete();
    virtual BasicData * get_data() const = 0;
    virtual QString drag_key() const;
    virtual QString drag_postfix() const;
    virtual void save(QTextStream &, bool ref, QString & warning) = 0;
    virtual void package_modified();
    virtual void get_classdiagramsettings(ClassDiagramSettings &) const;
    virtual void get_usecasediagramsettings(UseCaseDiagramSettings &) const;
    virtual void get_sequencediagramsettings(SequenceDiagramSettings &) const;
    virtual void get_collaborationdiagramsettings(CollaborationDiagramSettings &) const;
    virtual void get_objectdiagramsettings(ObjectDiagramSettings &) const;
    virtual void get_componentdiagramsettings(ComponentDiagramSettings &) const;
    virtual void get_deploymentdiagramsettings(DeploymentDiagramSettings &) const;
    virtual void get_statediagramsettings(StateDiagramSettings &) const;
    virtual void get_activitydiagramsettings(ActivityDiagramSettings &) const;
    virtual UmlColor get_color(UmlCode) const;
    virtual UmlVisibility get_visibility(UmlCode) const;
    virtual void package_settings(BooL & name_in_tab, ShowContextMode & show_context) const;
    virtual const QStringList & default_stereotypes(UmlCode, const BrowserNode *) const; // non class rel
    virtual BrowserNode * get_associated() const;
    virtual BasicData * add_relation(UmlCode, BrowserNode *);
    virtual QList<BrowserNode> parents() const;
    BrowserNode * get_container(UmlCode) const;
    virtual BrowserNode * container(UmlCode) const; // container for class, state machine and activity
    virtual QString check_inherit(const BrowserNode * parent) const;
    virtual void write_id(ToolCom * com);
    virtual bool tool_cmd(ToolCom * com, const char * args);
    virtual bool api_compatible(unsigned v) const;
    virtual void member_cpp_def(const QString & prefix,
				const QString & prefix_tmplop, 
				QString & s, bool templ) const;
    virtual void referenced_by(QList<BrowserNode> &, bool ondelete = FALSE);
    virtual AType class_association() const;
    virtual const char * constraint() const;
    
    bool save_open_list(QTextStream &);
    void save(QTextStream &) const;
    static void save_progress_closed();
    virtual void init_save_counter();
    void read(char * &, char * & k, int id);
    static BrowserNode * read_any_ref(char * &, char *);
    static void save_stereotypes(QTextStream & st, 
				 QStringList relations_stereotypes[]);
    static void read_stereotypes(char * &,
				 QStringList relations_stereotypes[]);

    static bool toggle_show_stereotypes();
    virtual void iconChanged();
    virtual void paintCell(QPainter * p, const QColorGroup & cg, int column,
			   int width, int alignment);
    
    void unconsistent_fixed(const char * what, BrowserNode * newone);
    void unconsistent_removed(const char * what, BrowserNode * newone);
    static void signal_unconsistencies();
    
    static void pre_load();
    static void post_load(bool light);
    
    virtual void renumber(int phase);

    virtual void DragMoveEvent(QDragMoveEvent * e);
    virtual void DropEvent(QDropEvent * e);
    virtual void DragMoveInsideEvent(QDragMoveEvent * e);
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
    
    // because of a Qt bug on windows
    static bool popupMenuActive() { return popup_menu_active; }
    static void setPopupMenuActive(bool y) { popup_menu_active = y; }
};

inline QString BrowserNode::fullname(bool rev) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);
  
  if (p.isEmpty()) 
    return QString((const char *) name);
  else if (rev)
    return name + (FullPathPrefix + p + FullPathPostfix);
  else
    return p + (FullPathDotDot + name);
}

inline QString BrowserNode::fullname(QString & s, bool rev) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);
  
  if (p.isEmpty()) 
    return s;
  else if (rev)
    return s + (FullPathPrefix + p + FullPathPostfix);
  else
    return p + (FullPathDotDot + s);
}

// a sortable list of BrowserNode

class BrowserNodeList : public QList<BrowserNode> {
  protected:
    virtual int compareItems(QCollection::Item item1, QCollection::Item item2);
  
  public:
    void search(BrowserNode * bn, UmlCode k, const QString & s,
		bool cs, bool even_deleted, bool for_name,
		bool for_stereotype);
    void search_ddb(BrowserNode * bn, UmlCode k, const QString & s,
		    bool cs, bool even_deleted);
  
    void names(QStringList & list) const;
    void full_names(QStringList & list) const;
    void full_defs(QStringList & list) const;
    void sort_it();
};


#endif

