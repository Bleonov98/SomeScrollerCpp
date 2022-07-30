#include "GameObject.h"

int GameObject::GetX()
{
	return _x;
}

int GameObject::GetY()
{
	return _y;
}

int GameObject::GetSpeed()
{
	return _speed;
}

int GameObject::GetWidth()
{
	return _width;
}

int GameObject::GetHeight()
{
	return _height;
}

void GameObject::SetX(int x)
{
	_x = x;
}

void GameObject::SetY(int y)
{
	_y = y;
}

void GameObject::DeleteObject()
{
	_deleteObject = true;
}

bool GameObject::IsObjectDelete()
{
	return _deleteObject;
}

void GameObject::SetGunType(int gunType)
{
	_gunType = gunType;
}

int GameObject::GetGunType()
{
	return _gunType;
}

// ----- PLAYER --------


void Player::DrawObject()
{
	for (int h = 0; h < REGULAR_HEIGHT; h++)
	{
		for (int w = 0; w < REGULAR_WIDTH - 1; w++)
		{
			wData->vBuf[_y + h][_x + w] = playerSprite[h][w] | (_color << 8);
		}
	}
}

void Player::EraseObject()
{
	for (int h = 0; h < REGULAR_HEIGHT; h++)
	{
		for (int w = 0; w < REGULAR_WIDTH - 1; w++)
		{
			wData->vBuf[_y + h][_x + w] = u' ';
		}
	}
}

void Player::MoveObject()
{
	EraseObject();

	ChangeDir();

	if (_direction == STOP) {
		return;
	}

	if (_direction == UP) {
		_y--;
	}
	else if (_direction == DOWN) {
		_y++;
	}
	else if (_direction == RIGHT) {
		_x++;
	}
	else if (_direction == LEFT) {
		_x--;
	}
}

void Player::Death(bool &worldIsRun)
{
	_hp--;

	if (_hp == 0) {
		Sleep(500);
		if (_lifes == 1) {
			worldIsRun = false;
			_hp = 0;
		}
		else {
			_lifes--;
			_hp = 2;
		}

		_gunType = SINGLESHOT;
		_gunSpeed = 2000;
	}
}

void Player::SetGunState(bool reload)
{
	_reload = reload;
}

void Player::SetGunSpeed(int gunSpeed)
{
	_gunSpeed = gunSpeed;
}

int Player::GetGunSpeed()
{
	return _gunSpeed;
}

bool Player::GetGunState()
{
	return _reload;
}

int Player::GetLifes()
{
	return _lifes;
}

void Player::AddLifes()
{
	_lifes++;
}

int Player::GetHp()
{
	return _hp;
}

void Player::ChangeDir()
{
	if (GetAsyncKeyState(VK_LEFT) && _x > 2) {
		_direction = LEFT;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && _x + REGULAR_WIDTH <= COLS) {
		_direction = RIGHT;
	}
	else if (GetAsyncKeyState(VK_UP) && _y > 2) {
		_direction = UP;
	}
	else if (GetAsyncKeyState(VK_DOWN) && _y < ROWS - 3) {
		_direction = DOWN;
	}
	else _direction = STOP;
}

// ----- BULLET --------


void Bullet::DrawObject()
{
	wData->vBuf[_y][_x] = u'-' | (_color << 8);
}

void Bullet::EraseObject()
{
	wData->vBuf[_y][_x] = u' ';
}

void Bullet::MoveObject()
{
	EraseObject();

	if (_direction == RIGHT) _x++;
	else if (_direction == LEFT) _x--;

	if (_x >= COLS - 2 || _x <= 3) {
		DeleteObject();
	}
}

void Bullet::SetGunState(bool reload)
{
}

bool Bullet::GetGunState()
{
	return false;
}

void Bullet::SetGunSpeed(int gunSpeed)
{
}

int Bullet::GetGunSpeed()
{
	return 0;
}

void Bullet::SetOwner(int owner)
{
	_owner = owner;

	if (_owner == PLAYER) {
		_direction = RIGHT;
	}
	else if (_owner == ENEMY) {
		_direction = LEFT;
	}
}

int Bullet::GetOwner()
{
	return _owner;
}

// ----- ENEMY --------


void Enemy::DrawObject()
{
	if (_type == SMALL) {
		for (int h = 0; h < SMALL_HEIGHT; h++)
		{
			for (int w = 0; w < SMALL_WIDTH - 1; w++)
			{
				wData->vBuf[_y + h][_x + w] = smallEnemy[h][w] | (_color << 8);
			}
		}
	}
	else if (_type == REGULAR) {
		for (int h = 0; h < REGULAR_HEIGHT; h++)
		{
			for (int w = 0; w < REGULAR_WIDTH - 1; w++)
			{
				wData->vBuf[_y + h][_x + w] = regularEnemy[h][w] | (_color << 8);
			}
		}
	}
	else if (_type == BOSS) {
		for (int h = 0; h < BOSS_HEIGHT; h++)
		{
			for (int w = 0; w < BOSS_WIDTH - 1; w++)
			{
				wData->vBuf[_y + h][_x + w] = bossEnemy[h][w] | (_color << 8);
			}
		}
	}
}

void Enemy::EraseObject()
{
	if (_type == SMALL) {
		for (int h = 0; h < SMALL_HEIGHT; h++)
		{
			for (int w = 0; w < SMALL_WIDTH - 1; w++)
			{
				wData->vBuf[_y + h][_x + w] = u' ';
			}
		}
	}
	else if (_type == REGULAR) {
		for (int h = 0; h < REGULAR_HEIGHT; h++)
		{
			for (int w = 0; w < REGULAR_WIDTH - 1; w++)
			{
				wData->vBuf[_y + h][_x + w] = u' ';
			}
		}
	}
	else if (_type == BOSS) {
		for (int h = 0; h < BOSS_HEIGHT; h++)
		{
			for (int w = 0; w < BOSS_WIDTH - 1; w++)
			{
				wData->vBuf[_y + h][_x + w] = u' ';
			}
		}
	}
}

void Enemy::MoveObject()
{
	EraseObject();
	if (_type == SMALL && targetFinded) {
		 
		if (!path.empty()) {
			SetX(path.back().first);
			SetY(path.back().second);

			path.pop_back();

			if (path.empty()) {
				DeleteObject();
			}
		}

	}
	else if (_type == BOSS) {
		BossDir();
	}
	else {
		_x--;
		if (_x <= 3) {
			DeleteObject();
		}
	}
}

void Enemy::Hit(int& score, bool& worldIsRun, bool& win)
{
	if (_lifes == 1) {
		DeleteObject();
		EraseObject();

		if (_type == SMALL) {
			score += 250;
		}
		else if (_type == REGULAR) {
			score += 500;
		}
		else if (_type == BOSS) {
			score += 2500;
			worldIsRun = false;
			win = true;
		}

	}
	else _lifes--;
}

int Enemy::GetEnemyType()
{
	return _type;
}

void Enemy::SetEnemyType(int type)
{
	_type = type;

	if (_type == SMALL) {
		_width = SMALL_WIDTH - 1;
		_height = SMALL_HEIGHT;
		_speed = 1;
		_lifes = 1;
	}
	else if (_type == REGULAR) {
		_width = REGULAR_WIDTH - 1;
		_height = REGULAR_HEIGHT;
		_gunType = DOUBLESHOT;
		_lifes = 1;
	}
	else if (_type == BOSS) {
		_width = BOSS_WIDTH - 1;
		_height = BOSS_HEIGHT;
		_speed = 12;
		_gunSpeed = 650;
		_y = ROWS / 2 - _height / 2;
		_x = COLS - BOSS_WIDTH;
		_lifes = 25;
	}
}


void Enemy::SetGunState(bool reload)
{
	_reload = reload;
}

bool Enemy::GetGunState()
{
	return _reload;
}

void Enemy::SetGunSpeed(int gunSpeed)
{
	_gunSpeed = gunSpeed;
}

int Enemy::GetGunSpeed()
{
	return _gunSpeed;
}



void Enemy::CheckKamikadzeArea(Player* player)
{
	if (targetFinded) return;

	attackRange.clear();

	for (int R = 1; R < VISIBLE_RADIUS; R++)
	{
		int x = 0;
		int y = R;

		int delta = 1 - 2 * R;
		int err = 0;

		while (y >= x) {
			attackRange.push_back(make_pair(_x + x, _y + y));
			attackRange.push_back(make_pair(_x + x, _y - y));
			attackRange.push_back(make_pair(_x - x, _y + y));
			attackRange.push_back(make_pair(_x - x, _y - y));
			attackRange.push_back(make_pair(_x + y, _y + x));
			attackRange.push_back(make_pair(_x + y, _y - x));
			attackRange.push_back(make_pair(_x - y, _y + x));
			attackRange.push_back(make_pair(_x - y, _y - x));

			err = 2 * (delta + y) - 1;

			if ((delta < 0) && (err <= 0)) {
				delta += 2 * ++x + 1;
				continue;
			}
			if ((delta > 0) && (err > 0)) {
				delta -= 2 * --y + 1;
				continue;
			}

			delta += 2 * (++x - --y);
		}
	}

	for (int i = 0; i < attackRange.size(); i++)
	{
		if ((player->GetX() == attackRange[i].first) && (player->GetY() == attackRange[i].second)) {
			targetFinded = true;
			Kamikadze(player->GetX(), player->GetY());
			break;
		}
	}
}

void Enemy::Kamikadze(int x, int y)
{
	if (y == _y) {	
		targetFinded = false;
		return; 
	}

	int X = _x, Y = _y;

	int dx = abs(x - X);
	int dy = abs(y - Y);

	int error = dx - dy;

	int dirX = (X < x) ? 1 : -1;
	int dirY = (Y < y) ? 1 : -1;

	while (X != x && Y != y)
	{
		const int error2 = error * 2;

		if (error2 > -dy)
		{
			error -= dy;
			X += dirX;
		}
		if (error2 < dx)
		{
			error += dx;
			Y += dirY;
		}

		path.insert(path.begin(), make_pair(X, Y));

	}
}

void Enemy::BossDir()
{
	if (_y + _height >= ROWS - 2) {
		bossDir = false;
	}
	if (_y <= 3) {
		bossDir = true;
	}

	if (bossDir) _y++;
	else _y--;

	_gunType = rand() % 2;
}

// ----- BONUS ---------


void Bonus::DrawObject()
{
	if (_type == GUNSPEED) {
		_color = Yellow;
		for (int h = 0; h < SMALL_HEIGHT; h++)
		{
			for (int w = 0; w < SMALL_WIDTH - 1; w++)
			{
				wData->vBuf[_y + h][_x + w] = gunspeedSprite[h][w] | (_color << 8);
			}
		}
	}
	else if (_type == GUNTYPE) {
		_color = Purple;
		for (int h = 0; h < SMALL_HEIGHT; h++)
		{
			for (int w = 0; w < SMALL_WIDTH - 1; w++)
			{
				wData->vBuf[_y + h][_x + w] = guntypeSprite[h][w] | (_color << 8);
			}
		}
	}
	else if (_type == LIFE) {
		_color = Green;
		for (int h = 0; h < SMALL_HEIGHT; h++)
		{
			for (int w = 0; w < SMALL_WIDTH - 1; w++)
			{
				wData->vBuf[_y + h][_x + w] = lifeBonusSprite[h][w] | (_color << 8);
			}
		}
	}
}

void Bonus::EraseObject()
{
	for (int h = 0; h < SMALL_HEIGHT; h++)
	{
		for (int w = 0; w < SMALL_WIDTH - 1; w++)
		{
			wData->vBuf[_y + h][_x + w] = u' ';
		}
	}
}

void Bonus::MoveObject()
{
	EraseObject();

	_x--;
	if (_x <= 3) {
		DeleteObject();
	}
}

void Bonus::SetBonusType(int type)
{
	_type = type;
}

int Bonus::GetBonusType()
{
	return _type;
}

void Bonus::SetGunState(bool reload)
{
}

bool Bonus::GetGunState()
{
	return false;
}

void Bonus::SetGunSpeed(int gunSpeed)
{
}

int Bonus::GetGunSpeed()
{
	return 0;
}
