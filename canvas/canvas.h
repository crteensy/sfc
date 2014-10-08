#ifndef SFC_CANVAS_H
#define SFC_CANVAS_H

//#include "../output/outputDispatcher.h"
#include "../output/outputManager.h"

/** \mainpage A Somewhat Flexible Display Driver Framework
So you have this new display and you don't want to come up with all the drawing functions, buffering and what-not yet again?
This framework should help by providing classes for setting up everything around a display driver that is \e not related to the actual I/O.

\section sec_main_classes Main Classes and Concepts
The main classes and bits that you need to set up a canvas for your display are described here, from higher to lower level. This is closely related
to their "closeness" to the actual display driver.

\subsection subsec_Frontend Frontend
A Frontend is used as a template parameter for a lot of classes, and defines the coordinate and color system for a,well, "frontend" layer.

\subsection subsec_Canvas Canvas
The Canvas class is a composite class that draws on an output device. It does a lot of things, and many of them can be tailored to your display.
A Canvas is created for a Display and with a certain Frontend. Display and Frontend can use different color spaces and coordinate systems.

\subsection subsec_PageBuffer Page Buffer
A PageBuffer is needed for displays that don't support drawing primitves, or for applications that simply decide to use one. It divides the display's
area into an integer number of pages. There is always one page in RAM, and consequently the number of pages must be chosen large enough to get a small
enough page for your microcontroller. A default PageBuffer class is available that does all this for you,
but it can also be specialized for special applications. The default implementation uses some more special classes internally that are described on the
\ref page_PageBuffer page.

**/

/** \page page_PageBuffer Page Buffers
A Page Buffer represents a part of a display's area and maintains it in RAM for read and write access. There are some details that make this
not so straight-forward as one would probably expect:
- A Page Buffer is created "between" a frontend and a display, so it needs to know about and correctly handle different color and coordinate systems.
- The other important aspect is that different displays have different graphics RAM layouts.
**/


template <typename Display>
struct DefaultFrontend
{
  typedef typename Display::color_t color_t;
};

/** \brief Main Canvas class
  \tparam Frontend class defining the Frontend properties
  \tparam Display what display class to draw on
**/
template <typename Display, typename Frontend = DefaultFrontend<Display> >
class Canvas
{
  public:
    typedef OutputManager<Display, Frontend, typename output_traits<Display, Frontend>::type> outputDispatcher_t;

    // Frontend types
    typedef typename Frontend::color_t color_t;
    typedef Point<Frontend> point_t;

    /** \brief Import display type from \ref OutputDispatcher **/
    typedef typename outputDispatcher_t::display_t display_t;

    /** \brief Import output device type from \ref OutputDispatcher **/
    typedef typename outputDispatcher_t::buffer_t output_device_t;

    /** \brief Construct a Canvas for the given Display
      \param display the display to draw on
    **/
    Canvas(display_t& display)
      : outputDispatcher_(display)
    {
    }

    void beginFrame()
    {
      outputDevice().beginFrame();
    }

    /** \brief grant the canvas and the underlying objects some CPU time
    **/
    void update()
    {
      outputDispatcher_.update();
    }

    /** \brief get the display this canvas draws on
      \return reference to the display this canvas draws on
    **/
    display_t& display()
    {
      return outputDispatcher_.display();
    }

    /** \brief get the output device this canvas draws on
      \return reference to the output device this canvas draws on
    **/
    output_device_t& outputDevice()
    {
      return outputDispatcher_.outputDevice();
    }

    const output_device_t& outputDevice() const
    {
      return outputDispatcher_.outputDevice();
    }

    /** \brief draw a pixel at the specified point, with the specified color.
     * The pixel will only be drawn if it is within the current bounding box.
     * The color will be cast (if possible) to the canvas' color_t.
     * \param p pixel location
     * \param c color.
     * \return true if the pixel could be drawn (i.e. it was within the bounding box)
    **/
    bool drawPixel(const point_t& p, const color_t& c)
    {
      return outputDevice().drawPixel(p, c);
    }

    /** \brief read a pixel at the specified point.
     * \param p pixel location
     * \return Color at the specified point or a default-constructed color_t
    **/
    color_t readPixel(const point_t& p) const
    {
      return outputDevice().readPixel(p);
    }

  private:
  outputDispatcher_t outputDispatcher_;
};

#endif // SFC_CANVAS_H

