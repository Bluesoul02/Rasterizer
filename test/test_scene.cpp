#include "scene.h"
using namespace aline;

int main() {
    std::vector<Vertex> verts = { Vertex({-1.0, 0.0 },1)
                , Vertex({-0.5,-0.75}, 1)
                , Vertex({ 0.5,-0.75}, 1)
                , Vertex({ 1.0, 0.0 }, 1) 
                , Vertex({ 0.5, 0.75}, 1)
                , Vertex({-0.5, 0.75}, 1) };
    std::vector<Face> faces = { Face(0,1,5, minwin::RED), Face(1,2,4, minwin::BLUE), Face(1,4,5, minwin::GREEN), Face(2,3,4, minwin::WHITE) };
    Scene scene = Scene();
    scene.initialise();
    scene.add_shape(Shape( "euh", verts, faces));
    scene.run();
}