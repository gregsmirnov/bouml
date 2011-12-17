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

#ifndef PROFILEDSTEREOTYPES_H
#define ProfiledSTEREOTYPES_H

#include <qstringlist.h>

#include "UmlEnum.h"

class QPixmap;

class BrowserPackage;
class BrowserClass;
class BrowserClassView;
class BrowserAttribute;
class BrowserNode;

class ProfiledStereotypes {
  public:
    static void init();
    static void post_load();
    static void recompute(bool warm);
    static void recompute(BrowserNode * bn);
    static bool hasStereotype();
    static const QStringList & defaults(UmlCode);
    static const QStringList & availableTargets();
    static QString pretty(QString s);
    static QString unpretty(QString s);
    static bool canAddPackage(BrowserPackage *, QString name);
    static void renamed(BrowserPackage *, QString oldname);
    static void deleted(BrowserPackage *, bool propag);
    static QString canAddStereotype(BrowserClass *, QString name);
    static QString canAddStereotype(BrowserClassView *, QString name);
    static void added(BrowserClass *);
    static void changed(BrowserClass *, QString oldname, bool newicon);
    static void deleted(BrowserClass *);
    static void added(BrowserAttribute *);
    static void changed(BrowserAttribute *, QString oldname);
    static void deleted(BrowserAttribute *);
    static void set(BrowserNode *, QString);
    static void unset(BrowserNode *, QString);
    static BrowserClass * isModeled(QString, bool case_sensitive = TRUE);
    static void modified(BrowserNode *, bool);
    static void applyStereotype(BrowserNode * bn);
    static void menu(QPopupMenu & m, BrowserNode * bn, int bias);
    static void choiceManagement(BrowserNode * bn, int choice);
    static void callCheck(BrowserNode *, bool rec);
    static bool enumerated(QString st, QStringList &);
    static const QPixmap * browserPixmap(const char * s);
    static const QPixmap * diagramPixmap(const char * s, double zoom);
};

#endif
