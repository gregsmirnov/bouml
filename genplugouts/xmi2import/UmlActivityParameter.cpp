
#include "UmlActivityParameter.h"
#include "FileIn.h"
#include "Token.h"

void UmlActivityParameter::readParameter(FileIn & in, Token & token) {
  QCString s;

  s = token.valueOf("direction");
  if ((s == "in") || (s == "pk_in"))
    set_Direction(InputDirection);
  else if ((s == "out") || (s == "pk_out"))
    set_Direction(OutputDirection);
  else if ((s == "inout") || (s == "pk_inout"))
    set_Direction(InputOutputDirection);
  else if ((s == "return") || (s == "pk_return"))
    set_Direction(ReturnDirection);
  else if (! s.isEmpty())
    in.warning("wrong direction");

  if (!(s = token.valueOf("effect")).isEmpty())
    setEffect(s, in);
  if (token.valueOf("isunique") == "true")
    set_IsUnique(TRUE);
  if (token.valueOf("isexception") == "true")
    set_IsException(TRUE);
  if (token.valueOf("isstream") == "true")
    set_IsStream(TRUE);
  if (!(s = token.valueOf("type")).isEmpty())
    setType(s);

  if (!token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
      
    while (in.read(), !token.close(kstr)) {
      s = token.what();
	
      if (s == "type") {
	setType(token);
	if (! token.closed())
	  in.finish(s);
      }
      else if (s == "lowervalue")
	importMultiplicity(in, token, FALSE);
      else if (s == "uppervalue")
	importMultiplicity(in, token, TRUE);
      else if (s == "defaultvalue") {
	set_DefaultValue(token.valueOf("value"));
	if (! token.closed())
	  in.finish(s);
      }
      else if (s == "upperbound") {
	if (! token.closed())
	  in.finish(s);
      }
      else
	UmlItem::import(in, token);
    }
  }
}

void UmlActivityParameter::readParameterNode(FileIn & in, Token & token) {
  QCString s;

  if (token.valueOf("iscontroltype") == "true")
    set_IsControlType(TRUE);
  if (!(s = token.valueOf("ordering")).isEmpty())
    setOrdering(s, in);
  if (!(s = token.valueOf("instate")).isEmpty())
    setInState(s);
  if (!(s = token.valueOf("selection")).isEmpty())
    setSelection(s);
  if (!(s = token.valueOf("type")).isEmpty())
    setType(s);
    
  if (!token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
      
    while (in.read(), !token.close(kstr)) {
      s = token.what();
      
      if (s == "selection") {
	setSelection(token.xmiIdref());
	if (! token.closed())
	  in.finish(s);
      }
      else if (s == "instate") {
	setInState(token.xmiIdref());
	if (! token.closed())
	  in.finish(s);
      }
      else if (s == "type") {
	setType(token);
	if (! token.closed())
	  in.finish(s);
      }
      else
	UmlItem::import(in, token);
    }
  }
}

void UmlActivityParameter::setEffect(QCString s, FileIn & in) {
  if (s == "create")
    set_Effect(createEffect);
  else if (s == "read")
    set_Effect(readEffect);
  else if (s == "update")
    set_Effect(updateEffect);
  else if (s == "delete")
    set_Effect(deleteEffect);
  else
    in.warning("illegal effect '" + s + "'");
}

