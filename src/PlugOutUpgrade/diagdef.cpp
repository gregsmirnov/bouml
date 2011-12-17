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

#include "UmlClassView.h"
#include "UmlDeploymentView.h"
#include "UmlClass.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"
#include "UmlRelation.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "UmlArtifact.h"
#include "UmlExtraClassMember.h"
#include "UmlPackage.h"
  
#include "util.h"
#include "diagdef.h"

//
// add access to use case, collaboration and sequence diagram 'machine'
//

void add_fragment(UmlClassView * base_class_view, UmlClassView * user_class_view,
		  UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		  UmlClass *& user_fragment, UmlClass *& user_fragment_compart)
{
  user_fragment = 
    UmlClass::made(user_class_view, user_depl_view, "UmlFragment", FALSE);
  user_fragment_compart = 
    UmlClass::made(user_class_view, user_depl_view, "UmlFragmentCompartment", FALSE);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  UmlClass * base_fragment = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseFragment", FALSE);
  UmlClass * base_fragment_compart = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseFragmentCompartment", FALSE);
  
  base_fragment->set_Description(" this class manages fragments");
  user_fragment->set_Description(" this class manages fragments, you can modify it");

  base_fragment_compart->set_Description(" this class manages fragments compartments,\n"
					 " a fragment without separator contains one compartment");
  user_fragment_compart->set_Description(" this class manages fragments compartments,\n"
					 " a fragment without separator contains one compartment"
					 " you can modify it");

  UmlOperation * op;
  
  op = base_fragment->add_op("name", PublicVisibility, "string");
  op->set_isCppConst(TRUE);
  op->set_Description(" return the name");
  op->set_cpp("const ${type} &", "", "  return _name;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _name;\n", TRUE);

  op = base_fragment->add_op("compartments", PublicVisibility, user_fragment_compart);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the compartments, at least one compartment exists");
  op->set_cpp("const QVector<${type}> &", "", "  return _compartments;\n", TRUE, 0, 0);
  op->set_java("${type}[]", "", "  return _compartments;\n", TRUE);
  
  op = base_fragment->add_op("container", PublicVisibility, user_fragment_compart);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the fragment compartment containing the fragment,\n"
		      " or 0/null");
  op->set_cpp("${type} *", "", "  return _container;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _container;\n", TRUE);
  
  //
  
  op = base_fragment_compart->add_op("fragment", PublicVisibility, user_fragment);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the fragment owning the compartment");
  op->set_cpp("${type} *", "", "  return _fragment;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _fragment;\n", TRUE);
  
  op = base_fragment_compart->add_op("rank", PublicVisibility, "int");
  op->set_isCppConst(TRUE);
  op->set_Description(" the rank of the compartment in the fragment (0..)");
  op->set_cpp("${type}", "", "  return _rank;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _rank;\n", TRUE);
  
  op = base_fragment_compart->add_op("texts", PublicVisibility, "string");
  op->set_isCppConst(TRUE);
  op->set_Description(" the texts placed in the compartment");
  op->set_cpp("const QVector<char> &", "", "  return _texts;\n", TRUE, 0, 0);
  op->set_java("${type}[]", "", "  return _texts;\n", TRUE);
  
  op = base_fragment_compart->add_op("contained", PublicVisibility, user_fragment);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the fragments contained in the compartment,\n"
		      " may be none");
  op->set_cpp("const QVector<${type}> &", "", "  return _contained;\n", TRUE, 0, 0);
  op->set_java("${type}[]", "", "  return _contained;\n", TRUE);
  
  //
  
  base_fragment_compart->add_relation(aDirectionalAssociation, "_fragment", PrivateVisibility,
				      user_fragment, 0, 0);
  base_fragment_compart->add_vect_assoc("_contained", PrivateVisibility, user_fragment, 0, 0);
  base_fragment->add_relation(aDirectionalAssociation, "_container", PrivateVisibility,
			      user_fragment_compart, 0, 0);
  base_fragment->add_vect_assoc("_compartments", PrivateVisibility, user_fragment_compart, 0, 0);
  
  UmlAttribute * att;
  QCString s;
  
  base_fragment->add_attribute("_name", PrivateVisibility, "string", 0, 0);
  base_fragment_compart->add_attribute("_rank", PrivateVisibility, "int", 0, 0);
  att = base_fragment_compart->add_attribute("_texts", PrivateVisibility, "string", 0, 0);
  s = att->cppDecl();
  att->set_CppDecl(s.replace(s.find("${type}"), 7, "QVector<char>"));
  s = att->javaDecl();
  att->set_JavaDecl(s.insert(s.find("${type}") + 7, "[]"));
  base_fragment->add_attribute("_x", PrivateVisibility, "int", 0, 0);
  base_fragment->add_attribute("_y", PrivateVisibility, "int", 0, 0);
  base_fragment->add_attribute("_w", PrivateVisibility, "int", 0, 0);
  base_fragment->add_attribute("_h", PrivateVisibility, "int", 0, 0);
  base_fragment_compart->add_attribute("_y", PrivateVisibility, "int", 0, 0);
  
  op = base_fragment->add_op("read_", PublicVisibility, "void");
  op->set_Description(" internal, don't call it");
  op->set_cpp("${type}", "",
	      "  _name = UmlCom::read_string();\n"
	      "  _x = (int) UmlCom::read_unsigned();\n"
	      "  _y = (int) UmlCom::read_unsigned();\n"
	      "  _w = (int) UmlCom::read_unsigned();\n"
	      "  _h = (int) UmlCom::read_unsigned();\n"
	      "\n"
	      "  unsigned n = UmlCom::read_unsigned();\n"
	      "\n"
	      "  _compartments.resize(n);\n"
	      "  for (unsigned rank = 0; rank != n; rank += 1) {\n"
	      "    UmlFragmentCompartment * fc = new UmlFragmentCompartment();\n"
	      "\n"
	      "    _compartments.insert(rank, fc);\n"
	      "    fc->init(this, rank, UmlCom::read_unsigned());\n"
	      "  }\n"
	      "  _compartments.setAutoDelete(TRUE);\n",
	      FALSE, 0, 0);
  op->set_CppVisibility(PrivateVisibility);
  op->set_java("${type}", "",
	       "  _name = UmlCom.read_string();\n"
	       "  _x = UmlCom.read_unsigned();\n"
	       "  _y = UmlCom.read_unsigned();\n"
	       "  _w = UmlCom.read_unsigned();\n"
	       "  _h = UmlCom.read_unsigned();\n"
	       "\n"
	       "  int n = UmlCom.read_unsigned();\n"
	       "  int rank;\n"
	       "\n"
	       "  _compartments = new UmlFragmentCompartment[n];\n"
	       "  for (rank = 0; rank != n; rank += 1) {\n"
	       "    UmlFragmentCompartment fc = new UmlFragmentCompartment();\n"
	       "\n"
	       "    _compartments[rank] = fc;\n"
	       "    fc.init(this, rank, UmlCom.read_unsigned());\n"
	       "  }\n",
	       FALSE);
  
  op = base_fragment->add_op("get_container_", PublicVisibility, user_fragment_compart);
  op->set_isClassMember(TRUE);
  op->set_Description(" internal");
  op->add_param(0, InputDirection, "x", "int");
  op->add_param(1, InputDirection, "y", "int");
  op->add_param(2, InputDirection, "w", "int");
  op->add_param(3, InputDirection, "h", "int");
  op->add_param(4, InputDirection, "fragments", user_fragment);
  op->set_cpp("${type} *", "${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, ${t3} ${p3}, const QVector<${t4}> & ${p4}",
	      "  UmlFragmentCompartment * r = 0;\n"
	      "  int nf = fragments.count();\n"
	      "  int frank;\n"
	      "\n"
	      "  for (frank = 0; frank != nf; frank += 1) {\n"
	      "    const UmlBaseFragment * f = fragments[frank];\n"
	      "\n"
	      "    if ((x > f->_x) && (y > f->_y) &&\n"
	      "        ((x + w) < (f->_x + f->_w)) && ((y + h) < (f->_y + f->_h))) {\n"
	      "      int y0 = f->_y;\n"
	      "      int nfc = f->_compartments.count();\n"
	      "      int fcrank;\n"
	      "\n"
	      "      for (fcrank = 0; fcrank != nfc; fcrank += 1) {\n"
	      "        UmlFragmentCompartment * fc = f->_compartments[fcrank];\n"
	      "\n"
	      "        if ((y > y0) &&\n"
	      "            ((y + h) < fc->b()) &&\n"
	      "            ((r == 0) || fc->smaller(r))) {\n"
	      "          r = fc;\n"
	      "          break;\n"
	      "        }\n"
	      "        y0 = fc->b();\n"
	      "      }\n"
	      "    }\n"
	      "  }\n"
	      "  return r;\n"
	      , FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, ${t3} ${p3}, ${t4}[] ${p4}",
	       "  UmlFragmentCompartment r = null;\n"
	       "  int nf = fragments.length;\n"
	       "  int frank;\n"
	       "\n"
	       "  for (frank = 0; frank != nf; frank += 1) {\n"
	       "    UmlBaseFragment f = fragments[frank];\n"
	       "\n"
	       "    if ((x > f._x) && (y > f._y) &&\n"
	       "        ((x + w) < (f._x + f._w)) && ((y + h) < (f._y + f._h))) {\n"
	       "      int y0 = f._y;\n"
	       "      int nfc = f._compartments.length;\n"
	       "      int fcrank;\n"
	       "\n"
	       "      for (fcrank = 0; fcrank != nfc; fcrank += 1) {\n"
	       "        UmlBaseFragmentCompartment fc = f._compartments[fcrank];\n"
	       "\n"
	       "        if ((y > y0) &&\n"
	       "            ((y + h) < fc.b()) &&\n"
	       "            ((r == null) || fc.smaller(r))) {\n"
	       "          r = f._compartments[fcrank];\n"
	       "          break;\n"
	       "        }\n"
	       "        y0 = fc.b();\n"
	       "      }\n"
	       "    }\n"
	       "  }\n"
	       "  return r;\n"
	       , FALSE);
  
  op = base_fragment->add_op("compute_container_", PublicVisibility, "void");
  op->set_isClassMember(TRUE);
  op->set_Description(" internal");
  op->add_param(0, InputDirection, "fragments", user_fragment);
  op->set_cpp("${type}", "QVector<${t0}> & ${p0}",
	      "  int rank = fragments.count();\n"
	      "\n"
	      "  while (rank-- != 0) {\n"
	      "    UmlBaseFragment * f = fragments[rank];\n"
	      "    UmlFragmentCompartment * fc = get_container_(f->_x, f->_y, f->_w, f->_h, fragments);\n"
	      "\n"
	      "    if (fc != 0) {\n"
	      "      f->_container = fc;\n"
	      "      fc->add_contained_((UmlFragment *) f);\n"
	      "    }\n"
	      "  }\n"
	      , FALSE, 0, 0);
  op->set_CppVisibility(PrivateVisibility);
  op->set_java("${type}", "${t0}[] ${p0}",
	       "  int rank = fragments.length;\n"
	       "\n"
	       "  while (rank-- != 0) {\n"
	       "    UmlBaseFragment f = fragments[rank];\n"
	       "    UmlFragmentCompartment fc = get_container_(f._x, f._y, f._w, f._h, fragments);\n"
	       "\n"
	       "    if (fc != null) {\n"
	       "      f._container = fc;\n"
	       "      fc.add_contained_((UmlFragment) f);\n"
	       "    }\n"
	       "  }\n"
	       , FALSE);
  
  op = base_fragment->add_op("w", PublicVisibility, "int");
  op->set_isCppConst(TRUE);
  op->set_cpp("${type}", "", "  return _w;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _w;\n", TRUE);
  
  op = base_fragment->add_op("h", PublicVisibility, "int");
  op->set_isCppConst(TRUE);
  op->set_cpp("${type}", "", "  return _h;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _h;\n", TRUE);
  
  base_fragment->add_destr(PublicVisibility, "  to avoid compiler warning, don't call it");
  base_fragment->add_friend("UmlBaseUseCaseDiagramDefinition");
  base_fragment->add_friend("UmlBaseSequenceDiagramDefinition");
  include_umlcom(base_fragment);
  
  op = base_fragment_compart->add_op("add_contained_", PublicVisibility, "void");
  op->add_param(0, InputDirection, "x", user_fragment);
  op->set_cpp("${type}", "${t0} * ${p0}",
	      "  _contained.resize(_contained.size() + 1);\n"
	      "  _contained.insert(_contained.size() - 1, x);\n"
	      , FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}",
	       "  UmlFragment[] v = _contained;\n"
	       "  int n = _contained.length;\n"
	       "\n"
	       "  _contained = new UmlFragment[n + 1];\n"
	       "  System.arraycopy(v, 0, _contained, 0, n);\n"
	       "  _contained[n] = x;\n"
	       , FALSE);
  
  op = base_fragment_compart->add_op("add_text_", PublicVisibility, "void");
  op->add_param(0, InputDirection, "x", "string");
  op->set_cpp("${type}", "${t0} ${p0}",
	      "  _texts.resize(_texts.size() + 1);\n"
	      "  _texts.insert(_texts.size() - 1, strdup(x));\n"
	      , FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}",
	       "  String[] v = _texts;\n"
	       "  int n = _texts.length;\n"
	       "\n"
	       "  _texts = new String[n + 1];\n"
	       "  System.arraycopy(v, 0, _texts, 0, n);\n"
	       "  _texts[n] = x;\n"
	       , FALSE);
  
  op = base_fragment_compart->add_op("b", PublicVisibility, "int");
  op->set_isCppConst(TRUE);
  op->set_cpp("${type}", "", "  return _y;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _y;\n", TRUE);
  
  op = base_fragment_compart->add_op("smaller", PublicVisibility, "bool");
  op->set_isCppConst(TRUE);
  op->add_param(0, InputDirection, "x", base_fragment_compart);
  op->set_cpp("${type}", "const ${t0} * ${p0}",
	      "  return ((_fragment->w() < x->_fragment->w()) &&\n"
	      "          (_fragment->h() < x->_fragment->h()));\n"
	      , FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}",
	      "  return ((_fragment.w() < x._fragment.w()) &&\n"
	      "          (_fragment.h() < x._fragment.h()));\n"
	       , FALSE);
  
  op = base_fragment_compart->add_op("init", PublicVisibility, "void");
  op->set_CppVisibility(PrivateVisibility);
  op->set_Description(" internal, don't call it");
  op->add_param(0, InputDirection, "fragment", base_fragment);
  op->add_param(1, InputDirection, "rank", "int");
  op->add_param(2, InputDirection, "y", "int");
  op->set_cpp("${type}", "${t0} * ${p0}, ${t1} ${p1}, ${t2} ${p2}",
	      " _fragment = (UmlFragment *) fragment;\n"
	      " _rank = rank;\n"
	      " _y = y;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}",
	       " _fragment = (UmlFragment) fragment;\n"
	       " _rank = rank;\n"
	       " _y = y;\n"
	       " _contained = new UmlFragment[0];\n"
	       " _texts = new String[0];\n"
	       , FALSE);
  
  base_fragment_compart->add_destr(PublicVisibility, "  to avoid compiler warning, don't call it");
  base_fragment_compart->add_friend("UmlBaseFragment");
  base_fragment_compart->add_friend("UmlBaseUseCaseDiagramDefinition");
  base_fragment_compart->add_friend("UmlBaseSequenceDiagramDefinition");
  base_fragment_compart->associatedArtifact()->add_includes("// to avoid compiler warning\n"
							    "#include \"UmlFragment.h\"\n"
							    "\n");
  
  UmlCom::set_user_id(uid);
  
  UmlRelation * rel;
  
  rel = UmlBaseRelation::create(aGeneralisation, user_fragment, base_fragment);
  rel->set_CppDecl("${type}");
  rel->set_JavaDecl("${type}");
  
  rel = UmlBaseRelation::create(aGeneralisation, user_fragment_compart, base_fragment_compart);
  rel->set_CppDecl("${type}");
  rel->set_JavaDecl("${type}");
}

UmlClass * add_subject(UmlClassView * base_class_view, UmlClassView * user_class_view,
		       UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view)
{
  UmlClass * user_subject = 
    UmlClass::made(user_class_view, user_depl_view, "UmlSubject", FALSE);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  UmlClass * base_subject = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseSubject", FALSE);  
  
  base_subject->set_Description(" this class manages subjects");
  user_subject->set_Description(" this class manages subjects, you can modify it");

  UmlOperation * op;
  
  op = base_subject->add_op("name", PublicVisibility, "string");
  op->set_isCppConst(TRUE);
  op->set_Description(" return the name");
  op->set_cpp("const ${type} &", "", "  return _name;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _name;\n", TRUE);

  base_subject->add_attribute("_name", PrivateVisibility, "string", 0, 0);
  base_subject->add_attribute("_x", PrivateVisibility, "int", 0, 0);
  base_subject->add_attribute("_y", PrivateVisibility, "int", 0, 0);
  base_subject->add_attribute("_w", PrivateVisibility, "int", 0, 0);
  base_subject->add_attribute("_h", PrivateVisibility, "int", 0, 0);
  
  op = base_subject->add_op("read_", PublicVisibility, "void");
  op->set_Description(" internal, don't call it");
  op->set_cpp("${type}", "",
	      "  _name = UmlCom::read_string();\n"
	      "  _x = (int) UmlCom::read_unsigned();\n"
	      "  _y = (int) UmlCom::read_unsigned();\n"
	      "  _w = (int) UmlCom::read_unsigned();\n"
	      "  _h = (int) UmlCom::read_unsigned();\n",
	      FALSE, 0, 0);
  op->set_CppVisibility(PrivateVisibility);
  op->set_java("${type}", "",
	       "  _name = UmlCom.read_string();\n"
	       "  _x = UmlCom.read_unsigned();\n"
	       "  _y = UmlCom.read_unsigned();\n"
	       "  _w = UmlCom.read_unsigned();\n"
	       "  _h = UmlCom.read_unsigned();\n",
	       FALSE);

  op = base_subject->add_op("get_container_", PublicVisibility, user_subject);
  op->set_isClassMember(TRUE);
  op->set_Description(" internal");
  op->add_param(0, InputDirection, "x", "int");
  op->add_param(1, InputDirection, "y", "int");
  op->add_param(2, InputDirection, "w", "int");
  op->add_param(3, InputDirection, "h", "int");
  op->add_param(4, InputDirection, "subjects", user_subject);
  op->set_cpp("${type} *", "${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, ${t3} ${p3}, const QVector<${t4}> & ${p4}",
	      "  int rank = subjects.count();\n"
	      "\n"
	      "  while (rank-- != 0) {\n"
	      "    UmlBaseSubject * s = subjects[rank];\n"
	      "\n"
	      "    if ((x > s->_x) && (y > s->_y) &&\n"
	      "        ((x + w) < (s->_x + s->_w)) && ((y + h) < (s->_y + s->_h)))\n"
	      "      return (UmlSubject  *) s;\n"
	      "  }\n"
	      "  return 0;\n"
	      , FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}, ${t2} ${p2}, ${t3} ${p3}, ${t4}[] ${p4}",
	       "  int rank = subjects.length;\n"
	       "\n"
	       "  while (rank-- != 0) {\n"
	       "    UmlBaseSubject s = subjects[rank];\n"
	       "\n"
	       "    if ((x > s._x) && (y > s._y) &&\n"
	       "        ((x + w) < (s._x + s._w)) && ((y + h) < (s._y + s._h)))\n"
	       "      return subjects[rank];\n"
	       "  }\n"
	       "  return null;\n"
	       , FALSE);
  
  base_subject->add_friend("UmlBaseUseCaseDiagramDefinition");
  include_umlcom(base_subject);
  
  UmlCom::set_user_id(uid);
  
  UmlRelation * rel = UmlBaseRelation::create(aGeneralisation, user_subject, base_subject);
  
  rel->set_CppDecl("${type}");
  rel->set_JavaDecl("${type}");
  
  return user_subject;
}

UmlClass * add_ucref(UmlClassView * base_class_view, UmlClassView * user_class_view,
		     UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		     UmlClass * user_fragment, UmlClass * user_fragment_compart,
		     UmlClass * user_subject)
{
  UmlClass * user_ucref = 
    UmlClass::made(user_class_view, user_depl_view, "UmlUseCaseReference", FALSE);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  UmlClass * base_ucref = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseUseCaseReference", FALSE);
  
  base_ucref->set_Description(" this class manages use case references");
  user_ucref->set_Description(" this class manages use case references, you can modify it");

  UmlOperation * op;
  
  op = base_ucref->add_op("useCase", PublicVisibility, UmlClass::get("UmlUseCase", 0));
  op->set_isCppConst(TRUE);
  op->set_Description(" return the use case");
  op->set_cpp("${type} *", "", "  return _use_case;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _use_case;\n", TRUE);
  
  op = base_ucref->add_op("fragment", PublicVisibility, user_fragment_compart);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the fragment compartment containing the\n"
		      " use case, or 0/null");
  op->set_cpp("${type} *", "", "  return _fragment;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _fragment;\n", TRUE);
  
  op = base_ucref->add_op("subject", PublicVisibility, user_subject);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the subject containing the use case, or 0/null");
  op->set_cpp("${type} *", "", "  return _subject;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _subject;\n", TRUE);
  
  base_ucref->add_relation(aDirectionalAssociation, "_use_case", PrivateVisibility,
			   UmlClass::get("UmlUseCase", 0), 0, 0);
  base_ucref->add_relation(aDirectionalAssociation, "_fragment", PrivateVisibility,
			   user_fragment_compart, 0, 0);
  base_ucref->add_relation(aDirectionalAssociation, "_subject", PrivateVisibility,
			   user_subject, 0, 0);
  base_ucref->add_attribute("_x", PrivateVisibility, "int", 0, 0);
  base_ucref->add_attribute("_y", PrivateVisibility, "int", 0, 0);
  base_ucref->add_attribute("_w", PrivateVisibility, "int", 0, 0);
  base_ucref->add_attribute("_h", PrivateVisibility, "int", 0, 0);
 
  op = base_ucref->add_op("read_", PublicVisibility, "void");
  op->set_Description(" internal, don't call it");
  op->add_param(0, InputDirection, "fragments", user_fragment);
  op->add_param(1, InputDirection, "subjects", user_subject);
  op->set_cpp("${type}", "const QVector<${t0}> & ${p0}, const QVector<${t1}> & ${p1}",
	      "  _use_case = (UmlUseCase *) UmlBaseItem::read_();\n"
	      "  _x = (int) UmlCom::read_unsigned();\n"
	      "  _y = (int) UmlCom::read_unsigned();\n"
	      "  _w = (int) UmlCom::read_unsigned();\n"
	      "  _h = (int) UmlCom::read_unsigned();\n"
	      "  _fragment = UmlBaseFragment::get_container_(_x, _y, _w, _h, fragments);\n"
	      "  _subject = UmlBaseSubject::get_container_(_x, _y, _w, _h, subjects);\n"
	      , FALSE, 0, 0);
  op->set_CppVisibility(PrivateVisibility);
  op->set_java("${type}", "${t0}[] ${p0}, ${t1}[] ${p1}",
	       "  _use_case = (UmlUseCase) UmlBaseItem.read_();\n"
	       "  _x = UmlCom.read_unsigned();\n"
	       "  _y = UmlCom.read_unsigned();\n"
	       "  _w = UmlCom.read_unsigned();\n"
	       "  _h = UmlCom.read_unsigned();\n"
	       "  _fragment = UmlBaseFragment.get_container_(_x, _y, _w, _h, fragments);\n"
	       "  _subject = UmlBaseSubject.get_container_(_x, _y, _w, _h, subjects);\n"
	       , FALSE);
  base_ucref->add_friend("UmlBaseUseCaseDiagramDefinition");
  UmlClass::get("UmlBaseItem", 0)->add_friend("UmlBaseUseCaseReference");
  include_umlcom(base_ucref);
  base_ucref->associatedArtifact()->add_import("import java.util.*;\n");
  
  UmlCom::set_user_id(uid);
  
  UmlRelation * rel = UmlBaseRelation::create(aGeneralisation, user_ucref, base_ucref);
  
  rel->set_CppDecl("${type}");
  rel->set_JavaDecl("${type}");
  
  return user_ucref;
}

UmlClass * add_ucassoc(UmlClassView * base_class_view, UmlClassView * user_class_view,
		       UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		       UmlClass * user_ucref)
{
  UmlClass * user_ucassoc = 
    UmlClass::made(user_class_view, user_depl_view, "UmlUseCaseAssociation", FALSE);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  UmlClass * base_ucassoc = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseUseCaseAssociation", FALSE);  
  
  base_ucassoc->set_Description(" this class manages association between use case and actor");
  user_ucassoc->set_Description(" this class manages association between use case and actor,\n"
				" you can modify it");

  UmlOperation * op;
  
  op = base_ucassoc->add_op("name", PublicVisibility, "string");
  op->set_isCppConst(TRUE);
  op->set_Description(" return the name");
  op->set_cpp("const ${type} &", "", "  return _name;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _name;\n", TRUE);

  op = base_ucassoc->add_op("stereotype", PublicVisibility, "string");
  op->set_isCppConst(TRUE);
  op->set_Description(" return the stereotype");
  op->set_cpp("const ${type} &", "", "  return _stereotype;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _stereotype;\n", TRUE);

  op = base_ucassoc->add_op("useCase", PublicVisibility, user_ucref);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the use case");
  op->set_cpp("${type} *", "", "  return _use_case;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _use_case;\n", TRUE);
  
  op = base_ucassoc->add_op("actor", PublicVisibility, UmlClass::get("UmlClass", 0));
  op->set_isCppConst(TRUE);
  op->set_Description(" return the actor");
  op->set_cpp("${type} *", "", "  return _actor;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _actor;\n", TRUE);
  
  op = base_ucassoc->add_op("primary", PublicVisibility, "bool");
  op->set_isCppConst(TRUE);
  op->set_Description(" return true if the actor is a primary actor for the use case ");
  op->set_cpp("${type}", "", "  return _primary;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _primary;\n", TRUE);
  
  base_ucassoc->add_relation(aDirectionalAssociation, "_use_case", PrivateVisibility,
			     user_ucref, 0, 0);
  base_ucassoc->add_relation(aDirectionalAssociation, "_actor", PrivateVisibility,
			     UmlClass::get("UmlClass", 0), 0, 0);
  base_ucassoc->add_attribute("_name", PrivateVisibility, "string", 0, 0);
  base_ucassoc->add_attribute("_stereotype", PrivateVisibility, "string", 0, 0);
  base_ucassoc->add_attribute("_primary", PrivateVisibility, "bool", 0, 0);

  op = base_ucassoc->add_op("read_", PublicVisibility, "void");
  op->set_Description(" internal, don't call it");
  op->add_param(0, InputDirection, "useCases", user_ucref);
  op->set_cpp("${type}", "const QPtrDict<${t0}> & ${p0}",
	      "  _use_case = (UmlUseCaseReference *) useCases[(void *) UmlCom::read_unsigned()];\n"
	      "  _actor = (UmlClass *) UmlBaseItem::read_();\n"
	      "  _primary = UmlCom::read_bool();\n"
	      "  _name = UmlCom::read_string();\n"
	      "  _stereotype = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_CppVisibility(PrivateVisibility);
  op->set_java("${type}", "Hashtable ${p0}",
	       "  _use_case = (UmlUseCaseReference) useCases.get(new Integer(UmlCom.read_unsigned()));\n"
	       "  _actor = (UmlClass) UmlBaseItem.read_();\n"
	       "  _primary = UmlCom.read_bool();\n"
	       "  _name = UmlCom.read_string();\n"
	       "  _stereotype = UmlCom.read_string();\n",
	       FALSE);
  base_ucassoc->add_friend("UmlBaseUseCaseDiagramDefinition");
  UmlClass::get("UmlBaseItem", 0)->add_friend("UmlBaseUseCaseAssociation");
  include_umlcom(base_ucassoc);
  base_ucassoc->associatedArtifact()->add_import("import java.util.*;\n");

  UmlCom::set_user_id(uid);
  
  UmlRelation * rel = UmlBaseRelation::create(aGeneralisation, user_ucassoc, base_ucassoc);
  
  rel->set_CppDecl("${type}");
  rel->set_JavaDecl("${type}");
  
  return user_ucassoc;
}

void add_ddef(UmlClass * cl, UmlClass * user_ddef)
{
  UmlOperation * def = cl->add_op("definition", PublicVisibility, user_ddef);
  QCString s;
  
  def->set_Description(" return the semantic part of the diagram not present in the model");
  s = 
    "  if (_def == 0) {\n"
    "    UmlCom::send_cmd(_identifier, sideCmd);\n"
    "    (_def = new " + user_ddef->name() + "())->read_();\n"
    "  }\n"
    "  return _def;\n";
  def->set_cpp("${type} *", "", s, FALSE, 0, 0);
  s = 
    "  if (_def == null) {\n"
    "    UmlCom.send_cmd(identifier_(), OnInstanceCmd.sideCmd);\n"
    "    (_def = new " + user_ddef->name() + "()).read_();\n"
    "  }\n"
    "  return _def;\n";
  def->set_java("${type}", "", s, FALSE);
  
  UmlOperation * op;
  
  op = cl->add_op("unload", PublicVisibility, "void");
  op->add_param(0, InputDirection, "rec", "bool");
  op->add_param(1, InputDirection, "del", "bool");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  if (_def != 0) { delete _def; _def = 0; }\n"
	      "  UmlBaseDiagram::unload(rec, del);\n",
	      FALSE, 0, 0);

  s = op->cppDecl();
  s.replace(s.find("${p0}"), 5, "= FALSE");
  s.replace(s.find("${p1}"), 5, "= FALSE");
  op->set_CppDecl(s);
  
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _def = null;\n"
	       "  super.unload(rec, del);\n",
	       FALSE);
  op->set_isCppVirtual(TRUE);
  op->set_Description(" to unload the object to free memory, it will be reloaded automatically\n\
 if needed. args unused");
  
  cl->add_relation(aDirectionalAssociation, "_def", PrivateVisibility,
		   user_ddef, 0, 0);

  op = cl->get_operation(cl->name());
  s = op->cppDecl();
  s.insert(s.findRev(")") + 1, ", _def(0)");
  op->set_CppDecl(s);
  s = op->javaDecl();
  s.insert(s.findRev("}"), "  _def = null;\n");
  op->set_JavaDecl(s);
}

void add_ucdiagdef(UmlClassView * base_class_view, UmlClassView * user_class_view,
		   UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		   UmlClass * user_ucref, UmlClass * user_ucassoc, 
		   UmlClass * user_fragment, UmlClass * user_subject)
{
  UmlClass * user_ddef = 
    UmlClass::made(user_class_view, user_depl_view, "UmlUseCaseDiagramDefinition", FALSE);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  UmlClass * base_ddef = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseUseCaseDiagramDefinition", FALSE);
  
  base_ddef->set_Description(" this class manages use case diagram definition");
  user_ddef->set_Description(" this class manages use case diagram definition,\n"
			     " you can modify it");

  UmlOperation * op;
  
  op = base_ddef->add_op("useCases", PublicVisibility, user_ucref);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the use cases present in the diagram");
  op->set_cpp("const QVector<${type}> &", "", "  return _use_cases;\n", TRUE, 0, 0);
  op->set_java("${type}[]", "", "  return _use_cases;\n", TRUE);

  op = base_ddef->add_op("actors", PublicVisibility, UmlClass::get("UmlClass", 0));
  op->set_isCppConst(TRUE);
  op->set_Description(" return the actors present in the diagram");
  op->set_cpp("const QVector<${type}> &", "", "  return _actors;\n", TRUE, 0, 0);
  op->set_java("${type}[]", "", "  return _actors;\n", TRUE);

  op = base_ddef->add_op("associations", PublicVisibility, user_ucassoc);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the associations between actor and use case present in the diagram");
  op->set_cpp("const QVector<${type}> &", "", "  return _rels;\n", TRUE, 0, 0);
  op->set_java("${type}[]", "", "  return _rels;\n", TRUE);
  
  op = base_ddef->add_op("fragments", PublicVisibility, user_fragment);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the fragments present in the diagram");
  op->set_cpp("const QVector<${type}> &", "", "  return _fragments;\n", TRUE, 0, 0);
  op->set_java("${type}[]", "", "  return _fragments;\n", TRUE);

  op = base_ddef->add_op("subjects", PublicVisibility, user_subject);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the subjects present in the diagram");
  op->set_cpp("const QVector<${type}> &", "", "  return _subjects;\n", TRUE, 0, 0);
  op->set_java("${type}[]", "", "  return _subjects;\n", TRUE);
  
  base_ddef->add_vect_assoc("_use_cases", PrivateVisibility, user_ucref, 0, 0);
  base_ddef->add_vect_assoc("_actors", PrivateVisibility, UmlClass::get("UmlClass", 0), 0, 0);
  base_ddef->add_vect_assoc("_rels", PrivateVisibility, user_ucassoc, 0, 0);
  base_ddef->add_vect_assoc("_fragments", PrivateVisibility, user_fragment, 0, 0);
  base_ddef->add_vect_assoc("_subjects", PrivateVisibility, user_subject, 0, 0);

  op = base_ddef->add_op("read_", PublicVisibility, "void");
  op->set_Description(" internal, don't call it");
  op->set_cpp("${type}", "",
	      "  QPtrDict<UmlUseCaseReference> ucrefs;\n"
	      "  unsigned n;\n"
	      "  unsigned rank;\n"
	      "\n"
	      "  n = UmlCom::read_unsigned();\n"
	      "  _fragments.resize(n);\n"
	      "  for (rank = 0; rank != n; rank += 1) {\n"
	      "    UmlFragment * f = new UmlFragment();\n"
	      "\n"
	      "    _fragments.insert(rank, f);\n"
	      "    f->read_();\n"
	      "  }\n"
	      "  _fragments.setAutoDelete(TRUE);\n"
	      "  UmlBaseFragment::compute_container_(_fragments);\n"
	      "\n"
	      "  n = UmlCom::read_unsigned();\n"
	      "  _subjects.resize(n);\n"
	      "  for (rank = 0; rank != n; rank += 1) {\n"
	      "    UmlSubject * sb = new UmlSubject();\n"
	      "\n"
	      "    _subjects.insert(rank, sb);\n"
	      "    sb->read_();\n"
	      "  }\n"
	      "  _subjects.setAutoDelete(TRUE);\n"
	      "\n"
	      "  n = UmlCom::read_unsigned();\n"
	      "  _use_cases.resize(n);\n"
	      "  for (rank = 0; rank != n; rank += 1) {\n"
	      "    UmlUseCaseReference * uc = new UmlUseCaseReference();\n"
	      "\n"
	      "    _use_cases.insert(rank, uc);\n"
	      "    ucrefs.insert((void *) UmlCom::read_unsigned(), uc);\n"
	      "    uc->read_(_fragments, _subjects);\n"
	      "  }\n"
	      "  _use_cases.setAutoDelete(TRUE);\n"
	      "\n"
	      "  n = UmlCom::read_unsigned();\n"
	      "  _actors.resize(n);\n"
	      "  for (rank = 0; rank != n; rank += 1)\n"
	      "    _actors.insert(rank, (UmlClass *) UmlBaseItem::read_());\n"
	      "\n"
	      "  n = UmlCom::read_unsigned();\n"
	      "  _rels.resize(n);\n"
	      "  for (rank = 0; rank != n; rank += 1) {\n"
	      "    UmlUseCaseAssociation * r = new UmlUseCaseAssociation();\n"
	      "\n"
	      "    _rels.insert(rank, r);\n"
	      "    r->read_(ucrefs);\n"
	      "  }\n"
	      "  _rels.setAutoDelete(TRUE);\n"
	      , FALSE, 0, 0);
  op->set_CppVisibility(PrivateVisibility);
  op->set_java("${type}", "",
	       "  Hashtable ucrefs;\n"
	       "  int n;\n"
	       "  int rank;\n"
	       "\n"
	       "  n = UmlCom.read_unsigned();\n"
	       "  _fragments = new UmlFragment[n];\n"
	       "  for (rank = 0; rank != n; rank += 1) {\n"
	       "    UmlFragment f = new UmlFragment();\n"
	       "\n"
	       "    _fragments[rank] = f;\n"
	       "    f.read_();\n"
	       "  }\n"
	      "  UmlBaseFragment.compute_container_(_fragments);\n"
	       "\n"
	       "  n = UmlCom.read_unsigned();\n"
	       "  _subjects = new UmlSubject[n];\n"
	       "  for (rank = 0; rank != n; rank += 1) {\n"
	       "    UmlSubject sb = new UmlSubject();\n"
	       "\n"
	       "    _subjects[rank] = sb;\n"
	       "    sb.read_();\n"
	       "  }\n"
	       "\n"
	       "  n = UmlCom.read_unsigned();\n"
	       "  _use_cases = new UmlUseCaseReference[n];\n"
	       "  ucrefs = new Hashtable((n == 0) ? 1 : n);\n"
	       "  for (rank = 0; rank != n; rank += 1) {\n"
	       "    UmlUseCaseReference uc = new UmlUseCaseReference();\n"
	       "\n"
	       "    _use_cases[rank] = uc;\n"
	       "    ucrefs.put(new Integer(UmlCom.read_unsigned()), uc);\n"
	       "    uc.read_(_fragments, _subjects);\n"
	       "  }\n"
	       "\n"
	       "  n = UmlCom.read_unsigned();\n"
	       "  _actors = new UmlClass[n];\n"
	       "  for (rank = 0; rank != n; rank += 1)\n"
	       "    _actors[rank] = (UmlClass) UmlBaseItem.read_();\n"
	       "\n"
	       "  n = UmlCom.read_unsigned();\n"
	       "  _rels = new UmlUseCaseAssociation[n];\n"
	       "  for (rank = 0; rank != n; rank += 1) {\n"
	       "    UmlUseCaseAssociation r = new UmlUseCaseAssociation();\n"
	       "\n"
	       "    _rels[rank] = r;\n"
	       "    r.read_(ucrefs);\n"
	       "  }\n"
	       , FALSE);

  UmlClass * d = UmlClass::get("UmlBaseUseCaseDiagram", 0);
  
  add_ddef(d, user_ddef);
  base_ddef->add_destr(PublicVisibility, "  to avoid compiler warning, don't call it");
  base_ddef->add_friend("UmlBaseUseCaseDiagram");
  UmlClass::get("UmlBaseItem", 0)->add_friend("UmlBaseUseCaseDiagramDefinition");
  include_umlcom(base_ddef);
  include_umlcom(d);
  base_ddef->associatedArtifact()->add_includes("// to avoid compiler warning\n"
						"#include \"UmlClass.h\"\n"
						"#include \"UmlSubject.h\"\n"
						"#include \"UmlFragment.h\"\n"
						"#include \"UmlFragmentCompartment.h\"\n"
						"#include \"UmlUseCaseReference.h\"\n"
						"#include \"UmlUseCaseAssociation.h\"\n"
						"\n");
  base_ddef->associatedArtifact()->add_import("import java.util.*;\n");

  UmlCom::set_user_id(uid);
  
  UmlRelation * rel = UmlBaseRelation::create(aGeneralisation, user_ddef, base_ddef);
  
  rel->set_CppDecl("${type}");
  rel->set_JavaDecl("${type}");
}

UmlClass * add_instanceref(UmlClassView * base_class_view, UmlClassView * user_class_view,
			   UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view)
{
  UmlClass * user_instref = 
    UmlClass::made(user_class_view, user_depl_view, "UmlClassInstanceReference", FALSE);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  UmlClass * base_instref = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseClassInstanceReference", FALSE);
  
  base_instref->set_Description(" this class manages class instance reference");
  user_instref->set_Description(" this class manages class instance reference,\n"
				" you can modify it");

  UmlOperation * op;
  
  op = base_instref->add_op("type", PublicVisibility, UmlClass::get("UmlClass", 0));
  op->set_isCppConst(TRUE);
  op->set_Description(" return the type of the instance");
  op->set_cpp("${type} *", "", 
	      "  return (_instance) ? _instance->type() : _class;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "", 
	       "  return (_instance != null) ? _instance.type() : _class;\n",
	       FALSE);

  op = base_instref->add_op("instance", PublicVisibility, UmlClass::get("UmlClassInstance", 0));
  op->set_isCppConst(TRUE);
  op->set_Description(" return the corresponding instance in the model, or 0/null");
  op->set_cpp("${type} *", "", "  return _instance;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _instance;\n", TRUE);

  op = base_instref->add_op("name", PublicVisibility, "string");
  op->set_isCppConst(TRUE);
  op->set_Description(" return the name of the instance");
  op->set_cpp("const ${type} &", "",
	      "  return (_instance) ? _instance->name() : _name;\n",
	      FALSE, 0, 0);
  op->set_java("${type}", "",
	       "  return (_instance != null) ? _instance.name() : _name;\n",
	       FALSE);
  
  base_instref->add_relation(aDirectionalAssociation, "_class", PrivateVisibility,
			     UmlClass::get("UmlClass", 0), 0, 0);
  base_instref->add_relation(aDirectionalAssociation, "_instance", PrivateVisibility,
			     UmlClass::get("UmlClassInstance", 0), 0, 0);
  base_instref->add_attribute("_name", PrivateVisibility, "string", 0, 0);
  
  op = base_instref->add_op("read_", PublicVisibility, "void");
  op->set_Description(" internal, don't call it");
  op->set_cpp("${type}", "",
	      "  _instance = (UmlClassInstance *) UmlBaseItem::read_();\n"
	      "  if (_instance == 0) {\n"
	      "    _name = UmlCom::read_string();\n"
	      "    _class = (UmlClass *) UmlBaseItem::read_();\n"
	      "  }\n",
	      FALSE, 0, 0);
  op->set_CppVisibility(PrivateVisibility);
  op->set_java("${type}", "",
	       "  _instance = (UmlClassInstance) UmlBaseItem.read_();\n"
	       "  if (_instance == null) {\n"
	       "    _name = UmlCom.read_string();\n"
	       "    _class = (UmlClass) UmlBaseItem.read_();\n"
	       "  }\n",
	       FALSE);
  base_instref->add_friend("UmlBaseSequenceDiagramDefinition");
  base_instref->add_friend("UmlBaseCollaborationDiagramDefinition");
  UmlClass::get("UmlBaseItem", 0)->add_friend("UmlBaseClassInstanceReference");
  include_umlcom(base_instref);

  UmlCom::set_user_id(uid);
  
  UmlRelation * rel = UmlBaseRelation::create(aGeneralisation, user_instref, base_instref);
  
  rel->set_CppDecl("${type}");
  rel->set_JavaDecl("${type}");
  
  return user_instref;
}

UmlClass * add_message(UmlClassView * base_class_view, UmlClassView * user_class_view,
		       UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		       UmlClass * user_instanceref)
{
  UmlClass * user_message = 
    UmlClass::made(user_class_view, user_depl_view, "UmlMessage", FALSE);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  UmlClass * base_message = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseMessage", FALSE);
  
  base_message->set_Description(" this class manages messages indenpendently of the diagram");
  user_message->set_Description(" this class manages messages indenpendently of the diagram,\n"
				" you can modify it");

  UmlOperation * op;
  
  op = base_message->add_op("from", PublicVisibility, user_instanceref);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the instance sending the message");
  op->set_cpp("${type} *", "", "  return _from;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _from;\n", TRUE);
  
  op = base_message->add_op("to", PublicVisibility, user_instanceref);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the instance receiving the message");
  op->set_cpp("${type} *", "", "  return _to;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _to;\n", TRUE);
  
  op = base_message->add_op("operation", PublicVisibility, UmlClass::get("UmlOperation", 0));
  op->set_isCppConst(TRUE);
  op->set_Description(" return the operation or 0/null,\n"
		      " exclusive with form()");
  op->set_cpp("${type} *", "", "  return _operation;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _operation;\n", TRUE);
  
  op = base_message->add_op("form", PublicVisibility, "string");
  op->set_isCppConst(TRUE);
  op->set_Description(" return message as a string, may be empty/null,\n"
		      " exclusive with operation()");
  op->set_cpp("const ${type} &", "", "  return _form;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _form;\n", TRUE);
  
  base_message->add_relation(aDirectionalAssociation, "_from", PrivateVisibility,
			     user_instanceref, 0, 0);
  base_message->add_relation(aDirectionalAssociation, "_to", PrivateVisibility,
			     user_instanceref, 0, 0);
  base_message->add_relation(aDirectionalAssociation, "_operation", PrivateVisibility,
			     UmlClass::get("UmlOperation", 0), 0, 0);
  base_message->add_attribute("_form", PrivateVisibility, "string", 0, 0);
  
  op = base_message->add_op("read_", PublicVisibility, "void");
  op->set_Description(" internal, don't call it");
  op->add_param(0, InputDirection, "instances", user_instanceref);
  op->set_cpp("${type}", "const QPtrDict<${t0}> & ${p0}",
	      "  _operation = (UmlOperation *) UmlBaseItem::read_();\n"
	      "  if (_operation == 0) _form = UmlCom::read_string();\n"
	      "  _from = instances[(void *) UmlCom::read_unsigned()];\n"
	      "  _to = instances[(void *) UmlCom::read_unsigned()];\n"
	      , FALSE, 0, 0);
  op->set_CppVisibility(PrivateVisibility);
  op->set_java("${type}", "Hashtable ${p0}",
	       "  _operation = (UmlOperation) UmlBaseItem.read_();\n"
	       "  if (_operation == null) _form = UmlCom.read_string();\n"
	       "  _from = (UmlClassInstanceReference) instances.get(new Integer(UmlCom.read_unsigned()));\n"
	       "  _to = (UmlClassInstanceReference) instances.get(new Integer(UmlCom.read_unsigned()));\n"
	       , FALSE);
  base_message->add_friend("UmlBaseSequenceMessage");
  base_message->add_friend("UmlBaseCollaborationMessage");
  UmlClass::get("UmlBaseItem", 0)->add_friend("UmlBaseMessage");
  include_umlcom(base_message);
  base_message->associatedArtifact()->add_import("import java.util.*;\n");

  UmlCom::set_user_id(uid);
  
  UmlRelation * rel = UmlBaseRelation::create(aGeneralisation, user_message, base_message);
  
  rel->set_CppDecl("${type}");
  rel->set_JavaDecl("${type}");
  
  return user_message;
}

UmlClass * add_msg_kind(UmlClassView * base_class_view, UmlDeploymentView * base_depl_view)
{
  // already root
  UmlClass * msgkind = 
    UmlClass::made(base_class_view, base_depl_view, "aMessageKind", FALSE);
  
  msgkind->set_Description(" indicate the kind of a message");
  
  msgkind->set_Stereotype("enum_pattern");
  msgkind->set_CppDecl(CppSettings::enumDecl());
  msgkind->set_JavaDecl(JavaSettings::enumPatternDecl());
  msgkind->add_enum_item("aSynchronousCall");
  msgkind->add_enum_item("anAsynchronousCall");
  msgkind->add_enum_item("anExplicitReturn");
  msgkind->add_enum_item("anImplicitReturn")
    ->set_Description("  message added to indicate the end of a duration bar");
  
  return msgkind;
}

UmlClass * add_seqmessage(UmlClassView * base_class_view, UmlClassView * user_class_view,
			  UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
			  UmlClass * user_message, UmlClass * user_instances,
			  UmlClass * user_fragment, UmlClass * user_fragment_compart)
{
  UmlClass * user_seqmessage = 
    UmlClass::made(user_class_view, user_depl_view, "UmlSequenceMessage", FALSE);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  UmlClass * msgkind = add_msg_kind(base_class_view, base_depl_view);
  UmlClass * base_seqmessage = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseSequenceMessage", FALSE);
  
  base_seqmessage->set_Description(" this class manages messages in a sequence diagram");
  user_seqmessage->set_Description(" this class manages messages in a sequence diagram,\n"
				   " you can modify it");

  QCString s = base_seqmessage->javaDecl();
  
  s.replace(s.find("${implements}"), 13, " implements java.lang.Comparable");
  base_seqmessage->set_JavaDecl(s);
  
  UmlOperation * op;
  
  op = base_seqmessage->add_op("kind", PublicVisibility, msgkind);
  op->set_isCppConst(TRUE);
  op->set_Description(" return kind of the message");
  op->set_cpp("${type}", "", "  return _kind;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _kind;\n", TRUE);
  
  op = base_seqmessage->add_op("fragment", PublicVisibility, user_fragment_compart);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the fragment compartment containing the message");
  op->set_cpp("${type} *", "", "  return _fragment;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _fragment;\n", TRUE);
  
  op = base_seqmessage->add_op("sentAt", PublicVisibility, "int");
  op->set_isCppConst(TRUE);
  op->set_Description(" return when the message is sent (arbitrary unit)");
  op->set_cpp("${type}", "", "  return _send_at;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _send_at;\n", TRUE);
  
  op = base_seqmessage->add_op("receivedAt", PublicVisibility, "int");
  op->set_isCppConst(TRUE);
  op->set_Description(" return when the message is received (arbitrary unit)");
  op->set_cpp("${type}", "", "  return _received_at;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _received_at;\n", TRUE);
  
  op = base_seqmessage->add_op("arguments", PublicVisibility, "string");
  op->set_isCppConst(TRUE);
  op->set_Description(" return the arguments of the operation, may be empty");
  op->set_cpp("const ${type} &", "", "  return _args;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _args;\n", TRUE);
  
  op = base_seqmessage->add_op("compareTo", PublicVisibility, "int");
  op->set_isJavaFinal(TRUE);
  op->add_param(0, InputDirection, "other", "Object");
  op->set_CppDecl("");
  op->set_CppDef("");
  op->set_java("${type}", "${t0} ${p0}",
	       "  return _send_at - ((UmlBaseSequenceMessage) other)._send_at;\n"
	       , TRUE);
  
  base_seqmessage->add_relation(aDirectionalAggregationByValue, "_kind", PrivateVisibility,
				msgkind, 0, 0);
  base_seqmessage->add_relation(aDirectionalAssociation, "_fragment", PrivateVisibility,
				user_fragment_compart, 0, 0);
  base_seqmessage->add_attribute("_x", PrivateVisibility, "int", 0, 0);
  base_seqmessage->add_attribute("_send_at", PrivateVisibility, "int", 0, 0);
  base_seqmessage->add_attribute("_received_at", PrivateVisibility, "int", 0, 0);
  base_seqmessage->add_attribute("_args", PrivateVisibility, "string", 0, 0);
  
  op = base_seqmessage->add_op("read_", PublicVisibility, "void");
  op->set_Description(" internal, don't call it");
  op->add_param(0, InputDirection, "instances", user_instances);
  op->add_param(1, InputDirection, "fragments", user_fragment);
  op->set_cpp("${type}", "const QPtrDict<${t0}> & ${p0}, const QVector<${t1}> & ${p1}",
	      "  UmlBaseMessage::read_(instances);\n"
	      "  _kind = (aMessageKind) UmlCom::read_char();\n"
	      "  _args = UmlCom::read_string();\n"
	      "  _x = (int) UmlCom::read_unsigned();\n"
	      "  _send_at = (int) UmlCom::read_unsigned();\n"
	      "  _received_at = (int) UmlCom::read_unsigned();\n"
	      "  _fragment = UmlBaseFragment::get_container_(_x, _send_at, 1, 1, fragments);\n"
	      , FALSE, 0, 0);
  op->set_CppVisibility(PrivateVisibility);
  op->set_java("${type}", "Hashtable ${p0}, ${t1}[] ${p1}", 
	       "  super.read_(instances);\n"
	       "  _kind = aMessageKind.fromInt((int) UmlCom.read_char());\n"
	       "  _args = UmlCom.read_string();\n"
	       "  _x = UmlCom.read_unsigned();\n"
	       "  _send_at = UmlCom.read_unsigned();\n"
	       "  _received_at = UmlCom.read_unsigned();\n"
	       "  _fragment = UmlBaseFragment.get_container_(_x, _send_at, 1, 1, fragments);\n"
	       , FALSE);
  base_seqmessage->add_friend("UmlBaseSequenceDiagramDefinition");
  include_umlcom(base_seqmessage);
  base_seqmessage->associatedArtifact()->add_import("import java.util.*;\n");

  UmlRelation * rel;
  
  rel = UmlBaseRelation::create(aGeneralisation, base_seqmessage, user_message);
  rel->set_CppDecl("${type}");
  rel->set_JavaDecl("${type}");
  
  UmlCom::set_user_id(uid);
  
  rel = UmlBaseRelation::create(aGeneralisation, user_seqmessage, base_seqmessage);
  rel->set_CppDecl("${type}");
  rel->set_JavaDecl("${type}");
  
  return user_seqmessage;
}

void add_seqdiagdef(UmlClassView * base_class_view, UmlClassView * user_class_view,
		    UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		    UmlClass * user_instref, UmlClass * user_seqmsg, UmlClass * user_fragment)
{
  UmlClass * user_ddef = 
    UmlClass::made(user_class_view, user_depl_view, "UmlSequenceDiagramDefinition", FALSE);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  UmlClass * base_ddef = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseSequenceDiagramDefinition", FALSE);
  
  base_ddef->set_Description(" this class manages sequence diagram definition");
  user_ddef->set_Description(" this class manages sequence diagram definition,\n"
			     " you can modify it");

  UmlOperation * op;
  
  op = base_ddef->add_op("instances", PublicVisibility, user_instref);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the instances present in the diagram");
  op->set_cpp("const QVector<${type}> &", "", "  return _instances;\n", TRUE, 0, 0);
  op->set_java("${type}[]", "", "  return _instances;\n", TRUE);

  op = base_ddef->add_op("messages", PublicVisibility, user_seqmsg);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the messages present in the diagram,\n"
		      " ordonned following the sending time");
  op->set_cpp("const QVector<${type}> &", "", "  return _messages;\n", TRUE, 0, 0);
  op->set_java("${type}[]", "", "  return _messages;\n", TRUE);

  op = base_ddef->add_op("fragments", PublicVisibility, user_fragment);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the fragments present in the diagram");
  op->set_cpp("const QVector<${type}> &", "", "  return _fragments;\n", TRUE, 0, 0);
  op->set_java("${type}[]", "", "  return _fragments;\n", TRUE);

  base_ddef->add_vect_assoc("_instances", PrivateVisibility, user_instref, 0, 0);
  base_ddef->add_vect_assoc("_messages", PrivateVisibility, user_seqmsg, 0, 0);
  base_ddef->add_vect_assoc("_fragments", PrivateVisibility, user_fragment, 0, 0);

  UmlExtraClassMember::create(base_ddef, "qsort")
    ->set_CppDef("static void qsort(UmlSequenceMessage ** v, int low, int high)\n"
		 "{\n"
		 "  if (low < high) {\n"
		 "    int lo = low;\n"
		 "    int hi = high + 1;\n"
		 "    int e = v[low]->sentAt();\n"
		 "    \n"
		 "    for (;;) {\n"
		 "      while ((++lo < hi) && (v[lo]->sentAt() <= e))\n"
		 "	;\n"
		 "      while (v[--hi]->sentAt() > e);\n"
		 "	;\n"
		 "      \n"
		 "      if (lo < hi) {\n"
		 "	UmlSequenceMessage * x = v[lo];\n"
		 "	\n"
		 "	v[lo] = v[hi];\n"
		 "	v[hi] = x;\n"
		 "      }\n"
		 "      else\n"
		 "	break;\n"
		 "    }\n"
		 "    \n"
		 "    UmlSequenceMessage * x = v[low];\n"
		 "	\n"
		 "    v[low] = v[hi];\n"
		 "    v[hi] = x;\n"
		 "    \n"
		 "    qsort(v, low, hi - 1);\n"
		 "    qsort(v, hi + 1, high);\n"
		 "  }\n"
		 "}\n");

  op = base_ddef->add_op("read_", PublicVisibility, "void");
  op->set_Description(" internal, don't call it");
  op->set_cpp("${type}", "",
	      "  QPtrDict<UmlClassInstanceReference> instances;\n"
	      "  unsigned n;\n"
	      "  unsigned rank;\n"
	      "\n"
	      "  n = UmlCom::read_unsigned();\n"
	      "  _fragments.resize(n);\n"
	      "  for (rank = 0; rank != n; rank += 1) {\n"
	      "    UmlFragment * f = new UmlFragment();\n"
	      "\n"
	      "    _fragments.insert(rank, f);\n"
	      "    f->read_();\n"
	      "  }\n"
	      "  _fragments.setAutoDelete(TRUE);\n"
	      "  UmlBaseFragment::compute_container_(_fragments);\n"
	      "\n"
	      "  n = UmlCom::read_unsigned();\n"
	      "  _instances.resize(n);\n"
	      "  for (rank = 0; rank != n; rank += 1) {\n"
	      "    UmlClassInstanceReference * i = new UmlClassInstanceReference();\n"
	      "\n"
	      "    _instances.insert(rank, i);\n"
	      "    instances.insert((void *) UmlCom::read_unsigned(), i);\n"
	      "    i->read_();\n"
	      "  }\n"
	      "  _instances.setAutoDelete(TRUE);\n"
	      "\n"
	      "  n = UmlCom::read_unsigned();\n"
	      "  _messages.resize(n);\n"
	      "  if (n != 0) {\n"
	      "    for (rank = 0; rank != n; rank += 1) {\n"
	      "      UmlSequenceMessage * m = new UmlSequenceMessage();\n"
	      "\n"
	      "      _messages.insert(rank, m);\n"
	      "      m->read_(instances, _fragments);\n"
	      "    }\n"
	      "    ::qsort(_messages.data(), 0, n - 1);\n"
	      "    _messages.setAutoDelete(TRUE);\n"
	      "  }\n"
	      "\n"
	      "  while (UmlCom::read_bool()) {\n"
	      "    QCString s = UmlCom::read_string();\n"
	      "    int x = (int) UmlCom::read_unsigned();\n"
	      "    int y = (int) UmlCom::read_unsigned();\n"
	      "    int w = (int) UmlCom::read_unsigned();\n"
	      "    int h = (int) UmlCom::read_unsigned();\n"
	      "    UmlFragmentCompartment * cp = UmlBaseFragment::get_container_(x, y, w, h, _fragments);\n"
	      "\n"
	      "    if (cp != 0) cp->add_text_(s);\n"
	      "  }\n"
	      , FALSE, 0, 0);
  op->set_CppVisibility(PrivateVisibility);
  op->set_java("${type}", "",
	       "  Hashtable instances;\n"
	       "  int n;\n"
	       "  int rank;\n"
	       "\n"
	       "  n = UmlCom.read_unsigned();\n"
	       "  _fragments = new UmlFragment[n];\n"
	       "  for (rank = 0; rank != n; rank += 1) {\n"
	       "    UmlFragment f = new UmlFragment();\n"
	       "\n"
	       "    _fragments[rank] = f;\n"
	       "    f.read_();\n"
	       "  }\n"
	      "  UmlBaseFragment.compute_container_(_fragments);\n"
	       "\n"
	       "  n = UmlCom.read_unsigned();\n"
	       "  _instances = new UmlClassInstanceReference[n];\n"
	       "  instances = new Hashtable((n == 0) ? 1 : n);\n"
	       "  for (rank = 0; rank != n; rank += 1) {\n"
	       "    UmlClassInstanceReference i = new UmlClassInstanceReference();\n"
	       "\n"
	       "    _instances[rank] = i;\n"
	       "    instances.put(new Integer(UmlCom.read_unsigned()), i);\n"
	       "    i.read_();\n"
	       "  }\n"
	       "\n"
	       "  n = UmlCom.read_unsigned();\n"
	       "  _messages = new UmlSequenceMessage[n];\n"
	       "  for (rank = 0; rank != n; rank += 1) {\n"
	       "    UmlSequenceMessage m = new UmlSequenceMessage();\n"
	       "\n"
	       "    _messages[rank] = m;\n"
	       "    m.read_(instances, _fragments);\n"
	       "  }\n"
	       "  Arrays.sort(_messages);\n"
	       "\n"
	       "  while (UmlCom.read_bool()) {\n"
	       "    String s = UmlCom.read_string();\n"
	       "    int x = (int) UmlCom.read_unsigned();\n"
	       "    int y = (int) UmlCom.read_unsigned();\n"
	       "    int w = (int) UmlCom.read_unsigned();\n"
	       "    int h = (int) UmlCom.read_unsigned();\n"
	      "    UmlFragmentCompartment cp = UmlBaseFragment.get_container_(x, y, w, h, _fragments);\n"
	      "\n"
	      "    if (cp != null) cp.add_text_(s);\n"
	       "  }\n"
	       , FALSE);

  UmlClass * d = UmlClass::get("UmlBaseSequenceDiagram", 0);
  
  add_ddef(d, user_ddef);
  base_ddef->add_destr(PublicVisibility, "  to avoid compiler warning, don't call it");
  base_ddef->add_friend("UmlBaseSequenceDiagram");
  UmlClass::get("UmlBaseItem", 0)->add_friend("UmlBaseSequenceDiagramDefinition");
  include_umlcom(base_ddef);
  include_umlcom(d);
  base_ddef->associatedArtifact()->add_includes("// to avoid compiler warning\n"
						"#include \"UmlFragment.h\"\n"
						"#include \"UmlFragmentCompartment.h\"\n"
						"#include \"UmlClassInstanceReference.h\"\n"
						"#include \"UmlSequenceMessage.h\"\n"
						"\n");
  base_ddef->associatedArtifact()->add_import("import java.util.*;\n");

  UmlCom::set_user_id(uid);
  
  UmlRelation * rel = UmlBaseRelation::create(aGeneralisation, user_ddef, base_ddef);
  
  rel->set_CppDecl("${type}");
  rel->set_JavaDecl("${type}");
}

UmlClass * add_colmessage(UmlClassView * base_class_view, UmlClassView * user_class_view,
			  UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
			  UmlClass * user_message, UmlClass * user_instanceref)
{
  UmlClass * user_colmessage = 
    UmlClass::made(user_class_view, user_depl_view, "UmlCollaborationMessage", FALSE);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  UmlClass * base_colmessage = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseCollaborationMessage", FALSE);
  
  base_colmessage->set_Description(" this class manages messages in a collaboration diagram");
  user_colmessage->set_Description(" this class manages messages in a collaboration diagram,\n"
				   " you can modify it");

  UmlOperation * op;
  
  op = base_colmessage->add_op("rank", PublicVisibility, "int");
  op->set_isCppConst(TRUE);
  op->set_Description(" return the global rank of the message");
  op->set_cpp("${type}", "", "  return _rank;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _rank;\n", TRUE);
  
  op = base_colmessage->add_op("hrank", PublicVisibility, "string");
  op->set_isCppConst(TRUE);
  op->set_Description(" return the hierarchical rank of the message");
  op->set_cpp("const ${type} &", "", "  return _hrank;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _hrank;\n", TRUE);
  
  base_colmessage->add_attribute("_rank", PrivateVisibility, "int", 0, 0);
  base_colmessage->add_attribute("_hrank", PrivateVisibility, "string", 0, 0);
  
  op = base_colmessage->add_op("read_", PublicVisibility, "void");
  op->set_Description(" internal, don't call it");
  op->add_param(0, InputDirection, "instances", user_instanceref);
  op->set_cpp("${type}", "const QPtrDict<${t0}> & ${p0}",
	      "  UmlBaseMessage::read_(instances);\n"
	      "  _rank = (int) UmlCom::read_unsigned();\n"
	      "  _hrank = UmlCom::read_string();\n",
	      FALSE, 0, 0);
  op->set_CppVisibility(PrivateVisibility);
  op->set_java("${type}", "Hashtable ${p0}",
	      "  super.read_(instances);\n"
	      "  _rank = UmlCom.read_unsigned();\n"
	      "  _hrank = UmlCom.read_string();\n",
	       FALSE);
  base_colmessage->add_friend("UmlBaseCollaborationDiagramDefinition");
  include_umlcom(base_colmessage);
  base_colmessage->associatedArtifact()->add_import("import java.util.*;\n");

  UmlRelation * rel;
  
  rel = UmlBaseRelation::create(aGeneralisation, base_colmessage, user_message);
  rel->set_CppDecl("${type}");
  rel->set_JavaDecl("${type}");
  
  UmlCom::set_user_id(uid);
  
  rel = UmlBaseRelation::create(aGeneralisation, user_colmessage, base_colmessage);
  rel->set_CppDecl("${type}");
  rel->set_JavaDecl("${type}");
  
  return user_colmessage;
}

UmlClass * add_coldiagdef(UmlClassView * base_class_view, UmlClassView * user_class_view,
			  UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
			  UmlClass * user_instref, UmlClass * user_colmsg)
{
  UmlClass * user_ddef = 
    UmlClass::made(user_class_view, user_depl_view, "UmlCollaborationDiagramDefinition", FALSE);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  UmlClass * base_ddef = 
    UmlClass::made(base_class_view, base_depl_view, "UmlBaseCollaborationDiagramDefinition", FALSE);
  
  base_ddef->set_Description(" this class manages collaboration diagram definition");
  user_ddef->set_Description(" this class manages collaboration diagram definition,\n"
			     " you can modify it");

  UmlOperation * op;
  
  op = base_ddef->add_op("instances", PublicVisibility, user_instref);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the instances present in the diagram");
  op->set_cpp("const QVector<${type}> &", "", "  return _instances;\n", TRUE, 0, 0);
  op->set_java("${type}[]", "", "  return _instances;\n", TRUE);

  op = base_ddef->add_op("messages", PublicVisibility, user_colmsg);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the messages present in the diagram,\n"
		      " ordonned following their rank");
  op->set_cpp("const QVector<${type}> &", "", "  return _messages;\n", TRUE, 0, 0);
  op->set_java("${type}[]", "", "  return _messages;\n", TRUE);

  base_ddef->add_vect_assoc("_instances", PrivateVisibility, user_instref, 0, 0);
  base_ddef->add_vect_assoc("_messages", PrivateVisibility, user_colmsg, 0, 0);

  op = base_ddef->add_op("read_", PublicVisibility, "void");
  op->set_Description(" internal, don't call it");
  op->set_cpp("${type}", "",
	      "  QPtrDict<UmlClassInstanceReference> instances;\n"
	      "  unsigned n;\n"
	      "  unsigned rank;\n"
	      "\n"
	      "  n = UmlCom::read_unsigned();\n"
	      "  _instances.resize(n);\n"
	      "  for (rank = 0; rank != n; rank += 1) {\n"
	      "    UmlClassInstanceReference * i = new UmlClassInstanceReference();\n"
	      "\n"
	      "    _instances.insert(rank, i);\n"
	      "    instances.insert((void *) UmlCom::read_unsigned(), i);\n"
	      "    i->read_();\n"
	      "  }\n"
	      "  _instances.setAutoDelete(TRUE);\n"
	      "\n"
	      "  n = UmlCom::read_unsigned();\n"
	      "  _messages.resize(n);\n"
	      "  for (rank = 0; rank != n; rank += 1) {\n"
	      "    UmlCollaborationMessage * m = new UmlCollaborationMessage();\n"
	      "\n"
	      "    _messages.insert(rank, m);\n"
	      "    m->read_(instances);\n"
	      "  }\n"
	      "  _messages.setAutoDelete(TRUE);\n"
	      , FALSE, 0, 0);
  op->set_CppVisibility(PrivateVisibility);
  op->set_java("${type}", "",
	       "  Hashtable instances;\n"
	       "  int n;\n"
	       "  int rank;\n"
	       "\n"
	       "  n = UmlCom.read_unsigned();\n"
	       "  _instances = new UmlClassInstanceReference[n];\n"
	       "  instances = new Hashtable((n == 0) ? 1 : n);\n"
	       "  for (rank = 0; rank != n; rank += 1) {\n"
	       "    UmlClassInstanceReference i = new UmlClassInstanceReference();\n"
	       "\n"
	       "    _instances[rank] = i;\n"
	       "    instances.put(new Integer(UmlCom.read_unsigned()), i);\n"
	       "    i.read_();\n"
	       "  }\n"
	       "\n"
	       "  n = UmlCom.read_unsigned();\n"
	       "  _messages = new UmlCollaborationMessage[n];\n"
	       "  for (rank = 0; rank != n; rank += 1) {\n"
	       "    UmlCollaborationMessage m = new UmlCollaborationMessage();\n"
	       "\n"
	       "    _messages[rank] = m;\n"
	       "    m.read_(instances);\n"
	       "  }\n"
	       , FALSE);

  UmlClass * d = UmlClass::get("UmlBaseCollaborationDiagram", 0);
  
  add_ddef(d, user_ddef);
  base_ddef->add_destr(PublicVisibility, "  to avoid compiler warning, don't call it");
  base_ddef->add_friend("UmlBaseCollaborationDiagram");
  UmlClass::get("UmlBaseItem", 0)->add_friend("UmlBaseCollaborationDiagramDefinition");
  include_umlcom(base_ddef);
  include_umlcom(d);
  base_ddef->associatedArtifact()->add_includes("// to avoid compiler warning\n"
						"#include \"UmlCollaborationMessage.h\"\n"
						"#include \"UmlClassInstanceReference.h\"\n"
						"\n");
  base_ddef->associatedArtifact()->add_import("import java.util.*;\n");  

  UmlCom::set_user_id(uid);
  
  UmlRelation * rel = UmlBaseRelation::create(aGeneralisation, user_ddef, base_ddef);
  
  rel->set_CppDecl("${type}");
  rel->set_JavaDecl("${type}");
  
  return user_ddef;
}

void add_diagram_def()
{
  UmlPackage * apiuser = (UmlPackage *) UmlClass::get("UmlItem", 0)->parent()->parent();
  UmlPackage * p_user = UmlPackage::create(apiuser, "Diagram");
  UmlClassView * cv_user = UmlClassView::create(p_user, "Diagram Class view");
  UmlDeploymentView * dv_user = UmlDeploymentView::create(p_user, "Diagram Deployment view");
  
  cv_user->set_AssociatedDeploymentView(dv_user);
  
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);

  UmlPackage * apibase = (UmlPackage *) UmlClass::get("UmlBaseItem", 0)->parent()->parent();
  UmlPackage * p_base = UmlPackage::create(apibase, "Diagram");
  UmlClassView * cv_base = UmlClassView::create(p_base, "Diagram Class view");
  UmlDeploymentView * dv_base = UmlDeploymentView::create(p_base, "Diagram Deployment view");
  
  cv_base->set_AssociatedDeploymentView(dv_base);

  UmlCom::set_user_id(uid);

  UmlClass * user_fragment;
  UmlClass * user_fragment_compart;
  
  add_fragment(cv_base, cv_user, dv_base, dv_user, user_fragment, user_fragment_compart);
  
  UmlClass * user_subject = add_subject(cv_base, cv_user, dv_base, dv_user);
  UmlClass * user_ucref = add_ucref(cv_base, cv_user, dv_base, dv_user,
				    user_fragment, user_fragment_compart, user_subject);
  UmlClass * user_ucassoc = add_ucassoc(cv_base, cv_user, dv_base, dv_user, user_ucref);
  UmlClass * user_instanceref = add_instanceref(cv_base, cv_user, dv_base, dv_user);
  UmlClass * user_message = add_message(cv_base, cv_user, dv_base, dv_user, user_instanceref);
  UmlClass * user_seqmessage = add_seqmessage(cv_base, cv_user, dv_base, dv_user,
					      user_message, user_instanceref,
					      user_fragment, user_fragment_compart);
  UmlClass * user_colmessage = add_colmessage(cv_base, cv_user, dv_base, dv_user,
					      user_message, user_instanceref);
  
  add_ucdiagdef(cv_base, cv_user, dv_base, dv_user,
		user_ucref, user_ucassoc, user_fragment, user_subject);
  add_seqdiagdef(cv_base, cv_user, dv_base, dv_user,
		 user_instanceref, user_seqmessage, user_fragment);
  add_coldiagdef(cv_base, cv_user, dv_base, dv_user,
		 user_instanceref, user_colmessage);
}

//
//
//

void upgrade_interaction()
{
  UmlCom::trace("<b>Upgrade interaction</b><br>");
    
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  //
  
  UmlClass * msgkind = UmlClass::get("aMessageKind", 0);
  
  msgkind->add_enum_item("aDestruction");
  msgkind->add_enum_item("anInteractionUse");
  
  //
  
  UmlClass * base_fragment = UmlClass::get("UmlBaseFragment", 0);
  UmlClass * diagram = UmlClass::get("UmlDiagram", 0);
  UmlOperation * op, * op2;
  
  base_fragment->associatedArtifact()->add_import("import java.util.*;\n");
  UmlClass::get("UmlBaseItem", 0)->add_friend("UmlBaseFragment");
  
  op = base_fragment->add_op("refer", PublicVisibility, diagram);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the diagram optionally referenced by the fragment,\n"
		      " generally associated to an interaction use");
  op->set_cpp("${type} *", "", "  return _refer;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _refer;\n", TRUE);
  op->moveAfter(base_fragment->get_operation("container"));
  
  UmlRelation * r = 
    base_fragment->add_relation(aDirectionalAssociation, "_refer",
				PrivateVisibility, diagram, 0, 0);
  
  r->moveAfter(base_fragment->get_attribute("_h"));
  
  op2 = base_fragment->add_op("arguments", PublicVisibility, "string");
  op2->set_isCppConst(TRUE);
  op2->set_Description(" return the form corresponding to the arguments\n"
		       " and return value of the interaction use");
  op2->set_cpp("${type}", "", "  return _arguments;\n", TRUE, 0, 0);
  op2->set_java("${type}", "", "  return _arguments;\n", TRUE);
  op2->moveAfter(op);
  
  UmlAttribute * a;
  
  a = base_fragment->add_attribute("_arguments", PrivateVisibility, "string",
				   0, 0);
  
  a->moveAfter(r);
  
  UmlClass * instref = UmlClass::get("UmlClassInstanceReference", 0);
  QString s;
  int index;
  
  op = base_fragment->add_op("covered", PublicVisibility, instref);
  op->set_isCppConst(TRUE);
  op->set_Description(" return the the list of covered instances (life lines)");
  op->set_cpp("const QVector<${type}> &", "", "  return _covered;\n", TRUE, 0, 0);
  op->set_java("${type}[]", "", "  return _covered.clone();\n", TRUE);
  op->moveAfter(op2);
  
  r = base_fragment->add_relation(aDirectionalAssociation, "_covered",
				  PrivateVisibility, instref, 0, 0);
  r->set_Stereotype("vector");
  r->set_Multiplicity("*");
  r->set_CppDecl(CppSettings::relationDecl(TRUE, "*"));
  s = r->javaDecl();
  index = s.find("${type} ${name}");
  if (index != -1) {
    s.insert(index + 7,"[]");
    r->set_JavaDecl(s);
  }
  r->moveAfter(a);

  op = base_fragment->get_operation("read_");
  s = op->cppBody();
  index = s.find("fc->init(this, rank, UmlCom::read_unsigned());");
  if (index != -1)
    s.replace(index, 46, "fc->read_(this, rank);");
  s += "  _refer = (UmlDiagram *) UmlBaseItem::read_();\n"
    "  _arguments = UmlCom::read_string();\n";
  op->set_CppBody(s);
  s = op->javaBody();
  index = s.find("fc.init(this, rank, UmlCom.read_unsigned());");
  if (index != -1)
    s.replace(index, 44, "fc.read_(this, rank);");
  s += "  _refer = (UmlDiagram) UmlBaseItem.read_();\n"
    "  _arguments = UmlCom.read_string();\n";
  op->set_JavaBody(s);
  
  op2 = base_fragment->add_op("vcenter_", PublicVisibility, "int");
  op2->set_Description("internal, do NOT use it\n");
  op2->set_isCppConst(TRUE);
  op2->add_param(0, InputDirection, "rank", "int");
  op2->set_cpp("${type}", "${t0} ${p0}",
	       "  int t = (rank == 0) ? _y : _compartments[rank - 1]->b();\n"
	       "\n"
	       "  return (t + _compartments[rank]->b())/2;\n",
	       FALSE, 0, 0);
  op2->set_java("${type}", "${t0} ${p0}",
	       "  int t = (rank == 0) ? _y : _compartments[rank - 1].b();\n"
	       "\n"
	       "  return (t + _compartments[rank].b())/2;\n",
		FALSE);
  op2->moveAfter(op);
  
  op = base_fragment->add_op("read_covered_", PublicVisibility, "void");
  op->set_Description("internal, do NOT use it\n");
  op->add_param(0, InputDirection, "instances", UmlClass::get("UmlClassInstanceReference", 0));
  op->set_cpp("${type}", "QPtrDict<${t0}> & ${p0}",
	      "  unsigned n = UmlCom::read_unsigned();\n"
	      "  unsigned rank;\n"
	      "\n"
	      "  _covered.resize(n);\n"
	      "  for (rank = 0; rank != n; rank += 1)\n"
	      "    _covered.insert(rank, instances[(void *) UmlCom::read_unsigned()]);\n"
	      , FALSE, 0, 0);
  op->set_java("${type}", "Hashtable ${p0}",
	       "  int n = UmlCom.read_unsigned();\n"
	       "  int rank;\n"
	       "\n"
	       "  _covered = new UmlClassInstanceReference[n];\n"
	       "  for (rank = 0; rank != n; rank += 1)\n"
	       "    _covered[rank] = (UmlClassInstanceReference) instances.get(new Integer(UmlCom.read_unsigned()));\n"
	       , FALSE);
  op->moveAfter(op2);
  
  //
  
  UmlClass * base_fragment_compart = UmlClass::get("UmlBaseFragmentCompartment", 0);
  
  include_umlcom(base_fragment_compart);
  
  op = base_fragment_compart->add_op("startContinuation", PublicVisibility, "string");
  op->set_isCppConst(TRUE);
  op->set_Description(" return the continuation ('label' case), or an empty string/null");
  op->set_cpp("${type}", "", "  return _start_continuation;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _start_continuation;\n", TRUE);
  op->moveAfter(base_fragment_compart->get_operation("contained"));
  
  a = base_fragment_compart->add_attribute("_start_continuation", PrivateVisibility, "string",
					    0, 0);
  a->moveAfter(base_fragment_compart->get_attribute("_y"));
  
  op2 = base_fragment_compart->add_op("endContinuation", PublicVisibility, "string");
  op2->set_isCppConst(TRUE);
  op2->set_Description(" return the continuation ('goto' case), or an empty string/null");
  op2->set_cpp("${type}", "", "  return _end_continuation;\n", TRUE, 0, 0);
  op2->set_java("${type}", "", "  return _end_continuation;\n", TRUE);
  op2->moveAfter(op);
  
  base_fragment_compart->add_attribute("_end_continuation", PrivateVisibility, "string",
				       0, 0)->moveAfter(a);

  op = base_fragment_compart->get_operation("init");
  op->set_Name("read_");
  op->removeParameter(2);
  op->set_cpp("${type}", "${t0} * ${p0}, ${t1} ${p1}",
	      "  _fragment = (UmlFragment *) fragment;\n"
	      "  _rank = rank;\n"
	      "  _y = UmlCom::read_unsigned();\n"
	      , FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	       "  _fragment = (UmlFragment) fragment;\n"
	       "  _rank = rank;\n"
	       "  _y = UmlCom.read_unsigned();\n"
	       "  _contained = new UmlFragment[0];\n"
	       "  _texts = new String[0];\n"
	       , FALSE);
  
  op = base_fragment_compart->add_op("add_cont_", PublicVisibility, "void");
  op->set_Description("internal, do NOT use it\n");
  op->add_param(0, InputDirection, "s", "string");
  op->add_param(1, InputDirection, "cy", "int");
  op->set_cpp("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  if (cy < _fragment->vcenter_(_rank))\n"
	      "    _start_continuation = s;\n"
	      "  else\n"
	      "    _end_continuation = s;\n"
	      , FALSE, 0, 0);
  op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
	      "  if (cy < _fragment.vcenter_(_rank))\n"
	      "    _start_continuation = s;\n"
	      "  else\n"
	      "    _end_continuation = s;\n"
	       , FALSE);
  op->moveAfter(base_fragment_compart->get_operation("add_text_"));

  //
  
  UmlClass * base_seq_def = UmlClass::get("UmlBaseSequenceDiagramDefinition", 0);
  
  op = base_seq_def->get_operation("read_");
  s = op->cppBody();
  index = s.find("  while (UmlCom::read_bool()) {");
  if (index != -1)
    s.insert(index,
	     "  n = _fragments.count();\n"
	     "  for (rank = 0; rank != n; rank += 1)\n"
	     "    _fragments.at(rank)->read_covered_(instances);\n\n"
	     "\n");
  s += "\n\
  while (UmlCom::read_bool()) {\n\
    QCString s = UmlCom::read_string();\n\
    int x = (int) UmlCom::read_unsigned();\n\
    int y = (int) UmlCom::read_unsigned();\n\
    int w = (int) UmlCom::read_unsigned();\n\
    int h = (int) UmlCom::read_unsigned();\n\
    UmlFragmentCompartment * cp = UmlBaseFragment::get_container_(x, y, w, h, _fragments);\n\
\n\
    if (cp != 0) cp->add_cont_(s, y + h/2);\n\
  }\n";
  op->set_CppBody(s);
  s = op->javaBody();
  index = s.find("  while (UmlCom.read_bool()) {");
  if (index != -1)
    s.insert(index,
	     "  n = _fragments.length;\n"
	     "  for (rank = 0; rank != n; rank += 1)\n"
	     "    _fragments[rank].read_covered_(instances);\n\n");
  s += "\n\
  while (UmlCom.read_bool()) {\n\
    String s = UmlCom.read_string();\n\
    int x = (int) UmlCom.read_unsigned();\n\
    int y = (int) UmlCom.read_unsigned();\n\
    int w = (int) UmlCom.read_unsigned();\n\
    int h = (int) UmlCom.read_unsigned();\n\
    UmlFragmentCompartment cp = UmlBaseFragment.get_container_(x, y, w, h, _fragments);\n\
\n\
    if (cp != null) cp.add_cont_(s, y + h/2);\n\
  }\n";
  op->set_JavaBody(s);
  
  //
  
  UmlCom::set_user_id(uid);
}

//
//
//

void add_stereotype_on_seq_msg(UmlClass * baseseqmsg)
{
  UmlCom::trace("<b>Add stereotype on sequence messages</b><br>");
    
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  //
  
  UmlOperation * op;
  
  op = baseseqmsg->add_op("stereotype", PublicVisibility, "string");
  
  op->set_isCppConst(TRUE);
  op->set_Description(" return the stereotype of the message");
  op->set_cpp("${type}", "", "  return _stereotype;\n", TRUE, 0, 0);
  op->set_java("${type}", "", "  return _stereotype;\n", TRUE);
  op->moveAfter(baseseqmsg->get_operation("kind"));
  
  //
  
  baseseqmsg->add_attribute("_stereotype", PrivateVisibility, "string",
			    0, 0)
    ->moveAfter(baseseqmsg->get_attribute("_args"));
  
  //
  
  QString s;
  int index;
  
  op = baseseqmsg->get_operation("read_");
  s = op->cppBody();
  index = s.find("_x = ");
  if (index != -1)
    s.insert(index, "_stereotype = UmlCom::read_string();\n  ");
  op->set_CppBody(s);
  s = op->javaBody();
  index = s.find("_x = ");
  if (index != -1)
    s.insert(index, "_stereotype = UmlCom.read_string();\n  ");
  op->set_JavaBody(s);
  
  //
  
  UmlCom::set_user_id(uid);
}

