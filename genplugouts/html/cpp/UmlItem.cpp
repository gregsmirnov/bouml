
#include "UmlItem.h"
#include "UmlDiagram.h"
#include "UmlTypeSpec.h"

#include <qdir.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

#include "UmlClass.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"
#include "UmlClassDiagram.h"
#include "UmlObjectDiagram.h"
#include "UmlActivityDiagram.h"
#include "UmlStateDiagram.h"
#include "UmlUseCaseDiagram.h"
#include "UmlSequenceDiagram.h"
#include "UmlCollaborationDiagram.h"
#include "UmlComponentDiagram.h"
#include "UmlDeploymentDiagram.h"
#include "UmlPackage.h"
#include "UmlUseCase.h"
#include "UmlActivity.h"
#include "UmlState.h"
#include "UmlNcRelation.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "JavaSettings.h"

 UmlItem::~UmlItem() {
}

QCString UmlItem::sKind() {
  return "???";
}

void UmlItem::set_dir(int argc, char ** argv) {
  QString d;
  bool ask;
  bool rem;
  bool replace_css;

  if ((argc != 0) && !strcmp(argv[0], "-flat")) {
    flat = TRUE;
    argc -= 1;
    argv += 1;
  }
  else
    flat = FALSE;

  if ((argc != 0) && !strcmp(argv[0], "-svg")) {
    svg = TRUE;
    argc -= 1;
    argv += 1;
  }
  else
    svg = FALSE;

  if (argc == 0) {
    if (!UmlBasePackage::getProject()->propertyValue("html dir", directory))
      directory = "/tmp/" + name() + "_html";

    d = directory;
    d = QFileDialog::getExistingDirectory(d, 0, "","Directory where the files will be produced", TRUE);
  
    if (d.isEmpty())
      throw 0;
    
#ifdef WIN32
    QDir dir(d);
    
    if (! dir.exists()) {
      int index = d.length() - 1;
      
      if (d.at(index) == QChar('/'))
	index -= 1;
      
      QString d2 = d;
	
      d2.remove(index, 1);
	
      QDir dir2(d2);
	
      if (dir2.exists())
	d = d2;
    }
#endif
    
    ask = TRUE;
    rem = FALSE;
    replace_css = TRUE;
  }
  else {
    d = argv[0];    
    ask = FALSE;
    
    if ((argc == 2) && !strcmp(argv[1], "-del_html")) {
      rem = TRUE;
      argc -= 1;
      argv += 1;
    }
    else
      rem = FALSE;
    
    replace_css = ((argc == 2) && !strcmp(argv[1], "-del_css"));
  }
    
  if ((d.at(d.length() - 1) != '/') && (d.at(d.length() - 1) != '\\'))
    d += '/';
    
  directory = (const char *) d;
  
  QDir dir(d);
  unsigned i;
  
  if (dir.exists()) {
    if (ask) {
      // remove old html files ?
      for (i = 0; i != dir.count(); i += 1) {
	if (dir[i].right(5).lower() == ".html") {
	  if (!rem &&
	      (QMessageBox::critical(0, "Html generator", 
				     "Delete already existing html files ?",
				     "Yes", "No", QString::null, 
				     1, 1) != 0))
	    break;
	  rem = TRUE;
	  dir.remove(dir[i]);
	}
      }
      
      // remove old css file ?
      for (i = 0; i != dir.count(); i += 1) {
	if (dir[i] == "style.css") {
	  if (QMessageBox::critical(0, "Html generator", 
				    "Replace already existing style.css ?",
				    "Yes", "No", QString::null, 
				    1, 1) != 0)
	    replace_css = FALSE;
	  break;
	}
      }
      
    }
    else {
      if (rem) {
	for (i = 0; i != dir.count(); i += 1) {
	  if (dir[i].right(5).lower() == ".html")
	    dir.remove(dir[i]);
	}
      }
      
      if (!replace_css) {
	replace_css = TRUE;	// to create it

	for (i = 0; i != dir.count(); i += 1) {
	  if (dir[i] == "style.css") {
	    // already exist, don't replace
	    replace_css = FALSE;
	    break;
	  }
	}
      }
    }
  }
  else if (! dir.mkdir(d)) {
    UmlCom::trace("Cannot create directory " + directory);
    throw 0;
  }
  else
    replace_css = TRUE;	// to create it

  if (ask)
    UmlBasePackage::getProject()->set_PropertyValue("html dir", directory);

  if (replace_css) {
    // write css file
    FileWriter fw;
    
    if (fw.open(directory + "style.css")) {
      fw.write("\
div.title { font-size: 150%; background: #87ceff; text-align: center; font-weight: bold; }\n\
\n\
div.sub { margin-left : 20px; }\n\
div.element { background: #d3d3d3; }\n\
\n\
h1.package { background: #ffe4c4; }\n\
h1.view { background: #98fb98; }\n\
\n\
h2.package { background: #ffe4c4; }\n\
h2.view { background: #98fb98; }\n\
h2.class { background: #87ceff; }\n\
h2.usecase { background: #87ceff; }\n\
h2.state { background: #87ceff; }\n\
h2.activity { background: #87ceff; }\n\
\n\
h3.package { background: #ffe4c4; }\n\
h3.view { background: #98fb98; }\n\
h3.class { background: #87ceff; }\n\
h3.usecase { background: #87ceff; }\n\
h3.state { background: #87ceff; }\n\
h3.stateregion { background: #87ceff; }\n\
h3.activity { background: #87ceff; }\n\
\n\
h4.package { background: #ffe4c4; }\n\
h4.view { background: #98fb98; }\n\
h4.class { background: #87ceff; }\n\
h4.usecase { background: #87ceff; }\n\
h4.state { background: #87ceff; }\n\
h4.stateregion { background: #87ceff; }\n\
h4.activity { background: #87ceff; }\n");
      fw.close();
    }
  }
}

void UmlItem::memo_ref() {
  all.addElement(this);
  known = TRUE;
  
  const QVector<UmlItem> ch = children();
  
  for (unsigned i = 0; i != ch.size(); i += 1)
    ch[i]->memo_ref();
}

void UmlItem::define() {
  fw.write("<a name=\"ref");
  fw.write((unsigned) kind());
  fw.write('_');
  fw.write((unsigned) getIdentifier());
  fw.write("\"></a>\n");
}

void UmlItem::start_file(QCString f, QCString s, bool withrefs)
{
  QCString filename = directory + f;
  bool is_frame = (f == "index-withframe");
  
  if (! fw.open(filename + ".html"))
    throw 0;
  
  fw.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  fw.write("<!-- Documentation produced by the Html generator of Bouml (http://bouml.free.fr) -->\n");
  fw.write((is_frame)
	   ? "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Frameset//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd\">\n"
	   : "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n");
  fw.write((svg) ? "<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:svg=\"http://www.w3.org/2000/svg\">\n"
		 : "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
  fw.write("\n");
  fw.write("<head>\n");
  
  if (s.isEmpty()) {
    fw.write("<title>" + filename + "</title>\n");
    fw.write("<link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\" />\n");
    fw.write("</head>\n");
    if (withrefs) {
      if (! is_frame)
	fw.write("<body bgcolor=\"#ffffff\">\n");
      ref_indexes();
    }
  }
  else {
    fw.write("<title>"); fw.write(s); fw.write("</title>\n");
    fw.write("<link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\" />\n");
    fw.write("</head>\n");
    fw.write("\n");
    if (! is_frame)
      fw.write("<body bgcolor=\"#ffffff\">\n");
    fw.write("\n");
    fw.write("<div class = \"title\">"); writeq(s); fw.write("</div>\n");
    fw.write("<p></p>\n");
    fw.write("\n");
    fw.write("<!-- ============================================================= -->\n");
    fw.write("\n");
  }
}

void UmlItem::end_file()
{
  fw.write("</body>\n");
  fw.write("</html>\n");
    
  fw.close();

}

void UmlItem::ref_indexes()
{
  fw.write("<hr />\n<p><a href=\"index.html\" target = \"projectFrame\"><b> -Top- </b></a>");
  
  UmlClass::ref_index();
  UmlOperation::ref_index();
  UmlAttribute::ref_index();
  UmlPackage::ref_index();
  UmlUseCase::ref_index();
  UmlActivity::ref_index();
  UmlState::ref_index();
  UmlClassDiagram::ref_index();
  UmlObjectDiagram::ref_index();
  UmlActivityDiagram::ref_index();
  UmlStateDiagram::ref_index();
  UmlUseCaseDiagram::ref_index();
  UmlSequenceDiagram::ref_index();
  UmlCollaborationDiagram::ref_index();
  UmlComponentDiagram::ref_index();
  UmlDeploymentDiagram::ref_index();
  fw.write("</p>\n<p>\n</p>\n<p>");
  
  for (unsigned i = 0; i != letters.length(); i += 1) {
    fw.write("<a href=\"index_");
    fw.write(letters.at(i) & 255u);
    fw.write(".html\" target = \"projectFrame\"><b> ");
    writeq(letters.at(i));
    fw.write(" </b></a>");
  }
  
  fw.write("</p>\n");
}

void UmlItem::generate_indexes()
{
  UmlClass::generate_index();
  UmlOperation::generate_index();
  UmlAttribute::generate_index();
  UmlPackage::generate_index();
  UmlUseCase::generate_index();
  UmlActivity::generate_index();
  UmlState::generate_index();
  UmlClassDiagram::generate_index();
  UmlObjectDiagram::generate_index();
  UmlActivityDiagram::generate_index();
  UmlStateDiagram::generate_index();
  UmlUseCaseDiagram::generate_index();
  UmlSequenceDiagram::generate_index();
  UmlCollaborationDiagram::generate_index();
  UmlComponentDiagram::generate_index();
  UmlDeploymentDiagram::generate_index();

  int n = all.size();
  int i;
  char previous;
  
  sort(all);
  
  previous = 0;
  for (i = 0; i != n; i += 1) {
    UmlItem * x = all.elementAt(i);
    QCString s = x->pretty_name();
    
    if (! s.isEmpty()) {
      char c = *((const char *) s);
      
      if ((c >= 'a') && (c <= 'z'))
	c += 'A' - 'a';
      
      if (c != previous) {
	previous = c;
	letters += c;
      }
    }
  }

  previous = 0;
  for (i = 0; i != n; i += 1) {
    UmlItem * x = all.elementAt(i);
    QCString s = x->pretty_name();
    
    if (! s.isEmpty()) {
      char c = *((const char *) s);
      
      if ((c >= 'a') && (c <= 'z'))
	c += 'A' - 'a';
      
      if (c != previous) {
	if (previous != 0) {
	  fw.write("</table>\n");
	  end_file();
	}
	
	previous = c;
	
	QCString sn;
	
	sn.setNum((int) (c & 255));
	
	start_file(QCString("index_") + sn, QCString("") + c, TRUE);
	
	fw.write("<table>\n");
	fw.write("<tr bgcolor=\"#f0f0f0\"><td align=\"center\"><b>Name</b></td><td align=\"center\"><b>Kind</b></td><td align=\"center\"><b>Description</b></td></tr>\n");
      }
      
      fw.write("<tr bgcolor=\"#f0f0f0\"><td>");
      x->write("projectFrame");
      fw.write("</td><td>");
      fw.write(x->sKind());
      fw.write("</td><td>");
      writeq(x->description());
      fw.write("</td></tr>\n");
    }
  }

  if (previous != 0) {
    fw.write("</table>\n");
    end_file();
  }
}

void UmlItem::frame()
{
  start_file("index-withframe", "", FALSE);

  fw.write("<frameset cols=\"20%,80%\">\n");
  fw.write("  <noframes>\n");
  fw.write("    <body>\n");
  fw.write("      <h2>Frame Alert</h2>\n");
  fw.write("      <p>This document is designed to be viewed using the frames feature. If you see this message, you are using a non-frame-capable web client.</p>\n");
  fw.write("      <p>Link to <a href=\"index.html\">Non-frame version.</a></p>\n");
  fw.write("    </body>\n");
  fw.write("  </noframes>\n");
  fw.write("  <frame src=\"classes_list.html\" name=\"classesListFrame\" />\n");
  fw.write("  <frameset rows=\"150,*\">\n");
  fw.write("    <frame src=\"navig.html\" name=\"navigFrame\" />\n");
  fw.write("    <frame src=\"index.html\" name=\"projectFrame\" />\n");
  fw.write("  </frameset>\n");
  fw.write("</frameset>\n");
 
  fw.write("</html>");
  fw.close();

  UmlCom::trace("document with frame produced in <i>"
		+ directory + "index-withframe.html");

  UmlCom::trace("document without frame produced in <i>"
		+ directory + "index.html");
}

bool UmlItem::chapterp() {
  return FALSE;
}

void UmlItem::html(QCString pfix, unsigned int rank, QCString what, unsigned int level, QCString kind) {
  define();
 
  chapter(what, pfix, rank, kind, level);

  if (! description().isEmpty()) {
    fw.write("<p>");
    writeq(description());
    fw.write("<br /></p>\n");
  }

  write_properties();
  
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  if (n != 0) {
    QCString spfix;
    
    if (rank != 0) {
      spfix.setNum(rank);
      spfix = pfix + spfix + ".";
    }
    
    level += 1;
    rank = 1;
    fw.write("<div class=\"sub\">\n");
    for (unsigned i = 0; i != n; i += 1) {
      ch[i]->html(spfix, rank, level);
      if (ch[i]->chapterp())
	rank += 1;
    }
    fw.write("</div>\n");
  }
}

void UmlItem::html(const char * what, UmlDiagram * diagram) {
  define();

  fw.write("<table><tr><td><div class=\"element\">");
  fw.write(what);
  fw.write(" <b>");
  writeq(name());
  fw.write("</b></div></td></tr></table>\n");
  
  QCString d = description();
  
  if (!d.isEmpty()) {
    fw.write("<p>");
    writeq(d);
    fw.write("<br /></p>\n");
  }
  
  write_dependencies();

  if (diagram != 0) {
    fw.write("<p>Diagram : ");
    diagram->write();
    fw.write("</p>\n");
  }

  write_properties();
}

void UmlItem::write_children(QCString pfix, unsigned int rank, unsigned int level) {
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  if (n != 0) {
    QCString spfix;
    unsigned srank = 1;
    
    if (rank != 0) {
      spfix.setNum(rank);
      spfix = pfix + spfix + ".";
      fw.write("<div class=\"sub\">\n");
    }

    level += 1;
    
    for (unsigned i = 0; i != n; i += 1) {
      ch[i]->html(spfix, srank, level);
      if (ch[i]->chapterp())
	srank += 1;
    }
    
    if (rank != 0)
      fw.write("</div>\n");
  }
}

void UmlItem::write_dependencies() {
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1) {
    if ((ch[i]->kind() == aNcRelation) &&
	(((UmlNcRelation *) ch[i])->relationKind() == aDependency)) {
      fw.write("<p>Depends on ");
      ((UmlNcRelation *) ch[i])->target()->write();
      fw.write("</p>");
    }
  }
}

void UmlItem::write_properties() {
  if (! stereotype().isEmpty()) {
    fw.write("<p>Stereotype: ");
    writeq(stereotype());
    fw.write("</p>\n");
  }
    
  const QDict<QCString> d = properties();
  
  if (! d.isEmpty()) {
    fw.write("<p>Properties:</p><ul>\n");
    
    QDictIterator<QCString> it(d);
    
    while (it.current()) {
      fw.write("<li>");
      writeq(it.currentKey().latin1());
      fw.write(":<br /><div class=\"sub\">");
      writeq(*(it.current()));
      fw.write("</div></li>\n");
      ++it;
    }
    
    fw.write("</ul>\n");
  }
}

void UmlItem::chapter(QCString k, QCString pfix, unsigned int rank, QCString kind, unsigned int level) {
  if (rank != 0) {
    if (level > 4)
      level = 4;
    
    fw.write("<h");
    fw.write(level);
    if (!kind.isEmpty()) {
      fw.write(" class =\"");
      fw.write(kind);
      fw.write("\">");
    }
    else
      fw.write(">");

    fw.write(pfix);
    fw.write(rank);
    fw.write(' ');
    fw.write(k);
    fw.write(' ');
    writeq(name());
    fw.write("</h");
    fw.write(level);
    fw.write(">\n");
  }
}

const char * UmlItem::bypass_comment(const char * s) {
  while (*s) {
    if (*s <= ' ')
      s += 1;
    else if ((*s == '/') && (s[1] != 0)) {
      if (s[1] == '/') {
	do 
	  s += 1;
	while ((*s != 0) &&
	       (*s != '\n') && (*s != '\r'));
      }
      else if (s[1] == '*') {
	const char * s2 = strstr(s + 2, "*/");
	
	if (s2 != 0)
	  s = s2 + 2;
	else
	  return s;
      }
      else
	return s;
    }
    else if (*s == '#') {
      do {
	if ((*++s == 0) ||
	    ((*s == '\\') && (*++s == 0)))
	  return s;
      } while ((*s != '\n') && (*s != '\r'));
    }
    else
      return s;
  }

  return s;
}

void UmlItem::manage_alias(const char *& p) {
  // p starts by '@'
  const char * pclosed;
  
  if ((p[1] == '{') && ((pclosed = strchr(p + 2, '}')) != 0)) {
    QCString key(p + 2, pclosed - p - 1);
    QCString value;
    UmlItem * node = this;

    do {
      if (node->propertyValue(key, value))
	break;
      node = node->parent();
    } while (node != 0);
    
    if (node != 0)
      // find, insert the value
      writeq(value);
    else {
      // not find, insert the key
      fw.write("@{");
      writeq(key);
      fw.write("}");
    }

    // bypass the key
    p += strlen(key) + 3;
  }
  else {
    // bypass '@'
    fw.write("@");
    p += 1;
  }
}

void UmlItem::write() {
  if (known) {
    fw.write("<a href=\"");
    if (!flat && (parent() != 0) && (parent()->kind() == aClass)){
      fw.write("class");
      fw.write((unsigned) parent()->getIdentifier());
    }
    else
      fw.write("index");
    fw.write(".html#ref");
    fw.write((unsigned) kind());
    fw.write('_');
    fw.write((unsigned) getIdentifier());
    fw.write("\"><b>");
    writeq(pretty_name());
    fw.write("</b></a>");
  }
  else
    writeq(name());
}

void UmlItem::write(QCString target) {
  if (known) {
    fw.write("<a href=\"index.html#ref");
    fw.write((unsigned) kind());
    fw.write('_');
    fw.write((unsigned) getIdentifier());
    fw.write("\" target = \"");
    fw.write(target);
    fw.write("\"><b>");
    writeq(pretty_name());
    fw.write("</b></a>");
  }
  else
    writeq(name());
}

void UmlItem::writeq(QCString s)
{
  const char * p = s;
  
  while (*p)
    writeq(*p++);
}

void UmlItem::writeq(char c)
{
 switch (c) {
 case '<':
   fw.write("&lt;");
   break;
 case '>':
   fw.write("&gt;");
   break;
 case '&':
   fw.write("&amp;");
   break;
 case '@':
   fw.write("&#64;");
   break;
 case '\n':
   fw.write("<br />");
   break;
 case '\r':
   break;
 default:
   fw.write(c);
   break;
 }
}

void UmlItem::write(const UmlTypeSpec & t, aLanguage lang)
{
  if (t.type != 0)
    t.type->write();
  else
    switch (lang) {
    case cppLanguage:
      writeq(CppSettings::type(t.toString()));
      break;
    case javaLanguage:
      writeq(JavaSettings::type(t.toString()));
      break;
    default:
      writeq(t.toString());
  }
}

void UmlItem::write(const UmlTypeSpec & t)
{
  if (t.type != 0)
    t.type->write();
  else
    writeq(t.toString());
}

void UmlItem::write(aVisibility v, aLanguage lang)
{
  switch (v) {
  case PublicVisibility:
    fw.write("public");
    break;
  case ProtectedVisibility:
    fw.write("protected");
    break;
  case PrivateVisibility:
    fw.write("private");
    break;
  case PackageVisibility:
    switch (lang) {
    case cppLanguage:
      fw.write("public");
      break;
    case javaLanguage:
      fw.write("package");
    default:
      break;
    }
    break;
  default:
    fw.write("???");
  }
}

void UmlItem::write(aVisibility v)
{
  switch (v) {
  case PublicVisibility:
    fw.write("+ ");
    break;
  case ProtectedVisibility:
    fw.write("# ");
    break;
  case PrivateVisibility:
    fw.write("- ");
    break;
  default:
    // PackageVisibility:
    fw.write("~ ");
  }
}

void UmlItem::write(aDirection d)
{
  switch (d) {
  case InputOutputDirection:
    fw.write("input output");
    break;
  case InputDirection:
    fw.write("input");
    break;
  case OutputDirection:
    fw.write("output");
    break;
  case ReturnDirection:
    fw.write("return");
    break;
  default:
    fw.write("???");
  }
}

void UmlItem::write(aParameterEffectKind d)
{
  switch (d) {
  case noEffect:
    fw.write("none");
    break;
  case createEffect:
    fw.write("create");
    break;
  case readEffect:
    fw.write("read");
    break;
  case updateEffect:
    fw.write("update");
    break;
  case deleteEffect:
    fw.write("delete");
    break;
  default:
    fw.write("???");
  }
}

void UmlItem::write(anOrdering d)
{
  switch (d) {
  case unordered:
    fw.write("unordered");
    break;
  case ordered:
    fw.write("ordered");
    break;
  case lifo:
    fw.write("lifo");
    break;
  case fifo:
    fw.write("fifo");
    break;
  default:
    fw.write("???");
  }
}

void UmlItem::generate_index(Vector & v, QCString k, QCString r)
{
  int n = v.size();
  
  if (n != 0) {
    sort(v);
    
    start_file(r, k + " Index", TRUE);
    
    fw.write("<table>\n");
    
    for (int i = 0; i != n; i += 1) {
      UmlItem * x = v.elementAt(i);
      
      fw.write("<tr bgcolor=\"#f0f0f0\"><td>");
      x->write("projectFrame");
      fw.write("</td><td>");
      writeq(x->stereotype());
      fw.write("</td><td>");
      writeq(x->description());
      fw.write("</td></tr>\n");
    }
    fw.write("</table>\n");
    
    end_file();
  }
}

void UmlItem::sort(Vector & v)
{
  sort(v, 0, v.size() - 1);
}

void UmlItem::sort(Vector & v, int low, int high)
{
  if (low < high) {
    int lo = low;
    int hi = high + 1;
    UmlItem * e = v.elementAt(low);
    
    for (;;) {
      while ((++lo < hi) && !v.elementAt(lo)->gt(e))
	;
      while (v.elementAt(--hi)->gt(e));
	;
      
      if (lo < hi) {
	UmlItem * x = v.elementAt(lo);
	
	v.setElementAt(v.elementAt(hi), lo);
	v.setElementAt(x, hi);
      }
      else
	break;
    }
    
    UmlItem * x = v.elementAt(low);
	
    v.setElementAt(v.elementAt(hi), low);
    v.setElementAt(x, hi);
    
    sort(v, low, hi - 1);
    sort(v, hi + 1, high);
  }
}

bool UmlItem::gt(UmlItem * other) {
 QCString s1 = pretty_name();
 QCString s2 = other->pretty_name();
 int i = qstricmp((const char *) s1, (const char *) s2);
 
 if ((i == 0) && (parent() != 0) && (other->parent() != 0)) {
   s1 = parent()->pretty_name();
   s2 = other->parent()->pretty_name();
   
   return (qstricmp((const char *) s1, (const char *) s2) > 0);
 }
 else
  return (i > 0);
}

QCString UmlItem::pretty_name() {
  return name().isEmpty() ? sKind() : name();
}

Vector UmlItem::all;

FileWriter UmlItem::fw;

QCString UmlItem::directory;

unsigned int UmlItem::nrefs= 0;

QCString UmlItem::letters;

//true => use SVG picture rather than PNG
bool UmlItem::flat;

//true => classes and tables are generated in index.html, else an own files are generated
bool UmlItem::svg;

