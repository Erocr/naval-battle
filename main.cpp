#include "View.h"
#include "InputManager.h"
#include "Mesh.h"
#include "constants.h"
#include <chrono>


int main(int argc, char* argv[]) {
    InputManager inputs = InputManager();
    View view = View();
    
    view.addMesh({
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}
        }, "triangle");

    Model3D* triangle = view.addModel({ "triangle" });
    triangle->translate(Vec3(0, 0, -1));
    triangle->scale(Vec3(1, 1, 1));

    view.finalizeMeshes();
    

    size_t i = 0;

    while (!inputs.is_quiting()) {
        auto start = std::chrono::system_clock::now();
        auto duration = start.time_since_epoch();
        auto milliseconds_start = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        triangle->rotateY(0.5);

        inputs.update();
        view.draw();

        i++;

        auto end = std::chrono::system_clock::now();
        auto duration_end = end.time_since_epoch();
        auto milliseconds_end = std::chrono::duration_cast<std::chrono::milliseconds>(duration_end).count();
        auto d = (milliseconds_end - milliseconds_start);
        //std::cout << ((d != 0) ? (1000 / d) : 1000) << ' ';

        SDL_Delay(15);

    }
    
    view.quit();

    return 0;
}
