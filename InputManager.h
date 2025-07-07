#pragma once
#include <SDL/SDL.h>
#include <map>


class InputManager {
private:
	bool quit;
	
	std::map<Uint16, Uint32> events;
	// Maps the scancode of the event to the time of pressing. 

public:
	InputManager() {
		quit = false;
		events = std::map<Uint16, Uint32>();
	}

	bool is_quiting() const { return quit; }

	void update();

	bool is_pressed(Uint16 keyCode) const;
	Uint32 time_pressed(Uint16 keyCode) const;
};

