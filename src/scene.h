#include <vector>
#include "shape.h"
#include "window.h"

namespace aline {

    class Scene {
        private:
            minwin::Window window;
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
                window.display();
            }

            void run() {

            }

            void shutdown() {
                window.close();
            }
    };
}