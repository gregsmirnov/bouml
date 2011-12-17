#include <qapplication.h>

#include "UmlCom.h"
#include "UmlItem.h"
#include "UmlComponent.h"
#include "UmlClass.h"

// the program is called with the socket port number in argument
// and a second argument which must be "ci" (check-in) or "co"

int main(int argc, char ** argv)
{
  if (argc != 3)
    return 0;
  
  QApplication a(argc, argv);

  if (UmlCom::connect(QString(argv[2]).toUInt())) {
    try {
      bool ci = (strcmp(argv[1], "ci") == 0);

      UmlCom::trace((ci) ? "<b>file control</b> release 1.0.6, check-in"
			 : "<b>file control</b> release 1.0.6, check-out");
      UmlCom::targetItem()->fileControl(ci);
    }
    catch (...) {
    }

    // must be called to cleanly inform that all is done
    UmlCom::bye();
  }
  
  UmlCom::close();
  return 0;
}
