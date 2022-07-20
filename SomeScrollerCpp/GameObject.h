#pragma once
#include "Tools.h"

static const int SMALL_HEIGHT = 3;
static const int SMALL_WIDTH = 3;
static const int REGULAR_HEIGHT = 3;
static const int REGULAR_WIDTH = 5;
static const int BOSS_HEIGHT = 13;
static const int BOSS_WIDTH = 15;

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

	int GetSpeed();

	void SetX(int x);

	void SetY(int y);

	virtual void DrawObject() = 0;

	virtual void EraseObject() = 0;

	virtual void MoveObject() = 0;

	void DeleteObject();

	bool IsObjectDelete();

protected:

	wd* wData;

	int _x, _y, _speed, _color, _direction = RIGHT;

	bool _deleteObject = false;
};



class Player : public GameObject
{
public:
	Player(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {

	}

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject() override;

	void Death(bool& worldIsRun);

private:

	void ChangeDir();

	int _lifes = 3;

	char16_t playerSprite[REGULAR_HEIGHT][REGULAR_WIDTH]{
		u"==  ",
		u"---@",
		u"==  ",
	};

};



class Enemy : public GameObject
{
public:
	Enemy(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {}

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject() override;

	void Hit(int score);

private:

	char16_t smallEnemy[SMALL_HEIGHT][SMALL_WIDTH]{
		u" -",
		u"0-",
		u" -"
	};
	char16_t regularEnemy[REGULAR_HEIGHT][REGULAR_WIDTH]{
		u"  --",
		u"0---",
		u"  --"
	};
	char16_t bossEnemy[BOSS_HEIGHT][BOSS_WIDTH]{
		u"        --@   ",
		u" @--------@---",
		u" @--------@---",
		u"        --@---",
		u"        --@---",
		u"@----@----@---",
		u"@----#----#---",
		u"@----@----@---",
		u"        --@---",
		u"        --@---", 
		u" @--------@---",
		u" @--------@---",
		u"        --@   ",
	};

	int _type = SMALL, _lifes = 1;

};



class Bullet : public GameObject
{
public:
	Bullet(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {}
	
	void DrawObject() override;

	void EraseObject() override;


	bool GetOwner();

	void SetOwner(int owner);


	void MoveObject() override;

private:

	bool enemyShot = false, playerShot = false;

	int _owner;

};






class Bonus : public GameObject
{

};



