# Author : Kévin Avot
# Description 
library containing the most fundamental data structures and functions for the implementation of the rasterizer, i.e., vectors and matrices.
window displaying figure in 3D
Press esc to quit
Press space to change vision mode, there is two different modes wireframe and filled
The program is configured to display the teapot
it takes some time to display the teapot

To move the camera Z,Q,S,D
To go up and W,X
To rotate on X axis : R,T
To rotate on Y axis : A,E
To rotate on Z axis : F,G

not looking at any objects will make the program crash

clipping, backface culling and hidden surface removal are not implemented

The project need the minwin library to work which is included in its archive file with the same name

# to recompile:
- make clean
- make all

# to run:
- ./bin/test_scene

# changelog (test):
- changed makefile
- added a test to test inverse of a matrix in test_matrix.cpp
- added div by zero test in test_matrix.cpp
