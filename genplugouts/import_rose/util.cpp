#include "qcstring.h"

#include "util.h"

QCString legalName(QCString s)
{
  for (unsigned index = 0; index != s.length(); index += 1) {
    char c = s.at(index);
    
    if ((c != '_') && 
	!((c >= 'a') && (c <= 'z')) &&
        !((c >= 'A') && (c <= 'Z')) &&
	!((c >= '0') && (c <= '9'))) {
      s.replace(index, 1, "__");
      index += 1;
    }
  }

  return s;
}

QCString replace(QCString f, QCString k, QCString v)
{
  int index = f.find(k);

  return (index != -1)
    ? f.replace(index, k.length(), v)
    : f;
}

bool isSep(int c)
{
  return ((c != '_') && 
	  !((c >= 'a') && (c <= 'z')) &&
          !((c >= 'A') && (c <= 'Z')) &&
	  !((c >= '0') && (c <= '9')));
}
