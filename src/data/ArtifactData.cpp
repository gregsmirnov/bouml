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





#include <qcursor.h>

#include "ArtifactData.h"
#include "BrowserClass.h"
#include "ClassData.h"
#include "BrowserArtifact.h"
#include "BrowserDeploymentDiagram.h"
#include "ArtifactDialog.h"
#include "GenerationSettings.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"

ArtifactData::ArtifactData() : associated(0) {
}

// doesn't copy associated classes or artifacts
ArtifactData::ArtifactData(ArtifactData * model, BrowserNode * bn)
    : SimpleData(model), associated(0) {
  browser_node = bn;
  cpp_h = model->cpp_h;
  cpp_src = model->cpp_src;
  java_src = model->java_src;
  php_src = model->php_src;
  python_src = model->python_src;
  idl_src = model->idl_src;
  
#if 0
  if (model->associated != 0) {
    associated = new QPtrDict<BrowserArtifact>();
    
    QPtrDictIterator<BrowserArtifact> it(*(model->associated));
    
    while (it.current()) {
      associated->insert(it.current(), it.current());
      connect(it.current()->get_data(), SIGNAL(deleted()),
	      this, SLOT(on_delete()));
      ++it;
    }
  }
#endif
}

ArtifactData::~ArtifactData() {
  if (associated != 0)
    delete associated;
}

void ArtifactData::edit() {
  setName(browser_node->get_name());
  
  (new ArtifactDialog(this))->show();
}

void ArtifactData::use_default_cpp_h() {
  cpp_h = GenerationSettings::cpp_default_header_content();
}

void ArtifactData::use_default_cpp_src() {
  cpp_src = GenerationSettings::cpp_default_source_content();
}

void ArtifactData::use_default_java_src() {
  java_src = GenerationSettings::java_default_source_content();
}

void ArtifactData::use_default_php_src() {
  php_src = GenerationSettings::php_default_source_content();
}

void ArtifactData::use_default_python_src() {
  python_src = GenerationSettings::python_default_source_content();
}

void ArtifactData::use_default_idl_src() {
  idl_src = GenerationSettings::idl_default_source_content();
}

//

void ArtifactData::send_uml_def(ToolCom * com, BrowserNode * bn, 
				 const QString & comment) {
  BasicData::send_uml_def(com, bn, comment);
  
  const QValueList<BrowserClass *> & l =
    ((BrowserArtifact *) browser_node)->get_associated_classes();
  QValueList<BrowserClass *>::ConstIterator itl;
  QValueList<BrowserClass *>::ConstIterator end = l.end();
  unsigned n;
  
  n = 0;
  
  for (itl = l.begin(); itl != end; ++itl)
    if (! (*itl)->deletedp())
      n += 1;
    
  com->write_unsigned(n);
    
  for (itl = l.begin(); itl != end; ++itl)
    if (! (*itl)->deletedp())
      (*itl)->write_id(com);

  //
  
  if (associated == 0)
    com->write_unsigned(0);
  else {    
    QPtrDictIterator<BrowserArtifact> itd(*associated);
    
    n = 0;
    
    while (itd.current()) {
      if (! itd.current()->deletedp())
	n += 1;
      ++itd;
    }
    
    com->write_unsigned(n);
    
    itd.toFirst();
    
    while (itd.current()) {
      if (! itd.current()->deletedp())
	itd.current()->write_id(com);
      ++itd;
    }
  }
}

void ArtifactData::send_cpp_def(ToolCom * com) {
  com->write_string(cpp_h);
  com->write_string(cpp_src);
}

void ArtifactData::send_java_def(ToolCom * com) {
  com->write_string(java_src);
}

void ArtifactData::send_php_def(ToolCom * com) {
  com->write_string(php_src);
}

void ArtifactData::send_python_def(ToolCom * com) {
  com->write_string(python_src);
}

void ArtifactData::send_idl_def(ToolCom * com) {
  com->write_string(idl_src);
}

bool ArtifactData::tool_cmd(ToolCom * com, const char * args,
			     BrowserNode * bn,
			     const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case setCppHCmd:
	cpp_h = args;
	break;
      case setCppSrcCmd:
	cpp_src = args;
	break;
      case setJavaSrcCmd:
	java_src = args;
	break;
      case setPhpSrcCmd:
	php_src = args;
	break;
      case setPythonSrcCmd:
	python_src = args;
	break;
      case setIdlSrcCmd:
	idl_src = args;
	break;
      case addAssocArtifactCmd:
	{
	  BrowserArtifact * cp = (BrowserArtifact *) com->get_id(args);
	  
	  associate(cp);
	}
	break;
      case removeAssocArtifactCmd:
	{
	  BrowserArtifact * cp = (BrowserArtifact *) com->get_id(args);
	  
	  unassociate(cp);
	}
	break;
      case removeAllAssocArtifactsCmd:
	if (associated != 0) {
	  QPtrDictIterator<BrowserArtifact> it(*associated);
	  
	  while (it.current()) {
	    disconnect(it.current()->get_data(), SIGNAL(deleted()),
		       this, SLOT(on_delete()));
	    ++it;
	  }
	  
	  delete associated;
	  associated = 0;
	}
	break;
      default:
	return BasicData::tool_cmd(com, args, bn, comment);
      }
      
      // ok case
      bn->modified();
      bn->package_modified();
      modified();
      com->write_ack(TRUE);
    }
  }
  else
    return BasicData::tool_cmd(com, args, bn, comment);
  
  return TRUE;
}
      
void ArtifactData::on_delete() {
  if (associated != 0) {
    bool modp = FALSE;
    QPtrDictIterator<BrowserArtifact> it(*associated);

    while (it.current()) {
      if (it.current()->deletedp()) {
	modp = TRUE;
	associated->remove(it.current()); // update it
      }
      else
	++it;
    }
    if (modp)
      modified();
  }
  
  browser_node->on_delete();
}

bool ArtifactData::decldefbody_contain(const QString & s, bool cs,
				       BrowserNode *) {
  return ((QString(get_cpp_h()).find(s, 0, cs) != -1) ||
	  (QString(get_cpp_src()).find(s, 0, cs) != -1) ||
	  (QString(get_java_src()).find(s, 0, cs) != -1) ||
	  (QString(get_php_src()).find(s, 0, cs) != -1) ||
	  (QString(get_python_src()).find(s, 0, cs) != -1) ||
	  (QString(get_idl_src()).find(s, 0, cs) != -1));
}
  
void ArtifactData::associate(BrowserArtifact * other) {
  if ((associated == 0) || (associated->find(other) == 0)) {
    connect(other->get_data(), SIGNAL(deleted()), this, SLOT(on_delete()));
    if (associated == 0)
      associated = new QPtrDict<BrowserArtifact>;
    
    associated->insert(other, other);
    browser_node->modified();
    browser_node->package_modified();
    modified();
  }
}

void ArtifactData::unassociate(BrowserArtifact * other) {
  disconnect(other->get_data(), SIGNAL(deleted()), this, SLOT(on_delete()));
  associated->remove(other);
  browser_node->modified();
  browser_node->package_modified();
  modified();
}

void ArtifactData::update_associated(QPtrDict<BrowserArtifact> & d) {
  if (associated != 0) {
    QPtrDictIterator<BrowserArtifact> it(*associated);

    while (it.current()) {
      if (d.find(it.current()) == 0) {
	disconnect(it.current()->get_data(), SIGNAL(deleted()),
		   this, SLOT(on_delete()));
	associated->remove(it.current());
      }
      else
	++it;
    }
  }
  else
    associated = new QPtrDict<BrowserArtifact>((d.count() >> 4) + 1);
  
  QPtrDictIterator<BrowserArtifact> it(d);
  
  while (it.current()) {
    if (associated->find(it.current()) == 0) {
      associated->insert(it.current(), it.current());
      if (associated->size() < (associated->count() >> 4))
	associated->resize((associated->count() >> 4) + 1);
      connect(it.current()->get_data(), SIGNAL(deleted()),
	      this, SLOT(on_delete()));
    }
    ++it;
  }
}

//

// translate plug out release < 2.0
void ArtifactData::convert_add_include_artifact() {
  if (!cpp_src.isEmpty()) {
    int index = cpp_src.find("#include \"UmlComponent.h\"");
    
    if (index != -1) {
      QCString s = cpp_src.SharedStr::operator QCString();
      
      s.insert(index, "#include \"UmlArtifact.h\"\n");
      cpp_src = s;
    }
  }
}

//

void ArtifactData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);
  
  bool a_text = (stereotype == "text");
  
  if (a_text || (stereotype == "source")) {
    if (!a_text && !cpp_h.isEmpty()) {
      nl_indent(st);
      st << "cpp_h ";
      save_string(cpp_h, st);
    }
    if (!cpp_src.isEmpty()) {
      nl_indent(st);
      st << "cpp_src ";
      save_string(cpp_src, st);
    }
    if (!java_src.isEmpty()) {
      nl_indent(st);
      st << "java_src ";
      save_string(java_src, st);
    }
    if (!php_src.isEmpty()) {
      nl_indent(st);
      st << "php_src ";
      save_string(php_src, st);
    }
    if (!python_src.isEmpty()) {
      nl_indent(st);
      st << "python_src ";
      save_string(python_src, st);
    }
    if (!idl_src.isEmpty()) {
      nl_indent(st);
      st << "idl_src ";
      save_string(idl_src, st);
    }
  }
  else if (associated != 0) {
    nl_indent(st);
    st << "associated_artifacts";
    indent(+1);
    
    QPtrDictIterator<BrowserArtifact> it(*associated);

    while (it.current()) {
      nl_indent(st);
      it.current()->save(st, TRUE, warning);
      ++it;
    }
    
    indent(-1);
    nl_indent(st);
    st << "end";
  }
}

void ArtifactData::read(char * & st, char * & k) {
  BasicData::read(st, k);	// updates k

  cpp_h = QString::null;
  cpp_src = QString::null;
  java_src = QString::null;
  php_src = QString::null;
  python_src = QString::null;
  idl_src = QString::null;
  
  if ((stereotype == "text") && (read_file_format() < 42))
    stereotype = "_text";
  
  if ((stereotype == "source") || (stereotype == "text")) {
    if (!strcmp(k, "cpp_h")) {
      cpp_h = read_string(st);
      k = read_keyword(st);
    }
    
    if (!strcmp(k, "cpp_src")) {
      // old -> new version
      QCString s = read_string(st);
      int index;
      
      if ((index = s.find("${class_attributes}\n${operations}")) != -1)
	s.replace(index, 33, "${members}");
      else if ((index = s.find("${class_attributes}\r\n${operations}")) != -1)
	s.replace(index, 34, "${members}");
      cpp_src = s;
      k = read_keyword(st);
    }
    
    if (!strcmp(k, "java_src")) {
      java_src = read_string(st);
      k = read_keyword(st);
    }
    
    if (!strcmp(k, "php_src")) {
      php_src = read_string(st);
      k = read_keyword(st);
    }
    
    if (!strcmp(k, "python_src")) {
      python_src = read_string(st);
      k = read_keyword(st);
    }
    
    if (!strcmp(k, "idl_src")) {
      idl_src = read_string(st);
      k = read_keyword(st);
    }
  }
  else if (!strcmp(k, "associated_artifacts") ||
	   ((read_file_format() < 20) && !strcmp(k, "associated_components"))) {
    associated = new QPtrDict<BrowserArtifact>();
    
    while (strcmp(k = read_keyword(st), "end")) {
      BrowserArtifact * c =
	BrowserArtifact::read_ref(st, k);	// k not updated
      
      associated->insert(c, c);
      connect(c->get_data(), SIGNAL(deleted()),
	      this, SLOT(on_delete()));
    }
    
    if (associated->size() < (associated->count() >> 4))
      associated->resize((associated->count() >> 4) + 1);
    
    k = read_keyword(st);
  }
}
