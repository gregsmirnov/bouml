
#include "Context.h"

Context::Context(const QCString & o, const QCString & f)
  : _old(o), _future(f), _n_match(0), _n_err(0) {
}

const char * Context::set_filters(const QCString & f1, const QCString & f2, const QCString & f3, bool w1, bool w2, bool w3, bool and12, bool and23) {
_filter1 = f1;
_filter2 = f2;
_filter3 = f3;
_with1 = w1;
_with2 = w2;
_with3 = w3;

if (! _filter1.isEmpty()) {
  if (! _filter2.isEmpty()) {
    if (! _filter3.isEmpty()) {
      if (and12)
	_equation = (and23) ? And12_And23 : And12_Or23;
      else 
	_equation = (and23) ? Or12_And23 : Or12_Or23;
    }
    else
      _equation = (and12) ? And12 : Or12;
  }
  else if (! _filter3.isEmpty())
    return "filter2 is empty but not filter3";
  else
    _equation = Filter1;
}
else if (! _filter2.isEmpty())
  return "filter1 is empty but not filter2";
else if (! _filter3.isEmpty())
  return "filter1 is empty but not filter3";
else 
  _equation = NoFilter;

return 0;
}

bool Context::match_stereotype(const QCString & s) {
  if (_is)
    return (s == _stereotype);
  else if (_isnot)
    return (s != _stereotype);
  else
    return TRUE;
}

bool Context::match(QCString s) {
  int index = 0;
  
  while ((index = s.find("\r", index)) != -1)
    s.remove(index, 1);
  
  if (s.find(_old) == -1)
    return FALSE;
  
  switch (_equation) {
  case NoFilter:
    _n_match += 1;
    return TRUE;
  case Filter1:
    return match(s, _filter1, _with1);
  case And12:
    return match(s, _filter1, _with1) && match(s, _filter2, _with2);
  case Or12:
    return match(s, _filter1, _with1) || match(s, _filter2, _with2);
  case And12_And23:
    return match(s, _filter1, _with1) && match(s, _filter2, _with2) && match(s, _filter3, _with3);
  case And12_Or23:
    return match(s, _filter1, _with1) && match(s, _filter2, _with2) || match(s, _filter3, _with3);
  case Or12_And23:
    return match(s, _filter1, _with1) || match(s, _filter2, _with2) && match(s, _filter3, _with3);
  default:
    //Or12_Or23:
    return match(s, _filter1, _with1) || match(s, _filter2, _with2) || match(s, _filter3, _with3);
  }
}

void Context::err() {
  _n_err += 1;
}

QCString Context::replace(QCString s) {
  int index = 0;
  
  while ((index = s.find("\r", index)) != -1)
    s.remove(index, 1);
  
  return s.replace(s.find(_old), _old.length(), _future);
}

bool Context::match(QCString s, const QCString & filter, bool with) {
if ((with) ? (s.find(filter) != -1) : (s.find(filter) == -1)) {
  _n_match += 1;
  return TRUE;
}

return FALSE;
}

