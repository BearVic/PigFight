#include <SDL2/SDL.h>
#include "frameGenerator.h"
#include "vector2f.h"
#include <iostream>
#include "homework2.h"
#include <math.h>

const std::string NAME = "linxiol";
const int WIDTH = 640;
const int HEIGHT = 480;

/*
  I write my most codes in "homework2.h" and "homework2.cpp"
  I creates some classes such as CircleTree and Background;
  And I overload operator[] in CircleTree class.
*/

int main(void) {

  if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) {
    std::cout << "Failed to initialize SDL2" << std::endl;
    return EXIT_FAILURE;
  }
  SDL_Window* window = SDL_CreateWindow(
      "Drawing a Triangle", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      WIDTH, HEIGHT, SDL_WINDOW_SHOWN
  );

  SDL_Renderer* renderer = SDL_CreateRenderer( 
    window, -1, SDL_RENDERER_ACCELERATED
  );



  SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
  SDL_RenderClear(renderer);

  // ===================================================

  Background bg;
  bg.drawSky(renderer);
  bg.drawHill(renderer);
  bg.drawOrchard(renderer);

  // ===================================================

  SDL_RenderPresent(renderer);
  FrameGenerator frameGen(renderer, window, WIDTH, HEIGHT, NAME);
  frameGen.makeFrame();

  SDL_Event event;
  const Uint8* keystate;
  while ( true ) {
    keystate = SDL_GetKeyboardState(0);
    if (keystate[SDL_SCANCODE_ESCAPE]) { break; }
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        break;
      }
    }
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
