#ifndef SFC_COLORSPACE_H
#define SFC_COLORSPACE_H

namespace color
{
/** \brief Declaration of color representation properties

  This must be specialized for every class that represents a color space.
  The specialized traits class must specify the number of bits occupied by the color,
  as \c storage_bit_size.

  \see colorRepresentation_traits<RGB24>
  \see colorRepresentation_traits<RGB565>
  \see \ref colorRepresentation_traits_Grayscale "colorRepresentation_traits<Grayscale<Bits>>"

  This traits is currently rather pointless, and the whole system of Color
  Model/Space/Representation should be revised.
**/
template<typename Space>
struct colorRepresentation_traits;
} // namespace color

#endif // SFC_COLORSPACE_H

