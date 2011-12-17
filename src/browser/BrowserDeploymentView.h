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

#ifndef BROWSER_DEPLOYMENTVIEW_H
#define BROWSER_DEPLOYMENTVIEW_H

#include <qintdict.h> 

class QPixmap;
class SimpleData;

#include "BrowserNode.h"
#include "Labeled.h"
#include "Settings.h"

class BrowserDeploymentView : public BrowserNode, public Labeled<BrowserDeploymentView> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserDeploymentView> all;
    static QStringList its_default_stereotypes;
  
    SimpleData * def;
    DeploymentDiagramSettings deploymentdiagram_settings;
    UmlColor package_color;
    UmlColor deploymentnode_color;
    UmlColor component_color;
    UmlColor artifact_color;
    UmlColor note_color;
    UmlColor fragment_color;
    
    BrowserDeploymentView(int id);
    void make();
    void exec_menu_choice(int rank);
    
  public:
    BrowserDeploymentView(QString s, BrowserNode * p, int id = 0);
    BrowserDeploymentView(const BrowserDeploymentView * model, BrowserNode * p);
    virtual ~BrowserDeploymentView();
    
    virtual const QPixmap* pixmap (int) const;
  
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool);
    virtual UmlCode get_type() const;
    virtual QString get_stype() const;
    virtual int get_identifier() const;
    virtual const char * help_topic() const;
    virtual bool may_contains_them(const QList<BrowserNode> &,
				   BooL & duplicable) const;
    virtual BasicData * get_data() const;
    virtual void get_deploymentdiagramsettings(DeploymentDiagramSettings &) const;
    virtual bool tool_cmd(ToolCom * com, const char * args);
    virtual UmlColor get_color(UmlCode) const;
    virtual void save(QTextStream &, bool ref, QString & warning);
    
    virtual void DragMoveEvent(QDragMoveEvent * e);
    virtual void DropEvent(QDropEvent * e);
    virtual void DragMoveInsideEvent(QDragMoveEvent * e);
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
    
    static BrowserDeploymentView * add_deployment_view(BrowserNode * future_parent);
    
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(QTextStream &);

    static BrowserDeploymentView * read(char * &, char *, BrowserNode *, bool);
    static BrowserDeploymentView * read_ref(char * &, const char *);
    static BrowserNode * get_it(const char * k, int id);

    static BrowserNodeList & instances(BrowserNodeList &);
    
    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    virtual void prepare_update_lib() const;
};

#endif
