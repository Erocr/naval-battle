#include "View.h"
#include "InputManager.h"
#include "Shader.h"



int main(int argc, char* argv[]) {
    InputManager inputs = InputManager();
    View view = View();
    Shader shader = Shader("vertex.glsl", "fragment.glsl");

    VertexAttributes vertices[] = {
        {{0.5f, -0.5f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}},
        {{ 0.0f,  0.5f, 0.0f}}
    };
    shader.init(vertices, 3);
    shader.putUniform("color", 1.0, 0.0, 0.0, 1.0);

    while (!inputs.is_quiting()) {
        inputs.update();
        shader.test_in_loop();
        view.refresh();
        SDL_Delay(100);
    }
    
    view.quit();

    return 0;
}
