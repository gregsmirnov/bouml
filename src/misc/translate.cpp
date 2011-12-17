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

#include <stdio.h>
#include <qdict.h>
#include <qmessagebox.h>

#include "translate.h"

static QDict<QString> * CurrentTranslation = 0;
static QString CurrentLang;

QString current_lang()
{
  return CurrentLang;
}

void set_lang(QString l)
{
  if (l != CurrentLang) {
    if (l.isEmpty()) {
      delete CurrentTranslation;
      CurrentTranslation = 0;
      CurrentLang = "";
    }
    else {
      FILE * fp = fopen((const char *) l, "rb");
      
      if (fp == 0) {
	QMessageBox::critical(0, "Bouml",
			      "cannot open file " +
			      l +
			      "\n, still use " + 
			      ((CurrentLang.isEmpty()) ? "english" : CurrentLang));

	return;
      }
      
      CurrentLang = l;
      
      if (CurrentTranslation == 0) {
	CurrentTranslation = new QDict<QString>(997);
	CurrentTranslation->setAutoDelete(TRUE);
      }
      else
	CurrentTranslation->clear();
      
      int len, c, i;
      
      while ((len = fgetc(fp)) != EOF) {
	len += (fgetc(fp) << 8);
	
	QString en((const QChar *) 0, (unsigned) len);
	
	for (i = 0; i != len; i += 1) {
	  c = fgetc(fp);
	  
	  en[i] = QChar(fgetc(fp), c);
	}
	
	len = fgetc(fp);
	len += (fgetc(fp) << 8);
	
	if (len != 0) {
	  QString * s = new QString((const QChar *) 0, (unsigned) len);
	  
	  for (i = 0; i != len; i += 1) {
	    c = fgetc(fp);
	    
	    s->at(i) = QChar(fgetc(fp), c);
	  }
	  
	  CurrentTranslation->insert(en, s);
	}
      }
      
      fclose(fp);
    }
  }
}

static void translate(QString & s)
{
  if (CurrentTranslation == 0)
    return;
  
  static QChar c1(' ');
  static QChar c2(':');
  static QChar c3('\n');
  
  QString k, b, e;
  int ln = s.length();
  int index1 = 0;
  
  while (index1 != ln) {
    QCharRef c = s.at(index1);
    
    if ((c != c1) && (c != c2) && (c != c3))
      break;
    index1 += 1;
  }
  
  int index2 = ln;
  
  while (--index2 > index1) {
    QCharRef c = s.at(index2);
    
    if ((c != c1) && (c != c2) && (c != c3))
      break;
  }
   
  if (index1 != 0) {
    // some removed chars at beginning
    b = s.left(index1);
    
    if (index2 != (ln - 1)) {
      // some removed chars at end
      e = s.mid(index2 + 1);
      k = s.mid(index1, index2 - index1 + 1);
    }
    else
      k = s.mid(index1);
  }
  else if (index2 != (ln - 1)) {
    // some removed chars at end
      // some removed chars at end
      e = s.mid(index2 + 1);
      k = s.mid(0, index2 + 1);
  }
  else
    k = s;

  QString * s2 = CurrentTranslation->find(k);
  
  if (s2 != 0)
    s = b + *s2 + e;
}

QString TR(QString s)
{
  translate(s);
  return s;
}

QString TR(QString s, QString a1)
{
  translate(s); 
  
  int index;
  
  if ((index = s.find("%1")) != -1)
    s.replace(index, 2, a1);
  
  return s;
}

QString TR(QString s, QString a1, QString a2)
{
  translate(s);  
  
  int index;
  
  if ((index = s.find("%1")) != -1)
    s.replace(index, 2, a1);
  
  if ((index = s.find("%2")) != -1)
    s.replace(index, 2, a2);
  
  return s;
}

QString TR(QString s, QString a1, QString a2, QString a3)
{
  translate(s); 
  
  int index;
  
  if ((index = s.find("%1")) != -1)
    s.replace(index, 2, a1);
  
  if ((index = s.find("%2")) != -1)
    s.replace(index, 2, a2);
  
  if ((index = s.find("%3")) != -1)
    s.replace(index, 2, a3);
  
  return s;
}
