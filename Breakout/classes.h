#pragma once
#include <iostream>
#include "FrameWork.h"
#include <string>
#include <ctime>
using namespace std;

float clamp(float value, float min, float max) {
	return std::max(min, std::min(max, value));
}

float length(float x, float y)
{
	return sqrt(x * x + y * y);
}

float distance(float x1, float y1, float x2, float y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}


int screenWidth = 800;
int screenHeight = 600;

// Map
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int heightMap = 4;
const int widthMap = 8;


const int blocksInRow = 4;
int amountOfBlocks = 12;
int redBlocks = 1;
float blockPixelsHeight = screenHeight / 10.0;
float blockPixelsWidth = screenWidth / widthMap;

std::string mainMap[widthMap] =
{
	"        ",
	"  1111  ",
	"  1121  ",
	"  1111  ",
};

string newMap[widthMap] =
{
	"11111111",
	"11    11",
	" 211112 ",
	"  1111  ",
};



/// Vector
/////////////////////////////////////////////////////////////////////////////////////
class vector
{
private:
	float x, y;
public:

	float mod();
	vector();
	vector(float _x, float _y);
	float dotProduct(vector& other);
	vector(int x1, int y1, int x2, int y2);
	vector normalize();

	vector& operator=(const vector& other);
	vector operator*(float scal) { x *= scal; y *= scal; }
	float operator*(vector other);
	float getX() { return x; }
	float getY() { return y; }
};


float vector::mod()
{
	return sqrt(x * x + y * y);
}

vector::vector()
{
	x = y = 0;
}

vector::vector(float _x, float _y)
{
	x = _x;
	y = _y;
}

float vector::dotProduct(vector& other)
{
	return (this->x * other.x + this->y * other.y) / (this->mod() * other.mod());
}

vector::vector(int x1, int y1, int x2, int y2)
{

	x = x2 - x1;
	y = y2 - y1;
}

vector vector::normalize()
{
	return vector(x / mod(), y / mod());
}

vector& vector::operator=(const vector& other)
{
	this->x = other.x;
	this->y = other.y;

	return *this;
}

float vector::operator*(vector other)
{
	return this->x * other.x + this->y * other.y;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class Player
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const float playerVelocity = screenWidth / 65;
class player
{
private:
	float velocity;
	int healthPoint;
	bool live;
	int x, y, width, height;
	Sprite* playerSprite;

public:
	player();
	void setVelocity(float _velocity);
	void setPlayerPosition(int _x, int _y);
	void setSprite(const char* arr) { playerSprite = createSprite(arr); }
	void setSize() { setSpriteSize(playerSprite, width, height); }
	Sprite* getPlayerSprite() { return playerSprite; }
	int getHeight() { return height; }
	int getWidth() { return width; }
	int getX() { return x; }
	int getY() { return y; }
	bool isAlive() { return live; }
	float getVelocity() { return velocity; }
	void checkColision();
	void changeHP(int hp);
	void update();
};


player::player()
{
	live = true;
	healthPoint = 3;
	velocity = x = y = 0;
	width = screenWidth / 4;
	height = screenHeight / 16;
	setPlayerPosition(screenWidth / 2, screenHeight - height);
}

void player::setVelocity(float _velocity)
{
	this->velocity = _velocity;
}

void player::setPlayerPosition(int _x, int _y)
{
	this->x = _x;
	this->y = _y;
}


void player::checkColision()
{
	if (x <= 0) x = 1;
	if (x + width >= screenWidth) x = screenWidth - width - 1;
}

void player::changeHP(int hp)
{
	this->healthPoint += hp;
}



void player::update()
{
	x += velocity;
	if (healthPoint <= 0) live = false;
	setSprite("data/50-Breakout-Tiles.png");
	setSize();
	checkColision();
	drawSprite(playerSprite, x, y);
	destroySprite(playerSprite);
}

/////////////////////////////////////////////////////////////////////////
/// class Ball
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ball
{
private:
	float dx, dy;
	float x, y;
	int mouseX, mouseY;
	float radius;
	bool shoot; // Is ball on deck to shoot
	Sprite* ballSprite;
	float differenceX, differenceY;
	int in_a_row;
public:
	ball();
	void update(player*);
	void followCursPosition();

	Sprite* getSprite() { return ballSprite; }
	float getX() { return x; }
	float getY() { return y; }
	int getMouseX() { return mouseX; }
	int getMouseY() { return mouseY; }
	int getRadius() { return radius; }
	bool getShoot() { return shoot; }
	bool checkColision(float blockX, float blockY, float blockPixelsWidth, float blockPixelsHeight);

	void inRowCounter() { ++in_a_row; }
	int getInRow() { return in_a_row; }
	void setShootAbility(bool a) { shoot = a; }
	void ballCollisionPlayer(player* main);
	void ballDirection(float blockX, float blockY);
	void setMousePos(int x, int y) { mouseX = x; mouseY = y; }
	void setSprite(const char* way) { ballSprite = createSprite(way); }
	void setPosition(int _x, int _y) { x = _x; y = _y; }
	void setSpeed(int _dx, int _dy) { dx = _dx; dy = _dy; }
	void setRadius(int _radius) { radius = _radius; }
	void setSpriteBallSize() { setSpriteSize(ballSprite, 2 * radius, 2 * radius); }

};


ball::ball()
{
	in_a_row = 0;
	differenceX = differenceY = 0;
	ballSprite = nullptr;
	x = y = dx = dy = 0;
	radius = screenWidth / 50;
	shoot = true;
	mouseX = mouseY = 0;
	setPosition(screenWidth / 2, screenHeight - screenHeight / 16);
}

void ball::update(player* mainHero)
{
	ballCollisionPlayer(mainHero);
	setSprite("data/63-Breakout-Tiles.png");
	if (shoot)
	{
		in_a_row = 0;
		showCursor(true);
		x = mainHero->getX() + mainHero->getWidth() / 2.0 - 15;
		y = mainHero->getY() - 2 * radius;
	}
	else
	{
		showCursor(false);
		if (x <= 0) { x = 1;  dx *= -1; }
		if (x + radius * 2 >= screenWidth) { x = screenWidth - 2 * radius - 1;  dx *= -1; }
		if (y <= 0) { y = 0; dy *= -1; }
		if (y + radius >= screenHeight) { shoot = true; mainHero->changeHP(-1); return; }
		x += dx;
		y += dy;
	}
	setSpriteBallSize();
	drawSprite(ballSprite, x, y);
	destroySprite(ballSprite);
}

void ball::followCursPosition()
{
	float dist = distance(x, y, mouseX, mouseY);
	dx = ((screenHeight + screenWidth) / 200) * (mouseX - x) / dist;
	dy = ((screenHeight + screenWidth) / 200) * (mouseY - y) / dist;
	shoot = false;
}



bool ball::checkColision(float blockX, float blockY, float blockPixelsWidth, float blockPixelsHeight)
{
	// i want to ball be circle but not rectangle sprite

	float ballCenterX = x + radius;
	float ballCenterY = y + radius;

	float blockHalfWidth = blockPixelsWidth / 2.0;
	float blockHalfHeight = blockPixelsHeight / 2.0;
	float blockCenterX = blockX + blockHalfWidth;
	float blockCenterY = blockY + blockHalfHeight;

	float differenceVectorX = ballCenterX - blockCenterX; // finding vector(name of vector A) between
	float differenceVectorY = ballCenterY - blockCenterY; //  center of circle and center of block

	float compressedX = clamp(differenceVectorX, -blockHalfWidth, blockHalfWidth); // clamp vector  A to 0.5 of width
	float compressedY = clamp(differenceVectorY, -blockHalfHeight, blockHalfHeight); // and 0.5 of height

	float closestX = blockCenterX + compressedX; // finding closet dot on block to circle
	float closestY = blockCenterY + compressedY; // likewise

	differenceX = closestX - ballCenterX; // findig vector(name B) beetwen center circle and closet dot on block
	differenceY = closestY - ballCenterY; // likewise


	return (radius > length(differenceX, differenceY)); // if radius > then length of vector B -> 
	//block and circle are touching

/*bool collisionY = y + radius  >= blockY && blockY + blockPixelsHeight >= y;
bool collisionX = x + radius  >= blockX && blockX + blockPixelsWidth >= x;

return collisionX && collisionY;*/
}




void ball::ballCollisionPlayer(player* main)
{
	if (checkColision(main->getX(), main->getY(), main->getWidth(), main->getHeight()))
	{
		in_a_row = 0;
		if (y >= main->getY() + main->getHeight() / (1.5))
		{
			shoot = true;
		}
		else
		{
			float centerOfBoard = main->getX() + main->getWidth() / 2.0;
			float distance = x + radius - centerOfBoard;
			float percentage = distance / (main->getWidth() / 2.0);

			float acceleration = 2;

			vector ballVector(dx, dy);
			dx = ((screenHeight + screenWidth) / 200) * percentage * acceleration;
			dy = abs(dy) * (-1);
			vector newBallVector = vector(dx, dy).normalize();
			dx = newBallVector.getX() * ballVector.mod();
			dy = newBallVector.getY() * ballVector.mod();
		}
	}
}

//enum direction
//{
//	UP,
//	LEFT,
//	DOWN,
//	RIGHT
//};

void ball::ballDirection(float blockX, float blockY)
{
	//this realisation will not work because some times ball can hit upper or down side of
	// block with low angel and in that case his dX speed will change not dY
	//vector  normball = vector(dx, dy).normalize();


	//vector directions[] =
	//{
	//	vector(0,1),vector(1,0),vector(0,-1),vector(-1,0)   // creating 4 side vectors (UP,LEFT,DOWN,RIGHT)
	//};
	//float maxDotProduct = 0;
	//direction dir = direction::UP;                           // direction of ball
	//for (size_t i = 0; i < 4; i++)
	//{
	//	float dotProd = normball.dotProduct(directions[i]); // dot product = 1 if vectors are if angel = 0
	//	if (maxDotProduct < dotProd)                        // so the dot product which will have maximum value of all dot products that's
	//		// equels to direction in which ball will run into
	//	{
	//		maxDotProduct = dotProd;
	//		dir = (direction)i;
	//	}
	//}
	//// there can be bugs if hit 2 blocks simultaneously. That's the best what i could come up with

	//if (dir == UP || dir == DOWN) {
	//	float penetrationY = radius - abs(differenceY); // ball sprite is entering in block sprit
	//	// I will take out ball from sprite buy using penetration

	//	dy *= -1;

	//	if (dir == UP)
	//		y -= penetrationY;
	//	else
	//		y += penetrationY;
	//}
	//else {
	//	float penetrationX = radius - abs(differenceX);
	//	dx *= -1;
	//	if (dir == LEFT)
	//		x += penetrationX;
	//	else
	//		x -= penetrationX;
	//}
	float penetrationY = radius - abs(differenceY);
	float penetrationX = radius - abs(differenceX);
	if (x + 2 * radius >= blockX && x + 2 * radius <= blockX + penetrationX && y >= blockY && y <= blockY + blockPixelsHeight)
	{
		dx *= -1;

	}
	else if (x <= blockX + blockPixelsWidth && x >= blockX + blockPixelsWidth - penetrationX && y >= blockY && y <= blockY + blockPixelsHeight)
	{
		dx *= -1;

	}
	else if (y + 2 * radius >= blockY && y + 2 * radius <= blockY + penetrationY && x >= blockX && x <= blockX + blockPixelsWidth)
	{
		dy *= -1;

	}
	else if (y <= blockY + blockPixelsHeight && y >= blockY + penetrationY && x >= blockX && x <= blockX + blockPixelsWidth)
	{
		dy *= -1;

	}
	else
	{
		dx *= -1;
	}

}
/// class Ability
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class ability
{
private:
	bool isOnMap;
	int abilityStatus; // Means deal dmg or heal
	float dy;
	float x, y;
	Sprite* abilitySprite;
	float width, height;
	int timeToFall;

public:
	ability();
	void draw() { drawSprite(abilitySprite, x, y); }
	void changeSize() { setSpriteSize(abilitySprite, width, height); }
	void setSpeed(float _dy) { dy = _dy; }
	void setPosition(float _x, float _y) { x = _x; y = _y; }
	void setAppearing(bool apear);
	void setTime(int time);
	void setSprite(const char* way) { abilitySprite = createSprite(way); }
	void update() { y += dy; }
	int getTime() { return timeToFall; }
	bool getIsOnMap() { return isOnMap; }
	float getX() { return x; }
	float getY() { return y; }
	void checkColisionWithPlayer(player* main);
};

ability::ability()
{
	height = width = 0;
	timeToFall = 0;
	isOnMap = false;
	x = y = 0;
	dy = 0;
	abilitySprite = nullptr;
	abilityStatus = rand() % 2;
}

void ability::setAppearing(bool apear)
{
	isOnMap = apear;
	if (apear == true) {
		int border = screenWidth - 2 * width;
		x = width + (rand() % border);
		y = 0;
		dy = screenHeight / 300;

		if (abilityStatus == 0)
		{
			width = screenWidth / 30;
			height = screenHeight / 10;
			setSprite("data/bomb.png");
		}
		else {
			width = screenWidth / 20;
			height = screenHeight / 20;
			setSprite("data/60-Breakout-Tiles.png");
		}
	}
}

void ability::setTime(int time)
{
	timeToFall = time;
}

void ability::checkColisionWithPlayer(player* main)
{
	bool collisionY = y + height >= main->getY() && main->getY() + main->getHeight() >= y;
	bool collisionX = x + width >= main->getX() && main->getX() + main->getWidth() >= x;

	if (y >= screenHeight)
	{
		isOnMap = false;
		abilityStatus = rand() % 2;
		timeToFall = 0;
		return;
	}
	if (collisionX && collisionY)
	{
		isOnMap = false;
		timeToFall = 0;
		if (abilityStatus == 1) main->changeHP(1);
		else main->changeHP(-1);
		abilityStatus = rand() % 2;
	}
}