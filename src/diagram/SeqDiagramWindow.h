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

#ifndef SEQDIAGRAMWINDOW_H
#define SEQDIAGRAMWINDOW_H

#include "DiagramWindow.h"

class QToolButton;
class QSpinBox;

class SeqDiagramView;
class BrowserSeqDiagram;

class SeqDiagramWindow : public DiagramWindow {
  Q_OBJECT
    
  protected:
    SeqDiagramView * view;
    QToolButton * addClass;
    QToolButton * addClassInstance;
    QToolButton * addContinuation;
    QToolButton * addFragment;
    QToolButton * syncMsg;
    QToolButton * asyncMsg;
    QToolButton * lostsyncMsg;
    QToolButton * lostasyncMsg;
    QToolButton * foundsyncMsg;
    QToolButton * foundasyncMsg;
    QToolButton * syncSelfMsg;
    QToolButton * asyncSelfMsg;
    QToolButton * returnMsg;
    QToolButton * selfreturnMsg;
    QToolButton * note;
    QToolButton * anchor;
    QToolButton * text;
    QToolButton * image;
  
  public:
    SeqDiagramWindow(const QString & s, BrowserSeqDiagram *, int id = -1);
    ~SeqDiagramWindow();

    virtual DiagramView * get_view() const;
    
    virtual void hit_button(UmlCode, QToolButton *);

  protected slots:
    void hit_class();
    void hit_classinstance();
    void hit_continuation();
    void hit_fragment();
    void hit_syncmsg();
    void hit_asyncmsg();
    void hit_foundsyncmsg();
    void hit_foundasyncmsg();
    void hit_lostsyncmsg();
    void hit_lostasyncmsg();
    void hit_syncselfmsg();
    void hit_asyncselfmsg();
    void hit_returnmsg();
    void hit_selfreturnmsg();
    void hit_note();
    void hit_anchor();
    void hit_text();
    void hit_image();
};

#endif
