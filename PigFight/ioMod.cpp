#include <SDL_image.h>
#include "ioMod.h"
#include "gamedata.h"
#include "renderContext.h"

IoMod& IoMod::getInstance() {
  static IoMod instance;
  return instance;
}

IoMod::~IoMod() {
  TTF_CloseFont(font1);
  TTF_CloseFont(font2);
  TTF_CloseFont(font3);
  TTF_Quit();
}

IoMod::IoMod() :
  init(TTF_Init()),
  renderer( RenderContext::getInstance().getRenderer() ),
  font1(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font/file1").c_str(),
                    Gamedata::getInstance().getXmlInt("font/size1"))),
  font2(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font/file2").c_str(),
                    Gamedata::getInstance().getXmlInt("font/size2"))),
  font3(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font/file3").c_str(),
                    Gamedata::getInstance().getXmlInt("font/size3"))),
  textColor({0xff, 0, 0, 0})
{
  if ( init == -1 ) {
    throw std::string("error: Couldn't init font");
  }
  if (font1 == NULL) {
    throw std::string("error: font1 not found");
  }
  if (font2 == NULL) {
    throw std::string("error: font2 not found");
  }
  if (font3 == NULL) {
    throw std::string("error: font3 not found");
  }
  textColor.r = Gamedata::getInstance().getXmlInt("font/red");
  textColor.g = Gamedata::getInstance().getXmlInt("font/green");
  textColor.b = Gamedata::getInstance().getXmlInt("font/blue");
  textColor.a = Gamedata::getInstance().getXmlInt("font/alpha");
}

SDL_Texture* IoMod::readTexture(const std::string& filename) {
  SDL_Texture *texture = IMG_LoadTexture(renderer, filename.c_str());
  if ( texture == NULL ) {
    throw std::string("Couldn't load ") + filename;
  }
  return texture;
}

SDL_Surface* IoMod::readSurface(const std::string& filename) {
  SDL_Surface *surface = IMG_Load(filename.c_str());
  if ( !surface ) {
    throw std::string("Couldn't load ") + filename;
  }
  return surface;
}

void IoMod::writeText(const std::string& msg, int x, int y) const {
  SDL_Surface* surface =
    TTF_RenderText_Solid(font1, msg.c_str(), textColor);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

void IoMod::writeText(const std::string& msg, int type, SDL_Color c, int x, int y) const {
  TTF_Font* font = nullptr;
  switch(type){
    case 1: font = font1; break;
    case 2: font = font2; break;
    case 3: font = font3; break;
    default: throw std::runtime_error("Wrong Font type!! ");break;
  }

  SDL_Surface* surface =
    TTF_RenderText_Solid(font, msg.c_str(), c);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}


void IoMod::writeText(const std::string& msg, const std::string& fontFile,
                        SDL_Color color, int fontSize, int x, int y) const
{
    //Open the font
    TTF_Font *font = nullptr;
    font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr)
        throw std::runtime_error("Failed to load font: " + fontFile + TTF_GetError());
    
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    SDL_Surface* surface =
    TTF_RenderText_Solid(font, msg.c_str(), color);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_FreeSurface(surface);
    SDL_Rect dst = {x, y, textWidth, textHeight};

    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
  
    //Render the message to an SDL_Surface, as that's what TTF_RenderText_X returns
    // SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
    // SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
    //Clean up unneeded stuff
    // SDL_FreeSurface(surf);
    TTF_CloseFont(font);
}


void IoMod::writeText(const std::string& msg, int x, int y, SDL_Color c) const {
  SDL_Surface* surface =
    TTF_RenderText_Solid(font1, msg.c_str(), c);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}
