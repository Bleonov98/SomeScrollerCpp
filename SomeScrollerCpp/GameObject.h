#pragma once
#include "Tools.h"

static const int SMALL_HEIGHT = 2;
static const int SMALL_WIDTH = 3;
static const int REGULAR_HEIGHT = 4;
static const int REGULAR_WIDTH = 5;
static const int BOSS_ENEMY_HEIGHT = 6;
static const int BOSS_ENEMY_WIDTH = 7;

static const int DIRECTION = 4;
static const int ANIMATION = 2;

class GameObject
{
public:
	GameObject(wd* wData, int x, int y, int speed, int color) {
		this->wData = wData;
		_x = x, _y = y, _speed = speed, _color = color;
	};

	int GetX();

	int GetY();

	void SetX(int x);

	void SetY(int y);

	virtual void DrawObject() = 0;

	virtual void EraseObject() = 0;

	void DeleteObject();

	bool IsObjectDelete();

protected:

	wd* wData;

	int _x, _y, _speed, _color, _direction = RIGHT;

	bool _deleteObject = false;
};



class Player : public GameObject
{

};



class Enemy : public GameObject
{

};



class Bullet : public GameObject
{

};






class Bonus : public GameObject
{

};



