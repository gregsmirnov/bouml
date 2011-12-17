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

#ifndef STRUTIL_H
#define STRUTIL_H

#include "qstring.h"

extern bool manage_comment(QString comment, const char *& p,
			   const char *& pp, bool javadoc);
extern bool manage_description(QString comment, const char *& p,
			       const char *& pp);
extern bool manage_python_comment(QString comment, const char *& p,
				  const char *& pp);
extern bool manage_python_description(QString comment, const char *& p,
				      const char *& pp);
extern void manage_python_docstring(QString comment, const char *& p, const char *& pp,
				    BooL & indent_needed, QString & indent,
				    QString & saved_indent);
extern QString capitalize(const QString & s);
extern QString true_name(const QString & name, const QString & decl);
extern bool is_char_of_name(char c);
extern QString extract_name(QString s);
extern QString quote(QString s);
extern void remove_comments(QCString & s);
extern void remove_comments(QString & s);
extern void remove_python_comments(QCString & s);
extern void remove_python_comments(QString & s);
extern void remove_preprocessor(QCString & s);
extern QString java_multiplicity(QString m);
extern bool need_equal(const char * p, QString v, bool cpp);
extern bool need_equal(const char * p, const char * v, bool cpp);

extern QString codec();
extern void set_codec(QString);
extern QString toUnicode(const char *);
extern QCString fromUnicode(const QString &);
extern bool hasCodec();

extern void remove_crlf(char *);

extern bool is_referenced(const char * s, int id, const char * kc, const char * kr);

#endif
