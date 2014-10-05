sfc
===

Somewhat Flexible Canvas library, hardware-independent, in C++0x/C++11

Introduction
-

Writing a driver for a new display often requires writing a lot of managing code, which is time-consuming and tedious.
This library aims at providing a flexible framework from the canvas level down to the point where you would say
"Hey Display, here is an array of bytes for your graphics RAM".

Current State
-

- Currently only the color class files in /color and one example in /color/examples/01_color are included. The example demonstrates the conversion between the different color types.
