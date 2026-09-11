#include "View.h"
#include "InputManager.h"
#include "Mesh.h"
#include "constants.h"
#include <chrono>


int main(int argc, char* argv[]) {
    InputManager inputs = InputManager();
    View view = View();

    
    std::cout << "loading meshes ... ";
    /*view.addMesh({
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}}
        }, "triangle", "textures/wood-texture.png");
        */
    //auto sphereRef = view.loadMeshes("3d_models_raw/sphere/source/anim8or sphere.obj", "3d_models/teapot", "rat");

    //Model3D* sphere = view.addModel(sphereRef);
    //sphere->translate(Vec3(0, 0, -3));
    //sphere->scale(3/2);

    view.addLight(Vec3(0, 0, 0), Vec4(1, 1, 1, 1));
    view.finalizeMeshes();
    std::cout << "finished" << std::endl;


    size_t i = 0;
    bool prev_scan_code_u = false;
    bool prev_scan_code_i = false;

    while (!inputs.is_quiting()) {
        auto start = std::chrono::system_clock::now();
        auto duration = start.time_since_epoch();
        auto milliseconds_start = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        if (inputs.is_pressed(SDL_SCANCODE_T)) {
            //rat->rotateY(1);
        }

        inputs.update();

        // Translations
        if (inputs.is_pressed(SDL_SCANCODE_W)) {
            view.translateCam(Vec3(0, 0, -0.02));
        } if (inputs.is_pressed(SDL_SCANCODE_S)) {
            view.translateCam(Vec3(0, 0, 0.02));
        } if (inputs.is_pressed(SDL_SCANCODE_A)) {
            view.translateCam(Vec3(-0.02, 0, 0));
        } if (inputs.is_pressed(SDL_SCANCODE_D)) {
            view.translateCam(Vec3(0.02, 0, 0));
        } if (inputs.is_pressed(SDL_SCANCODE_LSHIFT)) {
            view.translateCam(Vec3(0, -0.02, 0));
        } if (inputs.is_pressed(SDL_SCANCODE_SPACE)) {
            view.translateCam(Vec3(0, 0.02, 0));
        } 
        
        // Rotations
        if (inputs.is_pressed(SDL_SCANCODE_Q)) {
            view.rotateCamY(1);
        } if (inputs.is_pressed(SDL_SCANCODE_E)) {
            view.rotateCamY(-1);
        } 
        
        // Debug
        if (!prev_scan_code_u && inputs.is_pressed(SDL_SCANCODE_U)) {
            view.updateWaterSim();
        }
        if (!prev_scan_code_i && inputs.is_pressed(SDL_SCANCODE_I)) {
            view.draw_density(view.playerPos());
        }
        prev_scan_code_u = inputs.is_pressed(SDL_SCANCODE_U);
        prev_scan_code_i = inputs.is_pressed(SDL_SCANCODE_I);

        
        view.draw();
        

        i++;

        auto end = std::chrono::system_clock::now();
        auto duration_end = end.time_since_epoch();
        auto milliseconds_end = std::chrono::duration_cast<std::chrono::milliseconds>(duration_end).count();
        auto d = (milliseconds_end - milliseconds_start);
        if (inputs.is_pressed(SDL_SCANCODE_F))
            std::cout << "fps: " << ((d != 0) ? (1000.0 / d) : 1000) << std::endl;

        SDL_Delay(15);

    }
    
    view.quit();

    return 0;
}
