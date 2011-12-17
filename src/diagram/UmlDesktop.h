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

// all is static ... I don't like singleton

class UmlDesktop {
  public:
    static void init();
    static void limits(int & l, int & t, int & r, int & b);
    static void set_limits(int l, int t, int r, int b);
    static int width();
    static int height();
    static void tocenter(QWidget * who);
    static void limitsize_center(QWidget * who, QSize & previous, double max_w, double max_h);
    static void limitsize_move(QWidget * who, QSize & previous, 
			       double max_w, double max_h);
    static void setsize_center(QWidget * who, QSize & previous, 
			       double pw, double ph);
    static bool fixed();
    static bool nogui();
    static void set_nogui();
        
  private:
    static int left;
    static int top;
    static int right;
    static int bottom;
    static int w;
    static int h;
    static QPoint c;
    static bool fixedp;
    static bool noguip;
};
