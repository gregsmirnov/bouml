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





#include <stdlib.h>
#include <stdio.h>
#include <qmap.h>
#include <qobject.h>
#include <qfile.h>
#include <qdir.h>
#include <qtextstream.h>

#include "Shortcut.h"
#include "strutil.h"
#include "myio.h"
#include "mu.h"

static QMap<QString, int> NameToCode;
static QString CodeToName[256];
static QStringList ShortcutCmds;
static QStringList ShortcutKeys;
static QMap<QString, QString> Shortcuts;

void Shortcut::init(bool conv)
{
#define MEMOKEY(name, code) \
	NameToCode[#name] = code; \
	CodeToName[code - 0x1000 + 128] = #name; \
	ShortcutKeys.append(#name)

  MEMOKEY(Escape, 0x1000);
  MEMOKEY(Tab, 0x1001);
  MEMOKEY(Backtab, 0x1002);
  MEMOKEY(Backspace, 0x1003);
  MEMOKEY(Return, 0x1004);
  MEMOKEY(Enter, 0x1005);
  MEMOKEY(Insert, 0x1006);
  MEMOKEY(Delete, 0x1007);
  MEMOKEY(Pause, 0x1008);
  MEMOKEY(Print, 0x1009);
  MEMOKEY(SysReq, 0x100a);
  MEMOKEY(Home, 0x1010);
  MEMOKEY(End, 0x1011);
  MEMOKEY(Left, 0x1012);
  MEMOKEY(Up, 0x1013);
  MEMOKEY(Right, 0x1014);
  MEMOKEY(Down, 0x1015);
  MEMOKEY(PageUp, 0x1016);
  MEMOKEY(PageDown, 0x1017);
			    
#undef MEMOKEY
  
  int i;
  int code;
  char name[4];
  
  for (i = 1, code = 0x1030; i != 36; i += 1, code += 1) {
    sprintf(name, "F%d", i);
    NameToCode[name] = code;
    CodeToName[code - 0x1000 + 128] = name;
    ShortcutKeys.append(name);
  }
  
  name[1] = 0;
  
  for (i = '!'; i != 'a'; i += 1) {
    name[0] = i;
    NameToCode[name] = i;
    CodeToName[i] = name;
    ShortcutKeys.append(name);
  }
  
  for (i = '{'; i != 0x7f; i += 1) {
    name[0] = i;
    NameToCode[name] = i;
    CodeToName[i] = name;
    ShortcutKeys.append(name);
  }
  
  //
  
  static const char * cmds[] = {
    "Add related elements",
    "Align bottom",
    "Align center",
    "Align center horizontaly",
    "Align center verticaly",
    "Align left",
    "Align right",
    "Align top",
    "Arrow geometry",
    "Browser search",
    "Close",
    "Quit",
    "Copy",
    "Create source artifact",
    "Cut",
    "Delete",
    "Duplicate",
    "Edit",
    "Edit C++ body",
    "Edit class settings",
    "Edit default stereotypes",
    "Edit drawing settings",
    "Edit generation settings",
    "Edit reverse/roundtrip settings",
    "Edit Java body",
    "Edit Php body",
    "Edit Python body",
    "Generate C++",
    "Generate Idl",
    "Generate Java",
    "Generate Php",
    "Generate Python",
    "Go up",
    "Go down",
    "Lower",
    "Mark",
    "Menu",
    "Move down",
    "Move left",
    "Move right",
    "Move up",
    "New activity",
    "New activity action",
    "New activity diagram",
    "New actor",
    "New artifact",
    "New attribute",
    "New class",
    "New class diagram",
    "New class instance",
    "New class view",
    "New communication diagram",
    "New component",
    "New component diagram",
    "New component view",
    "New deployment diagram",
    "New deployment view",
    "New expansion node",
    "New expansion region",
    "New extra member",
    "New get and set operation",
    "New get operation",
    "New interruptible activity region",
    "New item",
    "New node",
    "New object diagram",
    "New object node",
    "New operation",
    "New package",
    "New parameter",
    "New parameter set",
    "New partition",
    "New pin",
    "New region",
    "New sequence diagram",
    "New set operation",
    "New state",
    "New state diagram",
    "New state machine",
    "New submachine",
    "New use case",
    "New use case diagram",
    "New use case view",
    "Open project",
    "Optimal scale",
    "Optimal window size",
    "Paste",
    "Print",
    "Referenced by",
    "Redo",
    "Remove from diagram",
    "Roundtrip C++ operation body",
    "Roundtrip Java operation body",
    "Roundtrip Php operation body",
    "Roundtrip Python operation body",
    "Same drawing settings",
    "Same width",
    "Same height",
    "Same size",
    "Save",
    "Save as",
    "See C++ header file",
    "See C++ source file",
    "See Idl source file",
    "See Java source file",
    "See Php source file",
    "See Python source file",
    "Select all",
    "Select associated artifact",
    "Select associated deployment view",
    "Select in browser",
    "Select linked items",
    "Select target",
    "Show",
    "Undelete",
    "Undelete recursively",
    "Undo",
    "Unmark",
    "Unmark all",
    "Upper",
    "Zoom +",
    "Zoom -",
    "Zoom 100%"
  };
  
  for (i = 0; i != sizeof(cmds)/sizeof(cmds[0]); i += 1)
    ShortcutCmds.append(cmds[i]);
			    
  load(conv);
}

int codeToIndex(int c)
{
  return (c >= 0x1000) ? c - 0x1000 + 128 : c;
}

int indexToCode(int i)
{
  return (i >= 128) ? i - 128 + 0x1000 : i;
}

QString codeToName(int c)
{
  c = codeToIndex(c);

  return ((c >= 0) && (c < 256))
    ? CodeToName[c]
    : QString::null;	// unexpected char code
}

int nameToCode(QString s)
{
  return NameToCode[s];
}

//

const QStringList & Shortcut::cmds()
{
  return ShortcutCmds;
}

const QStringList & Shortcut::keys()
{
  return ShortcutKeys;
}

//

// each key is <flags><key name>, where <flags> is a space ored
// with 1 if shift, 2 if control and 4 if alt

const QMap<QString, QString> & Shortcut::shortcuts()
{
  return Shortcuts;
}

QString Shortcut::extract(QString k, BooL & shift, BooL & ctrl, BooL & alt)
{
  const char * p = k;
  
  shift = ((p[0] & 1) != 0);
  ctrl = ((p[0] & 2) != 0);
  alt = ((p[0] & 4) != 0);
  
  return k.mid(1);
}    

QString Shortcut::shortcut(int key, int buttons)
{
  char flags[2];
  
  flags[0] = ' ';
  flags[1] = 0;
  
  if (buttons & ::Qt::ShiftButton) flags[0] += 1;
  if (buttons & ::Qt::ControlButton) flags[0] += 2;
  if (buttons & ::Qt::AltButton) flags[0] += 4;
  
  QMap<QString, QString>::ConstIterator it =
    Shortcuts.find(flags + codeToName(key));
  
  return (it == Shortcuts.end()) ? QString::null : *it;
}

void Shortcut::raz()
{
  Shortcuts.clear();
}

void Shortcut::add(QString k, bool shift, bool ctrl, bool alt, QString s)
{
  char flags[2];
  
  flags[0] = ' ';
  flags[1] = 0;
  
  if (shift) flags[0] += 1;
  if (ctrl) flags[0] += 2;
  if (alt) flags[0] += 4;

  Shortcuts[flags + k] = s;
}

//

void Shortcut::save()
{
  // note : QFile fp(QDir::home().absFilePath(".bouml_shortcuts")) doesn't work
  // if the path contains non latin1 characters, for instance cyrillic !
  QString s = homeDir().absFilePath(".bouml_shortcuts");
  FILE * fp = fopen((const char *) s, "w");
  
  if (fp != 0) {
    QMap<QString, QString>::ConstIterator it;
    
    for (it = Shortcuts.begin(); it != Shortcuts.end(); ++it) {
      QString k = it.key();
      QString d = it.data();
      const char * p = k;
      
      if (p[0] & 1)
	fwrite("shift ", 1, 6, fp);
      if (p[0] & 2)
	fwrite("control ", 1, 8, fp);
      if (p[0] & 4)
	fwrite("alt ", 1, 4, fp);
      fwrite(p+1, 1, k.length() - 1, fp);
      fputc(' ', fp);
      fwrite((const char *) d, 1, d.length(), fp);
      fputc('\n', fp);
    }
    
    fclose(fp);
  }
}

static QString shortcut_file_path(bool conv)
{
  if (! conv)
    return ".bouml_shortcuts";
  
  char * v = getenv("BOUML_ID");	// yes !
  int uid;
  
  if ((v == 0) || (sscanf(v, "%d", &uid) != 1))
    return "";
  
  char f[64];
  
  sprintf(f, ".bouml_shortcuts_%d", uid);
  return f;
}

void Shortcut::load(bool conv)
{
  // default shortcuts
  add("Delete", FALSE, FALSE, FALSE, "Remove from diagram");
  add("Left",	FALSE, FALSE, FALSE, "Move left");
  add("Right",	FALSE, FALSE, FALSE, "Move right");
  add("Down",	FALSE, FALSE, FALSE, "Move down");
  add("Up",	FALSE, FALSE, FALSE, "Move up");
  add("Return",	FALSE, FALSE, FALSE, "Menu");
  add("D",	FALSE, TRUE, FALSE, "Delete");
  add("A",	FALSE, TRUE, FALSE, "Select all");
  add("C",	FALSE, TRUE, FALSE, "Copy");
  add("V",	FALSE, TRUE, FALSE, "Paste");
  add("X",	FALSE, TRUE, FALSE, "Cut");
  add("U",	FALSE, TRUE, FALSE, "Undo");
  add("Z",	FALSE, TRUE, FALSE, "Undo");
  add("R",	FALSE, TRUE, FALSE, "Redo");
  add("Y",	FALSE, TRUE, FALSE, "Redo");
  add("S",	FALSE, TRUE, FALSE, "Save");
  add("W",	FALSE, TRUE, FALSE, "Close");
  add("Q",	FALSE, TRUE, FALSE, "Quit");
  add("L",	FALSE, TRUE, FALSE, "Arrow geometry");
  add("F",	FALSE, TRUE, FALSE, "Browser search");
  add("O",	FALSE, TRUE, FALSE, "Open project");
  add("P",	FALSE, TRUE, FALSE, "Print");
  add("S",	TRUE, TRUE, FALSE, "Save as");
  
  QString f = shortcut_file_path(conv);
  
  if (!f.isEmpty())  {
    // note : QFile fp(QDir::home().absFilePath(f)) doesn't work
    // if the path contains non latin1 characters, for instance cyrillic !
    QString s = homeDir().absFilePath(f);
    FILE * fp = fopen((const char *) s, "r");
    
    if (fp != 0) {
      char line[512];
      
      while (fgets(line, sizeof(line) - 1, fp) != 0) {
	remove_crlf(line);

	bool shift = FALSE;
	bool control = FALSE;
	bool alt = FALSE;
	int index = 0;
	QString ln = line;
	
	if (!strncmp(line, "shift ", 6)) {
	  shift = TRUE;
	  index = 6;
	}
	
	if (!strncmp(line+index, "control ", 8)) {
	  control = TRUE;
	  index += 8;
	}
	
	if (!strncmp(line+index, "alt ", 4)) {
	  alt = TRUE;
	  index += 4;
	}
	
	int index2 = ln.find(' ', index);
	
	if (index2 != -1) {
	  QString s = ln.mid(index2 + 1);
	  QString k = ln.mid(index, index2 - index);
	  
	  if (s == "New collaboration diagram")
	    // old
	    s = "New communication diagram";
	  else if (s == "Remove from view")
	    // old
	    s = "Remove from diagram";
	  
	  if (k == "Suppr")
	    // old
	    k = "Delete";
	  
	  add(k, shift, control, alt, s);
	}
      }
      
      fclose(fp);
      
      if (conv)
	save();
      
      return;
    }
  }
}

