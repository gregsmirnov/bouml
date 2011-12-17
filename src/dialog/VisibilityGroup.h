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

#ifndef VISIBILITYGROUP_H
#define VISIBILITYGROUP_H

#include <qstring.h>

#include "UmlEnum.h"

class QRadioButton;
class QButtonGroup;

class VisibilityGroup {
  protected:
    QButtonGroup * bgroup;
    QRadioButton * public_rb;
    QRadioButton * protected_rb;
    QRadioButton * private_rb;
    QRadioButton * package_rb;
    
    QString default_pfix;
    QRadioButton * default_visibility_rb;
    
  public:
    VisibilityGroup() {};
  
    QButtonGroup * init(QWidget * parent, UmlVisibility v,
			bool pack_allowed,
			const char * title = 0,
			const char * default_string = 0);
    void update_default(const VisibilityGroup & default_grp);
    void connect(const char *, QWidget *, const char *);
    void follow(const VisibilityGroup & other);
    UmlVisibility value();
    
    QString state();
};

#endif
