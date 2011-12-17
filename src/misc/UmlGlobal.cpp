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





#include <qapplication.h>

#include "UmlGlobal.h"
#include "strutil.h"

QFont NormalFont;
QFont BoldFont;
QFont ItalicFont;
QFont BoldItalicFont;
QFont UnderlineFont;
QFont BoldUnderlineFont;
QFont StrikeOutFont;

QColor UmlWhiteColor(0xff, 0xff, 0xff);
QColor UmlLightYellowColor(0xff, 0xff, 0xe0);
QColor UmlYellowColor(0xff, 0xff, 0xc0);
QColor UmlMediumYellowColor(0xff, 0xeb, 0x60);
QColor UmlDarkYellowColor(0xff, 0xd7, 0x0);
QColor UmlLightBlueColor(0xe0, 0xff, 0xff);
QColor UmlBlueColor(0xc0, 0xff, 0xff);
QColor UmlLightMediumBlueColor(0x90, 0xcc, 0xff);
QColor UmlMediumBlueColor(0x60, 0xaa, 0xff);
QColor UmlDarkBlueColor(0x0, 0xaa, 0xff);
QColor UmlLightGreenColor(0xdf, 0xff, 0xdf);
QColor UmlGreenColor(0xc0, 0xff, 0xc0);
QColor UmlLightMediumGreenColor(0x90, 0xe9, 0x90);
QColor UmlMediumGreenColor(0x60, 0xd4, 0x60);
QColor UmlDarkGreenColor(0x0, 0xaa, 0x0);
QColor UmlRedColor(0xff, 0x00, 0x00);
QColor UmlVeryLightGrayColor(0xf6, 0xf6, 0xf6);
QColor UmlLightGrayColor(0xed, 0xed, 0xed);
QColor UmlGrayColor(0xdc, 0xdc, 0xdc);
QColor UmlDarkGrayColor(0xcb, 0xcb, 0xcb);
QColor UmlBlackColor(0, 0, 0);

QColor UmlLightRedColor(0xff, 0xc0, 0xc0);
QColor UmlMidRedColor(0xc0, 0x00, 0x00);
QColor UmlVeryLightOrangeColor(0xff, 0xdc, 0xa8);
QColor UmlLightOrangeColor(0xff, 0xa8, 0x58);
QColor UmlOrangeColor(0xff, 0x80, 0x00);
QColor UmlDarkOrangeColor(0xc0, 0x58, 0x00);
QColor UmlLightMagentaColor(0xff, 0xc0, 0xff);
QColor UmlMagentaColor(0xff, 0x00, 0xff);
QColor UmlMidMagentaColor(0xc0, 0x00, 0xc0);
QColor UmlDarkMagentaColor(0x80, 0x00, 0x80);

void init_font()
{
  NormalFont = QApplication::font();
  if (! hasCodec())
    NormalFont.setFamily("Helvetica");

  BoldFont = NormalFont;
  BoldFont.setBold(TRUE);
  
  ItalicFont = NormalFont;
  ItalicFont.setItalic(TRUE);
  
  BoldItalicFont = ItalicFont;
  BoldItalicFont.setBold(TRUE);
  
  UnderlineFont = NormalFont;
  UnderlineFont.setUnderline(TRUE);
  
  BoldUnderlineFont = BoldFont;
  BoldUnderlineFont.setUnderline(TRUE);
  
  StrikeOutFont = NormalFont;
  StrikeOutFont.setStrikeOut(TRUE);
}

void resize_font(int i)
{
  NormalFont.setPointSizeFloat(i);
  BoldFont.setPointSizeFloat(i);
  ItalicFont.setPointSizeFloat(i);
  BoldItalicFont.setPointSizeFloat(i);
  UnderlineFont.setPointSizeFloat(i);
  BoldUnderlineFont.setPointSizeFloat(i);
  StrikeOutFont.setPointSizeFloat(i);
  
  QFont fn = QApplication::font();
  
  fn.setPointSizeFloat(i);
  QApplication::setFont(fn, TRUE);
}

const QColor & color(UmlColor c)
{
  // use a switch to not be linked with the color order
  
  switch (c) {
  case UmlTransparent:
  case UmlWhite:
    return UmlWhiteColor;
  case UmlLightYellow:
    return UmlLightYellowColor;
  case UmlYellow:
    return UmlYellowColor;
  case UmlMediumYellow:
    return UmlMediumYellowColor;
  case UmlDarkYellow:
    return UmlDarkYellowColor;
  case UmlLightBlue:
    return UmlLightBlueColor;
  case UmlBlue:
    return UmlBlueColor;
  case UmlLightMediumBlue:
    return UmlLightMediumBlueColor;
  case UmlMediumBlue:
    return UmlMediumBlueColor;
  case UmlDarkBlue:
    return UmlDarkBlueColor;
  case UmlLightGreen:
    return UmlLightGreenColor;
  case UmlGreen:
    return UmlGreenColor;
  case UmlLightMediumGreen:
    return UmlLightMediumGreenColor;
  case UmlMediumGreen:
    return UmlMediumGreenColor;
  case UmlDarkGreen:
    return UmlDarkGreenColor;
  case UmlRed:
    return UmlRedColor;
  case UmlVeryLightGray:
    return UmlVeryLightGrayColor;
  case UmlLightGray:
    return UmlLightGrayColor;
  case UmlGray:
    return UmlGrayColor;
  case UmlDarkGray:
    return UmlDarkGrayColor;
  case UmlBlack:
    return UmlBlackColor;
    
  case UmlLightRed:
    return UmlLightRedColor;
  case UmlMidRed:
    return UmlMidRedColor;
  case UmlVeryLightOrange:
    return UmlVeryLightOrangeColor;
  case UmlLightOrange:
    return UmlLightOrangeColor;
  case UmlOrange:
    return UmlOrangeColor;
  case UmlDarkOrange:
    return UmlDarkOrangeColor;
  case UmlLightMagenta:
    return UmlLightMagentaColor;
  case UmlMagenta:
    return UmlMagentaColor;
  case UmlMidMagenta:
    return UmlMidMagentaColor;
  case UmlDarkMagenta:
    return UmlDarkMagentaColor;
    
  default:	// error
    return UmlBlackColor;
  }
}

