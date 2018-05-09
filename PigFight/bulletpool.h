#ifndef BULLETPOOL__H
#define BULLETPOOL__H

#include <list>
#include "collisionStrategy.h"

class Bullet;

class BulletPool{
public:
	BulletPool(const std::string&);
	BulletPool(const BulletPool&);
	BulletPool& operator=(const BulletPool&);
	~BulletPool();
	void draw() const;
	void update(Uint32 ticks);
	void shoot(const Vector2f& pos, const Vector2f& vel);

	unsigned int bulletCount() const{return activeBullets.size();}
	unsigned int freeCount() const{return freeBullets.size();}

	bool shooting() const {return activeBullets.empty();}

	bool collidedWith(const Drawable* obj);

private:
	std::string name;
	Image* image;
	float interval;
	float timeSinceLastFrame;
	std::list<Bullet> activeBullets;
	std::list<Bullet> freeBullets;
	CollisionStrategy* strategy;
};

#endif