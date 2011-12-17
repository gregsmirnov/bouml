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

#include "Namespace.h"

// namespace stack, namespaces.last() = current namespace full name + "::",
// i.e. A::B...::Z:: in case it is embeded in other one(s)
QStringList Namespace::Stack;

// using list, each element ended by "::"
QStringList Namespace::Usings;

// level of nested anonymous namespace
int Namespace::AnonymousLevel;

// to lost usings defined under a namespace/class block
QValueList<QStringList> Namespace::UsingScope;

// namespace aliases
QMap<QCString,QCString> Namespace::Aliases;

void Namespace::set(const QCString & s)
{
  // for upload only
  Stack.append(QString(s) + "::");
}

void Namespace::unset()
{
  // for upload only
  Stack.remove(Stack.last());
}

void Namespace::enter(const QCString & s)
{
  save_using_scope();
  Stack.append((Stack.isEmpty())
	       ? QString(s) + "::"
	       : Stack.last() + QString(s) + "::");
}

void Namespace::exit()
{
  Stack.remove(Stack.last());
  restore_using_scope();
}

void Namespace::restore_using_scope()
{ 
  Usings = UsingScope.first();
  UsingScope.remove(UsingScope.begin());
}

QString Namespace::namespacify(QCString s, bool local) {
  QString r;
  int index = s.find("::");
  
  if (index == 0)
    r = ((const char *) s) + 2;
  else {
    if (index != -1) {
      QMap<QCString,QCString>::ConstIterator it = 
	Aliases.find(s.left(index));
      
      if (it != Aliases.end())
	s.replace(0, index, *it);
    }
    
    r = (Stack.isEmpty())
      ? QString(s)
      : Stack.last() + QString(s);
  }
  
  return (local)
    ? r + "\n" + Lex::filename()
    : r;
}

QCString Namespace::current() {
  if (Stack.isEmpty())
    return 0;
  
  QString & s = Stack.last();
  
  return QCString(s.left(s.length() - 2));
}
