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

#ifndef ARTIFACTDATA_H
#define ARTIFACTDATA_H

#include <qptrdict.h>

#include "SimpleData.h"

class BrowserArtifact;

class ArtifactData : public SimpleData {
  Q_OBJECT
    
  friend class ArtifactDialog;
  
  protected:
    // for the non <<source>>:
    QPtrDict<BrowserArtifact> * associated;
    
    // for the <<sources>> (the majority of artifacts) :
    
    // C++
    SharedStr cpp_h;
    SharedStr cpp_src;
    
    // Java
    SharedStr java_src;
    
    // PHP
    SharedStr php_src;
    
    // Python
    SharedStr python_src;
    
    // Idl
    SharedStr idl_src;
    
    virtual void send_uml_def(ToolCom * com, BrowserNode * bn,
			      const QString & comment);
    virtual void send_cpp_def(ToolCom * com);
    virtual void send_java_def(ToolCom * com);
    virtual void send_php_def(ToolCom * com);
    virtual void send_python_def(ToolCom * com);
    virtual void send_idl_def(ToolCom * com);
    
    void update_associated(QPtrDict<BrowserArtifact> & d);
    
  public:
    ArtifactData();
    ArtifactData(ArtifactData * model, BrowserNode *);
    virtual ~ArtifactData();
    
    const char * get_cpp_h() const { return cpp_h; };
    const char * get_cpp_src() const { return cpp_src; };
    void use_default_cpp_h();
    void use_default_cpp_src();
        
    const char * get_java_src() const { return java_src; };
    void use_default_java_src();
        
    const char * get_php_src() const { return php_src; };
    void use_default_php_src();
    
    const char * get_python_src() const { return python_src; };
    void use_default_python_src();
    
    const char * get_idl_src() const { return idl_src; };
    void use_default_idl_src();
    
    const QPtrDict<BrowserArtifact> * get_associated() { return associated; };
    void associate(BrowserArtifact *);
    void unassociate(BrowserArtifact *);
    
    virtual bool decldefbody_contain(const QString & s, bool cs, BrowserNode *);
    
    void edit();
    
    virtual bool tool_cmd(ToolCom * com, const char * args,
			  BrowserNode * bn, const QString & comment);
    
    void save(QTextStream &, QString & warning) const;
    void read(char * & st, char * & k);
    
    void convert_add_include_artifact();
    
  protected slots:
    void on_delete();
};

#endif
