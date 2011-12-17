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

// lexer written by hand rather than using [f]lex as usual,
// BOUML is also my first development including a source code
// reader where I do not use yacc/bison, to see how it is 
// without ...

//#define DEBUG_BOUML
#ifdef DEBUG_BOUML
#include <iostream>

using namespace std;
#endif

// Because stdio and fstream are bugged under Windows
// concerning fseek and equivalent, I read all the file in a buffer
// and manage it after.
#include <qfile.h>

#include "Lex.h"
#include "UmlCom.h"

QAsciiDict<char> Lex::_defines;
QString Lex::_filename;
char * Lex::_buffer;
LexContext Lex::_context;
LexContext Lex::_mark;

void Lex::defines(const QCString & f)
{
  QFile in(f);
  
  if (!in.open(IO_ReadOnly)) {
    UmlCom::trace("cannot open '" + f + "' (#file property value)");
    return;
  }
  
  char l[1024];
  int n;
  
  while ((n = in.readLine(l, sizeof(l) - 1)) != -1) {
    if (n != 0) {
      switch (l[n - 1]) {
      case '\n':
      case '\r':
	l[--n] = 0;
      }
      switch (l[n - 1]) {
      case '\n':
      case '\r':
	l[--n] = 0;
      }
      
      char * p = strchr(l, '=');
      const char * v;
      
      if (p != 0) {
	*p = 0;
	v = strdup(p+1);
      }
      else
	v = "";
      
      _defines.insert(strdup(l), v);
    }
  }
  
  in.close();
}

int Lex::get() 
{
  switch (*_context.pointer) {
  case 0:
    return EOF;
  case '\n':
    _context.line_number += 1;
    // no break;
  default:
    return *_context.pointer++;
  }
}

int Lex::peek()
{
  return (*_context.pointer == 0) ? EOF : *_context.pointer;
}

void Lex::unget()
{
  if (*--_context.pointer == '\n')
    _context.line_number -= 1;
}

static QCString Separators = " \r\t\f\n&~\"#{'(+-|`^)[]=}%*<>?,.;/:!";

const QString & Lex::filename()
{
  return _filename;
}

unsigned Lex::line_number()
{
  return _context.line_number;
}

bool Lex::open(const QString & f)
{
#ifdef DEBUG_BOUML
  cout << "Lex::open(" << f << ")\n";
#endif
  
  _filename = f;
  _context.line_number = 1;
  _context.comments = "";
  _context.description = "";

  QFile in(f);
  unsigned sz;
  
  if (!in.open(IO_ReadOnly) ||
      ((_buffer = new char[(sz = in.size()) + 1]) == 0))
    return FALSE;
    
  unsigned offset = 0;
  
  do offset += in.readBlock(_buffer + offset, sz - offset);
  while (offset != sz);

  _buffer[sz] = 0;
  _context.pointer = _buffer;
  
  return TRUE;
}

void Lex::close()
{
#ifdef DEBUG_BOUML
  cout << "Lex::close()\n";
#endif
  delete [] _buffer;
}

bool Lex::start_template(int c)
{
  if (c == '<')
    return TRUE;
  
  LexContext context = _context;
  
  unget();
  goes_to_word_beginning();
  
  if (get() == '<')
    return TRUE;
  
  _context = context;
  return FALSE;
}

void Lex::complete_template(QString & result)
{
  // template
  unsigned level = 1;
  char * pointer = _context.pointer;
  	  
  result += '<';
  
  for (;;) {
    switch (read_word_bis(FALSE, FALSE)) {
    case 0:
      return;
    case '<':
      level += 1;
      break;
    case '>':
      if (--level == 0) {
	char c = *_context.pointer;
	
	*_context.pointer = 0;
	result += pointer;
	*_context.pointer = c;
	return;
      }
    default:
      break;
    }
  }
}

void Lex::bypass_template()
{
  // template
  unsigned level = 1;
	  
  for (;;) {
    switch (read_word_bis(FALSE, FALSE)) {
    case 0:
      return;
    case '<':
      level += 1;
      break;
    case '>':
      if (--level == 0)
	return;
    default:
      break;
    }
  }
}

// the '<' was read, read up to the coresponding '>'

bool Lex::finish_template(QCString & s)
{
  // template
  unsigned level = 1;
	  
  for (;;) {
    // a comment at this place is not correctly managed !
    int c = get();
    
    if (c == EOF)
      return FALSE;
    
    s += c;
    
    if (c == '<')
      level += 1;
    else if ((c == '>') && (--level == 0))
      break;
  }
  
  return TRUE;
}

// an type is an identifier (probably a type)
// in case it is followed by '::qsd', '::qsd' is read and added to id

QCString Lex::complete_template_type(QCString id)
{
  QCString s;
	
  while (!(s = read_word()).isEmpty()) {
    if ((((const char *) s)[0] != ':') || (((const char *) s)[1] != ':')) {
      unread_word();
      break;
    }
    else
      id += s;
  }
  
  return id;
}
	
void Lex::bypass_pp()
{
  for (;;) {
    int c = get();
    
    switch (c) {
    case '\n':
    case EOF:
      _context.comments = QString::null;
      _context.description = QString::null;
      return;
    case '\\':
      get();
      break;
    case '/':
      if (peek() == '*')
	bypass_c_comment();
    }
  }
}

void Lex::bypass_cpp_comment()
{
  get();	// second '/'
  
  if (! _context.comments.isEmpty())
    _context.comments += '\n';
  _context.description += (! _context.description.isEmpty())
      ? "\n//" :  "//";
  
  int c;
  char * p = _context.pointer;
  
  for (;;) {
    c = get();
    if (c == EOF) {
      _context.comments += p;
      _context.description += p;
      return;
    }
    if (c == '\n') {
      _context.pointer[-1] = 0;
      _context.comments += p;
      _context.description += p;
      _context.pointer[-1] = '\n';
      return;
    }
  }
}

void Lex::bypass_c_comment()
{
  get();	// the *
  
  if (! _context.comments.isEmpty())
    _context.comments += '\n';
  _context.description += (! _context.description.isEmpty())
      ? "\n/*" :  "/*";
  
  char * p = _context.pointer;	// start of the comment on the line
  
  for (;;) {
    int c = get();
    
    if (c == EOF)
      break;
    else if (c == '\n') {
      if ((p != 0) && (_context.pointer != (p + 1))) {
	// non empty line
	c = *_context.pointer;
	*_context.pointer = 0;
	_context.comments += p;
	_context.description += p;
	*_context.pointer = c;
      }
      else {
	_context.comments += c;
	_context.description += c;
      }
      p = 0;
    }
    else if ((c == '*') && (peek() == '/')) {
      if ((p != 0) && (_context.pointer != (p + 1))) {
	// non empty line
	_context.pointer[-1] = 0;
	_context.comments += p;
	_context.pointer[-1] = '*';
	c = _context.pointer[1];
	_context.pointer[1] = 0;
	_context.description += p;
	_context.pointer[1] = c;
      }
      get();	// '/'
      break;
    }
    else if ((p == 0) && ((c != ' ') || (c != '\t')))
      p = _context.pointer - 1;
  }
}

QCString Lex::manage_operator(QString & result, int c, bool oper)
{
  // a comment is not correctly managed !
  result += c;
  
  int next = peek();
  
  switch (c) {
  case '!':
  case '%':
  case '*':
  case '/':
  case '=':
  case '^':
    if (next == '=')
      result += get();
    break;
  case '.':
    if (next == '*')
      result += get();
    else if (next == '.') {
      result += get();
      result += get();	// must be .
    }
    break;
  case ':':
    if (next == ':')
      result += get();
    break;
  case '<':
  case '>':
    if (next == '=')
      result += get();
    else if (next == c) {
      result += get();
      if (peek() == '=')
	result += get();
    }
    break;
  case '-':
    if (next == '>') {
      result += get();
      if (peek() == '*')
	result += get();
    }
    // no break;
  case '&':
  case '+':
  case '|':
    if ((next == '=') || (next == c))
      result += get();
    break;
  case '(':
  case '[':
    if (oper) // operator
      result += get();
  }
  
#ifdef DEBUG_BOUML
  cout << "retourne '" << result << "'\n";
#endif
  return QCString(result);
}

char Lex::bypass_operator(int c, bool oper)
{
  // a comment is not correctly managed !
  int next = peek();
  
  switch (c) {
  case '!':
  case '%':
  case '*':
  case '/':
  case '=':
  case '^':
    if (next == '=') {
      get();
      return '!'; // to not be = when ==
    }
    return (char) c;
  case '.':
    if (next == '*')
      get();
    else if (next == '.') {
      get();
      get();	// must be .
    }
    return (char) c;
  case ':':
    if (next == ':')
      get();
    return (char) c;
  case '<':
  case '>':
    if (next == '=')
      get();
    else if (next == c) {
      get();
      if (peek() == '=')
	get();
    }
    else
      return (char) c;
    return '!'; // to not be < or > if << >> <= or >=
  case '-':
    if (next == '>') {
      get();
      if (peek() == '*')
	get();
    }
    // no break;
  case '&':
  case '+':
  case '|':
    if ((next == '=') || (next == c))
      get();
    return (char) c;
  case '(':
  case '[':
    if (oper) {
      // operator
      get();
      return '!'; // to not be ( or [
    }
  default:
    return (char) c;
  }
}

QCString Lex::read_string()
{
  QString result = "\"";;
  
  for (;;) {
    int c = get();
    
    switch (c) {
    case EOF:
      return 0;
    case '\\':
      c = get();
      result += '\\';
      result += c;
      break;
    case '"':
      return QCString(result += c);
    default:
      result += c;
    }
  }
}

void Lex::bypass_string()
{
  for (;;) {
    switch (get()) {
    case EOF:
      return;
    case '\\':
      get();
      break;
    case '"':
      return;
    default:
      break;
    }
  }
}

QCString Lex::read_character()
{
  QCString result = "'";
  
  for (;;) {
    int c = get();
    
    switch (c) {
    case EOF:
      return 0;
    case '\'':
      return result += c;
    case '\\':
      result += c;
      result += get();
      break;
    default:
      result += c;
    }
  }
}

void Lex::bypass_character()
{
  for (;;) {
    switch (get()) {
    case EOF:
      return;
    case '\'':
      return;
    case '\\':
      get();
      break;
    default:
      break;
    }
  }
}

QCString Lex::read_array_dim() 
{
  QCString result = "[";
  char * pointer = _context.pointer;
	  
  for (;;) {
    switch (read_word_bis(FALSE, TRUE)) {
    case 0:
      result = 0;
      return result;
    case ']':
      {
	char c = *_context.pointer;
	
	*_context.pointer = 0;
	result += pointer;
	*_context.pointer = c;
	
#ifdef DEBUG_BOUML
	cout << "retourne '" << result << "'\n";
#endif
	return result;
      }
    default:
      break;
    }
  }
}

void Lex::bypass_array_dim() 
{
  for (;;) {
    switch (read_word_bis(FALSE, TRUE)) {
    case 0:
    case ']':
      return;
    default:
      break;
    }
  }
}



// remove first and last line in comment if non significant
QCString Lex::simplify_comment(QCString & comment)
{
  if (comment.isEmpty())
    return comment;
  
  const char * s = comment;
  const char * p = s;
  
  for (;;) {
    switch (*p) {
    case 0:
      return comment;
    case ' ':
    case '\t':
      p += 1;
      break;
    case '\n':
      comment.remove(0, p - s + 1);
      
      if (comment.isEmpty())
	return comment;
      
      s = comment;
      // no break
    default:
      p = s + comment.length() - 1;

      while (p != s) {
	switch(*p) {
	case ' ':
	case '\t':
	  p -= 1;
	  break;
	case '\n':
	  comment.resize(p - s + 1);
	  // no break
	default:
	  return comment;
	}
      }
      
      if (*p == '\n')
	comment = "";
      return comment;
    }
  }
}

void Lex::goes_to_word_beginning()
{
  for (;;) {
    int c = get();
    
    switch (c) {
    case EOF:
      return;
    case '#':
      bypass_pp();
      break;
    case '/':
      switch (peek()) {
      case '/':
	bypass_cpp_comment();
	break;
      case '*':
	bypass_c_comment();
	break;
      default:
	unget();
	return;
      }
      break;
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      break;
    default:
      unget();
      return;
    }
  }
}

void Lex::unread_word()
{
  _context.pointer = _context.read_word_pointer;
  _context.comments = _context.read_word_comments;
  _context.description = _context.read_word_description;
  _context.line_number = _context.read_word_line_number;
}

QCString Lex::read_word(bool in_expr)
{
  goes_to_word_beginning();

  _context.read_word_pointer = _context.pointer;
  _context.read_word_comments = _context.comments;
  _context.read_word_description = _context.description;
  _context.read_word_line_number = _context.line_number;
    
  static QCString spaces = " \t\n\r";

  QString result;
  
  for (;;) {
    int c = get();
    
    if (c == EOF)
      break;
    else if (Separators.find(c) == -1)
      result += c;
    else if ((c == ':') && (peek() == ':')) {
      result += "::";
      get();	// second ':'
      do c = get(); while (spaces.find(c) != -1);
      if (c == EOF)
	break;
      result += c;
      if (c == '*')
	break;
    }
    else if (! result.isEmpty()) {
      if (!in_expr  &&
	  (result != "template") &&
	  (result != "operator") &&
	  (result.right(9) != ":operator") &&
	  start_template(c))
	complete_template(result);
      else if (!in_expr &&
	       ((result == "operator") ||
		(result.right(9) == ":operator"))) {
	// a comment at this place is not correctly managed !
	while (spaces.find(c) != -1)
	  c = get();
	if (Separators.find(c) != -1)
	  return manage_operator(result += ' ', c, TRUE);
	else {
	  // operator new/delete, conversions
	  result += ' ';
	  unget();
	  
	  QCString w;
	  
	  while (!(w = read_word()).isEmpty()) {
	    if (w == "(") {
	      unget();
	      break;
	    }
	    result += w;
	  }
	}
      }
      else
	unget();
      break;
    }
    else {
      switch (c) {
      case '#':
	bypass_pp();
	break;
      case '"':
	return read_string();
      case '[':
	return read_array_dim();
      case '\'':
	return read_character();
      case '/':
	switch (peek()) {
	case '/':
	  bypass_cpp_comment();
	  break;
	case '*':
	  bypass_c_comment();
	  break;
	case '=':
	  get();
#ifdef DEBUG_BOUML
	  cout << "retourne '/='\n";
#endif
	  return "/=";
	default:
#ifdef DEBUG_BOUML
	  cout << "retourne '/'\n";
#endif
	  return "/";
	}
	break;
      default:
	if (c > ' ')
	  return manage_operator(result, c, FALSE);
	break;
      }
    }
  }
  
  if (! result.isEmpty()) {
    const char * v = _defines.find((const char *) result);
    
    if (v != 0) {
      if (*v == 0)
	// keyword removed
	return read_word(in_expr);
      
#ifdef DEBUG_BOUML
      cout << "retourne '" << v << "'\n";
#endif
      
      return QCString(v);
    }
  }
  
#ifdef DEBUG_BOUML
  cout << "retourne '" << result << "'\n";
#endif
  
  return QCString(result);
}

char Lex::read_word_bis(bool set_context, bool in_expr)
{
  goes_to_word_beginning();

  if (set_context) {
    _context.read_word_pointer = _context.pointer;
    _context.read_word_comments = _context.comments;
    _context.read_word_description = _context.description;
    _context.read_word_line_number = _context.line_number;
  }
    
  static QCString spaces = " \t\n\r";

  char result = 0;
  bool is_template = FALSE;
  bool is_operator = FALSE;
  
  for (;;) {
    int c = get();
    
    if (c == EOF)
      break;
    else if (Separators.find(c) == -1) {
      if (result == 0) {
	result = (char) c;
	
	const char * p = _context.pointer;
	
	while (*p && (Separators.find(*p) == -1))
	  p += 1;
	
	int n = p - _context.pointer;
	
	if (n == 8) {
	  if (!strncmp(_context.pointer, "template", 8))
	    is_template = TRUE;
	  else if (!strncmp(_context.pointer, "operator", 8))
	    is_operator = TRUE;
	}
	else 
	  is_operator = ((n > 8 ) && !strncmp(_context.pointer + n - 9, ":operator", 9));
      }
    }
    else if ((c == ':') && (peek() == ':')) {
      if (result == 0)
	result = (char) c;
      get();	// second ':'
      do c = get(); while (spaces.find(c) != -1);
      if (c == EOF)
	break;
      if (c == '*')
	break;
    }
    else if (result != 0) {
      if (!in_expr  && !is_template && !is_operator && start_template(c))
	bypass_template();
      else if (!in_expr && is_operator) {
	// a comment at this place is not correctly managed !
	while (spaces.find(c) != -1)
	  c = get();
	if (Separators.find(c) != -1)
	  return bypass_operator(c, TRUE);
	else {
	  // operator new/delete, conversions
	  unget();
	  
	  while ((c = read_word_bis(set_context, FALSE)) != 0) {
	    if (c == '(') {
	      unget();
	      break;
	    }
	  }
	}
      }
      else
	unget();
      break;
    }
    else {
      switch (c) {
      case '#':
	bypass_pp();
	break;
      case '"':
	bypass_string();
	return (char) c;
      case '[':
	bypass_array_dim();
	return '!';	// to not be [
      case '\'':
	bypass_character();
	return (char) c;
      case '/':
	switch (peek()) {
	case '/':
	  bypass_cpp_comment();
	  break;
	case '*':
	  bypass_c_comment();
	  break;
	case '=':
	  get();
	  return (char) c;
	default:
	  return (char) c;
	}
	break;
      default:
	if (c > ' ')
	  return bypass_operator(c, FALSE);
      }
    }
  }
  
  return result;
}

void Lex::finish_line()
{
  for (;;) {
    int c = get();
      
    switch (c) {
    case '/':
      switch (peek()) {
      case '/':
	bypass_cpp_comment();
	return;
      case '*':
	bypass_c_comment();
	continue;
      }
      // no break
    default:
      unget();
      return;
    case '\n':
    case EOF:
      return;
    case ' ':
    case '\t':
      break;
    }
  }
}

QCString Lex::get_comments() 
{
  QCString result = QCString(_context.comments);
  
  _context.comments = QString::null;
  return result;
}

QCString Lex::get_comments(QCString & co) 
{
  QCString result = QCString(_context.comments);
  
  _context.comments = QString::null;
  
  return (co.isEmpty())
    ? result
    : co += "\n" + result;
}

QCString Lex::get_description() 
{
  QCString result = QCString(_context.description);
  
  _context.description = QString::null;
  return result;
}

QCString Lex::get_description(QCString & co) 
{
  QCString result = QCString(_context.description);
  
  _context.description = QString::null;
  
  return (co.isEmpty())
    ? result
    : co += "\n" + result;
}

void Lex::clear_comments()
{
  _context.comments = QString::null;
  _context.description = QString::null;
}

bool Lex::identifierp(const char * s, bool strictp)
{
  if (s == 0)
    return FALSE;
  else if (! strictp) {
    if (*s == ':') {
      s += (s[1] == ':') ? 2 : 1;
      if (*s == 0)
	return FALSE;
    }
    else {
      int len = strlen(s);
      
      if ((len > 3) &&
	  (s[len - 1] == '*') &&
	  (s[len - 2] == ':') &&
	  (s[len - 3] == ':'))
	// AZE::*
	return FALSE;
    }
    return (Separators.find(*s) == -1);
  }
  else {
    // supposed not empty
    for (;;) {
      switch (*s) {
      case 0:
	return TRUE;
      default:
	if (Separators.find(*s) != -1)
	  return FALSE;
	// no break
      case ':':
	s += 1;
      }
    }
  }
}

bool Lex::star(const char * s)
{
  if (s == 0)
    return FALSE;
  
  if (*s == '*')
    return TRUE;
  
  int len = strlen(s);
    
  return ((len > 3) &&
	  (s[len - 1] == '*') &&
	  (s[len - 2] == ':') &&
	  (s[len - 3] == ':'));
}

void Lex::mark()
{
  _mark = _context;
}

void Lex::come_back()
{
  _context = _mark;
}

QCString Lex::region()
{
  char c = *_context.pointer;
  
  *_context.pointer = 0;
  
  QCString result = _mark.pointer;
  
  *_context.pointer = c;
  
  return result;
}

const LexContext & Lex::get_context()
{
  return _context;
}

void Lex::set_context(const LexContext & context)
{
  _context = context;
}

void Lex::syntax_error(QCString s)
{
  UmlCom::trace(QCString("<font face=helvetica>syntax error in <i> ")
		+ QCString(_filename) + "</i> line " +
		QCString().setNum(_context.line_number) + " <b>"
		+ s + "</b></font><br>"); 
  
#ifdef DEBUG_BOUML
  cout << "SYNTAX ERROR IN " << _filename
    << " LINE " << _context.line_number << " : " << s << '\n';
#endif
}

void Lex::warn(QCString s)
{
  UmlCom::trace(QCString("<font face=helvetica>in <i> ")
		+ QCString(_filename) + "</i> line " +
		QCString().setNum(_context.line_number) + " <b>"
		+ s + "</b></font><br>"); 
  
#ifdef DEBUG_BOUML
  cout << "ERROR IN " << _filename
    << " LINE " << _context.line_number << " : " << s << '\n';
#endif
}

void Lex::premature_eof()
{
  UmlCom::trace(QCString("<font face=helvetica>syntax error in <i> ")
		+ QCString(_filename) + "</i> line " +
		QCString().setNum(_context.line_number) +
		" <b>premature eof</b></font><br>"); 
  
#ifdef DEBUG_BOUML
  cout << "SYNTAX ERROR IN " << _filename 
    << " LINE " << _context.line_number << " : premature eof\n";
#endif
}

void Lex::error_near(QCString s)
{
  UmlCom::trace(QCString("<font face=helvetica>syntax error in <i> ")
		+ QCString(_filename) + "</i> line " +
		QCString().setNum(_context.line_number) + " <b>near <font color =\"red\">"
		+ quote(s) + "</font></b></font><br>"); 
  
#ifdef DEBUG_BOUML
  cout << "SYNTAX ERROR IN " << _filename
    << " LINE " << _context.line_number << " : near '" << s << "'\n";
#endif
}

// allows a string to be written as it is by an html writer

QCString Lex::quote(QCString s)
{
  QCString result;
  const char * p = s;
  
  if (p == 0)
    // empty string
    return result;
  
  for (;;) {
    switch (*p) {
    case 0:
      return result;
    case '<':
      result += "&lt;";
      break;
    case '>':
      result += "&gt;";
      break;
    case '&':
      result += "&amp;";
      break;
    default:
      result += *p;
    }

    p += 1;
  }
}

// normalize a template spec placing/removing appropriate spaces
// to be compared to an other one

static bool identifierp(char c)
{
  return (((c >= 'a') && (c <= 'z')) ||
	  ((c >= 'A') && (c <= 'Z')) ||
	  ((c >= '0') && (c <= '9')) ||
	  (c == '_'));
}

QCString Lex::normalize(const QCString & s)
{
  int index = s.find('<');
  
  if (index == -1)
    return s;
  
  QCString r;
  const char * p = s;
  
  index = 0;
  
  for (;;) {
    char c = *p++;
    
    switch (c) {
    case 0:
      return r;
    case '<':
      if (!r.isEmpty() && (r.at(r.length() - 1) == ' '))
	r.truncate(r.length() - 1);
      r += '<';
      break;
    case '>':
      if (!r.isEmpty() && (r.at(r.length() - 1) == '>'))
	r += " >";
      else
	r += '>';
      break;
    case '/':
      if (*p == '/') {
	// comment
	p += 1;
	do p += 1; 
	while (*p && (*p != '\n'));
      }
      else if (*p == '*') {
	/* comment */
	p += 1;
	do p += 1; 
	while (*p && ((*p != '*') || (p[1] != '/')));
	p += 2;
      }
      else {
	if (!r.isEmpty() && (r.at(r.length() - 1) == ' '))
	  r.truncate(r.length() - 1);
      }
      break;
    case '#':
      do {
	if (*++p == '\\')
	  p += 2;
	else if ((*p == '/') && (p[1] == '*')) {
	  /* comment */
	  p += 1;
	  do p += 1; while (*p && ((*p != '*') || (p[1] != '/')));
	  p += 2;
	}
      } while (*p && (*p != '\n'));
      break;
    case '"':
      if (!r.isEmpty() &&
	  (r.at(r.length() - 1) == '"') &&
	  (p[-2] != '"'))
	r += ' ';
      do {
	if (*++p == '\\')
	  p += 2;
      } while (*p && (*p != '"'));
      break;
    case '\'':
      if (!r.isEmpty() && (r.at(r.length() - 1) == ' '))
	r.truncate(r.length() - 1);
      do {
	if (*++p == '\\')
	  p += 2;
      } while (*p && (*p != '\''));
      break;
    case ':':
      if (!r.isEmpty() && (p[-2] <= ' '))
	r += " :";
      else
	r += ':';
      break;
    default:
      if (c > ' ') {
	if (::identifierp(c)) {
	  if (!r.isEmpty() && ::identifierp(r.at(r.length() - 1)))
	    r += ' ';
	  r += c;
	  while ((*p == ':') || ::identifierp(*p))
	    r += *p++;
	}
	else
	  r += c;
      }
    }
  }
  
  return r;
}

QCString Lex::read_list_elt()
{
  const char * p = _context.pointer;  
  int level = 1;
  QCString s;
  
  while (!((s = read_word(TRUE)).isEmpty())) {
    int c = *s;
    
    if ((c == '>') || (c == ')') || (c == ']')) {
      if (--level == 0)
	break;
    }
    else if (c == ',') {
      if (level == 1)
	break;
    }
    else if ((c == '<') || (c == '(') || (c == '['))
      level += 1;
  }
  
  if (! s.isEmpty())
    unread_word();
  
  char c = *_context.pointer;
  
  *_context.pointer = 0;
  s = p;  
  *_context.pointer = c;
  
  return s.stripWhiteSpace();
}

// to compare strings bypassing \r

bool neq(const QCString & s1, const QCString & s2)
{
  const char * p1 = (s1.isNull()) ? "" : (const char *) s1;
  const char * p2 = (s2.isNull()) ? "" : (const char *) s2;
  
  for (;;) {
    while (*p1 == '\r') p1 += 1;
    while (*p2 == '\r') p2 += 1;
    
    if (*p1 == 0)
      return (*p2 != 0);
    if (*p1 != *p2)
      return TRUE;
    
    p1 += 1;
    p2 += 1;
  }
}

// White space means the decimal ASCII codes 9, 10, 11, 12, 13 and 32.
inline bool is_white_space(char c)
{
  return ((c == ' ') || ((c >= '\t') && (c <= '\r')));
}

bool nequal(const QCString & s1, const QCString & s2)
{
  // don't take into account first and last white spaces (like a stripWhiteSpace())
  const char * p1 = (s1.isNull()) ? "" : (const char *) s1;
  const char * p2 = (s2.isNull()) ? "" : (const char *) s2;
  const char * e1 = p1 + s1.length();
  const char * e2 = p2 + s2.length();
  
  while ((p1 != e1) && is_white_space(p1[0]))
    p1 += 1;
  
  while ((p2 != e2) && is_white_space(p2[0]))
    p2 += 1;
  
  while ((e1 != p1) && is_white_space(e1[-1]))
    e1 -= 1;
  
   while ((e2 != p2) && is_white_space(e2[-1]))
    e2 -= 1;
  
  for (;;) {
    if (p1 >= e1)
      return (p2 < e2);
    if (*p1 != *p2)
      return TRUE;
    
    while (*++p1 == '\r') ;
    while (*++p2 == '\r') ;
  }
}
