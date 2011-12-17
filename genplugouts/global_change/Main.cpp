#include <qapplication.h>

#include "UmlCom.h"
#include "UmlItem.h"
#include "UmlComponent.h"
#include "UmlClass.h"
#include "Dialog.h"

// the program is called with the socket port number in argument

int main(int argc, char ** argv)
{
  if (argc != 2)
    return 0;

  QApplication a(argc, argv);
  
  if (UmlCom::connect(QString(argv[1]).toUInt())) {
    try {
      UmlCom::trace("<b>Global change</b> release 1.2.1<br>");
      
      Dialog dialog;

      dialog.exec();
    }
    catch (...) {
    }

    // must be called to cleanly inform that all is done
    UmlCom::bye();
  }
  
  UmlCom::close();
  return 0;
}
