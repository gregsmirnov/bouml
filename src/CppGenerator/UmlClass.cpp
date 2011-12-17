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

#include <stdlib.h>
#include <qtextstream.h>

#include "UmlClass.h"
#include "UmlPackage.h"
#include "UmlRelation.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"
#include "UmlArtifact.h"
#include "UmlSettings.h"
#include "CppSettings.h"
#include "UmlCom.h"
#include "CppRefType.h"
#include "util.h"

// the eventual class list where we are, used by write() to not generate
// parent classes and template in the class declaration of the class

QList<UmlClass> UmlClass::context;
QValueList<UmlActualParameter> UmlClass::noactuals;
 
QCString UmlClass::cpp_stereotype()
{
  QCString s = CppSettings::classStereotype(stereotype());
  
  return ((s == "struct") || (s == "union") || (s == "enum") || (s == "typedef"))
    ? s : QCString("class");
}

void UmlClass::generate() {
  if (! managed) {
    managed = TRUE;
    
    if (!isCppExternal() && !cppDecl().isEmpty()) {
      if (associatedArtifact() == 0) {
	if (verbose())
	  UmlCom::trace(QCString("<hr><font face=helvetica><i> ") + name() +
			" : </i> does not have associated <i>artifact</i></font><br>");
      }
      else if (cppDecl().isEmpty()) {
	if (verbose())
	  UmlCom::trace(QCString("<hr><font face=helvetica>class <i>")
			+ name() + "</i> has an empty C++ definition</font><br>");
      }
      else
	associatedArtifact()->generate();
    }
  }
}

void UmlClass::compute_dependencies(QList<CppRefType> & dependencies, 
				    bool all_in_h) {
  const QCString dummy;
  
  compute_dependency(dependencies, dummy, all_in_h);
}

void UmlClass::compute_dependency(QList<CppRefType> & dependencies,
				  const QCString &, bool all_in_h) {
  QVector<UmlItem> ch = children();
  const QCString stereotype = cpp_stereotype();
  bool a_typedef = (stereotype == "typedef");
  bool an_enum = (stereotype == "enum");
  const QValueList<UmlFormalParameter> formals = this->formals();
  const QValueList<UmlActualParameter> actuals = this->actuals();  
  
  if (!formals.isEmpty())
    // template class, force depend in h
    all_in_h = TRUE;
  
  for (unsigned index = 0; index != ch.size(); index += 1) {
    if (ch[index]->kind() != aNcRelation) {
      UmlClassItem * it = (UmlClassItem *) ch[index];
      
      if (! it->cppDecl().isEmpty())
	it->compute_dependency(dependencies, stereotype, all_in_h);
    }
  }
  
  if (an_enum && (!formals.isEmpty() || !actuals.isEmpty())) {
    write_trace_header();
    UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b><i>template enum</i></b></font><br>");
    incr_warning();
  }
  else if (a_typedef && !formals.isEmpty()) {
    write_trace_header();
    UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b><i>template typedef</i></b></font><br>");
    incr_warning();
  }
  else {
    QValueList<UmlFormalParameter>::ConstIterator itf;
    
    for (itf = formals.begin(); itf != formals.end(); ++itf)
      CppRefType::remove((*itf).name(), dependencies);
  
    QValueList<UmlActualParameter>::ConstIterator ita;
    
    for (ita = actuals.begin(); ita != actuals.end(); ++ita)
      UmlClassMember::compute_dependency(dependencies, "${type}",
					 (*ita).value(), all_in_h);
    
    if (a_typedef) {
      QCString decl = cppDecl();
      int index;
      
      remove_comments(decl);
      if ((index = decl.find("${name}")) != -1)
	decl.remove((unsigned) index, 7);
      replace_alias(decl);

      UmlClassMember::compute_dependency(dependencies, decl,
					 baseType(), all_in_h);
    }
  }
  
  if ((associatedArtifact() == 0) ||
      (associatedArtifact()->associatedClasses().count() == 1))
    CppRefType::remove(this, dependencies);
  else
    CppRefType::force_ref(this, dependencies);
}

void UmlClass::generate_decl(QTextOStream & f_h, QCString indent) {
  context.append(this);
  
  bool removed = FALSE;
  QVector<UmlItem> ch = children();
  const unsigned sup = ch.size();
  const QCString & stereotype = cpp_stereotype();
  bool a_typedef = (stereotype == "typedef");
  bool an_enum = (stereotype == "enum");
  const QValueList<UmlFormalParameter> formals = this->formals();
  const QValueList<UmlActualParameter> actuals = this->actuals();  
  unsigned index;
  const char * p = cppDecl();
  const char * pp = 0;
  const char * sep;
  bool nestedp = parent()->kind() == aClass;
  
  if (nestedp)
    indent += "    ";
  
  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  if (*p != '#')
    f_h << indent;
    
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      pp = 0;
      if (*p == 0)
	break;
      if (*p != '#')
	f_h << indent;
    }

    if (*p == '\n') {
      f_h << *p++;
      if (*p && (*p != '#') &&
	  strncmp(p, "${members}", 10) &&
	  strncmp(p, "${items}", 8))
	f_h << indent;
    }
    else if (*p == '@')
      manage_alias(p, f_h);
    else if (*p != '$')
      f_h << *p++;
    else if (!strncmp(p, "${comment}", 10))
      manage_comment(p, pp, CppSettings::isGenerateJavadocStyleComment());
    else if (!strncmp(p, "${description}", 14))
      manage_description(p, pp);
    else if (! strncmp(p, "${name}", 7)) {
      p += 7;
      f_h << name();
    }
    else if (a_typedef) {
      if (!strncmp(p, "${type}", 7)) {
	p += 7;
	UmlClass::write(f_h, baseType(), FALSE);
	
	UmlClass * cl = baseType().type;
	
	if ((cl != 0) && !actuals.isEmpty()) {
	  QValueList<UmlActualParameter>::ConstIterator ita;
	  BooL need_space = FALSE;
    
	  for (ita = actuals.begin(); ita != actuals.end(); ++ita)
	    if ((*ita).superClass() == cl)
	      if (! (*ita).generate(f_h, need_space))
		// no specified value
		break;
	  
	  if (need_space)
	    f_h << " >";
	  else
	    f_h << '>';
	}
      }
      else
	// strange
	f_h << *p++;
    }
    else if (an_enum) {
      if (!strncmp(p, "${items}", 8)) {
	p += 8;
	
	// items declaration
	
	aVisibility current_visibility = DefaultVisibility;
	unsigned max = sup - 1;
	BooL first = TRUE;
	
	for (index = 0; index < sup; index += 1) {
	  UmlItem * it = ch[index];
	  
	  switch (it->kind()) {
	  case aClass:
	  case aNcRelation:
	    break;
	  default:
	    if (! ((UmlClassItem *) it)->cppDecl().isEmpty())
	      ((UmlClassItem *) it)->generate_decl(current_visibility,
						   f_h, stereotype, indent,
						   first, index == max);
	  }
	}
      
	if (*p == '}')
	  f_h << indent;
      }
      else
	// strange
	f_h << *p++;
    }
    else if (! strncmp(p, "${template}", 11)) {
      p += 11;
      
      // template
      
      if (!formals.isEmpty()) {
	sep = "template<";
	const char * sep2 = "<";
	BooL need_space = FALSE;
	
	QValueList<UmlFormalParameter>::ConstIterator itf;
	
	for (itf = formals.begin(); itf != formals.end(); ++itf)
	  (*itf).generate(f_h, sep, sep2, need_space);
	
	f_h << ((need_space) ? " >\n" : ">\n");
	if (nestedp)
	  f_h << indent;
      }
      else if (name().find('<') != -1) {
	f_h << "template<>\n";
	if (nestedp)
	  f_h << indent;
      }
    }
    else if (! strncmp(p, "${inherit}", 10)) {
      p += 10;

      // inherit
  
      sep = " : ";
      
      for (index = 0; index != sup; index += 1) {
	UmlItem * x = ch[index];
	
	if ((x->kind() == aRelation) &&
	    !((UmlRelation *) ch[index])->cppDecl().isEmpty())
	  ((UmlRelation *) x)->generate_inherit(sep, f_h, actuals, stereotype);
      }
    }
    else if (! strncmp(p, "${members}", 10)) {
      p += 10;
  
      // members declaration
      
      aVisibility current_visibility;
      
      current_visibility = ((stereotype == "struct") || (stereotype == "union"))
	? PublicVisibility : DefaultVisibility;
      unsigned last = sup - 1;
      BooL first = TRUE;
      
      for (index = 0; index != sup; index += 1) {
	UmlItem * it = ch[index];
	  
	if ((it->kind() != aNcRelation) &&
	    ! ((UmlClassItem *) it)->cppDecl().isEmpty())
	  ((UmlClassItem *) it)->generate_decl(current_visibility,
					       f_h, stereotype, indent,
					       first, index == last);
      }
      
      if (*p == '}')
	f_h << indent;
    }
    else if (!strncmp(p, "${inlines}", 10)) {
      p += 10;
  
      context.removeLast();
      removed = TRUE;
      
      if (! nestedp) {
	// inline operations definition
	// template class members
	QCString templates;
	QCString cl_names;
	QCString templates_tmplop;
	QCString cl_names_tmplop;
	
	spec(templates, cl_names, templates_tmplop, cl_names_tmplop);
	
	for (index = 0; index != sup; index += 1)
	  if (ch[index]->kind() != aNcRelation)
	    ((UmlClassItem *) ch[index])
	      ->generate_def(f_h, indent, TRUE, templates, cl_names,
			     templates_tmplop, cl_names_tmplop);
      }
      
      if (*p == '\n')
	p += 1;
    }
    else
      // strange
      f_h << *p++;
  }
  
  if (! removed)
    context.removeLast();
}

void UmlClass::generate_decl(aVisibility & current_visibility, QTextOStream & f_h,
			     const QCString &, QCString indent,
			     BooL & first, bool) {
  generate_visibility(current_visibility, f_h, first, indent);
  first = FALSE;
  
  generate_decl(f_h, indent);
  f_h << '\n';
}

void UmlClass::generate_def(QTextOStream & f, QCString indent, bool h) {
  if (! cppDecl().isEmpty()) {
    QVector<UmlItem> ch = children();
    QCString templates;
    QCString cl_names;
    QCString templates_tmplop;
    QCString cl_names_tmplop;
    
    spec(templates, cl_names, templates_tmplop, cl_names_tmplop);
    
    for (unsigned index = 0; index != ch.size(); index += 1)
      if (ch[index]->kind() != aNcRelation)
	((UmlClassItem *) ch[index])
	  ->generate_def(f, indent, h, templates, cl_names,
			 templates_tmplop, cl_names_tmplop);
  }
}

void UmlClass::generate_def(QTextOStream & f, QCString indent, bool h,
			    QCString templates, QCString cl_names,
			    QCString, QCString) {
  if (! cppDecl().isEmpty()) {
    QCString template1;
    QCString template2;
    QCString templates_tmplop;
    QCString cl_names_tmplop;
    
    get_template_prefixes(template1, template2);
    templates_tmplop = templates + "template<>\n";
    templates += template1;
    cl_names_tmplop = cl_names + "::" + name()/* true_name */;
    cl_names = cl_names_tmplop + template2;
  
    QVector<UmlItem> ch = children();
    
    for (unsigned index = 0; index != ch.size(); index += 1)
      if (ch[index]->kind() != aNcRelation)
	((UmlClassItem *) ch[index])
	  ->generate_def(f, indent, h, templates, cl_names,
			 templates_tmplop, cl_names_tmplop);
  }
}

void UmlClass::get_template_prefixes(QCString & template1,
				     QCString & template2) {
  QValueList<UmlFormalParameter> formals = this->formals();
  
  if (!formals.isEmpty()) {
    QValueList<UmlFormalParameter>::ConstIterator it;
    const char * sep1 = "template<";
    const char * sep2 = "<";
    
    for (it = formals.begin(); it != formals.end(); ++it) {
      const UmlFormalParameter & f = *it;
      
      template1 += sep1;
      template1 += f.type();
      template1 += " ";
      template1 += f.name();
      
      template2 += sep2;
      template2 += f.name();
      
      sep1 = sep2 = ", ";
    }
    
    template1 += ">\n";
    template2 += ">";
  }
}

void UmlClass::spec(QCString & templates, QCString & names,
		    QCString & templates_tmplop,
		    QCString & names_tmplop) {
  if (parent()->kind() == aClass) {
    ((UmlClass *) parent())
      ->spec(templates, names, templates_tmplop, names_tmplop);
    names += "::";
  }
  
  QCString t1;
  QCString t2;
  
  get_template_prefixes(t1, t2);
  templates_tmplop = templates + "template<>\n";
  templates += t1;
  // true_name !
  names_tmplop = names + name();
  names = names_tmplop + t2;
}

// warning : nested case not managed

QCString UmlClass::decl() {      
  QCString result;
  QCString close_template;
  UmlArtifact * cp = associatedArtifact();
  QCString nasp = ((UmlPackage *)
		   ((cp != 0) ? (UmlItem *) cp : (UmlItem *) this)->package())
    ->cppNamespace();
    
  if (! nasp.isEmpty()) {
    int index = 
      // bypass :: allowing ::a...
      ((nasp.at(0) == ':') && (nasp != "::")) ? 2 : 0;
    int index2 = 0;
    
    while ((index2 = nasp.find("::", index)) != -1) {
      result += "namespace " + nasp.mid(index, index2 - index) + " { ";
      close_template += " } ";
      index = index2 + 2;
    }
    result += "namespace " + nasp.mid(index) + " { ";
    close_template += " } ";
  }
  
  QCString template1;
  QCString template2;
  
  get_template_prefixes(template1, template2);
  
  if (!template1.isEmpty())
    result += template1.left(template1.length() - 1) + ' ';
  
  result += cpp_stereotype() + ' ';
  
  return result + name() + ';' + close_template + '\n';
}

void UmlClass::write(QTextOStream & f, const UmlTypeSpec & t,
		     bool with_formals, BooL * is_template)
{
  if (t.type != 0)
    t.type->write(f, with_formals, is_template);
  else {
    QCString s = CppSettings::type(t.explicit_type);
    
    f << s;
    
    if (is_template != 0)
      *is_template = (!s.isEmpty() && (s.at(s.length() - 1) == '>'));
  }
	
}

void UmlClass::write(QTextOStream & f, bool with_formals, BooL * is_template,
		     const QValueList<UmlActualParameter> & actuals) {
  if (context.findRef(this) == -1) {
    if (parent()->kind() == aClass) {
      if (context.findRef((UmlClass *) parent()) == -1) {
	// parent cannot have formals, but may have actuals
	((UmlClass *) parent())->write(f, FALSE, 0, actuals);
	f << "::";
      }
    }
    else {
      UmlArtifact * cp = associatedArtifact();
      QCString nasp = ((UmlPackage *)
		       ((cp != 0) ? (UmlItem *) cp : (UmlItem *) this)->package())
	->cppNamespace();
      
      if (CppSettings::isForceNamespacePrefixGeneration() ||
	  (nasp != UmlArtifact::generation_package()->cppNamespace()))
	f << nasp << "::";
    }
  }
  
  QCString s;
  
  if (isCppExternal()) {
    s = cppDecl();
    
    int index = s.find('\n');
    
    s = (index == -1) ? s.stripWhiteSpace()
		      : s.left(index).stripWhiteSpace();
      
    if ((index = s.find("${name}")) != -1)
      s.replace(index, 7, name());
    else if ((index = s.find("${Name}")) != -1)
      s.replace(index, 7, capitalize(name()));
    else if ((index = s.find("${NAME}")) != -1)
      s.replace(index, 7, name().upper());
    else if ((index = s.find("${nAME}")) != -1)
      s.replace(index, 7, name().lower());
  }
  else 
    s = name();	// true_name
  
  if (! s.isEmpty()){
    f << s;
    
    if (is_template != 0)
      *is_template = (s.at(s.length() - 1) == '>');
  }
  else if (is_template != 0)
    *is_template = FALSE;
    
  if (with_formals) {
    QValueList<UmlFormalParameter> formals = this->formals();
    
    if (! formals.isEmpty()) {
      const char * sep = "<";
      QValueList<UmlFormalParameter>::ConstIterator it;
      
      for (it = formals.begin(); it != formals.end(); ++it) {
	f << sep << (*it).name();
	sep = ", ";
      }
      
      f << '>';
      
      if (is_template != 0)
	*is_template = TRUE;
    }
  }
  else if (!actuals.isEmpty()) {
    QValueList<UmlActualParameter>::ConstIterator ita;
    BooL need_space = FALSE;
    bool used = FALSE;
    
    for (ita = actuals.begin(); ita != actuals.end(); ++ita) {
      if ((*ita).superClass() == this) {
	used = TRUE;
	(*ita).generate(f, need_space);
      }
    }
    
    if (used) {
      if (need_space)
	f << " >";
      else
	f << '>';
      
      if (is_template != 0)
	*is_template = TRUE;
    }
  }
}

