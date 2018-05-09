#include "hud.h"
#include <SDL2/SDL.h>
#include "ioMod.h"
#include "clock.h"

Hud& Hud::getInstance(){
    static Hud instance;
    return instance;
}

Hud::Hud():
  renderer( RenderContext::getInstance().getRenderer() ),
  window(),
  visible(false)
{
    window.x = Gamedata::getInstance().getXmlInt("Hud/Loc/x");
    window.y = Gamedata::getInstance().getXmlInt("Hud/Loc/y");
    window.w = Gamedata::getInstance().getXmlInt("Hud/width");
    window.h = Gamedata::getInstance().getXmlInt("Hud/height");
}

void Hud::changeVisible(){
    visible = (visible==true?false:true);
}

void Hud::draw(){

    if(!visible) return;

    // Set the color for the background:
    // SDL_SetRenderDrawColor( renderer, 208, 209, 210, 255 );

    // Clear winow
    // SDL_RenderClear( renderer );


    // First set the blend mode so that alpha blending will work;
    // the default blend mode is SDL_BLENDMODE_NONE!
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Now set the color for the hud:
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );

    // Render rect
    SDL_RenderFillRect( renderer, &window );

    // Now set the color for the outline of the hud:
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255/2 );


    SDL_RenderDrawRect( renderer, &window );

    if (Gamedata::getInstance().getXmlBool("fps")) {
      IoMod::getInstance().
        writeText("FPS: "+ std::to_string(Clock::getInstance().getFps()), window.x+30, window.y+10);
    }
    IoMod::getInstance().writeText("Control w: Jump  a: Left  d: Right  s: Down", window.x+30, window.y+40);
    IoMod::getInstance().writeText("F1: show/hide HUD  k: speed up", window.x+30, window.y+70);
    IoMod::getInstance().writeText("r: restart game  p: pause", window.x+30, window.y+100);
    IoMod::getInstance().writeText("g: setup God Mode  l: difficulty level up", window.x+30, window.y+130);
    IoMod::getInstance().writeText("space: shoot  q: quit game", window.x+30, window.y+160);
    IoMod::getInstance().writeText("m: turn on/off bgm", window.x+30, window.y+190);
    // SDL_Color color = {0,0,0,0};
    // IoMod::getInstance().writeText("Pig Fight!", color, 100, window.x+30, window.y+200);

}



