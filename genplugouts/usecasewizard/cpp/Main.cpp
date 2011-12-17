
#include "UmlCom.h"
#include "UmlItem.h"

// the program is called with the socket port number in argument

int main(int argc, char ** argv)
{
  if (argc != 2)
    return 0;
  
  if (UmlCom::connect(QString(argv[1]).toUInt())) {
    try {
      UmlCom::trace("<b>Use case wizard</b> release 1.1.2 (C++ version)<br>");
      UmlCom::targetItem()->wizard();
    }
    catch (...) {
    }

    // must be called to cleanly inform that all is done
    UmlCom::bye();
  }
  
  UmlCom::close();
  return 0;
}
