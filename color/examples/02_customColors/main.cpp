/**
  COMPILE WITH:
  g++ -Os -std=c++0x -o 02_customColors main.cpp
  in the sfc/color/examples/02_customColors directory.
**/

#include <iostream>

#include "../../color.h"
#include "../../../ostream/color.h"

/** Forward declare your new color class, this on is derived from RgbBase **/
class RGB111;

/** Define RgbBase_traits for the new color. This must be done in the color namespace **/
namespace color
{

template<>
struct colorRepresentation_traits<RGB111>
{
  static constexpr uint8_t storage_bit_size = 4;
};
template<>
struct RgbBase_traits<RGB111>
{
  typedef uint8_t storage_type;
  typedef channel::Proxy<storage_type, 0, 1> b_proxy;
  typedef channel::Proxy<const storage_type, 0, 1> const_b_proxy;
  typedef channel::Proxy<storage_type, 1, 1> g_proxy;
  typedef channel::Proxy<const storage_type, 1, 1> const_g_proxy;
  typedef channel::Proxy<storage_type, 2, 1> r_proxy;
  typedef channel::Proxy<const storage_type, 2, 1> const_r_proxy;
};

}

/** Now define how your class behaves. **/
class RGB111 : public color::RgbBase<RGB111>
{
  public:
    // these are shortcuts
    typedef color::RgbBase_traits<RGB111> traits;
    SFC_COLOR_RGB_IMPORT_PROXIES(traits);

    typedef typename traits::storage_type storage_type;

    RGB111()
      : data_(0)
    {
    }

    template <typename alignment_type = color::channel::right_aligned_t>
    RGB111(const uint8_t& r_, const uint8_t& g_, const uint8_t& b_,
           alignment_type = alignment_type())
      : data_(0)
    {
      alignment_type alignment;
      r().write(r_, alignment);
      g().write(g_, alignment);
      b().write(b_, alignment);
    }

    const_r_proxy R() const {return const_r_proxy(data_);}

    r_proxy R() {return r_proxy(data_);}

    const_g_proxy G() const {return const_g_proxy(data_);}

    g_proxy G() {return g_proxy(data_);}

    const_b_proxy B() const {return const_b_proxy(data_);}

    b_proxy B() {return b_proxy(data_);}

    void Set()
    {
      data_ = 0b111;
    }

    void Clear()
    {
      data_ = 0;
    }

    template <typename From>
    RGB111(const From& from)
    {
      convert(*this, from);
    }

    template <typename From>
    RGB111& operator=(const From& from)
    {
      convert(*this, from);
      return *this;
    }

    operator bool() const
    {
      return data_ != 0;
    }

  private:
    storage_type data_;
};


using namespace std;

int main()
{
  RGB111 rgb(1,0,1);
  cout << "rgb = RGB111(1,0,1): rgb = " << rgb << "\n\n";

  rgb.r() = 0;
  cout << "rgb.r() = 0: rgb = " << rgb << "\n\n";

  cout << "Conversions happen automatically:\n\n";

  color::RGB24 rgb24(rgb);
  cout << "RGB24 rgb24(rgb): rgb24 = " << rgb24 << "\n";

  rgb24.r() = 0xFF;
  cout << "rgb24.r() = 0xFF: rgb24 = " << rgb24 << "\n";

  rgb = rgb24;
  cout << "rgb = rgb24: rgb = " << rgb << "\n";

  return 0;
}
