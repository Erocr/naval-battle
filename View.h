#pragma once

#include <SDL/SDL.h>

#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>


class View {
private:
	SDL_Window *window;
	SDL_GLContext glContext;

public:
	View();

	void quit();
	void refresh();
};