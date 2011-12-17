
#include "UmlCom.h"
#include "UmlItem.h"
#include "UmlComponent.h"
#include "UmlClass.h"

// the program is called with the socket port number in argument

int main(int argc, char ** argv)
{
  if (argc != 2)
    return 0;
  
  if (UmlCom::connect(QString(argv[1]).toUInt())) {
    try {
      UmlCom::trace("<b>Deploy classes</b> release 1.1 (C++ version)<br>");
      UmlCom::targetItem()->deploy();
      UmlCom::trace("Done");
    }
    catch (...) {
    }

    // must be called to cleanly inform that all is done
    UmlCom::bye();
  }
  
  UmlCom::close();
  return 0;
}
