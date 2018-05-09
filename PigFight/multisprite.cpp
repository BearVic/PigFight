#include "multisprite.h"
#include "gamedata.h"
#include "imageFactory.h"
#include "explodingSprite.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Vector2f MultiSprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);;
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);;
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

MultiSprite::~MultiSprite( ) { if (explosion) delete explosion; }

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
					 makeVelocity(Gamedata::getInstance().getXmlInt("Player/difficultLevel"),
					          Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  run_left(ImageFactory::getInstance().getImages(name+"Left")),
  run_right(ImageFactory::getInstance().getImages(name+"Right")),
  cur_state( run_right ),
  explosion(nullptr),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"Left/frames")),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  dead(false)
{ if(name=="Player") std::cout<<name<<std::endl; }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s),
  run_left(s.run_left),
  run_right(s.run_right),
  cur_state(s.cur_state),
  explosion(s.explosion),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  dead(s.dead)
  { }

MultiSprite& MultiSprite::operator=(const MultiSprite& s) {
  Drawable::operator=(s);
  run_left = (s.run_left);
  run_right = (s.run_right);
  cur_state = (s.cur_state);
  explosion = s.explosion;
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  dead = s.dead;
  return *this;
}

// void MultiSprite::draw() const {
//   cur_state[currentFrame]->draw(getX(), getY(), getScale());
// }

void MultiSprite::explode() {
  if ( !explosion ) {
    // std::cout<<getName()<<std::endl;
    dead = true;
    Sprite 
    sprite(getName(), getPosition(), getVelocity(), cur_state[0]);
    explosion = new ExplodingSprite(sprite);
  }
}

void MultiSprite::speedLevelUp(){

  setVelocityX(getVelocityX()+30);
  setVelocityY(getVelocityY()+30);

}

void MultiSprite::draw() const { 
  if ( explosion ) explosion->draw();
  else cur_state[currentFrame]->draw(getX(), getY(), getScale());
}

void MultiSprite::update(Uint32 ticks) {
  
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      dead = false;
      setPosition(Vector2f
        (Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"),
          Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y")));
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);


  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    turnRight();
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    turnLeft();
    setVelocityX( -fabs( getVelocityX() ) );
  }

  

}
