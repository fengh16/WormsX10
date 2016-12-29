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


// ������̺���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

VOID deletetempfile();
// ��ʼ��
VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam);

//˫�������
VOID Render(HWND hWnd);
VOID Render_Start(HWND hWnd);//������Ϸ��ʼ����
VOID Render_Game(HWND hWnd);//������Ϸ����
VOID Render_End(HWND hWnd);//������Ϸ��������
VOID Render_End_Red(HWND hWnd);//������Ϸ������ʤ����
VOID Render_End_Yellow(HWND hWnd);//������Ϸ������ʤ����
VOID Render_Help(HWND hWnd);//���ư�������
VOID Render_Guide(HWND hWnd);//������Ϸ�̳̽���

//��ʱ���¼�
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

//hero����
VOID HeroUpdate();

//�ж��Ƿ�����ť
BOOL ButtonClicked(POINT, RECT);

//���̰����¼�����
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

//�����ɿ��¼�����
VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

//��������¼�
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif // !EVENT_H
