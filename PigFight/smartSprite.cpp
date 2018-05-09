#include <cmath>
#include <random>
#include <functional>
#include "smartSprite.h"
#include "gamedata.h"
#include "renderContext.h"


float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

void SmartSprite::goLeft()  { turnLeft();setVelocityX( -fabs(getVelocityX()) );  }
void SmartSprite::goRight() { turnRight();setVelocityX( fabs(getVelocityX()) );  }
void SmartSprite::goUp()    { setVelocityY( -fabs(getVelocityY()) ); }
void SmartSprite::goDown()  { setVelocityY( fabs(getVelocityY()) );  }


SmartSprite::SmartSprite(const std::string& name, const Vector2f& pos, 
  int w, int h) :
  MultiSprite(name),
  playerPos(pos),
  playerWidth(w),
  playerHeight(h),
  currentMode(PATROL),
  spurtDistance(Gamedata::getInstance().getXmlFloat(name+"/spurtDistance")),
  basicSpeed(Gamedata::getInstance().getXmlFloat("Player/difficultLevel")),
  enemySpeedIncr(Gamedata::getInstance().getXmlFloat("Player/enemySpeedIncr")),
  level(Level::getInstance())
{}


SmartSprite::SmartSprite(const SmartSprite& s) : 
  MultiSprite(s),
  playerPos(s.playerPos),
  playerWidth(s.playerWidth),
  playerHeight(s.playerHeight),
  currentMode(s.currentMode),
  spurtDistance(s.spurtDistance),
  basicSpeed(s.basicSpeed),
  enemySpeedIncr(s.enemySpeedIncr),
  level(s.level)
{}

void SmartSprite::setPlayerPos(const Vector2f& p) { 
    playerPos = p; 
  }

// void SmartSprite::explode(){
//   MultiSprite::explode();
//   std::cout<<"boom"<<std::endl;
//   // throw std::string("Explosion in the SmartSprite;");
// }

// void SmartSprite::update(Uint32 ticks) { 
//   MultiSprite::update(ticks);
//   float x= getX()+getImage()->getWidth()/2;
//   float y= getY()+getImage()->getHeight()/2;
//   float ex= playerPos[0]+playerWidth/2;
//   float ey= playerPos[1]+playerHeight/2;
//   float distanceToEnemy = ::distance( x, y, ex, ey );

//   if  ( currentMode == PATROL ) {
//     if(distanceToEnemy < detectDistance) currentMode = SPURT;
//   }
//   else if  ( currentMode == SPURT ) {
//     if(distanceToEnemy > detectDistance) currentMode=PATROL;
//     else {
//       if ( x > ex ) goLeft();
//       if ( x < ex ) goRight();
//       if ( y > ey ) goUp();
//       if ( y < ey ) goDown();
//     }
//   }
// }

void SmartSprite::update(Uint32 ticks) { 
  MultiSprite::update(ticks);
  float x= getX()+getImage()->getWidth()/2;
  float y= getY()+getImage()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );

  // Vector2f spurtVel = Vector2f(0,0);

  if  ( currentMode == PATROL ) {
    // if(distanceToEnemy < detectDistance){
    //   setVelocityX((getVelocityX()>0?1:-1)*basicSpeed[0]*0.8);
    //   setVelocityY((getVelocityY()>0?1:-1)*basicSpeed[1]*0.8);
    //   currentMode = DETECTION;
    // }
    if(distanceToEnemy < spurtDistance) {
      if(ex == x){
        setVelocityX(0);
        setVelocityY((ey<y?-1:1)*basicSpeed*2);
      }else if(ey == y){
        setVelocityY(0);
        setVelocityX((ex<x?-1:1)*basicSpeed*2);
      }else{
        setVelocityX((ex<x?-1:1)*basicSpeed*2);
        setVelocityY(getVelocityX()*(ey-y)/(ex-x));
        float m = getVelocity().magnitude();
        if(m>100){
          setVelocity(getVelocity()/m*basicSpeed*2);
        }
      }
      
      currentMode = SPURT;
    }
  }
  // else if  ( currentMode == DETECTION ) {
  //   if(distanceToEnemy > detectDistance){
  //     setVelocityX((getVelocityX()>0?1:-1)*basicSpeed[0]*0.3);
  //     setVelocityY((getVelocityY()>0?1:-1)*basicSpeed[1]*0.3);
  //     currentMode = PATROL;
  //   }
  //   else if(distanceToEnemy < spurtDistance){ 
  //     setVelocityX(getVelocityX()*2);
  //     setVelocityY(getVelocityY()*2);
  //     currentMode=SPURT;
  //   }else {
  //     if ( x > ex ) goLeft();
  //     if ( x < ex ) goRight();
  //     if ( y > ey ) goUp();
  //     if ( y < ey ) goDown();
  //   }
  // }
  else if( currentMode == SPURT){
    // if(distanceToEnemy > detectDistance) {
    //   setVelocityX((getVelocityX()>0?1:-1)*basicSpeed[0]*0.3);
    //   setVelocityY((getVelocityY()>0?1:-1)*basicSpeed[1]*0.3);
    //   currentMode = PATROL;
    // }
    // else 
      if(distanceToEnemy > spurtDistance){
      setVelocityX((getVelocityX()>0?1:-1)*basicSpeed*0.8);
      setVelocityY((getVelocityY()>0?1:-1)*basicSpeed*0.8);
      currentMode = PATROL;
    }
  }
}
