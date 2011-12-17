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
#include "UmlClass.h"
#include "UmlOperation.h"
#include "CppRefType.h"
#include "CppSettings.h"
#include "UmlCom.h"
#include "util.h"

UmlPackage * UmlArtifact::package_of_generated_artifact;

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
    
    const QCString hdef = cppHeader();
    const QCString srcdef = cppSource();
    
    if (hdef.isEmpty() && srcdef.isEmpty()) {
      if (verbose())
	UmlCom::trace(QCString("<hr><font face=helvetica>artifact <i>")
		      + name() + "</i> has an empty C++ definition</font><br>");
      return;
    }
    
    const QCString & name = UmlArtifact::name();    
    UmlPackage * pack = package();
    QCString h_path = pack->header_path(name);
    QCString src_path = pack->source_path(name);
    QCString nasp_start;
    QCString nasp_end;
    const char * cnasp = pack->cppNamespace();
    QCString nasp = ((cnasp[0] == ':') && (cnasp[1] == ':'))
      ? cnasp + 2 : cnasp;
    
    if (!nasp.isEmpty()) {
      int index = 0;
      int index2;
      QCString closed = "\n} // namespace ";
      
      while ((index2 = nasp.find(':', index)) != -1) {
	QCString na = nasp.mid(index, index2 - index);
	
	nasp_start += QCString("namespace ") + na + " {\n\n";
	closed += na;
	nasp_end = closed + "\n" + nasp_end;
	closed += "::";
	nasp.replace(index2, 2, "_");
	index = index2 + 1;
      }
      
      nasp_start += QCString("namespace ") + nasp.mid(index) + " {\n\n";
      closed += nasp.mid(index);
      nasp_end = closed + "\n" + nasp_end;
    }
    
    {
      QCString s;
    
      if (!hdef.isEmpty())
	s = " in <i> " + h_path + "</i>";
      
      if (!srcdef.isEmpty()) {	
	if (!hdef.isEmpty())
	  s += " and <i> " + src_path + "</i>";
	else
	  s = " in <i> " + src_path + "</i>";
      }
      
      UmlCom::message(name);
      if (verbose())
	UmlCom::trace(QCString("<hr><font face=helvetica>Generate code for <i> ")
		      + name + "</i>" + s + "</font><br>");
      else
	set_trace_header(QCString("<font face=helvetica>Generate code for <i> ")
			 + name + "</i>" + s + "</font><br>");
    }

    // get bodies if preserve
    const QVector<UmlClass> & cls = associatedClasses();
    
    if (preserve())
      UmlOperation::read_bodies(h_path, src_path);
      
    // compute dependencies
    
    bool all_in_h = (hdef.find("${all_includes}") != -1);
    QList<CppRefType> dependencies;
    unsigned n = cls.count();
    unsigned index;

    for (index = 0; index != n; index += 1)
      cls[index]->compute_dependencies(dependencies, all_in_h);
    
    // generate header file
    
    QCString h_incl;
    QCString src_incl;
    QCString decl;
    bool incl_computed = FALSE;
	    
    if (!hdef.isEmpty()) {      
      QByteArray file;
      QTextOStream f_h(file);
      const char * p = hdef;
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
	  manage_alias(p, f_h);
	else if (*p != '$')
	  f_h << *p++;
	else if (!strncmp(p, "${comment}", 10))
	  manage_comment(p, pp, CppSettings::isGenerateJavadocStyleComment());
	else if (!strncmp(p, "${description}", 14))
	  manage_description(p, pp);
	else if (!strncmp(p, "${name}", 7)) {
	  p += 7;
	  f_h << name;
	}
	else if (!strncmp(p, "${Name}", 7)) {
	  p += 7;
	  f_h << capitalize(name);
	}
	else if (!strncmp(p, "${NAME}", 7)) {
	  p += 7;
	  f_h << name.upper();
	}
	else if (!strncmp(p, "${nAME}", 7)) {
	  p += 7;
	  f_h << name.lower();
	}
	else if (!strncmp(p, "${namespace}", 12)) {
	  p += 12;
	  f_h << nasp;
	}
	else if (!strncmp(p, "${NAMESPACE}", 12)) {
	  p += 12;
	  f_h << nasp.upper();
	}
	else if (!strncmp(p, "${includes}", 11)
		 || !strncmp(p, "${all_includes}", 15)) {
	  p += (p[2] == 'a') ? 15 : 11;
	  if (!incl_computed) {
	    incl_computed = TRUE;
	    CppRefType::compute(dependencies, hdef, srcdef, h_incl, decl, src_incl, this);
	  }
	  if (!h_incl.isEmpty()) {
	    f_h << h_incl;
	    if (*p != '\n')
	      f_h << '\n';
	  }
	  else if (*p == '\n')
	    p += 1;
	}
	else if (!strncmp(p, "${declarations}", 15)) {
	  p += 15;
	  if (!incl_computed) {
	    incl_computed = TRUE;
	    CppRefType::compute(dependencies, hdef, srcdef, h_incl, decl, src_incl, this);
	  }
	  if (!decl.isEmpty()) {
	    f_h << decl;
	    if (*p != '\n')
	      f_h << '\n';
	  }
	  else if (*p == '\n')
	    p += 1;
	}
	else if (!strncmp(p, "${namespace_start}", 18)) {
	  p += 18;
	  if (!nasp_start.isEmpty())
	    f_h << nasp_start;
	  if (*p == '\n')
	    p += 1;
	}
	else if (!strncmp(p, "${namespace_end}", 16)) {
	  p += 16;
	  if (!nasp_end.isEmpty())
	    f_h << nasp_end;
	  if (*p == '\n')
	    p += 1;
	}
	else if (!strncmp(p, "${definition}", 13)) {
	  p += 13;
	  for (index = 0; index != n; index += 1)
	    cls[index]->generate_decl(f_h, current_indent(p, hdef));
	  if (*p == '\n')
	    p += 1;
	}
	else
	  // strange
	  f_h << *p++;
      }
      
      f_h << '\000';

      if (must_be_saved(h_path, file)) {
	write_trace_header();
	
	FILE * fp_h;
      
	if ((fp_h = fopen((const char *) h_path, "wb")) == 0) {
	  UmlCom::trace(QCString("<font color=\"red\"><b><i> ")
			+ name + "</i> : cannot open <i> "
			+ h_path + "</i>, edit the <i> generation settings</i> (tab directory) or the <i>"
			+ pack->name() + "</i> C++ directory specification</b></font><br>");
	  incr_error();
	}
	else {
	  fputs((const char *) file, fp_h);
	  fclose(fp_h);
	}
      }
      else {
	QCString th = get_trace_header();
	
	if (th.isEmpty())
	  UmlCom::trace(QCString("<br><font face=helvetica><i> ")
			+ h_path + "</i> not modified</font><br>");
	else
	  set_trace_header(th + "<font face=helvetica><i> "
			   + h_path + "</i> not modified</font><br>");
      }
    }
    
    // generate source file
    
    if (!srcdef.isEmpty()) {
      QByteArray file;
      QTextOStream f_src(file);
      const char * p = srcdef;
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
	  manage_alias(p, f_src);
	else if (*p != '$')
	  f_src << *p++;
	else if (!strncmp(p, "${comment}", 10))
	  manage_comment(p, pp, CppSettings::isGenerateJavadocStyleComment());
	else if (!strncmp(p, "${description}", 14))
	  manage_description(p, pp);
	else if (!strncmp(p, "${name}", 7)) {
	  // for file header
	  p += 7;
	  f_src << name;
	}
	else if (!strncmp(p, "${includes}", 11)) {
	  p += 11;
	  if (!incl_computed) {
	    incl_computed = TRUE;
	    CppRefType::compute(dependencies, hdef, srcdef, h_incl, decl, src_incl, this);
	  }
	  if (!src_incl.isEmpty()) {
	    f_src << src_incl;
	    if (*p != '\n')
	      f_src << '\n';
	  }
	  else if (*p == '\n')
	    p += 1;
	}
	else if (!strncmp(p, "${members}", 10)) {
	  p += 10;
	  for (index = 0; index != n; index += 1)
	    cls[index]->generate_def(f_src, current_indent(p, srcdef), FALSE);
	  if (*p == '\n')
	    p += 1;
	}
	else if (!strncmp(p, "${namespace_start}", 18)) {
	  p += 18;
	  if (!nasp_start.isEmpty())
	    f_src << nasp_start;
	  if (*p == '\n')
	    p += 1;
	}
	else if (!strncmp(p, "${namespace_end}", 16)) {
	  p += 16;
	  if (!nasp_end.isEmpty())
	    f_src << nasp_end;
	  if (*p == '\n')
	    p += 1;
	}
	else
	  // strange
	  f_src << *p++;
      }
      
      f_src << '\000';
      
      if (must_be_saved(src_path, file)) {
	write_trace_header();
	
	FILE * fp_src;
	
	if ((fp_src = fopen((const char *) src_path, "wb")) == 0) {
	  write_trace_header();
	  UmlCom::trace(QCString("<font color=\"red\"><b><i> ")
			+ name + " : </i> cannot open <i> " 
			+ src_path + "</i>, edit the <i> generation settings</i> (tab directory) or the <i>"
			+ pack->name() + "</i> C++ directory specification</b></font><br>");
	  incr_error();
	}
	else {
	  fputs((const char *) file, fp_src);
	  fclose(fp_src);
	}
      }
      else if (get_trace_header().isEmpty())
	UmlCom::trace(QCString("<font face=helvetica><i> ")
		      + src_path + "</i> not modified</font><br>");
    }
  }
}

void UmlArtifact::generate_text() {
  const QCString srcdef = cppSource();
  
  if (srcdef.isEmpty()) {
    if (verbose())
      UmlCom::trace(QCString("<hr><font face=helvetica>artifact <i>")
		    + name() + "</i> has an empty C++ definition</font><br>");
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
		    + pack->name() + "</i> C++ directory specification</b></font><br>");
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

bool UmlArtifact::must_be_saved(const char * path, const char * new_contains)
{
  FILE * fp = fopen(path, "rb");
  
  if (fp == 0)
    return TRUE;
  
  int c;
  
  while ((c = fgetc(fp)) != EOF) {
    if (((char ) c) != *new_contains++) {
      fclose(fp);
      return TRUE;
    }
  }
  
  fclose(fp);
  
  return *new_contains != 0;
}
