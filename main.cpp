#include "View.h"
#include "InputManager.h"
#include "Mesh.h"
#include "constants.h"
#include <chrono>


int main(int argc, char* argv[]) {
    InputManager inputs = InputManager();
    View view = View();

    view.addMesh({
        {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.0f,  0.5f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}
        }, "triangle");

    Model3D* triangle = view.addModel({ "triangle" });
    triangle->translate(Vec3(0, 0, -1));
    triangle->scale(Vec3(1, 1, 1));
    Model3D* light = view.addModel({ "triangle" });
    light->scale(Vec3(0.5, 0.5, 1));

    view.finalizeMeshes();


    size_t i = 0;

    while (!inputs.is_quiting()) {
        auto start = std::chrono::system_clock::now();
        auto duration = start.time_since_epoch();
        auto milliseconds_start = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        triangle->rotateY(0.5);

        inputs.update();

        if (inputs.is_pressed(SDL_SCANCODE_W)) {
            view.translateCam(Vec3(0, 0, -0.02));
        } if (inputs.is_pressed(SDL_SCANCODE_S)) {
            view.translateCam(Vec3(0, 0, 0.02));
        } if (inputs.is_pressed(SDL_SCANCODE_A)) {
            view.translateCam(Vec3(-0.02, 0, 0));
        } if (inputs.is_pressed(SDL_SCANCODE_D)) {
            view.translateCam(Vec3(0.02, 0, 0));
        } if (inputs.is_pressed(SDL_SCANCODE_Q)) {
            view.rotateCamY(1);
        } if (inputs.is_pressed(SDL_SCANCODE_E)) {
            view.rotateCamY(-1);
        }

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
