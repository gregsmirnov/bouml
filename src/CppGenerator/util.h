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

#ifndef _UTIL_H
#define _UTIL_h

#include <qcstring.h>

extern QString capitalize(const QString & s);
extern QCString capitalize(const QCString & s);
extern QCString current_indent(const char * p, const char * begin);
extern const char * name_spec(const char *);
extern bool need_equal(const char * p, const char * v);
extern void set_verbose();
extern bool verbose();
extern void set_preserve();
extern bool preserve();
extern void set_trace_header(QCString s);
extern QCString get_trace_header();
extern void write_trace_header();
extern void incr_error();
extern void incr_warning();
extern int n_errors();
extern int n_warnings();
#endif
