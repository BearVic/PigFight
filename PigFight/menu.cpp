#include <iostream>
#include <algorithm>
#include <string>
#include "gamedata.h"
#include "menu.h"
#include "renderContext.h"


Menu::Menu():
    renderer(RenderContext::getInstance().getRenderer()),
    gdata(Gamedata::getInstance()),
    // menuColor({0,0,0,0}),c
    io(IoMod::getInstance()),
    soundMgr(SoundMgr::getInstance()),
	  options(),
    title("PigFight"),
	curIndicator(0),
	// indicatorPosX(300),
	// indicatorPosY(200),
	indicatorIcon(ImageFactory::getInstance().getImage("YellowStar")),
	icon(ImageFactory::getInstance().getImage("Maple")),
	screenWidth(gdata.getXmlInt("view/width")),
	screenHeight(gdata.getXmlInt("view/height"))
	{
		options.reserve(2);
		options.emplace_back("Start Game");
		options.emplace_back("Exit");
	}

void Menu::up(){
	curIndicator = curIndicator-1; 
	curIndicator = curIndicator>=0?curIndicator:curIndicator+2;

}

void Menu::down(){ 
	curIndicator = (curIndicator+1)%2; 
}

void Menu::draw() const {
  // soundMgr.
	SDL_RenderClear(renderer);
    // First set the blend mode so that alpha blending will work;
    // the default blend mode is SDL_BLENDMODE_NONE!
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Set the hud background color:
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, .9*255 );

    // ================Title Configure===================
    SDL_Color white = {255,255,255,0};
    std::string fontFile = "./fonts/contrast.ttf";
    int fontSize = 100;
    int titlePosY = screenHeight*0.2; 
    io.writeText(title, fontFile, white, fontSize, 180, titlePosY);
    icon->draw(550,titlePosY-80,3);

    io.writeText(options[0], screenWidth*0.4, screenHeight*0.6);
    io.writeText(options[1], screenWidth*0.4, screenHeight*0.7);
    // We have to draw the clickOn & clickOff relative to the screen,
    // and we don't want to offset by the location of the viewprot:
    indicatorIcon->draw(0, 0, screenWidth*0.35, screenHeight*0.6+screenHeight*0.1*curIndicator);
    SDL_RenderPresent(renderer);
}



bool Menu::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          return true;
        }
        if (keystate[SDL_SCANCODE_DOWN] ) down();
        if (keystate[SDL_SCANCODE_UP] ) up();
        if (keystate[SDL_SCANCODE_RETURN] ) {
          std::cout << "option chosen: " << curIndicator << std::endl;
          if ( curIndicator == 0 ) done = true;
          if (curIndicator == 1 ) return true;
        }
      }
    }
    // In this section of the event loop we allow key bounce:
    draw();
    //update(ticks);
  }
  return false;
}