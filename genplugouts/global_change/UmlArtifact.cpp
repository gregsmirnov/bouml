
#include "UmlArtifact.h"
#include "Context.h"

void UmlArtifact::change(Context & ctx) {
if (ctx.onArtifact() && ctx.match_stereotype(stereotype())) {
  if (ctx.cpp()) {
    const QCString & h = cppHeader();
    
    if (!h.isEmpty() && ctx.match(h)) {
      if (!set_CppHeader(ctx.replace(h)))
	ctx.err();
    }
    
    const QCString & c = cppSource();
    
    if (!h.isEmpty() && ctx.match(c)) {
      if (!set_CppSource(ctx.replace(c)))
	ctx.err();
    }
  }
  
  if (ctx.java()) {
    const QCString & c = javaSource();
    
    if (!c.isEmpty() && ctx.match(c)) {
      if (!set_JavaSource(ctx.replace(c)))
	ctx.err();
    }
  }
  
  if (ctx.php()) {
    const QCString & c = phpSource();
    
    if (!c.isEmpty() && ctx.match(c)) {
      if (!set_PhpSource(ctx.replace(c)))
	ctx.err();
    }
  }
  
  if (ctx.python()) {
    const QCString & c = pythonSource();
    
    if (!c.isEmpty() && ctx.match(c)) {
      if (!set_PythonSource(ctx.replace(c)))
	ctx.err();
    }
  }
  
  if (ctx.idl()) {
    const QCString & c = idlSource();
    
    if (!c.isEmpty() && ctx.match(c)) {
      if (!set_IdlSource(ctx.replace(c)))
	ctx.err();
    }
  }
}
}

