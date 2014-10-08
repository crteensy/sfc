#ifndef SFC_PAGEBUFFER_H
#define SFC_PAGEBUFFER_H

#include <iostream>

#include "../color/rgb24.h"
#include "../geo/bbx.h"
#include "ColorBuffer.h"
#include "PixelMapping.h"

/** \brief traits class for default page buffers.
 * \tparam Display the Display type for which a page buffer is created
**/
template <typename Display>
struct default_pageBuffer_traits
{
  /** \brief The color type is imported from the display **/
  typedef typename Display::color_t color_t;

  /** \brief The default number of pages is 4 **/
  static constexpr size_t pages = 4;
};


/** \brief Page Buffer class for a Frontend and a Display
 * \tparam Display the Display type for which a page buffer is created
 * \tparam Frontend the frontend type for which a page buffer is created
**/
template <typename Display, typename Frontend>
struct pageBuffer_traits : public default_pageBuffer_traits<Display>
{
};


/** \brief A Generic Page Buffer
 *
 * The Page Buffer is created by an \ref OutputDispatcher for a canvas that draws on a buffered display.
 * \tparam Display the Display type for which a page buffer is created
 * \tparam Frontend the frontend type for which a page buffer is created
**/
template <typename Display, typename Frontend>
class PageBuffer
{
  public:
    // Display concept types
    typedef typename Display::coordinate_t coordinate_t;
    typedef Point<Display> point_t;
    typedef Bbx<Display> bbx_t;
    typedef typename pageBuffer_traits<Display, Frontend>::color_t color_t;
    static constexpr coordinate_t width = Display::width;
    static constexpr coordinate_t height = Display::height;

    static constexpr size_t pages = pageBuffer_traits<Display, Frontend>::pages;
    static_assert(((height % pages) == 0), "PageBuffer: Device height must be integer divisable by the number of pages.");
    static constexpr coordinate_t pageHeight = height/pages;
    static constexpr size_t pixelsPerPage = width*pageHeight;
//    static constexpr size_t bytesPerPage = (color::colorRepresentation_traits<color_t>::storage_bit_size*pixelsPerPage)/8;
    static constexpr size_t maxPixelsPerChunk = pageBuffer_traits<Display, Frontend>::maxPixelsPerChunk;

    typedef ColorBuffer<Display, Frontend, pixelsPerPage, maxPixelsPerChunk> color_buffer_t;

    PageBuffer()
      : bbx_(point_t(0,height-pageHeight),point_t(width-1, height-1))
    {
    }

//    void resetPage()
//    {
//      bytesLeftInPage_ = bytesPerPage;
//    }

    void beginFrame()
    {
      bbx_ = bbx_t(point_t(0,0),point_t(width-1, pageHeight-1));
      buffer_.frontend().fill(color_t(color::RGB24(color::Grayscale<8>(0))));
//      resetPage();
    }

    bool ready()
    {
      return false;
//      return (display().ready() && (bytesLeftInPage_ == 0));
    }

    const uint8_t* makeChunk(const size_t& offset, const size_t& size)
    {
      return buffer_.makeChunk(offset, size);
    }

    /** \brief do something - whatever is necessary
    **/
    void update()
    {
      std::cout << "PageBuffer::update() : updating display\n";
//      display().update();
//      if (display().ready())
//      {
//        std::cout << "PageBuffer::update() : display is ready\n";
//        if (bytesLeftInPage_)
//        {
//          writeChunk();
//        }
//        else if (advance())
//        {
//          writeChunk();
//        }
//      }
    }

    /** \brief overall dimensions of the device area **/
//    point_t dimensions() const {return display_.dimensions();}

    /** \brief current bounding box **/
    bbx_t bbx() {return bbx_;}

    /** \brief draw a pixel, at the given point, with the given color
     * \param p where to draw the pixel
     * \param c what color the pixel should have
     * \return true if the pixel could be drawn, i.e. if it was in the current bounding box
    **/
    bool drawPixel(const point_t& p, const color_t& c)
    {
      if (bbx_.contains(p))
      {
        buffer_.frontend()[mapPixel(p)] = c;
        return true;
      }
      return false;
    }

    /** \brief read a pixel at the given point
     * \param p where to read
     * \return the color at the given point, or a default-constructed color if p was outside the current bounding box.
    **/
    color_t readPixel(const point_t& p) const
    {
      if(bbx_.contains(p))
      {
        return buffer_.frontend()[mapPixel<Display>(p)];
      }
      std::cout << "read out of range\n";
      return color_t();
    }

    bool advance()
    {
      if (bbx_.bottom() == (height-1))
      {
        return false;
      }
      bbx_.p0 += point_t(0, pageHeight);
      bbx_.p1 += point_t(0, pageHeight);
//      resetPage();
      return true;

    }

//    Display& display()
//    {
//      return display_;
//    }
//
  private:
    bbx_t bbx_;
//    size_t bytesLeftInPage_;
//    Display& display_;

    color_buffer_t buffer_;
};

#endif // SFC_PAGEBUFFER_H

