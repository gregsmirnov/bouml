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

#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <qvaluelist.h>
#include <qstringlist.h>
#include <qcstring.h>
#include <qdict.h>
#include <qmap.h>

#include "Lex.h"

// it is a singleton but I prefer static members
// Namespace::instance() is too long to hit

class Namespace {
  public:
    static void set(const QCString & s);
    static void unset();
    static void enter(const QCString & s);
    static void exit();
    static void enter_anonymous() { AnonymousLevel += 1; }
    static void exit_anonymous() { AnonymousLevel -= 1; }
    static bool underAnonymous() { return AnonymousLevel != 0; }
    
    static void save_using_scope() {
      UsingScope.prepend(Usings);
    }
    static void restore_using_scope();
    static const QStringList stack() { return Stack; }
    
    static void add_using(const QCString & s) {
      Usings.append(QString(s) + "::");
    }
    static void clear_usings() { Usings.clear(); }
    static const QStringList & usings() { return Usings; }
    
    static void add_alias(const QCString & a, const QCString & s) {
      Aliases.replace(a, s);
    }
    static void clear_aliases() { Aliases.clear(); }
    
    static QString namespacify(QCString s, bool local);
    static QCString current();
    
  private:
    static QStringList Stack;
    static int AnonymousLevel;
    static QStringList Usings;
    static QValueList<QStringList> UsingScope;
    static QMap<QCString,QCString> Aliases;
};

// does not not inherit QDict to not allow to use directly
// a QDict operation, to force to take into account the namespace
// management

template<class T>
class NDict {
  public:
    NDict() : hasAnonymous(FALSE) {}
    NDict(unsigned n) : hasAnonymous(FALSE) { d.resize(n); }
  
    void insert(const QCString & key, const T * item);
    void replace(const QCString & key, const T * item);
    bool remove(const QCString & key);
    T * operator[] (const QCString & key) const;
      
  private:
    bool hasAnonymous;
    QDict<T> d;
};

template<class T>
void NDict<T>::insert(const QCString & key, const T * item) {
  hasAnonymous |= Namespace::underAnonymous();
  d.insert(Namespace::namespacify(key, Namespace::underAnonymous()), item);
}

template<class T>
void NDict<T>::replace(const QCString & key, const T * item) {
  hasAnonymous |= Namespace::underAnonymous();
  d.replace(Namespace::namespacify(key, Namespace::underAnonymous()), item);
}

template<class T>
bool NDict<T>::remove(const QCString & key) {
  return d.remove(Namespace::namespacify(key, Namespace::underAnonymous()));
}

template<class T>
T * NDict<T>::operator[] (const QCString & key) const {
  QString k = Namespace::namespacify(key, FALSE);
  T * r = d[k];
  
  if (r != 0)
    return r;
  
  QString s;
  
  if ((((const char *) key)[0] != ':') ||
      (((const char *) key)[1] != ':')) {
    QStringList::ConstIterator it;
    
    s = key;
    
    for (it = Namespace::usings().begin();
	 it != Namespace::usings().end();
	 ++it)
      if ((r = d[*it + s]) != 0)
	return r;
    
    for (it = Namespace::stack().begin();
	 it != Namespace::stack().end();
	 ++it)
      if ((r = d[*it + s]) != 0)
	return r;
  }
  
  if (hasAnonymous) {
    k += "\n" + Lex::filename();
    r = d[k];
    
    if ((r == 0) && 
	((((const char *) key)[0] != ':') ||
	 (((const char *) key)[1] != ':'))) {
      QStringList::ConstIterator it;
      
      s += "\n" + Lex::filename();
      
      for (it = Namespace::usings().begin();
	   it != Namespace::usings().end();
	   ++it)
	if ((r = d[*it + s]) != 0)
	  return r;
      
      for (it = Namespace::stack().begin();
	   it != Namespace::stack().end();
	   ++it)
	if ((r = d[*it + s]) != 0)
	  return r;
    }
  }
  
  return r;
}

#endif
