#ifndef _UMLCOM_H
#define _UMLCOM_H


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
// - message()
//
// - bye()
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
    // to write a message in the status bar line
    // does not use Qt rich text
    static void message(const char * s);
    // must be called just before the disconnexion
    static void bye();
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
    static void check_size_out(unsigned int len);
    static void read_if_needed();
    static void read_buffer(unsigned int len);
    static void write_bool(bool b);
    static void write_char(char c);
    static void write_unsigned(unsigned int u);
    static void write_id(const void * id);
    static void write_string(const char * p);

  public:
    // do NOT call the followings yourself !!!!
    static void send_cmd(CmdFamily f, unsigned int cmd);
    static void send_cmd(CmdFamily f, unsigned int cmd, char arg);
    static void send_cmd(CmdFamily f, unsigned int cmd, void * id);
    static void send_cmd(CmdFamily f, unsigned int cmd, const char * s);
    static void send_cmd(CmdFamily f, unsigned int cmd, void * id, const char * n);
    static void send_cmd(CmdFamily f, unsigned int cmd, const char * s, const char * v);

    static void send_cmd(CmdFamily f, unsigned int cmd, bool b, const char * s, const char * v);

    static void send_cmd(const void * id, OnInstanceCmd cmd);
    static void send_cmd(const void * id, OnInstanceCmd cmd, const char * arg);
    static void send_cmd(const void * id, OnInstanceCmd cmd, char arg);
    static void send_cmd(const void * id, OnInstanceCmd cmd, unsigned int arg);
    static void send_cmd(const void * id, OnInstanceCmd cmd, const UmlTypeSpec & arg);
    static void send_cmd(const void * id, OnInstanceCmd cmd, const char * arg1, const char * arg2);
    static void send_cmd(const void * id, OnInstanceCmd cmd, anItemKind arg1, const char * arg2);
    static void send_cmd(const void * id, OnInstanceCmd cmd, anItemKind arg1, aRelationKind arg2, const void * id2);
    static void send_cmd(const void * id, OnInstanceCmd cmd, const void * id1);
    static void send_cmd(const void * id, OnInstanceCmd cmd, const void * id1, const char * arg2);
    static void send_cmd(const void * id, OnInstanceCmd cmd, unsigned int arg1, const UmlTypeSpec & arg2);
    static void send_cmd(const void * id, OnInstanceCmd cmd, unsigned int arg1, const char * arg2, const char * arg3, const UmlTypeSpec & arg4);
    static void send_cmd(const void * id, OnInstanceCmd cmd, unsigned int arg1, char arg2, const char * arg3, const char * arg4, const UmlTypeSpec & arg5);
    static void send_cmd(const void * id, OnInstanceCmd cmd, const QVector<UmlClass> & l);
    static void * read_id();
    static const char * read_string();
    static bool read_bool();
    static char read_char();
    static unsigned int read_unsigned();
    // reads the selected items when the tool is called
    // you MUST call it only one time after the connexion
    static void read_item_list(QVector<UmlItem> & v);
    static void fatal_error(const QCString & msg);
    static void flush();
};

#endif
