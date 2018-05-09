#ifndef UIENGINE__H
#define UIENGINE__H

#include "renderContext.h"
#include "ioMod.h"
#include "world.h"
#include <vector>
#include "drawable.h"
#include <SDL.h>
// #include "clock.h"
#include "level.h"
#include "image.h"


class UIEngine
{
public:
	static UIEngine& getInstance(){
		static UIEngine instance;
		return instance;
	}
	~UIEngine();

	void draw() const;
	void update(Uint32);
	void drawLives(int, int) const;
	void setLives(int n){lives = n;}
	void setKills(int x){kills = x;}
	void setPtcTicks(int t){ptcTicks = t;}

	void setGodMode(bool mode){ godMode = mode; }

	void showLose() const;

	UIEngine(const UIEngine&) = delete;
	UIEngine& operator=(const UIEngine&) = delete;
	
private:
	UIEngine();

	const RenderContext& rc;
	const IoMod& io;
	// Clock& clock;

	SDL_Renderer* const renderer;

	World floor1st;
	World floor2nd;
	World ground;
	std::vector<Drawable*> alienScene;

	bool godMode;
	Level& level;
	int lives;
	int kills;
	int time;
	int ptcTicks;
	Image* livesIcon;
};

#endif