#include <string>
#include <iostream>
#include <ctime>
#include "soundMgr.h"
using std::string;

SoundMgr::~SoundMgr() {
  std::cout << "Cleaning up sounds ..." << std::endl;
  std::clock_t start = std::clock();
  Mix_HaltMusic();
  Mix_FreeMusic(op);
  for (unsigned int i = 0; i < sounds.size(); ++i) {
    Mix_FreeChunk(sounds[i]);
  }
  Mix_CloseAudio();
  std::clock_t duration = std::clock() - start;
  std::cout << "Clean up took " << duration << " ticks\n";
}

SoundMgr::SoundMgr() : 
  volume(SDL_MIX_MAXVOLUME/10), 
  currentSound(-1), 
  op(nullptr),
  audioRate(22050), 
  audioChannels(2), 
  audioBuffers(4096),
  sounds(),
  channels() 
{

  if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, 
                   audioBuffers)){
    throw string("Unable to open audio!");
  }
  op = Mix_LoadMUS("sound/Leafre.ogg");
  // Need to install midi to play the following:
  // music = Mix_LoadMUS("sound/ballad2.mid");
  if (!op) throw string("Couldn't load op or bgm ")+Mix_GetError();

  startMusic();

  sounds.push_back( Mix_LoadWAV("sound/jump.wav") );
  sounds.push_back( Mix_LoadWAV("sound/shoot.wav") );
  sounds.push_back( Mix_LoadWAV("sound/kitenemy.wav") );
  sounds.push_back( Mix_LoadWAV("sound/beingattack.wav") );
  sounds.push_back( Mix_LoadWAV("sound/lose.wav") );
  for (unsigned int i = 0; i < sounds.size(); ++i) channels.push_back(i);
  std::cout << "Music and Sound is loaded" << std::endl;
}

void SoundMgr::toggleMusic() {
  if( Mix_PausedMusic() ) { 
    Mix_ResumeMusic(); 
  } 
  else { 
    Mix_PauseMusic(); 
  } 
}

void SoundMgr::operator[](int index) {
  if (currentSound >= 0) Mix_HaltChannel(currentSound);
  currentSound = index;
  Mix_VolumeChunk(sounds[index], volume);
  channels[index] = 
     Mix_PlayChannel(-1, sounds[index], 0);
}

void SoundMgr::startMusic() {
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(op, -1);
}

void SoundMgr::stopMusic() {
  Mix_HaltMusic();
  Mix_FreeMusic(op);
}

