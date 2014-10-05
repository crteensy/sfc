/**
  COMPILE WITH:
  g++ -Os -std=c++0x -o 01_colors main.cpp
  in the sfc/color/examples/01_colors directory.
**/

#include <iostream>

#include "../../color.h"
#include "../../../ostream/color.h"

int main()
{
  using namespace std;

  cout << "#==========================#\n"
       << "| SFC Color class examples |\n"
       << "#==========================#\n\n";

  cout << "Construction and assignment\n"
       << "===========================\n\n";

  cout << "RGB24: construction\n"
       << "-------------------\n\n";

  color::RGB24 rgb24(0,0,0);
  cout << "RGB24 rgb24(0,0,0): rgb24 = " << rgb24 << "\n\n";

  rgb24 = color::RGB24(0x80, 0x40, 0x20);
  cout << "rgb24 = RGB24(1,2,3): rgb24 = " << rgb24 << "\n\n";


  cout << "RGB24: conversion to others\n"
       << "---------------------------\n\n";

  color::RGB565 rgb565(rgb24);
  cout << "RGB565 rgb565(rgb24): rgb565 = " << rgb565 << "\n\n";

  color::Grayscale<8> g8(rgb24);
  cout << "Grayscale<8> g8(rgb24): g8 = " << g8 << "\n\n";

  color::Monochrome m(rgb24);
  cout << "Monochrome m(rgb24): m = " << m << "\n\n";


  cout << "RGB565: conversion to others\n"
       << "----------------------------\n\n";

  rgb565 = color::RGB565(0x80, 0x40, 0x20, color::channel::left_aligned);
  cout << "rgb565 = RGB565(0x80, 0x40, 0x20, left_aligned): rgb565 = " << rgb565 << "\n\n";

  rgb565 = color::RGB565(16,16,4, color::channel::right_aligned);
  cout << "rgb565 = RGB565(16,16,4, right_aligned): rgb565 = " << rgb565 << "\n\n";

  rgb24 = color::RGB24(rgb565);
  cout << "rgb24 = RGB24(rgb565): rgb24 = " << rgb24 << "\n\n";

  g8 = color::Grayscale<8>(rgb565);
  cout << "g8 = Grayscale<8> g8(rgb565): g8 = " << g8 << "\n\n";


  cout << "Grayscale<6>: conversion to others\n"
       << "----------------------------------\n\n";

  color::Grayscale<6> g6(0x80, color::channel::left_aligned);
  cout << "Grayscale<6> g6(0x80, left_aligned): g6 = " << g6 << "\n\n";

  g8 = color::Grayscale<8>(g6);
  cout << "g8 = Grayscale<8>(g6): g8 = " << g8 << "\n\n";

  rgb565 = color::RGB565(g6);
  cout << "rgb565 = RGB565(g6): rgb565 = " << rgb565 << "\n\n";

  rgb24 = color::RGB24(g6);
  cout << "rgb24 = RGB24(g6): rgb24 = " << rgb24 << "\n\n";


  cout << "Monochrome: conversion to others\n"
       << "--------------------------------\n\n";

  m = color::Monochrome(true);
  cout << "m = Monochrome(true): m = " << m << "\n\n";

  g6 = color::Grayscale<6>(m);
  cout << "g6 = Grayscale<6>(m): g6 = " << g6 << "\n\n";

  g8 = color::Grayscale<8>(m);
  cout << "g8 = Grayscale<8>(m): g8 = " << g8 << "\n\n";

  rgb565 = color::RGB565(m);
  cout << "rgb565 = RGB565(m): rgb565 = " << rgb565 << "\n\n";

  rgb24 = color::RGB24(m);
  cout << "rgb24 = RGB24(m): rgb24 = " << rgb24 << "\n\n";



  cout << "Reading from color channels\n"
       << "===========================\n\n";

  rgb565 = color::RGB565(0x80, 0x40, 0x20, color::channel::left_aligned);
  cout << "rgb565 = RGB565(0x80, 0x40, 0x20, left_aligned): rgb565 = " << rgb565 << "\n\n";

  cout << "rgb565.r() = " << rgb565.r() << "\n"
       << "      .g() = " << rgb565.g() << "\n"
       << "      .b() = " << rgb565.b() << "\n\n";

  cout << "rgb565.r().read(left_aligned) = " << std::hex << "0x" << rgb565.r().read(color::channel::left_aligned) << "\n"
       << "      .g().read(left_aligned) = " << std::hex << "0x" << rgb565.g().read(color::channel::left_aligned) << "\n"
       << "      .b().read(left_aligned) = " << std::hex << "0x" << rgb565.b().read(color::channel::left_aligned) << "\n\n";

  cout << "g6.k() = " << std::dec << g6.k() << "\n";
  cout << "g6.k().read(left_aligned) = " << (int)g6.k().read(color::channel::left_aligned) << "\n\n";


  cout << "Writing to color channels\n"
       << "=========================\n\n";

  rgb565.r() = 15;
  cout << "rgb565.r() = 15: rgb565.r() = " << rgb565.r() << "\n";
  rgb565.r().write(0x80, color::channel::left_aligned);
  cout << "rgb565.r().write(0x80, left_aligned): rgb565.r() = " << rgb565.r() << "\n";

  return 0;
}
