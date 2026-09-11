#include "View.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


View::View() {
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
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

    buffer[0] = { {-1, -1, 0}, {0, 0, 0}, {0, 0} };
    buffer[1] = { { 1, -1, 0}, {0, 0, 0}, {0, 0} };
    buffer[2] = { {-1,  1, 0}, {0, 0, 0}, {0, 0} };
    buffer[3] = { { 1, -1, 0}, {0, 0, 0}, {0, 0} };
    buffer[4] = { {-1,  1, 0}, {0, 0, 0}, {0, 0} };
    buffer[5] = { { 1,  1, 0}, {0, 0, 0}, {0, 0} };

    models = std::vector<Model3D*>();
    meshes = std::map<std::string, Mesh*>();
    textures = std::map<std::string, GLuint>();

    buffer_pos = 6;
    cam = Camera();

    lights = std::vector< Light > (0);

    sdfShader = Shader("emptyVertex.glsl", "sdf_fragment.glsl");
    glGenTextures(1, &color_buffer);
    glBindTexture(GL_TEXTURE_2D, color_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenTextures(1, &depth_buffer);
    glBindTexture(GL_TEXTURE_2D, depth_buffer);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT, 0,
        GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &fbo1);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo1);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        color_buffer,
        0);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_TEXTURE_2D,
        depth_buffer,
        0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(status == GL_FRAMEBUFFER_COMPLETE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    std::cout << "bfore water sim" << std::endl;
    waterSim = WaterSimulator(500);
    std::cout << "after water sim" << std::endl;
}
 

void View::updateLights() {
    std::vector<Vec3> lightPos = std::vector<Vec3>(0);
    std::vector<Vec4> lightCol = std::vector<Vec4>(0);
    for (Light &light : lights) {
        lightPos.push_back(light.pos);
        lightCol.push_back(light.color);
    }

    shader.activate();
    shader.putUniform("nbLights", (int)lights.size());
    shader.putUniform("lightPos", lightPos);
    shader.putUniform("lightColor", lightCol);

    sdfShader.activate();
    sdfShader.putUniform("nbLights", (int)lights.size());
    sdfShader.putUniform("lightPos", lightPos);
    sdfShader.putUniform("lightColor", lightCol);

    shader.activate();
}


void View::finalizeMeshes() {
    shader.init(buffer, VERTICES_BUFFER_SIZE);
    double fov = PI / 2;
    double near_ = 0.1;
    double far_ = 1000.0;
    glm::mat4 persp = glm::perspective(fov, double(WIDTH) / double(HEIGHT), near_, far_);
    shader.putUniform("projection", persp);
    shader.putUniform("ambient", 0.2, 0.2, 0.2, 1);
    updateLights();

    sdfShader.activate();
    sdfShader.putUniform("fov", (float) fov);
    sdfShader.putUniform("near", (float) near_);
    sdfShader.putUniform("far", (float) far_);
    sdfShader.putUniform("screenAspectRatio", float(WIDTH) / float(HEIGHT));
    sdfShader.putUniform("projectionInverse", glm::inverse(persp));
    sdfShader.putUniform("ambient", 0.2, 0.2, 0.2, 1);


    glEnable(GL_DEPTH_TEST);
    canAddMeshes = false;
}




void View::refresh() {
    cam.update(shader, sdfShader);
    SDL_GL_SwapWindow(window);
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void View::quit() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void View::draw() {
    waterSim.update();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo1);
    glViewport(0, 0, WIDTH, HEIGHT);

    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    shader.activate();

    
    for (Model3D* model : models) {
        model->draw(shader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, WIDTH, HEIGHT);

    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    sdfShader.activate();

    waterSim.putOnShader(sdfShader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, color_buffer);
    sdfShader.putUniform("previousColor", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depth_buffer);
    sdfShader.putUniform("depthBuffer", 1);

    glActiveTexture(GL_TEXTURE0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    //glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo1);
    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    //glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    refresh();
}


void View::addMesh(std::vector<VertexAttributes> vertices, std::string name, std::string texture_file_name) {
    if (not canAddMeshes) {
        std::cerr << "Adding a mesh after that the call of finalizeMeshes" << std::endl;
        exit(-1);
    }
    if (textures.find(texture_file_name) == textures.end()) {
        textures.insert({ texture_file_name, shader.load_image(texture_file_name) });
    }
    if (buffer_pos + vertices.size() > VERTICES_BUFFER_SIZE) {
        std::cerr << "To many vertices ! The max is " << VERTICES_BUFFER_SIZE << " but " << buffer_pos + vertices.size() << " have been loaded.";
        exit(-3);
    }
    meshes.insert({ name, new Mesh(vertices, buffer_pos, buffer, textures.at(texture_file_name)) });
    buffer_pos += vertices.size();
}

std::vector<std::string> View::loadMeshes(std::string obj_file_name, std::string mtl_file_name, std::string name) {
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warnings;
    std::string errors;
    bool success = tinyobj::LoadObj(&attributes, &shapes, &materials, &warnings, &errors, obj_file_name.c_str(), mtl_file_name.c_str(), true);
    if (warnings != "") {
        std::cerr << std::endl << "Warning in loading " << name << ':' << std::endl << warnings << std::endl;
    }
    if (!success) {
        std::cerr << std::endl << "Can't load " << obj_file_name << ':' << std::endl << errors << std::endl;
        exit(-2);
    }

    
    if (materials.size() == 0) {
        tinyobj::material_t default_mat = tinyobj::material_t();
        materials.push_back(default_mat);
    }

    std::vector<std::vector<VertexAttributes>> vertices = std::vector<std::vector<VertexAttributes>>(materials.size());
    // This is an array of groups of vertices. Each group of vertices is associated to a material.

    for (size_t i = 0; i < shapes.size(); i++) {
        tinyobj::shape_t& shape = shapes[i];
        tinyobj::mesh_t& mesh = shape.mesh;
        for (size_t j = 0; j < mesh.indices.size(); j++) {
            // It triangulates automatically, no need to triangulate
            tinyobj::index_t i = mesh.indices[j];
            float texCoordX, texCoordY;
            if (i.texcoord_index == -1) { texCoordX = 0; texCoordY = 0; }
            else {
                texCoordX = attributes.texcoords[i.texcoord_index * 2];
                texCoordY = attributes.texcoords[i.texcoord_index * 2 + 1];
            }
            float normalX, normalY, normalZ;
            if (i.normal_index != -1) {
                normalX = attributes.normals[i.normal_index * 3];
                normalY = attributes.normals[i.normal_index * 3 + 1];
                normalZ = attributes.normals[i.normal_index * 3 + 2];
            }
            else {
                Vec3 p1 = Vec3(attributes.vertices[i.vertex_index * 3],
                    attributes.vertices[i.vertex_index * 3 + 1],
                    attributes.vertices[i.vertex_index * 3 + 2]);

                size_t k;
                if (j % 3 == 2) k = j - 2;
                else k = j + 1;
                tinyobj::index_t i_t = mesh.indices[k];
                Vec3 p2 = Vec3(attributes.vertices[i_t.vertex_index * 3],
                    attributes.vertices[i_t.vertex_index * 3 + 1],
                    attributes.vertices[i_t.vertex_index * 3 + 2]);

                if (k % 3 == 2) k = k - 2;
                else k = k + 1;
                i_t = mesh.indices[k];
                Vec3 p3 = Vec3(attributes.vertices[i_t.vertex_index * 3],
                    attributes.vertices[i_t.vertex_index * 3 + 1],
                    attributes.vertices[i_t.vertex_index * 3 + 2]);

                Vec3 normal = cross(p2 - p1, p3 - p1);
                normalX = normal.getX();
                normalY = normal.getY();
                normalZ = normal.getZ();
            }

            VertexAttributes vert = { 
                {
                    attributes.vertices[i.vertex_index * 3],
                    attributes.vertices[i.vertex_index * 3 + 1],
                    attributes.vertices[i.vertex_index * 3 + 2] }, 
                { normalX, normalY, normalZ }, 
                { texCoordX, texCoordY }
            };
            if (mesh.material_ids[j / 3] == -1)
                vertices[0].push_back(vert);
            else
                vertices[mesh.material_ids[j / 3]].push_back(vert);
        }
    }

    std::cout << "number of meshes : " << vertices.size() << std::endl;

    std::vector<std::string> meshes_ = std::vector<std::string>(0);
    for (int i = 0; i < vertices.size(); i++) {
        std::string meshName = name + std::to_string(i);
        std::string tex_file_name = materials[i].diffuse_texname;
        if (tex_file_name == "") tex_file_name = "textures/white.jpg";
        addMesh(vertices[i], meshName, tex_file_name);
        meshes_.push_back(meshName);
        std::cout << meshes[meshName]->getStart() << " " << meshes[meshName]->getEnd() << std::endl;
    }

    return meshes_;
}

Model3D* View::addModel(std::vector<std::string> mesh_names) {
    std::vector<Mesh*> meshes_ = std::vector<Mesh*>();
    for (std::string name : mesh_names) {
        meshes_.push_back(meshes[name]);
    }
    models.push_back(new Model3D(meshes_));
    return models[models.size() - 1];
}



Light* View::addLight(Vec3 pos, Vec4 color) {
    lights.push_back({ pos, color });
    return &lights[lights.size() - 1];
}
