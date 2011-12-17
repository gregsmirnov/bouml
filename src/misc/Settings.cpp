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

#include "Settings.h"
#include "SettingsDialog.h"
#include "ClassSettingsDialog.h"
#include "UmlCanvas.h"
#include "myio.h"
#include "translate.h"

bool DrawingSettings::just_changed;

DrawingSettings * DrawingSettings::instance() {
  static DrawingSettings * the;
  
  if (the == 0)
    the = new DrawingSettings;
  
  return the;
}

void DrawingSettings::modified() {
  UmlCanvas::update_global_settings();
  instance()->is_modified();
}

void DrawingSettings::is_modified() {
  just_changed = TRUE;  // to optimize 'draw all relations'
  emit changed();
  just_changed = FALSE;
}

bool DrawingSettings::just_modified()
{
  return just_changed;
}

//

#define check_default(att, default)		\
	if ((result.att != default) ||		\
	    ((result.att = att) != default))	\
	  done += 1

#define assign_default(att, default)		\
	if (att == default) att = from.att

//
  
ClassSettings::ClassSettings() {
  attribute_visibility = UmlDefaultVisibility;
  relation_visibility = UmlDefaultVisibility;
  operation_visibility = UmlDefaultVisibility;
}
 
void ClassSettings::save(QTextStream & st) const {
  bool nl;
  
  if (attribute_visibility != UmlDefaultVisibility) {
    nl_indent(st);
    nl = TRUE;
    st << "default_attribute_visibility " << stringify(attribute_visibility);
  }
  else
    nl = FALSE;
  
  if (relation_visibility != UmlDefaultVisibility) {
    if (nl)
      st << ' ';
    else {
      nl_indent(st);
      nl = TRUE;
    }
    st << "default_relation_visibility " << stringify(relation_visibility);
  }
  
  if (operation_visibility != UmlDefaultVisibility) {
    if (nl)
      st << ' ';
    else
      nl_indent(st);
    st << "default_operation_visibility " << stringify(operation_visibility);
  }
}

void ClassSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "default_attribute_visibility") ||
      !strcmp(k, "default_attribut_visibility")) {
    attribute_visibility = visibility(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "default_relation_visibility")) {
    relation_visibility = visibility(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "default_operation_visibility")) {
    operation_visibility = visibility(read_keyword(st));
    k = read_keyword(st);
  }
}

bool ClassSettings::edit(UmlCode who) {
  ClassSettingsDialog dialog(this, (who == UmlProject));
  
  return (dialog.exec() == QDialog::Accepted);
}

//

ClassDiagramSettings::ClassDiagramSettings() {
  hide_attributes = UmlDefaultState;
  hide_operations = UmlDefaultState;
  hide_getset_operations = UmlDefaultState;
  show_full_members_definition = UmlDefaultState;
  show_members_visibility = UmlDefaultState;
  show_members_stereotype = UmlDefaultState;
  show_members_context = DefaultShowContextMode;
  show_members_multiplicity = UmlDefaultState;
  show_members_initialization = UmlDefaultState;
  show_attribute_modifiers = UmlDefaultState;
  member_max_width = UmlDefaultMaxMemberWidth;
  show_parameter_dir = UmlDefaultState;
  show_parameter_name = UmlDefaultState;
  package_name_in_tab = UmlDefaultState;
  class_drawing_mode = DefaultClassDrawingMode;
  drawing_language = DefaultDrawingLanguage;
  show_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  draw_all_relations = UmlDefaultState;
  show_relation_modifiers = UmlDefaultState;
  show_relation_visibility = UmlDefaultState;
  show_infonote = UmlDefaultState;
  shadow = UmlDefaultState;
  show_stereotype_properties = UmlDefaultState;
}

void ClassDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "draw_all_relations " << stringify(draw_all_relations)
    << " hide_attributes " << stringify(hide_attributes)
      << " hide_operations " << stringify(hide_operations)
	<< " hide_getset_operations " << stringify(hide_getset_operations)
	  << " show_members_full_definition " << stringify(show_full_members_definition)
	    << " show_members_visibility " << stringify(show_members_visibility)
	      << " show_members_stereotype " << stringify(show_members_stereotype)
		<< " show_members_context " << stringify(show_members_context)
		  << " show_members_multiplicity " << stringify(show_members_multiplicity)
		    << " show_members_initialization " << stringify(show_members_initialization)
		      << " show_attribute_modifiers " << stringify(show_attribute_modifiers)
			<< " member_max_width " << ((int) member_max_width)
			  << " show_parameter_dir " << stringify(show_parameter_dir)
			    << " show_parameter_name " << stringify(show_parameter_name)
			      << " package_name_in_tab " << stringify(package_name_in_tab)
				<< " class_drawing_mode " << stringify(class_drawing_mode)
				  << " drawing_language " << stringify(drawing_language)
				    << " show_context_mode " << stringify(show_context_mode)
				      << " auto_label_position " << stringify(auto_label_position)
					<< " show_relation_modifiers " << stringify(show_relation_modifiers)
					  << " show_relation_visibility " << stringify(show_relation_visibility)
					    << " show_infonote " << stringify(show_infonote)
					      << " shadow " << stringify(shadow)
						<< " show_stereotype_properties " << stringify(show_stereotype_properties);
}

void ClassDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "hide_attributes") ||
      !strcmp(k, "hide_attributs")) {
    hide_attributes = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "hide_operations")) {
    hide_operations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "hide_getset_operations")) {
    hide_getset_operations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_full_members_definition")) {
    // old version
    show_full_members_definition = state(read_keyword(st));
    show_members_visibility = show_full_members_definition;
    show_members_stereotype = UmlNo;
    show_members_context = noContext;
    show_members_multiplicity = UmlNo;
    show_members_initialization = UmlNo;
    show_attribute_modifiers = UmlNo;
    member_max_width = UmlUnlimitedMemberWidth;
    show_parameter_dir = UmlYes;
    show_parameter_name = UmlYes;
    show_relation_modifiers = UmlNo;
    show_relation_visibility = UmlNo;
    k = read_keyword(st);
  }
  else {
    if (!strcmp(k, "show_members_full_definition")) {
      // new version
      show_full_members_definition = state(read_keyword(st));
      k = read_keyword(st);
    }
    if (!strcmp(k, "show_members_visibility")) {
      // new version
      show_members_visibility = state(read_keyword(st));
      k = read_keyword(st);
    }
    if (!strcmp(k, "show_members_stereotype")) {
      // new version
      show_members_stereotype = state(read_keyword(st));
      k = read_keyword(st);
    }
    if (!strcmp(k, "show_members_context")) {
      // new version
      show_members_context = context_mode(read_keyword(st));
      k = read_keyword(st);
    }
    if (!strcmp(k, "show_members_multiplicity")) {
      // new version
      show_members_multiplicity = state(read_keyword(st));
      k = read_keyword(st);
    }    
    if (!strcmp(k, "show_members_initialization")) {
      // new version
      show_members_initialization = state(read_keyword(st));
      k = read_keyword(st);
    }   
    if (!strcmp(k, "show_attribute_modifiers")) {
      // new version
      show_attribute_modifiers = state(read_keyword(st));
      k = read_keyword(st);
    }      
    if (!strcmp(k, "member_max_width")) {
      // new version
      member_max_width = (char) read_unsigned(st);
      // to manage erroneous value set directly in the file by user
      switch (member_max_width) {
      case UmlDefaultMaxMemberWidth:
      case UmlUnlimitedMemberWidth:
	break;
      default:
	if (member_max_width < MinMemberWidthValue)
	  member_max_width = MinMemberWidthValue;
	else if (member_max_width >= SupMemberWidthValue)
	  member_max_width = UmlUnlimitedMemberWidth;
      }
      k = read_keyword(st);
    }      
    if (!strcmp(k, "show_parameter_dir")) {
      // new version
      show_parameter_dir = state(read_keyword(st));
      k = read_keyword(st);
    }
    if (!strcmp(k, "show_parameter_name")) {
      // new version
      show_parameter_name = state(read_keyword(st));
      k = read_keyword(st);
    }
  }
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if ((!strcmp(k, "class_view_mode")) ||	// old
      (!strcmp(k, "class_drawing_mode"))) {
    class_drawing_mode = drawing_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "drawing_language")) {
    drawing_language = ::drawing_language(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context_mode")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "auto_label_position")) {
    auto_label_position = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_relation_modifiers")) {
    show_relation_modifiers = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_relation_visibility")) {
    show_relation_visibility = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_infonote")) {
    show_infonote = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}


bool ClassDiagramSettings::complete(ClassDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(draw_all_relations, UmlDefaultState);
  check_default(hide_attributes, UmlDefaultState);
  check_default(hide_operations, UmlDefaultState);
  check_default(hide_getset_operations, UmlDefaultState);
  check_default(show_full_members_definition, UmlDefaultState);
  check_default(show_members_visibility, UmlDefaultState);
  check_default(show_members_stereotype, UmlDefaultState);
  check_default(show_members_context, DefaultShowContextMode);
  check_default(show_members_multiplicity, UmlDefaultState);
  check_default(show_members_initialization, UmlDefaultState);
  check_default(show_attribute_modifiers, UmlDefaultState);
  check_default(member_max_width, UmlDefaultMaxMemberWidth);
  check_default(show_parameter_dir, UmlDefaultState);
  check_default(show_parameter_name, UmlDefaultState);
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(class_drawing_mode, DefaultClassDrawingMode);
  check_default(drawing_language, DefaultDrawingLanguage);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(auto_label_position, UmlDefaultState);
  check_default(show_relation_modifiers, UmlDefaultState);
  check_default(show_relation_visibility, UmlDefaultState);
  check_default(show_infonote, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  check_default(show_stereotype_properties, UmlDefaultState);
  
  return done == 24;
}

void ClassDiagramSettings::assign(const ClassDiagramSettings & from) {
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(hide_attributes, UmlDefaultState);
  assign_default(hide_operations, UmlDefaultState);
  assign_default(hide_getset_operations, UmlDefaultState);
  assign_default(show_full_members_definition, UmlDefaultState);
  assign_default(show_members_visibility, UmlDefaultState);
  assign_default(show_members_stereotype, UmlDefaultState);
  assign_default(show_members_context, DefaultShowContextMode);
  assign_default(show_members_multiplicity, UmlDefaultState);
  assign_default(show_members_initialization, UmlDefaultState);
  assign_default(show_attribute_modifiers, UmlDefaultState);
  assign_default(member_max_width, UmlDefaultMaxMemberWidth);
  assign_default(show_parameter_dir, UmlDefaultState);
  assign_default(show_parameter_name, UmlDefaultState);
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(class_drawing_mode, DefaultClassDrawingMode);
  assign_default(drawing_language, DefaultDrawingLanguage);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(auto_label_position, UmlDefaultState);
  assign_default(show_relation_modifiers, UmlDefaultState);
  assign_default(show_relation_visibility, UmlDefaultState);
  assign_default(show_infonote, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void ClassDiagramSettings::complete(StateSpecVector & a, UmlCode who) {
  int i = a.size();
  
  switch (who) {
  case UmlClass:
    // order known by ClassDiagramSettings::set
    a.resize(i + 18);
  
    a[i].set(TR("drawing language"), &drawing_language);
    a[i + 1].set(TR("drawing mode"), &class_drawing_mode);
    a[i + 2].set(TR("show context"), &show_context_mode);
    a[i + 3].set(TR("hide attributes"), &hide_attributes);
    a[i + 4].set(TR("hide operations"), &hide_operations);
    a[i + 5].set(TR("hide get/set operations"), &hide_getset_operations);
    a[i + 6].set(TR("show members full definition"),
		 &show_full_members_definition);
    a[i + 7].set(TR("show members visibility"),
		 &show_members_visibility);
    a[i + 8].set(TR("show members stereotype"),
		 &show_members_stereotype);
    a[i + 9].set(TR("show context in members definition"),
		 &show_members_context);
    a[i + 10].set(TR("show attributes multiplicity"),
		 &show_members_multiplicity);
    a[i + 11].set(TR("show attributes initialisation"),
		 &show_members_initialization);
    a[i + 12].set(TR("show attribute modifiers"),
		 &show_attribute_modifiers);
    a[i + 13].set(TR("show parameter direction"),
		  &show_parameter_dir);
    a[i + 14].set(TR("show parameter name"),
		  &show_parameter_name);
    a[i + 15].set(TR("members max width"),
		  &member_max_width);
    a[i + 16].set(TR("show information note"),
		  &show_infonote);
    a[i + 17].set(TR("show stereotype properties"),
		  &show_stereotype_properties);
    break;
  case UmlPackage:
    a.resize(i + 3);
  
    a[i].set(TR("name in tab"), &package_name_in_tab);
    a[i + 1].set(TR("show context"), &show_context_mode);
    a[i + 2].set(TR("show stereotype properties"), &show_stereotype_properties);
    break;
  case UmlClassDiagram:
    a.resize(i + 24);
  
    a[i].set(TR("drawing language"), &drawing_language);
    a[i + 1].set(TR("classes drawing mode"), &class_drawing_mode);
    a[i + 2].set(TR("hide classes attributes"), &hide_attributes);
    a[i + 3].set(TR("hide classes operations"), &hide_operations);
    a[i + 4].set(TR("hide classes get/set operations"), &hide_getset_operations);
    a[i + 5].set(TR("show classes members full definition"),
		 &show_full_members_definition);
    a[i + 6].set(TR("show members visibility"),
		 &show_members_visibility);
    a[i + 7].set(TR("show members stereotype"),
		 &show_members_stereotype);
    a[i + 8].set(TR("show context in members definition"),
		 &show_members_context);
    a[i + 9].set(TR("show attributes multiplicity"),
		 &show_members_multiplicity);
    a[i + 10].set(TR("show attributes initialization"),
		  &show_members_initialization);
    a[i + 11].set(TR("show attribute modifiers"),
		 &show_attribute_modifiers);
    a[i + 12].set(TR("show parameter direction"),
		  &show_parameter_dir);
    a[i + 13].set(TR("show parameter name"),
		  &show_parameter_name);
    a[i + 14].set(TR("members max width"),
		  &member_max_width);
    a[i + 15].set(TR("draw all relations"), &draw_all_relations);
    a[i + 16].set(TR("show relation modifiers"), &show_relation_modifiers);
    a[i + 17].set(TR("show relation visibility"), &show_relation_visibility);
    a[i + 18].set(TR("show packages name in tab"), &package_name_in_tab);
    a[i + 19].set(TR("show classes and packages context"), &show_context_mode);
    a[i + 20].set(TR("automatic labels position"), &auto_label_position);
    a[i + 21].set(TR("show information note"), &show_infonote);
    a[i + 22].set(TR("show stereotype properties"),
		  &show_stereotype_properties);
    a[i + 23].set(TR("draw shadow"), &shadow);
    break;
  default:
    {
      QString s = TR("class") + "#";
      
      a.resize(i + 24);
      
      a[i].set(s + TR("drawing language"), &drawing_language);
      a[i + 1].set(s + TR("classes drawing mode"), &class_drawing_mode);
      a[i + 2].set(s + TR("hide classes attributes"), &hide_attributes);
      a[i + 3].set(s + TR("hide classes operations"), &hide_operations);
      a[i + 4].set(s + TR("hide classes get/set operations"), &hide_getset_operations);
      a[i + 5].set(s + TR("show classes members full definition"),
		   &show_full_members_definition);
      a[i + 6].set(s + TR("show classes members visibility"),
		   &show_members_visibility);
      a[i + 7].set(s + TR("show classes members stereotype"),
		   &show_members_stereotype);
      a[i + 8].set(s + TR("show context in classes members definition"),
		   &show_members_context);
      a[i + 9].set(s + TR("show attribute multiplicity"),
		   &show_members_multiplicity);
      a[i + 10].set(s + TR("show attribute initialization"),
		    &show_members_initialization);
      a[i + 11].set(s + TR("show attribute modifiers"),
		    &show_attribute_modifiers);
      a[i + 12].set(s + TR("show operation parameter direction"),
		    &show_parameter_dir);
      a[i + 13].set(s + TR("show operation parameter name"),
		    &show_parameter_name);
      a[i + 14].set(s + TR("max members width"),
		    &member_max_width);
      a[i + 15].set(s + TR("draw all relations"), &draw_all_relations);
      a[i + 16].set(s + TR("show packages name in tab"), &package_name_in_tab);
      a[i + 17].set(s + TR("show relation modifiers"), &show_relation_modifiers);
      a[i + 18].set(s + TR("show relation visibility"), &show_relation_visibility);
      a[i + 19].set(s + TR("show classes and packages context"), &show_context_mode);
      a[i + 20].set(s + TR("automatic labels position"), &auto_label_position);
      a[i + 21].set(s + TR("show information note"), &show_infonote);
      a[i + 22].set(s + TR("show stereotype properties"),
		    &show_stereotype_properties);
      a[i + 23].set(s + TR("draw shadow"), &shadow);
    }
  }
}

// to update class canvas settings
void ClassDiagramSettings::set(StateSpecVector & a, int index) {
  if (!a[index].name.isEmpty())
    drawing_language = (DrawingLanguage) *((DrawingLanguage *) a[index].state);
  if (!a[index + 1].name.isEmpty())
    class_drawing_mode = (ClassDrawingMode) *((ClassDrawingMode *) a[index + 1].state);
  if (!a[index + 2].name.isEmpty())
    show_context_mode = (ShowContextMode) *((ShowContextMode *) a[index + 2].state);
  if (!a[index + 3].name.isEmpty())
    hide_attributes = (Uml3States) *((Uml3States *) a[index + 3].state);
  if (!a[index + 4].name.isEmpty())
    hide_operations = (Uml3States) *((Uml3States *) a[index + 4].state);
  if (!a[index + 5].name.isEmpty())
    hide_getset_operations = (Uml3States) *((Uml3States *) a[index + 5].state);
  if (!a[index + 6].name.isEmpty())
    show_full_members_definition = (Uml3States) *((Uml3States *) a[index + 6].state);
  if (!a[index + 7].name.isEmpty())
    show_members_visibility = (Uml3States) *((Uml3States *) a[index + 7].state);
  if (!a[index + 8].name.isEmpty())
    show_members_stereotype = (Uml3States) *((Uml3States *) a[index + 8].state);
  if (!a[index + 9].name.isEmpty())
    show_members_context = (ShowContextMode) *((ShowContextMode *) a[index + 9].state);
  if (!a[index + 10].name.isEmpty())
    show_members_multiplicity = (Uml3States) *((Uml3States *) a[index + 10].state);
  if (!a[index + 11].name.isEmpty())
    show_attribute_modifiers = (Uml3States) *((Uml3States *) a[index + 11].state);
  if (!a[index + 12].name.isEmpty())
    show_members_initialization = (Uml3States) *((Uml3States *) a[index + 12].state);
  if (!a[index + 13].name.isEmpty())
    show_parameter_dir = (Uml3States) *((Uml3States *) a[index + 13].state);
  if (!a[index + 14].name.isEmpty())
    show_parameter_name = (Uml3States) *((Uml3States *) a[index + 14].state);
  if (!a[index + 15].name.isEmpty())
    member_max_width = *((char *) a[index + 15].state);
  if (!a[index + 16].name.isEmpty())
    show_infonote = (Uml3States) *((Uml3States *) a[index + 16].state);
  if (!a[index + 17].name.isEmpty())
    show_stereotype_properties = (Uml3States) *((Uml3States *) a[index + 17].state);
}


//

SimpleClassDiagramSettings::SimpleClassDiagramSettings() {
  class_drawing_mode = DefaultClassDrawingMode;
  show_context_mode = DefaultShowContextMode;
  show_stereotype_properties = UmlDefaultState;
}

void SimpleClassDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "class_drawing_mode " << stringify(class_drawing_mode)
    << " show_context_mode " << stringify(show_context_mode)
      << " show_stereotype_properties " << stringify(show_stereotype_properties);
}

void SimpleClassDiagramSettings::read(char * & st, char * & k) {
  if ((!strcmp(k, "class_view_mode")) ||	// old
      (!strcmp(k, "class_drawing_mode"))) {
    class_drawing_mode = drawing_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context_mode")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}

void SimpleClassDiagramSettings::assign(const UseCaseDiagramSettings & from) {
  assign_default(class_drawing_mode, DefaultClassDrawingMode);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void SimpleClassDiagramSettings::complete(StateSpecVector & a) {
  int i = a.size();
  
  a.resize(i + 3);
  
  a[i].set(TR("drawing mode"), &class_drawing_mode);
  a[i + 1].set(TR("show context"), &show_context_mode);
  a[i + 2].set(TR("show stereotype properties"), &show_stereotype_properties);
}

// to update class canvas settings
void SimpleClassDiagramSettings::set(StateSpecVector & a, int index) {
  if (!a[index].name.isEmpty())
    class_drawing_mode = (ClassDrawingMode) *((ClassDrawingMode *) a[index].state);
  if (!a[index + 1].name.isEmpty())
    show_context_mode = (ShowContextMode) *((ShowContextMode *) a[index + 1].state);
  if (!a[index + 2].name.isEmpty())
    show_stereotype_properties = (Uml3States) *((Uml3States *) a[index + 2].state);
}


//

SequenceDiagramSettings::SequenceDiagramSettings() {
  show_full_operations_definition = UmlDefaultState;
  write_horizontally = UmlDefaultState;
  instances_drawing_mode = DefaultClassDrawingMode;
  drawing_language = DefaultDrawingLanguage;
  draw_all_relations = UmlDefaultState;
  shadow = UmlDefaultState;
  show_stereotype_properties = UmlDefaultState;
  show_class_context_mode = DefaultShowContextMode;
  show_msg_context_mode = DefaultShowContextMode;
}

void SequenceDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "show_full_operations_definition " << stringify(show_full_operations_definition)
    << " write_horizontally " << stringify(write_horizontally)
      << " class_drawing_mode " << stringify(instances_drawing_mode)
	<< " drawing_language " << stringify(drawing_language)
	  << " draw_all_relations " << stringify(draw_all_relations)
	    << " shadow " << stringify(shadow)
	      << " show_stereotype_properties " << stringify(show_stereotype_properties)
		<< " show_class_context_mode " << stringify(show_class_context_mode)
		  << " show_msg_context_mode " << stringify(show_msg_context_mode);
}

void SequenceDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "show_full_operations_definition")) {
    show_full_operations_definition = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "write_horizontally") ||
      !strcmp(k, "write_horizontaly")) {
    write_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "class_drawing_mode")) {
    instances_drawing_mode = drawing_mode(read_keyword(st));
    k = read_keyword(st);
  }  
  if (!strcmp(k, "drawing_language")) {
    drawing_language = ::drawing_language(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_class_context_mode")) {
    show_class_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_msg_context_mode")) {
    show_msg_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
}

bool SequenceDiagramSettings::complete(SequenceDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(show_full_operations_definition, UmlDefaultState);
  check_default(write_horizontally, UmlDefaultState);
  check_default(instances_drawing_mode, DefaultClassDrawingMode);  
  check_default(drawing_language, DefaultDrawingLanguage);
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  check_default(show_stereotype_properties, UmlDefaultState);
  check_default(show_class_context_mode, DefaultShowContextMode);
  check_default(show_msg_context_mode, DefaultShowContextMode);
  
  return done == 9;
}

void SequenceDiagramSettings::assign(const SequenceDiagramSettings & from) {
  assign_default(show_full_operations_definition, UmlDefaultState);
  assign_default(write_horizontally, UmlDefaultState);
  assign_default(instances_drawing_mode, DefaultClassDrawingMode);  
  assign_default(drawing_language, DefaultDrawingLanguage);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);
  assign_default(show_stereotype_properties, UmlDefaultState);
  assign_default(show_class_context_mode, DefaultShowContextMode);
  assign_default(show_msg_context_mode, DefaultShowContextMode);
}

void SequenceDiagramSettings::assign_msg(const SequenceDiagramSettings & from) {
  assign_default(show_full_operations_definition, UmlDefaultState);
  assign_default(drawing_language, DefaultDrawingLanguage);
  assign_default(show_msg_context_mode, DefaultShowContextMode);
}

void SequenceDiagramSettings::complete(StateSpecVector & a, bool local) {
  int i = a.size();
  
  a.resize(i + 9);

  if (local) {
    a[i].set(TR("drawing language"), &drawing_language);
    a[i + 1].set(TR("instances drawing mode"), &instances_drawing_mode);
    a[i + 2].set(TR("show operations full definition"), &show_full_operations_definition);
    a[i + 3].set(TR("write name:type horizontally"), &write_horizontally);
    a[i + 4].set(TR("show classes context"), &show_class_context_mode);
    a[i + 5].set(TR("show messages context"), &show_msg_context_mode);
    a[i + 6].set(TR("draw all relations"), &draw_all_relations);
    a[i + 7].set(TR("show stereotype properties"), &show_stereotype_properties);
    a[i + 8].set(TR("draw shadow"), &shadow);
  }
  else {
    QString s = TR("sequence") + "#";
    
    a[i].set(s + TR("drawing language"), &drawing_language);
    a[i + 1].set(s + TR("instances drawing mode"), &instances_drawing_mode);
    a[i + 2].set(s + TR("show operations full definition"), &show_full_operations_definition);
    a[i + 3].set(s + TR("write name:type horizontally"), &write_horizontally);
    a[i + 4].set(s + TR("show classes context"), &show_class_context_mode);
    a[i + 5].set(s + TR("show messages context"), &show_msg_context_mode);
    a[i + 6].set(s + TR("draw all relations"), &draw_all_relations);
    a[i + 7].set(s + TR("show stereotype properties"), &show_stereotype_properties);
    a[i + 8].set(s + TR("draw shadow"), &shadow);
  }
}

//

CollaborationDiagramSettings::CollaborationDiagramSettings() {
  show_full_operations_definition = UmlDefaultState;
  show_hierarchical_rank = UmlDefaultState;
  write_horizontally = UmlDefaultState;
  drawing_language = DefaultDrawingLanguage;
  package_name_in_tab = UmlDefaultState;
  show_class_pack_context_mode = DefaultShowContextMode;
  show_msg_context_mode = DefaultShowContextMode;
  draw_all_relations = UmlDefaultState;
  shadow = UmlDefaultState;
  show_stereotype_properties = UmlDefaultState;
}

void CollaborationDiagramSettings::save(QTextStream & st) const {
    nl_indent(st);
    st << "show_full_operations_definition " << stringify(show_full_operations_definition)
      << " show_hierarchical_rank " << stringify(show_hierarchical_rank)
	<< " write_horizontally " << stringify(write_horizontally)
	  << " drawing_language " << stringify(drawing_language)
	    << " package_name_in_tab " << stringify(package_name_in_tab)
	      << " show_context " << stringify(show_class_pack_context_mode)
		<< " show_msg_context " << stringify(show_msg_context_mode)
		  << " draw_all_relations " << stringify(draw_all_relations)
		    << " shadow " << stringify(shadow)
		      << " show_stereotype_properties " << stringify(show_stereotype_properties);
}

void CollaborationDiagramSettings::save_msg(QTextStream & st) const {
    nl_indent(st);
    st << "show_full_operations_definition " << stringify(show_full_operations_definition)
      << " show_hierarchical_rank " << stringify(show_hierarchical_rank)
	<< " drawing_language " << stringify(drawing_language)
	  << " show_msg_context " << stringify(show_msg_context_mode);
}

void CollaborationDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "show_full_operations_definition")) {
    show_full_operations_definition = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_hierarchical_rank")) {
    show_hierarchical_rank = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "write_horizontally") ||
      !strcmp(k, "write_horizontaly")) {
    write_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "drawing_language")) {
    drawing_language = ::drawing_language(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context")) {
    show_class_pack_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_msg_context")) {
    show_msg_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool CollaborationDiagramSettings::complete(CollaborationDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(show_full_operations_definition, UmlDefaultState);
  check_default(show_hierarchical_rank, UmlDefaultState);
  check_default(write_horizontally, UmlDefaultState);
  check_default(drawing_language, DefaultDrawingLanguage);
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_class_pack_context_mode, DefaultShowContextMode);
  check_default(show_msg_context_mode, DefaultShowContextMode);
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  check_default(show_stereotype_properties, UmlDefaultState);
  
  return done == 10;
}

void CollaborationDiagramSettings::assign(const CollaborationDiagramSettings & from) {
  assign_default(show_full_operations_definition, UmlDefaultState);
  assign_default(show_hierarchical_rank, UmlDefaultState);
  assign_default(write_horizontally, UmlDefaultState);
  assign_default(drawing_language, DefaultDrawingLanguage);
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(show_class_pack_context_mode, DefaultShowContextMode);
  assign_default(show_msg_context_mode, DefaultShowContextMode);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void CollaborationDiagramSettings::complete(StateSpecVector & a, bool local) {
  int i = a.size();
  
  a.resize(i + 10);
  if (local) {
    a[i].set(TR("drawing language"), &drawing_language);
    a[i+1].set(TR("show operations full definition"),
	       &show_full_operations_definition);
    a[i+2].set(TR("show hierarchical rank"),
	       &show_hierarchical_rank);
    a[i+3].set(TR("write name:type horizontally"),
	       &write_horizontally);
    a[i+4].set(TR("packages name in tab"), &package_name_in_tab);
    a[i+5].set(TR("show classes and packages context"), &show_class_pack_context_mode);
    a[i+6].set(TR("show messages context"), &show_msg_context_mode);
    a[i+7].set(TR("draw all relations"), &draw_all_relations);
    a[i+8].set(TR("show stereotype properties"), &show_stereotype_properties);
    a[i+9].set(TR("draw shadow"), &shadow);
  }
  else {
    QString s = TR("communication") + "#";
    
    a[i].set(s + TR("drawing language"),
	     &drawing_language);
    a[i+1].set(s + TR("show operations full definition"),
	       &show_full_operations_definition);
    a[i+2].set(s + TR("show hierarchical rank"),
	       &show_hierarchical_rank);
    a[i+3].set(s + TR("write name:type horizontally"),
	       &write_horizontally);
    a[i+4].set(s + TR("show packages name in tab"), &package_name_in_tab);
    a[i+5].set(s + TR("show classes and packages context"), &show_class_pack_context_mode);
    a[i+6].set(s + TR("show messages context"), &show_msg_context_mode);
    a[i+7].set(s + TR("draw all relations"), &draw_all_relations);
    a[i+8].set(s + TR("show stereotype properties"), &show_stereotype_properties);
    a[i+9].set(s + TR("draw shadow"), &shadow);
  }
}

void CollaborationDiagramSettings::complete_msg(StateSpecVector & a) {
  int i = a.size();
  
  a.resize(i + 4);
  a[i].set(TR("drawing language"), &drawing_language);
  a[i+1].set(TR("show operations full definition"),
	     &show_full_operations_definition);
  a[i+2].set(TR("show messages context"), &show_msg_context_mode);
  a[i+3].set(TR("show hierarchical rank"),
	     &show_hierarchical_rank);
}

//

ObjectDiagramSettings::ObjectDiagramSettings() {
  write_horizontally = UmlDefaultState;
  package_name_in_tab = UmlDefaultState;
  show_class_pack_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  draw_all_relations = UmlDefaultState;
  shadow = UmlDefaultState;
  show_stereotype_properties = UmlDefaultState;
}

void ObjectDiagramSettings::save(QTextStream & st) const {
    nl_indent(st);
    st << " write_horizontally " << stringify(write_horizontally)
      << " package_name_in_tab " << stringify(package_name_in_tab)
	<< " show_context " << stringify(show_class_pack_context_mode)
	  << " auto_label_position " << stringify(auto_label_position)
	    << " draw_all_relations " << stringify(draw_all_relations)
	      << " shadow " << stringify(shadow)
		<< " show_stereotype_properties " << stringify(show_stereotype_properties);
}

void ObjectDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "write_horizontally") ||
      !strcmp(k, "write_horizontaly")) {
    write_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context")) {
    show_class_pack_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "auto_label_position")) {
    auto_label_position = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool ObjectDiagramSettings::complete(ObjectDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(write_horizontally, UmlDefaultState);
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_class_pack_context_mode, DefaultShowContextMode);
  check_default(auto_label_position, UmlDefaultState);
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  check_default(show_stereotype_properties, UmlDefaultState);
  
  return done == 7;
}

void ObjectDiagramSettings::assign(const ObjectDiagramSettings & from) {
  assign_default(write_horizontally, UmlDefaultState);
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(show_class_pack_context_mode, DefaultShowContextMode);
  assign_default(auto_label_position, UmlDefaultState);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void ObjectDiagramSettings::complete(StateSpecVector & a, bool local) {
  int i = a.size();
  
  a.resize(i + 7);
  if (local) {
    a[i].set(TR("write name:type horizontally"), &write_horizontally);
    a[i+1].set(TR("packages name in tab"), &package_name_in_tab);
    a[i+2].set(TR("show classes and packages context"), &show_class_pack_context_mode);
    a[i+3].set(TR("automatic labels position"), &auto_label_position);
    a[i+4].set(TR("draw all relations"), &draw_all_relations);
    a[i+5].set(TR("show stereotype properties"), &show_stereotype_properties);
    a[i+6].set(TR("draw shadow"), &shadow);
  }
  else {
    QString s = TR("object") + "#";
    
    a[i].set(s + TR("write name:type horizontally"), &write_horizontally);
    a[i+1].set(s + TR("show packages name in tab"), &package_name_in_tab);
    a[i+2].set(s + TR("show classes and packages context"), &show_class_pack_context_mode);
    a[i+3].set(s + TR("automatic labels position"), &auto_label_position);
    a[i+4].set(s + TR("draw all relations"), &draw_all_relations);
    a[i+5].set(s + TR("show stereotype properties"), &show_stereotype_properties);
    a[i+6].set(s + TR("draw shadow"), &shadow);
  }
}

//

UseCaseDiagramSettings::UseCaseDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  draw_all_relations = UmlDefaultState;
  class_drawing_mode = DefaultClassDrawingMode;
  shadow = UmlDefaultState;
  show_stereotype_properties = UmlDefaultState;
}

void UseCaseDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " auto_label_position " << stringify(auto_label_position)
	<< " draw_all_relations " << stringify(draw_all_relations)
	  << " class_drawing_mode " << stringify(class_drawing_mode)
	    << " shadow " << stringify(shadow)
	      << " show_stereotype_properties " << stringify(show_stereotype_properties);
}

void UseCaseDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "auto_label_position")) {
    auto_label_position = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "class_drawing_mode")) {
    class_drawing_mode = drawing_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool UseCaseDiagramSettings::complete(UseCaseDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(auto_label_position, UmlDefaultState);
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  check_default(class_drawing_mode, DefaultClassDrawingMode);
  check_default(show_stereotype_properties, UmlDefaultState);
  
  return done == 7;
}

void UseCaseDiagramSettings::assign(const UseCaseDiagramSettings & from) {
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(auto_label_position, UmlDefaultState);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);
  assign_default(class_drawing_mode, DefaultClassDrawingMode);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

bool UseCaseDiagramSettings::complete(SimpleClassDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(class_drawing_mode, DefaultClassDrawingMode);
  check_default(show_stereotype_properties, UmlDefaultState);
  
  return done == 3;
}

void UseCaseDiagramSettings::complete(StateSpecVector & a, bool local) {
  int i = a.size();
  
  a.resize(i + 7);
  if (local) {
    a[i].set(TR("packages name in tab"), &package_name_in_tab);
    a[i + 1].set(TR("show packages context"), &show_context_mode);
    a[i + 2].set(TR("automatic labels position"), &auto_label_position);
    a[i + 3].set(TR("draw all relations"), &draw_all_relations);
    a[i + 4].set(TR("class drawing mode"), &class_drawing_mode);
    a[i + 5].set(TR("show stereotype properties"), &show_stereotype_properties);
    a[i + 6].set(TR("draw shadow"), &shadow);
  }
  else {
    QString s = TR("use case") + "#";
    
    a[i].set(s + TR("show packages name in tab"), &package_name_in_tab);
    a[i + 1].set(s + TR("show packages context"), &show_context_mode);
    a[i + 2].set(s + TR("automatic labels position"), &auto_label_position);
    a[i + 3].set(s + TR("draw all relations"), &draw_all_relations);
    a[i + 4].set(s + TR("class drawing mode"), &class_drawing_mode);
    a[i + 5].set(s + TR("show stereotype properties"), &show_stereotype_properties);
    a[i + 6].set(s + TR("draw shadow"), &shadow);
  }
}

//

ComponentDiagramSettings::ComponentDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  draw_all_relations = UmlDefaultState;
  shadow = UmlDefaultState;
}

void ComponentDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " auto_label_position " << stringify(auto_label_position)
	<< " draw_all_relations " << stringify(draw_all_relations)
	  << " shadow " << stringify(shadow);
  componentdrawingsettings.save(st);
}

void ComponentDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "auto_label_position")) {
    auto_label_position = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  componentdrawingsettings.read(st, k);
}

bool ComponentDiagramSettings::complete(ComponentDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(auto_label_position, UmlDefaultState);
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return componentdrawingsettings.complete(result.componentdrawingsettings)
    && (done == 5);
}

void ComponentDiagramSettings::assign(const ComponentDiagramSettings & from) {
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(auto_label_position, UmlDefaultState);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);
  
  componentdrawingsettings.assign(from.componentdrawingsettings);
}

void ComponentDiagramSettings::complete(StateSpecVector & a, bool local) {
  int i = a.size();
  
  a.resize(i + 5);
  if (local) {
    a[i].set(TR("packages name in tab"), &package_name_in_tab);
    a[i + 1].set(TR("show packages context"), &show_context_mode);
    a[i + 2].set(TR("automatic labels position"), &auto_label_position);
    a[i + 3].set(TR("draw all relations"), &draw_all_relations);
    a[i + 4].set(TR("draw shadow"), &shadow);
  }
  else {
    QString s = TR("component") + "#";
    
    a[i].set(s + TR("show packages name in tab"), &package_name_in_tab);
    a[i + 1].set(s + TR("show packages context"), &show_context_mode);
    a[i + 2].set(s + TR("automatic labels position"), &auto_label_position);
    a[i + 3].set(s + TR("draw all relations"), &draw_all_relations);
    a[i + 4].set(s + TR("draw shadow"), &shadow);
  }
  componentdrawingsettings.complete(a, local, FALSE);
}

//

DeploymentDiagramSettings::DeploymentDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  write_horizontally = UmlDefaultState;
  auto_label_position = UmlDefaultState;
  draw_all_relations = UmlDefaultState;
  shadow = UmlDefaultState;
}

void DeploymentDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " write_horizontally " << stringify(write_horizontally)
	<< " auto_label_position " << stringify(auto_label_position)
	  << " draw_all_relations " << stringify(draw_all_relations)
	    << " shadow " << stringify(shadow);
  componentdrawingsettings.save(st);
}

void DeploymentDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "write_horizontally") ||
      !strcmp(k, "write_horizontaly")) {
    write_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "auto_label_position")) {
    auto_label_position = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  componentdrawingsettings.read(st, k);
}

bool DeploymentDiagramSettings::complete(DeploymentDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(write_horizontally, UmlDefaultState);
  check_default(auto_label_position, UmlDefaultState);
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return componentdrawingsettings.complete(result.componentdrawingsettings)
    && (done == 6);
}

void DeploymentDiagramSettings::assign(const DeploymentDiagramSettings & from) {
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(write_horizontally, UmlDefaultState);
  assign_default(auto_label_position, UmlDefaultState);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);

  componentdrawingsettings.assign(from.componentdrawingsettings);
}

void DeploymentDiagramSettings::complete(StateSpecVector & a, bool local) {
  int i = a.size();
  
  a.resize(i + 6);
  if (local) {
    a[i].set(TR("packages name in tab"), &package_name_in_tab);
    a[i + 1].set(TR("show packages context"), &show_context_mode);
    a[i + 2].set(TR("write node instance horizontally"), &write_horizontally);
    a[i + 3].set(TR("automatic labels position"), &auto_label_position);
    a[i + 4].set(TR("draw all relations"), &draw_all_relations);
    a[i + 5].set(TR("draw shadow"), &shadow);
  }
  else  {
    QString s = TR("deployment") + "#";
    
    a[i].set(s + TR("show packages name in tab"), &package_name_in_tab);
    a[i + 1].set(s + TR("show packages context"), &show_context_mode);
    a[i + 2].set(s + TR("write node instances horizontally"), &write_horizontally);
    a[i + 3].set(s + TR("automatic labels position"), &auto_label_position);
    a[i + 4].set(s + TR("draw all relations"), &draw_all_relations);
    a[i + 5].set(s + TR("draw shadow"), &shadow);
  }
  
  componentdrawingsettings.complete(a, local, TRUE);
}

//

ComponentDrawingSettings::ComponentDrawingSettings() {
  draw_component_as_icon = UmlDefaultState;
  show_component_req_prov = UmlDefaultState;
  show_component_rea = UmlDefaultState;
  show_stereotype_properties = UmlDefaultState;
}

void ComponentDrawingSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "draw_component_as_icon " << stringify(draw_component_as_icon)
    << " show_component_req_prov " << stringify(show_component_req_prov)
      << " show_component_rea " << stringify(show_component_rea)
	<< " show_stereotype_properties " << stringify(show_stereotype_properties);
}
  
void ComponentDrawingSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "draw_component_as_icon") ||
      !strcmp(k, "drawn_as_icon") /* old format */) {
    draw_component_as_icon = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_component_req_prov")) {
    show_component_req_prov = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_component_rea")) {
    show_component_rea = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool ComponentDrawingSettings::complete(ComponentDrawingSettings & result) const {
  unsigned done = 0;
  
  check_default(draw_component_as_icon, UmlDefaultState);
  check_default(show_component_req_prov, UmlDefaultState);
  check_default(show_component_rea, UmlDefaultState);
  check_default(show_stereotype_properties, UmlDefaultState);
  
 return (done == 4);
}

void ComponentDrawingSettings::assign(const ComponentDrawingSettings & from) {
  assign_default(draw_component_as_icon, UmlDefaultState);
  assign_default(show_component_req_prov, UmlDefaultState);
  assign_default(show_component_rea, UmlDefaultState);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void ComponentDrawingSettings::assign(const ComponentDiagramSettings & from) {
  assign(from.componentdrawingsettings);
}

void ComponentDrawingSettings::complete(StateSpecVector & a,
					bool local, bool depl) {
  QString s = ((depl) ? TR("deployment") : TR("component")) + "#";
  int i = a.size();
  
  a.resize(i + 4);
  a[i].set((local) ? TR("draw component as icon")
		   : s + ((depl) ? TR("draw component as icon")
				 : TR("draw component as icon")),
	   &draw_component_as_icon);
  a[i + 1].set((local) ? TR("show component's required\nand provided interfaces")
		       : s + ((depl) ? TR("show component's required\nand provided interfaces")
				     : TR("show component's required\nand provided interfaces")),
	       &show_component_req_prov);
  a[i + 2].set((local) ? TR("show component's realizations")
		       : s + ((depl) ? TR("show component's realizations")
				     : TR("show component's realizations")),
	       &show_component_rea);
  a[i + 3].set((local) ? TR("show stereotype properties")
		       : s + ((depl) ? TR("show stereotype properties")
				     : TR("show stereotype properties")),
	       &show_stereotype_properties);
}

//

StateDiagramSettings::StateDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  write_label_horizontally = UmlDefaultState;
  show_trans_definition = UmlDefaultState;
  draw_all_relations = UmlDefaultState;
  shadow = UmlDefaultState;
}

void StateDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " auto_label_position " << stringify(auto_label_position)
	<< " write_trans_label_horizontally " << stringify(write_label_horizontally)
	  << " show_trans_definition " << stringify(show_trans_definition)
	    << " draw_all_relations " << stringify(draw_all_relations)
	      << " shadow " << stringify(shadow);
  statedrawingsettings.save(st);
}

void StateDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "auto_label_position")) {
    auto_label_position = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "write_trans_label_horizontally")) {
    write_label_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_trans_definition")) {
    show_trans_definition = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  statedrawingsettings.read(st, k);
}

bool StateDiagramSettings::complete(StateDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(auto_label_position, UmlDefaultState);
  check_default(write_label_horizontally, UmlDefaultState);
  check_default(show_trans_definition, UmlDefaultState);
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return statedrawingsettings.complete(result.statedrawingsettings)
    && (done == 7);
}

void StateDiagramSettings::assign(const StateDiagramSettings & from) {
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(auto_label_position, UmlDefaultState);
  assign_default(write_label_horizontally, UmlDefaultState);
  assign_default(show_trans_definition, UmlDefaultState);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);
  
  statedrawingsettings.assign(from.statedrawingsettings);
}

void StateDiagramSettings::complete(StateSpecVector & a, bool local) {
  int i = a.size();
  
  a.resize(i + 7);
  if (local) {
    a[i].set(TR("packages name in tab"), &package_name_in_tab);
    a[i + 1].set(TR("show packages context"), &show_context_mode);
    a[i + 2].set(TR("automatic labels position"), &auto_label_position);
    a[i + 3].set(TR("write transition horizontally"), &write_label_horizontally);
    a[i + 4].set(TR("show transition definition"), &show_trans_definition);
    a[i + 5].set(TR("draw all relations"), &draw_all_relations);
    a[i + 6].set(TR("draw shadow"), &shadow);
  }
  else {
    QString s = TR("state") + "#";
    
    a[i].set(s + TR("show packages name in tab"), &package_name_in_tab);
    a[i + 1].set(s + TR("show packages context"), &show_context_mode);
    a[i + 2].set(s + TR("automatic labels position"), &auto_label_position);
    a[i + 3].set(s + TR("write transition horizontally"), &write_label_horizontally);
    a[i + 4].set(s + TR("show transition definition"), &show_trans_definition);
    a[i + 5].set(s + TR("draw all relations"), &draw_all_relations);
    a[i + 6].set(s + TR("draw shadow"), &shadow);
  }
  statedrawingsettings.complete(a, local);
}

//

StateDrawingSettings::StateDrawingSettings() {
  show_activities = UmlDefaultState;
  region_horizontally = UmlDefaultState;
  drawing_language = DefaultDrawingLanguage;
  show_stereotype_properties = UmlDefaultState;
}

void StateDrawingSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "show_activities " << stringify(show_activities)
    << " region_horizontally " << stringify(region_horizontally)
      << " drawing_language " << stringify(drawing_language)
	<< " show_stereotype_properties " << stringify(show_stereotype_properties);
}
  
void StateDrawingSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "show_activities")) {
    show_activities = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "region_horizontally")) {
    region_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "drawing_language")) {
    drawing_language = ::drawing_language(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool StateDrawingSettings::complete(StateDrawingSettings & result) const {
  unsigned done = 0;
  
  check_default(show_activities, UmlDefaultState);
  check_default(region_horizontally, UmlDefaultState);
  check_default(drawing_language, DefaultDrawingLanguage);
  check_default(show_stereotype_properties, UmlDefaultState);
  
 return (done == 4);
}

void StateDrawingSettings::assign(const StateDrawingSettings & from) {
  assign_default(show_activities, UmlDefaultState);
  assign_default(region_horizontally, UmlDefaultState);
  assign_default(drawing_language, DefaultDrawingLanguage);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void StateDrawingSettings::complete(StateSpecVector & a, bool local) {
  int i = a.size();
  
  a.resize(i + 4);

  if (local) {
    a[i].set(TR("show state activities"), &show_activities);
    a[i + 1].set(TR("draw state's regions horizontally"), &region_horizontally);
    a[i + 2].set(TR("drawing language"), &drawing_language);
    a[i + 3].set(TR("show stereotype properties"), &show_stereotype_properties);
    }
  else {
    QString s = TR("state") + "#";
    
    a[i].set(s + TR("show state activities"), &show_activities);
    a[i + 1].set(s + TR("draw state's regions horizontally"), &region_horizontally);
    a[i + 2].set(s + TR("drawing language"), &drawing_language);
    a[i + 3].set(s + TR("show stereotype properties"), &show_stereotype_properties);
  }
}

//

ActivityDiagramSettings::ActivityDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  show_opaque_action_definition = UmlDefaultState;
  auto_label_position = UmlDefaultState;
  write_label_horizontally = UmlDefaultState;
  draw_all_relations = UmlDefaultState;
  shadow = UmlDefaultState;
}

void ActivityDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " show_opaque_action_definition " << stringify(show_opaque_action_definition)
        << " auto_label_position " << stringify(auto_label_position)
	  << " write_flow_label_horizontally " << stringify(write_label_horizontally)
	    << " draw_all_relations " << stringify(draw_all_relations)
	      << " shadow " << stringify(shadow);
  activitydrawingsettings.save(st);
}

void ActivityDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_opaque_action_definition")) {
    show_opaque_action_definition = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "auto_label_position")) {
    auto_label_position = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "write_flow_label_horizontally")) {
    write_label_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  activitydrawingsettings.read(st, k);
}

bool ActivityDiagramSettings::complete(ActivityDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(show_opaque_action_definition, UmlDefaultState);
  check_default(auto_label_position, UmlDefaultState);
  check_default(write_label_horizontally, UmlDefaultState);
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return activitydrawingsettings.complete(result.activitydrawingsettings)
    && (done == 7);
}

void ActivityDiagramSettings::assign(const ActivityDiagramSettings & from) {
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(show_opaque_action_definition, UmlDefaultState);
  assign_default(auto_label_position, UmlDefaultState);
  assign_default(write_label_horizontally, UmlDefaultState);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);

  activitydrawingsettings.assign(from.activitydrawingsettings);
}

void ActivityDiagramSettings::complete(StateSpecVector & a, bool local) {
  int i = a.size();
  
  a.resize(i + 7);
  if (local) {
    a[i].set(TR("packages name in tab"), &package_name_in_tab);
    a[i + 1].set(TR("show packages context"), &show_context_mode);
    a[i + 2].set(TR("automatic labels position"), &auto_label_position);
    a[i + 3].set(TR("write flow label horizontally"), &write_label_horizontally);
    a[i + 4].set(TR("show opaque action definition"), &show_opaque_action_definition);
    a[i + 5].set(TR("draw all relations"), &draw_all_relations);
    a[i + 6].set(TR("draw shadow"), &shadow);
  }
  else {
    QString s = TR("activity") + "#";
    
    a[i].set(s + TR("show packages name in tab"), &package_name_in_tab);
    a[i + 1].set(s + TR("show packages context"), &show_context_mode);
    a[i + 2].set(s + TR("automatic labels position"), &auto_label_position);
    a[i + 3].set(s + TR("write flow label horizontally"), &write_label_horizontally);
    a[i + 4].set(s + TR("show opaque action definition"), &show_opaque_action_definition);
    a[i + 5].set(s + TR("draw all relations"), &draw_all_relations);
    a[i + 6].set(s + TR("draw shadow"), &shadow);
  }
  activitydrawingsettings.complete(a, local);
}

//

ActivityDrawingSettings::ActivityDrawingSettings() {
  show_infonote = UmlDefaultState;
  drawing_language = DefaultDrawingLanguage;
  show_stereotype_properties = UmlDefaultState;
}

void ActivityDrawingSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "show_infonote " << stringify(show_infonote)
    << " drawing_language " << stringify(drawing_language)
      << " show_stereotype_properties " << stringify(show_stereotype_properties);
}
  
void ActivityDrawingSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "show_infonote")) {
    show_infonote = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "drawing_language")) {
    drawing_language = ::drawing_language(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool ActivityDrawingSettings::complete(ActivityDrawingSettings & result) const {
  unsigned done = 0;
  
  check_default(show_infonote, UmlDefaultState);
  check_default(drawing_language, DefaultDrawingLanguage);
  check_default(show_stereotype_properties, UmlDefaultState);
  
 return (done == 3);
}

void ActivityDrawingSettings::assign(const ActivityDrawingSettings & from) {
  assign_default(show_infonote, UmlDefaultState);
  assign_default(drawing_language, DefaultDrawingLanguage);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void ActivityDrawingSettings::complete(StateSpecVector & a, bool local) {
  int i = a.size();
  
  a.resize(i + 3);

  if (local) {
    a[i].set(TR("show information note"), &show_infonote);
    a[i + 1].set(TR("drawing language"), &drawing_language);
    a[i + 2].set(TR("show stereotype properties"), &show_stereotype_properties);
  }
  else {
    QString s = TR("activity") + "#";
    
    a[i].set(s + TR("show information note"), &show_infonote);
    a[i + 1].set(s + TR("drawing language"), &drawing_language);
    a[i + 2].set(s + TR("show stereotype properties"), &show_stereotype_properties);
  }
}

// to update activity element settings
void ActivityDrawingSettings::set(StateSpecVector & a, int index) {
  // follow order of previous operation
  if (!a[index].name.isEmpty())
    show_infonote = (Uml3States) *((Uml3States *) a[index].state);
  if (!a[index + 1].name.isEmpty())
    drawing_language = (DrawingLanguage) *((DrawingLanguage *) a[index + 1].state);
  if (!a[index + 2].name.isEmpty())
    show_stereotype_properties = (Uml3States) *((Uml3States *) a[index + 2].state);
}
