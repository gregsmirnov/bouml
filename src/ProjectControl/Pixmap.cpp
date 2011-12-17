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





#include "Pixmap.h"

QPixmap * ProtectedPackageIcon;
QPixmap * FreePackageIcon;
QPixmap * UserPackageIcon;
QPixmap * OtherPackageIcon;
QPixmap * RoProtectedPackageIcon;
QPixmap * RoFreePackageIcon;
QPixmap * RoUserPackageIcon;
QPixmap * RoOtherPackageIcon;

//

void init_pixmaps()
{
  // browser icons
  
#include "package_xpm.xpm"

  ProtectedPackageIcon = new QPixmap((const char **) protected_package_xpm);
  FreePackageIcon = new QPixmap((const char **) free_package_xpm);
  UserPackageIcon = new QPixmap((const char **) user_package_xpm);
  OtherPackageIcon = new QPixmap((const char **) other_package_xpm);
  
  RoProtectedPackageIcon = new QPixmap((const char **) ro_protected_package_xpm);
  RoFreePackageIcon = new QPixmap((const char **) ro_free_package_xpm);
  RoUserPackageIcon = new QPixmap((const char **) ro_user_package_xpm);
  RoOtherPackageIcon = new QPixmap((const char **) ro_other_package_xpm);
}
