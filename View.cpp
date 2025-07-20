#include "View.h"


View::View() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init error: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    window = SDL_CreateWindow("OpenGL Demo",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_OPENGL);

    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    glContext = SDL_GL_CreateContext(window);

    glewInit();

    shader = Shader("vertex.glsl", "fragment.glsl");
    buffer = (VertexAttributes*)malloc(sizeof(VertexAttributes) * VERTICES_BUFFER_SIZE);

    models = std::vector<Model3D*>();
    meshes = std::map<std::string, Mesh*>();

    buffer_pos = 0;
}

void View::finalizeMeshes() {
    shader.init(buffer, VERTICES_BUFFER_SIZE);
    glm::mat4 persp = glm::perspective(PI / 2, double(WIDTH) / double(HEIGHT), 0.1, 1000.0);
    shader.putUniform("projection", persp);
    shader.putUniform("view", glm::mat4(1.0));
    shader.putUniform("ambient", 0.2, 0.2, 0.2, 1);
    shader.putUniform("nbLights", 1);
    shader.putUniform("lightPos", { Vec3(0, 0, 0) });
    shader.putUniform("lightColor", { Vec4(1, 1, 1, 1)});
    canAddMeshes = false;
}


void View::refresh() {
    SDL_GL_SwapWindow(window);
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void View::quit() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void View::draw() {
    shader.test_in_loop();
    for (Model3D* model : models) {
        model->draw(shader);
    }
    refresh();
}


void View::addMesh(std::vector<VertexAttributes> vertices, std::string name) {
    if (not canAddMeshes) {
        std::cerr << "Adding a mesh after that the call of finalizeMeshes" << std::endl;
        exit(-1);
    }
    meshes.insert({ name, new Mesh(vertices, buffer_pos, buffer) });
    buffer_pos += vertices.size();
}

Model3D* View::addModel(std::vector<std::string> mesh_names) {
    std::vector<Mesh*> meshes_ = std::vector<Mesh*>();
    for (std::string name : mesh_names) {
        meshes_.push_back(meshes[name]);
    }
    models.push_back(new Model3D(meshes_));
    return models[models.size() - 1];
}

