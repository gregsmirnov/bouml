
#include "UmlItem.h"
#include "FileOut.h"

#include "UmlCom.h"
 UmlItem::~UmlItem() {
}

void UmlItem::xmi(int, char **) {
  UmlCom::trace("Error : must be applied on a package<br>");
}

bool UmlItem::write_if_needed(FileOut & out) {
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  bool used = FALSE;
  
  for (unsigned i = 0; i != n; i += 1)
    used |= ch[i]->write_if_needed(out);

  return used;
}

void UmlItem::write(FileOut & out) {
  if (! _written) {
    _written = TRUE;
    
    parent()->write(out);
  }
}

void UmlItem::write_stereotype(FileOut & out) {
  if (! stereotype().isEmpty()) {
    out.indent(); 
    out << "<UML:ModelElement.stereotype>\n"; 
    out.indent(); 
    out << "\t<UML:Stereotype name=\"";
    out.quote(stereotype());
    out << "\"/>\n";
    out.indent(); 
    out << "</UML:ModelElement.stereotype>\n"; 

    switch (_taggedvalue_mode) {
    case 1:
      out.indent(); 
      out << "<UML:ModelElement.taggedValue>\n"; 
      out.indent(); 
      out << "\t<UML:TaggedValue tag=\"stereotype\" value=\"";
      out.quote(stereotype());
      out << "\"/>\n";
      out.indent(); 
      out << "</UML:ModelElement.taggedValue>\n"; 
      break;
    case 2:
      out.indent(); 
      out << "<UML:ModelElement.taggedValue>\n"; 
      out.indent(); 
      out << "\t<UML:TaggedValue.tag>stereotype</UML:TaggedValue.tag>\n";
      out.indent(); 
      out << "\t<UML:TaggedValue.value>";
      out.quote(stereotype());
      out << "</UML:TaggedValue.value>\n";
      out.indent(); 
      out << "</UML:ModelElement.taggedValue>\n"; 
    }
  }

}

void UmlItem::write_description_properties(FileOut & out) {
  if (_taggedvalue_mode != 0) {
    if (! description().isEmpty()) {
      out.indent();
      out << "<UML:ModelElement.taggedValue>\n";
      out.indent();
      if (_taggedvalue_mode == 1) {
	out << "\t<UML:TaggedValue tag=\"documentation\" value=\"";
	out.quote(description());
	out << "\"/>\n";
      }
      else {
	out << "\t<UML:TaggedValue.tag>documentation</UML:TaggedValue.tag>\n";
	out.indent();
	out << "\t<UML:TaggedValue.value>";
	out.quote(description());
	out << "</UML:TaggedValue.value>\n";
      }
      out.indent();
      out << "</UML:ModelElement.taggedValue>\n";
    }
      
    const QDict<QCString> up = properties();    
    QDictIterator<QCString> it(up);
    
    while (it.current()) {
      out.indent();
      out << "<UML:ModelElement.taggedValue>\n";
      out.indent();
      if (_taggedvalue_mode == 1) {
	out << "\t<UML:TaggedValue tag=\"";
	out.quote(it.currentKey());
	out << "\" value=\"";
	out.quote(*(it.current()));
	out << "\"/>\n";
      }
      else {
	out << "\t<UML:TaggedValue.tag>";
	out.quote(it.currentKey());
	out << "</UML:TaggedValue.tag>\n";
	out.indent();
	out << "\t<UML:TaggedValue.value>";
	out.quote(*(it.current()));
	out << "</UML:TaggedValue.value>\n";
      }
      out.indent();
      out << "</UML:ModelElement.taggedValue>\n";
      ++it;
    }
  }
}

void UmlItem::ref(FileOut & out) {
  // theorically not called
  out << "<UML:???kind=" << kind() << "??? ";
  out.idref(this); 
  out << "/>"; 
}

bool UmlItem::_gen_views;

Language UmlItem::_lang;

int UmlItem::_taggedvalue_mode;

