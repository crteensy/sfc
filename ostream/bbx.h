#ifndef SFC_OSTREAM_BBX_H
#define SFC_OSTREAM_BBX_H

#include "point.h"

template<typename Layer>
std::ostream& operator<<(std::ostream& o, const Bbx<Layer>& bbx)
{
  o << "Bbx[" << bbx.p0 << ", " << bbx.p1 << "]";
  return o;
}

#endif // SFC_OSTREAM_BBX_H
