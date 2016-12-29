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

// ����������WNDWIDTH��WNDHEIGHTΪ���ڵĿ�͸�
#define TIMER_ID             1		//��ʱ��ID
#define TIMER_ELAPSE         16		//��ʱ�����(ms)
#define WNDWIDTH             1380	//���ڵĿ�
#define WNDHEIGHT            700	//���ڵĸ�
#define TITLEBARHEIGHT       32		//��������

// ������Դ�ĳߴ�
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

//������Ϸ����
//#define MAX_TERRIAN_NUM      1		//���ؿ�����
//#define GRAVITY				10		//����
#define g_y					2
#define HERO_VEL_X			2		//Ӣ��ˮƽ�ٶ�
#define HERO_VEL_Y			12		//Ӣ����ֱ�ٶ�
#define FlyingDec			1		//��ը��ʱ��Ҫ�ļ��ٶ�
#define FlyingVel			16		//��ը��ʱ���ٶ�

// Ӣ�۽ṹ�壬Ӣ��λͼ��λ�á���С*/
typedef struct
{
	HBITMAP	hBmp;	//Ӣ��ͼƬ
	POINT	pos;	//Ӣ��
	POINT   vel;
	SIZE	size;
	int     curFrameIndex;
	int     maxFrameSize;
	int		position;  //״̬��0Ϊ���ң�1Ϊ����
	int		AllBleed;
	int		NowBleed;
} Hero;

//�����ṹ��
typedef struct
{
	HBITMAP	hBmp;	//����ͼƬ
	int     Vel;	//����Ĭ���ٶ�
	int		Radius;	//�����Ĵ�С
	int		Power;	//�����ı�ը������ը���ĳ��Ӽ���Ѫ��
	int		Range_Radius;	//������ը��Χ��ը�������ڵ��������������˺��ķ�ΧҪ����
	HBITMAP	BigBmp;	//���������ͼ����ʾ����Ļ��
	HBITMAP	HoldBmpL;
	HBITMAP	HoldBmpR;	//�����õ�������ͬʱ�����������������ģ����������ҵ�
} Weapon;

/*���νṹ�壬���η���λ�á���С*/
//typedef struct
//{
//	HBITMAP hBmp;
//	POINT	pos;
//	SIZE    size;
//} Terrian;

/*��ť�ṹ��*/
typedef struct
{
	HBITMAP	hBmp;	//��ťͼƬ
	POINT   pos;	//��ťλ��
	SIZE	size;	//��ť�ߴ�
	int		status;	//��ť״̬
}  GameButton;

/*��Ϸ״̬�ṹ��*/
typedef struct
{
	HBITMAP	hBmp;	//����ͼƬ
	int		status;	//��Ϸ״̬
	HBITMAP hS_Bmp;	//С��ͼ�б���ͼƬ
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
	int		type;	//1������׼��2������Ծ��3����ը����0������
} TargetCase;

typedef struct
{
	int		R_G_B[3];
	POINT	Points[WNDWIDTH * 5];
	int		Lenth;
	POINT	Destination;
} Track;

// ȫ�ֺ���

/*************************************************
Fucntion : ����Ӣ������
Parameter:
posX��posY��ʾ�������Ͻǵ�����
sizeX��sizeY��ʾ���ε�size
hBmp��ʾλͼ���
curFrameIndex��ǰ֡
maxFrameSize���֡��
*************************************************/
Hero CreateHero(int posX, int posY, int sizeX, int sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize, int position_hero, int allbleed, int nowbleed);

/*************************************************
Fucntion : ������Ϸ״̬
Parameter:
posX��posY��ʾ�������Ͻǵ�����
sizeX��sizeY��ʾ���ε�size
hBmp��ʾλͼ���
*************************************************/
GameButton CreateGameButton(int posX, int posY, int sizeX, int sizeY, HBITMAP hBmp, int status);

/*************************************************
Fucntion : ������������
Parameter:
posX��posY��ʾ�������Ͻǵ�����
sizeX��sizeY��ʾ���ε�size
*************************************************/
//Terrian CreateTerrian(int posX, int posY, int sizeX, int sizeY, HBITMAP hBmp);

#endif // !GLOBAL_H
