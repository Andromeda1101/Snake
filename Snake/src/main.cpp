#include "inc/screen.h"
#include<windows.h>
#include<iostream>
#include<conio.h>
using namespace std;

void full_screen() //全屏
{   
    HWND hwnd = GetForegroundWindow();
    int cx = GetSystemMetrics(SM_CXSCREEN);            /* 屏幕宽度 像素 */
    int cy = GetSystemMetrics(SM_CYSCREEN);            /* 屏幕高度 像素 */

    LONG l_WinStyle = GetWindowLong(hwnd,GWL_STYLE);   /* 获取窗口信息 */
    /* 设置窗口信息 最大化 取消标题栏及边框 */
    SetWindowLong(hwnd,GWL_STYLE,(l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);

    SetWindowPos(hwnd, HWND_TOP, 0, 0, cx, cy, 0);

}

int main()
{
    full_screen();
    printf("Please full screen!");
    getch();
    Sleep(250);
    CONSOLE_CURSOR_INFO cursor_info = { 20 , 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
    Sleep(250);
    screen Snake_Game;
    while(1)
    {
        Snake_Game.configGame();
    }
    cursor_info = { 20 , 100 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
    return 0;
}