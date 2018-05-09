#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class ExplodingSprite;

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&);
  MultiSprite(const MultiSprite&);
  ~MultiSprite();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const {
    return cur_state[currentFrame];
  }
  int getScaledWidth()  const {
    return getScale()*cur_state[currentFrame]->getWidth();
  }
  int getScaledHeight()  const {
    return getScale()*cur_state[currentFrame]->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return cur_state[currentFrame]->getSurface();
  }

  void turnLeft(){
    cur_state = run_left;
  }
  void turnRight(){
    cur_state = run_right;
  }

  virtual void explode();

  virtual bool isDead() {return dead;}

  void speedLevelUp();


protected:
  std::vector<Image *> run_left;
  std::vector<Image *> run_right;

  std::vector<Image *> cur_state;

  ExplodingSprite* explosion;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  bool dead;


  Vector2f makeVelocity(int, int) const;

  void advanceFrame(Uint32 ticks);
  MultiSprite& operator=(const MultiSprite&);
};
#endif
