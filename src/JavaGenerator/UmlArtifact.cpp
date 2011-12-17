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

#include <stdio.h>
#include <qtextstream.h> 

#include "UmlArtifact.h"
#include "UmlPackage.h"
#include "UmlOperation.h"
#include "UmlClass.h"
#include "JavaSettings.h"
#include "UmlCom.h"
#include "util.h"

UmlPackage * UmlArtifact::package_of_generated_artifact;
UmlArtifact * UmlArtifact::current;

void UmlArtifact::generate() {
  if (! managed) {
    managed = TRUE;
    
    if (stereotype() == "text") {
      generate_text();
      return;
    }
    else if (stereotype() != "source")
      return;
    
    package_of_generated_artifact = package();
    current = this;
    
    const QCString filedef = javaSource();
    
    if (filedef.isEmpty())
      return;
    
    const QCString & name = this->name();
    QCString path = package_of_generated_artifact->file_path(name);
    
    UmlCom::message(name);
    if (verbose())
      UmlCom::trace(QCString("<hr><font face=helvetica>Generate code for <i> ")
		    + name + "</i> in " + path + "</i></font><br>");      
    else
      set_trace_header(QCString("<font face=helvetica>Generate code for <i> ")
		       + name + "</i> in " + path + "</i></font><br>"); 
    
    // get bodies if preserve
    const QVector<UmlClass> & cls = associatedClasses();
    
    if (preserve())
      UmlOperation::read_bodies(path);
      
    // generate file
    
    unsigned n = cls.count();
    unsigned index;
    QCString incl;
    QByteArray file;
    // note : QTextOStream(FILE *) does not work under windows
    QTextOStream f(file);
    const char * p = filedef;
    const char * pp = 0;
      
    for (;;) {
      if (*p == 0) {
	if (pp == 0)
	  break;
	
	// comment management done
	p = pp;
	pp = 0;
	if (*p == 0)
	  break;
      }
      
      if (*p == '@')
	manage_alias(p, f);
      else if (*p != '$')
	f << *p++;
      else if (!strncmp(p, "${comment}", 10))
	manage_comment(p, pp, JavaSettings::isGenerateJavadocStyleComment());
      else if (!strncmp(p, "${description}", 14))
	manage_description(p, pp);
      else if (!strncmp(p, "${name}", 7)) {
	p += 7;
	f << name;
      }
      else if (!strncmp(p, "${Name}", 7)) {
	p += 7;
	f << capitalize(name);
      }
      else if (!strncmp(p, "${NAME}", 7)) {
	p += 7;
	f << name.upper();
      }
      else if (!strncmp(p, "${nAME}", 7)) {
	p += 7;
	f << name.lower();
      }
      else if (!strncmp(p, "${imports}", 10)) {
	QCString indent = current_indent(p, filedef);
	
	for (index = 0; index != n; index += 1)
	  cls[index]->generate_import(f, indent);
	p += 10;
	if (*p == '\n')
	  p += 1;
      }
      else if (!strncmp(p, "${package}", 10)) {
	p += 10;
	
	const QCString & package = package_of_generated_artifact->javaPackage();
	
	if (!package.isEmpty())
	  f << "package " << package << ";\n\n";
	if (*p == '\n')
	  p += 1;
      }
      else if (!strncmp(p, "${definition}", 13)) {
	QCString indent = current_indent(p, filedef);
	
	for (index = 0; index != n; index += 1)
	  cls[index]->generate(f, indent);
	p += 13;
	if (*p == '\n')
	  p += 1;
      }
      else
	// strange
	f << *p++;
    }
      
    f << '\000';
    
    if (must_be_saved(path, file)) {
      write_trace_header();
	
      FILE * fp;
      
      if ((fp = fopen((const char *) path, "wb")) == 0) {
	write_trace_header();
	UmlCom::trace(QCString("<font color=\"red\"><b><i> ")
		      + name + "</i> : cannot open <i> "
		      + path + "</i>, edit the <i> generation settings</i> (tab directory) or the <i>"
		      + package_of_generated_artifact->name()
		      + "</i> Java directory specification</b></font><br>");
	incr_error();
      }
      else {
	fputs((const char *) file, fp);
	fclose(fp);
      }
    }
    else if (get_trace_header().isEmpty())
      UmlCom::trace(QCString("<font face=helvetica><i> ")
		    + path + "</i> not modified</font><br>");
    
    if (imports != 0) {
      delete imports;
      imports = 0;
    }
  }
}

void UmlArtifact::generate_text() {
  const QCString srcdef = javaSource();
  
  if (srcdef.isEmpty()) {
    if (verbose())
      UmlCom::trace(QCString("<hr><font face=helvetica>artifact <i>")
		    + name() + "</i> has an empty Java definition</font><br>");
    return;
  }
    
  UmlPackage * pack = package();
  const QCString & name = UmlArtifact::name();    
  QCString src_path = pack->text_path(name);
  
  QCString s = " in <i> " + src_path + "</i>";
      
  UmlCom::message(name);
  if (verbose())
    UmlCom::trace(QCString("<hr><font face=helvetica>Generate code for <i> ")
		  + name + "</i>" + s + "</font><br>");
  else
    set_trace_header(QCString("<font face=helvetica>Generate code for <i> ")
		     + name + "</i>" + s + "</font><br>");
      
  if (must_be_saved(src_path, (const char *) srcdef)) {
    write_trace_header();
    
    FILE * fp_src;
    
    if ((fp_src = fopen((const char *) src_path, "wb")) == 0) {
      write_trace_header();
      UmlCom::trace(QCString("<font color=\"red\"><b><i> ")
		    + name + " : </i> cannot open <i> " 
		    + src_path + "</i>, edit the <i> generation settings</i> (tab directory) or the <i>"
		    + pack->name() + "</i> Java directory specification</b></font><br>");
      incr_error();
    }
    else {
      fputs((const char *) srcdef, fp_src);
      fclose(fp_src);
    }
  }
  else if (get_trace_header().isEmpty())
    UmlCom::trace(QCString("<font face=helvetica><i> ")
		  + src_path + "</i> not modified</font><br>");
}

UmlPackage * UmlArtifact::generation_package()
{
  return package_of_generated_artifact;
}

UmlArtifact * UmlArtifact::generated_one()
{
  return current;
}

void UmlArtifact::imported(const QCString & s) {
  if (imports == 0)
    // init it
    (void) is_imported("", "");
  
  imports->insert(s, this);
}

bool UmlArtifact::is_imported(const QCString & s) {
  if (imports == 0)
    // init it
    (void) is_imported("", "");

  return (imports->find(s) != 0);
}

bool UmlArtifact::is_imported(QCString path, QCString class_name) {
  if (imports == 0) {
    imports = new QAsciiDict<UmlArtifact>(17);
    
    QCString s = javaSource();
    int index = 0;
    
    while ((index = s.find("import", index)) != -1) {
      if ((index == 0) || (s[index - 1] < ' ')) {
	int index2 = index + 6;
	
	while ((s[index2] == ' ') || (s[index2] == '\t'))
	  index2 += 1;
	
	if ((index2 != (index + 6)) &&
	    ((index = s.find(';', index2)) != -1) &&
	    (index != index2)) {
	 QCString p = s.mid(index2, index - index2);
	 
	 imports->insert(p, this);
	}
	else
	  index = index2;
      }
      else
	index += 6;
    }
  }
  
  return ((imports->find(path + '.' + class_name) != 0) ||
	  (imports->find(path + ".*") != 0));
}

bool UmlArtifact::must_be_saved(const char * path, const char * new_contains)
{
  FILE * fp = fopen(path, "rb");
  
  if (fp == 0)
    return TRUE;
  
  int c;
  
  while ((c = fgetc(fp)) != EOF) {
    if (((char) c) != *new_contains++) {
      fclose(fp);
      return TRUE;
    }
  }
  
  fclose(fp);
  
  return *new_contains != 0;
}
