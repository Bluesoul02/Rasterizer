#include <vector>
#include "shape.h"
#include <window.h>
#include "ibehavior.h"

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
                        // set color
                        window.set_draw_color( minwin::WHITE );
                        draw_line(s.get_vertices()[0], s.get_vertices()[1]);
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
                    for( int i = v0[0]; i < v1[1]; ++i )
                        window.put_pixel( i, 80 );
                }

            class QuitButtonBehavior : public minwin::IButtonBehavior
            {
                public:
                    QuitButtonBehavior( Scene &scene ) : owner { scene } {}
                    void on_click() const { this->owner.running = false; }
                private:
                    Scene & owner;
            };
    };
}