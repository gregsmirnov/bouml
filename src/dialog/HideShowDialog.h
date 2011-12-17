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

#ifndef HIDESHOWDIALOG_H
#define HIDESHOWDIALOG_H

#include <qdialog.h>
#include <qvaluelist.h>

class QListBox;
class QCheckBox;
class BrowserNode;
class BrowserNodeList;

class HideShowDialog : public QDialog {
  Q_OBJECT
 
  protected:
    const BrowserNodeList & all;
    QValueList<BrowserNode *> & hidden_visible;
    BooL & on_visible;
    QListBox * lb_visible;
    QListBox * lb_hidden;
    QCheckBox * cb_visible;
 
    static QSize previous_size;
  
  public:
    HideShowDialog(const BrowserNodeList & a,
		   QValueList<BrowserNode *> & h, BooL & visible);
    virtual ~HideShowDialog();
    
  protected slots:
    virtual void polish();
    void hide_them();
    void show_them();
    void show_all();
    void hide_all();
    void hide_private();
    void hide_private_protected();
    virtual void accept();
};

#endif

