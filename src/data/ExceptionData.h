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

#ifndef EXCEPTIONDATA_H
#define EXCEPTIONDATA_H

#include "AType.h"

class QTextStream;

class ExceptionData {
  protected:
    AType type;
  public:
    ExceptionData(){};
  
    const AType & get_type() const { return type; };
    void set_type(const AType & t);

    void send_uml_def(ToolCom * com);
    
    void save(QTextStream &, QString & warning) const;
    void read(char * &, char * &);
};

#endif
