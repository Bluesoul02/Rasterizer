#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <memory>
#include "shape.h"
#include <window.h>
#include "ibehavior.h"

#define VIEWPORT_WIDTH 2.0
#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 800

namespace aline {

    class Scene {
        private:
            minwin::Window window;
            bool running { true };
            int display = 0;
            std::vector<Object> objects;

            void draw_object(const Object &o) {
                Mat44r transform = o.transform();

                for (Face f : o.get_faces()) {
                    // set color
                    window.set_draw_color(f.get_color());
                    Vertex vert0 = o.get_vertices()[f.get_v0()];
                    Vertex vert1 = o.get_vertices()[f.get_v1()];
                    Vertex vert2 = o.get_vertices()[f.get_v2()];
                    Vec4r vec0 { vert0.get_vector()[0], vert0.get_vector()[1], vert0.get_vector()[2], 1.0 };
                    Vec4r vec1 { vert1.get_vector()[0], vert1.get_vector()[1], vert1.get_vector()[2], 1.0 };
                    Vec4r vec2 { vert2.get_vector()[0], vert2.get_vector()[1], vert2.get_vector()[2], 1.0 };

                    //perspective projection
                    Vec2r v0 = perspective_projection(transform * vec0, 50);
                    Vec2r v1 = perspective_projection(transform * vec1, 50);
                    Vec2r v2 = perspective_projection(transform * vec2 , 50);

                    if (display == 0) {
                        draw_wireframe_triangle(v0, v1, v2);
                    }
                    else if (display == 1) {
                        draw_filled_triangle(v0, v1, v2);
                    }
                }
            }

            Vec2r perspective_projection( const Vec4r & v, real d ) {
                Vec2r result;
                if (v[2] == 0) {
                    result[0] = 0;
                    result[1] = 0;
                    return result;
                }
                result[0] = (d * v[0]) / v[2];
                result[1] = (d * v[1]) / v[2];
                return result;
            }

            void load_obj_file(const char * file_name) {

                std::ifstream objFile(file_name);
                if (!objFile.is_open()) {
                    throw std::invalid_argument("can't open OBJ file");
                }

                std::vector<Vertex> vertices;
                std::vector<Face> faces;
                std::string line;
                uint v1;
                uint v2;
                uint v3;
                real x;
                real y;
                real z;
                while (std::getline(objFile, line)) {
                    std::istringstream lineStream(line);
                    std::string type;
                    lineStream >> type;
                    if (type == "v") {
                        lineStream >> x >> y >> z;
                        Vec3r vec {x, y, z};
                        Vertex v(vec, 1);
                        vertices.push_back(v);
                    } else if (type == "f") {
                        lineStream >> v1 >> v2 >> v3;
                        v1--;
                        v2--;
                        v3--;
                        Face f(v1, v2, v3, minwin::WHITE);
                        faces.push_back(f);
                    }
                }
                Shape *s = new Shape(file_name, vertices, faces);
                Object o(*s, {40, 0, 3000}, {0, 0, 0}, {1, 1, 1});
                //Object o(*s, {2.5, 1, 100}, {10, -10, 10}, {1, 1, 1});
                Object o_copy(o);
                add_object(o_copy);
            }

            void unload_data() {
                // free objects shapes
            }

        public:
            Scene() {
                objects = std::vector<Object>();
            }

            void add_object( const Object & o) {
                objects.push_back(o);
            }

            void initialise() {
                window.set_title("Rasterizer");
                window.set_width(1366);
                window.set_height(768);
            }

            void run() {
                window.register_key_behavior(minwin::KEY_ESCAPE, new QuitKeyBehavior(*this));
                window.register_key_behavior(minwin::KEY_SPACE, new ChangeDisplayBehavior(*this));
                window.register_quit_behavior(new QuitButtonBehavior(*this));

                // open window
                if( not window.open() )
                {
                    std::cerr << "Couldn't open window.\n";
                    return;
                }

                while( this->running )
                {
                    // process keyboard inputs, etc.
                    window.process_input();
                    
                    // clear window
                    window.clear();
                    
                    for (Object o : objects) {
                        draw_object(o);
                    }
                    
                    // display elements drawn so far
                    window.display();
                }
            }

            void shutdown() {
                unload_data();
                window.close();
            }

            void draw_wireframe_triangle( const Vec2r & v0, const Vec2r & v1, const Vec2r & v2 ) const {
                draw_line (v0, v1);
                draw_line (v1, v2);
                draw_line (v2, v0);
            }   

            void draw_filled_triangle( const Vec2r & v0, const Vec2r & v1, const Vec2r & v2 ) const {
                Vec2i u = canvas_to_window(viewport_to_canvas(v0));
                Vec2i v = canvas_to_window(viewport_to_canvas(v1));
                Vec2i w = canvas_to_window(viewport_to_canvas(v2));

                if(v[1] < u[1]) std::swap(v, u);
                if(w[1] < u[1]) std::swap(w, u);
                if(w[1] < v[1]) std::swap(w, v);

                int x0 = u[0];
                int x1 = v[0];
                int x2 = w[0];
                int y0 = u[1];
                int y1 = v[1];
                int y2 = w[1];

                std::vector<real> x02 = interpolate(y0, x0, y2, x2);
                std::vector<real> x01 = interpolate(y0, x0, y1, x1);
                std::vector<real> x12 = interpolate(y1 ,x1 ,y2 ,x2);

                x01. pop_back();
                std::vector<real> x012(x01);
                x012.insert(x012.end(), x12.begin(), x12.end());

                int m = floor(x012.size() / 2);
                std::vector<real> x_left;
                std::vector<real> x_right;
                if(x02[m] < x012[m]) {
                    x_left = x02;
                    x_right = x012;
                } else {
                    x_left = x012;
                    x_right = x02;
                }

                for(int y = y0; y <= y2; ++y)
                    for(int x = x_left[y - y0]; x <= x_right [y - y0]; ++x)
                        window.put_pixel(x, y);
            }

            std::vector<real> interpolate (int i0, real d0, int i1, real d1) const {
                std::vector<real> values;
                if( i0 == i1 ) {
                    values.push_back(d0);
                    return values;
                }
            
                real a = (d0 - d1) / (i0 - i1);
                real d = d0;
            
                for(int i = i0; i <= i1; ++i) {
                    values.push_back(d);
                    d = d + a;
                }
                return values ;
            }

            void draw_line( const Vec2r & v0, const Vec2r & v1 ) const { 
                Vec2i u = canvas_to_window(viewport_to_canvas(v0));
                Vec2i v = canvas_to_window(viewport_to_canvas(v1));

                int x0 = u[0];
                int x1 = v[0];
                int y0 = u[1];
                int y1 = v[1];

                int dx = abs(x1 - x0);
                int sx = x0 < x1 ? 1 : -1;
                int dy = -abs(y1 - y0);
                int sy = y0 < y1 ? 1 : -1;
                int error = dx + dy;
                
                while (true) {
                    window.put_pixel(x0, y0);
                    if (x0 == x1 && y0 == y1) break;
                    int e2 = 2 * error;
                    if (e2 >= dy) {
                        if (x0 == x1) break;
                        error = error + dy;
                        x0 = x0 + sx;
                    }
                    if (e2 <= dx) {
                        if (y0 == y1) break;
                        error = error + dx;
                        y0 = y0 + sy;
                    }
                }
            }

            void load_data(int argc, char * argv[]) {
                if(argc < 2) {
                    throw std::invalid_argument("no file provided");
                }

                std::cout << "loading objects..." << std::endl;

                for (int i = 1; i < argc; i++) {
                    char* filename = argv[i];
                    load_obj_file(filename);
                }

                std::cout << "objects loaded" << std::endl;
            }
                        
            Vec2r viewport_to_canvas( const Vec2r & point ) const {
                Vec2r vec = Vec2r();
                vec[0] = point[0] * CANVAS_WIDTH / VIEWPORT_WIDTH;
                vec[1] = point[1] * CANVAS_HEIGHT / ((CANVAS_HEIGHT/CANVAS_WIDTH) * VIEWPORT_WIDTH);
                return vec;
            }

            Vec2i canvas_to_window( const Vec2r & point ) const {
                Vec2i vec = Vec2i();
                vec[0] = CANVAS_WIDTH/2 + point[0];
                vec[1] = CANVAS_HEIGHT/2 - point[1];
                return vec;
            }

            class QuitButtonBehavior : public minwin::IButtonBehavior
            {
                public:
                    QuitButtonBehavior( Scene &scene ) : owner { scene } {}
                    void on_click() const { this->owner.running = false; }
                private:
                    Scene & owner;
            };

            class QuitKeyBehavior : public minwin::IKeyBehavior
            {
                public:
                    QuitKeyBehavior( Scene & scene ) : owner { scene } {}
                    void on_press() const { this->owner.running = false; }
                    void on_release() const {}
                private:
                    Scene & owner;
            };

            class ChangeDisplayBehavior : public minwin::IKeyBehavior
            {
                public:
                    ChangeDisplayBehavior( Scene & scene ) : owner { scene } {}
                    void on_press() const {
                        if (this->owner.display == 0) {
                            this->owner.display = 1;
                        }
                        else if (this->owner.display == 1) {
                            this->owner.display = 0;
                        }
                    }
                    void on_release() const {}
                private:
                    Scene & owner;
            };
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
            Frustum frustum;
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

            void update() {

            }
    };
}
