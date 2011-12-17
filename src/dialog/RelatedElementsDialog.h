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

#ifndef RELATEDELEMENTSDIALOG_H
#define RELATEDELEMENTSDIALOG_H

#include <qdialog.h>
#include <qstringlist.h>

#include "BrowserNode.h"

class QCheckBox;
class QSpinBox;

class RelatedElementsDialog : public QDialog {
  Q_OBJECT
    
  protected:
    BrowserNode * elt;
    BrowserNodeList & elts;
    QCheckBox * sametype_rb;
    QSpinBox * sb_level;
    QCheckBox * inh_rb;
    QCheckBox * dep_rb;
    QCheckBox * assoc_rb;
    QCheckBox * referenced_rb;
    QCheckBox * referencing_rb;
    
    static QSize previous_size;
  
  public:
    RelatedElementsDialog(BrowserNode *, const char * what,
			  bool inh, bool assoc, BrowserNodeList &);
    virtual ~RelatedElementsDialog();
  
  protected slots:
    virtual void polish();
    void accept();
};

#endif
