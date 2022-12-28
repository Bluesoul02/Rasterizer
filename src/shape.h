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
            Object(const Shape & shape, const Vec3r & translation, const Vec3r & rotation, const Vec3r & scale) {
                this->shape = &shape;
                this->translation = translation;
                this->rotation = rotation;
                this->scale = scale;
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

                real r0 = rotation[0] * M_PI / 180;
                real r1 = rotation[1] * M_PI / 180;
                real r2 = rotation[2] * M_PI / 180;

                rotationMatrix = {
                    {cos(r1) * cos(r2), cos(r1) * sin(r2), -sin(r1), 0},
                    {sin(r0) * sin(r1) * cos(r2) - cos(r0) * sin(r2), sin(r0) * sin(r1) * sin(r2) + cos(r0) * cos(r2), sin(r0) * cos(r1), 0},
                    {cos(r0) * sin(r1) * cos(r2) + sin(r0) * sin(r2), cos(r0) * sin(r1) * sin(r2) - sin(r0) * cos(r2), cos(r0) * cos(r1), 0},
                    {0, 0, 0, 1}
                };

                translationMatrix = {
                    {1, 0, 0, translation[0]},
                    {0, 1, 0, translation[1]},
                    {0, 0, 1, translation[2]},
                    {0, 0, 0, 1}
                };

                scaleMatrix = {
                    {scale[0], 0, 0, 0},
                    {0, scale[1], 0, 0},
                    {0, 0, scale[2], 0},
                    {0, 0, 0, 1}
                };

                // SRT
                // transformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

                // TRS
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