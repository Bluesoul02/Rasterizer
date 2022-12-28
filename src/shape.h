#include "matrix.h"
#include "color.h"

namespace aline {

    class Vertex {
        private:
            Vec3r vec;
            real h;
        public:
            Vertex( const Vec3r & c, real h ) {
                vec = c;
                this->h = h;
            }

            Vec3r get_vector() const {
                return vec;
            }

            real get_H() const {
                return h;
            }
    };

    class Face {
        private:
            uint v0;
            uint v1;
            uint v2;
            minwin::Color c;
        public:
            Face( uint v0, uint v1, uint v2, const minwin::Color & color ) {
                this->v0 = v0;
                this->v1 = v1;
                this->v2 = v2;
                c = color;
            }

            uint get_v0() const {
                return v0;
            }

            uint get_v1() const {
                return v1;
            }

            uint get_v2() const {
                return v2;
            }

            minwin::Color get_color() const {
                return c;
            }
    };

    class Shape {
        private:
            std::string name;
            std::vector<Vertex> vertices;
            std::vector<Face> faces;
        public:
            Shape( const std::string & name, const std::vector<Vertex> & vertices, const std::vector<Face> & faces ) {
                this->name = name;
                this->vertices = vertices;
                this->faces = faces;
            }

            std::string get_name() const {
                return name;
            }

            std::vector<Vertex> get_vertices() const {
                return vertices;
            }

            std::vector<Face> get_faces() const {
                return faces;
            }
    };

    class Object {
        private:
            const Shape * shape;
            Vec3r translation;
            Vec3r rotation;
            Vec3r scale;
        public:
            Object(const Shape & shape, const Vec3r & translation, const Vec3r & rotation, const Vec3r & scale): translation(translation), rotation(rotation), scale(scale) {
                this->shape = &shape;
            }

            Matrix<real, 4, 4> transform() const{

                Matrix<real, 4, 4> translateMatrix = { 
                    { 1, 0, 0, translation[0] }, 
                    { 0, 1, 0, translation[1] }, 
                    { 0, 0, 1, translation[2] }, 
                    { 0, 0, 0, 1 } };

                Matrix<real, 4, 4> rotateXMatrix = { 
                    { 1, 0, 0, 0 }, 
                    { 0, std::cos(rotation[0]), -std::sin(rotation[0]), 0 }, 
                    { 0, std::sin(rotation[0]), std::cos(rotation[0]), 0 }, 
                    { 0, 0, 0, 1 } };

                Matrix<real, 4, 4> rotateYMatrix = { 
                    { std::cos(rotation[1]), 0, std::sin(rotation[1]), 0 }, 
                    { 0, 1, 0, 0 }, 
                    { -std::sin(rotation[1]), 0, std::cos(rotation[1]), 0 }, 
                    { 0, 0, 0, 1 } };

                Matrix<real, 4, 4> rotateZMatrix = { 
                    { std::cos(rotation[2]), -std::sin(rotation[2]), 0, 0 }, 
                    { std::sin(rotation[2]), std::cos(rotation[2]), 0, 0 }, 
                    { 0, 0, 1, 0 }, 
                    { 0, 0, 0, 1 } };

                Matrix<real, 4, 4> scaleMatrix = { 
                    { scale[0], 0, 0, 0 }, 
                    { 0, scale[1], 0, 0 }, 
                    { 0, 0, scale[2], 0 }, 
                    { 0, 0, 0, 1 } };

                std::vector<Vertex> vertices = shape->get_vertices();

                Matrix<real, 4, 4> transform = { 
                    { vertices[0].get_vector()[0], vertices[0].get_vector()[1], vertices[0].get_vector()[0], 0 }, 
                    { vertices[1].get_vector()[0], vertices[1].get_vector()[1], vertices[1].get_vector()[1], 0 }, 
                    { vertices[2].get_vector()[0], vertices[2].get_vector()[1], vertices[2].get_vector()[2], 0 }, 
                    { 0, 0, 0, 1 } };

                // for ( int i=0; i<3; i++ ) {
                //     Vector<real, 3> temp = {vertices[i].get_vector()[0], vertices[i].get_vector()[1], vertices[i].get_vector()[2]};
                //     Vector<real, 3> transformed = rotation * translation * temp;
                //     transform[i][0] = transformed[0]; // x
                //     transform[i][1] = transformed[1]; // y
                //     transform[i][2] = transformed[2]; // z
                //     transform[i][3] = scale[i]; // scale
                // }
                // transform[4][0] = 0;
                // transform[4][1] = 0;
                // transform[4][2] = 0;
                // transform[4][3] = 1;
                return transform * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
            }

            std::vector<Vertex> get_vertices() const {
                return shape->get_vertices();
            }

            std::vector<Face> get_faces() const {
                return shape->get_faces();
            }
    };
}