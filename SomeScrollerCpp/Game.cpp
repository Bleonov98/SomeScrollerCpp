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
			cout << GameMap[h][w] << flush;
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
		SetPos(COLS + 4, 20);
		cout << "CONGRATULATION! YOU WIN!";
	}
	else {
		SetPos(COLS + 11, 20);
		cout << "GAME OVER!";
	}

	SetPos(COLS + 5, 22);
	cout << "PRESS ENTER TO RESTART";
	SetPos(COLS + 7, 23);
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
	SetPos(COLS + 10, 2);
	cout << score;
	SetPos(COLS + 10, 4);
	// cout << player->GetLifes();
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
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i]->IsObjectDelete()) {
			bulletList.erase(bulletList.begin() + i);
		}
	}

	for (int i = 0; i < allObjectList.size(); i++)
	{
		if (allObjectList[i]->IsObjectDelete()) {
			wData.vBuf[allObjectList[i]->GetY()][allObjectList[i]->GetX()] = u' ';
			allObjectList.erase(allObjectList.begin() + i);
		}
	}

	for (int i = 0; i < allObjectList.size(); i++)
	{
		allObjectList[i]->DrawObject();
	}
}

void Game::SpawnEnemy(int x, int y)
{
	Enemy* enemy = new Enemy(&wData, x, y, 2, Blue);
	allObjectList.push_back(enemy);
	enemyList.push_back(enemy);
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

	Player* player = new Player(&wData, 15, ROWS / 2, 2, Red);
	allObjectList.push_back(player);

	SpawnEnemy(COLS - 10, 3 + rand() % ROWS);

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

			if (GetAsyncKeyState(VK_SPACE)) {
				Bullet* bullet = new Bullet(&wData, player->GetX() + REGULAR_WIDTH - 1, player->GetY() + REGULAR_HEIGHT / 2, 1, Red);
				bullet->SetOwner(PLAYER);
				bulletList.push_back(bullet);
				allObjectList.push_back(bullet);
			}
		}

		for (int i = 0; i < enemyList.size(); i++)
		{
			if (tick % enemyList[i]->GetSpeed() == 0) {
				enemyList[i]->MoveObject();
				if (tick % 20 == 0) {
					Bullet* bullet = new Bullet(&wData, enemyList[i]->GetX(), enemyList[i]->GetY() + REGULAR_HEIGHT / 2, 1, Red);
					bullet->SetOwner(ENEMY);
					bulletList.push_back(bullet);
					allObjectList.push_back(bullet);
				}
			}
		}

		for (int i = 0; i < bulletList.size(); i++)
		{
			if (tick % bulletList[i]->GetSpeed() == 0) bulletList[i]->MoveObject();
		}

		DrawToMem();

		DrawChanges();

		//Collision(player);

		//DrawInfo(player);

		Sleep(20);

		if (tick % scrollSpeed == 0 && tick > 0) ScrollWindow();
		
		tick++;
	}

	DrawEndInfo(restart);

	hotKeys.join();

	printf(CSI "?1049l");
}