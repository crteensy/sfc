#ifndef SFC_POINT_H
#define SFC_POINT_H

#include <type_traits>

template <typename Layer, typename = typename std::enable_if<std::is_integral<typename Layer::coordinate_t>::value>::type>
class PointT
{
  public:
    /** \brief use the layer's coordinate type **/
    typedef typename Layer::coordinate_t coordinate_t;

    PointT() : x_(0), y_(0) {}

    PointT(const coordinate_t& x, const coordinate_t y) : x_(x), y_(y) {}

    PointT(const PointT& other) : x_(other.x()), y_(other.y()) {}

    template <typename From>
    PointT& operator=(const PointT<From>& from)
    {
      convert(*this, from);
      return *this;
    }

    template<typename From>
    PointT(const PointT<From>& from)
    {
      *this = from;
    }

    PointT& operator+=(const PointT& other)
    {
      x() += other.x();
      y() += other.y();
      return *this;
    }

    coordinate_t& x()
    {
      return x_;
    }

    const coordinate_t& x() const
    {
      return x_;
    }

    coordinate_t& y()
    {
      return y_;
    }

    const coordinate_t& y() const
    {
      return y_;
    }

  private:
    coordinate_t x_, y_;
};


/** \brief A generic 2D point suitable for Canvas and Display operations.
 * You can specialize this for a certain Layer to use fixed or floating points, or any other coordinate type you want.
 * Specializing a Point also means that conversion methods between this and other points must be defined, see convert() .
 * \tparam layer the Layer type for which this point class is defined
**/
template <typename Layer>
class Point : public PointT<Layer>
{
  public:
    /** \brief use the layer's coordinate type **/
    typedef typename Layer::coordinate_t coordinate_t;
    typedef PointT<Layer> Parent;

    Point() : Parent()
    {
    }

    Point(const coordinate_t& x, const coordinate_t y) : Parent(x,y)
    {
    }

    Point(const Point& other) : Parent(other)
    {
    }

    template<typename From>
    Point(const PointT<From>& from) : Parent(from)
    {
    }

};


/** \brief Converts points between different Layers
 * \tparam To The Layer type to convert to
 * \tparam From The Layer type to convert from
 * \param to the point to covert from
 * \param from the point to convert from
**/
template <typename To, typename From>
void convert(PointT<To>& to, const PointT<From>& from)
{
  to.x() = from.x();
  to.y() = from.y();
}

#endif // SFC_POINT_H
