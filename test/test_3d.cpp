#include "scene.h"
using namespace aline;

int main(int argc, char* argv[]) {
    Scene scene;
    scene.initialise();
    scene.load_data(argc, argv);
    scene.run();

    return 0;
}