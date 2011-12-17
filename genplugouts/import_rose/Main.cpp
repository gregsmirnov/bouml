
#include <qapplication.h>

#include "UmlCom.h"
#include "UmlItem.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "IdlSettings.h"

// the program is called with the socket port number in argument

int main(int argc, char ** argv)
{
  if (argc != 2)
    return 0;
  
  QApplication a(argc, argv);

  if (UmlCom::connect(QString(argv[1]).toUInt())) {
    UmlCom::trace("<b>Rose import</b> release 2.1.2 (import model only, without diagram contents)<hr>");

    bool cpp_dflt = CppSettings::useDefaults();
    bool java_dflt = JavaSettings::useDefaults();
    bool idl_dflt = IdlSettings::useDefaults();

    CppSettings::set_UseDefaults(FALSE);
    JavaSettings::set_UseDefaults(FALSE);
    IdlSettings::set_UseDefaults(FALSE);

    try {
      UmlCom::targetItem()->roseImport();
    }
    catch (...) {
    }

    CppSettings::set_UseDefaults(cpp_dflt);
    JavaSettings::set_UseDefaults(java_dflt);
    IdlSettings::set_UseDefaults(idl_dflt);

    UmlItem::statistic();
    UmlCom::trace("<br>Done");
    UmlCom::message("");

    // must be called to cleanly inform that all is done
    UmlCom::bye();
  }
  
  UmlCom::close();
  return 0;
}
