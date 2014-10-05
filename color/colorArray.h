#ifndef SFC_COLORARRAY_H
#define SFC_COLORARRAY_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <iterator>

#include "../color/colorRepresentation.h"

/** \file colorArray.h Color Container (Array) classes

 * A ColorArray is an array of, well, colors. However, the color classes occupy very different
 * amounts of space, and might need to be packed (for example 2 4-bit grayscale objects should
 * go into one single byte of storage. The ColorArray class examines the given color and chooses
 * between a std::array<Color,Size> and a PackedColorArray<Color,Size> depending on what the color
 * class needs.
**/

namespace color
{

template <typename Color>
struct PackedColorArray_traits
{
  typedef uint8_t storage_type;
  typedef storage_type value_type;
};


/** \brief A packed array of colors that are smaller than the array's underlying storage type.
 *
 * Colors that have a colorRepresentation_traits<Color>::storage_bit_size of less than 8 bits may
 * require "packing" so that no space is unused between the color elements. This array class does
 * just that. It provides proxies that manipulate an individual color object and iterators that
 * facilitate the use of STL algorithms, such as std::copy and std::generate.
 * \tparam Bits the number of bits of each element
 * \tparam Elements the number of elements
 *
 */
template<typename Color, size_t Elements>
class PackedColorArray
{
  public:
    static constexpr uint8_t Bits = color::colorRepresentation_traits<Color>::storage_bit_size;
    typedef Color color_type;
    typedef typename PackedColorArray_traits<Color>::storage_type storage_type;
    typedef typename PackedColorArray_traits<Color>::value_type value_type;

    /** \brief number if bits in the underlying storage type */
    static constexpr size_t value_bitwidth = 8*sizeof(value_type);
    static_assert(Bits <= value_bitwidth, "Bit size must not be greater than the underlying storage type's bit size");
    static_assert(value_bitwidth % Bits == 0, "Cannot pack this : (value_bitwidth % Bits per element) != 0");

    /** \brief example: for Bits = 3, arg_mask = 0b111 (right aligned) */
    static constexpr value_type arg_mask = (1<< Bits)-1;

    /** \brief number of underlying storage elements */
    static constexpr size_t storage_size = (Bits*Elements+(value_bitwidth-1))/value_bitwidth;

    /** \brief Accesses a set of Bits bits in the underlying data of a PackedColorArray.

     * What the created proxy manipulates in a \ref PackedColorArray with Bits = 2, Elements = 8, V = uint8_t:
     * \code
     *
     *      +--------+--------+
     *  LSB |data_[0]|data_[1]|
     *      +--------+--------+
     *         ^^
     *         ||
     *         ++---- proxy(data_[0],2) accesses these bits (2..3) when assignment/conversion operators are called
     *                const_proxy only provides read access
     * \endcode
     * \tparam isConst whether this is a constant or non-constant version of the proxy class.
     */


    template<bool isConst>
    class proxyT : public Color
    {
      public:
        typedef Color color_type;
        typedef PackedColorArray::storage_type storage_base_type;
        typedef typename std::conditional<isConst,
                                            const storage_base_type,
                                            storage_base_type>::type storage_type;

        typedef typename std::conditional<Bits == 1,
                                          bool,
                                          PackedColorArray::value_type>::type value_base_type;

        typedef typename std::conditional<isConst,
                                          const value_base_type,
                                          value_base_type>::type value_type;

        value_type read() const;

      protected:
        /** \brief Creates a proxy for the bits at data : offset
         * \param data reference to the underlying storage
         * \param offset first bit to access in the underlying storage
         *
         */
        proxyT(storage_type& data, size_t offset);

        storage_type& data_; /**< reference to the underlying storage */

        size_t offset_; /**< first bit to access in the underlying storage */
    };


    /** \brief See \ref proxyT and \ref const_proxy
     */
    class proxy : public proxyT<false>
    {
      public:
        /** \brief See \ref proxyT::proxyT() for the constructor description
         */
        proxy(value_type& data, size_t offset);

        void write(const value_type& value);

        template<typename C>
        proxy& operator=(const C& color);

        proxy& operator=(const proxy& other);

        ~proxy();
    };


    /** \brief see \ref proxyT and \ref proxy
    **/
    class const_proxy : public proxyT<true>
    {
      public:
        const_proxy(const value_type& data, size_t offset);
    };


    /** \brief reference type to refer to container elements */
    typedef proxy reference;

    /** \brief pointer to the underlying storage array */
    value_type* data();

    /** \brief const pointer to the underlying storage array */
    const value_type* data() const;

    /** \brief convenience method to clear the underlying storage */
    void clear();

    /** \brief element access
     * \param i element index
     */
    proxy operator[](size_t i);

    /** \brief const element access
     * \param i element index
     */
    const_proxy operator[](size_t i) const;


    /** \brief Random access iterator base class, template for const vs non-const access
     *
     * \tparam isConst sets constness if true, and non-constness otherwise.
     */
    template<bool isConst>
    class iteratorT
    {
      public:
        typedef std::random_access_iterator_tag iterator_category;

        /** \brief the value type is a const or non-const proxy object **/
        typedef typename std::conditional<isConst, const_proxy, proxy>::type value_type;

        /** \brief the storage type is the same as that for the underlying container, but const-qualified if necessary **/
        typedef typename std::conditional<isConst, const PackedColorArray::value_type, PackedColorArray::value_type>::type storage_type;

        typedef value_type& reference;

        typedef value_type* pointer;

        typedef ptrdiff_t difference_type;

        /** \brief Constructor
         *
         * \param data pointer to a storage_type array
         * \param i index of the element this iterator should refer to after construction
         */
        iteratorT(storage_type* data, size_t i);

        // Input iterator, Forward Iterator
        iteratorT& operator++(); /**< \brief prefix increment */

        iteratorT operator++(int); /**< \brief postfix increment */

        template <bool b>
        bool operator==(const iteratorT<b>& rhs) const;

        template <bool b>
        bool operator!=(const iteratorT<b>& rhs) const;

        value_type operator*() const;

        // Bidirectional iterator
        iteratorT& operator--(); /**< \brief prefix decrement */

        iteratorT operator--(int); /**< \brief postfix decrement */

        // Random Access Iterator
        iteratorT& operator+=(size_t n);

        iteratorT operator+(size_t n) const;

        iteratorT& operator-=(size_t n);

        iteratorT operator-(size_t n) const;

        // Difference
        typename iteratorT::difference_type operator-(const iteratorT& rhs) const;

        // More comparison operators

      private:
        storage_type* data_;
        size_t i_;
    };


    typedef iteratorT<false> iterator;

    typedef iteratorT<true> const_iterator;

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

    const std::array<storage_type, storage_size>& array() const
    {
      return data_;
    }

    std::array<storage_type, storage_size>& array()
    {
      return data_;
    }

  private:
    std::array<storage_type, storage_size> data_;
};
#include "colorArray.inl"


template <typename Color, size_t Size, bool pack>
struct ColorArrayT;


template <typename Color, size_t Size>
struct ColorArrayT<Color, Size, true> : public PackedColorArray<Color, Size>
{
};


template <typename Color, size_t Size>
struct ColorArrayT<Color, Size, false> : public std::array<Color, Size>
{
  std::array<Color, Size>& array()
  {
    return *this;
  }
};


template <typename Color, size_t Size>
struct ColorArray : public ColorArrayT<Color, Size,
  ((color::colorRepresentation_traits<Color>::storage_bit_size % 8 != 0)
   && (8*sizeof(typename Color::storage_type) / color::colorRepresentation_traits<Color>::storage_bit_size >= 1))>
{
};

} // namespace color

#endif // SFC_COLORARRAY_H
