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

    class Frustum {
        private:
            real near; // front
            real far; // back
            real right;
            real left;
            real bottom;
            real top;
        public:
            Frustum(real near_dist, real far_dist) {
                
            }

            void get_near() {
                
            }
            
            void get_far() {
                
            }

            Object clip(const Object &obj) const {
                return obj;
            }
    };

    class Camera {
        private:
            real aspect_ratio;
            real focal_dist;
            Vec4r position;
            Vec3r orientation;
            real move_speed;
            real rot_speed;
            real zoom_speed;
            real current_move_speed;
            real current_rot_speed;
            real current_zoom_speed;
            // Frustum frustum;
        public:
            Camera(real aspect_ratio, real focal_dist = 2.0, Vec4r position = {0.0, 0.0, 0.0, 1.0}, Vec3r orientation = {0.0, 0.0, 0.0}, real move_speed = 0.125, real rot_speed = 0.25, real zoom_speed = 0.0625) {
                this->aspect_ratio = aspect_ratio;
                this->focal_dist = focal_dist;
                this->position = position;
                this->orientation = orientation;
                this->move_speed = move_speed;
                this->rot_speed = rot_speed;
                this->zoom_speed = zoom_speed;
                current_move_speed = 0;
                current_rot_speed = 0;
                current_zoom_speed = 0;
            }

            void move_forward(uint axis) {
                position[axis] += move_speed;
            }

            void move_backward(uint axis) {
                position[axis] -= move_speed;
            }

            void rotate_cw(uint axis) {
                orientation[axis] += rot_speed;
            }

            void rotate_acw(uint axis) {
                orientation[axis] -= rot_speed;
            }

            void zoom_in() {
                focal_dist -= zoom_speed;
            }

            void zoom_out() {
                focal_dist += zoom_speed;
            }

            void stop_movement() {

            }

            void stop_rotation() {

            }

            void stop_zoom() {

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

                real r0 = orientation[0] * M_PI / 180;
                real r1 = orientation[1] * M_PI / 180;
                real r2 = orientation[2] * M_PI / 180;

                rotationMatrix = {
                    {cos(r1) * cos(r2), cos(r1) * sin(r2), -sin(r1), 0},
                    {sin(r0) * sin(r1) * cos(r2) - cos(r0) * sin(r2), sin(r0) * sin(r1) * sin(r2) + cos(r0) * cos(r2), sin(r0) * cos(r1), 0},
                    {cos(r0) * sin(r1) * cos(r2) + sin(r0) * sin(r2), cos(r0) * sin(r1) * sin(r2) - sin(r0) * cos(r2), cos(r0) * cos(r1), 0},
                    {0, 0, 0, 1}
                };

                translationMatrix = {
                    {1, 0, 0, position[0]},
                    {0, 1, 0, position[1]},
                    {0, 0, 1, position[2]},
                    {0, 0, 0, 1}
                };

                scaleMatrix = {
                    {position[3], 0, 0, 0},
                    {0, position[3], 0, 0},
                    {0, 0, position[3], 0},
                    {0, 0, 0, 1}
                };

                // SRT
                // transformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

                // TRS
                transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;

                return transformMatrix;
            }

            void update() {

            }
    };
}