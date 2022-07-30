#include "Game.h"

void Game::SetPos(int x, int y)
{
	sprintf_s(coord, "%s%d;%dH", CSI, y, x);
	printf(coord);
}

void Game::HotKeys(bool& pause)
{
	while (worldIsRun) {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			worldIsRun = false;
		}
		else if (GetAsyncKeyState(0x50) & 0x8000) {
			pause = !pause;

			this_thread::sleep_for(milliseconds(1000));
		}
	}
}

void Game::DrawArea()
{
	// Set console code page to UTF-8 so console known how to interpret string data
	SetConsoleOutputCP(CP_UTF8);

	// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(IDR_TEXT1), L"TEXT");

	if (hResource)
	{
		HGLOBAL hLoadedResource = LoadResource(hInstance, hResource);

		if (hLoadedResource)
		{
			LPCSTR area = (LPCSTR)LockResource(hLoadedResource);

			if (area)
			{
				DWORD dwResourceSize = SizeofResource(hInstance, hResource);

				if (0 != dwResourceSize)
				{
					int row = 0, col = 0;
					for (int i = 0; i < strnlen(area, 1025*50); i++) {
						GameMap[row][col] = area[i];
						if (i % 1025 == 0 && i > 0) {
							row++;
							col = 0;
						}
						col++;
					}
				}
			}
		}
	}

	for (int h = 0; h < ROWS; h++)
	{
		for (int w = 0; w < COLS; w++)
		{
			cout << GameMap[h][w];
			activeAreaBuf[h][w] = GameMap[h][w];
		}
	}

	setvbuf(stdout, NULL, _IONBF, 0);
}

void Game::CreateWorld() {

	term.Terminal();  // Set virtual terminal settings
	term.SetScreenSize();
	term.SetConsoleFont();

	printf(CSI "?1049h"); // enable alt buffer
	printf(CSI "?25l"); // hide cursor blinking

	DrawArea();
	//DrawLevel();
}

void Game::DrawEndInfo(bool& restart)
{
	if (win) {
		SetPos(24, ROWS + 2);
		cout << "CONGRATULATION! YOU WIN!";
	}
	else {
		SetPos(30, ROWS + 2);
		cout << "GAME OVER!";
	}

	SetPos(25, ROWS + 3);
	cout << "PRESS ENTER TO RESTART";
	SetPos(27, ROWS + 4);
	cout << "PRESS ESC TO EXIT";

	bool pressed = false;
	restart = false;

	while (!pressed) {
		if (GetAsyncKeyState(VK_RETURN)) {
			restart = true;
			pressed = true;
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			restart = false;
			pressed = true;
		}
	}
}

void Game::DrawInfo(Player* player)
{
	int _speed = 1;

	if (player->GetGunSpeed() == 2000) _speed = 1;
	else if (player->GetGunSpeed() == 1750) _speed = 2;
	else if (player->GetGunSpeed() == 1500) _speed = 3;
	else if (player->GetGunSpeed() == 1250) _speed = 4;
	else if (player->GetGunSpeed() == 1000) _speed = 5;
	else if (player->GetGunSpeed() == 750) _speed = 6;
	else if (player->GetGunSpeed() == 500) _speed = 7;

	SetPos(5, ROWS + 2);
	cout << "SCORE: " << score;
	SetPos(5, ROWS + 3);
	cout << "HP: " << "   ";
	SetPos(5, ROWS + 3);
	cout << "HP: " << player->GetHp() * 50;
	SetPos(5, ROWS + 4);
	cout << "LIFES: " << player->GetLifes();
	SetPos(5, ROWS + 5);
	cout << "SPEED: " << _speed;
}

void Game::DrawChanges()
{
	for (int y = 0; y < ROWS; y++)
	{
		for (int x = 0; x < COLS; x++)
		{
			if (prevBuf[y][x] == wData.vBuf[y][x]) {
				continue;
			}
			else {
				prevBuf[y][x] = wData.vBuf[y][x];

				SetPos(x, y);

				if ((prevBuf[y][x] >> 8) == Red) {
					printf(CSI "22;31m");
				}
				else if ((prevBuf[y][x] >> 8) == Green) {
					printf(CSI "22;32m");
				}
				else if ((prevBuf[y][x] >> 8) == Blue) {
					printf(CSI "22;34m");
				}
				else if ((prevBuf[y][x] >> 8) == Yellow) {
					printf(CSI "22;33m");
				}
				else if ((prevBuf[y][x] >> 8) == Purple) {
					printf(CSI "22;35m");
				}
				else if ((prevBuf[y][x] >> 8) == Cyan) {
					printf(CSI "22;36m");
				}
				else if ((prevBuf[y][x] >> 8) == BrRed) {
					printf(CSI "22;91m");
				}
				else if ((prevBuf[y][x] >> 8) == BrGreen) {
					printf(CSI "22;92m");
				}
				else if ((prevBuf[y][x] >> 8) == BrBlue) {
					printf(CSI "22;94m");
				}
				else if ((prevBuf[y][x] >> 8) == BrCyan) {
					printf(CSI "22;96m");
				}
				else if ((prevBuf[y][x] >> 8) == BrPurple) {
					printf(CSI "22;95m");
				}
				else if ((prevBuf[y][x] >> 8) == BrYellow) {
					printf(CSI "22;93m");
				}
				else if ((prevBuf[y][x] >> 8) == White) {
					printf(CSI "47;37m");
				}
				else printf(CSI "22; 44m");

				cout << char(prevBuf[y][x]);

				printf(CSI "1;0m");
			}
		}
	}
}

void Game::DrawToMem()
{
	for (int i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->IsObjectDelete()) {
			enemyList.erase(enemyList.begin() + i);
			i = -1;
		}
	}

	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i]->IsObjectDelete()) {
			bulletList.erase(bulletList.begin() + i);

			i = -1;
		}
	}

	for (int i = 0; i < bonusList.size(); i++)
	{
		if (bonusList[i]->IsObjectDelete()) {
			bonusList.erase(bonusList.begin() + i);

			i = -1;
		}
	}

	for (int i = 0; i < allObjectList.size(); i++)
	{
		if (allObjectList[i]->IsObjectDelete()) {
			wData.vBuf[allObjectList[i]->GetY()][allObjectList[i]->GetX()] = u' ';
			allObjectList.erase(allObjectList.begin() + i);

			i = -1;
		}
	}

	for (int i = 0; i < allObjectList.size(); i++)
	{
		allObjectList[i]->DrawObject();
	}
}

void Game::ReloadGun(Player* player)
{
	player->SetGunState(false);

	this_thread::sleep_for(milliseconds(player->GetGunSpeed()));

	player->SetGunState(true);
}

void Game::ReloadEnGun(Enemy* enemy)
{
	enemy->SetGunState(false);

	this_thread::sleep_for(milliseconds(enemy->GetGunSpeed()));

	enemy->SetGunState(true);
}

void Game::Shot(int owner, GameObject* gmObj)
{
	if (gmObj->GetGunType() == SINGLESHOT) {
		if (owner == PLAYER) {
			bullet = new Bullet(&wData, gmObj->GetX() + gmObj->GetWidth(), gmObj->GetY() + gmObj->GetHeight() / 2, 1, Red);
			bullet->SetOwner(owner);
			bulletList.push_back(bullet);
			allObjectList.push_back(bullet);
		}
		else {
			bullet = new Bullet(&wData, gmObj->GetX() - 1, gmObj->GetY() + gmObj->GetHeight() / 2, 1, Red);
			bullet->SetOwner(owner);
			bulletList.push_back(bullet);
			allObjectList.push_back(bullet);
		}
	}
	else if (gmObj->GetGunType() == DOUBLESHOT) {
		for (int i = 0; i < 2; i++)
		{
			if (i == 0) {
				if (owner == PLAYER) {
					bullet = new Bullet(&wData, gmObj->GetX() + gmObj->GetWidth(), gmObj->GetY(), 1, Red);
					bullet->SetOwner(owner);
					bulletList.push_back(bullet);
					allObjectList.push_back(bullet);
				}
				else {
					bullet = new Bullet(&wData, gmObj->GetX() - 1, gmObj->GetY(), 1, Red);
					bullet->SetOwner(owner);
					bulletList.push_back(bullet);
					allObjectList.push_back(bullet);
				}
				
			}
			else {
				if (owner == PLAYER) {
					bullet = new Bullet(&wData, gmObj->GetX() + gmObj->GetWidth(), gmObj->GetY() + gmObj->GetHeight() - 1, 1, Red);
					bullet->SetOwner(owner);
					bulletList.push_back(bullet);
					allObjectList.push_back(bullet);
				}
				else {
					bullet = new Bullet(&wData, gmObj->GetX() - 1, gmObj->GetY() + gmObj->GetHeight() - 1, 1, Red);
					bullet->SetOwner(owner);
					bulletList.push_back(bullet);
					allObjectList.push_back(bullet);
				}
				
			}
		}
	}
}

void Game::SpawnBonus(Enemy* enemy, int type)
{
	if (type >= 3) return;

	bonus = new Bonus(&wData, enemy->GetX() - 1, enemy->GetY(), 1, Yellow);

	bonus->SetBonusType(type);
	allObjectList.push_back(bonus);
	bonusList.push_back(bonus);
}

void Game::SpawnEnemy(int x, int y, int type)
{
	enemy = new Enemy(&wData, x, y, 2, BrPurple);

	for (int i = 0; i < enemyList.size(); i++)
	{
		for (int h = 0; h < enemyList[i]->GetHeight(); h++)
		{
			for (int w = 0; w < enemyList[i]->GetWidth(); w++)
			{
				if (enemy->GetX() + w == enemyList[i]->GetX() + w && enemy->GetY() + h == enemyList[i]->GetY() + h) {
					return;
				}
			}
		}
	}

	enemy->SetEnemyType(type);
	allObjectList.push_back(enemy);
	enemyList.push_back(enemy);
}

void Game::Collision(Player* player) {

	for (int enemy = 0; enemy < enemyList.size(); enemy++)
	{

		bool finded = false;

		for (int h = 0; h < enemyList[enemy]->GetHeight(); h++)
		{
			for (int w = 0; w < enemyList[enemy]->GetWidth(); w++)
			{

				for (int ph = 0; ph < player->GetHeight(); ph++)
				{
					for (int pw = 0; pw < player->GetWidth(); pw++)
					{
						if ((enemyList[enemy]->GetX() + w == player->GetX() + pw) && (enemyList[enemy]->GetY() + h == player->GetY() + ph)) {

							enemyList[enemy]->DeleteObject();

							player->Death(worldIsRun);


							finded = true;
							break;
						}
					}
					if (finded) break;
				}
				if (finded) break;
			}
			if (finded) break;
		}

		finded = false;
	}

	for (int bullet = 0; bullet < bulletList.size(); bullet++)
	{

		bool finded = false;

		if (bulletList[bullet]->GetOwner() == PLAYER) {
			for (int enemy = 0; enemy < enemyList.size(); enemy++)
			{

				for (int h = 0; h < enemyList[enemy]->GetHeight(); h++)
				{
					for (int w = 0; w < enemyList[enemy]->GetWidth(); w++)
					{

						if (bulletList[bullet]->GetX() == enemyList[enemy]->GetX() + w &&
							bulletList[bullet]->GetY() == enemyList[enemy]->GetY() + h)
						{
							enemyList[enemy]->Hit(score, worldIsRun, win);
							bulletList[bullet]->DeleteObject();

							SpawnBonus(enemyList[enemy], rand() % 7);

							finded = true;

							break;
						}

					}
					if (finded) break;
				}
				if (finded) break;

			}
		}
		else if (bulletList[bullet]->GetOwner() == ENEMY) {
			for (int h = 0; h < player->GetHeight(); h++)
			{
				for (int w = 0; w < player->GetWidth(); w++)
				{
					if (bulletList[bullet]->GetX() == player->GetX() + w && bulletList[bullet]->GetY() == player->GetY() + h) {
						player->Death(worldIsRun);

						bulletList[bullet]->DeleteObject();

						finded = true;
						break;
					}
				}
				if (finded) break;
			}
			if (finded) break;
		}
		finded = false;
	}

	for (int bonus = 0; bonus < bonusList.size(); bonus++)
	{
		bool finded = false;
		for (int h = 0; h < bonusList[bonus]->GetHeight(); h++)
		{
			for (int w = 0; w < bonusList[bonus]->GetWidth(); w++)
			{

				for (int ph = 0; ph < player->GetHeight(); ph++)
				{
					for (int pw = 0; pw < player->GetWidth(); pw++)
					{

						if (bonusList[bonus]->GetX() + w == player->GetX() + pw &&
							bonusList[bonus]->GetY() + h == player->GetY() + ph) {

							bonusList[bonus]->DeleteObject();
							score += 150;

							if (bonusList[bonus]->GetBonusType() == GUNSPEED) {
								if (player->GetGunSpeed() > 500) {
									player->SetGunSpeed(player->GetGunSpeed() - 250);

									finded = true;
									break;
								}
								else {
									finded = true;
									break;
								}
							}
							else if (bonusList[bonus]->GetBonusType() == GUNTYPE) {
								player->SetGunType(DOUBLESHOT);

								finded = true;
								break;
							}
							else if (bonusList[bonus]->GetBonusType() == LIFE) {
								player->AddLifes();

								finded = true;
								break;
							}

						}
					}
					if (finded) break;
				}
				if (finded) break;
			}
			if (finded) break;
		}
		finded = false;
	}
}

void Game::RunWorld(bool& restart)
{
	srand(time(NULL));
	CreateWorld();

	bool pause = false;
	score = 0;

	thread hotKeys([&]
		{ HotKeys(pause); }
	);
	
	int tick = 0;
	int scrollSpeed = 2;

	Player* player = new Player(&wData, 15, ROWS / 2, 2, Cyan);
	allObjectList.push_back(player);

	SpawnEnemy(COLS - 10, 3 + rand() % (ROWS - 6), SMALL);

	while (worldIsRun) {

		if (pause) {

			SetPos(COLS / 2 - 2, ROWS / 2);
			cout << "PAUSED";

			while (pause) {

			}

			SetPos(COLS / 2 - 2, ROWS / 2);
			cout << "      ";

		}

		if (tick % player->GetSpeed() == 0) {
			player->MoveObject();

			if (GetAsyncKeyState(VK_SPACE) && player->GetGunState()) {
				Shot(PLAYER, player);
				thread reloadGun([&] {
					ReloadGun(player);
					});
				reloadGun.detach();
			}

		}

		for (int i = 0; i < enemyList.size(); i++)
		{

			if (tick % enemyList[i]->GetSpeed() == 0) {

				if (enemyList[i]->GetEnemyType() == SMALL) {
					enemyList[i]->CheckKamikadzeArea(player);
				}

				enemyList[i]->MoveObject();

				if ((tick % 20 == 0) && (enemyList[i]->GetGunState()) && (enemyList[i]->GetEnemyType() != SMALL)) {
					Shot(ENEMY, enemyList[i]); 
					enemy = enemyList[i];
					thread reloadEnGun([&] {
						ReloadEnGun(enemy);
						});
					reloadEnGun.detach();
				}
				
			}

		}

		for (int i = 0; i < bulletList.size(); i++)
		{
			if (tick % bulletList[i]->GetSpeed() == 0) bulletList[i]->MoveObject();
		}

		for (int i = 0; i < bonusList.size(); i++)
		{
			bonusList[i]->MoveObject();
		}

		Collision(player);

		DrawToMem();

		DrawChanges();

		if (tick % scrollSpeed == 0 && tick > 0) ScrollWindow();

		DrawInfo(player);

		Sleep(10);

		tick++;
	}

	DrawEndInfo(restart);

	hotKeys.join();

	printf(CSI "?1049l");
}