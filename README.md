# Author : Kévin Avot
# Description 
library containing the most fundamental data structures and functions for the implementation of the rasterizer, i.e., vectors and matrices.
window displaying figure in 2D
Press esc to quit
Press space to change vision mode, there is two different modes wireframe and filled
The program is configured to display the teapot, to display something else you must change in the code, the values of the vector translation to display it properly, you can comment the line in load_obj_file and uncomment the other line, the second line is made to display the tetrahedron
it takes some time to display the teapot

To move the camera Z,Q,S,D
To go up and W,X
To rotate on X axis : R,T
To rotate on Y axis : A,E
To rotate on Z axis : F,G

clipping, backface culling and hidden surface removal are not implemented


# to recompile:
- make clean
- make all

# to run:
- ./bin/test_scene

# changelog (test):
- changed makefile
- added a test to test inverse of a matrix in test_matrix.cpp
- added div by zero test in test_matrix.cpp
