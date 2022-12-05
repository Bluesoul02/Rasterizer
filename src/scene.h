#include <vector>
#include "shape.h"
#include <window.h>
#include "ibehavior.h"

#define VIEWPORT_WIDTH 2.0
#define CANVAS_WIDTH 1366.0
#define CANVAS_HEIGHT 768.0

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
                window.set_title( "Scene" );
                window.set_width( 600 );
                window.set_height( 400 );
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

                    // set color
                    window.set_draw_color( minwin::WHITE );
                    
                    for (Shape s : shapes) {
                        for (Face f : s.get_faces()) {
                            draw_line(s.get_vertices()[f[0]], s.get_vertices()[f[1]]);
                            draw_line(s.get_vertices()[f[1]], s.get_vertices()[f[2]]);
                            draw_line(s.get_vertices()[f[0]], s.get_vertices()[f[2]]);
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
                // draw a line
                
            }

            Vec2r viewport_to_canvas( const Vec2r & point ) {
                Vec2r vec = Vec2r();
                vec[0] = point[0] * CANVAS_WIDTH / VIEWPORT_WIDTH;
                vec[1] = point[1] * CANVAS_HEIGHT / ((CANVAS_HEIGHT/CANVAS_WIDTH) * VIEWPORT_WIDTH);
                return vec;
            }

            Vec2i canvas_to_window( const Vec2r & point ) {
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