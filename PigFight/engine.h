#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include <vector>
#include "smartSprite.h"
// #include "collisionStrategy.h"
#include "menu.h"
#include "uiengine.h"
#include "soundMgr.h"
// #include "lights.h"
#include "level.h"

class CollisionStrategy;
// class SmartSprite;
class Player;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  void switchSprite();
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

private:
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  // World world;
  Viewport& viewport;

  Menu menu;
  UIEngine& uiEngine;

  SoundMgr& soundMgr;

  std::vector<Drawable*> enemy;
  // std::vector<Drawable*> stars;
  // std::vector<SmartSprite*> sprites;
  Player* player;
  // Drawable* spinningStar;
  std::vector<CollisionStrategy*> strategies;

  Level& level;

  // Lights lights;
  int currentSprite;

  
  int currentStrategy;

  bool makeVideo;

  void draw() const;
  void update(Uint32);

  // Engine(const Engine&);
  // Engine& operator=(const Engine&);
  void printScales() const;
  void checkForCollisions();
};
