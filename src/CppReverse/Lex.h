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

#ifndef LEX_H
#define LEX_H

// fstream is bugged under Windows
#include <stdio.h>
#include <qstring.h>
#include <qasciidict.h>

class LexContext {
  friend class Lex;
  private:
    char * pointer;
    QString comments;
    QString description;
    unsigned line_number;
    
    char * read_word_pointer;
    QString read_word_comments;
    QString read_word_description;
    unsigned read_word_line_number;    
};
    
class Lex {
  private:
    static QAsciiDict<char> _defines;
    static QString _filename;
    static char * _buffer;
    static LexContext _context;
    static LexContext _mark;
    
    static int get();
    static int peek();
    static void unget();
    
    static void goes_to_word_beginning();
    static void complete_template(QString & result);
    static QCString manage_operator(QString  & result, int c, bool oper);  
    static QCString read_string();     
    static QCString read_character();    
    static QCString read_array_dim(); 
    static void bypass_template();
    static void bypass_pp();
    static void bypass_cpp_comment();
    static void bypass_c_comment();
    static char bypass_operator(int c, bool oper);  
    static void bypass_string();     
    static void bypass_character();    
    static void bypass_array_dim();  
    static bool start_template(int c);

  public:
    static void defines(const QCString & f);
    static QCString read_word(bool in_expr = FALSE);
    static char read_word_bis(bool set_context, bool in_expr);
    static void unread_word();
    static QCString get_comments(QCString & co);
    static QCString get_comments();
    static QCString get_description(QCString & co);
    static QCString get_description();
    static void clear_comments();
    static void finish_line();
    static bool open(const QString &);
    static void close();
    static const QString & filename();
    static unsigned line_number();
    static bool identifierp(const char *, bool strictp);
    static bool star(const char *);
    static void mark();
    static QCString region();
    static void come_back();
    static const LexContext & get_context();
    static void set_context(const LexContext & context);
    static void syntax_error(QCString = 0);
    static void warn(QCString = 0);
    static void premature_eof();
    static void error_near(QCString);
    static QCString quote(QCString);
    static QCString complete_template_type(QCString id);
    static QCString normalize(const QCString & s);
    static QCString read_list_elt();
    static bool finish_template(QCString &);
    static QCString simplify_comment(QCString &);
};

bool neq(const QCString & s1, const QCString & s2);
bool nequal(const QCString & s1, const QCString & s2);

#endif
