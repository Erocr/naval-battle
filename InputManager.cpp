#include "InputManager.h"



void InputManager::update() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			events.emplace(event.key.keysym.scancode, event.key.timestamp);
			break;
		case SDL_KEYUP:
			if (is_pressed(event.key.keysym.scancode))
				events.erase(event.key.keysym.scancode);
			break;
		case SDL_QUIT:
			quit = true;
			break;
		}
	}
}


bool InputManager::is_pressed(Uint16 keyCode) const {
	auto it = events.find(keyCode);
	return it != events.end();
}

Uint32 InputManager::time_pressed(Uint16 keyCode) const {
	return SDL_GetTicks() - events.at(keyCode);
}
