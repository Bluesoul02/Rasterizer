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
            Vec4r near; // front
            Vec4r far; // back
            Vec4r right;
            Vec4r left;
            Vec4r bottom;
            Vec4r top;
        public:
            Frustum(real near_dist, real far_dist) {
                near = {0, 0, -1, -near_dist};
                far = {0, 0, 1, far_dist};
                left = {2, 0, -1, 0};
                right = {-2, 0, -1, 0};
                bottom = {0, 2, -1, 0};
                top = {0, -2, -1, 0};
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
            Frustum frustum = Frustum(0.1, 5.0);
            Vec3r orientation;
            real move_speed;
            real rot_speed;
            real zoom_speed;
            real current_move_speed;
            real current_rot_speed;
            real current_zoom_speed;
            uint axis;
            uint axisR;
        public:
            Camera(real aspect_ratio, real focal_dist = 2, Vec4r position = {0.0, 0.0, 0.0, 1.0}, Vec3r orientation = {0.0, 0.0, 0.0}, real move_speed = 2, real rot_speed = 0.25, real zoom_speed = 0.0625) {
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
                current_move_speed = move_speed;
                this->axis = axis;
            }

            void move_backward(uint axis) {
                current_move_speed = -move_speed;
                this->axis = axis;
            }

            void rotate_cw(uint axis) {
                current_rot_speed = rot_speed;
                this->axisR = axis;
            }

            void rotate_acw(uint axis) {
                current_rot_speed = -rot_speed;
                this->axisR = axis;
            }

            void zoom_in() {
                current_zoom_speed = zoom_speed;
            }

            void zoom_out() {
                current_zoom_speed = -zoom_speed;
            }

            void stop_movement() {
                current_move_speed = 0;
            }

            void stop_rotation() {
                current_rot_speed = 0;
            }

            void stop_zoom() {
                current_zoom_speed = 0;
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
                    {1, 0, 0, -position[0]},
                    {0, 1, 0, -position[1]},
                    {0, 0, 1, -position[2]},
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
                position[axis] += current_move_speed;
                // Vec3r a = {0, 0, 0};
                // a[axis] += current_rot_speed;
                // Vec3r tmp =  a * sin(current_rot_speed / 2 );
                // Vec4r r = {tmp[0], tmp[1], tmp[2], cos(current_rot_speed / 2)};
                orientation[axisR] += current_rot_speed;
                // rotating the camera will also move forward this a counter measure to attenuate the zoom
                if (axisR != 2 && current_move_speed == 0 && current_rot_speed != 0)
                    position[2] -= move_speed + current_rot_speed;
            }
    };
}