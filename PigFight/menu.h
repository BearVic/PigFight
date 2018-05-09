#ifndef MENU__H
#define MENU__H

#include <string>
#include <SDL.h>
#include <vector>
#include "image.h"
#include "imageFactory.h"
#include "gamedata.h"
#include "ioMod.h"
#include "soundMgr.h"

class Menu{
public:
	Menu();
	~Menu(){}
	void draw() const;
	// void update();
	void up();
	void down();
	bool play();

	


private:
	SDL_Renderer* renderer;
    Gamedata& gdata;
    // SDL_Rect hudFrame;
    // SDL_Color menuColor;
    IoMod& io;
    SoundMgr& soundMgr;
    std::vector<std::string> options;

	std::string title;
	int curIndicator;
	// int indicatorPosX;
	// int indicatorPosY;
	Image* indicatorIcon;
	Image* icon;
	int screenWidth;
	int screenHeight;

	Menu(const Menu&);
	Menu& operator=(const Menu&);
};

#endif