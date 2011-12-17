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

#ifdef TRACE
#include <iostream>

using namespace std;
#endif

// Because stdio and fstream are bugged under Windows
// concerning fseek and equivalent, I read all the file in a buffer
// and manage it after.
#include <qfile.h>

#include "Lex.h"
#include "PhpCatWindow.h"

// the current context
Lex::Context Lex::context;

// to save/restore the current context
QStack<Lex::Context> Lex::stack;

void Lex::push_context()
{
  Context * c = new Context;
  
  *c = context;
  stack.push(c);
}

void Lex::pop_context()
{
  Context * c = stack.pop();
  
  context = *c;
  delete c;  
}

int Lex::get() 
{
  return (*context.pointer == 0) ? EOF : *context.pointer++;
}

int Lex::peek()
{
  return (*context.pointer == 0) ? EOF : *context.pointer;
}

void Lex::unget()
{
  context.pointer -= 1;
}

static QCString Separators = " \r\t\f\n&~\"#{'(-|`)[]=}%*<>?,;/:!";

const QString & Lex::filename()
{
  return context.filename;
}

unsigned Lex::line_number()
{
  return context.line_number;
}

bool Lex::open(const QString & f)
{
#ifdef TRACE
  cout << "Lex::open(" << f << ")\n";
#endif
  
  context.filename = f;
  context.line_number = 1;
  context.comments = "";
  context.description = "";
  context.reread = "";
  context.mark = 0;

  QFile in(f);
  unsigned sz;
  
  if (!in.open(IO_ReadOnly) ||
      ((context.buffer = new char[(sz = in.size()) + 1]) == 0))
    return FALSE;
    
  unsigned offset = 0;
  
  do offset += in.readBlock(context.buffer + offset, sz - offset);
  while (offset != sz);

  context.buffer[sz] = 0;
  context.pointer = context.buffer;
  
  return TRUE;
}

void Lex::close()
{
#ifdef TRACE
  cout << "Lex::close()\n";
#endif
  delete [] context.buffer;
}

void Lex::unread_word(const char * s)
{
  context.reread = s;
}

// '//' or '#' read
void Lex::bypass_cpp_comment()
{  
  if (! context.comments.isEmpty())
    context.comments += '\n';
  context.description += (! context.description.isEmpty())
    ? "\n//" :  "//";
  
  int c;
  char * p = context.pointer;
  
  for (;;) {
    c = get();
    if (c == EOF) {
      context.comments += p;
      context.description += p;
      return;
    }
    if (c == '\n') {
      context.line_number += 1;
      context.pointer[-1] = 0;
      context.comments += p;
      context.description += p;
      context.pointer[-1] = '\n';
      return;
    }
  }
}

// '/' read, '*' only peek

void Lex::bypass_c_comment()
{
  while (*context.pointer == '*')
    context.pointer += 1;
  
  if ((*context.pointer == '/') && (context.pointer[-2] != '/')) {
    // /**/ or /***/ etc ...
    context.pointer += 1;
    return;
  }
  
  if (! context.comments.isEmpty())
    context.comments += '\n';
  context.description += (! context.description.isEmpty())
    ? "\n/*" :  "/*";
  
  char * p = context.pointer;	// start of the comment on the line
  
  for (;;) {
    int c = get();
    
    switch (c) {
    case EOF:
      return;
    case '\n':
      context.line_number += 1;
      if ((p != 0) && (context.pointer != (p + 1))) {
	// non empty line
	c = *context.pointer;
	*context.pointer = 0;
	context.comments += p;
	context.description += p;
	*context.pointer = c;
      }
      else {
	context.comments += c;
	context.description += c;
      }
  
      p = 0;
      break;
    case '*':
      if (peek() == '/') {
	if ((p != 0) && (context.pointer != (p + 1))) {
	  // non empty line
	  context.pointer[-1] = 0;
	  context.comments += p;
	  context.pointer[-1] = '*';
	  c = context.pointer[1];
	  context.pointer[1] = 0;
	  context.description += p;
	  context.pointer[1] = c;
	}
	get();	// '/'
	return;
      }
      if (p == 0) {
	if (peek() == ' ')
	  get();
	p = context.pointer;
      }
      break;
    case ' ':
    case '\t':
      break;
    default:
      if (p == 0)
	p = context.pointer - 1;
    }
  }
}

QCString Lex::manage_operator(QString & result, int c)
{
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
  case '&':
  case '+':
  case '|':
    if ((next == '=') || (next == c))
      result += get();
    break;
  }
  
#ifdef TRACE
  cout << "retourne '" << result << "'\n";
#endif
  return QCString(result);
}

char Lex::bypass_operator(int c)
{
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
  case '&':
  case '+':
  case '|':
    if ((next == '=') || (next == c))
      get();
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
      if (c == '\n')
	context.line_number += 1;
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
    int c = get();
    
    switch (c) {
    case EOF:
      return;
    case '\\':
      c = get();
      if (c == '\n')
	context.line_number += 1;
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
      return QCString(result += c);
    default:
      result += c;
    }
  }
}

void Lex::bypass_character()
{
  for (;;) {
    int c = get();
    
    switch (c) {
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
  char * pointer = context.pointer;
	  
  for (;;) {
    switch (read_word_bis()) {
    case 0:
      result = 0;
      return result;
    case ']':
      {
	char c = *context.pointer;
	
	*context.pointer = 0;
	result += pointer;
	*context.pointer = c;
#ifdef TRACE
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
    switch (read_word_bis()) {
    case 0:
    case ']':
      return;
    default:
      break;
    }
  }
}

QCString Lex::read_word()
{
  QString result;
  
  if (!context.reread.isEmpty()) {
    result = context.reread;
    context.reread = QString::null;
  }
  else {
    for (;;) {
      int c = get();
      
#ifdef TRACE
      //cout << "deja \"" << result << "\", '" << ((char) c) << "'\n";
#endif
      if (c == EOF)
	break;
      else if (Separators.find(c) == -1)
	result += c;
      else if (! result.isEmpty()) {	
	unget();
	break;
      }
      else {
	switch (c) {
	case '#':
	  bypass_cpp_comment();
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
	    get();	// second '/'
	    bypass_cpp_comment();
	    break;
	  case '*':
	    bypass_c_comment();
	    break;
	  case '=':
	    get();
#ifdef TRACE
	    cout << "retourne '/='\n";
#endif
	    return "/=";
	  default:
#ifdef TRACE
	    cout << "retourne '/'\n";
#endif
	    return "/";
	  }
	  break;
	case '\n':
	  context.line_number += 1;
	  break;
	default:
	  if (c > ' ')
	    return manage_operator(result, c);
	  break;
	}
      }
    }
  }
#ifdef TRACE
  if (result.isEmpty())
    cout << "EOF\n";
  else
    cout << "retourne '" << result << "'\n";
#endif
  return QCString(result);
}

char Lex::read_word_bis()
{
  if (!context.reread.isEmpty()) {
    char result = context.reread[0].latin1();
    
    context.reread = QString::null;
    return result;
  }
  else {
    char result = 0;
    int c;
    
    for (;;) {
      c = get();
      
#ifdef TRACE
      //cout << "deja \"" << result << "\", '" << ((char) c) << "'\n";
#endif
      if (c == EOF)
	return result;
      else if (Separators.find(c) == -1) {
	if (result == 0)
	  result = (char) c;
      }
      else if (result != 0) {	
	unget();
	return result;
      }
      else {
	switch (c) {
	case '#':
	  bypass_cpp_comment();
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
	    get();	// second '/'
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
	case '\n':
	  context.line_number += 1;
	  break;
	default:
	  if (c > ' ')
	    return bypass_operator(c);
	}
      }
    }
  }
}

void Lex::finish_line()
{
  for (;;) {
    int c = get();
      
    switch (c) {
    case '#':
      bypass_cpp_comment();
      break;
    case '/':
      switch (peek()) {
      case '/':
	get();	// second '/'
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
      context.line_number += 1;
      // no break
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
  QCString result = QCString(context.comments);
  
  context.comments = QString::null;
  return result;
}

QCString Lex::get_comments(QCString & co) 
{
  QCString result = QCString(context.comments);
  
  context.comments = QString::null;
  
  return (co.isEmpty())
    ? result
    : co += "\n" + result;
}

QCString Lex::get_description() 
{
  QCString result = QCString(context.description);
  
  context.description = QString::null;
  return result;
}

QCString Lex::get_description(QCString & co) 
{
  QCString result = QCString(context.description);
  
  context.description = QString::null;
  
  return (co.isEmpty())
    ? result
    : co += "\n" + result;
}

void Lex::clear_comments()
{
  context.comments = QString::null;
}

bool Lex::identifierp(const char * s)
{
  return (Separators.find(*s) == -1);
}

void Lex::mark() {
  context.mark = context.pointer;
}

QCString Lex::region() {
  char c = *context.pointer;
  
  *context.pointer = 0;
  
  QCString result = context.mark;
  
  *context.pointer = c;
  
  return result;
}

//

void Lex::syntax_error(QCString s)
{
  PhpCatWindow::trace(QCString("<font face=helvetica>syntax error in <i> ")
			+ QCString(context.filename) + "</i> line " +
			QCString().setNum(context.line_number) + " <b>"
			+ s + "</b></font><br>"); 
  
#ifdef TRACE
  cout << "ERROR IN " << context.filename
    << " LINE " << context.line_number << " : " << s << '\n';
#endif
}

void Lex::premature_eof()
{
  PhpCatWindow::trace(QCString("<font face=helvetica>syntax error in <i> ")
		       + QCString(context.filename) + "</i> line " +
		       QCString().setNum(context.line_number) +
		       " <b>premature eof</b></font><br>"); 
  
#ifdef TRACE
  cout << "SYNTAX ERROR IN " << context.filename 
    << " LINE " << context.line_number << " : premature eof\n";
#endif
}

void Lex::error_near(QCString s)
{
  PhpCatWindow::trace(QCString("<font face=helvetica>syntax error in <i> ")
		       + QCString(context.filename) + "</i> line " +
		       QCString().setNum(context.line_number) + " <b>near <font color =\"red\">"
		       + quote(s) + "</font></b></font><br>"); 
  
#ifdef TRACE
  cout << "SYNTAX ERROR IN " << context.filename
    << " LINE " << context.line_number << " : near '" << s << "'\n";
#endif
}

// allows a string to be written as it is by an html writer

QCString Lex::quote(QCString s)
{
  QCString result;
  const char * p = s;
  
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

//

static QCString get_next_word(QCString s, int & index, int & index2)
{
  QCString result;
  const char * p = ((const char *)  s) + index;
  
  for (;;) {
    // bypass spaces before word
    switch (*p) {
    case ' ':
    case '\t':
      index += 1;
      p += 1;
      break;
    case '\r':
    case '\n':
    case 0:
      index2 = index;
      return result;
    default:
      {
	index2 = index;
	
	for (;;) {
	  // search for end of word
	  switch (*p) {
	  case ' ':
	  case '\t':
	  case '\r':
	  case '\n':
	  case 0:
	    if (index2 != index)
	      result = s.mid(index, index2 - index);
	    return result;
	  default:
	    index2 += 1;
	    p += 1;
	    break;
	  }
	}
      }
    }
  }
}

QCString value_of(QCString s, QCString k, int & index)
{
  index = s.find(k);
  
  if (index == -1) {
    QCString result;
    
    return result;
  }
  else {
    int index2;
    
    index += k.length();
    return get_next_word(s, index, index2);
  }
}

QCString value_of(QCString s, QCString k, int & index,
		  QCString & next, int & index2)
{
  QCString result;
  
  index = s.find(k, index);
  
  if (index != -1) {
    index += k.length();
    result = get_next_word(s, index, index2);
    
    if (! result.isEmpty()) {
      int index3;
      
      next = get_next_word(s, index2, index3);
    }
  }
  
  return result;
}

