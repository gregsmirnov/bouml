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

#ifndef SIMPLEDATA_H
#define SIMPLEDATA_H

#include "BasicData.h"

class SimpleData : public BasicData {
  Q_OBJECT
    
  protected:
    bool is_deleted;

  public:
    SimpleData() : BasicData(), is_deleted(FALSE) {};
    SimpleData(BasicData * model) : BasicData(model), is_deleted(FALSE) {};
    
    virtual bool deletedp() const;
    virtual void set_deletedp(bool y);
};

#endif
