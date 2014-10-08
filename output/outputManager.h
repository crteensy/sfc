#ifndef SFC_OUTPUTMANAGER_H
#define SFC_OUTPUTMANAGER_H

#include "../pageBuffer/PageBuffer.h"

namespace output_mode
{
  /** \brief direct drawing tag, no page buffer is used **/
  struct direct {};


  /** \brief buffered drawing tag, a \ref PageBuffer is used **/
  struct buffered {};
}


/** \brief Output traits class
 * Defines the output properties for the given Frontend and Display
 * \tparam D the Display class
 * \tparam F the Frontend class
**/
template <typename D, typename F>
struct output_traits
{
  typedef output_mode::buffered type;
};


/** \brief Output Dispatcher class
 * Dispatches drawing function either directly to the display or to an intermediate \ref PageBuffer
 * \tparam D the Display class
 * \tparam F the Frontend class
 * \tparam O the output mode tag (one of \ref outputmode::direct or \ref outputmode::buffered)
**/
template <typename D, typename F, typename O>
class OutputManager;


/** \brief Output Dispatcher for unbuffered displays
 * \tparam Display the Display class
 * \tparam Frontend the Frontend class
**/
template <typename Display, typename Frontend>
class OutputManager<Display, Frontend, output_mode::direct>
{
  public:

    typedef Display output_device_t;

    typedef Display display_t;

    OutputManager(Display& display)
      : display_(display)
    {
    }

    output_device_t& device()
    {
      return display_;
    }

    const output_device_t& device() const
    {
      return display_;
    }

    display_t& display()
    {
      return display_;
    }

  private:
    output_device_t& display_;
};


/** \brief Output Dispatcher for buffered displays
 * \tparam Display the Display class
 * \tparam Frontend the Frontend class
**/
template <typename Display, typename Frontend>
class OutputManager<Display, Frontend, output_mode::buffered>
{
  public:
    OutputManager(Display& display)
      : display_(display),
      pixelsLeftInPage_(0)
    {
    }

    typedef PageBuffer<Display, Frontend> buffer_t;
    typedef Display display_t;

    void writeChunk()
    {
      size_t chunkOffset = buffer_t::pixelsPerPage - pixelsLeftInPage_;
      size_t chunkSize = std::min((size_t)buffer_t::maxPixelsPerChunk, pixelsLeftInPage_);
      size_t bytes = (color::colorRepresentation_traits<typename Display::color_t>::storage_bit_size*chunkSize)/8;
      const uint8_t* data = buffer_.makeChunk(chunkOffset, chunkSize);
      std::cout << "OutputManager::writeChunk() : sending " << chunkSize << " pixels starting at " << chunkOffset << "\n"
                << "  data starts at " << std::hex << (size_t)data << ", " << std::dec << bytes << " bytes\n";
      display().writeChunk(data, bytes);
      pixelsLeftInPage_ -= chunkSize;
      std::cout << "  " << pixelsLeftInPage_ << " pixels left in page\n";
    }

    bool newFrameAllowed()
    {
      return true;
    }

    void beginPage()
    {
      std::cout << "OutputManager::beginPage() : draw()\n";
      pixelsLeftInPage_ = buffer_t::pixelsPerPage;
      writeChunk();
    }

    void update()
    {
      display().update();
      if (display().ready())
      {
        std::cout << "OutputManager::update() : display is ready\n";
        if (pixelsLeftInPage_) // page not finished : write next chunk
        {
          std::cout << "OutputManager::update() : there are pixels left in the page\n";
          writeChunk();
        }
        else if (buffer_.advance()) // frame not finished: start next page
        {
          beginPage();
        }
        else if (newFrameAllowed()) // frame finished, see if a new one may be started
        {
          std::cout << "OutputManager::update() : new frame\n";
          buffer_.beginFrame();
          beginPage();
        }
      }
      else
      {
        std::cout << "OutputManager::update() : display is busy\n";
      }
    }

    buffer_t& outputDevice()
    {
      return buffer_;
    }

    const buffer_t& outputDevice() const
    {
      return buffer_;
    }

    display_t& display()
    {
      return  display_;
    }

    const display_t& display() const
    {
      return  display_;
    }
  private:
    buffer_t buffer_;
    display_t& display_;
    size_t pixelsLeftInPage_;
};

#endif // SFC_OUTPUTMANAGER_H

