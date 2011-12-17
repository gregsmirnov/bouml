
#include "SmallPushButton.h"

QSize SmallPushButton::sizeHint() const {
  QFontMetrics fm = fontMetrics();
  QSize sz = fm.size(ShowPrefix, text());
  
  return QSize(sz.width() + sz.width()/8 + 4,
	       sz.height() + sz.height()/8 + 4);
}

