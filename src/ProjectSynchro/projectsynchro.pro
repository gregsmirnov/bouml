TEMPLATE      = app
CONFIG	      += qt warn_on release
HEADERS	      = BrowserNode.h BrowserView.h SynchroWindow.h SynchroDialog.h
SOURCES	      = main.cpp BrowserNode.cpp SynchroWindow.cpp \
		BrowserView.cpp SynchroDialog.cpp Pixmap.cpp myio.cpp 


TARGET	      = projectSynchro
DEFINES	      = BooL=bool
INCLUDEPATH   = 
