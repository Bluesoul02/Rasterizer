#include <vector>
#include "shape.h"
#include <window.h>
#include "ibehavior.h"

#define VIEWPORT_WIDTH 2.0
#define CANVAS_WIDTH 1000
#define CANVAS_HEIGHT 1000

namespace aline {

    class Scene {
        private:
            minwin::Window window;
            bool running { true };
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
                window.register_key_behavior( minwin::KEY_ESCAPE, new QuitKeyBehavior( *this ) );
                window.register_quit_behavior( new QuitButtonBehavior( *this ) );

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
                            draw_line(v0, v1);
                            draw_line(v1, v2);
                            draw_line(v0, v2);
                        }
                    }
                    
                    // display elements drawn so far
                    window.display();
                }
            }

            void shutdown() {
                window.close();
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
    };
}