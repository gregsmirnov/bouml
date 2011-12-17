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

#include <qptrdict.h>
#include <qdict.h>
#include <qdir.h>
#include <qimage.h>

#include "Images.h"
#include "UmlWindow.h"
#include "BrowserView.h"
#include "translate.h"
#include "DialogUtil.h"

// all the pixmap for diagram with scale 100, key = path,
static QDict<QPixmap> DiagramPixmap;

// all the pixmap for diagram, key = path,
// value is QPtrDict with key = width
static QDict<QPtrDict<QPixmap> > DiagramScaledPixmap;

// return pixmap for zoom 100%
QPixmap * get_pixmap(const char * path)
{
  QPixmap * px = DiagramPixmap.find(path);
  
  if (px == 0) {
    QString abspath;
    
    if (!QDir::isRelativePath(path))
      abspath = path;
    else if ((UmlWindow::images_root_dir().isEmpty() ||
	      !QFile::exists(abspath = QDir::cleanDirPath(UmlWindow::images_root_dir() + '/' + path))) &&
	     !QFile::exists(abspath = path))
      abspath = BrowserView::get_dir().absFilePath(path);
    
    px = new QPixmap(abspath);
    
    DiagramPixmap.insert(path, px);
    
    if (px->isNull()) {
      msg_critical(TR("Error"),
		   QString(path) + TR("\ndoesn't exist or is not a know image format"));
      return 0;
    }
    
    DiagramScaledPixmap.insert(path, new QPtrDict<QPixmap>());
  }

  return (px->isNull()) ? 0 : px;
}

// return pixmap for a given zoom != 100
QPixmap * get_pixmap(const char * path, double zoom)
{
  QPixmap * px = get_pixmap(path);
  
  if (px == 0)
    return 0;
    
  if (((int) (zoom * 100)) == 100)
    return px;
  
  QPtrDict<QPixmap> * d = DiagramScaledPixmap[path]; // != 0
  int scaled_w = (int) (px->width() * zoom);
  void * k = (void *) scaled_w;
  QPixmap * scaled_px = d->find(k);
  
  if (scaled_px == 0) {
    QImage img = 
      px->convertToImage().smoothScale(scaled_w, (int) (px->height() * zoom));

    scaled_px = new QPixmap();
    scaled_px->convertFromImage(img);
    d->insert(k, scaled_px);
  }
  
  return scaled_px;
}

void init_images()
{
  DiagramPixmap.setAutoDelete(TRUE);
  DiagramPixmap.clear();
  
  DiagramScaledPixmap.setAutoDelete(TRUE);
  DiagramScaledPixmap.clear();
}
