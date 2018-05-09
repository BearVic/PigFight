#include <iostream>
#include <cmath>
#include "bullet.h"

void Bullet::update(Uint32 ticks) { 
  Vector2f pos = getPosition();
  Sprite::update(ticks);
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
}

Bullet::Bullet(const string& name, const Vector2f& pos, const Vector2f& vel,Image* image) :
    Sprite(name,pos,vel, image), 
    distance(0), 
    maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
    tooFar(false) 
  { }
