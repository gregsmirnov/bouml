#include <qapplication.h>

#include "UmlCom.h"
#include "UmlItem.h"

// the program is called with the socket port number in argument

int main(int argc, char ** argv)
{
  QApplication a(argc, argv);
  QString file;
 
  switch (argc) {
  case 2:
    break;
  case 3:
    file = argv[1];
    break;
  default:
    return 0;
  }

  if (UmlCom::connect(QString(argv[argc - 1]).toUInt())) {
    int r = 0;

    try {
      UmlCom::trace("<b>Xmi 2.1 import</b> release 1.9<br>");
      UmlCom::targetItem()->import(file);
    }
    catch (...) {
      r = 1;
    }

    // must be called to cleanly inform that all is done
    UmlCom::bye(1);
  }
  
  UmlCom::close();
  return 0;
}
