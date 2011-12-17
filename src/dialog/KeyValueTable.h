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

#ifndef KEYVALUETABLE_H
#define KEYVALUETABLE_H

#include <qdict.h>
#include "StringTable.h"

class HaveKeyValueData;

class KeyValuesTable : public StringTable {
  public:
    KeyValuesTable(HaveKeyValueData * h, QWidget * parent, bool visit);
  
    virtual void init_row(int index);
				
    bool check_unique();
    void update(HaveKeyValueData * h);
    bool get_value(const char * key, QString & value);
    void remove(const char * key);

  private:
    QDict<QStringList> props;
};

#endif
