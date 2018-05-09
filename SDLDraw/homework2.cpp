#include <SDL2/SDL.h>
#include "homework2.h"
#include "vector2f.h"
#include <stdlib.h>
#include <time.h>

// ================= Circle Class =================== 

Circle::Circle():center(320.0, 240.0), radius(10.0){
    color.r = 0;
    color.g = 0;
    color.b = 255;
    color.a = 255;
}

Circle::Circle(float xCenter, float yCenter, float ra, 
	int r, int g, int b):
  center(xCenter, yCenter),
  radius(ra){
  	color.r = r;
  	color.g = g;
  	color.b = b;
  	color.a = 255;
  }

Circle::Circle(const Circle& c): center(c.center),
radius(c.radius), color(c.color){};

// void Circle::setCenter(float x, float y){
// 	center[0] = x;
// 	center[1] = y;
// }

void Circle::setColor(float r, float g, float b, float a){
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

void Circle::drawCircle(SDL_Renderer* renderer) {
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w; // horizontal offset
      int dy = radius - h; // vertical offset
      SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255/2);
      if ((dx*dx + dy*dy) <= (radius * radius)) {
        SDL_RenderDrawPoint(renderer, center[0] + dx, center[1] + dy);
      }
    }
  }
}

float &Circle::operator[](int index){
	return center[index];
}
// ================= Circle Class ===================

// ================= Triangle Class ===================

Triangle::Triangle(float x0, float y0, int elen, int r, int g, int b): 
edge_len(elen), v0(x0,y0){
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = 255;
};

void Triangle::setV0(float x, float y){
	v0[0] = x;
	v0[1] = y;
}

void Triangle::setColor(float r, float g, float b, float a){
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}
 
void Triangle::drawTriangle(SDL_Renderer* renderer){
	float n = 0.0;
 	SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, 255/2);
 	for (int i = 0; i < edge_len; ++i)
 	{
 		n = (edge_len-i)/2.0;
 		SDL_RenderDrawLine(renderer, v0[0]-n, v0[1]+i, v0[0]+n, v0[1]+i);
 	}
 }

// ================= Triangle Class ===================

// ================= CircleTree Class ===================

CircleTree::CircleTree(float ra, const Vector2f& ct, float h, SDL_Color c):
radius(ra), leaves(ct[0], ct[1], ra/5, c.r, c.g, c.b), 
trunk_height(h), color(c){}

void CircleTree::setColor(SDL_Color c){
	color = c;
	leaves.setColor(c.r, c.g, c.b, c.a);
}

// void CircleTree::setCenter(float x, float y){
// 	center[0] = x;
// 	center[1] = y;
// 	leaves.setCenter(x, y);
// }

void CircleTree::drawTrunk(SDL_Renderer* renderer){
	SDL_Rect r;
	// r.x = center[0] - trunk_height/8;
	// r.y = center[1];
	r.x = leaves[0] - trunk_height/8;
	r.y = leaves[1];
	r.w = trunk_height/4;
	r.h = trunk_height;

  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, 185, 124, 60, 255);

  // Render rect
  SDL_RenderFillRect( renderer, &r );

  // To set opacity to half: 255/2
  // SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255/2 );
  SDL_RenderDrawRect( renderer, &r );
}

void CircleTree::drawLeaves(SDL_Renderer* renderer){
	srand(time(0));
	Vector2f center;
	center[0] = leaves[0];
	center[1] = leaves[1];

	for (int w = 0; w < radius * 2; w++) {
		for (int h = 0; h < radius * 2; h++) {
		int dx = radius - w; // horizontal offset
        int dy = radius - h; // vertical offset
        // SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        if ((dx*dx + dy*dy) <= (radius * radius) && (rand()%100>98)) {
      	  // leaves.setCenter(leaves[0] + dx,leaves[1] + dy);
      	  leaves[0] = center[0] + dx;
      	  leaves[1] = center[1] + dy;
      	  leaves.drawCircle(renderer);
          // SDL_RenderDrawPoint(renderer, center[0] + dx, center[1] + dy);
      	}
      }
    }
    leaves[0] = center[0];
    leaves[1] = center[1];
}

void CircleTree::drawCircleTree(SDL_Renderer* renderer){
	drawTrunk(renderer);
	drawLeaves(renderer);
}
float &CircleTree::operator[](int index){
	return leaves[index];
}

// ================= CircleTree Class ===================

// ================= TriangleTree Class ===================

TriTree::TriTree(float ra, const Vector2f& ct, float h, SDL_Color c):
radius(ra), leaves(ct[0], ct[1], ra/5, c.r, c.g, c.b), 
center(ct[0],ct[1]), trunk_height(h), color(c){}

void TriTree::setColor(SDL_Color c){
	color = c;
	leaves.setColor(c.r, c.g, c.b, c.a);
}

void TriTree::setCenter(float x, float y){
	center[0] = x;
	center[1] = y;
	leaves.setV0(x, y);
}

void TriTree::drawTriTrunk(SDL_Renderer* renderer){
	SDL_Rect r;
	r.x = center[0] - trunk_height/8;
	r.y = center[1];
	r.w = trunk_height/4;
	r.h = trunk_height;

  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, 185, 124, 60, 255);

  // Render rect
  SDL_RenderFillRect( renderer, &r );

  // To set opacity to half: 255/2
  // SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255/2 );
  SDL_RenderDrawRect( renderer, &r );
}

void TriTree::drawTriLeaves(SDL_Renderer* renderer){
	
	srand(time(0));

	for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w; // horizontal offset
      int dy = radius - h; // vertical offset
      // SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
      if ((dx*dx + dy*dy) <= (radius * radius) && (rand()%30>28)) {
      	leaves.setV0(center[0] + dx,center[1] + dy);
      	leaves.drawTriangle(renderer);
      }
    }
  }
}

void TriTree::drawTriTree(SDL_Renderer* renderer){	
	drawTriTrunk(renderer);
	drawTriLeaves(renderer);
}

// ================= TriangleTree Class ===================

// ================= Background Class ===================

void Background::drawSky(SDL_Renderer* renderer){
	for (int i = 0; i < WIDTH; ++i)
	{
		SDL_SetRenderDrawColor( renderer, 6.0+148.0/WIDTH*i, 
			43+176.0/WIDTH*i, 123.0+129.0/WIDTH*i, 255 );
		SDL_RenderDrawLine(renderer, i, 0, i, HEIGHT);
	}
}

void Background::drawHill(SDL_Renderer* renderer){
	int y = 0;
	for (int i = 0; i < WIDTH; ++i)
	{
		y = 200.0 / (1.0 + exp(-(i-320)/50.0))+200;
		SDL_SetRenderDrawColor(renderer, 58.0+57.0/WIDTH*i, 
			120.0+73.0/WIDTH*i, 28.0+26.0/WIDTH*i, 255);
		for (int j = y; j < HEIGHT; ++j)
		{
			SDL_RenderDrawPoint(renderer, i, j);	
		}
  	}
}

void Background::drawOrchard(SDL_Renderer* renderer){
	  SDL_Color red;
  red.r = 255;
  red.g = 0;
  red.b = 0;
  red.a = 255;

  SDL_Color blue;
  blue.r = 0;
  blue.g = 0;
  blue.b = 255;
  blue.a = 255;

  SDL_Color orange;
  orange.r = 254;
  orange.g = 114;
  orange.b = 34;
  orange.a = 255;

  SDL_Color yellow;
  yellow.r = 246;
  yellow.g = 176;
  yellow.b = 56;
  yellow.a = 255;

  SDL_Color purple;
  purple.r = 0x0046;
  purple.g = 0x0011;
  purple.b = 0x007a;
  purple.a = 255;

  Vector2f position(80, 200);
  float tree_radius = 60;
  float tree_height = 100;
  CircleTree ct(tree_radius, position, tree_height, red);
  ct.drawCircleTree(renderer);

  TriTree tt(tree_radius, position, tree_height, orange);
  tt.setCenter(240, 230);
  tt.drawTriTree(renderer);

  // ct.setCenter(400, 350);
  ct[0] = 400;
  ct[1] = 350;
  ct.setColor(yellow);
  ct.drawCircleTree(renderer);

  tt.setCenter(270, 320);
  tt.setColor(purple);
  tt.drawTriTree(renderer);

  tt.setCenter(130, 340);
  tt.setColor(blue);
  tt.drawTriTree(renderer);  
}

// ================= Background Class ===================