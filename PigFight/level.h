#ifndef LEVEL__H
#define LEVEL__H

#include <list>
#include "drawable.h"

class Level{
public:
  static Level& getInstance(){
    static Level instance;
    return instance;
  }
    void setLevelTo(int x){level = x;}
    int getLevel(){return level;}

    void attach( Drawable* o ) { observers.push_back(o); }
    void detach( Drawable* o );
  void levelUp();
  void reset(){level = 0; observers.clear();}

    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;
private:
  int level;
  std::list<Drawable*> observers;
  Level():level(0),observers(){}
};
#endif
