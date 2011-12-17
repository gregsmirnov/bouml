#include <qapplication.h>

#include "UmlCom.h"
#include "UmlItem.h"
#include "UmlDiagram.h"
#include "UmlClass.h"

// the program is called with the socket port number in argument

int main(int argc, char ** argv)
{
  if (argc < 2)
    return 0;

  QApplication a(argc, argv);
  
  if (UmlCom::connect(QString(argv[argc - 1]).toUInt())) {
    try {
      UmlCom::trace("<b>Html generator</b> release 2.20 (C++ version)<br />");

      UmlItem * target = UmlCom::targetItem();

      target->set_dir(argc - 2, argv + 1);
      UmlCom::message("Memorize references ...");
      target->memo_ref();
      UmlItem::frame();
      UmlCom::message("Indexes ...");
      UmlItem::generate_indexes();
      UmlItem::start_file("index", target->name() + "\nDocumentation", FALSE);
      target->html(QCString(""), 0u, 0u);
      UmlItem::end_file();
      UmlItem::start_file("navig", "", TRUE);
      UmlItem::end_file();
      UmlClass::generate();
      UmlCom::trace("Done");
      UmlCom::message("");
    }
    catch (...) {
    }

    // must be called to cleanly inform that all is done
    UmlCom::bye(0);
  }
  
  UmlCom::close();
  return 0;
}
