# SAP V2 Game Library
This is basically a version 2 of SAP Utils, this follows a more complex interface with bindings.

In the future, this could potentially be compiled as a DLL instead of as just a static library.
This could make file sizes smaller, as it would compact all of the large parts such as Raylib
into a single DLL instead of having them all have an individual copy. This would be less
portable, but could potentially be better.

This approach uses layers, instead of just having a single thing rendering at a time, this allows
for a better use of input blocking. There are 3 main layers to this in terms of both rendering,
and input (separate for keyboard and mouse).

This may either use Raylib, or SDL(2).