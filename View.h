#pragma once

#include <SDL/SDL.h>

#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <map>

#include "constants.h"
#include "Shader.h"
#include "Model3D.h"
#include "Camera.h"


class View {
private:
	SDL_Window *window;
	SDL_GLContext glContext;
	Shader shader;
	VertexAttributes* buffer;
	size_t buffer_pos;
	std::vector<Model3D*> models;
	std::map<std::string, Mesh*> meshes;
	bool canAddMeshes = true;

	Camera cam;


public:
	View();

	void finalizeMeshes();

	void quit();
	void refresh();
	void draw();

	void addMesh(std::vector<VertexAttributes> vertices, std::string name);
	Model3D* addModel(std::vector<std::string> mesh_names);

	void translateCam(Vec3 v) { cam.translate(v); }
	void rotateCamX(float angle) { cam.rotateX(angle); }
	void rotateCamY(float angle) { cam.rotateY(angle); }
	void rotateCamZ(float angle) { cam.rotateZ(angle); }
	void rotateCam(Vec3 angles) { cam.rotate(angles); }
};