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

#ifndef SDDURATIONSUPPORT_H
#define SDDURATIONSUPPORT_H

#include <qlist.h>

class SdDurationCanvas;
class SdLifeLineCanvas;

class SdDurationSupport {
  public:
    virtual ~SdDurationSupport() {};
  
    virtual void add(SdDurationCanvas *) = 0;
    virtual void remove(SdDurationCanvas *) = 0;

    virtual void update_instance_dead() = 0;
    virtual void update_v_to_contain(SdDurationCanvas *, bool) = 0;
    virtual int sub_x(int sub_w) const = 0;
    virtual double min_y() const = 0;
    virtual double getZ() const = 0;

    virtual bool isaDuration() const = 0;
    virtual SdLifeLineCanvas * get_line() const = 0;
};

#endif
