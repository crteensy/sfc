#ifndef SFC_OSTREAM_COLOR_H
#define SFC_OSTREAM_COLOR_H

#include <iostream>

#include "../color/monochrome.h"
#include "../color/rgb.h"
#include "../color/colorArray.h"

template<typename T, unsigned int Offset_, unsigned int Width_>
std::ostream& operator<<(std::ostream& o, const color::channel::Proxy<T, Offset_, Width_>& p)
{
  o << (unsigned int)p.read(color::channel::right_aligned);
  return o;
}

template<typename D>
std::ostream& operator<<(std::ostream& o, const color::RgbBase<D>& c)
{
  o << "RGB(" << c.r()
    << ", " << c.g()
    << ", " << c.b() << ")";
  return o;
}

template<uint8_t Bits>
std::ostream& operator<<(std::ostream& o, const color::Grayscale<Bits>& g)
{
  o << "Gray(" << g.k() << ")";
  return o;
}

std::ostream& operator<<(std::ostream& o, const color::Monochrome& g)
{
  o << "Mono(" << (bool)g.k() << ")";
  return o;
}

template<typename Color, size_t Size>
std::ostream& operator<<(std::ostream& o, color::ColorArray<Color, Size>& a)
{
  o << "ColorArray[\n";
  for(auto it = a.begin(); it != a.end(); ++it)
  {
    o << "  " << *it << "\n";
  }
  o << "]";
  return o;
}

#endif // SFC_OSTREAM_COLOR_H
