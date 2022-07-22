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

	int GetWidth();

	int GetHeight();

	void SetX(int x);

	void SetY(int y);



	virtual void DrawObject() = 0;

	virtual void EraseObject() = 0;

	virtual void MoveObject() = 0;



	void DeleteObject();

	bool IsObjectDelete();



	void SetGunType(int gunType);

	bool GetGunState();

	int GetGunType();

	int GetGunSpeed();

	void SetGunSpeed(int gunSpeed);

	void ReloadGun(int reloadTime);

protected:

	wd* wData;

	int _x, _y, _speed, _color, _direction = RIGHT, _gunType = NONE, _width, _height, _gunSpeed = 3000;

	bool _deleteObject = false;

	bool _reload = true;
};



class Player : public GameObject
{
public:
	Player(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {
		_width = REGULAR_WIDTH - 1;
		_height = REGULAR_HEIGHT;
		_gunType = SINGLESHOT;
		_gunSpeed = 2000;
	}

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject() override;

	void Death(bool& worldIsRun);

	int GetLifes();

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
	Enemy(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {
	}

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject() override;

	void Hit(int score);

	int GetEnemyType();

	void SetEnemyType(int type);

	void CheckKamikadzeArea(Player* player);

	void Kamikadze(int x, int y);

	void BossDir();

private:

	const int VISIBLE_RADIUS = 15;
 
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

	bool targetFinded = false;
	bool bossDir = true;

	vector <pair<int, int>> attackRange;
	vector <pair<int, int>> path;

};



class Bullet : public GameObject
{
public:
	Bullet(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {
	}
	
	void DrawObject() override;

	void EraseObject() override;


	bool GetOwner();

	void SetOwner(int owner);


	void MoveObject() override;

private:

	bool enemyShot = false, playerShot = false, reload = false;

	int _owner;

};






class Bonus : public GameObject
{
public:

	Bonus(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {
		
	}

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject() override;


	void SetBonusType(int type);

	int GetBonusType();

private:

	int _type = SPEED; 

};



