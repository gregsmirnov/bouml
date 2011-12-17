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





#include <qtextstream.h> 

#include "HaveKeyValueData.h"
#include "KeyValueData.h"
#include "myio.h"
#include "ToolCom.h"

HaveKeyValueData::HaveKeyValueData() : keyvalues(0), nkeyvalues(0) {
}

HaveKeyValueData::~HaveKeyValueData() {
  if (keyvalues)
    delete [] keyvalues;
}

const char * HaveKeyValueData::get_key(int rank) const {
  return keyvalues[rank].get_key();
}

void HaveKeyValueData::set_key(int rank, const char * k) {
  keyvalues[rank].set_key(k);
}

const char * HaveKeyValueData::get_value(int rank) const {
  return keyvalues[rank].get_value();
}

const char * HaveKeyValueData::get_value(const char * k) const {
  unsigned index;
  
  for (index = 0; index != nkeyvalues; index += 1)
    if (strcmp(keyvalues[index].get_key(), k) == 0)
      return keyvalues[index].get_value();

  return 0;
}

void HaveKeyValueData::set_value(int rank, const char * v) {
  keyvalues[rank].set_value(v);
}

void HaveKeyValueData::remove_key_value(unsigned index) {
  nkeyvalues -= 1;
  keyvalues[index].set_key(keyvalues[nkeyvalues].get_key());
  keyvalues[index].set_value(keyvalues[nkeyvalues].get_value());
}

void HaveKeyValueData::remove_key_value(const char * k) {
  unsigned index;
  
  for (index = 0; index != nkeyvalues; index += 1) {
    if (strcmp(keyvalues[index].get_key(), k) == 0) {
      nkeyvalues -= 1;
      keyvalues[index].set_key(keyvalues[nkeyvalues].get_key());
      keyvalues[index].set_value(keyvalues[nkeyvalues].get_value());
      break;
    }
  }
}

void HaveKeyValueData::set_n_keys(unsigned n) {
  if (n > nkeyvalues) {
    if (keyvalues)
      delete [] keyvalues;
    keyvalues = new KeyValueData[n];
  }
  
  nkeyvalues = n;
}

void HaveKeyValueData::resize_n_keys(unsigned n, bool realloc) {
  if (realloc) {
    KeyValueData * kv = new KeyValueData[n];
    
    if (keyvalues) {
      for (unsigned index = 0; index != nkeyvalues; index += 1) {
	kv[index].set_key(keyvalues[index].get_key());
	kv[index].set_value(keyvalues[index].get_value());
      }
      delete [] keyvalues;
    }
    keyvalues = kv;
  }
  
  nkeyvalues = n;
}

void HaveKeyValueData::send_def(ToolCom * com) const {
  com->write_unsigned(nkeyvalues);
  
  for (unsigned index = 0; index != nkeyvalues; index += 1) {
    com->write_string(keyvalues[index].get_key());
    com->write_string(keyvalues[index].get_value());
  }
}

void HaveKeyValueData::set_value(const char * k, const char * v) {
  unsigned index;
  
  for (index = 0; index != nkeyvalues; index += 1) {
    if (strcmp(keyvalues[index].get_key(), k) == 0) {
      keyvalues[index].set_value(v);
      return;
    }
  }
  
  // value must be added
  KeyValueData * t = new KeyValueData[nkeyvalues + 1];
  
  for (index = 0; index != nkeyvalues; index += 1) {
    t[index].set_key(keyvalues[index].get_key());
    t[index].set_value(keyvalues[index].get_value());
  }
  
  t[index].set_key(k);
  t[index].set_value(v);
  
  delete [] keyvalues;
  keyvalues = t;
  nkeyvalues += 1;
}

void HaveKeyValueData::save(QTextStream & st) const {
  if (nkeyvalues) {
    nl_indent(st);
    st << "ncouples " << nkeyvalues;
    
    for (unsigned i = 0; i != nkeyvalues; i += 1) {
      nl_indent(st);
      st << "key ";
      save_string(keyvalues[i].get_key(), st);
      st << " value ";
      save_string(keyvalues[i].get_value(), st);
    }
  }
}

void HaveKeyValueData::read(char * & st, char * & k) {
  if (!strcmp(k, "ncouples")) {
    set_n_keys(read_unsigned(st));
    
    for (unsigned i = 0; i != nkeyvalues; i += 1) {
      read_keyword(st, "key");
      keyvalues[i].set_key(read_string(st));
      read_keyword(st, "value");
      keyvalues[i].set_value(read_string(st));
    }
    
    k = read_keyword(st);
  }
  else
    set_n_keys(0);
}
