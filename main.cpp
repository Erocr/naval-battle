#include "View.h"
#include "InputManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "Model3D.h"
#include "constants.h"
#include <chrono>


size_t VERTICES_BUFFER_SIZE = 3;


int main(int argc, char* argv[]) {
    InputManager inputs = InputManager();
    View view = View();
    Shader shader = Shader("vertex.glsl", "fragment.glsl");

    std::vector<VertexAttributes> vertices = {
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}
    };
    VertexAttributes* buffer = (VertexAttributes*)malloc(sizeof(VertexAttributes) * VERTICES_BUFFER_SIZE);
    Mesh mesh = Mesh(vertices, 0, buffer);
    std::vector<Mesh*> meshes = { &mesh };
    Model3D triangle = Model3D(meshes);
    triangle.translate(Vec3(0, 0, -1));

    shader.init(buffer, VERTICES_BUFFER_SIZE);

    glm::mat4 persp = glm::perspective(PI / 2, double(WIDTH) / double(HEIGHT), 0.1, 1000.0);
    shader.putUniform("projection", persp);
    shader.putUniform("view", glm::mat4(1.0));

    size_t i = 0;

    while (!inputs.is_quiting()) {
        auto start = std::chrono::system_clock::now();
        auto duration = start.time_since_epoch();
        auto milliseconds_start = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        triangle.rotateY(1);

        inputs.update();
        shader.test_in_loop();
        triangle.draw(shader);
        view.refresh();

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
