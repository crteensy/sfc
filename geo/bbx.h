#ifndef SFC_BBX_H
#define SFC_BBX_H

#include "point.h"

template <typename Layer>
struct Bbx
{
	typedef Point<Layer> point_t;
	typedef typename point_t::coordinate_t coordinate_t;

  /** \brief Constructs an invalid box **/
	Bbx() : p0(1,1), p1(0,0) {}
  Bbx(const point_t& p0_, const point_t& p1_) : p0(p0_), p1(p1_) {}

  bool contains(const point_t& p) const
  {
    return (((p0.x() <= p.x()) && (p.x() <= p1.x()))
            && ((p0.y() <= p.y()) && (p.y() <= p1.y())));
  }

  coordinate_t bottom() const
  {
    return std::max(p0.y(), p1.y());
  }

	point_t p0;
	point_t p1;
};

#endif // SFC_BBX_H

