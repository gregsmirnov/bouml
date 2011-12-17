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

#ifndef MYIO_H
#define MYIO_H

#include <stdio.h>
#include "UmlEnum.h"

#define FILEFORMAT 75

class QDir;
class QFile;
class QFileInfo;
class QStringList;
class QCanvasItem;
class QTextStream;
class QCanvasRectangle;
class QBuffer;
class QPoint;
class QPointArray;
class QColor;
class QRect;
class QFont;

class DiagramItem;

extern QString my_baseName(QFileInfo & fi);

typedef struct {
  QString filename;
  unsigned linenum;
  char * previous_word;	// Q[C]String too expensive
  bool read_only;
  bool previous_word_allocated;
  char removed_char;
  unsigned read_file_format;
} ReadContext;

extern ReadContext current_context();
extern void restore_context(ReadContext &);

extern bool read_only_file();
extern void set_read_only_file();
extern void force_read_only(bool y);

extern bool on_load_diagram();
extern void set_on_load_diagram(bool);

extern void backup(QDir & d, QString fn);
extern void delete_backup(QDir & d);

extern int open_file(QFile & fp, int mode, bool silent = FALSE);
extern void read_in(const QString &);
extern bool copy_file(QFileInfo * src, const QDir & dest);
extern void save_if_needed(const char * filename, const char * newdef);

extern char * read_file(QString filename);
extern char * read_file(QString filename, int offset, int len);

extern QString last_used_directory();
extern void set_last_used_directory(QString);

extern void set_read_file_format(unsigned);
extern unsigned read_file_format();

extern QString abs_file_path(int id, const char * ext);
extern char * read_definition(int id, const char * ext);
char * read_definition(int id, const char * ext, int offset, int len);
extern void save_definition(int id, const char * ext, const char * def, BooL & is_new);
extern void delete_definition(int id, const char * ext);

extern void save_string(const char *, QTextStream & st);
extern void save_string_list(QStringList & list, QTextStream & st);
extern void save_unicode_string_list(QStringList & list, QTextStream & st);
extern void nl_indent(QTextStream & st);
extern void indent(int);
extern int indent();
extern void indent0();

extern bool at_end(char * & st);
extern char * read_string(char * & st);
extern char * read_keyword(char * & st);
extern char * read_keyword(char * & st, const char * expected);
extern char * read_keyword(char * & st, const char * expected1,
			   const char * expected2);
extern void unread_keyword(char * keyword, char * & st);
extern int read_id(char * & st);
extern unsigned read_unsigned(char * & st);
extern double read_double(char * & st);
extern void read_string_list(QStringList & list, char * & st);
extern void read_unicode_string_list(QStringList & list, char * & st);
extern char * skip_until(char * & st, const char * expected);

extern void unknown_keyword(const char * k);
extern void wrong_keyword(const char * k, const char * expected);
extern void unknown_ref(const char * kind, int id);

// ifdef save as int to bypass bug in << float in qt 3.3.3
#undef FORCE_INT_COORD
extern void save_xy(QTextStream & st, const QCanvasItem * c, const char * s);
extern void save_xyz(QTextStream & st, const QCanvasItem * c, const char * s);
extern void save_xyzwh(QTextStream & st, const QCanvasRectangle * c, const char * s);
extern void read_xy(char * & st, QCanvasItem * c);
extern void read_xyz(char * & st, QCanvasItem * c);
extern void read_xyzwh(char * & st, QCanvasRectangle * c);
extern void read_zwh(char * & st, QCanvasRectangle * c);
extern void bypass_xy(char * & st);
extern void save_color(QTextStream & st, const char *, UmlColor, BooL &);
extern void read_color(char *& st, const char *, UmlColor &, char * &);

extern void read_font(char *& st, const char *, UmlFont &, char * &);

extern void warn(const QString &);

// history

extern void save(const DiagramItem *, QBuffer &);
extern DiagramItem * load_item(QBuffer &);

extern void save_ptr(const void *, QBuffer &);
extern void * load_ptr(QBuffer &);

extern void save(const QPoint & p, QBuffer &);
extern void load(QPoint & p, QBuffer &);

extern void save(const QPointArray & a, QBuffer &);
extern void load(QPointArray & a, QBuffer &);

extern void save(double, QBuffer &);
extern void load(double &, QBuffer &);
extern double load_double(QBuffer &);

extern void save(float, QBuffer &);
extern void load(float &, QBuffer &);
extern double load_float(QBuffer &);

extern void save(int, QBuffer &);
extern void load(int &, QBuffer &);


// svg

extern bool start_svg(const char * f, int w, int h);
extern void end_svg();
extern FILE * svg();
extern int svg_height();
extern const char * svg_color(UmlColor);
extern void draw_shadow(FILE * fp, QPointArray & poly);
extern void draw_poly(FILE * fp, QPointArray & poly,
		      UmlColor color, bool stroke = TRUE);
extern void draw_text(const QRect & r, int align, QString s, const QFont &, FILE *);
extern void draw_text(int x, int y, int w, int h, int align, QString s,
		      const QFont &, FILE *, UmlColor fg = UmlBlack,
		      UmlColor bg = UmlTransparent);
extern void draw_rotate_text(int cx, int cy, int angle, QString s, 
			     const QFont &, FILE *, UmlColor fg = UmlBlack);

// windows wrong home dir




#define homeDir() QDir::home()


#endif
