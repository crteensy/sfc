#ifndef SFC_COLOR_RGB_H
#define SFC_COLOR_RGB_H

#include "colorRepresentation.h"
#include "channel.h"

namespace color
{

/** \brief Class that describes properties of a specific RGB representation **/
template <typename ColorSpace>
struct RgbBase_traits;


/** \brief Base class for RGB colors (CRTP)
 *
 * \tparam A color space (like RGB24 and RGB565, colorSpace_traits<ColorSpace> must be defined)
 */
template<typename ColorSpace>
class RgbBase
{
  public:
    SFC_COLOR_RGB_IMPORT_PROXIES(RgbBase_traits<ColorSpace>);

    /** \brief return a proxy for the red channel */
    r_proxy r()
    {
      return static_cast<ColorSpace*>(this)->R();
    }

    /** \brief return a const_proxy for the red channel */
    const_r_proxy r() const
    {
      return static_cast<const ColorSpace*>(this)->R();
    }

    /** \brief return a proxy for the green channel */
    g_proxy g()
    {
      return static_cast<ColorSpace*>(this)->G();
    }

    /** \brief return a const_proxy for the green channel */
    const_g_proxy g() const
    {
      return static_cast<const ColorSpace*>(this)->G();
    }

    /** \brief return a proxy for the blue channel */
    b_proxy b()
    {
      return static_cast<ColorSpace*>(this)->B();
    }

    /** \brief return a const_proxy for the blue channel */
    const_b_proxy b() const
    {
      return static_cast<const ColorSpace*>(this)->B();
    }

    /** \brief Set all channels to full value (all bits set) **/
    void set()
    {
      static_cast<ColorSpace*>(this)->Set();
    }

    /** \brief Set all channels to zero (all bits cleared) **/
    void clear()
    {
      static_cast<ColorSpace*>(this)->Clear();
    }

  private:
};

} // namespace color

#endif // SFC_COLOR_RGB_H

