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

#include <qtextstream.h> 

#include "UmlItem.h"

UmlItem::~UmlItem() {
}

void UmlItem::manage_comment(const char *& p, const char *& pp) {
  static QCString the_comment;
  
  p += 10;
  
  if ((pp != 0) || // comment contains ${comment} !
      description().isEmpty())
    return;
  
  const char * comment = description();
  
  the_comment = "#";
  
  do {
    the_comment += *comment;
    if ((*comment++ == '\n') && *comment)
      the_comment += "#";
  } while (*comment);
  
  switch (*p) {
  case 0:
  case '\n':
    break;
  default:
    the_comment += '\n';
  }
    
  pp = p;
  p = the_comment;
}

void UmlItem::manage_description(const char *& p, const char *& pp) {
  static QCString the_comment;
  
  p += 14;
  
  the_comment = description();
  
  if ((pp != 0) || // comment contains ${description} !
      the_comment.isEmpty())
    return;
  
  pp = p;
  p = the_comment;
}

void UmlItem::manage_docstring(const char *& p, const char *& pp, BooL & indent_needed,
			       QCString & indent, QCString & saved_indent)
{
  static QCString the_comment;
  
  p += 12;
  
  the_comment = description();
  
  if ((pp != 0) || // comment contains ${description} !
      the_comment.isEmpty())
    return;
    
  int index = 0;
  
  while ((index = the_comment.find("\"\"\"", index)) != -1) {
    the_comment.insert(index, "\\");
    index += 2;
  }
  
  if (!indent.isEmpty()) {
    int len = indent.length() + 1;
    
    index = 0;
    
    while ((index = the_comment.find('\n', index)) != -1) {
      the_comment.insert(index + 1, indent);
      index += len;
    }
  }
  
  the_comment = "\"\"\"" + the_comment + "\"\"\"\n";
  
  if (indent_needed) {
    indent_needed = FALSE;
    the_comment = indent + the_comment;
  }

  pp = p;
  p = the_comment;
  saved_indent =  indent;
  indent = "";
}

void UmlItem::manage_alias(const char *& p, QTextOStream & ts,
			   QCString indent, BooL & indent_needed) {
  if (indent_needed) {
    indent_needed = FALSE;
    ts << indent;
  }
  
  // p starts by '@'
  const char * pclosed;
  
  if ((p[1] == '{') && ((pclosed = strchr(p + 2, '}')) != 0)) {
    QCString key(p + 2, pclosed - p - 1);
    QCString value;
    UmlItem * node = this;

    do {
      if (node->propertyValue(key, value))
	break;
      node = node->parent();
    } while (node != 0);
    
    if (node != 0)
      // find, insert the value
      ts << value;
    else
      // not find, insert the key
      ts << "@{" << key << '}';

    // bypass the key
    p += strlen(key) + 3;
  }
  else
    // bypass '$'
    ts << *p++;
}

void UmlItem::generate() {
  // does nothing
}

UmlPackage * UmlItem::package() {
  return parent()->package();
}
