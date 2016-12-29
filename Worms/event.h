#ifndef EVENT_H
#define EVENT_H

#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <mmsystem.h>
#include <imm.h>  
#include "global.h"
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "winmm.lib")
#define Pi 3.1415927
#define temp_r_firing 7
#define temp_R_firing 80
#define temp_r_power 1
#define Times_Of_Map 9
#define BloodBarSizeX 80
#define BloodBarSizeY 30
#define BloodBarPosX 200
#define BloodBarPosY 30
#define BloodBarPosPlus 20


// 窗体过程函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

VOID deletetempfile();
// 初始化
VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam);

//双缓冲绘制
VOID Render(HWND hWnd);
VOID Render_Start(HWND hWnd);//绘制游戏开始界面
VOID Render_Game(HWND hWnd);//绘制游戏界面
VOID Render_End(HWND hWnd);//绘制游戏结束界面
VOID Render_End_Red(HWND hWnd);//绘制游戏结束红胜界面
VOID Render_End_Yellow(HWND hWnd);//绘制游戏结束黄胜界面
VOID Render_Help(HWND hWnd);//绘制帮助界面
VOID Render_Guide(HWND hWnd);//绘制游戏教程界面

//定时器事件
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

//hero更新
VOID HeroUpdate();

//判断是否点击按钮
BOOL ButtonClicked(POINT, RECT);

//键盘按下事件处理
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

//键盘松开事件处理
VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

//左鼠标点击事件
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif // !EVENT_H
