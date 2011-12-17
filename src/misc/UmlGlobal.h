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

#ifndef UMLGLOBAL_H
#define UMLGLOBAL_H

#include <qfont.h>
#include <qcolor.h>

#include "UmlEnum.h"

extern QFont NormalFont;
extern QFont BoldFont;
extern QFont ItalicFont;
extern QFont BoldItalicFont;
extern QFont UnderlineFont;
extern QFont BoldUnderlineFont;
extern QFont StrikeOutFont;

extern QColor UmlWhiteColor;
extern QColor UmlLightYellowColor;
extern QColor UmlYellowColor;
extern QColor UmlMediumYellowColor;
extern QColor UmlDarkYellowColor;
extern QColor UmlLightBlueColor;
extern QColor UmlBlueColor;
extern QColor UmlLightMediumBlueColor;
extern QColor UmlMediumBlueColor;
extern QColor UmlDarkBlueColor;
extern QColor UmlLightGreenColor;
extern QColor UmlGreenColor;
extern QColor UmlLightMediumGreenColor;
extern QColor UmlMediumGreenColor;
extern QColor UmlDarkGreenColor;
extern QColor UmlRedColor;
extern QColor UmlVeryLightGrayColor;
extern QColor UmlLightGrayColor;
extern QColor UmlGrayColor;
extern QColor UmlDarkGrayColor;
extern QColor UmlBlackColor;

extern QColor UmlLightRedColor;
extern QColor UmlMidRedColor;
extern QColor UmlVeryLightOrangeColor;
extern QColor UmlLightOrangeColor;
extern QColor UmlOrangeColor;
extern QColor UmlDarkOrangeColor;
extern QColor UmlLightMagentaColor;
extern QColor UmlMagentaColor;
extern QColor UmlMidMagentaColor;
extern QColor UmlDarkMagentaColor;

extern const QColor & color(UmlColor);

void init_font();
void resize_font(int);

#endif
