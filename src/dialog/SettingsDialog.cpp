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






#include <qhbox.h>
#include <qgrid.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h>

#include "Settings.h"
#include "SettingsDialog.h"
#include "UmlPixmap.h"
#include "UmlDesktop.h"
#include "translate.h"

void ColorSpecVector::resize(int s) {
  ColorSpec * v = new ColorSpec[s];
  
  for (int i = 0; i != _size; i += 1)
    v[i] = _v[i];
  
  delete [] _v;
  _v = v;
  _size = s;
}

class ComboStates : public QComboBox {
  public:
    ComboStates(QWidget * parent, Uml3States v, bool nodefault, bool unchanged);
    ComboStates(QWidget * parent, ClassDrawingMode v, bool nodefault, bool unchanged);
    ComboStates(QWidget * parent, DrawingLanguage v, bool nodefault, bool unchanged);
    ComboStates(QWidget * parent, ShowContextMode v, bool nodefault, bool unchanged);
    ComboStates(QWidget * parent, char v, bool nodefault, bool unchanged);
    
    virtual QSize sizeHint() const;
};

ComboStates::ComboStates(QWidget * parent, Uml3States v,
			 bool nodefault, bool unchanged) 
    : QComboBox(FALSE, parent) {
  insertItem(TR(stringify((Uml3States) 0)));
  insertItem(TR(stringify((Uml3States) 1)));
  // the last value MUST be default
  if (!nodefault)
    insertItem(TR(stringify((Uml3States) 2)));
  if (unchanged) {
    insertItem(TR("<unchanged>"));
    setCurrentItem(count() - 1); 
  }
  else
    setCurrentItem(v);
}

ComboStates::ComboStates(QWidget * parent, ClassDrawingMode v,
			 bool nodefault, bool unchanged) 
    : QComboBox(FALSE, parent) {
  int i;
  
  for (i = 0; i != (int) DefaultClassDrawingMode; i += 1)
    insertItem(TR(stringify((ClassDrawingMode) i)));
  // the last value MUST be default
  if (!nodefault)
    insertItem(TR(stringify((ClassDrawingMode) i)));
  if (unchanged) {
    insertItem(TR("<unchanged>"));
    setCurrentItem(count() - 1); 
  }
  else
    setCurrentItem(v);
}

ComboStates::ComboStates(QWidget * parent, DrawingLanguage v,
			 bool nodefault, bool unchanged) 
    : QComboBox(FALSE, parent) {
  int i;
  
  for (i = 0; i != (int) DefaultDrawingLanguage; i += 1)
    insertItem(stringify((DrawingLanguage) i));
  // the last value MUST be default
  if (!nodefault)
    insertItem(TR(stringify((DrawingLanguage) i)));
  if (unchanged) {
    insertItem(TR("<unchanged>"));
    setCurrentItem(count() - 1); 
  }
  else
    setCurrentItem(v);
}

static QString _2space(QString s)
{
  int index = s.find("_");
  
  return (index == -1) ? s : s.replace(index, 1, " ");
}

ComboStates::ComboStates(QWidget * parent, ShowContextMode v,
			 bool nodefault, bool unchanged) 
    : QComboBox(FALSE, parent) {
  int i;
  
   for (i = 0; i != (int) DefaultShowContextMode; i += 1)
    insertItem(_2space(TR(stringify((ShowContextMode) i))));
  // the last value MUST be default
  if (!nodefault)
    insertItem(_2space(TR(stringify((ShowContextMode) i))));
  if (unchanged) {
    insertItem(TR("<unchanged>"));
    setCurrentItem(count() - 1); 
  }
  else
    setCurrentItem(v);
}


ComboStates::ComboStates(QWidget * parent, char v,
			 bool nodefault, bool unchanged) 
    : QComboBox(FALSE, parent) {
  int i;
  
  for (i = MinMemberWidthValue; i != SupMemberWidthValue; i += 1) {
    char s[4];
    
    sprintf(s, "%d", i);
    insertItem(s);
  }
  insertItem(TR("unlimited"));
  if (!nodefault)
    insertItem(TR("default"));
  if (unchanged) {
    insertItem(TR("<unchanged>"));
    setCurrentItem(count() - 1); 
  }
  else {
    switch (v) {
    case UmlDefaultMaxMemberWidth:
      setCurrentItem(SupMemberWidthValue-MinMemberWidthValue+1);
      break;
    case UmlUnlimitedMemberWidth:
      setCurrentItem(SupMemberWidthValue-MinMemberWidthValue);
      break;
    default:
      setCurrentItem(v-MinMemberWidthValue);
      break;
    }
  }
}

QSize ComboStates::sizeHint() const {
  QSize sz = QComboBox::sizeHint();
  
  sz.setHeight(fontMetrics().height() + 4);
  return sz;
}


class ComboColor : public QComboBox {
  public:
    ComboColor(QWidget * parent, UmlColor v,
	       bool nodefault, bool unchanged);
};

ComboColor::ComboColor(QWidget * parent, UmlColor v,
		       bool nodefault, bool unchanged) 
    : QComboBox(FALSE, parent) {
  QString s_transparent = TR("Transparent");
  QString s_unknown_color = TR("Unknown color");
  
  // the last value MUST be default
  for (int i = 0; i != (int) UmlDefaultColor; i += 1) {
    // use switch to not be dependant on the colors items order and number
    switch (i) {
    case UmlTransparent:
      insertItem(s_transparent);
      break;
    case UmlWhite:
      insertItem(* WhitePixmap);
      break;
    case UmlLightYellow:
      insertItem(* LightYellowPixmap);
      break;
    case UmlYellow:
      insertItem(* YellowPixmap);
      break;
    case UmlMediumYellow:
      insertItem(* MediumYellowPixmap);
      break;
    case UmlDarkYellow:
      insertItem(* DarkYellowPixmap);
      break;
    case UmlLightBlue:
      insertItem(* LightBluePixmap);
      break;
    case UmlBlue:
      insertItem(* BluePixmap);
      break;
    case UmlLightMediumBlue:
      insertItem(* LightMediumBluePixmap);
      break;
    case UmlMediumBlue:
      insertItem(* MediumBluePixmap);
      break;
    case UmlDarkBlue:
      insertItem(* DarkBluePixmap);
      break;
    case UmlLightGreen:
      insertItem(* LightGreenPixmap);
      break;
    case UmlGreen:
      insertItem(* GreenPixmap);
      break;
    case UmlLightMediumGreen:
      insertItem(* LightMediumGreenPixmap);
      break;
    case UmlMediumGreen:
      insertItem(* MediumGreenPixmap);
      break;
    case UmlDarkGreen:
      insertItem(* DarkGreenPixmap);
      break;
    case UmlLightRed:
      insertItem(* LightRedPixmap);
      break;
    case UmlMidRed:
      insertItem(* MidRedPixmap);
      break;
    case UmlRed:
      insertItem(* RedPixmap);
      break;
    case UmlVeryLightOrange:
      insertItem(* VeryLightOrangePixmap);
      break;
    case UmlLightOrange:
      insertItem(* LightOrangePixmap);
      break;
    case UmlOrange:
      insertItem(* OrangePixmap);
      break;
    case UmlDarkOrange:
      insertItem(* DarkOrangePixmap);
      break;
    case UmlLightMagenta:
      insertItem(* LightMagentaPixmap);
      break;
    case UmlMagenta:
      insertItem(* MagentaPixmap);
      break;
    case UmlMidMagenta:
      insertItem(* MidMagentaPixmap);
      break;
    case UmlDarkMagenta:
      insertItem(* DarkMagentaPixmap);
      break;
    case UmlVeryLightGray:
      insertItem(* VeryLightGrayPixmap);
      break;
    case UmlLightGray:
      insertItem(* LightGrayPixmap);
      break;
    case UmlGray:
      insertItem(* GrayPixmap);
      break;
    case UmlDarkGray:
      insertItem(* DarkGrayPixmap);
      break;
    case UmlBlack:
      insertItem(* BlackPixmap);
      break;
    default:
      insertItem(s_unknown_color);
    }
  }
  if (!nodefault)
    insertItem(TR(stringify(UmlDefaultColor)));
  if (unchanged) {
    insertItem(TR("<unchanged>"));
    setCurrentItem(count() - 1); 
  }
  else
    setCurrentItem(v);    
  



  setSizeLimit(25);	// yes !, don't set it to count() !

}

QSize SettingsDialog::previous_size;
QPoint SettingsDialog::previous_position;
QString SettingsDialog::previous_active_tab;

SettingsDialog::SettingsDialog(StateSpecVector * st, ColorSpecVector * co,
			       bool nodefault, bool unchanged, QString title)
    : QTabDialog(0, title, TRUE),
      states(st), colors(co), first_visible_page(0),
      several(unchanged), did_apply(FALSE) {
  setCaption((title.isEmpty()) ? TR("Diagram Drawing Settings dialog") : title);
  
  QString s_diagram = TR("diagram");
  QGrid * grid = 0;
  QString tabname;
  unsigned i;
  unsigned n;
  
  if (states != 0) {
    n = states->size();
    cbstates = new QVector<ComboStates>(n);
    
    for (i = 0; i != n; i += 1) {
      StateSpec & st = states->at(i);
      QString s = st.name;
      int index = s.find('#');
      QString tbn;
      
      if (index != -1) {
	tbn = s.left(index);
	s = s.mid(index + 1);
      }
      else
	tbn = s_diagram;
      
      if ((grid == 0) || (tabname != tbn)) {
	if (grid != 0) {
	  addTab(grid, tabname);
	  grid->setName(tabname);
	  if (tabname == previous_active_tab)
	    first_visible_page = grid;
	}
	grid = new QGrid(5, this);
	grid->setMargin(2);
	grid->setSpacing(2);
	tabname = tbn;
      }
      
      new QLabel("", grid);
      new QLabel(s + " : ", grid);
      QHBox * hb = new QHBox(grid);
            
      switch (st.who) {
      case StateSpec::is3states:
	cbstates->insert(i, new ComboStates(hb, *((Uml3States *) st.state),
					    nodefault, unchanged));
	break;
      case StateSpec::isClassDrawingMode:
	cbstates->insert(i, new ComboStates(hb, *((ClassDrawingMode *) st.state),
					    nodefault, unchanged));
	break;
      case StateSpec::isDrawingLanguage:
	cbstates->insert(i, new ComboStates(hb, *((DrawingLanguage *) st.state),
					    nodefault, unchanged));
	break;
      case StateSpec::isShowContextMode:
	cbstates->insert(i, new ComboStates(hb, *((ShowContextMode *) st.state),
					    nodefault, unchanged));
	break;
      default:
	cbstates->insert(i, new ComboStates(hb, *((char *) st.state),
					    nodefault, unchanged));
      }
      new QLabel("", hb);
      new QLabel("", grid);
      new QLabel("", grid);
    }
    
    addTab(grid, tabname);
    grid->setName(tabname);
    if (tabname == previous_active_tab)
      first_visible_page = grid;
  }
  
  if (colors != 0) {
    QString lbl = TR("color");
    QString s_default = TR("default ");
    
    n = colors->size();
    cbcolors = new QVector<ComboColor>(n);
    grid = new QGrid(5, this);
    grid->setMargin(2);
    grid->setSpacing(2);
    
    for (i = 0; i != n; i += 1) {
      if (i == 11) {
	lbl = TR("color [1]");
	addTab(grid, lbl);
	grid->setName(lbl);
	if (previous_active_tab == lbl)
	  first_visible_page = grid;
	lbl = TR("color [2]");
	grid = new QGrid(5, this);
	grid->setMargin(2);
	grid->setSpacing(2);
      }
      new QLabel("", grid);
      
      QString s = colors->at(i).name;
      
      s += " : ";
      new QLabel(s, grid);
      cbcolors->insert(i, new ComboColor(grid, *(colors->at(i).color),
					 nodefault, unchanged));
      new QLabel("", grid);
      new QLabel("", grid);
    }
    
    addTab(grid, lbl);
    grid->setName(lbl);
    if (previous_active_tab == lbl)
      first_visible_page = grid;
  }
  
  // use help rather than apply because when the OK button is clicked,
  // the applyButtonPressed() signal is emitted
  setOkButton(TR("OK"));
  setHelpButton(TR("Apply"));
  setCancelButton(TR("Cancel"));
  
  connect(this, SIGNAL(helpButtonPressed()),
	  this, SLOT(apply()));
}

void SettingsDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
  
  if (first_visible_page != 0) {
    move(previous_position);
    previous_active_tab = "";
    showPage(first_visible_page);
  }
}

SettingsDialog::~SettingsDialog() {
  if (colors != 0)
    delete cbcolors;
  if (states != 0)
    delete cbstates;
  previous_size = size();
}

void SettingsDialog::apply() {
  did_apply = TRUE;
  previous_position = pos();
  previous_active_tab = currentPage()->name();
  accept();
}

void SettingsDialog::accept() {
  QString s_unchanged = TR("<unchanged>");
  QString s_default = TR("default");
  QString s_unlimited = TR("unlimited");
  unsigned i, n;
  
  if (states != 0) {
    n = states->size();
    
    for (i = 0; i != n; i += 1) {
      StateSpec & st = states->at(i);
      QString s = cbstates->at(i)->currentText();
      
      if (s == s_unchanged)
	st.name = QString();
      else if (st.who == StateSpec::isMemberWidth) {
	if (s == s_default)
	  st.set_state(UmlDefaultMaxMemberWidth);
	else if (s == s_unlimited)
	  st.set_state(UmlUnlimitedMemberWidth);
	else
	  st.set_state(cbstates->at(i)->currentItem() + MinMemberWidthValue);
      }
      else
	st.set_state(cbstates->at(i)->currentItem());
    }
  }
  
  if (colors != 0) {
    n = colors->size();
    
    for (i = 0; i != n; i += 1) {
      if (cbcolors->at(i)->currentText() == s_unchanged)
	colors->at(i).name = QString();
      else
	*(colors->at(i).color) = (UmlColor) cbcolors->at(i)->currentItem();
    }
  }

  QDialog::accept();
}
