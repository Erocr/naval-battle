#include "View.h"
#include "InputManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "Model3D.h"


size_t VERTICES_BUFFER_SIZE = 3;


int main(int argc, char* argv[]) {
    InputManager inputs = InputManager();
    View view = View();
    Shader shader = Shader("vertex.glsl", "fragment.glsl");

    std::vector<VertexAttributes> vertices = {
        {{0.5f, -0.5f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}},
        {{ 0.0f,  0.5f, 0.0f}}
    };
    VertexAttributes* buffer = (VertexAttributes*)malloc(sizeof(VertexAttributes) * VERTICES_BUFFER_SIZE);
    Mesh mesh = Mesh(vertices, 0, buffer);
    std::vector<Mesh*> meshes = { &mesh };
    Model3D triangle = Model3D(meshes);
    Model3D triangle2 = Model3D(meshes);
    triangle2.translate(Vec3(0, 0.1, 0));
    shader.init(buffer, VERTICES_BUFFER_SIZE);
    shader.putUniform("color", 1.0, 0.0, 0.0, 1.0);
    size_t i = 0;

    while (!inputs.is_quiting()) {
        inputs.update();
        shader.test_in_loop();
        triangle.draw(shader);
        triangle2.draw(shader);
        shader.putUniform("color", abs(sin(i/100.0)), abs(cos(i/100.0)), 0.0, 1.0);
        view.refresh();

        triangle.translate(Vec3(0.001, 0, 0));

        SDL_Delay(100);
        i++;
    }
    
    view.quit();

    return 0;
}
