#ifndef SFC_PIXELMAPPING_H
#define SFC_PIXELMAPPING_H

/** Default pixel mapping **/
template<typename Display>
struct LinearXYPixelMapping
{
  static size_t map(const Point<Display>& p)
  {
    return p.y()*Display::width + p.x();
  }
};


/** 8-bit vertical pixel mapping

Byte  0 1 ...
     +-+-+-...
 LSB |0|0|
     |1|1|
     |2|2|
     |3|3|
     |4|4|
     |5|5|
     |6|6|
 MSB |7|7|
     +-+-+-...
**/
template<typename Display>
struct Staggered8BitPixelMapping
{
  static size_t map(const Point<Display>& p)
  {
    uint8_t yOffset = p.y()%8;
    uint8_t yBank = p.y()/8;

    size_t result = 8*yBank*Display::width + 8*p.x() + yOffset;

    return result;
  }
};


/** Pixel Mapping class, can be specialized for a certain display type if required. By default, it maps from upper left, x->right, y->bottom
**/
template <typename Display>
struct PixelMapping : public LinearXYPixelMapping<Display>
{
};


/** \brief Maps a point (x,y) to an index in a pixel buffer.
 * \param p the point to map
 * \return the point's offset in a pixel buffer
**/
template <typename Display, typename = typename std::enable_if<std::is_unsigned<typename Display::coordinate_t>::value>::type>
size_t mapPixel(const Point<Display>& p)
{
  return PixelMapping<Display>::map(p);
}

#endif // SFC_PIXELMAPPING_H

