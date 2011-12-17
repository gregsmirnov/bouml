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

#ifndef UMLPACKAGE_H
#define UMLPACKAGE_H

#include "UmlBasePackage.h"

class UmlClassView;
#ifdef REVERSE
class UmlDeploymentView;
# ifdef ROUNDTRIP
class Package;
class ClassContainer;
# endif
#endif

class UmlPackage : public UmlBasePackage {
  public:
    UmlPackage(void * id, const QCString & n);
  
    void reverse_main(const QCString & type, QCString comment);
    UmlClassView * get_classview(const QCString & nmsp);
#ifdef REVERSE
    UmlDeploymentView * get_deploymentview(const QCString & nmsp);  
# ifdef ROUNDTRIP
    void init(Package *);
    virtual void upload(ClassContainer *);
    virtual bool set_roundtrip_expected();
    virtual void mark_useless(QList<UmlItem> & l);
    virtual void scan_it(int & n);
    virtual void send_it(int n);
    Package * get_package() const { return package; }
    int count_roundtriped();
# endif
#endif  

  private:
    UmlClassView * class_view;
#ifdef REVERSE
    UmlDeploymentView * deployment_view;
# ifdef ROUNDTRIP
    Package * package;
# endif
#endif
    bool namespace_fixedp;
};

#endif
