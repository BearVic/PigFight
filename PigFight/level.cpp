#include "level.h"
#include "multisprite.h"
#include "smartSprite.h"



void Level::detach( Drawable* o ){
    	std::list<Drawable*>::iterator ptr = observers.begin();
    	while ( ptr != observers.end() ) {
    		if ( *ptr == o ) {
    		      ptr = observers.erase(ptr);
    		      return;
    		  }
    		  ++ptr;
    		}
    	}


void Level::levelUp(){
	++level;
	auto it = observers.begin();
	while(it != observers.end()){
		if(dynamic_cast<MultiSprite*> (*it)){
			dynamic_cast<MultiSprite*> (*it)->speedLevelUp();
		}else if(dynamic_cast<SmartSprite*> (*it)){
			dynamic_cast<SmartSprite*> (*it)->sLevelUp();
		}
		++it;
	}	
}

