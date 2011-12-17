
#include "UmlCom.h"

#include <qsocketdevice.h> 

#include "UmlCom.h"
#include "UmlItem.h"
#include "UmlTypeSpec.h"
#include "UmlClass.h"
#include "UmlSettings.h"
#include "MiscGlobalCmd.h"
bool UmlCom::connect(unsigned int port)
{
  sock = new QSocketDevice(QSocketDevice::Stream);
  sock->setAddressReusable(TRUE);
  
  buffer_in_size = 1024;
  buffer_in = new char[buffer_in_size];
  buffer_in_end = p_buffer_in = 0;
  
  buffer_out_size = 1024;
  buffer_out = new char[buffer_out_size];
  p_buffer_out = buffer_out + 4/*bytes for length*/;
  
  QHostAddress ha;
  
  ha.setAddress("127.0.0.1");
  
  if (sock->connect(ha, port)) {
    // send API version
    write_unsigned(13);
    flush();
    return TRUE;
  }
  else
    return FALSE;
}

UmlItem * UmlCom::targetItem()
{
  send_cmd(miscGlobalCmd, targetCmd);
  return UmlBaseItem::read_();
}

void UmlCom::trace(const char * s)
{
  send_cmd(miscGlobalCmd, traceCmd, s);
}

void UmlCom::message(const char * s)
{
  send_cmd(miscGlobalCmd, messageCmd, s);
}

void UmlCom::bye()
{
  send_cmd(miscGlobalCmd, byeCmd);
}

void UmlCom::close()
{
  delete sock;
  sock = 0;
}

QSocketDevice * UmlCom::sock;

char * UmlCom::buffer_in;

unsigned int UmlCom::buffer_in_size;

char * UmlCom::p_buffer_in;

char * UmlCom::buffer_in_end;

char * UmlCom::buffer_out;

char * UmlCom::p_buffer_out;

unsigned int UmlCom::buffer_out_size;

void UmlCom::check_size_out(unsigned int len)
{
  unsigned used = p_buffer_out - buffer_out;
  
  if ((used + len) >= buffer_out_size) {
    buffer_out_size = used + len + 1024;
    
    char * newbuff = new char[buffer_out_size];
    
    memcpy(newbuff, buffer_out, used);
    delete [] buffer_out;
    
    p_buffer_out = (buffer_out = newbuff) + used;
  }
}

void UmlCom::read_if_needed()
{
#ifdef TRACE
  //cout << "UmlCom::read_if_needed " << buffer_in_end - p_buffer_in << '\n';
#endif
  
  if (p_buffer_in == buffer_in_end) {
    read_buffer(4);
#ifdef TRACE
    cout << "UmlCom::read " << ((((unsigned char *) buffer_in)[0] << 24) + (((unsigned char *) buffer_in)[1] << 16) +(((unsigned char *) buffer_in)[2] << 8) +((unsigned char *) buffer_in)[3]) << " bytes\n";
#endif
    read_buffer((((unsigned char *) buffer_in)[0] << 24) +
		(((unsigned char *) buffer_in)[1] << 16) +
		(((unsigned char *) buffer_in)[2] << 8) +
		((unsigned char *) buffer_in)[3]);
  }
}

void UmlCom::read_buffer(unsigned int len)
{
#ifdef TRACE
  //cout << "enter UmlCom::read_buffer(" << len << ")\n";
#endif
  
  if (buffer_in_size < len) {
    delete [] buffer_in;
    buffer_in_size = len + 256;
    buffer_in = new char[buffer_in_size];
  }
  
  int remainder = (int) len;
  int nread;
  char * p = buffer_in;
  
  for (;;) {
    if ((nread = sock->readBlock(p, remainder)) == -1) {
      if (sock->error() != 0) {
#ifdef TRACE
	cout << "UmlCom::read_buffer ERROR, already " << p - buffer_in
	  << " remainder " << remainder << '\n';
#endif
	fatal_error("UmlCom read error");
      }
      else
	nread = 0;
    }
    
#ifdef TRACE
    //cout << "UmlCom a lu " << nread << '\n';
#endif
    if ((remainder -= nread) == 0)
      break;
    p += nread;
    sock->waitForMore(100);
  }
#ifdef TRACE
  //cout << "exit UmlCom::read_buffer()\n";
#endif
  
  p_buffer_in = buffer_in;
  buffer_in_end = buffer_in + len;
}

void UmlCom::write_bool(bool b)
{
  check_size_out(1);
  
  *p_buffer_out++ = (b == 0) ? 0 : 1;
}

void UmlCom::write_char(char c)
{
  check_size_out(1);
  
  *p_buffer_out++ = c;
}

void UmlCom::write_unsigned(unsigned int u)
{
  check_size_out(4);
  
  p_buffer_out[0] = u >> 24;
  p_buffer_out[1] = u >> 16;
  p_buffer_out[2] = u >> 8;
  p_buffer_out[3] = u;
  
  p_buffer_out += 4;
}

void UmlCom::write_id(const void * id)
{
  check_size_out(sizeof(void *));
  
  memcpy(p_buffer_out, &id, sizeof(void *));
  p_buffer_out += sizeof(void *);
}

void UmlCom::write_string(const char * p)
{
  if (p == 0)
    p = "";
  
  unsigned len = strlen(p) + 1;
  
  check_size_out(len);
  memcpy(p_buffer_out, p, len);
  p_buffer_out += len;
}

void UmlCom::send_cmd(CmdFamily f, unsigned int cmd)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd((CmdFamily) " << f << ", " << cmd << ")\n";
#endif
  
  write_char(f);
  write_char(cmd);
  flush();
}

void UmlCom::send_cmd(CmdFamily f, unsigned int cmd, char arg)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd((CmdFamily) " << f << ", " << cmd << ", " << (int) arg << ")\n";
#endif
  
  write_char(f);
  write_char(cmd);
  write_char(arg);
  flush();
}

void UmlCom::send_cmd(CmdFamily f, unsigned int cmd, void * id)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd((CmdFamily) " << f << ", " << cmd << ", " << id << ")\n";
#endif
  
  write_char(f);
  write_char(cmd);
  write_id(id);
  flush();
}

void UmlCom::send_cmd(CmdFamily f, unsigned int cmd, const char * s)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd((CmdFamily) " << f << ", " << cmd << ", " << ((s) ? s : "") << ")\n";
#endif
  
  write_char(f);
  write_char(cmd);
  write_string(s);
  flush();
}

void UmlCom::send_cmd(CmdFamily f, unsigned int cmd, void * id, const char * n)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd((CmdFamily) " << f << ", " << cmd << ", " << id << ", " << ((n) ? n : "") << ")\n";
#endif
  
  write_char(f);
  write_char(cmd);
  write_id(id);
  write_string(n);
  flush();
}

void UmlCom::send_cmd(CmdFamily f, unsigned int cmd, const char * s, const char * v)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd((CmdFamily) " << f << ", " << cmd << ", " << ((s) ? s : "") << ", " << ((v) ? v : "") << ")\n";
#endif
  
  write_char(f);
  write_char(cmd);
  write_string(s);
  write_string(v);
  flush();
}

void UmlCom::send_cmd(CmdFamily f, unsigned int cmd, bool b, const char * s, const char * v)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd((CmdFamily) " << f << ", " << cmd << ", " << b << ", " << ((s) ? s : "") << ", " << ((v) ? v : "") << ")\n";
#endif
  
  write_char(f);
  write_char(cmd);
  write_bool(b);
  write_string(s);
  write_string(v);
  flush();
}

void UmlCom::send_cmd(const void * id, OnInstanceCmd cmd)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd(id, " << cmd << ")\n";
#endif
  
  write_char(onInstanceCmd);
  write_id(id);
  write_char(cmd);
  flush();
}

void UmlCom::send_cmd(const void * id, OnInstanceCmd cmd, const char * arg)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd(id, " << cmd << ", " << ((arg) ? arg : "") << ")\n";
#endif
  
  write_char(onInstanceCmd);
  write_id(id);
  write_char(cmd);
  write_string(arg);
  flush();
}

void UmlCom::send_cmd(const void * id, OnInstanceCmd cmd, char arg)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd(id, " << cmd << ", " << ((int) arg) << '\n';
#endif
  
  write_char(onInstanceCmd);
  write_id(id);
  write_char(cmd);
  write_char(arg);
  flush();
}

void UmlCom::send_cmd(const void * id, OnInstanceCmd cmd, unsigned int arg)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd(id, " << cmd << ", " << arg << '\n';
#endif
  
  write_char(onInstanceCmd);
  write_id(id);
  write_char(cmd);
  write_unsigned(arg);
  flush();
}

void UmlCom::send_cmd(const void * id, OnInstanceCmd cmd, const UmlTypeSpec & arg)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd(id, " << cmd << ", UmlTypeSpec)\n";
#endif
  
  write_char(onInstanceCmd);
  write_id(id);
  write_char(cmd);
  if (arg.type) {
    write_id(arg.type->_identifier);
    write_string("");
  }
  else {
    write_id(0);
    write_string(arg.explicit_type);
  }
  flush();
}

void UmlCom::send_cmd(const void * id, OnInstanceCmd cmd, const char * arg1, const char * arg2)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd(id, " << cmd << ", " << ((arg1) ? arg1 : "") << ", " << ((arg2) ? arg2 : "") << ")\n";
#endif
  
  write_char(onInstanceCmd);
  write_id(id);
  write_char(cmd);
  write_string(arg1);
  write_string(arg2);
  flush();
}

void UmlCom::send_cmd(const void * id, OnInstanceCmd cmd, anItemKind arg1, const char * arg2)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd(id, " << cmd << ", " << arg1 << ", " << ((arg2) ? arg2 : "") << ")\n";
#endif
  
  write_char(onInstanceCmd);
  write_id(id);
  write_char(cmd);
  write_char(arg1);
  write_string(arg2);
  flush();
}

void UmlCom::send_cmd(const void * id, OnInstanceCmd cmd, anItemKind arg1, aRelationKind arg2, const void * id2)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd(id, " << cmd << ", " << arg1 << ", " << arg2 << ", " << id2 << ")\n";
#endif
  
  write_char(onInstanceCmd);
  write_id(id);
  write_char(cmd);
  write_char(arg1);
  write_char(arg2);
  write_id(id2);
  flush();
}

void UmlCom::send_cmd(const void * id, OnInstanceCmd cmd, const void * id1)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd(id, " << cmd << ", id1)\n";
#endif
  
  write_char(onInstanceCmd);
  write_id(id);
  write_char(cmd);
  write_id(id1);
  flush();
}

void UmlCom::send_cmd(const void * id, OnInstanceCmd cmd, const void * id1, const char * arg2)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd(id, " << cmd << ", id1, " << ((arg2) ? arg2 : "") << ")\n";
#endif
  
  write_char(onInstanceCmd);
  write_id(id);
  write_char(cmd);
  write_id(id1);
  write_string(arg2);
  flush();
}

void UmlCom::send_cmd(const void * id, OnInstanceCmd cmd, unsigned int arg1, const UmlTypeSpec & arg2)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd(id, " << cmd << ", " << arg1 << ", UmlTypeSpec)\n";
#endif
  
  write_char(onInstanceCmd);
  write_id(id);
  write_char(cmd);
  write_unsigned(arg1);
  if (arg2.type) {
    write_id(arg2.type->_identifier);
    write_string("");
  }
  else {
    write_id(0);
    write_string(arg2.explicit_type);
  }
  flush();
}

void UmlCom::send_cmd(const void * id, OnInstanceCmd cmd, unsigned int arg1, const char * arg2, const char * arg3, const UmlTypeSpec & arg4)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd(id, " << cmd << ", " << arg1 << ", " << ((arg2) ? arg2 : "") << ", " << ((arg3) ? arg3 : "") << ", " << ", UmlTypeSpec)\n";
#endif
  
  write_char(onInstanceCmd);
  write_id(id);
  write_char(cmd);
  write_unsigned(arg1);
  write_string(arg2);
  write_string(arg3);
  if (arg4.type) {
    write_id(arg4.type->_identifier);
    write_string("");
  }
  else {
    write_id(0);
    write_string(arg4.explicit_type);
  }
  flush();
}

void UmlCom::send_cmd(const void * id, OnInstanceCmd cmd, unsigned int arg1, char arg2, const char * arg3, const char * arg4, const UmlTypeSpec & arg5)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd(id, " << cmd << ", " << arg1 << ", " << (int) arg2 << ", " << ((arg3) ? arg3 : "") << ", " << ((arg4) ? arg4 : "") << ", UmlTypeSpec)\n";
#endif
  
  write_char(onInstanceCmd);
  write_id(id);
  write_char(cmd);
  write_unsigned(arg1);
  write_char(arg2);
  write_string(arg3);
  write_string(arg4);
  if (arg5.type) {
    write_id(arg5.type->_identifier);
    write_string("");
  }
  else {
    write_id(0);
    write_string(arg5.explicit_type);
  }
  flush();
}

void UmlCom::send_cmd(const void * id, OnInstanceCmd cmd, const QVector<UmlClass> & l)
{
#ifdef TRACE
  cout << "UmlCom::send_cmd(id, " << cmd << ", const QVector<UmlClass> & l)\n";
#endif
  
  write_char(onInstanceCmd);
  write_id(id);
  write_char(cmd);
  
  unsigned n = l.count();
  
  write_unsigned(n);
  
  for (unsigned i = 0; i != n; i += 1)
    write_id(((UmlBaseItem *) l[i])->_identifier);
  
  flush();
}

void * UmlCom::read_id()
{
  read_if_needed();
  
  void * a;
  
  // sizeof(void *) must be the same for bouml and
  // the plug-out, bypass it
  memcpy((char *) &a, p_buffer_in + 1, sizeof(void *));
  p_buffer_in += sizeof(void *) + 1;
  return a;
}

const char * UmlCom::read_string()
{
  read_if_needed();
  
  unsigned len = strlen(p_buffer_in) + 1;
  
  p_buffer_in += len;
  
#ifdef TRACE
  //cout << "UmlCom::read_string : \"" << p_buffer_in - len << "\"\n";
#endif
  
  return p_buffer_in - len;
}

bool UmlCom::read_bool()
{
  read_if_needed();
  
  return *p_buffer_in++ != 0;
}

char UmlCom::read_char()
{
  read_if_needed();
  
  return *p_buffer_in++;
}

unsigned int UmlCom::read_unsigned()
{
  read_if_needed();
  
  p_buffer_in += 4;
  
  return (((unsigned char *) p_buffer_in)[-4] << 24) +
    (((unsigned char *) p_buffer_in)[-3] << 16) +
      (((unsigned char *) p_buffer_in)[-2] << 8) +
	((unsigned char *) p_buffer_in)[-1];
}

void UmlCom::read_item_list(QVector<UmlItem> & v)
{
  unsigned n = read_unsigned();
  
  v.resize(n);
  
#ifdef TRACE
  //cout << "UmlCom::read_item_list " << n << " items\n";
#endif
  
  for (unsigned index = 0; index != n; index += 1)
    v.insert(index, UmlBaseItem::read_());
}

void UmlCom::fatal_error(const QCString & 
#ifdef DEBUG_BOUML
                         msg
#endif
                         )
{
#ifdef DEBUG_BOUML
  cout << msg << '\n';
#endif

  throw 0;
}

void UmlCom::flush()
{
  if (sock != 0) {
    int len = p_buffer_out - buffer_out - 4;
    /* the four first bytes of buffer_out are free to contains the length */
    buffer_out[0] = len >> 24;
    buffer_out[1] = len >> 16;
    buffer_out[2] = len >> 8;
    buffer_out[3] = len;

    len += 4;
    p_buffer_out = buffer_out;

    for (;;) {
      int sent = sock->writeBlock(p_buffer_out, len);

      if (sent == -1) {
	close();	// to not try to send "bye" !
	fatal_error("send error");
      }
      else if ((len -= sent) == 0) {
	sock->flush();
	p_buffer_out = buffer_out + 4/*bytes for length*/;
	return;
      }
      else
	p_buffer_out += sent;
    }      
  }
}

