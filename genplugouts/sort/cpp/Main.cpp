
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
      // does something on the target, here suppose that a virtual
      // operation exist at UmlItem level (and probably sub_level !)
      UmlCom::trace("<b>Sort</b> release 1.3.1<br>");
      UmlCom::targetItem()->sort();
      UmlCom::trace("Done<br>");
    }
    catch (...) {
    }

    // must be called to cleanly inform that all is done
    UmlCom::bye();
  }
  
  UmlCom::close();
  return 0;
}
