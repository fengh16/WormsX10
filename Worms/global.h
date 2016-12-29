#ifndef GLOBAL_H
#define GLOBAL_H

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <math.h>
#include "resource.h"

#define num_hero 8
#define num_weapon 4
#define num_AidCase 7
#define num_TargetCase 6
#define Weapon_Big_Size 108

// 定义宏变量，WNDWIDTH、WNDHEIGHT为窗口的宽和高
#define TIMER_ID             1		//计时器ID
#define TIMER_ELAPSE         16		//计时器间隔(ms)
#define WNDWIDTH             1380	//窗口的宽
#define WNDHEIGHT            700	//窗口的高
#define TITLEBARHEIGHT       32		//标题栏高

// 定义资源的尺寸
#define HERO_SIZE_X					30
#define HERO_SIZE_Y					30
#define HERO_MAX_FRAME_NUM			20
#define GAME_START_BUTTON_SIZE_X	147
#define GAME_START_BUTTON_SIZE_Y	60
#define ChooseBackHeight			59
#define ChooseBackWidth				60
#define AidCaseSize_X				18
#define AidCaseSize_Y				21
#define TargetCaseSize_X			18
#define TargetCaseSize_Y			21

//定义游戏常量
//#define MAX_TERRIAN_NUM      1		//最大地块数量
//#define GRAVITY				10		//重力
#define g_y					2
#define HERO_VEL_X			2		//英雄水平速度
#define HERO_VEL_Y			12		//英雄竖直速度
#define FlyingDec			1		//被炸飞时需要的加速度
#define FlyingVel			16		//被炸飞时的速度

// 英雄结构体，英雄位图、位置、大小*/
typedef struct
{
	HBITMAP	hBmp;	//英雄图片
	POINT	pos;	//英雄
	POINT   vel;
	SIZE	size;
	int     curFrameIndex;
	int     maxFrameSize;
	int		position;  //状态，0为向右，1为向左
	int		AllBleed;
	int		NowBleed;
} Hero;

//武器结构体
typedef struct
{
	HBITMAP	hBmp;	//武器图片
	int     Vel;	//武器默认速度
	int		Radius;	//武器的大小
	int		Power;	//武器的爆炸威力（炸中心虫子几点血）
	int		Range_Radius;	//武器爆炸范围（炸掉多少内的土，但虫子受伤害的范围要更大）
	HBITMAP	BigBmp;	//武器高清大图，显示在屏幕上
	HBITMAP	HoldBmpL;
	HBITMAP	HoldBmpR;	//虫子拿的武器，同时如果虫子向左则向左的，向右则向右的
} Weapon;

/*地形结构体，地形方块位置、大小*/
//typedef struct
//{
//	HBITMAP hBmp;
//	POINT	pos;
//	SIZE    size;
//} Terrian;

/*按钮结构体*/
typedef struct
{
	HBITMAP	hBmp;	//按钮图片
	POINT   pos;	//按钮位置
	SIZE	size;	//按钮尺寸
	int		status;	//按钮状态
}  GameButton;

/*游戏状态结构体*/
typedef struct
{
	HBITMAP	hBmp;	//背景图片
	int		status;	//游戏状态
	HBITMAP hS_Bmp;	//小地图中背景图片
}  GameStatus;

typedef struct
{
	HBITMAP	hBmp;
	int		Power;
	POINT	pos;
	BOOL	exist;
} AidCase;

#define DFrontSightCase	1
#define DJumpCase		2
#define DBoomCase		3
#define DSpringCase		0

typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	BOOL	exist;
	int		type;	//1代表瞄准，2代表跳跃，3代表炸弹，0代表弹簧
} TargetCase;

typedef struct
{
	int		R_G_B[3];
	POINT	Points[WNDWIDTH * 5];
	int		Lenth;
	POINT	Destination;
} Track;

// 全局函数

/*************************************************
Fucntion : 创建英雄人物
Parameter:
posX、posY表示矩形左上角的坐标
sizeX、sizeY表示矩形的size
hBmp表示位图句柄
curFrameIndex当前帧
maxFrameSize最大帧率
*************************************************/
Hero CreateHero(int posX, int posY, int sizeX, int sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize, int position_hero, int allbleed, int nowbleed);

/*************************************************
Fucntion : 创建游戏状态
Parameter:
posX、posY表示矩形左上角的坐标
sizeX、sizeY表示矩形的size
hBmp表示位图句柄
*************************************************/
GameButton CreateGameButton(int posX, int posY, int sizeX, int sizeY, HBITMAP hBmp, int status);

/*************************************************
Fucntion : 创建单个地形
Parameter:
posX、posY表示矩形左上角的坐标
sizeX、sizeY表示矩形的size
*************************************************/
//Terrian CreateTerrian(int posX, int posY, int sizeX, int sizeY, HBITMAP hBmp);

#endif // !GLOBAL_H
