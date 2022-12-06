#include <vector>
#include "shape.h"
#include <window.h>
#include "ibehavior.h"

#define VIEWPORT_WIDTH 2.0
#define CANVAS_WIDTH 500
#define CANVAS_HEIGHT 500

namespace aline {

    class Scene {
        private:
            minwin::Window window;
            bool running { true };
            int display = 0;
            std::vector<Shape> shapes;
        public:
            Scene() {
                shapes = std::vector<Shape>();
            }

            void add_shape( const Shape & s) {
                shapes.push_back(s);
            }

            void initialise() {
                window.set_title("Rasterizer");
                window.set_width(CANVAS_WIDTH);
                window.set_height(CANVAS_HEIGHT);
            }

            void run() {
                window.register_key_behavior(minwin::KEY_ESCAPE, new QuitKeyBehavior(*this));
                window.register_key_behavior( minwin::KEY_SPACE, new ChangeDisplayBehavior(*this));
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
                    
                    for (Shape s : shapes) {
                        for (Face f : s.get_faces()) {
                            // set color
                            window.set_draw_color(f.get_color());
                            Vec2r v0 = s.get_vertices()[f.get_v0()].get_vector();
                            Vec2r v1 = s.get_vertices()[f.get_v1()].get_vector();
                            Vec2r v2 = s.get_vertices()[f.get_v2()].get_vector();
                            if (display == 0) {
                                draw_wireframe_triangle(v0, v1, v2);
                            }
                            else if (display == 1) {
                                draw_filled_triangle(v0, v1, v2);
                            }
                        }
                    }
                    
                    // display elements drawn so far
                    window.display();
                }
            }

            void shutdown() {
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

            Vec2r viewport_to_canvas( const Vec2r & point ) const {
                Vec2r vec = Vec2r();
                vec[0] = point[0] * CANVAS_WIDTH / VIEWPORT_WIDTH;
                vec[1] = point[1] * CANVAS_HEIGHT / ((CANVAS_HEIGHT/CANVAS_WIDTH) * VIEWPORT_WIDTH);
                return vec;
            }

            Vec2i canvas_to_window( const Vec2r & point ) const {
                Vec2i vec = Vec2i();
                vec[0] = CANVAS_WIDTH/2 + point[0];
                vec[1] = CANVAS_HEIGHT/2 + point[1];
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
}