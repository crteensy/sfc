#ifndef SFC_COLOR_GRAYSCALE_H
#define SFC_COLOR_GRAYSCALE_H

#include <type_traits>

#include "colorRepresentation.h"
#include "channel.h"

namespace color
{

template<uint8_t Bits>
class Grayscale;

/** \brief colorRepresentation_traits specialization for Grayscale colors

  \anchor colorRepresentation_traits_Grayscale
**/
template<>
template<uint8_t Bits>
struct colorRepresentation_traits<Grayscale<Bits> >
{
  static constexpr uint8_t storage_bit_size = Bits;
};


/** \brief represents a grayscale color model
  \tparam Bits the number of bits in the gray (k) channel
**/
template<uint8_t Bits>
class Grayscale
{
  public:
    typedef uint8_t storage_type;
    static_assert(Bits <= 8, "Grayscale<Bits>: Number of Bits must be <= 8");
    static_assert(Bits >= 1, "Grayscale<Bits>: Number of Bits must be > 1");

    typedef channel::Proxy<storage_type, 0, Bits> proxy;
    typedef channel::Proxy<const storage_type, 0, Bits> const_proxy;

    Grayscale() :data_(0)
    {
    }

    /** \brief Copy constructor for conversion between different grayscale models
    **/
    template <uint8_t FromBits>
    Grayscale(const Grayscale<FromBits>& from)
    {
      k().write(from.k().read(channel::left_aligned), channel::left_aligned);
    }

    /** \brief constructor for non-integer arguments (other colors)
    **/
    template <typename From,
              typename = typename std::enable_if<!std::is_integral<From>::value>::type>
    Grayscale(const From& from)
    {
      convert(*this, from);
    }

    /** \brief constructor for integer arguments
    **/
    template <typename From, typename alignment_type = channel::right_aligned_t,
              typename = typename std::enable_if<std::is_integral<From>::value>::type>
    Grayscale(const From& value, alignment_type alignment = alignment_type())
    {
      proxy p(data_);
      p.write(value, alignment);
    }

    const_proxy k() const
    {
      return const_proxy(data_);
    }

    proxy k()
    {
      return proxy(data_);
    }

    template<typename From>
    Grayscale& operator=(const From& from)
    {
      convert(*this, from);
      return *this;
    }

    storage_type data_;
};

} // namespace color

#endif // SFC_COLOR_GRAYSCALE_H

