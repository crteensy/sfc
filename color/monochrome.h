#ifndef SFC_COLOR_MONOCHROME_H
#define SFC_COLOR_MONOCHROME_H

#include "grayscale.h"

namespace color
{

/** \brief Monochrome color class (a specialized Grayscale with a width of 1 bit)
**/
typedef Grayscale<1> Monochrome;

} // namespace color

#endif // SFC_COLOR_MONOCHROME_H
