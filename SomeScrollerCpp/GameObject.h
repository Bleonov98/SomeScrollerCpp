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

	virtual void SetGunState(bool reload) = 0;

	virtual bool GetGunState() = 0;

	int GetGunType();

	virtual int GetGunSpeed() = 0;

	virtual void SetGunSpeed(int gunSpeed) = 0;

protected:

	wd* wData;

	int _x, _y, _speed, _color, _direction = RIGHT, _gunType = NONE, _width, _height;

	bool _deleteObject = false;
};



class Player : public GameObject
{
public:
	Player(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {
		_width = REGULAR_WIDTH - 1;
		_height = REGULAR_HEIGHT;
		_gunType = SINGLESHOT;
		_gunSpeed = 1000;
	}

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject() override;

	void Death(bool& worldIsRun);


	void SetGunState(bool reload) override;

	bool GetGunState() override;


	void SetGunSpeed(int gunSpeed) override;

	int GetGunSpeed() override;


	int GetLifes();

	void AddLifes();

	int GetHp();

private:

	void ChangeDir();

	int _lifes = 3, _hp = 2, _gunSpeed = 2000;

	bool _reload = true;

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

	void Hit(int& score, bool& worldIsRun, bool& win);


	int GetEnemyType();

	void SetEnemyType(int type);


	void SetGunState(bool reload) override;

	bool GetGunState() override;


	void SetGunSpeed(int gunSpeed) override;

	int GetGunSpeed() override;



	void CheckArea(Player* player);
	

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
	char16_t landEnemy[REGULAR_HEIGHT][REGULAR_WIDTH]{
		u" @@ ",
		u" -- ",
		u"----"
	};


	int _type = SMALL, _lifes = 1;

	bool targetFinded = false;
	bool bossDir = true;

	vector <pair<int, int>> attackRange;
	vector <pair<int, int>> path;

	void BossDir();

	void Kamikadze(int x, int y);

	int _gunSpeed = 3000;

	bool _reload = true;
};



class Bullet : public GameObject
{
public:
	Bullet(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {
	}
	
	void DrawObject() override;

	void EraseObject() override;


	int GetOwner();

	void SetOwner(int owner);


	void MoveObject() override;

	
	void SetGunState(bool reload) override;

	bool GetGunState() override;


	void SetGunSpeed(int gunSpeed) override;

	int GetGunSpeed() override;


	void BulletPath(int x, int y);

private:

	int _owner;

	vector <pair<int, int>> path;

	bool targetMove = false;
};



class Bonus : public GameObject
{
public:

	Bonus(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {
		_width = SMALL_WIDTH - 1;
		_height = SMALL_HEIGHT;
		_gunType = NONE;
		_speed = 1;
	}

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject() override;


	void SetBonusType(int type);

	int GetBonusType();


	void SetGunState(bool reload) override;

	bool GetGunState() override;


	void SetGunSpeed(int gunSpeed) override;

	int GetGunSpeed() override;

private:

	int _type = GUNSPEED; 

	char16_t gunspeedSprite[SMALL_HEIGHT][SMALL_WIDTH]{
		u"--",
		u"SP",
		u"--"
	};
	char16_t guntypeSprite[SMALL_HEIGHT][SMALL_WIDTH]{
		u"--",
		u"TP",
		u"--"
	};
	char16_t lifeBonusSprite[SMALL_HEIGHT][SMALL_WIDTH]{
		u"--",
		u"LF",
		u"--"
	};

};



