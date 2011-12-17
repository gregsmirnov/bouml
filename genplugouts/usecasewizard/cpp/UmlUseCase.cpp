
#include <qapplication.h>

#include "UmlCom.h"
#include "TabDialog.h"
#include "UmlUseCase.h"

void UmlUseCase::wizard() {
  char ** argv = 0;
  
  int argc = 0;
  
  QApplication a(argc, argv);

  TabDialog tabdialog(this);
  
  tabdialog.resize(600, 300);
  a.setMainWidget( &tabdialog );
  tabdialog.show();
  
  a.exec();
  
  UmlCom::bye();	// application must not be deleted
  UmlCom::close();	// application must not be deleted
  
  throw 0;
}

