#ifndef _UMLCOM_H
#define _UMLCOM_H

#define APIVERSION 55

#include <qsocketdevice.h>
#include "CmdFamily.h"
#include "OnInstanceCmd.h"
#include "anItemKind.h"
#include "aRelationKind.h"
#include <qcstring.h>

#include <qvector.h>
#include <qstring.h>

class UmlItem;
class UmlTypeSpec;
class UmlClass;

class QSocketDevice;
class UmlItem;
class UmlTypeSpec;
class UmlClass;
// This class manages the communications
//
// This class may be defined as a 'singleton', but I prefer to use static 
// members allowing to just write 'UmlCom::member' rather than
// 'UmlCom::instance()->member' or other long sentence like this.
//
// The operation you can use yourself are :
//
// - connect()
//
// - targetItem()
//
// - trace()
//
// - showTrace()
//
// - traceAutoRaise()
//
// - message()
//
// - bye(exit value)
//
// - close()
// 
// you must NOT call the others

class UmlCom {
  public:
    // does the connexion
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    static bool connect(unsigned int port);
    //  returns the item on which the tool is applied
    static UmlItem * targetItem();

    // to write messages in the tool window,
    // use Qt rich text : allows HTML like formatting
    static void trace(const char * s);
    //to show the trace window
    
    static void showTrace();

    //to automatically raise the trace window
    //each time trace() is called
    
    static void traceAutoRaise(bool y);

    // to write a message in the status bar line
    // does not use Qt rich text
    static void message(const char * s);
    // must be called just before the disconnexion
    static void bye(int);
    // disconnexion
    static void close();

  private:
    static QSocketDevice * sock;

    static char * buffer_in;

    static unsigned int buffer_in_size;

    static char * p_buffer_in;

    static char * buffer_in_end;

    static char * buffer_out;

    static char * p_buffer_out;

    static unsigned int buffer_out_size;


  protected:
    //internal, do NOT use it
    
    static void check_size_out(unsigned int len);
    //internal, do NOT use it
    
    static void read_if_needed();
    //internal, do NOT use it
    
    static void read_buffer(unsigned int len);
    //internal, do NOT use it
    
    static void write_bool(bool b);
    //internal, do NOT use it
    
    static void write_char(char c);
    //internal, do NOT use it
    
    static void write_unsigned(unsigned int u);
    //internal, do NOT use it
    
    static void write_id(const void * id);
    //internal, do NOT use it
    
    static void write_string(const char * p);

  public:
    // internal, do NOT use it
    
    static void send_cmd(CmdFamily f, unsigned int cmd);
    //internal, do NOT use it
    
    static void send_cmd(CmdFamily f, unsigned int cmd, char arg);
    //internal, do NOT use it
    
    static void send_cmd(CmdFamily f, unsigned int cmd, int arg, const char * dummy);
    //internal, do NOT use it
    
    static void send_cmd(CmdFamily f, unsigned int cmd, void * id);
    //internal, do NOT use it
    
    static void send_cmd(CmdFamily f, unsigned int cmd, const char * s);
    //internal, do NOT use it
    
    static void send_cmd(CmdFamily f, unsigned int cmd, const char * s, bool b);
    //internal, do NOT use it
    
    static void send_cmd(CmdFamily f, unsigned int cmd, void * id, const char * n);
    //internal, do NOT use it
    
    static void send_cmd(CmdFamily f, unsigned int cmd, const char * s, const char * v);

    //internal, do NOT use it
    
    static void send_cmd(CmdFamily f, unsigned int cmd, bool b, const char * s, const char * v);

    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd);
    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd, const char * arg);
    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd, char arg);
    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd, unsigned int arg);
    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd, const UmlTypeSpec & arg);
    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd, const char * arg1, const char * arg2);
    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd, anItemKind arg1, const char * arg2);
    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd, anItemKind arg1, aRelationKind arg2, const void * id2);
    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd, const void * id1);
    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd, const void * id1, const char * arg2);
    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd, unsigned int arg1, const UmlTypeSpec & arg2);
    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd, unsigned int arg1, const char * arg2, const char * arg3, const UmlTypeSpec & arg4, const UmlTypeSpec & arg5);
    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd, unsigned int arg1, char arg2, const char * arg3, const char * arg4, const UmlTypeSpec & arg5);
    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd, const QVector<UmlItem> & l);
    static void send_cmd(const void * id, OnInstanceCmd cmd, const QVector<UmlClass> & l1, const QVector<UmlClass> & l2, const QVector<UmlClass> & l3);

    //internal, do NOT use it
    
    static void send_cmd(const void * id, OnInstanceCmd cmd, anItemKind arg, const void * id2);

    //internal, do NOT use it
    
    static void * read_id();
    //internal, do NOT use it
    
    static const char * read_string();
    //internal, do NOT use it
    
    static bool read_bool();
    //internal, do NOT use it
    
    static char read_char();
    //internal, do NOT use it
    
    static unsigned int read_unsigned();
    // internal, do NOT use it
    
    static void read_item_list(QVector<UmlItem> & v);
    //internal, do NOT use it
    
    static void fatal_error(const QCString & msg);
    //internal, do NOT use it
    
    static void flush();
    
    //
    
    static void send_cmd(CmdFamily f, unsigned int cmd, unsigned int u, char c);
    
    static unsigned user_id();
    static void set_user_id(unsigned uid);
    static bool set_root_permission(bool);
};

#endif
