/**
  COMPILE WITH:
  g++ -Os -std=c++0x -o 03_customConversions main.cpp
  in the sfc/color/examples/03_customConversions directory.
**/

#include <iostream>

#include "../../color.h"
#include "../../../ostream/color.h"

/** A special conversion function for Monochrome -> RGB565 **/

namespace color
{
void convert(RGB565& rgb, const Monochrome& m)
{
  std::cout << "special conversion from Monochrome to RGB565!\n";
  rgb.r() = m.k() ? ((1<<5)-1) : 0; // set all bits
  rgb.g() = 0;
  rgb.b() = 0;
}
}

using namespace std;

int main()
{
  cout << "Custom Color conversion functions\n"
       << "=================================\n\n";

  color::RGB565 rgb(0,0,0);
  cout << "RGB565 rgb(0,0,0): rgb = " << rgb << "\n";
  color::Monochrome m(true);
  cout << "Monochrome m(true): m = " << m << "\n\n";
  rgb = m;
  cout << "rgb = m: rgb = " << rgb << "\n\n";
  color::RGB565 rgb2(m);
  cout << "RGB565 rgb2(m): rgb2 = " << rgb2 << "\n";
  return 0;
}
