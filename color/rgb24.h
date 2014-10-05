#ifndef SFC_COLOR_RGB24_H
#define SFC_COLOR_RGB24_H

#include <array>

#include "rgb.h"
#include "convert.h"

namespace color
{

class RGB24;

/** \brief colorRepresentation_traits specialization for RGB24 colors **/
template<>
struct colorRepresentation_traits<RGB24>
{
  static constexpr uint8_t storage_bit_size = 24;
};

template<>
struct RgbBase_traits<RGB24>
{
  typedef uint8_t channel_storage_type;
  typedef std::array<channel_storage_type, 3> storage_type;
  typedef channel::Proxy<channel_storage_type, 0, 8> proxy;
  typedef channel::Proxy<const channel_storage_type, 0, 8> const_proxy;
  typedef proxy r_proxy;
  typedef const_proxy const_r_proxy;
  typedef proxy g_proxy;
  typedef const_proxy const_g_proxy;
  typedef proxy b_proxy;
  typedef const_proxy const_b_proxy;
};


class RGB24 : public RgbBase<RGB24>
{
  public:
    RGB24()
    {
    }

    RGB24(const uint8_t& r, const uint8_t& g, const uint8_t& b)
    {
      data_[0] = r;
      data_[1] = g;
      data_[2] = b;
    }

    template <typename From>
    RGB24(const From& from)
    {
      convert(*this, from);
    }

    typedef RgbBase_traits<RGB24> traits;

    typedef typename traits::storage_type storage_type;

    SFC_COLOR_RGB_IMPORT_PROXIES(traits);

    const_r_proxy R() const {return const_r_proxy(data_[0]);}

    r_proxy R() {return r_proxy(data_[0]);}

    const_g_proxy G() const {return const_g_proxy(data_[1]);}

    g_proxy G() {return g_proxy(data_[1]);}

    const_b_proxy B() const {return const_b_proxy(data_[2]);}

    b_proxy B() {return b_proxy(data_[2]);}

    void Set()
    {
      data_.fill(0xFF);
    }

    void Clear()
    {
      data_.fill(0);
    }

    template <typename From>
    RGB24& operator=(const From& from)
    {
      convert(*this, from);
      return *this;
    }

  private:
    storage_type data_;
};

}

#endif // SFC_COLOR_RGB24_H
