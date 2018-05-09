#ifndef PLAYER__H
#define PLAYER__H

#include "multisprite.h"
#include "vector2f.h"
#include <list>
#include "smartSprite.h"
#include "bulletpool.h"
#include "uiengine.h"
#include "lights.h"
#include "soundMgr.h"

// class SmartSprite;
// class BulletPool;

class Player : public MultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&);

  void right();
  void left();
  void up();
  void down();
  void stop();

  // void changeState();
  void checkJump();
  void advanceFrame(Uint32 ticks);

  // void attach( SmartSprite* o ) { observers.push_back(o); } 
  // void detach( SmartSprite* o );

  void attach( Drawable* o ) { observers.push_back(o); } 
  void detach( Drawable* o );  

  void shoot();
  void checkColled();

  void incrLives(){if(lives<maxLives) ++lives; uiengine.setLives(lives);}
  void reduceLives(){if(lives>0) --lives; uiengine.setLives(lives);}

  void incrKills() {++kills; uiengine.setKills(kills);}

  int getLives() {return lives;}

  void setSpeedEnhance(float ratio){speedEnhance = ratio;}

  void beingAttack();

  bool isProtected(){return isPtc;}
  void setPtc(){ isPtc=true; ptcTicks = 100; }

  unsigned int bulletCount() const{return bullets.bulletCount();}
  unsigned int freeCount() const{return bullets.freeCount();}

  void setGodMode();

private:

  std::string bulletName;
  BulletPool bullets;

  float speedEnhance;
  int maxLives;
  int lives;
  int kills;
  UIEngine& uiengine;
  SoundMgr& soundMgr;

  bool collision;
  bool isJump;
  bool isLeft;
  int jump_height;
  int ground_height;

  Lights lights;

  bool isPtc;
  int ptcTicks;

  bool godMode;
  
  std::vector<Image *> idle_left;
  std::vector<Image *> idle_right;
  std::vector<Image *> jump_left;
  std::vector<Image *> jump_right;
  std::vector<Image *> under_attack;
  
  Vector2f initialVelocity;

  // std::list<SmartSprite*> observers;
  std::list<Drawable*> observers;
};
#endif
