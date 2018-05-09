#ifndef HOMEWORK2_H
#define HOMEWORK2_H

#include "vector2f.h"
#include <SDL2/SDL.h>

class Circle{
public:
	Circle();
	Circle(float xCenter, float yCenter, float ra, 
	int r, int g, int b);
	Circle(const Circle& c);
	void drawCircle(SDL_Renderer* renderer);
	// void setCenter(float x, float y);
	void setColor(float r, float g, float b, float a);
	float &operator[](int index);
private:
	Vector2f center;
	float radius;
	SDL_Color color;

};

class Triangle{
public:
	Triangle(float x0, float y0, int elen, int r, int g, int b);
	void drawTriangle(SDL_Renderer* renderer);
	void setV0(float x, float y);
	void setColor(float r, float g, float b, float a);
private:
	int edge_len;
    Vector2f v0;
    SDL_Color color;
};

class CircleTree{
public:
	CircleTree();
	// CircleTree(const Circle& l, const Vector2f& c, float h);
	// CircleTree(float ra, const Vector2f& ct, float h, SDL_Color c);
	CircleTree(float ra, const Vector2f& ct, float h, SDL_Color c);
	void drawCircleTree(SDL_Renderer* renderer);
	void drawTrunk(SDL_Renderer* renderer);
	void drawLeaves(SDL_Renderer* renderer);
	void setColor(SDL_Color c);
	// void setCenter(float x, float y);
	float &operator[](int index);

private:
	float radius;
	Circle leaves;
	// Vector2f center;
	float trunk_height;
	SDL_Color color;
};

class TriTree{
public:
	TriTree();
	// CircleTree(const Circle& l, const Vector2f& c, float h);
	TriTree(float ra, const Vector2f& ct, float h, SDL_Color c);
	void drawTriTree(SDL_Renderer* renderer);
	void drawTriTrunk(SDL_Renderer* renderer);
	void drawTriLeaves(SDL_Renderer* renderer);
	void setColor(SDL_Color c);
	void setCenter(float x, float y);

private:
	float radius;
	Triangle leaves;
	Vector2f center;
	float trunk_height;
	SDL_Color color;

};

class Background{
public:
	void drawSky(SDL_Renderer* renderer);
	void drawHill(SDL_Renderer* renderer);
	void drawOrchard(SDL_Renderer* renderer);
};


extern const int WIDTH;
extern const int HEIGHT;

#endif