#ifndef BULLET__H
#define BULLET__H

#include <iostream>
#include "sprite.h"
#include "gamedata.h"

class Bullet : public Sprite {
public:
  Bullet(const string& name, const Vector2f& pos, const Vector2f& vel, Image* image);

  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }

private:
  float distance;
  float maxDistance;
  bool tooFar;
};

#endif
