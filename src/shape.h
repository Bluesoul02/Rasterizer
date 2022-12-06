#include "vector.h"
#include "color.h"

namespace aline {

    class Vertex {
        private:
            Vec2r vec;
            real h;
        public:
            Vertex( const Vec2r & c, real h ) {
                vec = c;
                this->h = h;
            }

            Vec2r get_vector() const {
                return vec;
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
}