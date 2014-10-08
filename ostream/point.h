#ifndef SFC_OSTREAM_POINT_H
#define SFC_OSTREAM_POINT_H

template<typename Layer>
std::ostream& operator<<(std::ostream& o, const Point<Layer>& p)
{
  o << "P(" << (size_t)p.x() << "," << (size_t)p.y() << ")";
  return o;
}

#endif // SFC_OSTREAM_POINT_H

