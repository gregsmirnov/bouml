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

#ifndef MY_STR_H
#define MY_STR_H

// I consider that QString and QCString are too memory expensive

#include <qstrlist.h>
#include <qstring.h>

class SharedStr {
  public:
    SharedStr() { p = Empty; };
    SharedStr(const SharedStr & s) { p = s.p; };
    bool isEmpty() const { return *p == 0; };
    unsigned int length() const;
    int find(const char *, int index = 0) const;
    int find(int c, int index = 0) const;
    
    SharedStr & operator=(const SharedStr & s) { p = s.p; return *this; };
    SharedStr & operator=(const char *);
    SharedStr & operator=(const QString &);
    SharedStr & operator=(const QCString &);
    operator const char *() const { return p; };
    operator QString() const { return p; };
    operator QCString() const { return p; };
    
    //static void statistics();
    
  protected:
    char * p;
    static char Empty[1];
#define SHAREDDICTSIZE 128
    static QStrList shared[SHAREDDICTSIZE];
    
    char * assign(const char *, int len);
};

bool operator==(const SharedStr & s1, const SharedStr & s2);
bool operator==(const SharedStr & s1, const char * s2);
bool operator==(const char * s1, const SharedStr & s2);
bool operator==(const SharedStr & s1, const QString & s2);
bool operator==(const QString & s1, const SharedStr & s2);

bool operator!=(const SharedStr & s1, const SharedStr & s2);
bool operator!=(const SharedStr & s1, const char * s2);
bool operator!=(const char * s1, const SharedStr & s2);
bool operator!=(const SharedStr & s1, const QString & s2);
bool operator!=(const QString & s1, const SharedStr & s2);

// may be shared

class MayBeSharedStr : public SharedStr {
  private:
    MayBeSharedStr & operator=(const MayBeSharedStr &) {
      // must not be used, use assign operation
      extern MayBeSharedStr & illegal_MayBeSharedStr_usage();
      return illegal_MayBeSharedStr_usage();
    };
    MayBeSharedStr & operator=(const char *) {
      // must not be used, use assign operation
      extern MayBeSharedStr & illegal_MayBeSharedStr_usage();
      return illegal_MayBeSharedStr_usage();
    };
    MayBeSharedStr & operator=(const QString &) {
      // must not be used, use assign operation
      extern MayBeSharedStr & illegal_MayBeSharedStr_usage();
      return illegal_MayBeSharedStr_usage();
    };
    MayBeSharedStr & operator=(const QCString &) {
      // must not be used, use assign operation
      extern MayBeSharedStr & illegal_MayBeSharedStr_usage();
      return illegal_MayBeSharedStr_usage();
    };
    
  public:
    MayBeSharedStr() : SharedStr() {};
    MayBeSharedStr(const MayBeSharedStr &);
    ~MayBeSharedStr();
    operator const char *() const { return p; };
    operator QString() const { return p; };
    operator QCString() const { return p; };
    
    char * assign(const char *, bool share);
    char * assign(const QString &, bool share);
    char * assign(const QCString &, bool share);
    
  protected:
    char * assign(const char * s, int len, bool share);
};

// never shared

class MyStr {    
  public:
    MyStr() { p = 0; };
    MyStr(const MyStr &);
    MyStr(const QString &);
    MyStr(const char *);
    ~MyStr();
    MyStr & operator=(const MyStr &);
    MyStr & operator=(const char *);
    MyStr & operator=(const QString &);
    MyStr & operator=(const QCString &);
    operator const char *() const { return (p != 0) ? p : ""; };
    operator QString() const { return p; };
    operator QCString() const { return p; };
    bool isEmpty() const { return p == 0; };
    unsigned int length() const;
    
  protected:
    char * p;
    
    void assign(const char *, int len);
};

bool operator==(const MyStr & s1, const char * s2);
bool operator==(const char * s1, const MyStr & s2);
bool operator==(const MyStr & s1, const QString & s2);
bool operator==(const QString & s1, const MyStr & s2);

bool operator!=(const MyStr & s1, const char * s2);
bool operator!=(const char * s1, const MyStr & s2);
bool operator!=(const MyStr & s1, const QString & s2);
bool operator!=(const QString & s1, const MyStr & s2);

QString operator+(const MyStr & s1, const char * s2);
QString operator+(const char * s1, const MyStr & s2);
QString operator+(const MyStr & s1, const QString & s2);
QString operator+(const QString & s1, const MyStr & s2);

#endif
