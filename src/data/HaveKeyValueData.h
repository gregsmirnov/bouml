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

#ifndef HAVEKEYVALUEDATA_H
#define HAVEKEYVALUEDATA_H

#include <qstring.h>

class QTextStream;
class KeyValueData;
class ToolCom;

// this class manage a table of couple key-value

class HaveKeyValueData {
  protected:
    KeyValueData * keyvalues;	// remark : do NOT use QArray
    unsigned nkeyvalues;
    
  public:
    HaveKeyValueData();
    virtual ~HaveKeyValueData();
    
    const char * get_key(int rank) const;
    void set_key(int rank, const char * k);
    const char * get_value(int rank) const;
    const char * get_value(const char * k) const;
    void set_value(int rank, const char * v);
    unsigned get_n_keys() const { return nkeyvalues; };
    void remove_key_value(unsigned index);
    void remove_key_value(const char * k);
    void set_n_keys(unsigned n);	// the old key-values are lost
    void resize_n_keys(unsigned n, bool realloc); // the old key-values are not lost
    
    void send_def(ToolCom * com) const;
    void set_value(const char * k, const char * v);
    
    void save(QTextStream &) const;
    void read(char * &, char * & k);
};

#endif
