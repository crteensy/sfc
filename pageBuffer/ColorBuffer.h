#ifndef SFC_COLORBUFFER_H
#define SFC_COLORBUFFER_H

#include <algorithm>

#include "../color/colorArray.h"

template <typename Display, typename Frontend, size_t Size, size_t ChunkSize, bool EqualColors>
class ColorBufferT;


/** \brief ColorBufferT specialized for equal color types in backend and frontend
 * \tparam Display the display type
 * \tparam Frontend the frontend type
 * \tparam Size the number of pixels in the buffer
**/
template <typename Display, typename Frontend, size_t Size, size_t ChunkSize>
class ColorBufferT<Display, Frontend, Size, ChunkSize, true>
{
  public:
    typedef color::ColorArray<typename Frontend::color_t, Size> array_type;
    typedef array_type frontend_array_type;
    typedef array_type backend_array_type;

    frontend_array_type& frontend()
    {
      return array_;
    }

    const frontend_array_type& frontend() const
    {
      return array_;
    }

    backend_array_type& backend()
    {
      return array_;
    }

    const backend_array_type& backend() const
    {
      return array_;
    }

    const uint8_t* makeChunk(const size_t& pixelOffset, const size_t& size)
    {
      std::cout << "ColorBuffer<sameTypes>::makeChunk(offset " << (size_t)pixelOffset << " , " << size << " bytes )\n";
      std::cout << "  storage size: " << (size_t)color::colorRepresentation_traits<typename Display::color_t>::storage_bit_size << "\n";
      size_t byteOffset = (color::colorRepresentation_traits<typename Display::color_t>::storage_bit_size*pixelOffset)/8;
      return (const uint8_t*)(array_.data())+byteOffset;
    }
  private:
    array_type array_;
};


/** \brief ColorBufferT specialized for different color types in display and frontend
 * \tparam Display the display type
 * \tparam Frontend the frontend type
 * \tparam Size the number of pixels in the buffer
**/
template <typename Display, typename Frontend, size_t Size, size_t ChunkSize>
class ColorBufferT<Display, Frontend, Size, ChunkSize, false>
{
  public:
    typedef color::ColorArray<typename Frontend::color_t, Size> frontend_array_type;

    typedef color::ColorArray<typename Display::color_t, ChunkSize> backend_array_type;

    frontend_array_type& frontend()
    {
      return frontendArray_;
    }

    const frontend_array_type& frontend() const
    {
      return frontendArray_;
    }

    backend_array_type& backend()
    {
      return outputArray_;
    }

    const backend_array_type& backend() const
    {
      return outputArray_;
    }

    const uint8_t* makeChunk(const size_t& pixelOffset, const size_t& size)
    {
      std::cout << "ColorBuffer<diffTypes>::makeChunk(offset " << (size_t)pixelOffset << " , " << size << " bytes )\n";
      std::copy_n(frontendArray_.begin() + pixelOffset, size, outputArray_.begin());
      return (const uint8_t*)(outputArray_.data());
    }

  private:
    frontend_array_type frontendArray_;
    backend_array_type outputArray_;
};


template<typename Display, typename Frontend, size_t Size, size_t ChunkSize>
class ColorBuffer : public ColorBufferT<Display, Frontend, Size, ChunkSize,
                                        std::is_same<typename Display::color_t,
                                        typename Frontend::color_t>::value>
{

};

#endif // SFC_COLORBUFFER_H

