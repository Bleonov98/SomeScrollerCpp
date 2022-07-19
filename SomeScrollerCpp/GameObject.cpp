#include "GameObject.h"

int GameObject::GetX()
{
	return _x;
}

int GameObject::GetY()
{
	return _y;
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

	if (_direction == UP) {
		_y -= _speed;
	}
	else if (_direction == DOWN) {
		_y += _speed;
	}
	else if (_direction == RIGHT) {
		_x += _speed;
	}
	else if (_direction == LEFT) {
		_x -= _speed;
	}
}

void Player::Death(bool &worldIsRun)
{
	Sleep(500);

	if (_lifes == 1) {
		worldIsRun = false;
	}
	else _lifes--;
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

	if (_x >= COLS) {
		DeleteObject();
	}
}

void Bullet::SetOwner(int owner)
{
	if (_owner == PLAYER) {
		playerShot = true;
		_direction = RIGHT;
	}
	else if (_owner == ENEMY) {
		enemyShot = true;
		_direction = LEFT;
	}
}

bool Bullet::GetOwner()
{
	if (_owner == PLAYER) {
		return playerShot;
	}
	else if (_owner == ENEMY) {
		return enemyShot;
	}
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
	_x--;
	if (_x <= 2) {
		DeleteObject();
	}
}

void Enemy::Hit(int score)
{
	if (_lifes == 1) {
		DeleteObject();
		score += 500;
	}
	else _lifes--;
}