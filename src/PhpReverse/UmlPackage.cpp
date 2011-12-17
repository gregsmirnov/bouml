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

#ifndef REVERSE
#include <qapplication.h>
#include <qmessagebox.h>
#endif

#include "UmlPackage.h"
#include "UmlCom.h"
#include "UmlClassView.h"
#ifdef REVERSE
#include "UmlDeploymentView.h"
#endif

UmlPackage::UmlPackage(void * id, const QCString & n)
    : UmlBasePackage(id, n) {
  classview = 0;
#ifdef REVERSE
  deploymentview = 0;
#endif
  namespace_fixedp = FALSE;
}

UmlClassView * UmlPackage::get_classview(const QCString & nmsp) {
  UmlPackage * pack;
  
  if (nmsp != phpNamespace()) {
    if (namespace_fixedp) {
      if ((pack = findPhpNamespace(nmsp)) == 0) {
	QCString s = nmsp;
	
	if (s.isEmpty())
	  s = name();
	else {
	  int index = 0;
	  
	  while ((index = s.find("::", index)) != -1)
	    s.replace(index++, 2, " ");
	}
	
	if (((pack = UmlBasePackage::create(this, s)) == 0) &&
	    ((pack = UmlBasePackage::create(this, s += "_")) == 0) &&
	    ((pack = UmlBasePackage::create(this, s += "_")) == 0) &&
	    ((pack = UmlBasePackage::create(this, s += "_")) == 0) &&
	    ((pack = UmlBasePackage::create(this, s += "_")) == 0)) {
#ifdef REVERSE
	  UmlCom::trace(QCString("<font face=helvetica><b>cannot create package <i>")
			+ s + "</i> under package <i>"
			+ name() + "</b></font><br>");
	  UmlCom::message("");
	  throw 0;
#else
	  QMessageBox::critical(0, "Fatal Error", 
				QCString("<font face=helvetica><b>cannot create package <i>")
				+ s + "</i> under package <i>"
				+ Name() + "</b></font><br>");
	  QApplication::exit(1);
#endif	  
	}
	
	pack->set_PhpNamespace(nmsp);
	pack->set_PhpDir(phpDir());
	pack->namespace_fixedp = TRUE;
      }
    }
    else {
      pack = this;
      pack->set_PhpNamespace(nmsp);
      pack->namespace_fixedp = TRUE;
    }
  }
  else
    pack = this;
  
  if (pack->classview == 0) {
    QVector<UmlItem> ch = pack->children();
    
    for (unsigned index = 0; index != ch.size(); index += 1)
      // return the first class view find
      if (ch[index]->kind() == aClassView)
	return pack->classview = (UmlClassView *) ch[index];
    
    if ((pack->classview = UmlBaseClassView::create(pack, name())) == 0) {
#ifdef REVERSE
      UmlCom::trace(QCString("<font face=helvetica><b>cannot create class view <i>")
		    + name() + "</i> under package <i>"
		    + pack->name() + "</b></font><br>");
      UmlCom::message("");
      throw 0;
#else
      QMessageBox::critical(0, "Fatal Error", 
			    QCString("<font face=helvetica><b>cannot create class view <i>")
			    + name() + "</i> under package <i>"
			    + pack->name() + "</b></font><br>");
      QApplication::exit(1);
#endif
    }
  }
  
  return pack->classview;
}

#ifdef REVERSE

UmlDeploymentView * UmlPackage::get_deploymentview(const QCString & nmsp) {
  UmlPackage * pack;
  
  if (nmsp != phpNamespace()) {
    if (namespace_fixedp) {
      if ((pack = findPhpNamespace(nmsp)) == 0) {
	QCString s = nmsp;
	
	if (s.isEmpty())
	  s = name();
	else {
	  int index = 0;
	  
	  while ((index = s.find("::", index)) != -1)
	    s.replace(index++, 2, " ");
	}
	
	if (((pack = UmlBasePackage::create(this, s)) == 0) &&
	    ((pack = UmlBasePackage::create(this, s += "_")) == 0) &&
	    ((pack = UmlBasePackage::create(this, s += "_")) == 0) &&
	    ((pack = UmlBasePackage::create(this, s += "_")) == 0) &&
	    ((pack = UmlBasePackage::create(this, s += "_")) == 0)) {
#ifdef REVERSE
	  UmlCom::trace(QCString("<font face=helvetica><b>cannot create package <i>")
			+ s + "</i> under package <i>"
			+ name() + "</b></font><br>");
	  UmlCom::message("");
	  throw 0;
#else
	  QMessageBox::critical(0, "Fatal Error", 
				QCString("<font face=helvetica><b>cannot create package <i>")
				+ s + "</i> under package <i>"
				+ Name() + "</b></font><br>");
	  QApplication::exit(1);
#endif	  
	}
	
	pack->set_PhpNamespace(nmsp);
	pack->set_PhpDir(phpDir());
	pack->namespace_fixedp = TRUE;
      }
    }
    else {
      pack = this;
      pack->set_PhpNamespace(nmsp);
      pack->namespace_fixedp = TRUE;
    }
  }
  else
    pack = this;
  
  if (pack->deploymentview == 0) {
    QVector<UmlItem> ch = pack->children();
    
    for (unsigned index = 0; index != ch.size(); index += 1)
      // return the first class view find
      if (ch[index]->kind() == aDeploymentView)
	return pack->deploymentview = (UmlDeploymentView *) ch[index];
    
    if ((pack->deploymentview = UmlBaseDeploymentView::create(pack, name())) == 0) {
#ifdef REVERSE
      UmlCom::trace(QCString("<font face=helvetica><b>cannot create deployment view <i>")
		    + name() + "</i> under package <i>"
		    + pack->name() + "</b></font><br>");
      UmlCom::message("");
      throw 0;
#else
      QMessageBox::critical(0, "Fatal Error", 
			    QCString("<font face=helvetica><b>cannot create deployment view <i>")
			    + name() + "</i> under package <i>"
			    + pack->name() + "</b></font><br>");
      QApplication::exit(1);
#endif
    }
  }
  
  return pack->deploymentview;
}

#endif
