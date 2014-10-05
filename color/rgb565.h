#ifndef SFC_COLOR_RGB565_H
#define SFC_COLOR_RGB565_H

#include "rgb.h"

namespace color
{

class RGB565;


/** \brief colorSpace_traits specialization for RGB565 colors **/
template<>
struct colorRepresentation_traits<RGB565>
{
  static constexpr uint8_t storage_bit_size = 16;
};

template<>
struct RgbBase_traits<RGB565>
{
  typedef uint16_t storage_type;
  static constexpr uint8_t storage_bit_size = 8;
  typedef channel::Proxy<storage_type, 0, 5> b_proxy;
  typedef channel::Proxy<const storage_type, 0, 5> const_b_proxy;
  typedef channel::Proxy<storage_type, 5, 6> g_proxy;
  typedef channel::Proxy<const storage_type, 5, 6> const_g_proxy;
  typedef channel::Proxy<storage_type, 11, 5> r_proxy;
  typedef channel::Proxy<const storage_type, 11, 5> const_r_proxy;
};

class RGB565 : public RgbBase<RGB565> // 16 bits: MSB | RRRRR GGGGGG BBBBB | LSB
{
  public:
    typedef RgbBase_traits<RGB565> traits;

    typedef typename traits::storage_type storage_type;

    SFC_COLOR_RGB_IMPORT_PROXIES(traits);

    RGB565()
    {
    }

    template <typename alignment_type = channel::right_aligned_t>
    RGB565(const uint8_t& r_, const uint8_t& g_, const uint8_t& b_,
           alignment_type = alignment_type())
    {
      alignment_type alignment;
      r().write(r_, alignment);
      g().write(g_, alignment);
      b().write(b_, alignment);
    }

    template <typename From>
    RGB565(const From& from)
    {
      convert(*this, from);
    }

    const_r_proxy R() const {return const_r_proxy(data_);}

    r_proxy R() {return r_proxy(data_);}

    const_g_proxy G() const {return const_g_proxy(data_);}

    g_proxy G() {return g_proxy(data_);}

    const_b_proxy B() const {return const_b_proxy(data_);}

    b_proxy B() {return b_proxy(data_);}

    void Set()
    {
      data_ = 0xFFFF;
    }

    void Clear()
    {
      data_ = 0;
    }

    template <typename From>
    RGB565& operator=(const From& from)
    {
      convert(*this, from);
      return *this;
    }

  protected:
    storage_type data_;
};

}
#endif // SFC_COLOR_RGB565_H
