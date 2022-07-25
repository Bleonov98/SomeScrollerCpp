#pragma once
#include "GameObject.h"

class Game
{
private:

    wd wData;

    char16_t prevBuf[ROWS][COLS];
    char coord[100];

    unsigned char GameMap[ROWS][1025];

    unsigned char prevActiveAreaBuf[ROWS][COLS];
    unsigned char activeAreaBuf[ROWS][COLS];

    int scrollX = 0;

    bool worldIsRun = true, win = false, alreadySpawn = false;

    int score = 0;

    vector<GameObject*> allObjectList;
    vector<Bullet*> bulletList;
    vector<Enemy*> enemyList;

    HINSTANCE hInstance;

    class VirtualTerminal {
    public:

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO csbi;

        _SMALL_RECT Rect;

        HWND hWindowConsole = GetForegroundWindow();

        int Width = 90, Height = 55, err = 30;

        bool Terminal() {

            // Set output mode to handle virtual terminal sequences
            if (hOut == INVALID_HANDLE_VALUE)
            {
                return false;
            }
            HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
            if (hIn == INVALID_HANDLE_VALUE)
            {
                return false;
            }

            DWORD dwOriginalOutMode = 0;
            DWORD dwOriginalInMode = 0;
            if (!GetConsoleMode(hOut, &dwOriginalOutMode))
            {
                return false;
            }
            if (!GetConsoleMode(hIn, &dwOriginalInMode))
            {
                return false;
            }

            DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
            DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

            DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
            if (!SetConsoleMode(hOut, dwOutMode))
            {
                // we failed to set both modes, try to step down mode gracefully.
                dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
                if (!SetConsoleMode(hOut, dwOutMode))
                {
                    // Failed to set any VT mode, can't do anything here.
                    return -1;
                }
            }

            DWORD dwInMode = dwOriginalInMode | ENABLE_VIRTUAL_TERMINAL_INPUT;
            if (!SetConsoleMode(hIn, dwInMode))
            {
                // Failed to set VT input mode, can't do anything here.
                return -1;
            }

            return 0;
        }

        void SetScreenSize() {
            COORD monitorSize = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
            COORD bufferSize = { Width, Height };

            _SMALL_RECT Rect;
            Rect.Top = 0;
            Rect.Left = 0;
            Rect.Bottom = Height - 1;
            Rect.Right = Width - 1;

            SetConsoleScreenBufferSize(hOut, bufferSize);

            SetWindowPos(hWindowConsole, HWND_NOTOPMOST,
                (monitorSize.X / 2 - Width * 4 - err * 4), (monitorSize.Y / 2 - Height * 8 - err), 0, 0,
                SWP_NOZORDER | SWP_NOREPOSITION | SWP_NOREDRAW);

            SetConsoleWindowInfo(hOut, TRUE, &Rect);

            SetWindowLong(hWindowConsole, GWL_STYLE, GetWindowLong(hWindowConsole, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

            SetConsoleTitleA("ScrollerTypeGame C++");
        }

        void SetConsoleFont() {

            CONSOLE_FONT_INFOEX cfi;

            cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
            cfi.nFont = 6;
            cfi.dwFontSize.X = 30;
            cfi.dwFontSize.Y = 13;
            cfi.FontFamily = 54;
            cfi.FontWeight = 1000;

            wcscpy_s(cfi.FaceName, L"Lucida Console");
            SetCurrentConsoleFontEx(hOut, 0, &cfi);
        }
    };
    
    void ScrollWindow() {

        for (int i = 0; i < ROWS; i++)
        {   
            int activeX = 0;
            for (int j = scrollX; j < scrollX + COLS; j++)
            {
                activeAreaBuf[i][activeX] = GameMap[i][j];
                activeX++;
            }
        }

        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (i < 2 && i >= ROWS - 1) continue;
                if (prevActiveAreaBuf[i][j] != activeAreaBuf[i][j] && (wData.vBuf[i][j] == 0 || wData.vBuf[i][j] == ' '))
                {
                    prevActiveAreaBuf[i][j] = activeAreaBuf[i][j];

                    SetPos(j, i + 1);

                    cout << prevActiveAreaBuf[i][j];
                }
            }
        }

        if (scrollX + COLS < 1023) scrollX++;
    }

    VirtualTerminal term; // console setting


protected:

    void SetPos(int x, int y);

    void DrawArea();

    void CreateWorld();

    void DrawToMem();

    void HotKeys(bool& pause);

    void DrawEndInfo(bool& restart);

    void DrawInfo(Player* player);

    void DrawChanges();

    void SpawnEnemy(int x, int y, int type);

    void Shot(int owner, GameObject* gmObj);

    //void Collision(Player* player);

public:

    void RunWorld(bool& restart);

};

