#ifndef SFC_COLOR_CONVERT_H
#define SFC_COLOR_CONVERT_H

#include "rgb.h"
#include "grayscale.h"
#include "monochrome.h"

namespace color
{

/** \brief converts between different color classes derived from RgbBase
  \tparam To the RGB space to convert fo
  \tparam From the RGB base to convert from
  \param to the instance to convert to
  \param from the instance to convert from
**/
template <typename To, typename From>
void convert(RgbBase<To>& to, const RgbBase<From>& from)
{
  to.r().write(from.r().read(channel::left_aligned), channel::left_aligned);
  to.g().write(from.g().read(channel::left_aligned), channel::left_aligned);
  to.b().write(from.b().read(channel::left_aligned), channel::left_aligned);
}

/** \brief converts between different Grayscale color classes
  \tparam To the grayscale depth to convert fo
  \tparam From the grayscale depth to convert from
  \param to the instance to convert to
  \param from the instance to convert from
**/
template <uint8_t To, uint8_t From>
void convert(Grayscale<To>& to, const Grayscale<From>& from)
{
  to.k().write(from.k().read(channel::left_aligned), channel::left_aligned);
}

/** \brief converts a Grayscale color to a color derived from RgbBase
  \tparam To the RGB space to convert fo
  \tparam From the grayscale depth to convert from
  \param to the instance to convert to
  \param from the instance to convert from
**/
template<typename To, uint8_t From>
void convert(RgbBase<To>& to, const Grayscale<From>& from)
{
  uint8_t k = from.k().read(channel::left_aligned);
  to.r().write(k, channel::left_aligned);
  to.g().write(k, channel::left_aligned);
  to.b().write(k, channel::left_aligned);
}

/** \brief converts a color derived from RgbBase color to a Grayscale color
  \tparam To the grayscale depth to convert from
  \tparam From the RGB space to convert fo
  \param to the instance to convert to
  \param from the instance to convert from
**/
template<uint8_t To, typename From>
void convert(Grayscale<To>& to, const RgbBase<From>& from)
{
  uint16_t k = ((uint16_t)(0.39*256.))*from.r().read(channel::left_aligned)
             + ((uint16_t)(0.59*256.))*from.g().read(channel::left_aligned)
             + ((uint16_t)(0.11*256.))*from.b().read(channel::left_aligned);
  to.k().write(k >> 8);
}

} // namespace color

#endif // SFC_COLOR_CONVERT_H

