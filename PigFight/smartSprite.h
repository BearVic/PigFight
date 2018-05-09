#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include <string>
#include "multisprite.h"
#include "level.h"

class SmartSprite : public MultiSprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, int w, int h);
  SmartSprite(const SmartSprite&);
  virtual ~SmartSprite() { } 

  virtual void update(Uint32 ticks);
  void setPlayerPos(const Vector2f&);

  void sLevelUp(){
    spurtDistanceLevelUp();
    speedLevelUp();
  }
  
  // virtual void explode();

private:
  enum MODE {PATROL, DETECTION, SPURT};
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  // float safeDistance;
  // float detectDistance;
  float spurtDistance;
  int basicSpeed;
  int enemySpeedIncr;
  Level& level;

  void goLeft();
  void goRight();
  void goUp();
  void goDown();

  void spurtDistanceLevelUp(){ spurtDistance =  spurtDistance>600?600:spurtDistance+50;}
  void speedLevelUp(){basicSpeed += enemySpeedIncr;}
};
#endif
