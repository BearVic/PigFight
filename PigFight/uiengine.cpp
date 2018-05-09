#include "uiengine.h"
#include "multisprite.h"
#include "level.h"
#include "clock.h"
#include <SDL.h>

UIEngine::UIEngine():
    rc(RenderContext::getInstance()),
    io(IoMod::getInstance()),
    // clock(Clock::getInstance()),
    renderer(rc.getRenderer()),
    floor1st("floor1st", Gamedata::getInstance().getXmlInt("floor1st/factor") ),
    floor2nd("floor2nd", Gamedata::getInstance().getXmlInt("floor2nd/factor") ),
    ground("ground", Gamedata::getInstance().getXmlInt("ground/factor") ),
    alienScene(),
    godMode(false),
    level(Level::getInstance()),
    lives(5),
    kills(0),
    time(0),
    ptcTicks(0),
    livesIcon(ImageFactory::getInstance().getImage("Maple"))
    {
    	int number = 10;
    	alienScene.reserve(number);
        for (int i = 0; i < number; ++i) {
        	MultiSprite* temp = new MultiSprite("Alien");
        	float random = (rand()%15)/20+0.5;
        	temp->setScale(random);
        	temp->setVelocity(temp->getVelocity()*random);
        	alienScene.emplace_back(temp);
        }

    }

UIEngine::~UIEngine(){
	for(auto it:alienScene){
		delete it;
	}
}

void UIEngine::showLose() const {

    std::string fontFile = "./fonts/contrast.ttf";
    int fontSize = 100;
    SDL_Color white = {255,0,0,0};
    drawLives(120,20);
    io.writeText("You Lose!" , fontFile, white, fontSize, 80, 200);
    io.writeText("Level: " + std::to_string(level.getLevel()), fontFile, white, fontSize, 80, 300);
    io.writeText("Kills: " + std::to_string(kills), fontFile, white, fontSize, 80, 400);
    io.writeText("press R to restart or Q quit",  170, 500);
}



void UIEngine::drawLives(int x, int y) const{
    for (int i = 0;i < lives;++i){
        livesIcon->draw(0,0,x+i*livesIcon->getWidth()*0.7,y);
    }
}

void UIEngine::draw() const{
    floor1st.draw();
    for(auto alien:alienScene){
        alien->draw();
    }
    floor2nd.draw();
    ground.draw();
	SDL_Color white = {255,255,255,0};
	// int temp = (Clock::getInstance().getTicks());
	io.writeText("Lives: ", 3, white, 30,30);
    drawLives(120,20);
	io.writeText("Level: " + std::to_string(level.getLevel()), 3, white, 30,65);
	io.writeText("Kills: " + std::to_string(kills), 3, white, 30,90);
    io.writeText("Time: " + std::to_string(time), 3, white, 30,115);
	io.writeText("Protected: " + std::to_string(ptcTicks), 3, white, 30,145);
    if(godMode)
        io.writeText("God Mode", 3, white, 30,175);
}

void UIEngine::update(Uint32 ticks){
	time = Clock::getInstance().getTicks();
    if(int(time/5000)>level.getLevel()) level.levelUp();
	floor1st.update();
	for(auto alien:alienScene){
		alien->update(ticks);
	}
	floor2nd.update();
	ground.update();
}
