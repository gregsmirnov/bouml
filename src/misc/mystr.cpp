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





#include <qstring.h>

#include "mystr.h"

char SharedStr::Empty[1] = { 0 };
QStrList SharedStr::shared[SHAREDDICTSIZE];

unsigned int SharedStr::length() const {
  return strlen(p);
}

int SharedStr::find(const char * s, int index) const
{
  char * ss = strstr(p + index, s);
  
  return (ss == 0) ? -1 : ss - p;
}

int SharedStr::find(int c, int index) const
{
  char * ss = strchr(p + index, c);
  
  return (ss == 0) ? -1 : ss - p;
}

SharedStr & SharedStr::operator=(const char * s) {
  if ((s == 0) || (*s == 0))
    p = Empty;
  else
    assign(s, strlen(s));
  
  return *this;
}

SharedStr & SharedStr::operator=(const QString & s) {
  if (s.isEmpty())
    p = Empty;
  else
    assign((const char *) s, s.length());
  
  return *this;
}

SharedStr & SharedStr::operator=(const QCString & s) {
  if (s.isEmpty())
    p = Empty;
  else
    assign((const char *) s, s.length());
  
  return *this;
}

char * SharedStr::assign(const char * s, int len) {
  QStrList & l = shared[len & (SHAREDDICTSIZE-1)];
  
  if (l.find(s) == -1)
    l.append(s);

  return p = l.current();
}

bool operator==(const SharedStr & s1, const SharedStr & s2)
{
  return (((const char *) s1) == ((const char *) s2)) ||
    (strcmp(s1, s2) == 0);
}

bool operator==(const SharedStr & s1, const char * s2)
{
  return (strcmp(((const char *) s1), s2) == 0);
}

bool operator==(const char * s1, const SharedStr & s2)
{
  return (strcmp(s1, ((const char *) s2)) == 0);
}

bool operator==(const SharedStr & s1, const QString & s2)
{
  return (strcmp(((const char *) s1), (const char *) s2) == 0);
}

bool operator==(const QString & s1, const SharedStr & s2)
{
  return (strcmp((const char *) s1, ((const char *) s2)) == 0);
}

bool operator!=(const SharedStr & s1, const SharedStr & s2)
{
  return (((const char *) s1) != ((const char *) s2)) &&
    (strcmp(((const char *) s1), ((const char *) s2)) != 0);
}

bool operator!=(const SharedStr & s1, const char * s2)
{
  return (strcmp(((const char *) s1), s2) != 0);
}

bool operator!=(const char * s1, const SharedStr & s2)
{
  return (strcmp(s1, ((const char *) s2)) != 0);
}

bool operator!=(const SharedStr & s1, const QString & s2)
{
  return (strcmp(((const char *) s1), (const char *) s2) != 0);
}

bool operator!=(const QString & s1, const SharedStr & s2)
{
  return (strcmp((const char *) s1, ((const char *) s2)) != 0);
}

#if 0
#include <stdio.h>

void SharedStr::statistics()
{
  int l[128];
  int n = 0;
  
  memset(l, 0, sizeof(l));
  
  int index = SHAREDDICTSIZE;
  
  while (index--) {
    int ln = shared[index].count();
    
    n += ln;
    l[(ln > 127) ? 127 : ln] += 1;
  }
  
  printf("%d strings\n", n);
  
  for (index = 0; index != 128; index += 1)
    if (l[index] != 0)
      printf("%d : %d\n", index, l[index]);
}
#endif

//

MayBeSharedStr::MayBeSharedStr(const MayBeSharedStr & s) : SharedStr() {
  if ((s.p[0] == 0) ||
      (shared[strlen(s.p) & (SHAREDDICTSIZE-1)].findRef(s.p) != -1))
    // shared
    p = s.p;
  else {
    int len = strlen(s.p) + 1;
    
    p = new char[len];
    memcpy(p, s.p, len);
  }
}

MayBeSharedStr::~MayBeSharedStr() {
  // free if necessary the old value
  if (*p && (shared[strlen(p) & (SHAREDDICTSIZE-1)].findRef(p) == -1))
    // not shared
    delete [] p;
}

char * MayBeSharedStr::assign(const char * s, bool share) {
  return assign(s, (s) ? strlen(s) : 0, share);
}

char * MayBeSharedStr::assign(const QString & s, bool share) {
  return assign((const char *) s, s.length(), share);
}

char * MayBeSharedStr::assign(const QCString & s, bool share) {
  return assign((const char *) s, s.length(), share);
}

char * MayBeSharedStr::assign(const char * s, int len, bool share) {
  // free if necessary the old value
  if (*p && (shared[strlen(p) & (SHAREDDICTSIZE-1)].findRef(p) == -1))
    // not shared
    delete [] p;
  
  if ((s == 0) || (*s == 0))
    p = Empty;
  else if (share)
    SharedStr::assign(s, len);
  else {
    p = new char[len + 1];
    memcpy(p, s, len + 1);
  }
    
  return p;
}

//

MyStr::MyStr(const MyStr & s) {
  if (s.p == 0)
    p = 0;
  else {
    int len = strlen(s.p) + 1;
  
    p = new char[len];
    memcpy(p, s, len);
  }
}

MyStr::MyStr(const QString & s) {
  int len = s.length();
  
  if (len == 0)
    p = 0;
  else {
    p = new char[len + 1];
    memcpy(p, s, len + 1);
  }
}

MyStr::MyStr(const char * s) {
  int len = strlen(s);
  
  if (len == 0)
    p = 0;
  else {
    p = new char[len + 1];
    memcpy(p, s, len + 1);
  }
}

MyStr::~MyStr() {
  if (p)
    delete [] p;
}

MyStr & MyStr::operator=(const MyStr & s) {
  assign(s.p, (s.p) ? strlen(s.p) : 0);
  
  return *this;
}

MyStr & MyStr::operator=(const char * s) {
  assign(s, (s) ? strlen(s) : 0);
  
  return *this;
}

MyStr & MyStr::operator=(const QString & s) {
  assign(s, s.length());
  
  return *this;
}

MyStr & MyStr::operator=(const QCString &s) {
  assign(s, s.length());
  
  return *this;
}

unsigned int MyStr::length() const {
  return (p == 0) ? 0 : strlen(p);
}

void MyStr::assign(const char * s, int len) {
  // free previous value if necessary
  if (p != 0)
    delete [] p;
  
  if (len == 0)
    p = 0;
  else {
    p = new char[len + 1];
    memcpy(p, s, len + 1);
  }
}

bool operator==(const MyStr & s1, const char * s2)
{
  return !strcmp((const char *) s1, s2);
}

bool operator==(const char * s1, const MyStr & s2)
{
  return !strcmp(s1, (const char *) s2);
}

bool operator==(const MyStr & s1, const QString & s2)
{
  return !strcmp((const char *) s1, (const char *) s2);
}

bool operator==(const QString & s1, const MyStr & s2)
{
  return !strcmp((const char *) s1, (const char *) s2);
}

bool operator!=(const MyStr & s1, const char * s2)
{
  return strcmp((const char *) s1, s2);
}

bool operator!=(const char * s1, const MyStr & s2)
{
  return strcmp(s1, (const char *) s2);
}

bool operator!=(const MyStr & s1, const QString & s2)
{
  return strcmp((const char *) s1, (const char *) s2);
}

bool operator!=(const QString & s1, const MyStr & s2)
{
  return strcmp((const char *) s1, (const char *) s2);
}

QString operator+(const MyStr & s1, const char * s2)
{
  return QString((const char *) s1) + s2;
}

QString operator+(const char * s1, const MyStr & s2)
{
  return s1 + QString((const char *) s2);
}

QString operator+(const MyStr & s1, const QString & s2)
{
  return QString((const char *) s1) + s2;
}

QString operator+(const QString & s1, const MyStr & s2)
{
  return s1 + QString((const char *) s2);
}
