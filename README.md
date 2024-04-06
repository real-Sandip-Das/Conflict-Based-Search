# Conflict-Based-Search

Implementation of Conflict Based Search

Structure of the Implementation(Description of each file that has some code in it):

Convert_Map.pyx, Convert_Maps.py: This is the first step, convert Maps in .map files into .npz file with a collection of numpy.ndarrays that describe the map in a way easier to deal with in the programs

cpp_files/CMakeLists.txt: Please ignore this, this is only to make it easier to edit these files in a IDE/Text Editor with C++ support

cpp_files/include/definitions.h: some basic #defines and typedefs to use throughout the implementation

cpp_files/include/AStar.h, cpp_files/src/AStar.cpp: The Implementation of the low-level individual Path Finding algorithm(i.e. A\* in this case)

A_Star.pyx, A_Star.py: Files intended to test the A\* Implementation on individual agents

## Why was everything in the C++ part kept in a separate namespace?

    This was done to keep the original definitions(e.g. of classes or functions) from the C++ source files separate from the definitions coming from the Cython counterpart. Without this, the complier would refuse to compile due to multiple definitions of the same thing
