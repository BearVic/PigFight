#include <iostream>
#include "renderContext.h"

class Hud{
public:
	static Hud& getInstance();
	// ~Hud();
	void draw();
	void changeVisible();

private:
	SDL_Renderer* renderer;
	SDL_Rect window;
	bool visible;
	Hud();
	Hud(const Hud&) = delete;
	Hud& operator=(const Hud&) = delete;
};