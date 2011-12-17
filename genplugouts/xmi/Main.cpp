#include <qapplication.h>

#include "UmlCom.h"
#include "UmlItem.h"

// the program is called with the socket port number in argument

int main(int argc, char ** argv)
{
  if (argc < 2)
    return 0;
  
  QApplication a(argc, argv);
  
  if (UmlCom::connect(QString(argv[argc - 1]).toUInt())) {
    try {
      // warning : update release number in UmlPackage::xmi !
      UmlCom::trace("<b>Xmi generator</b> release 1.4.6<br>");
      UmlCom::targetItem()->xmi(argc - 2, argv + 1);
    }
    catch (...) {
    }

    // must be called to cleanly inform that all is done
    UmlCom::bye();
  }
  
  UmlCom::close();
  return 0;
}
