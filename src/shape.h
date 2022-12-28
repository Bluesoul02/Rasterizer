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

            Mat44r transform() const {
                Mat44r transformMatrix = 
                {
                    {1, 0, 0, 0},
                    {0, 1, 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}
                };
                Mat44r rotationMatrix;
                Mat44r scaleMatrix;
                Mat44r translationMatrix;

                scaleMatrix[0][0] = scale[0];
                scaleMatrix[1][1] = scale[1];
                scaleMatrix[2][2] = scale[2];
                scaleMatrix[3][3] = 1;

                real r0 = rotation[0] * M_PI / 180;
                real r1 = rotation[1] * M_PI / 180;
                real r2 = rotation[2] * M_PI / 180;

                rotationMatrix[0][0] = cos(r1) * cos(r2);
                rotationMatrix[0][1] = cos(r1) * sin(r2);
                rotationMatrix[0][2] = -sin(r1);

                rotationMatrix[1][0] = sin(r0) * sin(r1) * cos(r2) - cos(r0) * sin(r2);
                rotationMatrix[1][1] = sin(r0) * sin(r1) * sin(r2) + cos(r0) * cos(r2);
                rotationMatrix[1][2] = sin(r0) * cos(r1);

                rotationMatrix[2][0] = cos(r0) * sin(r1) * cos(r2) + sin(r0) * sin(r2);
                rotationMatrix[2][1] = cos(r0) * sin(r1) * sin(r2) - sin(r0) * cos(r2);
                rotationMatrix[2][2] = cos(r0) * cos(r1);

                rotationMatrix[3][3] = 1;

                translationMatrix[0][3] = translation[0];
                translationMatrix[1][3] = translation[1];
                translationMatrix[2][3] = translation[2];
                translationMatrix[3][3] = 1;

                transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;

                return transformMatrix;
            }

            std::vector<Vertex> get_vertices() const {
                return shape->get_vertices();
            }

            std::vector<Face> get_faces() const {
                return shape->get_faces();
            }
    };
}