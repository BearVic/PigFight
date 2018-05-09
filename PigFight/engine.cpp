#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"
#include "hud.h"
#include "collisionStrategy.h"

Engine::~Engine() {
  delete player;
  for(auto it:enemy){
    delete it;
  }
  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
  }
  level.reset();
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  viewport( Viewport::getInstance() ),
  menu(),
  uiEngine(UIEngine::getInstance()),
  soundMgr(SoundMgr::getInstance()),
  enemy(),
  player(new Player("Player")),
  strategies(),
  level(Level::getInstance()),
  // lights(),
  currentSprite(0),
  currentStrategy(0),
  makeVideo( false )
{
  clock.startClock();
  int bats_num = Gamedata::getInstance().getXmlInt("Bat/number");
  int aliens_num = Gamedata::getInstance().getXmlInt("Alien/number");
  int fireghosts_num = Gamedata::getInstance().getXmlInt("Fire/number");
  enemy.reserve(bats_num+aliens_num+fireghosts_num);
  // stars.reserve(stars_num+aliens_num);
  // sprites.reserve(bats_num+fireghosts_num);
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();

  for (int i = 0; i < fireghosts_num; ++i) {
    enemy.emplace_back(new SmartSprite("Fire",pos,w,h));
    player->attach(enemy.back());
    level.attach(enemy.back());
  }
  for (int i = 0; i < bats_num; ++i) {
    enemy.emplace_back(new SmartSprite("Bat",pos,w,h));
    player->attach(enemy.back());
    level.attach(enemy.back());
  }
  for (int i = 0; i < aliens_num; ++i) {
    enemy.emplace_back(new MultiSprite("Alien"));
    player->attach(enemy.back());
    level.attach(enemy.back());
  }
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );
  Viewport::getInstance().setObjectToTrack(player);

  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {

  uiEngine.draw();

  for(auto e:enemy){
    e->draw();
  }

  // std::stringstream strm;
  // strm << enemy.size() << " Enemies Remaining";
  // IoMod::getInstance().writeText(strm.str(),  30, 30,yellow);
  // strategies[currentStrategy]->draw();
  // if ( collision>0 ) {
  //   IoMod::getInstance().writeText("Oops: Collision", 500, 90);

  // }
  player->draw();
  // star->draw();
  // spinningStar->draw();

  viewport.draw();
  // lights.draw();

  if(player->getLives()<=0) {
    soundMgr[4];
    uiEngine.showLose();
    clock.pause();
  }
  Hud::getInstance().draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  uiEngine.update(ticks);
  checkForCollisions();
  for(auto e:enemy){
    e->update(ticks);
  }
  player->update(ticks);
  // lights.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % 2;
  if ( currentSprite ) {
    Viewport::getInstance().setObjectToTrack(player);
  }
  else {
    Viewport::getInstance().setObjectToTrack(enemy.back());
  }
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  bool first = true;
  // std::cout<<"menu play return to engine play"<<std::endl;
  // menu.play();

  while ( !done ) {

    while ( SDL_PollEvent(&event) ) {
      if(first){
        // soundMgr.startMusic();
      clock.pause();
      if(menu.play()) {done=true; break;}
      // soundMgr.stopMusic();
      clock.unpause();
      first = false;
    }
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if ( keystate[SDL_SCANCODE_L] ) {
          level.levelUp();
        }
        if ( keystate[SDL_SCANCODE_SPACE] ) {
          player->shoot();
        }
        if ( keystate[SDL_SCANCODE_E] ) {
          enemy[1]->explode();
        }
        if ( keystate[SDL_SCANCODE_B] ) {
          player->explode();
        }
        if ( keystate[SDL_SCANCODE_G] ) {
           player->setGodMode();
        }
        if ( keystate[SDL_SCANCODE_M] ) {
           soundMgr.toggleMusic();
        }
        if (keystate[SDL_SCANCODE_F1]) {
          Hud::getInstance().changeVisible();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // std::cout<<"after event loop"<<std::endl;



    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        player->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        player->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        player->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        player->down();
      }
      if (keystate[SDL_SCANCODE_K]) {
        player->setSpeedEnhance(2);
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  // std::cout<<"inside engine while done loop"<<std::endl;
  }
  return false;
}

void Engine::checkForCollisions() {
  auto it = enemy.begin();
  while ( it != enemy.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **it) ) {
      // if(!(*it)->isDead()){
      //   player->incrKills();
      //   player->beingAttack();
      //   (*it)->explode();
      // }

      if(!(*it)->isDead()){

        if(!player->isProtected()){
          player->beingAttack();
          (*it)->explode();
        }
      }
      // player->reduceLives();
      // SmartSprite* doa = *it;
      // player->detach(doa);
      // delete doa;
      // it = sprites.erase(it);
    }
    // else ++it;
    ++it;
  }
  // auto it2 = stars.begin();
  // while ( it2 != stars.end() ) {
  //   if ( strategies[currentStrategy]->execute(*player, **it2) ) {
  //     player->explode();
  //     break;
  //     // Drawable* doa = *it2;
  //     // player->detach(doa);
  //     // delete doa;
  //     // it2 = stars.erase(it2);
  //     // (*it2)->explode();
  //   }
  //   ++it2;
  // }
}
