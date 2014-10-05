#ifndef SFC_COLOR_CHANNEL_H
#define SFC_COLOR_CHANNEL_H

namespace color
{

/** \brief Handling of color channels is done using proxy objects. This is their namespace.
*/
namespace channel
{

/** \brief imports proxy typedefs from an RGB color class.
It works not only for RgbBase, but for others with these proxy definitions as well.
**/
#define SFC_COLOR_RGB_IMPORT_PROXIES(T)         \
  typedef typename T::r_proxy r_proxy;              \
  typedef typename T::const_r_proxy const_r_proxy;  \
  typedef typename T::g_proxy g_proxy;              \
  typedef typename T::const_g_proxy const_g_proxy;  \
  typedef typename T::b_proxy b_proxy;              \
  typedef typename T::const_b_proxy const_b_proxy;  \


/** \brief Tag for extracting channel data left aligned. There is also a \c left_aligned object you can
  passto methods as an actual argument.
**/
struct left_aligned_t
{
};

static constexpr left_aligned_t left_aligned = left_aligned_t();


/** \brief Tag for extracting channel data right aligned There is also a \c right_aligned object you can
  passto methods as an actual argument.
**/
struct right_aligned_t
{
};

static constexpr right_aligned_t right_aligned = right_aligned_t();


/** \brief data access proxy class that can write to or read from a number of contiguous bits in a value
  \tparam Storage the underlying data storage type (typically uint8_t or similar)
  \tparam Offset_ the bit offset where the accessed bits start in the underyling storage
  \tparam Width_ the number of bits accessed by this proxy
**/
template<typename Storage, unsigned int Offset_, unsigned int Width_>
class Proxy
{
  public:
    /* Some checks and typedefs */
    static_assert(std::is_unsigned<Storage>::value, "ChannelProxy: T must be an unsigned integer type.");
    typedef Storage storage_type;
    typedef Storage value_type;

    /** \brief the number of bits accessed by this proxy
    **/
    static constexpr unsigned int Width = Width_;
    static_assert(Width <= 8, "ChannelProxy: Width must be <= 8.");

    /** \brief the bit offset where the accessed bits start in the underyling storage
    **/
    static constexpr unsigned int Offset = Offset_;
    static_assert((Offset + Width) <= 8*sizeof(storage_type),
                  "ChannelProxy: Channel is out of the storage type's bounds. \
                  Check data type, offset and width.");

    Proxy(value_type& data) : data_(data) {}

    value_type read(right_aligned_t) const
    {
      return ((data_ & read_mask) >> Offset);
    }

    value_type read(left_aligned_t) const
    {
      return read(right_aligned) << (8-Width);
    }

    /** \brief Read from the proxy, using default (right) alignment
    **/
    value_type read() const
    {
      return read(right_aligned);
    }

    operator value_type() const
    {
      return read();
    }

    void write(const value_type& value, right_aligned_t)
    {
      data_ = (data_ & clear_data_mask) | ((value & value_mask) << Offset);
    }

    void write(const value_type& value, left_aligned_t)
    {
      write(value >> (8-Width), right_aligned);
    }

    /** \brief Write to the proxy, using default (right) alignment
    **/
    void write(const value_type& value)
    {
      write(value, channel::right_aligned);
    }

    /** \brief Assign a value to the proxy, using write() with default (right) alignment
    **/
    Proxy& operator=(const value_type& value)
    {
      write(value);
      return *this;
    }

  private:
    static constexpr value_type value_mask = (value_type)((1<<Width)-1);
    static constexpr storage_type read_mask = (value_mask << Offset);
    static constexpr storage_type clear_data_mask = (storage_type)~read_mask;
    storage_type& data_;
};

} // namespace channel
} // namespace color

#endif // SFC_COLOR_CHANNEL_H

