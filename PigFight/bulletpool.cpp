#include <iostream>
#include <cmath>
#include "gamedata.h"
#include "bullet.h"
#include "imageFactory.h"
#include "bulletpool.h"
#include "ioMod.h"

BulletPool::~BulletPool(){
	delete strategy;
}

BulletPool::BulletPool(const std::string& name):
  name(name),
  image(ImageFactory::getInstance().getImage(name)),
  interval(Gamedata::getInstance().getXmlInt(name+"/interval")),
  timeSinceLastFrame(0),
  activeBullets(),
  freeBullets(),
  strategy(new RectangularCollisionStrategy)
  {}

BulletPool::BulletPool(const BulletPool& b):
  name(b.name),
  image(b.image),
  interval(b.interval),
  timeSinceLastFrame(b.timeSinceLastFrame),
  activeBullets(b.activeBullets),
  freeBullets(b.freeBullets),
  strategy(b.strategy)
  {}

BulletPool& BulletPool::operator=(const BulletPool& b){
	name = (b.name);
    image = (b.image);
    interval = (b.interval);
  timeSinceLastFrame = (b.timeSinceLastFrame);
  activeBullets = (b.activeBullets);
  freeBullets = (b.freeBullets);
  strategy = b.strategy;
  return *this;
}

void BulletPool::shoot(const Vector2f& pos, const Vector2f& vel){
	if(timeSinceLastFrame > interval){
		if(freeBullets.empty()){
			Bullet b(name, pos, vel, image);
			activeBullets.push_back(b);
		}else{
			Bullet b = freeBullets.front();
			freeBullets.pop_front();
			b.reset();
			b.setPosition(pos);
			b.setVelocity(vel);
			activeBullets.push_back(b);
		}
	}
}

bool BulletPool::collidedWith(const Drawable* obj) {
		std::list<Bullet>::iterator it = activeBullets.begin();
		while(it!=activeBullets.end()){
			if(strategy->execute(*it, *obj)){
				freeBullets.push_back(*it);
				it = activeBullets.erase(it);
				return true;
			}
			++it;
		}
		return false;
	}

void BulletPool::draw() const{
	// IoMod::getInstance().writeText("activeBullets:"+std::to_string(bulletCount()), 600, 150 );
	// IoMod::getInstance().writeText("freeBullets:"+std::to_string(freeCount()), 600, 200 );
	for(const auto& b:activeBullets){
		b.draw();
	}
}

void BulletPool::update(Uint32 ticks){
    timeSinceLastFrame += ticks;
	std::list<Bullet>::iterator it = activeBullets.begin();
	while(it!=activeBullets.end()){
		it->update(ticks);
		if(it->goneTooFar()){
			freeBullets.push_back(*it);
			it = activeBullets.erase(it);
		}else{
			++it;
		}
	}
}
