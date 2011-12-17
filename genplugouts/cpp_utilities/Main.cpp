#include <qapplication.h>

#include "UmlCom.h"
#include "UmlItem.h"

// the program is called with the socket port number in argument

int main(int argc, char ** argv)
{
  if (argc != 2)
    return 0;
  
  QApplication a(argc, argv);
  
  if (UmlCom::connect(QString(argv[1]).toUInt())) {
    UmlCom::trace("<b>Class utilities</b> release 1.1.2<hr>");

    try {
      UmlCom::targetItem()->utilities();
    }
    catch (...) {
    }

    // must be called to cleanly inform that all is done
    UmlCom::bye();
  }
  
  UmlCom::close();
  return 0;
}
