#include "player.h"
#include <string>
#include "imageFactory.h"
#include "vector2f.h"
#include "bulletpool.h"
#include "explodingSprite.h"

Player::Player( const std::string& name) :
  MultiSprite(name),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bulletName")),
  bullets(bulletName),
  speedEnhance(1.0),
  maxLives(Gamedata::getInstance().getXmlInt(name+"/maxLives")),
  lives(maxLives),
  kills(0),
  uiengine(UIEngine::getInstance()),
  soundMgr(SoundMgr::getInstance()),
  collision(false),
  isJump(false),
  isLeft(false),
  jump_height(250),
  ground_height(500),
  lights(),
  isPtc(false),
  ptcTicks(0),
  godMode(false),
  idle_left(ImageFactory::getInstance().getImages(name+"IdleLeft")),
  idle_right(ImageFactory::getInstance().getImages(name+"IdleRight")),
  jump_left(ImageFactory::getInstance().getImages(name+"JumpLeft")),
  jump_right(ImageFactory::getInstance().getImages(name+"JumpRight")),
  under_attack(ImageFactory::getInstance().getImages(name+"UnderAttack")),
  initialVelocity(Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                           Gamedata::getInstance().getXmlInt(name+"/speedY"))),
  observers()
  { }

// Player::Player(const Player& s) :
//   MultiSprite(s),
//   bulletName(s.bulletName),
//   bullets(s.bullets),
//   speedEnhance(s.speedEnhance),
//   collision(s.collision),
//   isJump(s.isJump),
//   isLeft(s.isLeft),
//   jump_height(s.jump_height),
//   ground_height(s.ground_height),
//   idle_left(s.idle_left),
//   idle_right(s.idle_right),
//   jump_left(s.jump_left),
//   jump_right(s.jump_right),
//   initialVelocity(s.initialVelocity)
//   { }

// Player& Player::operator=(const Player& s) {
//   MultiSprite::operator=(s);
//   bulletName = s.bulletName;
//   bullets = s.bullets;
//   speedEnhance = s.speedEnhance;
//   collision = s.collision;
//   isJump = s.isJump;
//   isLeft = s.isLeft;
//   jump_height = s.jump_height;
//   ground_height = s.ground_height;
//   idle_left = s.idle_left;
//   idle_right = s.idle_right;
//   jump_left = s.jump_left;
//   jump_right = s.jump_right;
//   initialVelocity = s.initialVelocity;
//   return *this;
// }


void Player::stop() {
  //setVelocity( Vector2f(0, 0) );
  if(!isJump) cur_state = isLeft?idle_left:idle_right;
  setVelocityX( 0 );
  setVelocityY(0);
  setSpeedEnhance(1.0);
}

void Player::right() {
  cur_state = isJump?jump_right:run_right;
  isLeft = false;
  if ( getX() < worldWidth-getScaledWidth()) {
      setVelocityX(initialVelocity[0]);
  }
}
void Player::left()  {
  cur_state = isJump?jump_left:run_left;
  isLeft = true;
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
}
void Player::up()    {
  if(getY()>=ground_height) {
    soundMgr[0];
    isJump = true;
  }
  // if ( getY() > 0) {
  //   setVelocityY( -initialVelocity[1] );
  // }
}
void Player::down()  {
  // if(getY()==ground_height) isJump = false;
  // if ( getY() < worldHeight-getScaledHeight()) {
  //   setVelocityY( initialVelocity[1] );
  // }
  isJump = false;
}

void Player::checkJump(){
  if(isJump&&getY()>(jump_height)){
    setVelocityY( -initialVelocity[1]*speedEnhance );
  }else if(isJump&&getY()<=(jump_height)){
    isJump = false;
  }else if(!isJump&&getY()<ground_height){
    setVelocityY( initialVelocity[1]*speedEnhance );
  }else if(!isJump&&getY()>=ground_height){
    setVelocityY(0);
  }
}

void Player::shoot(){
  soundMgr[1];
  float x;
  if(!isLeft)
    x = getX()+getImage()->getWidth();
  else
    x = getX()-getImage()->getWidth();
  float y = getY()+getImage()->getHeight()/2;
  bullets.shoot(Vector2f(x,y),Vector2f(
    (isLeft?-1:1)*initialVelocity[0]*1.5, getVelocityY()));
}

void Player::advanceFrame(Uint32 ticks) {
  if(isJump){
    currentFrame = getY()-jump_height>ground_height-getY()?0:1;
  }else{
    timeSinceLastFrame += ticks;
    if (timeSinceLastFrame > frameInterval) {
      currentFrame = (currentFrame+1) % numberOfFrames;
      timeSinceLastFrame = 0;
    }
  }
}

// void Player::detach( SmartSprite* o ) {
//   std::list<SmartSprite*>::iterator ptr = observers.begin();
//   while ( ptr != observers.end() ) {
//     if ( *ptr == o ) {
//       ptr = observers.erase(ptr);
//       return;
//     }
//     ++ptr;
//   }
// }

void Player::detach( Drawable* o ) {
  std::list<Drawable*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::beingAttack(){
  cur_state = under_attack;
  incrKills();
  if(!godMode){
    soundMgr[3];
    reduceLives();
    setPtc();
  }
}

void Player::setGodMode(){

  if(godMode){
    godMode = false;
    uiengine.setGodMode(false);
  }else{
    godMode = true;
    uiengine.setGodMode(true);
  }

}

void Player::draw() const {
  MultiSprite::draw();
  if(isPtc)
    lights.draw();
  bullets.draw();
}

void Player::checkColled(){

  std::list<Drawable*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if(bullets.collidedWith(*ptr)){
      (*ptr)->explode();
      incrKills();
      soundMgr[2];
    }
    ++ptr;
  }
}

void Player::update(Uint32 ticks) {

  if(isPtc&&ptcTicks>0) {
    --ptcTicks;
    uiengine.setPtcTicks(ptcTicks);
  }
  else if(ptcTicks<=0) isPtc=false;

  // std::cout<<getVelocityX()<<std::endl;

  advanceFrame(ticks);
  checkColled();



  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    bullets.update(ticks);
    return;
  }

  checkJump();

  if(0<getX() && getX()<worldWidth-getScaledWidth())
    setVelocityX(getVelocityX()*speedEnhance);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  // changeState();
  // if ( !collision ) MultiSprite::update(ticks);

  std::list<Drawable*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    SmartSprite* temp = dynamic_cast<SmartSprite*>(*ptr);
    if(temp){
      temp->setPlayerPos( getPosition() );
    }
    ++ptr;
  }
  bullets.update(ticks);
  stop();

  if(isPtc)
    lights.update();

}

