#include "event.h"
#include "item.h"
#define WAIT_TIME 10
POINT Terrianheight[WNDWIDTH * 2 + 2];
POINT temp_point[5];
POINT Temp_Boom, Temp_Boom_i;
POINT SmallMapPos = {WNDWIDTH * 9 / 14 - 15, 20};
POINT SmallMapSize = {WNDWIDTH * 2 / Times_Of_Map, WNDHEIGHT / Times_Of_Map};
POINT WindPos = {WNDWIDTH / 2 - 2 * 29 - 10, 30};
POINT Pre;
TCHAR szDist[13];		//比分
TCHAR BloodNum[9];		//虫子现在的血量
TCHAR BloodDecNum[9];	//虫子减少的血量
TCHAR HeroNumText[5];	//虫子标号
TCHAR DeadText[11];		//死亡信息
TCHAR mcicmdBoom[300], mcicmdChangeWeapon[300], mcicmdDrown[300], mcicmdHeroDie[300], mcicmdHi1[300], mcicmdHi2[300], mcicmdHi3[300], mcicmdHi4[300], mcicmdHi5[300], mcicmdHi6[300], mcicmdHit[300], mcicmdMySpring[300], mcicmdPressButton[300];
TCHAR tmpBoomwav[_MAX_PATH], tmpChangeWeaponwav[_MAX_PATH], tmpDrownwav[_MAX_PATH], tmpHeroDiewav[_MAX_PATH], tmpHi1wav[_MAX_PATH], tmpHi2wav[_MAX_PATH], tmpHi3wav[_MAX_PATH], tmpHi4wav[_MAX_PATH], tmpHi5wav[_MAX_PATH], tmpHi6wav[_MAX_PATH], tmpHitwav[_MAX_PATH], tmpMySpringwav[_MAX_PATH], tmpPressButtonwav[_MAX_PATH];
HBRUSH hBrush;
HPEN hPen;
BOOL Hitting();
BOOL If_Fired = FALSE;
BOOL FirstBoom;
BOOL Controlable = TRUE;
BOOL If_AI_Controling = FALSE;
BOOL Touched;
BOOL NOT_Firing_Me;
BOOL ShowTrack[2] = {FALSE};
BOOL JumpHigher[2] = {FALSE};
BOOL HeroShout = TRUE;
BOOL IF_Guide_ChangedWorld = FALSE;
VOID DefineHeight();
VOID GetHighest();
VOID HeroDie(int);
VOID Hit(int Degree, int Power, int x, int y, int IF_LEFT, int vel0);
VOID CreateAidCase();
VOID CreateTargetCase();
VOID CreateWindSpeed();
VOID AI(int);
VOID Render_Choose_AI(HWND hWnd);
VOID GetTrack(BOOL If_Need_Draw, int Degree, int Power, int x, int y, int IF_LEFT, int vel0);
double temp_tangent;
int velx, vely;
int temp_degree_of_wind;
int temp_new_try, a_y = 0, if_pressing_ad[num_hero + 1] = {0}, if_ChangeDirection = 0, if_jump[num_hero + 1] = {0}, min = WNDHEIGHT, max = 0, SeaLevel, i_hero, now_hero = 1, R_blood, G_blood, temp_new_try_p, resolution_ratio = 25, max_of_height, temp_compare, No_Hero_Dead[num_hero + 1];
int No_Hero_Dec[num_hero + 1][2];	//第一列表示时间，第二列表示数字
int num_alive[3] = {num_hero, num_hero / 2, num_hero - num_hero / 2};
int i_AidCase;
int i_TargetCase;
int temp_x, temp_y;
int Status_OF_Game = 0;				//0表示正在选方向，1表示正在选力度，2表示发射那一刻，3表示正在发射,4表示发射完毕，需要换人
int Degree_Of_Firing = 0;			//表示圆形的瞄准
int Degree_Of_power = 30, power_add[num_hero + 1] = {0};
int Power_Of_Boom, Range_Of_Boom;
int gameover = 0, i_Weapon, now_Weapon = 0;
int now_hero_1 = 1, now_hero_2 = num_hero;
int temp_boom_int;
int Now_Screen_Pos = - 1;
int Pre_Screen_Pos;
int temp_hero_i;
int temp_createHero;
int WindSpeed;
int temp_createAidCase, temp_createTargetCase, temp_i;
int if_add_wind;
int temp_bloodbar;
int Game_Type = 1;	//1代表AI模式，0代表双人对战，2代表指导模式
int AI_Aim;
int AI_Type;	//0_Easy; 1_Normal; 2_Hard;
int AI_Step[3] = {0};	//第一位代表现在需要向哪个方向移动多少次，第二位代表应该向哪个方向像哪里（上或者下，上为正）来进行准星的调整，第三位代表应该蓄力多长时间（单位都是一次刷新间隔）
int AI_WAIT = 0;
int AI_Temp_Hero;	//在AI程序中使用，用于判断现有的可以满足条件的人物中距离最近的一个
int Hero_Boomed;	//在探测轨迹的时候使用，0表示现在没有炸到，否则表示炸到了谁
int temp_i_AI;
int wait = WAIT_TIME;
int GuideModeStatus = 0;	//0刚进入，1介绍场景移动，小地图，各个按钮的作用；2介绍移动，3介绍跳跃，4介绍切换武器，5介绍海洋，6介绍血包血量，7介绍地雷，弹簧，瞄准包，8介绍风速，9开始介绍发射方法
int temp_i_GUIDE;

VOID deletetempfile()			//删除为了播放音乐而产生的临时文件
{
    mciSendString(_T("stop myBoom"), NULL, 0, NULL);
    mciSendString(_T("close myBoom"), NULL, 0, NULL);
	DeleteFile(tmpBoomwav);
    mciSendString(_T("stop myChangeWeapon"), NULL, 0, NULL);
    mciSendString(_T("close myChangeWeapon"), NULL, 0, NULL);
	DeleteFile(tmpChangeWeaponwav);
    mciSendString(_T("stop myDrown"), NULL, 0, NULL);
    mciSendString(_T("close myDrown"), NULL, 0, NULL);
	DeleteFile(tmpDrownwav);
    mciSendString(_T("stop myHeroDie"), NULL, 0, NULL);
    mciSendString(_T("close myHeroDie"), NULL, 0, NULL);
	DeleteFile(tmpHeroDiewav);
    mciSendString(_T("stop myHi1"), NULL, 0, NULL);
    mciSendString(_T("close myHi1"), NULL, 0, NULL);
	DeleteFile(tmpHi1wav);
    mciSendString(_T("stop myHi2"), NULL, 0, NULL);
    mciSendString(_T("close myHi2"), NULL, 0, NULL);
	DeleteFile(tmpHi2wav);
    mciSendString(_T("stop myHi3"), NULL, 0, NULL);
    mciSendString(_T("close myHi3"), NULL, 0, NULL);
	DeleteFile(tmpHi3wav);
    mciSendString(_T("stop myHi4"), NULL, 0, NULL);
    mciSendString(_T("close myHi4"), NULL, 0, NULL);
	DeleteFile(tmpHi4wav);
    mciSendString(_T("stop myHi5"), NULL, 0, NULL);
    mciSendString(_T("close myHi5"), NULL, 0, NULL);
	DeleteFile(tmpHi5wav);
    mciSendString(_T("stop myHi6"), NULL, 0, NULL);
    mciSendString(_T("close myHi6"), NULL, 0, NULL);
	DeleteFile(tmpHi6wav);
    mciSendString(_T("stop myHit"), NULL, 0, NULL);
    mciSendString(_T("close myHit"), NULL, 0, NULL);
	DeleteFile(tmpHitwav);
    mciSendString(_T("stop myMySpring"), NULL, 0, NULL);
    mciSendString(_T("close myMySpring"), NULL, 0, NULL);
	DeleteFile(tmpMySpringwav);
    mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
    mciSendString(_T("close myPressButton"), NULL, 0, NULL);
	DeleteFile(tmpPressButtonwav);
}

BOOL ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName){//中间调用函数
    // 创建文件
    HANDLE hFile = ::CreateFile(strDstFile, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return false;

    // 查找资源文件中、加载资源到内存、得到资源大小
    HRSRC    hRes    = ::FindResource(NULL, strResName, strResType);
    HGLOBAL    hMem    = ::LoadResource(NULL, hRes);
    DWORD    dwSize    = ::SizeofResource(NULL, hRes);
    
    // 写入文件
    DWORD dwWrite = 0;      // 返回写入字节
    ::WriteFile(hFile, hMem, dwSize, &dwWrite, NULL);
    ::CloseHandle(hFile);

    return true;
}

VOID Take_Weapon()
{
	m_hero[0].size.cx = m_Weapon[now_Weapon].Radius * 2;
	m_hero[0].size.cy = m_Weapon[now_Weapon].Radius * 2;
	m_hero[0].hBmp = m_Weapon[now_Weapon].hBmp;
	Power_Of_Boom = m_Weapon[now_Weapon].Power;
	Range_Of_Boom = m_Weapon[now_Weapon].Range_Radius;
}

BOOL Now_Hero_Change()
{
	HeroShout = TRUE;
	wait = WAIT_TIME;
	if(num_alive[0] > 0)
	{
		if(now_hero == now_hero_1 && num_alive[2] > 0)
			do
			{
					now_hero_2 = (now_hero = (now_hero_2 - num_hero / 2) % (num_hero - num_hero / 2) + num_hero / 2 + 1);
			}
			while(!m_hero[now_hero].NowBleed);
		else if(num_alive[1] > 0)
			do
			{
					now_hero_1 = (now_hero = now_hero_1 % (num_hero / 2) + 1);
			}
			while(!m_hero[now_hero].NowBleed);
		return TRUE;
	}
	else
		return FALSE;
}

VOID Init_Again(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	IF_Guide_ChangedWorld = FALSE;
	GuideModeStatus = 0;
	KillTimer(hWnd, TIMER_ID);
	SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
	Status_OF_Game = 0;
	if_ChangeDirection = 0;
	now_hero = 1;
	min = WNDHEIGHT;
	max = 0;
	num_alive[0] = num_hero;
	num_alive[1] = num_hero / 2;
	num_alive[2] = num_hero - num_hero / 2;
	Degree_Of_Firing = 0;
	Degree_Of_power = 30;
	for(temp_i = 0; temp_i <= num_hero; temp_i++)
		power_add[temp_i] = 0;
	If_AI_Controling = FALSE;
	gameover = 0;
	now_Weapon = 0;
	Take_Weapon();
	now_hero_1 = 1;
	now_hero_2 = num_hero;
	for(i_hero = 1; i_hero <= num_hero; i_hero++)
	{
		if_pressing_ad[i_hero] = 0;
		if_jump[i_hero] = 0;
		No_Hero_Dec[i_hero][0] = 0;
		No_Hero_Dec[i_hero][1] = 0;
	}
	InvalidateRect(hWnd, NULL, TRUE);
	DefineHeight();
	int temp_createHero;
	m_hero[0] = CreateHero(0, 0, 0, 0, NULL, 0, HERO_MAX_FRAME_NUM, 0, 0, 0);
	for(i_hero = 1; i_hero <= num_hero; i_hero++)
	{
		do
		{
			temp_createHero = 1 + rand() % (WNDWIDTH * 2 - HERO_SIZE_X - 2);
		}
		while(Terrianheight[temp_createHero].y > SeaLevel - 25);
		m_hero[i_hero] = CreateHero(temp_createHero, 0 - HERO_SIZE_Y, HERO_SIZE_X, HERO_SIZE_Y, m_hHeroBmp, 0, HERO_MAX_FRAME_NUM, 0, 100, 100);
		for(temp_hero_i = 1; temp_hero_i < i_hero && (m_hero[temp_hero_i].pos.x - m_hero[i_hero].pos.x <= - HERO_SIZE_X - 20 || m_hero[temp_hero_i].pos.x - m_hero[i_hero].pos.x >= HERO_SIZE_X + 20); temp_hero_i++);
		if(temp_hero_i < i_hero)
			i_hero--;
	}
	for(i_AidCase = 0; i_AidCase <= num_AidCase; i_AidCase++)
		m_AidCase[i_AidCase].exist = FALSE;
	for(i_TargetCase = 0; i_TargetCase <= num_TargetCase; i_TargetCase++)
		m_TargetCase[i_TargetCase].exist = FALSE;
	Now_Screen_Pos = -1;
	CreateWindSpeed();
	Controlable = TRUE;
	ShowTrack[0] = FALSE;
	ShowTrack[1] = FALSE;
	JumpHigher[0] = FALSE;
	JumpHigher[1] = FALSE;
	wait = WAIT_TIME;
	Take_Weapon();
	HeroShout = TRUE;
}

VOID HeroShoutSelect()
{
	switch (rand() % 6)
	{
	case 1:
	{
		mciSendString(_T("stop myHi1"), NULL, 0, NULL);
		mciSendString(_T("close myHi1"), NULL, 0, NULL);
		mciSendString(mcicmdHi1, NULL,0, NULL);
		mciSendString(_T("play myHi1"), NULL, 0, NULL);
	}
	break;
	case 2:
	{
		mciSendString(_T("stop myHi2"), NULL, 0, NULL);
		mciSendString(_T("close myHi2"), NULL, 0, NULL);
		mciSendString(mcicmdHi2, NULL,0, NULL);
		mciSendString(_T("play myHi2"), NULL, 0, NULL);
	}
	break;
	case 3:
	{
		mciSendString(_T("stop myHi3"), NULL, 0, NULL);
		mciSendString(_T("close myHi3"), NULL, 0, NULL);
		mciSendString(mcicmdHi3, NULL,0, NULL);
		mciSendString(_T("play myHi3"), NULL, 0, NULL);
	}
	break;
	case 4:
	{
		mciSendString(_T("stop myHi4"), NULL, 0, NULL);
		mciSendString(_T("close myHi4"), NULL, 0, NULL);
		mciSendString(mcicmdHi4, NULL,0, NULL);
		mciSendString(_T("play myHi4"), NULL, 0, NULL);
	}
	break;
	case 5:
	{
		mciSendString(_T("stop myHi5"), NULL, 0, NULL);
		mciSendString(_T("close myHi5"), NULL, 0, NULL);
		mciSendString(mcicmdHi5, NULL,0, NULL);
		mciSendString(_T("play myHi5"), NULL, 0, NULL);
	}
	break;
	case 0:
	{
		mciSendString(_T("stop myHi6"), NULL, 0, NULL);
		mciSendString(_T("close myHi6"), NULL, 0, NULL);
		mciSendString(mcicmdHi6, NULL,0, NULL);
		mciSendString(_T("play myHi6"), NULL, 0, NULL);
	}
	break;
	default:
		break;
	}
	HeroShout = FALSE;
}

VOID Same_Team_Change()
{
	HeroShout = TRUE;
	wait = WAIT_TIME;
	if(num_alive[0] > 0)
	{
		if(now_hero == now_hero_2 && num_alive[2] > 0)
			do
			{
					now_hero_2 = (now_hero = (now_hero_2 - num_hero / 2) % (num_hero - num_hero / 2) + num_hero / 2 + 1);
			}
			while(!m_hero[now_hero].NowBleed);
		else if(num_alive[1] > 0)
			do
			{
					now_hero_1 = (now_hero = now_hero_1 % (num_hero / 2) + 1);
			}
			while(!m_hero[now_hero].NowBleed);
	}
}

VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    ::GetTempPath(_MAX_PATH,tmpBoomwav);
	TCHAR BoomTempFile[] = _T("FHBoomTempFile.wav");
	lstrcat(tmpBoomwav, BoomTempFile);
    ExtractResource(tmpBoomwav, _T("WAVE"), MAKEINTRESOURCE(IDR_Boom));
    _stprintf_s(mcicmdBoom, _T("open \"%s\" alias myBoom"),tmpBoomwav);
    mciSendString(mcicmdBoom, NULL,0, NULL);

    ::GetTempPath(_MAX_PATH,tmpChangeWeaponwav);
	TCHAR ChangeWeaponTempFile[] = _T("FHChangeWeaponTempFile.wav");
	lstrcat(tmpChangeWeaponwav, ChangeWeaponTempFile);
    ExtractResource(tmpChangeWeaponwav, _T("WAVE"), MAKEINTRESOURCE(IDR_ChangeWeapon));
    _stprintf_s(mcicmdChangeWeapon, _T("open \"%s\" alias myChangeWeapon"),tmpChangeWeaponwav);
    mciSendString(mcicmdChangeWeapon, NULL,0, NULL);

    ::GetTempPath(_MAX_PATH,tmpDrownwav);
	TCHAR DrownTempFile[] = _T("FHDrownTempFile.wav");
	lstrcat(tmpDrownwav, DrownTempFile);
    ExtractResource(tmpDrownwav, _T("WAVE"), MAKEINTRESOURCE(IDR_Drown));
    _stprintf_s(mcicmdDrown, _T("open \"%s\" alias myDrown"),tmpDrownwav);
    mciSendString(mcicmdDrown, NULL,0, NULL);

    ::GetTempPath(_MAX_PATH,tmpHeroDiewav);
	TCHAR HeroDieTempFile[] = _T("FHHeroDieTempFile.wav");
	lstrcat(tmpHeroDiewav, HeroDieTempFile);
    ExtractResource(tmpHeroDiewav, _T("WAVE"), MAKEINTRESOURCE(IDR_HeroDie));
    _stprintf_s(mcicmdHeroDie, _T("open \"%s\" alias myHeroDie"),tmpHeroDiewav);
    mciSendString(mcicmdHeroDie, NULL,0, NULL);

    ::GetTempPath(_MAX_PATH,tmpHi1wav);
	TCHAR Hi1TempFile[] = _T("FHHi1TempFile.wav");
	lstrcat(tmpHi1wav, Hi1TempFile);
    ExtractResource(tmpHi1wav, _T("WAVE"), MAKEINTRESOURCE(IDR_Hi1));
    _stprintf_s(mcicmdHi1, _T("open \"%s\" alias myHi1"),tmpHi1wav);
    mciSendString(mcicmdHi1, NULL,0, NULL);

    ::GetTempPath(_MAX_PATH,tmpHi2wav);
	TCHAR Hi2TempFile[] = _T("FHHi2TempFile.wav");
	lstrcat(tmpHi2wav, Hi2TempFile);
    ExtractResource(tmpHi2wav, _T("WAVE"), MAKEINTRESOURCE(IDR_Hi2));
    _stprintf_s(mcicmdHi2, _T("open \"%s\" alias myHi2"),tmpHi2wav);
    mciSendString(mcicmdHi2, NULL,0, NULL);

    ::GetTempPath(_MAX_PATH,tmpHi3wav);
	TCHAR Hi3TempFile[] = _T("FHHi3TempFile.wav");
	lstrcat(tmpHi3wav, Hi3TempFile);
    ExtractResource(tmpHi3wav, _T("WAVE"), MAKEINTRESOURCE(IDR_Hi3));
    _stprintf_s(mcicmdHi3, _T("open \"%s\" alias myHi3"),tmpHi3wav);
    mciSendString(mcicmdHi3, NULL,0, NULL);

    ::GetTempPath(_MAX_PATH,tmpHi4wav);
	TCHAR Hi4TempFile[] = _T("FHHi4TempFile.wav");
	lstrcat(tmpHi4wav, Hi4TempFile);
    ExtractResource(tmpHi4wav, _T("WAVE"), MAKEINTRESOURCE(IDR_Hi4));
    _stprintf_s(mcicmdHi4, _T("open \"%s\" alias myHi4"),tmpHi4wav);
    mciSendString(mcicmdHi4, NULL,0, NULL);

    ::GetTempPath(_MAX_PATH,tmpHi5wav);
	TCHAR Hi5TempFile[] = _T("FHHi5TempFile.wav");
	lstrcat(tmpHi5wav, Hi5TempFile);
    ExtractResource(tmpHi5wav, _T("WAVE"), MAKEINTRESOURCE(IDR_Hi5));
    _stprintf_s(mcicmdHi5, _T("open \"%s\" alias myHi5"),tmpHi5wav);
    mciSendString(mcicmdHi5, NULL,0, NULL);

    ::GetTempPath(_MAX_PATH,tmpHi6wav);
	TCHAR Hi6TempFile[] = _T("FHHi6TempFile.wav");
	lstrcat(tmpHi6wav, Hi6TempFile);
    ExtractResource(tmpHi6wav, _T("WAVE"), MAKEINTRESOURCE(IDR_Hi6));
    _stprintf_s(mcicmdHi6, _T("open \"%s\" alias myHi6"),tmpHi6wav);
    mciSendString(mcicmdHi6, NULL,0, NULL);

    ::GetTempPath(_MAX_PATH,tmpHitwav);
	TCHAR HitTempFile[] = _T("FHHitTempFile.wav");
	lstrcat(tmpHitwav, HitTempFile);
    ExtractResource(tmpHitwav, _T("WAVE"), MAKEINTRESOURCE(IDR_Hit));
    _stprintf_s(mcicmdHit, _T("open \"%s\" alias myHit"),tmpHitwav);
    mciSendString(mcicmdHit, NULL,0, NULL);

    ::GetTempPath(_MAX_PATH,tmpMySpringwav);
	TCHAR MySpringTempFile[] = _T("FHMySpringTempFile.wav");
	lstrcat(tmpMySpringwav, MySpringTempFile);
    ExtractResource(tmpMySpringwav, _T("WAVE"), MAKEINTRESOURCE(IDR_MSPRING));
    _stprintf_s(mcicmdMySpring, _T("open \"%s\" alias myMySpring"),tmpMySpringwav);
    mciSendString(mcicmdMySpring, NULL,0, NULL);

    ::GetTempPath(_MAX_PATH,tmpPressButtonwav);
	TCHAR PressButtonTempFile[] = _T("FHPressButtonTempFile.wav");
	lstrcat(tmpPressButtonwav, PressButtonTempFile);
    ExtractResource(tmpPressButtonwav, _T("WAVE"), MAKEINTRESOURCE(IDR_PressButton));
    _stprintf_s(mcicmdPressButton, _T("open \"%s\" alias myPressButton"),tmpPressButtonwav);
    mciSendString(mcicmdPressButton, NULL,0, NULL);

	srand((unsigned) time(NULL));
	m_Track.R_G_B[0] = 255;
	m_Track.R_G_B[1] = 255;
	m_Track.R_G_B[2] = 0;
	HIMC g_hIMC = NULL;//g_hIMC 用于恢复时使用  
	g_hIMC = ImmAssociateContext(hWnd, NULL);//handle 为要禁用的窗口句柄
	if(rand() % 2)
		PlaySound((LPCTSTR)IDR_NotGaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
	else
		PlaySound((LPCTSTR)IDR_TwoNotGaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
	// 加载背景位图
	hDBoomCase = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BoomCase));

	hGuide1 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GUIDE1));

	hGuide2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GUIDE2));

	hGuide3 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GUIDE3));

	hGuide4 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GUIDE4));

	hGuide5 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GUIDE5));

	hGuide6 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GUIDE6));

	hGuide7 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GUIDE7));

	hGuide8 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GUIDE8));

	hGuide9 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GUIDE9));

	hGuide10 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GUIDE10));

	hGuide11 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GUIDE11));

	hDSpringCase = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Spring));

	hDJumpCase = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_JumpCase));

	hY_Jump = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_YellowJump));

	hR_Jump = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_RedJump));

	m_hCase_Aid = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Case_Aid));

	m_hSCase_Aid = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_SCase_Aid));

	hC_Target = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_C_Target));

	m_hL_Wind = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_L_Wind));

	m_hR_Wind = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_R_Wind));

	m_hS_Mountain = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_S_Mountain));

	m_hS_Water = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_S_Water));

	m_hSbmp_Back1 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Sbmp_Back1));

	m_hS_Back2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_S_Back2));

	m_hS_Back3 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_S_Back3));

	m_hWater = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Water));

	m_hMountain = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Mountain));

	m_hSwitch = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Switch));

	m_hSticker = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Sticker));

	m_hY_Sticker = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Y_Sticker));

	m_hChooseBack = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_ChooseBack));

	m_hBack1 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Back1));

	m_hBack2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Back2));

	m_hBack3 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Back3));

	m_hStartBack1 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_StartBack1));

	sBack2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_StartBack2));

	m_hEndBack1 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_EndBack1));

	m_hEndBack2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_EndBack2));

	// 加载Hero位图
	m_hHeroBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO_RUN));

	m_hHeroLeftBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO_RUNLEFT));


	// 加载开始游戏按钮位图
	hSelectReturn = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_SelectReturn));

	m_hGameStartButtonBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_START));

	m_hGameDStartButtonBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Dstart));

	m_hGameGStartButtonBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GStart));

	m_hGameEasyButtonBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Easy));

	m_hGameNormalButtonBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Normal));

	m_hGameHardButtonBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Hard));

	m_hQuit = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Quit));

	m_hMainQuitGame = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_MainQuitGame));

	m_hQ_Button = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Q_Button));

	m_hHelpButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HelpButton));

	m_hRestart = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Restart));

	m_hGameBalanceButtonBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Balance));

	m_hGameRedWinButtonBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_RedWin));

	m_hGameYellowWinButtonBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_YellowWin));

	m_hBoomBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Boom));

	m_hWeapon0_big = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Weapon0_big));

	m_hWeapon1_big = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Weapon1_big));

	m_hWeapon2_big = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Weapon2_big));

	m_hWeapon3_big = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Weapon3_big));

	m_hFrontSightBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_FrontSight));

	m_hY_FrontSightBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Y_FrontSight));

	m_hRestart = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Restart));

	m_hFormHelp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_Form_Help));

	// 创建地形
	DefineHeight();

	// 创建英雄
	m_hero[0] = CreateHero(0, 0, 0, 0, NULL, 0, HERO_MAX_FRAME_NUM, 0, 0, 0);
	for(i_hero = 1; i_hero <= num_hero; i_hero++)
	{
		do
		{
			temp_createHero = 1 + rand() % (WNDWIDTH * 2 - HERO_SIZE_X - 2);
		}
		while(Terrianheight[temp_createHero].y > SeaLevel - 25);
		m_hero[i_hero] = CreateHero(temp_createHero, 0 - HERO_SIZE_Y, HERO_SIZE_X, HERO_SIZE_Y, m_hHeroBmp, 0, HERO_MAX_FRAME_NUM, 0, 100, 100);
		for(temp_hero_i = 1; temp_hero_i < i_hero && (m_hero[temp_hero_i].pos.x - m_hero[i_hero].pos.x <= - HERO_SIZE_X - 20 || m_hero[temp_hero_i].pos.x - m_hero[i_hero].pos.x >= HERO_SIZE_X + 20); temp_hero_i++);
		if(temp_hero_i < i_hero)
			i_hero--;
	}

	//创建武器_______z之前有定义num_weapon = 4
	m_Weapon[0].hBmp = m_hBoomBmp;
	m_Weapon[0].Power = 45;
	m_Weapon[0].Range_Radius = 40;
	m_Weapon[0].Radius = 7;
	m_Weapon[0].Vel = 30;
	m_Weapon[0].BigBmp = m_hWeapon0_big;

	m_Weapon[1].hBmp = m_hBoomBmp;
	m_Weapon[1].Power = 20;
	m_Weapon[1].Range_Radius = 100;
	m_Weapon[1].Radius = 7;
	m_Weapon[1].Vel = 20;
	m_Weapon[1].BigBmp = m_hWeapon1_big;

	m_Weapon[2].hBmp = m_hBoomBmp;
	m_Weapon[2].Power = 60;
	m_Weapon[2].Range_Radius = 40;
	m_Weapon[2].Radius = 7;
	m_Weapon[2].Vel = 45;
	m_Weapon[2].BigBmp = m_hWeapon2_big;

	m_Weapon[3].hBmp = m_hBoomBmp;
	m_Weapon[3].Power = 10;
	m_Weapon[3].Range_Radius = 160;
	m_Weapon[3].Radius = 7;
	m_Weapon[3].Vel = 10;
	m_Weapon[3].BigBmp = m_hWeapon3_big;

	for(i_AidCase = 0; i_AidCase <= num_AidCase; i_AidCase++)
	{
		m_AidCase[i_AidCase].exist = FALSE;
		m_AidCase[i_AidCase].hBmp = m_hCase_Aid;
		m_AidCase[i_AidCase].pos.x = 0;
		m_AidCase[i_AidCase].pos.x = 0;
		m_AidCase[i_AidCase].Power = 25;
	}

	for(i_TargetCase = 0; i_TargetCase <= num_TargetCase; i_TargetCase++)
	{
		m_TargetCase[i_TargetCase].exist = FALSE;
		m_TargetCase[i_TargetCase].hBmp = hC_Target;
		m_TargetCase[i_TargetCase].pos.x = 0;
		m_TargetCase[i_TargetCase].pos.x = 0;
	}
	// 创建游戏开始按钮i
	m_gameStartButton = CreateGameButton(WNDWIDTH / 2 - GAME_START_BUTTON_SIZE_X / 2, WNDHEIGHT / 2 - GAME_START_BUTTON_SIZE_Y / 2, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hGameStartButtonBmp, 0);
	m_gameDStartButton = CreateGameButton(WNDWIDTH / 2 - GAME_START_BUTTON_SIZE_X / 2, WNDHEIGHT / 2 - GAME_START_BUTTON_SIZE_Y * 2, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hGameDStartButtonBmp, 0);
	m_gameGStartButton = CreateGameButton(WNDWIDTH / 2 - GAME_START_BUTTON_SIZE_X / 2, WNDHEIGHT / 2 - GAME_START_BUTTON_SIZE_Y * 7 / 2, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hGameGStartButtonBmp, 0);
	m_gameEasyButton = CreateGameButton(WNDWIDTH / 2 - GAME_START_BUTTON_SIZE_X / 2, WNDHEIGHT / 2 - GAME_START_BUTTON_SIZE_Y * 2, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hGameEasyButtonBmp, 0);
	m_gameNormalButton = CreateGameButton(WNDWIDTH / 2 - GAME_START_BUTTON_SIZE_X / 2, WNDHEIGHT / 2 - GAME_START_BUTTON_SIZE_Y / 2, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hGameNormalButtonBmp, 0);
	m_gameHardButton = CreateGameButton(WNDWIDTH / 2 - GAME_START_BUTTON_SIZE_X / 2, WNDHEIGHT / 2 + GAME_START_BUTTON_SIZE_Y, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hGameHardButtonBmp, 0);
	m_gameBalanceButton = CreateGameButton(WNDWIDTH / 2 - GAME_START_BUTTON_SIZE_X / 2, WNDHEIGHT / 2 - GAME_START_BUTTON_SIZE_Y / 2, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hGameBalanceButtonBmp, 0);
	m_gameRedWinButton = CreateGameButton(WNDWIDTH / 2 - GAME_START_BUTTON_SIZE_X / 2, WNDHEIGHT / 2 - GAME_START_BUTTON_SIZE_Y / 2, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hGameRedWinButtonBmp, 0);
	m_gameYellowWinButton = CreateGameButton(WNDWIDTH / 2 - GAME_START_BUTTON_SIZE_X / 2, WNDHEIGHT / 2 - GAME_START_BUTTON_SIZE_Y / 2, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hGameYellowWinButtonBmp, 0);
	m_gameSelectReturnButton = CreateGameButton(WNDWIDTH / 2 - GAME_START_BUTTON_SIZE_X / 2, WNDHEIGHT / 2 + GAME_START_BUTTON_SIZE_Y * 5 / 2, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, hSelectReturn, 0);
	m_gameMainQuitGameButton = CreateGameButton(WNDWIDTH / 2 - GAME_START_BUTTON_SIZE_X / 2, WNDHEIGHT / 2 + GAME_START_BUTTON_SIZE_Y * 5 / 2, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hMainQuitGame, 0);
	m_gameQuitButton = CreateGameButton(WNDWIDTH / 2 - 100 - 70, WNDHEIGHT / 2 + 90 - 35, 140, 70, m_hQuit, 0);
	m_gameRestartButton = CreateGameButton(WNDWIDTH / 2 + 100 - 70, WNDHEIGHT / 2 + 90 - 35, 140, 70, m_hRestart, 0);
	m_gameReturnButton = CreateGameButton(1077, 601, 194, 61, NULL, 0);
	m_gameHelpButton = CreateGameButton(WNDWIDTH / 2 - 70, WNDHEIGHT / 2 + 90 - 35, 140, 70, m_hHelpButton, 0);

	//设定初始游戏状态
	gameStatus.status=0;
	if(rand() % 2)
		gameStatus.hBmp = m_hStartBack1;
	else
		gameStatus.hBmp = sBack2;
	Take_Weapon();
	CreateWindSpeed();
}

VOID Render(HWND hWnd)
{
	//判断游戏状态
	switch(gameStatus.status)
	{
	case 0:
		//绘制开始界面
		Render_Start(hWnd);
		break;
	case 1:
		//绘制游戏界面
		Render_Game(hWnd);
		break;
	case 2:
		//绘制结束界面
		Render_End(hWnd);
		break;
	case 3:
		//绘制结束界面_红胜
		Render_End_Red(hWnd);
		break;
	case 4:
		//绘制结束界面_黄胜
		Render_End_Yellow(hWnd);
		break;
	case 5:
		//绘制帮助界面
		Render_Help(hWnd);
		break;
	case 6:
		//绘制帮助界面
		Render_Choose_AI(hWnd);
		break;
	default:
		//绘制教程界面
		Render_Guide(hWnd);
		break;
	}
}

VOID Render_Start(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);
	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;
	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);
	SelectObject(hdcBmp, gameStatus.hBmp);
	BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
		hdcBmp, 0, 0, SRCCOPY);

	// 绘制开始按钮
	SelectObject(hdcBmp, m_gameStartButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameStartButton.pos.x, m_gameStartButton.pos.y, m_gameStartButton.size.cx, m_gameStartButton.size.cy,
		hdcBmp, 0, 0,
		m_gameStartButton.size.cx, m_gameStartButton.size.cy, RGB(255, 255, 255));
	SelectObject(hdcBmp, m_gameHelpButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameHelpButton.pos.x, m_gameHelpButton.pos.y, m_gameHelpButton.size.cx, m_gameHelpButton.size.cy,
		hdcBmp, 0, 0,
		m_gameHelpButton.size.cx, m_gameHelpButton.size.cy, RGB(255, 255, 255));
	SelectObject(hdcBmp, m_gameDStartButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameDStartButton.pos.x, m_gameDStartButton.pos.y, m_gameDStartButton.size.cx, m_gameDStartButton.size.cy,
		hdcBmp, 0, 0,
		m_gameDStartButton.size.cx, m_gameDStartButton.size.cy, RGB(255, 255, 255));
	SelectObject(hdcBmp, m_gameGStartButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameGStartButton.pos.x, m_gameGStartButton.pos.y, m_gameGStartButton.size.cx, m_gameGStartButton.size.cy,
		hdcBmp, 0, 0,
		m_gameGStartButton.size.cx, m_gameGStartButton.size.cy, RGB(255, 255, 255));
	SelectObject(hdcBmp, m_gameMainQuitGameButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameMainQuitGameButton.pos.x, m_gameMainQuitGameButton.pos.y, m_gameMainQuitGameButton.size.cx, m_gameMainQuitGameButton.size.cy,
		hdcBmp, 0, 0,
		m_gameMainQuitGameButton.size.cx, m_gameMainQuitGameButton.size.cy, RGB(255, 255, 255));

	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);
	EndPaint(hWnd, &ps);
}

VOID Render_Guide(HWND hWnd)
{
	if(!IF_Guide_ChangedWorld)
	{
		IF_Guide_ChangedWorld = TRUE;
		while(Terrianheight[m_hero[1].pos.x].y >= SeaLevel - 1 || m_hero[1].pos.x <= 120 || (m_hero[2].pos.x <= m_hero[1].pos.x + 120 && m_hero[2].pos.x >= m_hero[1].pos.x - 130) || (m_hero[3].pos.x <= m_hero[1].pos.x + 120 && m_hero[3].pos.x >= m_hero[1].pos.x - 130) || (m_hero[4].pos.x <= m_hero[1].pos.x + 120 && m_hero[4].pos.x >= m_hero[1].pos.x - 130) || (m_hero[5].pos.x <= m_hero[1].pos.x + 120 && m_hero[5].pos.x >= m_hero[1].pos.x - 130) || (m_hero[6].pos.x <= m_hero[1].pos.x + 120 && m_hero[6].pos.x >= m_hero[1].pos.x - 130) || (m_hero[7].pos.x <= m_hero[1].pos.x + 120 && m_hero[7].pos.x >= m_hero[1].pos.x - 130) || (m_hero[8].pos.x <= m_hero[1].pos.x + 120 && m_hero[8].pos.x >= m_hero[1].pos.x - 130))
		{
			DefineHeight();
			for(i_hero = 1; i_hero <= num_hero; i_hero++)
			{
				do
				{
					temp_createHero = 1 + rand() % (WNDWIDTH * 2 - HERO_SIZE_X - 2);
				}
				while(Terrianheight[temp_createHero].y > SeaLevel - 25);
				m_hero[i_hero] = CreateHero(temp_createHero, 0 - HERO_SIZE_Y, HERO_SIZE_X, HERO_SIZE_Y, m_hHeroBmp, 0, HERO_MAX_FRAME_NUM, 0, 100, 100);
				for(temp_hero_i = 1; temp_hero_i < i_hero && (m_hero[temp_hero_i].pos.x - m_hero[i_hero].pos.x <= - HERO_SIZE_X - 20 || m_hero[temp_hero_i].pos.x - m_hero[i_hero].pos.x >= HERO_SIZE_X + 20); temp_hero_i++);
				if(temp_hero_i < i_hero)
					i_hero--;
			}
		}
		for(temp_i = -120; temp_i < 120; temp_i++)
			Terrianheight[m_hero[1].pos.x - temp_i].y = Terrianheight[m_hero[1].pos.x].y;
	}
	if(wait == 0 && HeroShout)
		HeroShoutSelect();
	if(GuideModeStatus != 0 && GuideModeStatus != 2)
	{
		if(Now_Screen_Pos == -1)
		{
			Controlable = FALSE;
			Pre_Screen_Pos = -5;
			Now_Screen_Pos = 0;
		}
		else if(Pre_Screen_Pos == -5)
		{
			Now_Screen_Pos += 6;
			if(Now_Screen_Pos >= WNDWIDTH)
			{
				Now_Screen_Pos = WNDWIDTH;
				Pre_Screen_Pos = -10;
			}
		}
		else if(Pre_Screen_Pos == -10)
		{
			Now_Screen_Pos -= 8;
			if(Now_Screen_Pos <= 0)
			{
				Now_Screen_Pos = 0;
				Pre_Screen_Pos = 0;
				Controlable = TRUE;
			}
		}
		else
		{
			Pre_Screen_Pos = Now_Screen_Pos;
			if(Status_OF_Game <= 2)
				Now_Screen_Pos = m_hero[now_hero].pos.x + HERO_SIZE_X / 2 - WNDWIDTH / 2;
			else
				Now_Screen_Pos = m_hero[0].pos.x - WNDWIDTH / 2;
			if(Now_Screen_Pos < 0)
				Now_Screen_Pos = 0;
			else if(Now_Screen_Pos > WNDWIDTH)
				Now_Screen_Pos = WNDWIDTH;
			if(abs(Pre_Screen_Pos - Now_Screen_Pos) > 10 && (Status_OF_Game == 4 || Status_OF_Game == 0))
			{
				if(wait <= 0)
					wait = 2;
				if(Pre_Screen_Pos > Now_Screen_Pos)
					Now_Screen_Pos = Pre_Screen_Pos - 10;
				else
					Now_Screen_Pos = Pre_Screen_Pos + 10;
			}
			if(GuideModeStatus == 1 && Pre_Screen_Pos == Now_Screen_Pos)
				GuideModeStatus = 2;
		}
	}

	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);
	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;
	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH * 2, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);
	SelectObject(hdcBmp, gameStatus.hBmp);
	BitBlt(hdcBuffer, 0, 0, WNDWIDTH * 2, WNDHEIGHT, hdcBmp, 0, 0, SRCCOPY);

	//画小地图
	hPen = CreatePen(PS_SOLID, 2, RGB(129, 72, 3));
	hBrush = CreateSolidBrush(NULL_BRUSH);
	SelectObject(hdcBuffer, hPen);		//必须要select到hdcBuffer！！！
	SelectObject(hdcBuffer, hBrush);
	Rectangle(hdcBuffer, SmallMapPos.x - 1 + Now_Screen_Pos, SmallMapPos.y - 2, SmallMapPos.x + 2 + SmallMapSize.x + Now_Screen_Pos, SmallMapPos.y + 2 + SmallMapSize.y);
	DeleteObject(hPen);
	DeleteObject(hBrush);
	SelectObject(hdcBuffer, cptBmp);
	SelectObject(hdcBmp, gameStatus.hS_Bmp);
	BitBlt(hdcBuffer, SmallMapPos.x + Now_Screen_Pos, SmallMapPos.y, SmallMapSize.x, SmallMapSize.y, hdcBmp, 0, 0, SRCCOPY);
	
	//绘制水面
	SelectObject(hdcBmp, m_hS_Water);
	TransparentBlt(hdcBuffer, SmallMapPos.x + Now_Screen_Pos, SmallMapPos.y + SeaLevel / Times_Of_Map, SmallMapSize.x, SmallMapSize.y - SeaLevel / Times_Of_Map, hdcBmp, 0, 0, SmallMapSize.x, SmallMapSize.y - SeaLevel / Times_Of_Map, RGB(255, 255, 255));
	//绘制水面结束

	//绘制山
	SelectObject(hdcBmp, m_hS_Mountain);
	for(temp_new_try = 1; temp_new_try <= WNDWIDTH * 2 + 1; temp_new_try += Times_Of_Map)
	{
		TransparentBlt(hdcBuffer, Terrianheight[temp_new_try].x / Times_Of_Map + SmallMapPos.x + Now_Screen_Pos, Terrianheight[temp_new_try].y / Times_Of_Map + SmallMapPos.y, 1, (WNDHEIGHT - Terrianheight[temp_new_try].y) / Times_Of_Map, hdcBmp, Terrianheight[temp_new_try].x / Times_Of_Map, Terrianheight[temp_new_try].y / Times_Of_Map, 1, (WNDHEIGHT - Terrianheight[temp_new_try].y) / Times_Of_Map, RGB(255, 255, 255));
	}
	//绘制山结束

	//绘制虫子位置
	for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
	{
		if(!m_hero[i_hero].NowBleed)
			continue;
		if(i_hero <= num_hero / 2)
			hPen=CreatePen(PS_SOLID,5,RGB(255, 0, 0));
		else
			hPen=CreatePen(PS_SOLID,5,RGB(255, 255, 0));
		SelectObject(hdcBuffer, hPen);
		temp_point[0].x = temp_point[1].x = m_hero[i_hero].pos.x / Times_Of_Map + 3 + SmallMapPos.x + Now_Screen_Pos;
		temp_point[0].y = temp_point[1].y = m_hero[i_hero].pos.y / Times_Of_Map + SmallMapPos.y;
		Polyline(hdcBuffer, temp_point, 2);
		DeleteObject(hPen);
	}
	//绘制虫子位置结束

	//绘制框
	hPen = CreatePen(PS_SOLID, 2, RGB(129, 72, 3));
	temp_point[0].x = SmallMapPos.x + Now_Screen_Pos + Now_Screen_Pos / Times_Of_Map;
	temp_point[0].y = SmallMapPos.y - 5;
	temp_point[1].x = SmallMapPos.x + Now_Screen_Pos + Now_Screen_Pos / Times_Of_Map;
	temp_point[1].y = SmallMapPos.y + 5 + SmallMapSize.y;
	temp_point[2].x = SmallMapPos.x + Now_Screen_Pos + (Now_Screen_Pos + WNDWIDTH) / Times_Of_Map;
	temp_point[2].y = SmallMapPos.y + 5 + SmallMapSize.y;
	temp_point[3].x = SmallMapPos.x + Now_Screen_Pos + (Now_Screen_Pos + WNDWIDTH) / Times_Of_Map;
	temp_point[3].y = SmallMapPos.y - 5;
	temp_point[4].x = SmallMapPos.x + Now_Screen_Pos + Now_Screen_Pos / Times_Of_Map;
	temp_point[4].y = SmallMapPos.y - 5;
	SelectObject(hdcBuffer, hPen);
	Polyline(hdcBuffer, temp_point, 5);
	DeleteObject(hPen);
	//绘制框结束
	//画小地图结束

	//绘制风速指示图标
	for(temp_i = 1; temp_i <= WindSpeed; temp_i++)
	{
		SelectObject(hdcBmp, m_hR_Wind);
		TransparentBlt(hdcBuffer, WindPos.x + Now_Screen_Pos + temp_i * 29, WindPos.y, 29, 30, hdcBmp, 0, 0, 29, 30, RGB(255, 255, 255));
	}
	for(temp_i = -1; temp_i >= WindSpeed; temp_i--)
	{
		SelectObject(hdcBmp, m_hL_Wind);
		TransparentBlt(hdcBuffer, WindPos.x + Now_Screen_Pos - temp_i * 29, WindPos.y, 29, 30, hdcBmp, 0, 0, 29, 30, RGB(255, 255, 255));
	}
	//绘制风速指示图标结束

	//绘制风速旁边的框
	hPen = CreatePen(PS_SOLID, 2, RGB(181, 230, 29));
	temp_point[0].x = WindPos.x + Now_Screen_Pos + 29 - 10;
	temp_point[0].y = WindPos.y - 10;
	temp_point[1].x = WindPos.x + Now_Screen_Pos + 29 - 10;
	temp_point[1].y = WindPos.y + 40;
	temp_point[2].x = WindPos.x + Now_Screen_Pos + 29 * 5 + 10;
	temp_point[2].y = WindPos.y + 40;
	temp_point[3].x = WindPos.x + Now_Screen_Pos + 29 * 5 + 10;
	temp_point[3].y = WindPos.y - 10;
	temp_point[4].x = WindPos.x + Now_Screen_Pos + 29 - 10;
	temp_point[4].y = WindPos.y - 10;
	SelectObject(hdcBuffer, hPen);
	Polyline(hdcBuffer, temp_point, 5);
	DeleteObject(hPen);
	//绘制风速旁边的框结束

	// 绘制分数
	SetBkMode(hdcBuffer, TRANSPARENT);
	SetTextColor(hdcBuffer, RGB(255, 0, 0));
	wsprintf(szDist, _T("%d"), num_alive[1]);
	TextOut(hdcBuffer, WNDWIDTH / 2 + Now_Screen_Pos, 85, szDist, _tcslen(szDist));
	SetTextColor(hdcBuffer, RGB(255, 255, 0));
	wsprintf(szDist, _T("%d"), num_alive[2]);
	TextOut(hdcBuffer, WNDWIDTH / 2 + 30 + Now_Screen_Pos, 85, szDist, _tcslen(szDist));
	//绘制分数结束

	//绘制血量条
	hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	hBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdcBuffer, hPen);
	SelectObject(hdcBuffer, hBrush);
	temp_bloodbar = 0;
	for(i_hero = 1; i_hero <= num_hero / 2; i_hero++)
	{
		Rectangle(hdcBuffer, BloodBarPosX + Now_Screen_Pos + temp_bloodbar, BloodBarPosY, BloodBarPosX + Now_Screen_Pos + temp_bloodbar + (BloodBarSizeX * m_hero[i_hero].NowBleed + m_hero[i_hero].AllBleed - 1) / m_hero[i_hero].AllBleed, BloodBarSizeY + BloodBarPosY);
		temp_bloodbar += 10 + BloodBarSizeX * m_hero[i_hero].NowBleed / m_hero[i_hero].AllBleed;
	}
	DeleteObject(hPen);
	DeleteObject(hBrush);

	hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	hBrush = CreateSolidBrush(RGB(255, 255, 0));
	SelectObject(hdcBuffer, hPen);
	SelectObject(hdcBuffer, hBrush);
	temp_bloodbar = 0;
	for(; i_hero <= num_hero; i_hero++)
	{
		Rectangle(hdcBuffer, BloodBarPosX + Now_Screen_Pos + temp_bloodbar, BloodBarPosY + BloodBarSizeY + BloodBarPosPlus, BloodBarPosX + Now_Screen_Pos + temp_bloodbar + (BloodBarSizeX * m_hero[i_hero].NowBleed + m_hero[i_hero].AllBleed - 1) / m_hero[i_hero].AllBleed, BloodBarSizeY + BloodBarPosY + BloodBarSizeY + BloodBarPosPlus);
		temp_bloodbar += 10 + BloodBarSizeX * m_hero[i_hero].NowBleed / m_hero[i_hero].AllBleed;
	}
	DeleteObject(hPen);
	DeleteObject(hBrush);
	//绘制血量条结束

	//绘制技能点
	if(ShowTrack[0])
	{
		SelectObject(hdcBmp, m_hFrontSightBmp);
		TransparentBlt(hdcBuffer, BloodBarPosX + Now_Screen_Pos - 40, BloodBarPosY, 15, 15, hdcBmp, 0, 0, 28, 28, RGB(255, 255, 255));
	}
	if(ShowTrack[1])
	{
		SelectObject(hdcBmp, m_hY_FrontSightBmp);
		TransparentBlt(hdcBuffer, BloodBarPosX + Now_Screen_Pos - 40, BloodBarPosY + BloodBarSizeY + BloodBarPosPlus, 15, 15, hdcBmp, 0, 0, 28, 28, RGB(255, 255, 255));
	}
	if(JumpHigher[0])
	{
		SelectObject(hdcBmp, hR_Jump);
		TransparentBlt(hdcBuffer, BloodBarPosX + Now_Screen_Pos - 28, BloodBarPosY, 15, 15, hdcBmp, 0, 0, 28, 28, RGB(255, 255, 255));
	}
	if(JumpHigher[1])
	{
		SelectObject(hdcBmp, hY_Jump);
		TransparentBlt(hdcBuffer, BloodBarPosX + Now_Screen_Pos - 28, BloodBarPosY + BloodBarSizeY + BloodBarPosPlus, 15, 15, hdcBmp, 0, 0, 28, 28, RGB(255, 255, 255));
	}
	//绘制技能点结束

	//绘制水面
	SelectObject(hdcBmp, m_hWater);
	TransparentBlt(hdcBuffer, 0, SeaLevel, WNDWIDTH * 2, WNDHEIGHT - SeaLevel, hdcBmp, 0, 0, WNDWIDTH * 2, WNDHEIGHT - SeaLevel, RGB(255, 255, 255));
	//绘制水面结束

	//绘制山
	SelectObject(hdcBmp, m_hMountain);
	for(temp_new_try = 1; temp_new_try <= WNDWIDTH * 2 + 1; temp_new_try++)
	{
		TransparentBlt(hdcBuffer, Terrianheight[temp_new_try].x, Terrianheight[temp_new_try].y, 1, WNDHEIGHT - Terrianheight[temp_new_try].y, hdcBmp, Terrianheight[temp_new_try].x, Terrianheight[temp_new_try].y, 1, WNDHEIGHT - Terrianheight[temp_new_try].y, RGB(255, 255, 255));
	}
	//绘制山结束


	// 绘制Hero到缓存
	for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
	{
		if(!m_hero[i_hero].NowBleed)
			continue;
		SelectObject(hdcBmp, m_hero[i_hero].hBmp);
		TransparentBlt(hdcBuffer, m_hero[i_hero].pos.x, m_hero[i_hero].pos.y, m_hero[i_hero].size.cx, m_hero[i_hero].size.cy, hdcBmp, 0, m_hero[i_hero].curFrameIndex * m_hero[i_hero].size.cy, m_hero[i_hero].size.cx, m_hero[i_hero].size.cy, RGB(255, 255, 255));
	}
	//绘制Hero结束

	//绘制虫子手上拿的武器
	if(Status_OF_Game < 3)
	{
		m_hero[0].pos.x = m_hero[now_hero].pos.x + HERO_SIZE_X / 2;
		m_hero[0].pos.y = m_hero[now_hero].pos.y + HERO_SIZE_Y * 3 / 4;
	}
	//绘制虫子手上拿的武器结束

	if(Status_OF_Game <= 4)
	{
		SelectObject(hdcBmp, m_hero[0].hBmp);
		TransparentBlt(
			hdcBuffer, m_hero[0].pos.x - m_hero[0].size.cx / 2, m_hero[0].pos.y - m_hero[0].size.cy / 2,
			m_hero[0].size.cx, m_hero[0].size.cy,
			hdcBmp, 0, 0, m_hero[0].size.cx, m_hero[0].size.cy,
			RGB(255, 255, 255)
		);
		SelectObject(hdcBmp, m_Weapon[now_Weapon].BigBmp);
		TransparentBlt(hdcBuffer, 30 + Now_Screen_Pos, 30, Weapon_Big_Size, Weapon_Big_Size, hdcBmp, 0, 0, Weapon_Big_Size, Weapon_Big_Size, RGB(255, 255, 255));
		SelectObject(hdcBmp, m_hChooseBack);
		TransparentBlt(hdcBuffer, WNDWIDTH - ChooseBackWidth - 30 + Now_Screen_Pos, 30, ChooseBackWidth, ChooseBackHeight, hdcBmp, 0, 0, ChooseBackWidth, ChooseBackHeight, RGB(255, 255, 255));
		SelectObject(hdcBmp, m_hQ_Button);
		TransparentBlt(hdcBuffer, WNDWIDTH - ChooseBackWidth - 110 + Now_Screen_Pos, 30, ChooseBackWidth, ChooseBackHeight, hdcBmp, 0, 0, 72, 72, RGB(255, 255, 255));
	}

	//绘制血槽和血量
	for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
	{
		if(No_Hero_Dec[i_hero][0] > 0)
		{
			No_Hero_Dec[i_hero][0]--;
			if(i_hero <= num_hero / 2)
				SetTextColor(hdcBuffer, RGB(255, 0, 0));
			else
				SetTextColor(hdcBuffer, RGB(255, 255, 0));
			SetBkMode(hdcBuffer, TRANSPARENT);
			if(No_Hero_Dec[i_hero][1] >= 0)
				wsprintf(BloodDecNum, L"+%d", No_Hero_Dec[i_hero][1]);
			else
				wsprintf(BloodDecNum, L"%d", No_Hero_Dec[i_hero][1]);
			if(No_Hero_Dec[i_hero][1] < -99)
				TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 19, m_hero[i_hero].pos.y - 65, BloodDecNum, _tcslen(BloodDecNum));
			else if(No_Hero_Dec[i_hero][1] < -9 || No_Hero_Dec[i_hero][1] > 0)
				TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 14, m_hero[i_hero].pos.y - 65, BloodDecNum, _tcslen(BloodDecNum));
			else
				TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 9, m_hero[i_hero].pos.y - 65, BloodDecNum, _tcslen(BloodDecNum));
		}
		if(!m_hero[i_hero].NowBleed)
		{
			if(No_Hero_Dead[i_hero])
			{
				No_Hero_Dead[i_hero]--;
				SetTextColor(hdcBuffer, RGB(255, 0, 0));
				SetBkMode(hdcBuffer, TRANSPARENT);
				wsprintf(DeadText, L"Dead");
				TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 14, m_hero[i_hero].pos.y, DeadText, _tcslen(L"Dead"));
			}
			continue;
		}
		if(m_hero[i_hero].NowBleed >= 100)
		{
			R_blood = 0;
			G_blood = 255;
		}
		else if(m_hero[i_hero].NowBleed >= 50)
		{
			R_blood = 255 * (100 - m_hero[i_hero].NowBleed) / 50;
			G_blood = 255;
		}
		else
		{
			R_blood = 255;
			G_blood = 255 * m_hero[i_hero].NowBleed / 50;
		}
		hBrush=CreateSolidBrush(RGB(R_blood, G_blood, 0));
		SelectObject(hdcBuffer, hBrush);
		hPen=CreatePen(PS_SOLID,2,RGB(R_blood, G_blood, 0));
		SelectObject(hdcBuffer, hPen);
		RoundRect(hdcBuffer, m_hero[i_hero].pos.x, m_hero[i_hero].pos.y - 15, m_hero[i_hero].pos.x + 2 + (HERO_SIZE_X - 2) * m_hero[i_hero].NowBleed / m_hero[i_hero].AllBleed, m_hero[i_hero].pos.y - 5, 4, 3);
		DeleteObject(hPen);
		DeleteObject(hBrush);
	//绘制虫子号
		if(i_hero <= num_hero / 2)
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
		else
			SetTextColor(hdcBuffer, RGB(255, 255, 0));
		SetBkMode(hdcBuffer, TRANSPARENT);
		wsprintf(HeroNumText, L"%d", i_hero);
		if(i_hero > 9)
			TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 9, m_hero[i_hero].pos.y - 33, HeroNumText, _tcslen(HeroNumText));
		else
			TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 4, m_hero[i_hero].pos.y - 33, HeroNumText, _tcslen(HeroNumText));
	//绘制血量数字
		wsprintf(HeroNumText, L"%d", m_hero[i_hero].NowBleed);
		if(m_hero[i_hero].NowBleed > 99)
			TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 14, m_hero[i_hero].pos.y - 53, HeroNumText, _tcslen(HeroNumText));
		else if(m_hero[i_hero].NowBleed > 9)
			TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 9, m_hero[i_hero].pos.y - 53, HeroNumText, _tcslen(HeroNumText));
		else
			TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 4, m_hero[i_hero].pos.y - 53, HeroNumText, _tcslen(HeroNumText));
	}
	//绘制血量数字结束

	//绘制医疗箱
	for(i_AidCase = 0; i_AidCase < num_AidCase; i_AidCase++)
	{
		if(m_AidCase[i_AidCase].exist)
		{
			SelectObject(hdcBmp, m_AidCase[i_AidCase].hBmp);
			TransparentBlt(hdcBuffer, m_AidCase[i_AidCase].pos.x, m_AidCase[i_AidCase].pos.y, AidCaseSize_X, AidCaseSize_Y, hdcBmp, 0, 0, AidCaseSize_X, AidCaseSize_Y, RGB(255, 255, 255));
		}
	}
	//绘制医疗箱结束

	//绘制瞄准箱
	for(i_TargetCase = 0; i_TargetCase < num_TargetCase; i_TargetCase++)
	{
		if(m_TargetCase[i_TargetCase].exist && m_TargetCase[i_TargetCase].type != DSpringCase)
		{
			SelectObject(hdcBmp, m_TargetCase[i_TargetCase].hBmp);
			TransparentBlt(hdcBuffer, m_TargetCase[i_TargetCase].pos.x, m_TargetCase[i_TargetCase].pos.y, TargetCaseSize_X, TargetCaseSize_Y, hdcBmp, 0, 0, TargetCaseSize_X, TargetCaseSize_Y, RGB(255, 255, 255));
		}
		else if(m_TargetCase[i_TargetCase].exist)
		{
			SelectObject(hdcBmp, m_TargetCase[i_TargetCase].hBmp);
			TransparentBlt(hdcBuffer, m_TargetCase[i_TargetCase].pos.x, m_TargetCase[i_TargetCase].pos.y, TargetCaseSize_X, TargetCaseSize_Y, hdcBmp, 0, 0, 120, 129, RGB(255, 255, 255));
		}
	}
	//绘制瞄准箱结束

	if(wait <= 0)
	{
	//绘制指示哪一只虫子（线）
	if(now_hero <= num_hero / 2)
		hPen=CreatePen(PS_SOLID,2,RGB(255, 0, 0));
	else
		hPen=CreatePen(PS_SOLID,2,RGB(255, 255, 0));
	SelectObject(hdcBuffer, hPen);
	temp_point[0].x = m_hero[now_hero].pos.x;
	temp_point[0].y = m_hero[now_hero].pos.y - 35;
	temp_point[1].x = m_hero[now_hero].pos.x + HERO_SIZE_X;
	temp_point[1].y = m_hero[now_hero].pos.y - 35;
	Polyline(hdcBuffer, temp_point, 2);
	DeleteObject(hPen);
	//绘制指示哪一只虫子（线）结束

	//绘制指示哪一只虫子（箭头）
	if(now_hero <= num_hero / 2)
		SelectObject(hdcBmp, m_hSticker);
	else
		SelectObject(hdcBmp, m_hY_Sticker);
	TransparentBlt(hdcBuffer, m_hero[now_hero].pos.x + HERO_SIZE_X / 2 - 15, m_hero[now_hero].pos.y - 90, 29, 30, hdcBmp, 0, 0, 29, 30, RGB(255, 255, 255));
	//绘制指示哪一只虫子（箭头）结束
	}

	if(Status_OF_Game == 0)
	{
		for(temp_i = 0; temp_i <= num_hero; temp_i++)
			power_add[temp_i] = 0;
		Degree_Of_power = 30;
		if(m_hero[now_hero].position) //向左
			temp_x = int(m_hero[now_hero].pos.x + HERO_SIZE_X / 2 - temp_R_firing * cos(Degree_Of_Firing * Pi / 180));
		else
			temp_x = int(m_hero[now_hero].pos.x + HERO_SIZE_X / 2 + temp_R_firing * cos(Degree_Of_Firing * Pi / 180));
		temp_y = int(m_hero[now_hero].pos.y + HERO_SIZE_Y / 4 - temp_R_firing * sin(Degree_Of_Firing * Pi / 180));
		if(temp_x < 1)
		{
			temp_y = (temp_y - (m_hero[now_hero].pos.y + HERO_SIZE_Y / 4)) * (1 - (m_hero[now_hero].pos.x + HERO_SIZE_X / 2)) / (temp_x - (m_hero[now_hero].pos.x + HERO_SIZE_X / 2)) + m_hero[now_hero].pos.y + HERO_SIZE_Y / 4;
			temp_x = 1;
		}
		else if(temp_x >= WNDWIDTH * 2)
		{
			temp_y = (temp_y - (m_hero[now_hero].pos.y + HERO_SIZE_Y / 4)) * (WNDWIDTH * 2 - 1 - (m_hero[now_hero].pos.x + HERO_SIZE_X / 2)) / (temp_x - (m_hero[now_hero].pos.x + HERO_SIZE_X / 2)) + m_hero[now_hero].pos.y + HERO_SIZE_Y / 4;
			temp_x = WNDWIDTH * 2 - 1;
		}
		if(now_hero <= num_hero / 2)
			SelectObject(hdcBmp, m_hFrontSightBmp);
		else
			SelectObject(hdcBmp, m_hY_FrontSightBmp);
		TransparentBlt(hdcBuffer, temp_x - 14, temp_y - 14, 28, 28, hdcBmp, 0, 0, 28, 28, RGB(255, 255, 255));
	}
	else if(Status_OF_Game == 1)
	{
		if(m_hero[now_hero].position) //向左
			temp_point[0].x = int(m_hero[now_hero].pos.x + HERO_SIZE_X / 2 - temp_r_power * Degree_Of_power * cos(Degree_Of_Firing * Pi / 180));
		else
			temp_point[0].x = int(m_hero[now_hero].pos.x + HERO_SIZE_X / 2 + temp_r_power * Degree_Of_power * cos(Degree_Of_Firing * Pi / 180));
		temp_point[0].y = int(m_hero[now_hero].pos.y + HERO_SIZE_Y / 4 - temp_r_power * Degree_Of_power * sin(Degree_Of_Firing * Pi / 180));
		temp_point[1].x = m_hero[now_hero].pos.x + HERO_SIZE_X / 2;
		temp_point[1].y = m_hero[now_hero].pos.y + HERO_SIZE_Y / 4;
		Polyline(hdcBuffer, temp_point, 2);
		
		//画出轨迹
		if((ShowTrack[0] && now_hero <= num_hero / 2) || (ShowTrack[1] && now_hero > num_hero / 2))
		{
			if(now_hero <= num_hero / 2)
				m_Track.R_G_B[1] = 0;
			else
				m_Track.R_G_B[1] = 255;
			GetTrack(TRUE, Degree_Of_Firing, Degree_Of_power, m_hero[now_hero].pos.x + HERO_SIZE_X / 2, m_hero[now_hero].pos.y + HERO_SIZE_Y / 4, m_hero[now_hero].position, m_Weapon[now_Weapon].Vel);
			hPen = CreatePen(PS_SOLID, 2, RGB(m_Track.R_G_B[0], m_Track.R_G_B[1], m_Track.R_G_B[2]));
			SelectObject(hdcBuffer, hPen);
			Polyline(hdcBuffer, m_Track.Points, m_Track.Lenth);
			DeleteObject(hPen);
			hPen=CreatePen(PS_SOLID, 5 ,RGB(255, 255, 255));
			SelectObject(hdcBuffer, hPen);
			temp_point[0].x = temp_point[1].x = m_Track.Destination.x / Times_Of_Map + 3 + SmallMapPos.x + Now_Screen_Pos;
			temp_point[0].y = temp_point[1].y = m_Track.Destination.y / Times_Of_Map + SmallMapPos.y;
			Polyline(hdcBuffer, temp_point, 2);
			DeleteObject(hPen);
		}
		//画轨迹结束
	}
	else if(Status_OF_Game == 2)
	{
		Status_OF_Game = 3;
		Hit(Degree_Of_Firing, Degree_Of_power, m_hero[now_hero].pos.x + HERO_SIZE_X / 2, m_hero[now_hero].pos.y + HERO_SIZE_Y / 4, m_hero[now_hero].position, m_Weapon[now_Weapon].Vel);
		Degree_Of_Firing = 0;
		Degree_Of_power = 30;
	}
	else if(Status_OF_Game == 3)
	{
		m_hero[now_hero].vel.x = 0;
		if(Hitting())
			Status_OF_Game = 4;
	}
	else if(Status_OF_Game == 4)
	{
		CreateWindSpeed();
		if(Now_Hero_Change())
			Status_OF_Game = 0;
	}
	if(GuideModeStatus == 0)
	{
		SelectObject(hdcBmp, hGuide1);
		TransparentBlt(hdcBuffer, Now_Screen_Pos, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, WNDWIDTH, WNDHEIGHT, RGB(255, 255, 255));
	}
	else if(GuideModeStatus == 1)
	{
		SelectObject(hdcBmp, hGuide2);
		TransparentBlt(hdcBuffer, Now_Screen_Pos, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, WNDWIDTH, WNDHEIGHT, RGB(255, 255, 255));
		temp_i_GUIDE = -1;
	}
	else if(GuideModeStatus == 2)
	{
		SelectObject(hdcBmp, hGuide3);
		TransparentBlt(hdcBuffer, Now_Screen_Pos, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, WNDWIDTH, WNDHEIGHT, RGB(255, 255, 255));
	}
	else if(GuideModeStatus == 3)
	{
		m_hero[now_hero].vel.x = 0;
		if_pressing_ad[now_hero] = FALSE;
		SelectObject(hdcBmp, hGuide4);
		TransparentBlt(hdcBuffer, Now_Screen_Pos, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, WNDWIDTH, WNDHEIGHT, RGB(255, 255, 255));
	}
	else if(GuideModeStatus == 4)
	{
		SelectObject(hdcBmp, hGuide5);
		TransparentBlt(hdcBuffer, Now_Screen_Pos, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, WNDWIDTH, WNDHEIGHT, RGB(255, 255, 255));
	}
	else if(GuideModeStatus == 5)
	{
		SelectObject(hdcBmp, hGuide6);
		TransparentBlt(hdcBuffer, Now_Screen_Pos, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, WNDWIDTH, WNDHEIGHT, RGB(255, 255, 255));
	}
	else if(GuideModeStatus == 6)
	{
		SelectObject(hdcBmp, hGuide7);
		TransparentBlt(hdcBuffer, Now_Screen_Pos, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, WNDWIDTH, WNDHEIGHT, RGB(255, 255, 255));
		if(m_AidCase[2].exist == FALSE)
		{
			GuideModeStatus = 7;
			m_hero[now_hero].vel.x = 0;
			if_pressing_ad[now_hero] = 0;
			m_TargetCase[1].exist = TRUE;
			m_TargetCase[1].pos.x = m_hero[1].pos.x - 24;
			m_TargetCase[1].pos.y = m_hero[1].pos.y + HERO_SIZE_Y - AidCaseSize_Y - 5;
			m_TargetCase[1].type = DBoomCase;
			m_TargetCase[1].hBmp = hDBoomCase;
			m_TargetCase[2].exist = TRUE;
			m_TargetCase[2].pos.x = m_hero[1].pos.x + HERO_SIZE_X * 5 / 4 + 5;
			m_TargetCase[2].pos.y = m_hero[1].pos.y + HERO_SIZE_Y - AidCaseSize_Y - 5;
			m_TargetCase[2].type = DSpringCase;
			m_TargetCase[2].hBmp = hDSpringCase;
			m_TargetCase[3].exist = TRUE;
			m_TargetCase[3].pos.x = m_hero[1].pos.x + HERO_SIZE_X * 5 / 4 + 30;
			m_TargetCase[3].pos.y = m_hero[1].pos.y + HERO_SIZE_Y - AidCaseSize_Y - 5;
			m_TargetCase[3].type = DFrontSightCase;
			m_TargetCase[3].hBmp = hC_Target;
			m_TargetCase[4].exist = TRUE;
			m_TargetCase[4].pos.x = m_hero[1].pos.x + HERO_SIZE_X * 5 / 4 + 55;
			m_TargetCase[4].pos.y = m_hero[1].pos.y + HERO_SIZE_Y - AidCaseSize_Y - 5;
			m_TargetCase[4].type = DJumpCase;
			m_TargetCase[4].hBmp = hDJumpCase;
		}
	}
	else if(GuideModeStatus == 7)
	{
		SelectObject(hdcBmp, hGuide8);
		TransparentBlt(hdcBuffer, Now_Screen_Pos, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, WNDWIDTH, WNDHEIGHT, RGB(255, 255, 255));
		if(m_TargetCase[4].exist == FALSE)
		{
			GuideModeStatus = 8;
			m_hero[now_hero].vel.x = 0;
			if_pressing_ad[now_hero] = 0;
		}
	}
	else if(GuideModeStatus == 8)
	{
		SelectObject(hdcBmp, hGuide9);
		TransparentBlt(hdcBuffer, Now_Screen_Pos, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, WNDWIDTH, WNDHEIGHT, RGB(255, 255, 255));
	}
	else if(GuideModeStatus == 9)
	{
		SelectObject(hdcBmp, hGuide10);
		TransparentBlt(hdcBuffer, Now_Screen_Pos, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, WNDWIDTH, WNDHEIGHT, RGB(255, 255, 255));
	}
	else if(GuideModeStatus == 10)
	{
		SelectObject(hdcBmp, hGuide11);
		TransparentBlt(hdcBuffer, Now_Screen_Pos, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, WNDWIDTH, WNDHEIGHT, RGB(255, 255, 255));
	}

	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, Now_Screen_Pos, 0, SRCCOPY);
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);
	EndPaint(hWnd, &ps);
}

VOID Render_Choose_AI(HWND hWnd)	//选择难度界面
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);
	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;
	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);
	SelectObject(hdcBmp, gameStatus.hBmp);
	BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
		hdcBmp, 0, 0, SRCCOPY);

	// 绘制开始按钮
	SelectObject(hdcBmp, m_gameEasyButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameEasyButton.pos.x, m_gameEasyButton.pos.y, m_gameEasyButton.size.cx, m_gameEasyButton.size.cy,
		hdcBmp, 0, 0,
		m_gameEasyButton.size.cx, m_gameEasyButton.size.cy, RGB(255, 255, 255));
	SelectObject(hdcBmp, m_gameNormalButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameNormalButton.pos.x, m_gameNormalButton.pos.y, m_gameNormalButton.size.cx, m_gameNormalButton.size.cy,
		hdcBmp, 0, 0,
		m_gameNormalButton.size.cx, m_gameNormalButton.size.cy, RGB(255, 255, 255));
	SelectObject(hdcBmp, m_gameHardButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameHardButton.pos.x, m_gameHardButton.pos.y, m_gameHardButton.size.cx, m_gameHardButton.size.cy,
		hdcBmp, 0, 0,
		m_gameHardButton.size.cx, m_gameHardButton.size.cy, RGB(255, 255, 255));
	SelectObject(hdcBmp, m_gameSelectReturnButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameSelectReturnButton.pos.x, m_gameSelectReturnButton.pos.y, m_gameSelectReturnButton.size.cx, m_gameSelectReturnButton.size.cy,
		hdcBmp, 0, 0,
		m_gameSelectReturnButton.size.cx, m_gameSelectReturnButton.size.cy, RGB(255, 255, 255));
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);
	EndPaint(hWnd, &ps);
}

VOID Render_Game(HWND hWnd)
{
	if(wait == 0 && HeroShout)
		HeroShoutSelect();
	if(Now_Screen_Pos == -1)
	{
		Controlable = FALSE;
		Pre_Screen_Pos = -5;
		Now_Screen_Pos = 0;
	}
	else if(Pre_Screen_Pos == -5)
	{
		Now_Screen_Pos += 6;
		if(Now_Screen_Pos >= WNDWIDTH)
		{
			Now_Screen_Pos = WNDWIDTH;
			Pre_Screen_Pos = -10;
		}
	}
	else if(Pre_Screen_Pos == -10)
	{
		Now_Screen_Pos -= 8;
		if(Now_Screen_Pos <= 0)
		{
			Now_Screen_Pos = 0;
			Pre_Screen_Pos = 0;
			Controlable = TRUE;
		}
	}
	else
	{
		Pre_Screen_Pos = Now_Screen_Pos;
		if(Status_OF_Game <= 2)
			Now_Screen_Pos = m_hero[now_hero].pos.x + HERO_SIZE_X / 2 - WNDWIDTH / 2;
		else
			Now_Screen_Pos = m_hero[0].pos.x - WNDWIDTH / 2;
		if(Now_Screen_Pos < 0)
			Now_Screen_Pos = 0;
		else if(Now_Screen_Pos > WNDWIDTH)
			Now_Screen_Pos = WNDWIDTH;
		if(abs(Pre_Screen_Pos - Now_Screen_Pos) > 10 && (Status_OF_Game == 4 || Status_OF_Game == 0))
		{
			if(wait <= 0)
				wait = 2;
			if(Pre_Screen_Pos > Now_Screen_Pos)
				Now_Screen_Pos = Pre_Screen_Pos - 10;
			else
				Now_Screen_Pos = Pre_Screen_Pos + 10;
		}
	}

	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);
	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;
	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH * 2, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);
	SelectObject(hdcBmp, gameStatus.hBmp);
	BitBlt(hdcBuffer, 0, 0, WNDWIDTH * 2, WNDHEIGHT, hdcBmp, 0, 0, SRCCOPY);

	//画小地图
	hPen = CreatePen(PS_SOLID, 2, RGB(129, 72, 3));
	hBrush = CreateSolidBrush(NULL_BRUSH);
	SelectObject(hdcBuffer, hPen);		//必须要select到hdcBuffer！！！
	SelectObject(hdcBuffer, hBrush);
	Rectangle(hdcBuffer, SmallMapPos.x - 1 + Now_Screen_Pos, SmallMapPos.y - 2, SmallMapPos.x + 2 + SmallMapSize.x + Now_Screen_Pos, SmallMapPos.y + 2 + SmallMapSize.y);
	DeleteObject(hPen);
	DeleteObject(hBrush);
	SelectObject(hdcBuffer, cptBmp);
	SelectObject(hdcBmp, gameStatus.hS_Bmp);
	BitBlt(hdcBuffer, SmallMapPos.x + Now_Screen_Pos, SmallMapPos.y, SmallMapSize.x, SmallMapSize.y, hdcBmp, 0, 0, SRCCOPY);
	
	//绘制水面
	SelectObject(hdcBmp, m_hS_Water);
	TransparentBlt(hdcBuffer, SmallMapPos.x + Now_Screen_Pos, SmallMapPos.y + SeaLevel / Times_Of_Map, SmallMapSize.x, SmallMapSize.y - SeaLevel / Times_Of_Map, hdcBmp, 0, 0, SmallMapSize.x, SmallMapSize.y - SeaLevel / Times_Of_Map, RGB(255, 255, 255));
	//绘制水面结束

	//绘制山
	SelectObject(hdcBmp, m_hS_Mountain);
	for(temp_new_try = 1; temp_new_try <= WNDWIDTH * 2 + 1; temp_new_try += Times_Of_Map)
	{
		TransparentBlt(hdcBuffer, Terrianheight[temp_new_try].x / Times_Of_Map + SmallMapPos.x + Now_Screen_Pos, Terrianheight[temp_new_try].y / Times_Of_Map + SmallMapPos.y, 1, (WNDHEIGHT - Terrianheight[temp_new_try].y) / Times_Of_Map, hdcBmp, Terrianheight[temp_new_try].x / Times_Of_Map, Terrianheight[temp_new_try].y / Times_Of_Map, 1, (WNDHEIGHT - Terrianheight[temp_new_try].y) / Times_Of_Map, RGB(255, 255, 255));
	}
	//绘制山结束

	//绘制虫子位置
	for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
	{
		if(!m_hero[i_hero].NowBleed)
			continue;
		if(i_hero <= num_hero / 2)
			hPen=CreatePen(PS_SOLID,5,RGB(255, 0, 0));
		else
			hPen=CreatePen(PS_SOLID,5,RGB(255, 255, 0));
		SelectObject(hdcBuffer, hPen);
		temp_point[0].x = temp_point[1].x = m_hero[i_hero].pos.x / Times_Of_Map + 3 + SmallMapPos.x + Now_Screen_Pos;
		temp_point[0].y = temp_point[1].y = m_hero[i_hero].pos.y / Times_Of_Map + SmallMapPos.y;
		Polyline(hdcBuffer, temp_point, 2);
		DeleteObject(hPen);
	}
	//绘制虫子位置结束

	//绘制框
	hPen = CreatePen(PS_SOLID, 2, RGB(129, 72, 3));
	temp_point[0].x = SmallMapPos.x + Now_Screen_Pos + Now_Screen_Pos / Times_Of_Map;
	temp_point[0].y = SmallMapPos.y - 5;
	temp_point[1].x = SmallMapPos.x + Now_Screen_Pos + Now_Screen_Pos / Times_Of_Map;
	temp_point[1].y = SmallMapPos.y + 5 + SmallMapSize.y;
	temp_point[2].x = SmallMapPos.x + Now_Screen_Pos + (Now_Screen_Pos + WNDWIDTH) / Times_Of_Map;
	temp_point[2].y = SmallMapPos.y + 5 + SmallMapSize.y;
	temp_point[3].x = SmallMapPos.x + Now_Screen_Pos + (Now_Screen_Pos + WNDWIDTH) / Times_Of_Map;
	temp_point[3].y = SmallMapPos.y - 5;
	temp_point[4].x = SmallMapPos.x + Now_Screen_Pos + Now_Screen_Pos / Times_Of_Map;
	temp_point[4].y = SmallMapPos.y - 5;
	SelectObject(hdcBuffer, hPen);
	Polyline(hdcBuffer, temp_point, 5);
	DeleteObject(hPen);
	//绘制框结束
	//画小地图结束

	//绘制风速指示图标
	for(temp_i = 1; temp_i <= WindSpeed; temp_i++)
	{
		SelectObject(hdcBmp, m_hR_Wind);
		TransparentBlt(hdcBuffer, WindPos.x + Now_Screen_Pos + temp_i * 29, WindPos.y, 29, 30, hdcBmp, 0, 0, 29, 30, RGB(255, 255, 255));
	}
	for(temp_i = -1; temp_i >= WindSpeed; temp_i--)
	{
		SelectObject(hdcBmp, m_hL_Wind);
		TransparentBlt(hdcBuffer, WindPos.x + Now_Screen_Pos - temp_i * 29, WindPos.y, 29, 30, hdcBmp, 0, 0, 29, 30, RGB(255, 255, 255));
	}
	//绘制风速指示图标结束

	//绘制风速旁边的框
	hPen = CreatePen(PS_SOLID, 2, RGB(181, 230, 29));
	temp_point[0].x = WindPos.x + Now_Screen_Pos + 29 - 10;
	temp_point[0].y = WindPos.y - 10;
	temp_point[1].x = WindPos.x + Now_Screen_Pos + 29 - 10;
	temp_point[1].y = WindPos.y + 40;
	temp_point[2].x = WindPos.x + Now_Screen_Pos + 29 * 5 + 10;
	temp_point[2].y = WindPos.y + 40;
	temp_point[3].x = WindPos.x + Now_Screen_Pos + 29 * 5 + 10;
	temp_point[3].y = WindPos.y - 10;
	temp_point[4].x = WindPos.x + Now_Screen_Pos + 29 - 10;
	temp_point[4].y = WindPos.y - 10;
	SelectObject(hdcBuffer, hPen);
	Polyline(hdcBuffer, temp_point, 5);
	DeleteObject(hPen);
	//绘制风速旁边的框结束

	// 绘制分数
	SetBkMode(hdcBuffer, TRANSPARENT);
	SetTextColor(hdcBuffer, RGB(255, 0, 0));
	wsprintf(szDist, _T("%d"), num_alive[1]);
	TextOut(hdcBuffer, WNDWIDTH / 2 + Now_Screen_Pos, 85, szDist, _tcslen(szDist));
	SetTextColor(hdcBuffer, RGB(255, 255, 0));
	wsprintf(szDist, _T("%d"), num_alive[2]);
	TextOut(hdcBuffer, WNDWIDTH / 2 + 30 + Now_Screen_Pos, 85, szDist, _tcslen(szDist));
	//绘制分数结束

	//绘制血量条
	hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	hBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdcBuffer, hPen);
	SelectObject(hdcBuffer, hBrush);
	temp_bloodbar = 0;
	for(i_hero = 1; i_hero <= num_hero / 2; i_hero++)
	{
		Rectangle(hdcBuffer, BloodBarPosX + Now_Screen_Pos + temp_bloodbar, BloodBarPosY, BloodBarPosX + Now_Screen_Pos + temp_bloodbar + (BloodBarSizeX * m_hero[i_hero].NowBleed + m_hero[i_hero].AllBleed - 1) / m_hero[i_hero].AllBleed, BloodBarSizeY + BloodBarPosY);
		temp_bloodbar += 10 + BloodBarSizeX * m_hero[i_hero].NowBleed / m_hero[i_hero].AllBleed;
	}
	DeleteObject(hPen);
	DeleteObject(hBrush);

	hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	hBrush = CreateSolidBrush(RGB(255, 255, 0));
	SelectObject(hdcBuffer, hPen);
	SelectObject(hdcBuffer, hBrush);
	temp_bloodbar = 0;
	for(; i_hero <= num_hero; i_hero++)
	{
		Rectangle(hdcBuffer, BloodBarPosX + Now_Screen_Pos + temp_bloodbar, BloodBarPosY + BloodBarSizeY + BloodBarPosPlus, BloodBarPosX + Now_Screen_Pos + temp_bloodbar + (BloodBarSizeX * m_hero[i_hero].NowBleed + m_hero[i_hero].AllBleed - 1) / m_hero[i_hero].AllBleed, BloodBarSizeY + BloodBarPosY + BloodBarSizeY + BloodBarPosPlus);
		temp_bloodbar += 10 + BloodBarSizeX * m_hero[i_hero].NowBleed / m_hero[i_hero].AllBleed;
	}
	DeleteObject(hPen);
	DeleteObject(hBrush);
	//绘制血量条结束

	//绘制技能点
	if(ShowTrack[0])
	{
		SelectObject(hdcBmp, m_hFrontSightBmp);
		TransparentBlt(hdcBuffer, BloodBarPosX + Now_Screen_Pos - 40, BloodBarPosY, 15, 15, hdcBmp, 0, 0, 28, 28, RGB(255, 255, 255));
	}
	if(ShowTrack[1])
	{
		SelectObject(hdcBmp, m_hY_FrontSightBmp);
		TransparentBlt(hdcBuffer, BloodBarPosX + Now_Screen_Pos - 40, BloodBarPosY + BloodBarSizeY + BloodBarPosPlus, 15, 15, hdcBmp, 0, 0, 28, 28, RGB(255, 255, 255));
	}
	if(JumpHigher[0])
	{
		SelectObject(hdcBmp, hR_Jump);
		TransparentBlt(hdcBuffer, BloodBarPosX + Now_Screen_Pos - 28, BloodBarPosY, 15, 15, hdcBmp, 0, 0, 28, 28, RGB(255, 255, 255));
	}
	if(JumpHigher[1])
	{
		SelectObject(hdcBmp, hY_Jump);
		TransparentBlt(hdcBuffer, BloodBarPosX + Now_Screen_Pos - 28, BloodBarPosY + BloodBarSizeY + BloodBarPosPlus, 15, 15, hdcBmp, 0, 0, 28, 28, RGB(255, 255, 255));
	}
	//绘制技能点结束

	//绘制水面
	SelectObject(hdcBmp, m_hWater);
	TransparentBlt(hdcBuffer, 0, SeaLevel, WNDWIDTH * 2, WNDHEIGHT - SeaLevel, hdcBmp, 0, 0, WNDWIDTH * 2, WNDHEIGHT - SeaLevel, RGB(255, 255, 255));
	//绘制水面结束

	//绘制山
	SelectObject(hdcBmp, m_hMountain);
	for(temp_new_try = 1; temp_new_try <= WNDWIDTH * 2 + 1; temp_new_try++)
	{
		TransparentBlt(hdcBuffer, Terrianheight[temp_new_try].x, Terrianheight[temp_new_try].y, 1, WNDHEIGHT - Terrianheight[temp_new_try].y, hdcBmp, Terrianheight[temp_new_try].x, Terrianheight[temp_new_try].y, 1, WNDHEIGHT - Terrianheight[temp_new_try].y, RGB(255, 255, 255));
	}
	//绘制山结束


	// 绘制Hero到缓存
	for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
	{
		if(!m_hero[i_hero].NowBleed)
			continue;
		SelectObject(hdcBmp, m_hero[i_hero].hBmp);
		TransparentBlt(hdcBuffer, m_hero[i_hero].pos.x, m_hero[i_hero].pos.y, m_hero[i_hero].size.cx, m_hero[i_hero].size.cy, hdcBmp, 0, m_hero[i_hero].curFrameIndex * m_hero[i_hero].size.cy, m_hero[i_hero].size.cx, m_hero[i_hero].size.cy, RGB(255, 255, 255));
	}
	//绘制Hero结束

	//绘制虫子手上拿的武器
	if(Status_OF_Game < 3)
	{
		m_hero[0].pos.x = m_hero[now_hero].pos.x + HERO_SIZE_X / 2;
		m_hero[0].pos.y = m_hero[now_hero].pos.y + HERO_SIZE_Y * 3 / 4;
	}
	//绘制虫子手上拿的武器结束

	if(Status_OF_Game <= 4)
	{
		SelectObject(hdcBmp, m_hero[0].hBmp);
		TransparentBlt(
			hdcBuffer, m_hero[0].pos.x - m_hero[0].size.cx / 2, m_hero[0].pos.y - m_hero[0].size.cy / 2,
			m_hero[0].size.cx, m_hero[0].size.cy,
			hdcBmp, 0, 0, m_hero[0].size.cx, m_hero[0].size.cy,
			RGB(255, 255, 255)
		);
		SelectObject(hdcBmp, m_Weapon[now_Weapon].BigBmp);
		TransparentBlt(hdcBuffer, 30 + Now_Screen_Pos, 30, Weapon_Big_Size, Weapon_Big_Size, hdcBmp, 0, 0, Weapon_Big_Size, Weapon_Big_Size, RGB(255, 255, 255));
		SelectObject(hdcBmp, m_hChooseBack);
		TransparentBlt(hdcBuffer, WNDWIDTH - ChooseBackWidth - 30 + Now_Screen_Pos, 30, ChooseBackWidth, ChooseBackHeight, hdcBmp, 0, 0, ChooseBackWidth, ChooseBackHeight, RGB(255, 255, 255));
		SelectObject(hdcBmp, m_hQ_Button);
		TransparentBlt(hdcBuffer, WNDWIDTH - ChooseBackWidth - 110 + Now_Screen_Pos, 30, ChooseBackWidth, ChooseBackHeight, hdcBmp, 0, 0, 72, 72, RGB(255, 255, 255));
	}

	//绘制血槽和血量
	for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
	{
		if(No_Hero_Dec[i_hero][0] > 0)
		{
			No_Hero_Dec[i_hero][0]--;
			if(i_hero <= num_hero / 2)
				SetTextColor(hdcBuffer, RGB(255, 0, 0));
			else
				SetTextColor(hdcBuffer, RGB(255, 255, 0));
			SetBkMode(hdcBuffer, TRANSPARENT);
			if(No_Hero_Dec[i_hero][1] >= 0)
				wsprintf(BloodDecNum, L"+%d", No_Hero_Dec[i_hero][1]);
			else
				wsprintf(BloodDecNum, L"%d", No_Hero_Dec[i_hero][1]);
			if(No_Hero_Dec[i_hero][1] < -99)
				TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 19, m_hero[i_hero].pos.y - 65, BloodDecNum, _tcslen(BloodDecNum));
			else if(No_Hero_Dec[i_hero][1] < -9 || No_Hero_Dec[i_hero][1] > 0)
				TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 14, m_hero[i_hero].pos.y - 65, BloodDecNum, _tcslen(BloodDecNum));
			else
				TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 9, m_hero[i_hero].pos.y - 65, BloodDecNum, _tcslen(BloodDecNum));
		}
		if(!m_hero[i_hero].NowBleed)
		{
			if(No_Hero_Dead[i_hero])
			{
				No_Hero_Dead[i_hero]--;
				SetTextColor(hdcBuffer, RGB(255, 0, 0));
				SetBkMode(hdcBuffer, TRANSPARENT);
				wsprintf(DeadText, L"Dead");
				TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 14, m_hero[i_hero].pos.y, DeadText, _tcslen(L"Dead"));
			}
			continue;
		}
		if(m_hero[i_hero].NowBleed >= 100)
		{
			R_blood = 0;
			G_blood = 255;
		}
		else if(m_hero[i_hero].NowBleed >= 50)
		{
			R_blood = 255 * (100 - m_hero[i_hero].NowBleed) / 50;
			G_blood = 255;
		}
		else
		{
			R_blood = 255;
			G_blood = 255 * m_hero[i_hero].NowBleed / 50;
		}
		hBrush=CreateSolidBrush(RGB(R_blood, G_blood, 0));
		SelectObject(hdcBuffer, hBrush);
		hPen=CreatePen(PS_SOLID,2,RGB(R_blood, G_blood, 0));
		SelectObject(hdcBuffer, hPen);
		RoundRect(hdcBuffer, m_hero[i_hero].pos.x, m_hero[i_hero].pos.y - 15, m_hero[i_hero].pos.x + 2 + (HERO_SIZE_X - 2) * m_hero[i_hero].NowBleed / m_hero[i_hero].AllBleed, m_hero[i_hero].pos.y - 5, 4, 3);
		DeleteObject(hPen);
		DeleteObject(hBrush);
	//绘制虫子号
		if(i_hero <= num_hero / 2)
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
		else
			SetTextColor(hdcBuffer, RGB(255, 255, 0));
		SetBkMode(hdcBuffer, TRANSPARENT);
		wsprintf(HeroNumText, L"%d", i_hero);
		if(i_hero > 9)
			TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 9, m_hero[i_hero].pos.y - 33, HeroNumText, _tcslen(HeroNumText));
		else
			TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 4, m_hero[i_hero].pos.y - 33, HeroNumText, _tcslen(HeroNumText));
	//绘制血量数字
		wsprintf(HeroNumText, L"%d", m_hero[i_hero].NowBleed);
		if(m_hero[i_hero].NowBleed > 99)
			TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 14, m_hero[i_hero].pos.y - 53, HeroNumText, _tcslen(HeroNumText));
		else if(m_hero[i_hero].NowBleed > 9)
			TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 9, m_hero[i_hero].pos.y - 53, HeroNumText, _tcslen(HeroNumText));
		else
			TextOut(hdcBuffer, m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - 4, m_hero[i_hero].pos.y - 53, HeroNumText, _tcslen(HeroNumText));
	}
	//绘制血量数字结束

	//绘制医疗箱
	for(i_AidCase = 0; i_AidCase < num_AidCase; i_AidCase++)
	{
		if(m_AidCase[i_AidCase].exist)
		{
			SelectObject(hdcBmp, m_AidCase[i_AidCase].hBmp);
			TransparentBlt(hdcBuffer, m_AidCase[i_AidCase].pos.x, m_AidCase[i_AidCase].pos.y, AidCaseSize_X, AidCaseSize_Y, hdcBmp, 0, 0, AidCaseSize_X, AidCaseSize_Y, RGB(255, 255, 255));
		}
	}
	//绘制医疗箱结束

	//绘制瞄准箱
	for(i_TargetCase = 0; i_TargetCase < num_TargetCase; i_TargetCase++)
	{
		if(m_TargetCase[i_TargetCase].exist && m_TargetCase[i_TargetCase].type != DSpringCase)
		{
			SelectObject(hdcBmp, m_TargetCase[i_TargetCase].hBmp);
			TransparentBlt(hdcBuffer, m_TargetCase[i_TargetCase].pos.x, m_TargetCase[i_TargetCase].pos.y, TargetCaseSize_X, TargetCaseSize_Y, hdcBmp, 0, 0, TargetCaseSize_X, TargetCaseSize_Y, RGB(255, 255, 255));
		}
		else if(m_TargetCase[i_TargetCase].exist)
		{
			SelectObject(hdcBmp, m_TargetCase[i_TargetCase].hBmp);
			TransparentBlt(hdcBuffer, m_TargetCase[i_TargetCase].pos.x, m_TargetCase[i_TargetCase].pos.y, TargetCaseSize_X, TargetCaseSize_Y, hdcBmp, 0, 0, 120, 129, RGB(255, 255, 255));
		}
	}
	//绘制瞄准箱结束

	if(wait <= 0)
	{
	//绘制指示哪一只虫子（线）
	if(now_hero <= num_hero / 2)
		hPen=CreatePen(PS_SOLID,2,RGB(255, 0, 0));
	else
		hPen=CreatePen(PS_SOLID,2,RGB(255, 255, 0));
	SelectObject(hdcBuffer, hPen);
	temp_point[0].x = m_hero[now_hero].pos.x;
	temp_point[0].y = m_hero[now_hero].pos.y - 35;
	temp_point[1].x = m_hero[now_hero].pos.x + HERO_SIZE_X;
	temp_point[1].y = m_hero[now_hero].pos.y - 35;
	Polyline(hdcBuffer, temp_point, 2);
	DeleteObject(hPen);
	//绘制指示哪一只虫子（线）结束

	//绘制指示哪一只虫子（箭头）
	if(now_hero <= num_hero / 2)
		SelectObject(hdcBmp, m_hSticker);
	else
		SelectObject(hdcBmp, m_hY_Sticker);
	TransparentBlt(hdcBuffer, m_hero[now_hero].pos.x + HERO_SIZE_X / 2 - 15, m_hero[now_hero].pos.y - 90, 29, 30, hdcBmp, 0, 0, 29, 30, RGB(255, 255, 255));
	//绘制指示哪一只虫子（箭头）结束
	}

	if(Status_OF_Game == 0)
	{
		for(temp_i = 0; temp_i <= num_hero; temp_i++)
			power_add[temp_i] = 0;
		Degree_Of_power = 30;
		if(m_hero[now_hero].position) //向左
			temp_x = int(m_hero[now_hero].pos.x + HERO_SIZE_X / 2 - temp_R_firing * cos(Degree_Of_Firing * Pi / 180));
		else
			temp_x = int(m_hero[now_hero].pos.x + HERO_SIZE_X / 2 + temp_R_firing * cos(Degree_Of_Firing * Pi / 180));
		temp_y = int(m_hero[now_hero].pos.y + HERO_SIZE_Y / 4 - temp_R_firing * sin(Degree_Of_Firing * Pi / 180));
		if(temp_x < 1)
		{
			temp_y = (temp_y - (m_hero[now_hero].pos.y + HERO_SIZE_Y / 4)) * (1 - (m_hero[now_hero].pos.x + HERO_SIZE_X / 2)) / (temp_x - (m_hero[now_hero].pos.x + HERO_SIZE_X / 2)) + m_hero[now_hero].pos.y + HERO_SIZE_Y / 4;
			temp_x = 1;
		}
		else if(temp_x >= WNDWIDTH * 2)
		{
			temp_y = (temp_y - (m_hero[now_hero].pos.y + HERO_SIZE_Y / 4)) * (WNDWIDTH * 2 - 1 - (m_hero[now_hero].pos.x + HERO_SIZE_X / 2)) / (temp_x - (m_hero[now_hero].pos.x + HERO_SIZE_X / 2)) + m_hero[now_hero].pos.y + HERO_SIZE_Y / 4;
			temp_x = WNDWIDTH * 2 - 1;
		}
		if(now_hero <= num_hero / 2)
			SelectObject(hdcBmp, m_hFrontSightBmp);
		else
			SelectObject(hdcBmp, m_hY_FrontSightBmp);
		TransparentBlt(hdcBuffer, temp_x - 14, temp_y - 14, 28, 28, hdcBmp, 0, 0, 28, 28, RGB(255, 255, 255));
	}
	else if(Status_OF_Game == 1)
	{
		if(m_hero[now_hero].position) //向左
			temp_point[0].x = int(m_hero[now_hero].pos.x + HERO_SIZE_X / 2 - temp_r_power * Degree_Of_power * cos(Degree_Of_Firing * Pi / 180));
		else
			temp_point[0].x = int(m_hero[now_hero].pos.x + HERO_SIZE_X / 2 + temp_r_power * Degree_Of_power * cos(Degree_Of_Firing * Pi / 180));
		temp_point[0].y = int(m_hero[now_hero].pos.y + HERO_SIZE_Y / 4 - temp_r_power * Degree_Of_power * sin(Degree_Of_Firing * Pi / 180));
		temp_point[1].x = m_hero[now_hero].pos.x + HERO_SIZE_X / 2;
		temp_point[1].y = m_hero[now_hero].pos.y + HERO_SIZE_Y / 4;
		Polyline(hdcBuffer, temp_point, 2);
		
		//画出轨迹
		if((ShowTrack[0] && now_hero <= num_hero / 2) || (ShowTrack[1] && now_hero > num_hero / 2))
		{
			if(now_hero <= num_hero / 2)
				m_Track.R_G_B[1] = 0;
			else
				m_Track.R_G_B[1] = 255;
			GetTrack(TRUE, Degree_Of_Firing, Degree_Of_power, m_hero[now_hero].pos.x + HERO_SIZE_X / 2, m_hero[now_hero].pos.y + HERO_SIZE_Y / 4, m_hero[now_hero].position, m_Weapon[now_Weapon].Vel);
			hPen = CreatePen(PS_SOLID, 2, RGB(m_Track.R_G_B[0], m_Track.R_G_B[1], m_Track.R_G_B[2]));
			SelectObject(hdcBuffer, hPen);
			Polyline(hdcBuffer, m_Track.Points, m_Track.Lenth);
			DeleteObject(hPen);
			hPen=CreatePen(PS_SOLID, 5 ,RGB(255, 255, 255));
			SelectObject(hdcBuffer, hPen);
			temp_point[0].x = temp_point[1].x = m_Track.Destination.x / Times_Of_Map + 3 + SmallMapPos.x + Now_Screen_Pos;
			temp_point[0].y = temp_point[1].y = m_Track.Destination.y / Times_Of_Map + SmallMapPos.y;
			Polyline(hdcBuffer, temp_point, 2);
			DeleteObject(hPen);
		}
		//画轨迹结束
	}
	else if(Status_OF_Game == 2)
	{
		Status_OF_Game = 3;
		Hit(Degree_Of_Firing, Degree_Of_power, m_hero[now_hero].pos.x + HERO_SIZE_X / 2, m_hero[now_hero].pos.y + HERO_SIZE_Y / 4, m_hero[now_hero].position, m_Weapon[now_Weapon].Vel);
		Degree_Of_Firing = 0;
		Degree_Of_power = 30;
	}
	else if(Status_OF_Game == 3)
	{
		m_hero[now_hero].vel.x = 0;
		if(Hitting())
			Status_OF_Game = 4;
	}
	else if(Status_OF_Game == 4)
	{
		CreateWindSpeed();
		if(Now_Hero_Change())
			Status_OF_Game = 0;
	}

	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, Now_Screen_Pos, 0, SRCCOPY);
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);
	EndPaint(hWnd, &ps);
}

VOID Render_End(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);
	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;
	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);
	SelectObject(hdcBmp, gameStatus.hBmp);
	BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, SRCCOPY);
	SelectObject(hdcBmp, m_gameBalanceButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameBalanceButton.pos.x, m_gameBalanceButton.pos.y, m_gameBalanceButton.size.cx, m_gameBalanceButton.size.cy, hdcBmp, 0, 0, m_gameBalanceButton.size.cx, m_gameBalanceButton.size.cy, RGB(255, 255, 255));
	SelectObject(hdcBmp, m_gameQuitButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameQuitButton.pos.x, m_gameQuitButton.pos.y, m_gameQuitButton.size.cx, m_gameQuitButton.size.cy, hdcBmp, 0, 0, m_gameQuitButton.size.cx, m_gameQuitButton.size.cy, RGB(255, 255, 255));
		SelectObject(hdcBmp, m_gameRestartButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameRestartButton.pos.x, m_gameRestartButton.pos.y, m_gameRestartButton.size.cx, m_gameRestartButton.size.cy, hdcBmp, 0, 0, m_gameRestartButton.size.cx, m_gameRestartButton.size.cy, RGB(255, 255, 255));
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);
	EndPaint(hWnd, &ps);
}

VOID Render_End_Red(HWND hWnd)//绘制游戏结束红胜界面
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);
	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;
	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);
	SelectObject(hdcBmp, gameStatus.hBmp);
	BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, SRCCOPY);
	SelectObject(hdcBmp, m_gameRedWinButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameRedWinButton.pos.x, m_gameRedWinButton.pos.y, m_gameRedWinButton.size.cx, m_gameRedWinButton.size.cy, hdcBmp, 0, 0, m_gameRedWinButton.size.cx, m_gameRedWinButton.size.cy, RGB(255, 255, 255));
	SelectObject(hdcBmp, m_gameQuitButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameQuitButton.pos.x, m_gameQuitButton.pos.y, m_gameQuitButton.size.cx, m_gameQuitButton.size.cy, hdcBmp, 0, 0, m_gameQuitButton.size.cx, m_gameQuitButton.size.cy, RGB(255, 255, 255));
	SelectObject(hdcBmp, m_gameRestartButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameRestartButton.pos.x, m_gameRestartButton.pos.y, m_gameRestartButton.size.cx, m_gameRestartButton.size.cy, hdcBmp, 0, 0, m_gameRestartButton.size.cx, m_gameRestartButton.size.cy, RGB(255, 255, 255));
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);
	EndPaint(hWnd, &ps);
}

VOID Render_End_Yellow(HWND hWnd)//绘制游戏结束黄胜界面
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);
	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;
	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);
	SelectObject(hdcBmp, gameStatus.hBmp);
	BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBmp, 0, 0, SRCCOPY);
	SelectObject(hdcBmp, m_gameYellowWinButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameYellowWinButton.pos.x, m_gameYellowWinButton.pos.y, m_gameYellowWinButton.size.cx, m_gameYellowWinButton.size.cy, hdcBmp, 0, 0, m_gameYellowWinButton.size.cx, m_gameYellowWinButton.size.cy, RGB(255, 255, 255));
	SelectObject(hdcBmp, m_gameQuitButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameQuitButton.pos.x, m_gameQuitButton.pos.y, m_gameQuitButton.size.cx, m_gameQuitButton.size.cy, hdcBmp, 0, 0, m_gameQuitButton.size.cx, m_gameQuitButton.size.cy, RGB(255, 255, 255));
	SelectObject(hdcBmp, m_gameRestartButton.hBmp);
	TransparentBlt(hdcBuffer, m_gameRestartButton.pos.x, m_gameRestartButton.pos.y, m_gameRestartButton.size.cx, m_gameRestartButton.size.cy, hdcBmp, 0, 0, m_gameRestartButton.size.cx, m_gameRestartButton.size.cy, RGB(255, 255, 255));
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);
	EndPaint(hWnd, &ps);
}

VOID Render_Help(HWND hWnd)		//帮助页面
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);
	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;
	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);
	SelectObject(hdcBmp, gameStatus.hBmp);
	BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
		hdcBmp, 0, 0, SRCCOPY);
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);
	EndPaint(hWnd, &ps);
}

VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)	//刷新
{
	HeroUpdate();
	InvalidateRect(hWnd, NULL, FALSE);
}

VOID boom(int x, int y, int range, int power)		//开始爆炸
{
	//PlaySound((LPCTSTR)IDR_Boom,NULL,SND_RESOURCE|SND_ASYNC);
	if(rand() % 2)
	{
		mciSendString(_T("stop myBoom"), NULL, 0, NULL);
		mciSendString(_T("close myBoom"), NULL, 0, NULL);
		mciSendString(mcicmdBoom, NULL,0, NULL);
		mciSendString(_T("play myBoom"), NULL, 0, NULL);
	}
	else
	{
		mciSendString(_T("stop myHit"), NULL, 0, NULL);
		mciSendString(_T("close myHit"), NULL, 0, NULL);
		mciSendString(mcicmdHit, NULL,0, NULL);
		mciSendString(_T("play myHit"), NULL, 0, NULL);
	}

	if(rand() % 6 == 1)
		CreateAidCase();
	else if(rand() % 7 > 3)
		CreateTargetCase(); 
	int x_leftest = x - range + 2, x_rightest = x + range - 2, temp_boom_height, temp_boom_i;
	if(x_leftest <= 0)
		x_leftest = 1;
	if(x_rightest >= WNDWIDTH * 2)
		x_rightest = WNDWIDTH * 2 - 1;
	for(temp_boom_i = x_leftest; temp_boom_i <= x_rightest; temp_boom_i++)
	{
		temp_boom_height = y + int(sqrt(((range - 2) * (range - 2) - (temp_boom_i - x) * (temp_boom_i - x)) * 1.0));
		Terrianheight[temp_boom_i].y = Terrianheight[temp_boom_i].y < temp_boom_height ? temp_boom_height : Terrianheight[temp_boom_i].y;
		if(Terrianheight[temp_boom_i].y >= WNDHEIGHT)
			Terrianheight[temp_boom_i].y = WNDHEIGHT;
	}
	for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
	{
		if(m_hero[i_hero].NowBleed <= 0)
			continue;
		if(m_hero[i_hero].pos.x + HERO_SIZE_X / 2 > x - range - 10 && m_hero[i_hero].pos.x + HERO_SIZE_X / 2 < x + range + 10 && m_hero[i_hero].pos.y + HERO_SIZE_Y / 2 < y + range + 10 && m_hero[i_hero].pos.y + HERO_SIZE_Y / 2 > y - range - 10)
		{
			m_hero[i_hero].NowBleed -= (temp_boom_height = (((range + 10) * (range + 10) * 2 - (y - (m_hero[i_hero].pos.y + HERO_SIZE_Y / 2)) * (y - (m_hero[i_hero].pos.y + HERO_SIZE_Y / 2)) - (x - (m_hero[i_hero].pos.x + HERO_SIZE_X / 2)) * (x - (m_hero[i_hero].pos.x + HERO_SIZE_X / 2))) / 2) * power / ((range + 10) * (range + 10)));
			if(m_hero[i_hero].pos.x + HERO_SIZE_X / 2 > x - range - 10 && m_hero[i_hero].pos.x + HERO_SIZE_X / 2 < x + range + 10)
			{
				if(m_hero[i_hero].pos.y + HERO_SIZE_Y / 2 - y < 0)
					m_hero[i_hero].vel.y -= (-m_hero[i_hero].pos.y - HERO_SIZE_Y / 2 + y) * FlyingVel / (range + 10) + 5;
				else
					m_hero[i_hero].vel.y -= 8;
				if(m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - x > 0)
					m_hero[i_hero].vel.x += (int)(sqrt((1.0 * m_hero[i_hero].pos.x + HERO_SIZE_X / 2 - x) / (range + 10)) * FlyingVel) + 2;
				else
					m_hero[i_hero].vel.x -= (int)(sqrt((-1.0 * m_hero[i_hero].pos.x + HERO_SIZE_X / 2 + x) / (range + 10)) * FlyingVel) + 2;
			}
			No_Hero_Dec[i_hero][0] = 50;
			No_Hero_Dec[i_hero][1] = -temp_boom_height;
			if(m_hero[i_hero].NowBleed <= 0)
			{
				m_hero[i_hero].NowBleed = 0;
				HeroDie(i_hero);
			}
		}
	}
}

VOID Game_Over_Run()		//游戏结束倒计时结束，显示游戏结束界面
{
	PlaySound((LPCTSTR)IDR_NotGaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
	if(!num_alive[0])
	{
		gameStatus.status = 2;  //全死
		if(rand() % 2)
			gameStatus.hBmp = m_hEndBack1;
		else
			gameStatus.hBmp = m_hEndBack2;
	}
	else if(!num_alive[1])
	{
		gameStatus.status = 4;  //1队死
		if(rand() % 2)
			gameStatus.hBmp = m_hEndBack1;
		else
			gameStatus.hBmp = m_hEndBack2;
	}
	else if(!num_alive[2])
	{
		gameStatus.status = 3;  //2队死
		if(rand() % 2)
			gameStatus.hBmp = m_hEndBack1;
		else
			gameStatus.hBmp = m_hEndBack2;
	}
}

VOID Game_over()		//游戏结束，但是不立即显示
{
	if(gameover == 0)
		gameover = 51;
}

VOID HeroDie(int hero_dead)		//人物死了
{
	num_alive[0]--;
	if(hero_dead <= num_hero / 2)
		num_alive[1]--;
	else
		num_alive[2]--;
	No_Hero_Dead[hero_dead] = 80;
	if(m_hero[hero_dead].pos.y + HERO_SIZE_Y < SeaLevel)
	{
		mciSendString(_T("stop myHeroDie"), NULL, 0, NULL);
		mciSendString(_T("close myHeroDie"), NULL, 0, NULL);
		mciSendString(mcicmdHeroDie, NULL,0, NULL);
		mciSendString(_T("play myHeroDie"), NULL, 0, NULL);
		boom(m_hero[hero_dead].pos.x + HERO_SIZE_X / 2, m_hero[hero_dead].pos.y + HERO_SIZE_Y / 2, HERO_SIZE_X + 20, 20);
	}
	else
	{
		mciSendString(_T("stop myDrown"), NULL, 0, NULL);
		mciSendString(_T("close myDrown"), NULL, 0, NULL);
		mciSendString(mcicmdDrown, NULL,0, NULL);
		mciSendString(_T("play myDrown"), NULL, 0, NULL);
	}
	if((!num_alive[1] || !num_alive[2]))
		Game_over();
	else if(now_hero == hero_dead)
	{
		Same_Team_Change();
	}
}

VOID HeroUpdate()		//人物更新相关
{
	if(wait > 0 && Pre_Screen_Pos == Now_Screen_Pos)
		wait--;
	if(((AI_WAIT != 0 && Pre_Screen_Pos == Now_Screen_Pos) || !AI_WAIT) && num_alive[1] && num_alive[2] && Game_Type == 1)
	{
		if(now_hero > num_hero / 2)
		{
			if(If_AI_Controling)
			{
				if(AI_WAIT == 0)
				{
					if(AI_Step[0] > 0)
					{
						AI_Step[0]--;
						if(m_hero[now_hero].pos.x + HERO_VEL_X <= WNDWIDTH * 2)
							m_hero[now_hero].vel.x = HERO_VEL_X;
						if((Terrianheight[m_hero[now_hero].pos.x + 1 + HERO_SIZE_X].y <= m_hero[now_hero].pos.y + HERO_SIZE_Y + 2) || (Terrianheight[m_hero[now_hero].pos.x + HERO_VEL_X + HERO_SIZE_X].y <= m_hero[now_hero].pos.y + HERO_SIZE_Y + 2))
						{
							if(JumpHigher[1])
								m_hero[now_hero].vel.y = - 2 * HERO_VEL_Y;
							else
								m_hero[now_hero].vel.y = - HERO_VEL_Y;
						}
					}
					else if(AI_Step[0] < 0)
					{
						AI_Step[0]++;
						if(m_hero[now_hero].pos.x - HERO_VEL_X <= WNDWIDTH * 2)
							m_hero[now_hero].vel.x = -HERO_VEL_X;
						if((Terrianheight[m_hero[now_hero].pos.x - 1 + HERO_SIZE_X / 4].y <= m_hero[now_hero].pos.y + HERO_SIZE_Y + 2) || (Terrianheight[m_hero[now_hero].pos.x - HERO_VEL_X + HERO_SIZE_X / 4].y <= m_hero[now_hero].pos.y + HERO_SIZE_Y + 2))
						{
							if(JumpHigher[1])
								m_hero[now_hero].vel.y = - 2 * HERO_VEL_Y;
							else
								m_hero[now_hero].vel.y = - HERO_VEL_Y;
						}
					}
					else if(AI_Step[0] == 0)
					{
						m_hero[now_hero].vel.x = 0;
						if(m_hero[now_hero].vel.y == 0)
						{
							if(AI_Step[1] > 0)
							{
								Degree_Of_Firing += 3;
								AI_Step[1] -= 1;
								if(AI_Step[1] == 0)
									Status_OF_Game = 1;
							}
								else if(AI_Step[1] < 0)
								{
								Degree_Of_Firing -= 3;
								AI_Step[1] += 1;
								if(AI_Step[1] == 0)
									Status_OF_Game = 1;
							}
							else if(AI_Step[1] == 0)
							{
								if(AI_Step[2] > 0)
								{
									AI_Step[2]--;
									power_add[now_hero] = 4;
									if(Degree_Of_power >= 150)
									{
										power_add[now_hero] = 0;
										AI_Step[2] = 0;
									}
								}
								else
								{
									power_add[now_hero] = 0;
									if(Status_OF_Game <= 1)
									{
										If_AI_Controling = FALSE;
										Status_OF_Game = 2;
									}
								}
							}
						}
					}
				}
				else
					AI_WAIT--;
			}
			else if(Status_OF_Game == 0)
			{
				Controlable = FALSE;
				for(i_hero = 1; i_hero <= num_hero && ((m_hero[i_hero].vel.x == 0 && m_hero[i_hero].vel.y == 0) || m_hero[i_hero].NowBleed <= 0); i_hero++);
				if(i_hero > num_hero)
				{
					If_AI_Controling = TRUE;
					AI(AI_Type);
				}
			}
		}
		else
		{
			if(AI_Step[2] != 0)
			{
				AI_Step[2] = 0;
				power_add[now_hero] = 0;
			}
			Controlable = TRUE;
		}
	}
	if(Pre_Screen_Pos < 0)
		Controlable = FALSE;
	else if(Controlable == FALSE && Game_Type == 0)
		Controlable = TRUE;
	if(now_hero <= num_hero / 2 && AI_Step[2] != 0)
	{
		AI_Step[2] = 0;
		power_add[now_hero] = 0;
	}
	Degree_Of_power += power_add[now_hero];
	if(Degree_Of_power >= 150)
	{
		Degree_Of_power = 150;
		Status_OF_Game = 2;
	}
	if(gameover == 1)
	{
		Game_Over_Run();
		gameover = 0;
	}
	else if(gameover > 1)
		gameover--;

	if(if_pressing_ad[now_hero] || (If_AI_Controling && m_hero[now_hero].vel.x != 0 && now_hero > num_hero / 2))
		m_hero[now_hero].curFrameIndex = (m_hero[now_hero].curFrameIndex + 1) % HERO_MAX_FRAME_NUM;
	for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
	{
		if(!m_hero[i_hero].NowBleed)
			continue;
		if((!If_AI_Controling && now_hero == i_hero && !if_pressing_ad[i_hero] && m_hero[i_hero].vel.x) || (now_hero != i_hero && !if_pressing_ad[i_hero] && m_hero[i_hero].vel.x))
		{
			if(m_hero[i_hero].vel.x > 0)
				m_hero[i_hero].vel.x -= FlyingDec;
			else
				m_hero[i_hero].vel.x += FlyingDec;
		}
		m_hero[i_hero].pos.y += m_hero[i_hero].vel.y;
		m_hero[i_hero].pos.x += m_hero[i_hero].vel.x;
		if(!m_hero[i_hero].position)
		{
			max_of_height = Terrianheight[m_hero[i_hero].pos.x].y;
			for(temp_compare = 1; temp_compare <= HERO_SIZE_X * 3 / 4; temp_compare++)
				max_of_height = max_of_height < Terrianheight[m_hero[i_hero].pos.x  + temp_compare].y ? max_of_height : Terrianheight[m_hero[i_hero].pos.x  + temp_compare].y;
		}
		else
		{
			max_of_height = Terrianheight[m_hero[i_hero].pos.x + HERO_SIZE_X / 4].y;
			for(temp_compare = 1 +  HERO_SIZE_X / 4; temp_compare <= HERO_SIZE_X; temp_compare++)
				max_of_height = max_of_height < Terrianheight[m_hero[i_hero].pos.x  + temp_compare].y ? max_of_height : Terrianheight[m_hero[i_hero].pos.x  + temp_compare].y;
		}
		if(m_hero[i_hero].pos.x < 1)
		{
			m_hero[i_hero].pos.x = 1;
			if(m_hero[i_hero].vel.x < 0)
				m_hero[i_hero].vel.x = 0;
		}
		if(m_hero[i_hero].pos.x > WNDWIDTH * 2 - 1 - HERO_SIZE_X)
		{
			m_hero[i_hero].pos.x = WNDWIDTH * 2 - 1 - HERO_SIZE_X;
			if(m_hero[i_hero].vel.x > 0)
				m_hero[i_hero].vel.x = 0;
		}
		if(if_ChangeDirection)
		{
			if(m_hero[i_hero].pos.y > max_of_height - HERO_SIZE_Y)
				m_hero[i_hero].pos.y = max_of_height - HERO_SIZE_Y;
			if_ChangeDirection = 0;
		}
		if(if_jump[i_hero] && m_hero[i_hero].pos.y >= max_of_height - HERO_SIZE_Y - 1)
		{
			if((i_hero <= num_hero / 2 && JumpHigher[0]) || (i_hero > num_hero / 2 && JumpHigher[1]))
				m_hero[i_hero].pos.y += m_hero[i_hero].vel.y = - 2 * HERO_VEL_Y;
			else
				m_hero[i_hero].pos.y += m_hero[i_hero].vel.y = - HERO_VEL_Y;
			if_jump[i_hero] = 0;
		}
		if(m_hero[i_hero].pos.y + HERO_SIZE_Y < max_of_height)
			m_hero[i_hero].vel.y += g_y;
		else if(m_hero[i_hero].pos.y + HERO_SIZE_Y > max_of_height)
		{
			if(m_hero[i_hero].pos.y + HERO_SIZE_Y < max_of_height + 3 + m_hero[i_hero].vel.y)
			{
				m_hero[i_hero].pos.y = max_of_height - HERO_SIZE_Y;
				m_hero[i_hero].vel.y = 0;
			}
			else
			{
				m_hero[i_hero].pos.x -= m_hero[i_hero].vel.x;
				m_hero[i_hero].pos.y -= m_hero[i_hero].vel.y;
				m_hero[i_hero].vel.x = 0;
				m_hero[i_hero].vel.y = 0;
				GetHighest();
				if(m_hero[i_hero].pos.y + HERO_SIZE_Y > max_of_height)
					m_hero[i_hero].pos.y = max_of_height - HERO_SIZE_Y;
			}
		}
		if(m_hero[i_hero].pos.y + HERO_SIZE_Y >= SeaLevel)
			m_hero[i_hero].NowBleed = 0;
		if(m_hero[i_hero].NowBleed <= 0)
		{
			m_hero[i_hero].NowBleed = 0;
			HeroDie(i_hero);
		}
		if(if_pressing_ad[i_hero] && !m_hero[i_hero].vel.x && m_hero[i_hero].vel.y < 0 && m_hero[i_hero].pos.y + m_hero[i_hero].vel.y + HERO_SIZE_Y < Terrianheight[m_hero[i_hero].pos.x + if_pressing_ad[i_hero] * HERO_VEL_X].y + HERO_SIZE_X * (1 - m_hero[i_hero].position))
		{
			m_hero[i_hero].vel.x = if_pressing_ad[i_hero] * HERO_VEL_X;
		}
	}

	//医药箱下落
	for(i_AidCase = 0; i_AidCase < num_AidCase && num_alive[1] && num_alive[2]; i_AidCase++)
	{
		if(m_AidCase[i_AidCase].exist)
		{
			max_of_height = Terrianheight[m_AidCase[i_AidCase].pos.x].y;
			for(temp_compare = 1; temp_compare <= AidCaseSize_X; temp_compare++)
				max_of_height = max_of_height < Terrianheight[m_AidCase[i_AidCase].pos.x  + temp_compare].y ? max_of_height : Terrianheight[m_AidCase[i_AidCase].pos.x  + temp_compare].y;
			m_AidCase[i_AidCase].pos.y = max_of_height - AidCaseSize_Y;
			for(i_hero = 1; i_hero <= num_hero; i_hero++)
			{
				if(m_hero[i_hero].pos.x + HERO_SIZE_X >= m_AidCase[i_AidCase].pos.x && m_hero[i_hero].pos.x <= m_AidCase[i_AidCase].pos.x + AidCaseSize_X && m_hero[i_hero].pos.y + HERO_SIZE_Y >= m_AidCase[i_AidCase].pos.y && m_hero[i_hero].pos.y <= m_AidCase[i_AidCase].pos.y + AidCaseSize_Y && m_hero[i_hero].NowBleed > 0)
				{
					if(m_hero[i_hero].NowBleed + m_AidCase[i_AidCase].Power <= 160)
						m_hero[i_hero].NowBleed += (No_Hero_Dec[i_hero][1] = m_AidCase[i_AidCase].Power);
					else
						m_hero[i_hero].NowBleed += (No_Hero_Dec[i_hero][1] = 160 - m_hero[i_hero].NowBleed);
					No_Hero_Dec[i_hero][0] = 50;
					m_AidCase[i_AidCase].exist = FALSE;
					break;
				}
			}
			if(m_AidCase[i_AidCase].pos.y + AidCaseSize_Y >= SeaLevel)
				m_AidCase[i_AidCase].exist = FALSE;
		}
	}
	//医药箱下落结束

	//瞄准箱下落
	for(i_TargetCase = 0; i_TargetCase < num_TargetCase && num_alive[1] && num_alive[2]; i_TargetCase++)
	{
		if(m_TargetCase[i_TargetCase].exist)
		{
			max_of_height = Terrianheight[m_TargetCase[i_TargetCase].pos.x].y;
			for(temp_compare = 1; temp_compare <= TargetCaseSize_X; temp_compare++)
				max_of_height = max_of_height < Terrianheight[m_TargetCase[i_TargetCase].pos.x  + temp_compare].y ? max_of_height : Terrianheight[m_TargetCase[i_TargetCase].pos.x  + temp_compare].y;
			m_TargetCase[i_TargetCase].pos.y = max_of_height - TargetCaseSize_Y;
			for(i_hero = 1; i_hero <= num_hero; i_hero++)
			{
				if(m_hero[i_hero].pos.x + HERO_SIZE_X >= m_TargetCase[i_TargetCase].pos.x && m_hero[i_hero].pos.x <= m_TargetCase[i_TargetCase].pos.x + TargetCaseSize_X && m_hero[i_hero].pos.y + HERO_SIZE_Y >= m_TargetCase[i_TargetCase].pos.y && m_hero[i_hero].pos.y <= m_TargetCase[i_TargetCase].pos.y + TargetCaseSize_Y && m_hero[i_hero].NowBleed > 0)
				{
					if(m_TargetCase[i_TargetCase].type == DFrontSightCase)
					{
						if(i_hero <= num_hero / 2)
							ShowTrack[0] = TRUE;
						else
							ShowTrack[1] = TRUE;
						m_TargetCase[i_TargetCase].exist = FALSE;
					}
					else if(m_TargetCase[i_TargetCase].type == DBoomCase)
					{
						boom(m_TargetCase[i_TargetCase].pos.x + TargetCaseSize_X / 2, m_TargetCase[i_TargetCase].pos.y + TargetCaseSize_Y / 2, 40, 40);
						m_TargetCase[i_TargetCase].exist = FALSE;
					}
					else if(m_TargetCase[i_TargetCase].type == DSpringCase)
					{
						m_hero[i_hero].vel.y -= 30;
						m_TargetCase[i_TargetCase].exist = FALSE;
						{
							mciSendString(_T("stop myMySpring"), NULL, 0, NULL);
							mciSendString(_T("close myMySpring"), NULL, 0, NULL);
							mciSendString(mcicmdMySpring, NULL,0, NULL);
							mciSendString(_T("play myMySpring"), NULL, 0, NULL);
						}
					}
					else if(m_TargetCase[i_TargetCase].type == DJumpCase)
					{
						if(i_hero <= num_hero / 2)
							JumpHigher[0] = TRUE;
						else
							JumpHigher[1] = TRUE;
						m_TargetCase[i_TargetCase].exist = FALSE;
					}
					break;
				}
			}
			if(m_TargetCase[i_TargetCase].pos.y + TargetCaseSize_Y >= SeaLevel)
				m_TargetCase[i_TargetCase].exist = FALSE;
		}
	}
	//瞄准箱下落结束
}

BOOL ButtonClicked(POINT ptMouse, POINT buttonPos, SIZE buttonSize)		//按下按钮
{
	RECT rectButton;
	rectButton.left = buttonPos.x;
	rectButton.top = buttonPos.y;
	rectButton.right = buttonPos.x + buttonSize.cx;
	rectButton.bottom = buttonPos.y + buttonSize.cy;
	return PtInRect(&rectButton, ptMouse);
}

VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)		//按下键盘
{
	if(Controlable && !wait && Game_Type != 2)
	{
	switch (wParam)
	{
	case VK_RETURN:
		if_jump[now_hero] = 1;
		break;
	case 'A':
		if(Status_OF_Game < 3)
			m_hero[now_hero].vel.x = -HERO_VEL_X;
		if_pressing_ad[now_hero] = -1;
		if(m_hero[now_hero].position == 0)
		{
			m_hero[now_hero].hBmp = m_hHeroLeftBmp;
			m_hero[now_hero].position = 1;
			m_hero[now_hero].pos.x -= HERO_SIZE_X / 4;
			if_ChangeDirection = 1;
		}
		break;
	case 'D':
		if(Status_OF_Game < 3)
			m_hero[now_hero].vel.x = HERO_VEL_X;
		if_pressing_ad[now_hero] = 1;
		if(m_hero[now_hero].position == 1)
		{
			m_hero[now_hero].hBmp = m_hHeroBmp;
			m_hero[now_hero].position = 0;
			m_hero[now_hero].pos.x += HERO_SIZE_X / 4;
			if_ChangeDirection = 1;
		}
		break;
	case VK_TAB:
		Now_Hero_Change();
		break;
	case VK_SPACE:
		if(Status_OF_Game == 0)
		{
			Status_OF_Game = 1;
		}
		if(Status_OF_Game == 1)
		{
			power_add[now_hero] = 4;
		}
		break;
	case 'W':
		if(Status_OF_Game == 0)
		{
			Degree_Of_Firing += 3;
			if(Degree_Of_Firing > 90)
				Degree_Of_Firing = 90;
		}
		break;
	case 'S':
		if(Status_OF_Game == 0)
		{
			Degree_Of_Firing -= 3;
			if(Degree_Of_Firing < -60)
				Degree_Of_Firing = -60;
		}
		break;
	default:
		break;
	}
	}
	else if(Game_Type == 2)
	{
		if(GuideModeStatus == 2 && wParam == 'A')
		{
			if(temp_i_GUIDE == -1)
				temp_i_GUIDE = 3;
			else if(temp_i_GUIDE > 0)
				temp_i_GUIDE--;
			else
			{
				GuideModeStatus = 3;
				m_hero[now_hero].vel.x = 0;
				if_pressing_ad[now_hero] = FALSE;
			}
			m_hero[now_hero].vel.x = -HERO_VEL_X;
			if_pressing_ad[now_hero] = -1;
			if(m_hero[now_hero].position == 0)
			{
				m_hero[now_hero].hBmp = m_hHeroLeftBmp;
				m_hero[now_hero].position = 1;
				m_hero[now_hero].pos.x -= HERO_SIZE_X / 4;
				if_ChangeDirection = 1;
			}
		}
		else if(GuideModeStatus == 6 && wParam == 'A')
		{
			m_hero[now_hero].vel.x = -HERO_VEL_X;
			if_pressing_ad[now_hero] = -1;
			if(m_hero[now_hero].position == 0)
			{
				m_hero[now_hero].hBmp = m_hHeroLeftBmp;
				m_hero[now_hero].position = 1;
				m_hero[now_hero].pos.x -= HERO_SIZE_X / 4;
				if_ChangeDirection = 1;
			}
		}
		else if(GuideModeStatus == 7 && wParam == 'D')
		{
			m_hero[now_hero].vel.x = +HERO_VEL_X;
			if_pressing_ad[now_hero] = 1;
			if(m_hero[now_hero].position == 1)
			{
				m_hero[now_hero].hBmp = m_hHeroBmp;
				m_hero[now_hero].position = 0;
				m_hero[now_hero].pos.x += HERO_SIZE_X / 4;
				if_ChangeDirection = 1;
			}
		}
		else if(GuideModeStatus == 9 && wParam == 'W')
		{
			Degree_Of_Firing += 3;
			if(Degree_Of_Firing > 90)
				Degree_Of_Firing = 90;
		}
		else if(GuideModeStatus == 9 && wParam == 'S')
		{
			Degree_Of_Firing -= 3;
			if(Degree_Of_Firing < -60)
				Degree_Of_Firing = -60;
		}
		else if(GuideModeStatus == 9 && wParam == VK_SPACE)
		{
			if(Status_OF_Game == 0)
			{
				Status_OF_Game = 1;
			}
			if(Status_OF_Game == 1)
			{
				power_add[now_hero] = 4;
			}
		}
	}
}

VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)		//松开键盘
{
	if(Controlable && !wait && Game_Type != 2)
	{
	switch (wParam)
	{
	case VK_RETURN:
		if_jump[now_hero] = 0;
		break;
	case 'A':
		m_hero[now_hero].vel.x = 0;
		if_pressing_ad[now_hero] = 0;
		break;
	case 'D':
		m_hero[now_hero].vel.x = 0;
		if_pressing_ad[now_hero] = 0;
		break;
	case VK_SPACE:
		power_add[now_hero] = 0;
		if(Status_OF_Game == 1)
		{
			Status_OF_Game = 2;
		}
		break;
	case VK_SHIFT:
		if(Status_OF_Game == 0)
		{
			now_Weapon = (now_Weapon + 1) % num_weapon;
			Take_Weapon();
			mciSendString(_T("stop myChangeWeapon"), NULL, 0, NULL);
			mciSendString(_T("close myChangeWeapon"), NULL, 0, NULL);
			mciSendString(mcicmdChangeWeapon, NULL,0, NULL);
			mciSendString(_T("play myChangeWeapon"), NULL, 0, NULL);
		}
		break;
	default:
		break;
	}
	}
	else if(Game_Type == 2)
	{
		if(GuideModeStatus == 0 && wParam == VK_SPACE)
			GuideModeStatus = 1;
		else if(GuideModeStatus == 2 && wParam == 'A')
		{
			GuideModeStatus = 3;
			m_hero[now_hero].vel.x = 0;
			if_pressing_ad[now_hero] = 0;
		}
		else if(GuideModeStatus == 3 && wParam == VK_RETURN)
		{
			temp_i = 0;
			GuideModeStatus = 4;
			m_hero[1].vel.y -= HERO_VEL_Y;
		}
		else if(GuideModeStatus == 4 && wParam == VK_SHIFT)
		{
			temp_i++;
			now_Weapon = (now_Weapon + 1) % num_weapon;
			Take_Weapon();
			mciSendString(_T("stop myChangeWeapon"), NULL, 0, NULL);
			mciSendString(_T("close myChangeWeapon"), NULL, 0, NULL);
			mciSendString(mcicmdChangeWeapon, NULL,0, NULL);
			mciSendString(_T("play myChangeWeapon"), NULL, 0, NULL);
			if(temp_i >= 4)
				GuideModeStatus = 5;
		}
		else if(GuideModeStatus == 5 && wParam == VK_SPACE)
		{
			GuideModeStatus = 6;
			m_AidCase[1].exist = TRUE;
			m_AidCase[1].pos.x = m_hero[1].pos.x - 24;
			m_AidCase[1].pos.y = m_hero[1].pos.y + HERO_SIZE_Y - AidCaseSize_Y - 5;
			m_AidCase[1].Power = 25;
			m_AidCase[1].hBmp = m_hSCase_Aid;
			m_AidCase[2].exist = TRUE;
			m_AidCase[2].pos.x = m_hero[1].pos.x - 42;
			m_AidCase[2].pos.y = m_hero[1].pos.y + HERO_SIZE_Y - AidCaseSize_Y - 5;
			m_AidCase[2].Power = 50;
			m_AidCase[2].hBmp = m_hCase_Aid;
		}
		else if(GuideModeStatus == 6 && wParam == 'A')
		{
			m_hero[now_hero].vel.x = 0;
			if_pressing_ad[now_hero] = 0;
		}
		else if(GuideModeStatus == 7 && wParam == 'D')
		{
			m_hero[now_hero].vel.x = 0;
			if_pressing_ad[now_hero] = 0;
		}
		else if(GuideModeStatus == 8 && wParam == VK_SPACE)
			GuideModeStatus = 9;
		else if(GuideModeStatus == 9 && wParam == VK_SPACE)
		{
			GuideModeStatus = 10;
			power_add[now_hero] = 0;
			Status_OF_Game = 2;
		}
		else if(GuideModeStatus == 10 && wParam == VK_SPACE)
		{
			gameStatus.status=0;
			if(rand() % 2)
				gameStatus.hBmp = m_hStartBack1;
			else
				gameStatus.hBmp = sBack2;
			Take_Weapon();
			Init_Again(hWnd, wParam, lParam);
			PlaySound((LPCTSTR)IDR_NotGaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
		}
	}
}

VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)		//判断鼠标点击哪里
{
	POINT ptMouse; // 鼠标点击的坐标
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);

	//换背景
	if (gameStatus.status == 1 && ptMouse.x >= WNDWIDTH - ChooseBackWidth - 30 && ptMouse.x <= WNDWIDTH - 30 && ptMouse.y >= 30 && ptMouse.y <= 30 + ChooseBackHeight)
	{
		mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
		mciSendString(_T("close myPressButton"), NULL, 0, NULL);
		mciSendString(mcicmdPressButton, NULL,0, NULL);
		mciSendString(_T("play myPressButton"), NULL, 0, NULL);
		if(gameStatus.hBmp == m_hBack1)
		{
			gameStatus.hBmp = m_hBack2;
			gameStatus.hS_Bmp = m_hS_Back2;
		}
		else if(gameStatus.hBmp == m_hBack2)
		{
			gameStatus.hBmp = m_hBack3;
			gameStatus.hS_Bmp = m_hS_Back3;
		}
		else
		{
			gameStatus.hBmp = m_hBack1;
			gameStatus.hS_Bmp = m_hSbmp_Back1;
		}
	}
	//换背景结束

	//游戏过程中退出游戏
	else if (gameStatus.status == 1 && ptMouse.x >= WNDWIDTH - ChooseBackWidth - 110 && ptMouse.x <= WNDWIDTH - 30 && ptMouse.y >= 30 && ptMouse.y <= 30 + ChooseBackHeight)
	{
		mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
		mciSendString(_T("close myPressButton"), NULL, 0, NULL);
		mciSendString(mcicmdPressButton, NULL,0, NULL);
		mciSendString(_T("play myPressButton"), NULL, 0, NULL);
		gameStatus.status=0;
		if(rand() % 2)
			gameStatus.hBmp = m_hStartBack1;
		else
			gameStatus.hBmp = sBack2;
		Take_Weapon();
		Init_Again(hWnd, wParam, lParam);
		PlaySound((LPCTSTR)IDR_NotGaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
	}
	//游戏过程中退出游戏结束

	//退出游戏
	else if (gameStatus.status==0 && ButtonClicked(ptMouse, m_gameMainQuitGameButton.pos, m_gameMainQuitGameButton.size))
	{
		mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
		mciSendString(_T("close myPressButton"), NULL, 0, NULL);
		mciSendString(mcicmdPressButton, NULL,0, NULL);
		mciSendString(_T("play myPressButton"), NULL, 0, NULL);
		deletetempfile();
		PostQuitMessage(0);
	}
	//退出游戏结束

	//游戏开始
	else if (gameStatus.status==0 && ButtonClicked(ptMouse, m_gameStartButton.pos, m_gameStartButton.size))
	{
		mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
		mciSendString(_T("close myPressButton"), NULL, 0, NULL);
		mciSendString(mcicmdPressButton, NULL,0, NULL);
		mciSendString(_T("play myPressButton"), NULL, 0, NULL);
		Game_Type = 1;
		SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
		gameStatus.status=6;
		if(gameStatus.hBmp == m_hStartBack1)
			gameStatus.hBmp = sBack2;
		else
			gameStatus.hBmp = m_hStartBack1;
		InvalidateRect(hWnd, NULL, TRUE);//第三个参数设为TRUE，重画整个画布。
	}
	else if (gameStatus.status==0 && ButtonClicked(ptMouse, m_gameDStartButton.pos, m_gameDStartButton.size))
	{
		mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
		mciSendString(_T("close myPressButton"), NULL, 0, NULL);
		mciSendString(mcicmdPressButton, NULL,0, NULL);
		mciSendString(_T("play myPressButton"), NULL, 0, NULL);
		Game_Type = 0;
		SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
		if(rand() % 2 == 0)
			PlaySound((LPCTSTR)IDR_Gaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
		else
			PlaySound((LPCTSTR)IDR_2Gaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
		gameStatus.status=1;
		if(rand() % 3 == 0)
		{
			gameStatus.hBmp = m_hBack1;
			gameStatus.hS_Bmp = m_hSbmp_Back1;
		}
		else if(rand() % 3 == 1)
		{
			gameStatus.hBmp = m_hBack2;
			gameStatus.hS_Bmp = m_hS_Back2;
		}
		else
		{
			gameStatus.hBmp = m_hBack3;
			gameStatus.hS_Bmp = m_hS_Back3;
		}
		InvalidateRect(hWnd, NULL, TRUE);//第三个参数设为TRUE，重画整个画布。
	}
	else if (gameStatus.status==0 && ButtonClicked(ptMouse, m_gameGStartButton.pos, m_gameGStartButton.size))
	{
		mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
		mciSendString(_T("close myPressButton"), NULL, 0, NULL);
		mciSendString(mcicmdPressButton, NULL,0, NULL);
		mciSendString(_T("play myPressButton"), NULL, 0, NULL);
		Game_Type = 2;
		SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
		if(rand() % 2 == 0)
			PlaySound((LPCTSTR)IDR_Gaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
		else
			PlaySound((LPCTSTR)IDR_2Gaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
		gameStatus.status=7;
		if(rand() % 3 == 0)
		{
			gameStatus.hBmp = m_hBack1;
			gameStatus.hS_Bmp = m_hSbmp_Back1;
		}
		else if(rand() % 3 == 1)
		{
			gameStatus.hBmp = m_hBack2;
			gameStatus.hS_Bmp = m_hS_Back2;
		}
		else
		{
			gameStatus.hBmp = m_hBack3;
			gameStatus.hS_Bmp = m_hS_Back3;
		}
		InvalidateRect(hWnd, NULL, TRUE);//第三个参数设为TRUE，重画整个画布。
	}
	else if (gameStatus.status==0 && ButtonClicked(ptMouse, m_gameHelpButton.pos, m_gameHelpButton.size))
	{
		mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
		mciSendString(_T("close myPressButton"), NULL, 0, NULL);
		mciSendString(mcicmdPressButton, NULL,0, NULL);
		mciSendString(_T("play myPressButton"), NULL, 0, NULL);
		gameStatus.status=5;
		gameStatus.hBmp = m_hFormHelp;
		InvalidateRect(hWnd, NULL, TRUE);
	}
	else if ((gameStatus.status == 2 && ButtonClicked(ptMouse, m_gameBalanceButton.pos, m_gameBalanceButton.size)) || (gameStatus.status == 3 && ButtonClicked(ptMouse, m_gameRedWinButton.pos, m_gameRedWinButton.size)) || (gameStatus.status == 4 && ButtonClicked(ptMouse, m_gameYellowWinButton.pos, m_gameYellowWinButton.size)) || (gameStatus.status >= 2 && gameStatus.status <= 4 && ButtonClicked(ptMouse, m_gameRestartButton.pos, m_gameRestartButton.size)))
	{
		mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
		mciSendString(_T("close myPressButton"), NULL, 0, NULL);
		mciSendString(mcicmdPressButton, NULL,0, NULL);
		mciSendString(_T("play myPressButton"), NULL, 0, NULL);
		if(rand() % 2 == 0)
			PlaySound((LPCTSTR)IDR_Gaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
		else
			PlaySound((LPCTSTR)IDR_2Gaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
		gameStatus.status=1;
		if(rand() % 3 == 0)
		{
			gameStatus.hBmp = m_hBack1;
			gameStatus.hS_Bmp = m_hSbmp_Back1;
		}
		else if(rand() % 3 == 1)
		{
			gameStatus.hBmp = m_hBack2;
			gameStatus.hS_Bmp = m_hS_Back2;
		}
		else
		{
			gameStatus.hBmp = m_hBack3;
			gameStatus.hS_Bmp = m_hS_Back3;
		}
		Take_Weapon();
		Init_Again(hWnd, wParam, lParam);
		Render(hWnd);
	}
	else if (gameStatus.status >= 2 && gameStatus.status <= 4 && ButtonClicked(ptMouse, m_gameQuitButton.pos, m_gameQuitButton.size))
	{
		mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
		mciSendString(_T("close myPressButton"), NULL, 0, NULL);
		mciSendString(mcicmdPressButton, NULL,0, NULL);
		mciSendString(_T("play myPressButton"), NULL, 0, NULL);
		gameStatus.status=0;
		if(rand() % 2)
			gameStatus.hBmp = m_hStartBack1;
		else
			gameStatus.hBmp = sBack2;
		Take_Weapon();
		Init_Again(hWnd, wParam, lParam);
		//为什么必须要再次创建？！？！？！？！？？！？！？！？？！？！？！？！？！？！？！？！？！


		m_gameStartButton = CreateGameButton(WNDWIDTH / 2 - GAME_START_BUTTON_SIZE_X / 2, WNDHEIGHT / 2 - GAME_START_BUTTON_SIZE_Y / 2, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hGameStartButtonBmp, 0);

		//为什么必须要再次创建？！？！？！？！？？！？！？！？？！？！？！？！？！？！？！？！？！
		Render(hWnd);
	}
	else if (gameStatus.status == 5 && ButtonClicked(ptMouse, m_gameReturnButton.pos, m_gameReturnButton.size))
	{
		mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
		mciSendString(_T("close myPressButton"), NULL, 0, NULL);
		mciSendString(mcicmdPressButton, NULL,0, NULL);
		mciSendString(_T("play myPressButton"), NULL, 0, NULL);
		gameStatus.status=0;
		if(rand() % 2)
			gameStatus.hBmp = m_hStartBack1;
		else
			gameStatus.hBmp = sBack2;
		Take_Weapon();
		Init_Again(hWnd, wParam, lParam);
		//为什么必须要再次创建？！？！？！？！？？！？！？！？？！？！？！？！？！？！？！？！？！


		m_gameStartButton = CreateGameButton(WNDWIDTH / 2 - GAME_START_BUTTON_SIZE_X / 2, WNDHEIGHT / 2 - GAME_START_BUTTON_SIZE_Y / 2, GAME_START_BUTTON_SIZE_X, GAME_START_BUTTON_SIZE_Y, m_hGameStartButtonBmp, 0);

		//为什么必须要再次创建？！？！？！？！？？！？！？！？？！？！？！？！？！？！？！？！？！
		Render(hWnd);
	}
	else if (gameStatus.status==6 && ButtonClicked(ptMouse, m_gameEasyButton.pos, m_gameEasyButton.size))
	{
		mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
		mciSendString(_T("close myPressButton"), NULL, 0, NULL);
		mciSendString(mcicmdPressButton, NULL,0, NULL);
		mciSendString(_T("play myPressButton"), NULL, 0, NULL);
		if(rand() % 2 == 0)
			PlaySound((LPCTSTR)IDR_Gaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
		else
			PlaySound((LPCTSTR)IDR_2Gaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
		AI_Type = 0;
		SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
		gameStatus.status=1;
		if(rand() % 3 == 0)
		{
			gameStatus.hBmp = m_hBack1;
			gameStatus.hS_Bmp = m_hSbmp_Back1;
		}
		else if(rand() % 3 == 1)
		{
			gameStatus.hBmp = m_hBack2;
			gameStatus.hS_Bmp = m_hS_Back2;
		}
		else
		{
			gameStatus.hBmp = m_hBack3;
			gameStatus.hS_Bmp = m_hS_Back3;
		}
		InvalidateRect(hWnd, NULL, TRUE);//第三个参数设为TRUE，重画整个画布。
	}
	else if (gameStatus.status==6 && ButtonClicked(ptMouse, m_gameNormalButton.pos, m_gameNormalButton.size))
	{
		mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
		mciSendString(_T("close myPressButton"), NULL, 0, NULL);
		mciSendString(mcicmdPressButton, NULL,0, NULL);
		mciSendString(_T("play myPressButton"), NULL, 0, NULL);
		if(rand() % 2 == 0)
			PlaySound((LPCTSTR)IDR_Gaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
		else
			PlaySound((LPCTSTR)IDR_2Gaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
		AI_Type = 1;
		SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
		gameStatus.status=1;
		if(rand() % 3 == 0)
		{
			gameStatus.hBmp = m_hBack1;
			gameStatus.hS_Bmp = m_hSbmp_Back1;
		}
		else if(rand() % 3 == 1)
		{
			gameStatus.hBmp = m_hBack2;
			gameStatus.hS_Bmp = m_hS_Back2;
		}
		else
		{
			gameStatus.hBmp = m_hBack3;
			gameStatus.hS_Bmp = m_hS_Back3;
		}
		InvalidateRect(hWnd, NULL, TRUE);//第三个参数设为TRUE，重画整个画布。
	}
	else if (gameStatus.status==6 && ButtonClicked(ptMouse, m_gameHardButton.pos, m_gameHardButton.size))
	{
		mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
		mciSendString(_T("close myPressButton"), NULL, 0, NULL);
		mciSendString(mcicmdPressButton, NULL,0, NULL);
		mciSendString(_T("play myPressButton"), NULL, 0, NULL);
		if(rand() % 2 == 0)
			PlaySound((LPCTSTR)IDR_Gaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
		else
			PlaySound((LPCTSTR)IDR_2Gaming,NULL,SND_RESOURCE|SND_ASYNC|SND_LOOP);
		AI_Type = 2;
		SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
		gameStatus.status=1;
		if(rand() % 3 == 0)
		{
			gameStatus.hBmp = m_hBack1;
			gameStatus.hS_Bmp = m_hSbmp_Back1;
		}
		else if(rand() % 3 == 1)
		{
			gameStatus.hBmp = m_hBack2;
			gameStatus.hS_Bmp = m_hS_Back2;
		}
		else
		{
			gameStatus.hBmp = m_hBack3;
			gameStatus.hS_Bmp = m_hS_Back3;
		}
		InvalidateRect(hWnd, NULL, TRUE);//第三个参数设为TRUE，重画整个画布。
	}
	else if (gameStatus.status==6 && ButtonClicked(ptMouse, m_gameSelectReturnButton.pos, m_gameSelectReturnButton.size))
	{
		mciSendString(_T("stop myPressButton"), NULL, 0, NULL);
		mciSendString(_T("close myPressButton"), NULL, 0, NULL);
		mciSendString(mcicmdPressButton, NULL,0, NULL);
		mciSendString(_T("play myPressButton"), NULL, 0, NULL);
		gameStatus.status=0;
		if(rand() % 2)
			gameStatus.hBmp = m_hStartBack1;
		else
			gameStatus.hBmp = sBack2;
		Take_Weapon();
		Init_Again(hWnd, wParam, lParam);
	}
}

INT RandCreateHeight(int Terrianheight_i, int num)		//后面一个随机生成地形的函数中的一部分
{
	int tempheight = rand() % 20;
	if(Terrianheight_i == -1)
		return rand() % (WNDHEIGHT / 4) + 2 * WNDHEIGHT / 4;
	else
	{
		if(Terrianheight_i == 0)
		{
			switch(tempheight)
			{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
					tempheight = 0;
					break;
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
					tempheight = 1;
					break;
				case 15:
				case 16:
				case 17:
				case 18:
					tempheight = 2;
					break;
				default:
					tempheight = 3;
			}
		}
		else if(Terrianheight_i < WNDHEIGHT / 4)
		{
			switch(tempheight)
			{
				case 0:
					tempheight = -2;
					break;
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
					tempheight = -1;
					break;
				case 6:
				case 7:
				case 8:
				case 9:
					tempheight = 0;
					break;
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
					tempheight = 1;
					break;
				case 15:
				case 16:
				case 17:
				case 18:
					tempheight = 2;
					break;
				default:
					tempheight = 3;
			}
		}
		else if(Terrianheight_i < WNDHEIGHT / 2)
		{
			switch(tempheight)
			{
				case 0:
				case 1:
				case 2:
					tempheight = -2;
					break;
				case 3:
				case 4:
				case 5:
				case 6:
					tempheight = -1;
					break;
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
					tempheight = 0;
					break;
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
					tempheight = 1;
					break;
				case 17:
				case 18:
				default:
					tempheight = 2;
			}
		}
		else if(Terrianheight_i < WNDHEIGHT * 3 / 4)
		{
			switch(tempheight)
			{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
					tempheight = -2;
					break;
				case 7:
				case 8:
					tempheight = -1;
					break;
				case 9:
				case 10:
					tempheight = 0;
					break;
				case 11:
				case 12:
				case 13:
					tempheight = 1;
					break;
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				default:
					tempheight = 2;
			}
		}
		else if(Terrianheight_i < WNDHEIGHT)
		{
			switch(tempheight)
			{
				case 0:
				case 1:
				case 2:
				case 3:
					tempheight = -2;
					break;
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
					tempheight = -1;
					break;
				case 9:
				case 10:
				case 11:
					tempheight = 0;
					break;
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
					tempheight = 1;
					break;
				default:
					tempheight = 2;
			}
		}
		else
		{
			switch(tempheight)
			{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
					tempheight = -2;
					break;
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
					tempheight = -1;
					break;
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				default:
					tempheight = 0;
					break;
			}
		}
		if(Terrianheight_i + tempheight * 3 > max)
			max = Terrianheight_i + tempheight * 3;
		if(Terrianheight_i + tempheight * 3 < min)
			min = Terrianheight_i + tempheight * 3;
		return Terrianheight_i + tempheight * 3;
	}
}

VOID DefineHeight()		//随机生成地形
{
	int p;
	Terrianheight[0].x = -1;
	Terrianheight[0].y = -1;
	for(p = 1; p <= WNDWIDTH * 2; p++)
	{
		Terrianheight[p].x = p - 1;
		Terrianheight[p].y = RandCreateHeight(Terrianheight[p - 1].y, p);
	}
	SeaLevel = max * 5 / 7 + min * 2 / 7;
	Terrianheight[0].y = WNDHEIGHT;
	Terrianheight[WNDWIDTH * 2].x = WNDWIDTH * 2;
	Terrianheight[WNDWIDTH * 2].y = WNDHEIGHT;
	Terrianheight[WNDWIDTH * 2 + 1].x = 0;
	Terrianheight[WNDWIDTH * 2 + 1].y = WNDHEIGHT;
	for(temp_new_try = 0; temp_new_try < (WNDWIDTH * 2 + 1) / resolution_ratio + 1; temp_new_try ++)
	{
		if((temp_new_try + 1) * resolution_ratio + 1 < WNDWIDTH * 2 + 2)
		{
			for(temp_new_try_p = 0; temp_new_try_p < resolution_ratio; temp_new_try_p++)
				Terrianheight[temp_new_try * resolution_ratio + 1 + temp_new_try_p].y = Terrianheight[temp_new_try * resolution_ratio + 1].y + temp_new_try_p * (Terrianheight[(temp_new_try + 1) * resolution_ratio + 1].y - Terrianheight[temp_new_try * resolution_ratio + 1].y) / resolution_ratio;
		}
		Terrianheight[WNDWIDTH * 2 + 1].x = WNDWIDTH * 2 - 1;
		Terrianheight[WNDWIDTH * 2 + 1].y = 0;
		Terrianheight[0].x = -1;
		Terrianheight[0].y = 0;
	}
}

VOID GetHighest()		//找到最高处，为了能够让虫子从土里出来
{
	if(!m_hero[i_hero].position)
	{
		max_of_height = Terrianheight[m_hero[i_hero].pos.x].y;
		for(temp_compare = 1; temp_compare <= HERO_SIZE_X * 3 / 4; temp_compare++)
			max_of_height = max_of_height < Terrianheight[m_hero[i_hero].pos.x  + temp_compare].y ? max_of_height : Terrianheight[m_hero[i_hero].pos.x  + temp_compare].y;
	}
	else
	{
		max_of_height = Terrianheight[m_hero[i_hero].pos.x + HERO_SIZE_X / 4].y;
		for(temp_compare = 1 +  HERO_SIZE_X / 4; temp_compare <= HERO_SIZE_X; temp_compare++)
			max_of_height = max_of_height < Terrianheight[m_hero[i_hero].pos.x  + temp_compare].y ? max_of_height : Terrianheight[m_hero[i_hero].pos.x  + temp_compare].y;
	}
}

VOID Hit(int Degree, int Power, int x, int y, int IF_LEFT, int vel0)	//发射出来了
{
	NOT_Firing_Me = TRUE;
	vel0 = vel0 * Power / 100;
	if(IF_LEFT)
		m_hero[0].vel.x = - (int)(vel0 * cos(Degree * Pi / 180));
	else
		m_hero[0].vel.x = (int)(vel0 * cos(Degree * Pi / 180));
	m_hero[0].vel.y = - (int)(vel0 * sin(Degree * Pi / 180));
	m_hero[0].pos.x = x;
	m_hero[0].pos.y = y;
	FirstBoom = TRUE;
	if_add_wind = 0;
	if(Hitting())
		Status_OF_Game = 4;
}

BOOL Hitting()		//在发射的过程中，返回1表示击中了
{
	Temp_Boom.x = m_hero[0].pos.x;
	Temp_Boom.y = m_hero[0].pos.y;
	m_hero[0].pos.x += m_hero[0].vel.x;
	m_hero[0].pos.y += m_hero[0].vel.y;
	if(WindSpeed == 1 || WindSpeed == -1)
	{
		if(!if_add_wind)
			m_hero[0].vel.x += WindSpeed;
		if_add_wind = (if_add_wind + 1) % 4;
	}
	else if(WindSpeed == 2 || WindSpeed == -2)
	{
		if(!if_add_wind)
			m_hero[0].vel.x += WindSpeed / 2;
		if_add_wind = (if_add_wind + 1) % 2;
	}
	else if(WindSpeed == 3 || WindSpeed == -3)
	{
		if(if_add_wind)
			m_hero[0].vel.x += WindSpeed / 3;
		if_add_wind = (if_add_wind + 1) % 4;
	}
	else if(WindSpeed == 4 || WindSpeed == -4)
	{
		m_hero[0].vel.x += WindSpeed / 4;
	}
	m_hero[0].vel.y += g_y;
	if(m_hero[0].vel.x > 0)
	{
		temp_tangent = (double)m_hero[0].vel.y / m_hero[0].vel.x;
		for(temp_boom_int = 0; temp_boom_int <= m_hero[0].vel.x; temp_boom_int++)
		{
			Temp_Boom_i.x = Temp_Boom.x + temp_boom_int;
			Temp_Boom_i.y = int(Temp_Boom.y + temp_boom_int * temp_tangent) + 1;
			if(Temp_Boom_i.x >= WNDWIDTH * 2 || Temp_Boom_i.x <= 0)
			{
				boom(Temp_Boom_i.x, Temp_Boom_i.y, Range_Of_Boom, Power_Of_Boom);
				return 1;
			}
			if(Terrianheight[Temp_Boom_i.x].y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius)
			{
				boom(Temp_Boom_i.x, Temp_Boom_i.y, Range_Of_Boom, Power_Of_Boom);
				return 1;
			}
			else
			{
				if(NOT_Firing_Me && (m_hero[now_hero].pos.x - 10 > Temp_Boom_i.x + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.x + HERO_SIZE_X + 10 < Temp_Boom_i.x - m_Weapon[now_Weapon].Radius) && (m_hero[now_hero].pos.y - 10 > Temp_Boom_i.y + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.y + HERO_SIZE_Y + 10 < Temp_Boom_i.y - m_Weapon[now_Weapon].Radius))
			NOT_Firing_Me = FALSE;
				for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
				{
					if(!m_hero[i_hero].NowBleed)
						continue;
					if(m_hero[i_hero].pos.x <= Temp_Boom_i.x + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.x + HERO_SIZE_X >= Temp_Boom_i.x - m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y + HERO_SIZE_Y >= Temp_Boom_i.y - m_Weapon[now_Weapon].Radius)
					{
						if(NOT_Firing_Me && (i_hero == now_hero || (Temp_Boom_i.x >= m_hero[now_hero].pos.x - 10 && Temp_Boom_i.x <= m_hero[now_hero].pos.x + HERO_SIZE_X + 10)))
							continue;
						else
						{
							boom(Temp_Boom_i.x, Temp_Boom_i.y, Range_Of_Boom, Power_Of_Boom);
							return 1;
						}
					}
				}
			}
		}
	}
	//后一段复制前一段，如果要改要都改
	else if(m_hero[0].vel.x < 0)
	{
		temp_tangent = (double)m_hero[0].vel.y / m_hero[0].vel.x;
		for(temp_boom_int = 0; temp_boom_int >= m_hero[0].vel.x; temp_boom_int--)
		{
			Temp_Boom_i.x = Temp_Boom.x + temp_boom_int;
			Temp_Boom_i.y = int(Temp_Boom.y + temp_boom_int * temp_tangent) + 1;
			if(Temp_Boom_i.x >= WNDWIDTH * 2 || Temp_Boom_i.x <= 0)
			{
				boom(Temp_Boom_i.x, Temp_Boom_i.y, Range_Of_Boom, Power_Of_Boom);
				return 1;
			}
			if(Terrianheight[Temp_Boom_i.x].y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius)
			{
				boom(Temp_Boom_i.x, Temp_Boom_i.y, Range_Of_Boom, Power_Of_Boom);
				return 1;
			}
			else
			{
				if(NOT_Firing_Me && (m_hero[now_hero].pos.x - 10 > Temp_Boom_i.x + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.x + HERO_SIZE_X + 10 < Temp_Boom_i.x - m_Weapon[now_Weapon].Radius) && (m_hero[now_hero].pos.y - 10 > Temp_Boom_i.y + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.y + HERO_SIZE_Y + 10 < Temp_Boom_i.y - m_Weapon[now_Weapon].Radius))
			NOT_Firing_Me = FALSE;
				for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
				{
					if(!m_hero[i_hero].NowBleed)
						continue;
					if(m_hero[i_hero].pos.x <= Temp_Boom_i.x + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.x + HERO_SIZE_X >= Temp_Boom_i.x - m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y + HERO_SIZE_Y >= Temp_Boom_i.y - m_Weapon[now_Weapon].Radius)
					{
						if(NOT_Firing_Me && (i_hero == now_hero || (Temp_Boom_i.x >= m_hero[now_hero].pos.x - 10 && Temp_Boom_i.x <= m_hero[now_hero].pos.x + HERO_SIZE_X + 10)))
							continue;
						else
						{
							boom(Temp_Boom_i.x, Temp_Boom_i.y, Range_Of_Boom, Power_Of_Boom);
							return 1;
						}
					}
				}
			}
		}
	}
	else
	{
		Temp_Boom_i.x = Temp_Boom.x;
		if(m_hero[0].vel.y <= 0)
		{
		for(temp_boom_int = 0; temp_boom_int >= m_hero[0].vel.y; temp_boom_int--)
		{
			Temp_Boom_i.y = Temp_Boom.y + temp_boom_int + 1;
			if(Temp_Boom_i.x >= WNDWIDTH * 2 || Temp_Boom_i.x <= 0)
			{
				boom(Temp_Boom_i.x, Temp_Boom_i.y, Range_Of_Boom, Power_Of_Boom);
				return 1;
			}
			if(Terrianheight[Temp_Boom_i.x].y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius)
			{
				boom(Temp_Boom_i.x, Temp_Boom_i.y, Range_Of_Boom, Power_Of_Boom);
				return 1;
			}
			else
			{
				if(NOT_Firing_Me && (m_hero[now_hero].pos.x - 10 > Temp_Boom_i.x + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.x + HERO_SIZE_X + 10 < Temp_Boom_i.x - m_Weapon[now_Weapon].Radius) && (m_hero[now_hero].pos.y - 10 > Temp_Boom_i.y + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.y + HERO_SIZE_Y + 10 < Temp_Boom_i.y - m_Weapon[now_Weapon].Radius))
			NOT_Firing_Me = FALSE;
				for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
				{
					if(!m_hero[i_hero].NowBleed)
						continue;
					if(m_hero[i_hero].pos.x <= Temp_Boom_i.x + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.x + HERO_SIZE_X >= Temp_Boom_i.x - m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y + HERO_SIZE_Y >= Temp_Boom_i.y - m_Weapon[now_Weapon].Radius)
					{
						if(NOT_Firing_Me && (i_hero == now_hero || (Temp_Boom_i.x >= m_hero[now_hero].pos.x - 10 && Temp_Boom_i.x <= m_hero[now_hero].pos.x + HERO_SIZE_X + 10)))
							continue;
						else
						{
							boom(Temp_Boom_i.x, Temp_Boom_i.y, Range_Of_Boom, Power_Of_Boom);
							return 1;
						}
					}
				}
			}
		}
		}
		else
		{
		for(temp_boom_int = 0; temp_boom_int <= m_hero[0].vel.y; temp_boom_int++)
		{
			Temp_Boom_i.y = Temp_Boom.y + temp_boom_int + 1;
			if(Temp_Boom_i.x >= WNDWIDTH * 2 || Temp_Boom_i.x <= 0)
			{
				boom(Temp_Boom_i.x, Temp_Boom_i.y, Range_Of_Boom, Power_Of_Boom);
				return 1;
			}
			if(Terrianheight[Temp_Boom_i.x].y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius)
			{
				boom(Temp_Boom_i.x, Temp_Boom_i.y, Range_Of_Boom, Power_Of_Boom);
				return 1;
			}
			else
			{
				if(NOT_Firing_Me && (m_hero[now_hero].pos.x - 10 > Temp_Boom_i.x + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.x + HERO_SIZE_X + 10 < Temp_Boom_i.x - m_Weapon[now_Weapon].Radius) && (m_hero[now_hero].pos.y - 10 > Temp_Boom_i.y + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.y + HERO_SIZE_Y + 10 < Temp_Boom_i.y - m_Weapon[now_Weapon].Radius))
			NOT_Firing_Me = FALSE;
				for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
				{
					if(!m_hero[i_hero].NowBleed)
						continue;
					if(m_hero[i_hero].pos.x <= Temp_Boom_i.x + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.x + HERO_SIZE_X >= Temp_Boom_i.x - m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y + HERO_SIZE_Y >= Temp_Boom_i.y - m_Weapon[now_Weapon].Radius)
					{
						if(NOT_Firing_Me && (i_hero == now_hero || (Temp_Boom_i.x >= m_hero[now_hero].pos.x - 10 && Temp_Boom_i.x <= m_hero[now_hero].pos.x + HERO_SIZE_X + 10)))
							continue;
						else
						{
							boom(Temp_Boom_i.x, Temp_Boom_i.y, Range_Of_Boom, Power_Of_Boom);
							return 1;
						}
					}
				}
			}
		}
		}
	}
	return 0;
}

VOID CreateAidCase()		//创建医疗箱
{
	temp_i = 0;
 	for(i_AidCase = 0; i_AidCase < num_AidCase && m_AidCase[i_AidCase].exist; i_AidCase++);
	if(i_AidCase < num_AidCase)
	{
		temp_createAidCase = 1 + rand() % (WNDWIDTH * 2 - AidCaseSize_X - 2);
		do
		{
			temp_i++;
			for(i_hero = 1; i_hero <= num_hero && (m_hero[i_hero].NowBleed <= 0 || (m_hero[i_hero].pos.x + HERO_SIZE_X < temp_createAidCase - 10 || m_hero[i_hero].pos.x > temp_createAidCase + AidCaseSize_X + 10)); i_hero++);
			if(i_hero > num_hero)
				break;
			temp_createAidCase = (temp_createAidCase + 25) % (WNDWIDTH * 2 - AidCaseSize_X - 2) + 1;
		}
		while(Terrianheight[temp_createAidCase].y > SeaLevel - 3 && temp_i < WNDWIDTH * 2 / 25);
		if(temp_i < WNDWIDTH * 2 / 25 && Terrianheight[temp_createAidCase].y <= SeaLevel - 3 && temp_i < WNDWIDTH * 2 / 25)
		{
			if(rand() % 5 == 0)
			{
				m_AidCase[i_AidCase].Power = 50;
				m_AidCase[i_AidCase].hBmp = m_hCase_Aid;
			}
			else
			{
				m_AidCase[i_AidCase].Power = 25;
				m_AidCase[i_AidCase].hBmp = m_hSCase_Aid;
			}
			m_AidCase[i_AidCase].exist = TRUE;
			m_AidCase[i_AidCase].pos.x = temp_createAidCase;
			m_AidCase[i_AidCase].pos.y = -100;
		}
	}
}

VOID CreateTargetCase()		//创建瞄准箱
{
	temp_i = 0;
 	for(i_TargetCase = 0; i_TargetCase < num_TargetCase && m_TargetCase[i_TargetCase].exist; i_TargetCase++);
	if(i_TargetCase < num_TargetCase)
	{
		temp_createTargetCase = 1 + rand() % (WNDWIDTH * 2 - TargetCaseSize_X - 2);
		do
		{
			temp_i++;
			for(i_hero = 1; i_hero <= num_hero && (m_hero[i_hero].NowBleed <= 0 || (m_hero[i_hero].pos.x + HERO_SIZE_X < temp_createTargetCase - 10 || m_hero[i_hero].pos.x > temp_createTargetCase + TargetCaseSize_X + 10)); i_hero++);
			if(i_hero > num_hero)
				break;
			temp_createTargetCase = (temp_createTargetCase + 25) % (WNDWIDTH * 2 - TargetCaseSize_X - 2) + 1;
		}
		while(Terrianheight[temp_createTargetCase].y > SeaLevel - 3 && temp_i < WNDWIDTH * 2 / 25);
		if(temp_i < WNDWIDTH * 2 / 25 && Terrianheight[temp_createTargetCase].y <= SeaLevel - 3 && temp_i < WNDWIDTH * 2 / 25)
		{
			switch(rand() % 4)
			{
				case 0:
					m_TargetCase[i_TargetCase].type = DFrontSightCase;
					m_TargetCase[i_TargetCase].hBmp = hC_Target;
					break;
				case 1:
					m_TargetCase[i_TargetCase].type = DJumpCase;
					m_TargetCase[i_TargetCase].hBmp = hDJumpCase;
					break;
				case 2:
					m_TargetCase[i_TargetCase].type = DBoomCase;
					m_TargetCase[i_TargetCase].hBmp = hDBoomCase;
					break;
				default:
					m_TargetCase[i_TargetCase].type = DSpringCase;
					m_TargetCase[i_TargetCase].hBmp = hDSpringCase;
					break;
			}
			m_TargetCase[i_TargetCase].exist = TRUE;
			m_TargetCase[i_TargetCase].pos.x = temp_createTargetCase;
			m_TargetCase[i_TargetCase].pos.y = -100;
		}
	}
}

VOID CreateWindSpeed()		//生成风速
{
	WindSpeed = rand() % 20;
	switch(WindSpeed)
	{
		case 0:
			WindSpeed = -4;
			break;
		case 1:
		case 2:
		case 3:
			WindSpeed = -3;
			break;
		case 4:
		case 5:
		case 6:
			WindSpeed = -2;
			break;
		case 7:
		case 8:
			WindSpeed = -1;
			break;
		case 9:
		case 10:
			WindSpeed = 0;
			break;
		case 11:
		case 12:
			WindSpeed = 1;
			break;
		case 13:
		case 14:
		case 15:
			WindSpeed = 2;
			break;
		case 16:
		case 17:
		case 18:
			WindSpeed = 3;
			break;
		default:
			WindSpeed = 4;
			break;
	}
}

VOID AI(int i)		//人工智能AI
{
	int D_x, D_y;
	switch(i)
	{
		case 0:


	Controlable = FALSE;
	for(AI_Aim = 1; AI_Aim <= num_hero / 2; AI_Aim++)
	{
		if(m_hero[AI_Aim].NowBleed <= 0)
			continue;
		if(m_hero[AI_Aim].pos.x < m_hero[now_hero].pos.x)
		{
			m_hero[now_hero].hBmp = m_hHeroLeftBmp;
			m_hero[now_hero].position = 1;
		}
		else
		{
			m_hero[now_hero].hBmp = m_hHeroBmp;
			m_hero[now_hero].position = 0;	
		}
	
		D_x = m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x;
		if(D_x < 0)
		{
			for(temp_i_AI = 0; m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x + HERO_VEL_X * (temp_i_AI + 1) < -50 && Terrianheight[m_hero[now_hero].pos.x - HERO_VEL_X * (temp_i_AI + 1)].y < SeaLevel - 1; temp_i_AI++);
			D_x = -(m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x + HERO_VEL_X * (temp_i_AI + 1));
			temp_i_AI *= -1;
		}
		else
		{
			for(temp_i_AI = 0; m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x - HERO_VEL_X * (temp_i_AI + 1) > 50 && Terrianheight[m_hero[now_hero].pos.x + HERO_SIZE_X + HERO_VEL_X * (temp_i_AI + 1)].y < SeaLevel - 1; temp_i_AI++);
			D_x = m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x - HERO_VEL_X * (temp_i_AI + 1);
		}
		if(D_x > 0)
		{
			max_of_height = Terrianheight[m_hero[now_hero].pos.x + HERO_VEL_X * (temp_i_AI + 1)].y;
			for(temp_compare = 1; temp_compare <= HERO_SIZE_X * 3 / 4; temp_compare++)
				max_of_height = max_of_height < Terrianheight[m_hero[now_hero].pos.x + HERO_VEL_X * (temp_i_AI + 1) + temp_compare].y ? max_of_height : Terrianheight[m_hero[now_hero].pos.x + HERO_VEL_X * (temp_i_AI + 1) + temp_compare].y;
		}
		else if(D_x < 0)
		{
			max_of_height = Terrianheight[m_hero[now_hero].pos.x - HERO_VEL_X * (temp_i_AI + 1) + HERO_SIZE_X / 4].y;
			for(temp_compare = 1 +  HERO_SIZE_X / 4; temp_compare <= HERO_SIZE_X; temp_compare++)
				max_of_height = max_of_height < Terrianheight[m_hero[now_hero].pos.x - HERO_VEL_X * (temp_i_AI + 1) + temp_compare].y ? max_of_height : Terrianheight[m_hero[now_hero].pos.x - HERO_VEL_X * (temp_i_AI + 1) + temp_compare].y;
		}
		else
			max_of_height = m_hero[AI_Aim].pos.y + HERO_SIZE_Y;
		D_y = m_hero[AI_Aim].pos.y - max_of_height + HERO_SIZE_Y + HERO_SIZE_Y / 4;
	
		for(Degree_Of_Firing = 87; D_y + tan(Degree_Of_Firing * Pi / 180) * D_x >= 0 && Degree_Of_Firing > -57; Degree_Of_Firing -= 3)
		{
			Degree_Of_power = int(100 * (D_x / (cos(Degree_Of_Firing * Pi / 180) * sqrt(2 * (D_y + tan(Degree_Of_Firing * Pi / 180) * D_x) / g_y))) / m_Weapon[now_Weapon].Vel);
			if(m_hero[now_hero].position)
				Degree_Of_power += WindSpeed * 25;
			else
				Degree_Of_power -= WindSpeed * 25;
			if(Degree_Of_power > 150 || Degree_Of_power < 30)
				continue;
			else
				break;
		}
		if(Degree_Of_Firing <= -57)
			continue;
		else
			break;
	}
	if(AI_Aim <= num_hero / 2)
	{
		AI_Step[0] = temp_i_AI;
		AI_Step[1] = Degree_Of_Firing / 3;
		AI_Step[2] = (Degree_Of_power - 30) / 4;

	}
	else
	{
		Now_Hero_Change();
		Status_OF_Game = 0;
	}
	Degree_Of_Firing = 0;
	Degree_Of_power = 30;


	break;
		case 1:











	now_Weapon = 2;
	Take_Weapon();
	Controlable = FALSE;
	AI_Temp_Hero = 0;
	while(1)
	{
		AI_Aim = 0;
		for(i_hero = 1; i_hero <= num_hero / 2; i_hero++)
		{
			if(m_hero[i_hero].NowBleed <= 0)
				continue;
			if(AI_Aim == 0)
				AI_Aim = i_hero;
			if(AI_Temp_Hero == 0)
			{
				if(abs(m_hero[i_hero].pos.x - m_hero[now_hero].pos.x) < abs(m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x))
					AI_Aim = i_hero;
			}
			else
			{
				if(abs(m_hero[i_hero].pos.x - m_hero[now_hero].pos.x) < abs(m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x) && abs(m_hero[i_hero].pos.x - m_hero[now_hero].pos.x) > abs(m_hero[AI_Temp_Hero].pos.x - m_hero[now_hero].pos.x))
					AI_Aim = i_hero;
			}
		}
		if(AI_Aim == 0)
			break;
		if(AI_Temp_Hero == 0)
			AI_Temp_Hero = AI_Aim;
		if(m_hero[AI_Aim].pos.x < m_hero[now_hero].pos.x)
		{
			m_hero[now_hero].hBmp = m_hHeroLeftBmp;
			m_hero[now_hero].position = 1;
		}
		else
		{
			m_hero[now_hero].hBmp = m_hHeroBmp;
			m_hero[now_hero].position = 0;	
		}
	
		D_x = m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x;
		if(D_x < 0)
		{
			for(temp_i_AI = 0; m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x + HERO_VEL_X * (temp_i_AI + 1) < -100 && Terrianheight[m_hero[now_hero].pos.x - HERO_VEL_X * (temp_i_AI + 1)].y < SeaLevel - 1; temp_i_AI++);
			D_x = -(m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x + HERO_VEL_X * (temp_i_AI + 1));
			temp_i_AI *= -1;
		}
		else
		{
			for(temp_i_AI = 0; m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x - HERO_VEL_X * (temp_i_AI + 1) > 100 && Terrianheight[m_hero[now_hero].pos.x + HERO_SIZE_X + HERO_VEL_X * (temp_i_AI + 1)].y < SeaLevel - 1; temp_i_AI++);
			D_x = m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x - HERO_VEL_X * (temp_i_AI + 1);
		}
		if(D_x > 0)
		{
			max_of_height = Terrianheight[m_hero[now_hero].pos.x + HERO_VEL_X * (temp_i_AI + 1)].y;
			for(temp_compare = 1; temp_compare <= HERO_SIZE_X * 3 / 4; temp_compare++)
				max_of_height = max_of_height < Terrianheight[m_hero[now_hero].pos.x + HERO_VEL_X * (temp_i_AI + 1) + temp_compare].y ? max_of_height : Terrianheight[m_hero[now_hero].pos.x + HERO_VEL_X * (temp_i_AI + 1) + temp_compare].y;
		}
		else if(D_x < 0)
		{
			max_of_height = Terrianheight[m_hero[now_hero].pos.x - HERO_VEL_X * (temp_i_AI + 1) + HERO_SIZE_X / 4].y;
			for(temp_compare = 1 +  HERO_SIZE_X / 4; temp_compare <= HERO_SIZE_X; temp_compare++)
				max_of_height = max_of_height < Terrianheight[m_hero[now_hero].pos.x - HERO_VEL_X * (temp_i_AI + 1) + temp_compare].y ? max_of_height : Terrianheight[m_hero[now_hero].pos.x - HERO_VEL_X * (temp_i_AI + 1) + temp_compare].y;
		}
		else
			max_of_height = m_hero[AI_Aim].pos.y + HERO_SIZE_Y;
		D_y = m_hero[AI_Aim].pos.y - max_of_height + HERO_SIZE_Y + HERO_SIZE_Y / 4;
	
		for(Degree_Of_Firing = 30; D_y + tan(Degree_Of_Firing * Pi / 180) * D_x >= 0 && Degree_Of_Firing > -57; Degree_Of_Firing -= 3)
		{
			Degree_Of_power = int(100 * (D_x / (cos(Degree_Of_Firing * Pi / 180) * sqrt(2 * (D_y + tan(Degree_Of_Firing * Pi / 180) * D_x) / g_y))) / m_Weapon[now_Weapon].Vel);
			temp_degree_of_wind = 0;
			if(abs(m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x) > 200 && abs(m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x) < WNDWIDTH / 3)
			{
				switch (WindSpeed)
				{
				case 1:
					temp_degree_of_wind = 1;
					break;
				case -1:
					temp_degree_of_wind = -1;
					break;
				case 2:
					temp_degree_of_wind = 1;
					break;
				case -2:
					temp_degree_of_wind = -1;
					break;
				case 3:
					temp_degree_of_wind = 1;
					break;
				case -3:
					temp_degree_of_wind = -1;
					break;
				case 4:
					temp_degree_of_wind = 1;
					break;
				case -4:
					temp_degree_of_wind = -1;
					break;
				default:
					break;
				}
			}
			else if(abs(m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x) <= WNDWIDTH * 2 / 3)
			{
				switch (WindSpeed)
				{
				case 1:
					temp_degree_of_wind = 3;
					break;
				case -1:
					temp_degree_of_wind = -3;
					break;
				case 2:
					temp_degree_of_wind = 4;
					break;
				case -2:
					temp_degree_of_wind = -4;
					break;
				case 3:
					temp_degree_of_wind = 6;
					break;
				case -3:
					temp_degree_of_wind = -6;
					break;
				case 4:
					temp_degree_of_wind = 8;
					break;
				case -4:
					temp_degree_of_wind = -8;
					break;
				default:
					break;
				}
			}
			else if(abs(m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x) <= WNDWIDTH)
			{
				switch (WindSpeed)
				{
				case 1:
					temp_degree_of_wind = 5;
					break;
				case -1:
					temp_degree_of_wind = -5;
					break;
				case 2:
					temp_degree_of_wind = 9;
					break;
				case -2:
					temp_degree_of_wind = -9;
					break;
				case 3:
					temp_degree_of_wind = 13;
					break;
				case -3:
					temp_degree_of_wind = -13;
					break;
				case 4:
					temp_degree_of_wind = 18;
					break;
				case -4:
					temp_degree_of_wind = -18;
					break;
				default:
					break;
				}
			}
			else if(abs(m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x) <= WNDWIDTH * 4 / 3)
			{
				switch (WindSpeed)
				{
				case 1:
					temp_degree_of_wind = 7;
					break;
				case -1:
					temp_degree_of_wind = -7;
					break;
				case 2:
					temp_degree_of_wind = 14;
					break;
				case -2:
					temp_degree_of_wind = -14;
					break;
				case 3:
					temp_degree_of_wind = 21;
					break;
				case -3:
					temp_degree_of_wind = -21;
					break;
				case 4:
					temp_degree_of_wind = 28;
					break;
				case -4:
					temp_degree_of_wind = -28;
					break;
				default:
					break;
				}
			}
			else if(abs(m_hero[AI_Aim].pos.x - m_hero[now_hero].pos.x) <= WNDWIDTH * 5 / 3)
			{
				switch (WindSpeed)
				{
				case 1:
					temp_degree_of_wind = 9;
					break;
				case -1:
					temp_degree_of_wind = -9;
					break;
				case 2:
					temp_degree_of_wind = 18;
					break;
				case -2:
					temp_degree_of_wind = -18;
					break;
				case 3:
					temp_degree_of_wind = 27;
					break;
				case -3:
					temp_degree_of_wind = -27;
					break;
				case 4:
					temp_degree_of_wind = 36;
					break;
				case -4:
					temp_degree_of_wind = -36;
					break;
				default:
					break;
				}
			}
			else
			{
				switch (WindSpeed)
				{
				case 1:
					temp_degree_of_wind = 11;
					break;
				case -1:
					temp_degree_of_wind = -11;
					break;
				case 2:
					temp_degree_of_wind = 22;
					break;
				case -2:
					temp_degree_of_wind = -22;
					break;
				case 3:
					temp_degree_of_wind = 33;
					break;
				case -3:
					temp_degree_of_wind = -33;
					break;
				case 4:
					temp_degree_of_wind = 44;
					break;
				case -4:
					temp_degree_of_wind = -44;
					break;
				default:
					break;
				}
			}
			if(!m_hero[now_hero].position)
				temp_degree_of_wind *= -1;
			if(Degree_Of_power > 150 || Degree_Of_power < 30)
				continue;
			else
				break;
		}
		if(Degree_Of_Firing <= -57)
			continue;
		else
			break;
	}
	if(AI_Aim != 0)
	{
		AI_Step[0] = temp_i_AI;
		AI_Step[1] = Degree_Of_Firing / 3;
		AI_Step[2] = (Degree_Of_power - 30) / 4;
	}
	else
	{
		Now_Hero_Change();
		Status_OF_Game = 0;
	}
	Degree_Of_Firing = 0;
	Degree_Of_power = 30;


			break;













		case 2:
	Controlable = FALSE;
	temp_i_AI = 0;
	now_Weapon = 2;
	for(temp_i_AI = 0; Terrianheight[m_hero[now_hero].pos.x - HERO_VEL_X * (temp_i_AI + 1)].y < SeaLevel - 1; temp_i_AI += 3)
	{
		for(m_hero[now_hero].position = 0; m_hero[now_hero].position < 2; m_hero[now_hero].position++)
		{
			for(now_Weapon = 2; now_Weapon <= 2; now_Weapon = (3 + now_Weapon) % 4)
			{
				for(Degree_Of_Firing = 87; Degree_Of_Firing > -57; Degree_Of_Firing -= 6)
				{
					for(Degree_Of_power = 30; Degree_Of_power < 146; Degree_Of_power += 4)
					{
						GetTrack(FALSE, Degree_Of_Firing, Degree_Of_power, m_hero[now_hero].pos.x + HERO_VEL_X * temp_i_AI + HERO_SIZE_X / 2, m_hero[now_hero].pos.y + HERO_SIZE_Y / 4, m_hero[now_hero].position, m_Weapon[now_Weapon].Vel);
						if(Hero_Boomed > 0 && Hero_Boomed <= num_hero / 2)
							break;
					}
					if(Degree_Of_power < 146)
					{
						//Degree_Of_power -= 4;
						break;
					}
				}
				if(Degree_Of_Firing > -57)
				{
					//Degree_Of_Firing += 3;
					break;
				}
			}
			if(now_Weapon <= 2)
				break;
		}
		if(m_hero[now_hero].position < 2)
			break;
	}

	if(m_hero[now_hero].position = 2)
	{
		for(temp_i_AI = 0; Terrianheight[m_hero[now_hero].pos.x - HERO_VEL_X * (temp_i_AI - 1)].y < SeaLevel - 1; temp_i_AI -= 3)
		{
			for(m_hero[now_hero].position = 0; m_hero[now_hero].position < 2; m_hero[now_hero].position++)
			{
				for(now_Weapon = 2; now_Weapon <= 2; now_Weapon = (3 + now_Weapon) % 4)
				{
					for(Degree_Of_Firing = 87; Degree_Of_Firing > -57; Degree_Of_Firing -= 6)
					{
						for(Degree_Of_power = 30; Degree_Of_power < 146; Degree_Of_power += 4)
						{
							GetTrack(FALSE, Degree_Of_Firing, Degree_Of_power, m_hero[now_hero].pos.x + HERO_VEL_X * temp_i_AI + HERO_SIZE_X / 2, m_hero[now_hero].pos.y + HERO_SIZE_Y / 4, m_hero[now_hero].position, m_Weapon[now_Weapon].Vel);
							if(Hero_Boomed > 0 && Hero_Boomed <= num_hero / 2)
								break;
						}
						if(Degree_Of_power < 146)
						{
							//Degree_Of_power -= 4;
							break;
						}
					}
					if(Degree_Of_Firing > -57)
					{
						//Degree_Of_Firing += 3;
						break;
					}
				}
				if(now_Weapon <= 2)
					break;
			}
			if(m_hero[now_hero].position < 2)
				break;
		}
	}
	if(m_hero[now_hero].position < 2 && now_Weapon < 3)
	{
		AI_Step[0] = temp_i_AI;
		AI_Step[1] = Degree_Of_Firing / 3;
		AI_Step[2] = (Degree_Of_power - 30) / 4;
	}
	else
	{
		m_hero[now_hero].position = 0;
		Now_Hero_Change();
		Status_OF_Game = 0;
	}
	if(m_hero[now_hero].position == 0)
		m_hero[now_hero].hBmp = m_hHeroBmp;
	else
		m_hero[now_hero].hBmp = m_hHeroLeftBmp;
	Take_Weapon();
	Degree_Of_Firing = 0;
	Degree_Of_power = 30;

			break;
		default:
			break;
	}
	AI_WAIT = 30;
	mciSendString(_T("stop myChangeWeapon"), NULL, 0, NULL);
	mciSendString(_T("close myChangeWeapon"), NULL, 0, NULL);
	mciSendString(mcicmdChangeWeapon, NULL,0, NULL);
	mciSendString(_T("play myChangeWeapon"), NULL, 0, NULL);
}

VOID GetTrack(BOOL If_Need_Draw, int Degree, int Power, int x, int y, int IF_LEFT, int vel0)
{
	Hero_Boomed = 0;
	if(If_Need_Draw)
	{
	NOT_Firing_Me = TRUE;
	Touched = FALSE;
	temp_i = 0;
	vel0 = vel0 * Power / 100;
	if(IF_LEFT)
		velx = - (int)(vel0 * cos(Degree * Pi / 180));
	else
		velx = (int)(vel0 * cos(Degree * Pi / 180));
	vely = - (int)(vel0 * sin(Degree * Pi / 180));
	m_Track.Points[0].x = Pre.x = x;
	m_Track.Points[0].y = Pre.y = y;
	if_add_wind = 0;
	while(!Touched)
	{
		Temp_Boom.x = x;
		Temp_Boom.y = y;
		x += velx;
		y += vely;
		if(WindSpeed == 1 || WindSpeed == -1)
		{
			if(!if_add_wind)
				velx += WindSpeed;
			if_add_wind = (if_add_wind + 1) % 4;
		}
		else if(WindSpeed == 2 || WindSpeed == -2)
		{
			if(!if_add_wind)
				velx += WindSpeed / 2;
			if_add_wind = (if_add_wind + 1) % 2;
		}
		else if(WindSpeed == 3 || WindSpeed == -3)
		{
			if(if_add_wind)
				velx += WindSpeed / 3;
			if_add_wind = (if_add_wind + 1) % 4;
		}
		else if(WindSpeed == 4 || WindSpeed == -4)
		{
			velx += WindSpeed / 4;
		}
		vely += g_y;


		//下面直接照搬hitting里面的内容，如果要改就都改！！！！！！！！！！
	if(velx > 0)
	{
		temp_tangent = (double)vely / velx;
		for(temp_boom_int = 0; temp_boom_int <= velx; temp_boom_int++)
		{
			temp_i++;
			m_Track.Points[temp_i].x = Temp_Boom_i.x = Temp_Boom.x + temp_boom_int;
			m_Track.Points[temp_i].y = Temp_Boom_i.y = int(Temp_Boom.y + temp_boom_int * temp_tangent) + 1;
			if(Temp_Boom_i.x >= WNDWIDTH * 2 || Temp_Boom_i.x <= 0)
			{
				Touched = TRUE;
				break;
			}
			if(Terrianheight[Temp_Boom_i.x].y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius)
			{
				Touched = TRUE;
				break;
			}
			else
			{
				if(NOT_Firing_Me && (m_hero[now_hero].pos.x - 10 > Temp_Boom_i.x + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.x + HERO_SIZE_X + 10 < Temp_Boom_i.x - m_Weapon[now_Weapon].Radius) && (m_hero[now_hero].pos.y - 10 > Temp_Boom_i.y + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.y + HERO_SIZE_Y + 10 < Temp_Boom_i.y - m_Weapon[now_Weapon].Radius))
			NOT_Firing_Me = FALSE;
				for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
				{
					if(!m_hero[i_hero].NowBleed)
						continue;
					if(m_hero[i_hero].pos.x <= Temp_Boom_i.x + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.x + HERO_SIZE_X >= Temp_Boom_i.x - m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y + HERO_SIZE_Y >= Temp_Boom_i.y - m_Weapon[now_Weapon].Radius)
					{
						if(NOT_Firing_Me && (i_hero == now_hero || (Temp_Boom_i.x >= m_hero[now_hero].pos.x - 10 && Temp_Boom_i.x <= m_hero[now_hero].pos.x + HERO_SIZE_X + 10)))
							continue;
						else
						{
							Touched = TRUE;
							Hero_Boomed = i_hero;
							break;
						}
					}
				}
			}
		}
	}
	//后一段复制前一段，如果要改要都改
	else if(velx < 0)
	{
		temp_tangent = (double)vely / velx;
		for(temp_boom_int = 0; temp_boom_int >= velx; temp_boom_int--)
		{
			temp_i++;
			m_Track.Points[temp_i].x = Temp_Boom_i.x = Temp_Boom.x + temp_boom_int;
			m_Track.Points[temp_i].y = Temp_Boom_i.y = int(Temp_Boom.y + temp_boom_int * temp_tangent) + 1;
			if(Temp_Boom_i.x >= WNDWIDTH * 2 || Temp_Boom_i.x <= 0)
			{
				Touched = TRUE;
				break;
			}
			if(Terrianheight[Temp_Boom_i.x].y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius)
			{
				Touched = TRUE;
				break;
			}
			else
			{
				if(NOT_Firing_Me && (m_hero[now_hero].pos.x - 10 > Temp_Boom_i.x + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.x + HERO_SIZE_X + 10 < Temp_Boom_i.x - m_Weapon[now_Weapon].Radius) && (m_hero[now_hero].pos.y - 10 > Temp_Boom_i.y + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.y + HERO_SIZE_Y + 10 < Temp_Boom_i.y - m_Weapon[now_Weapon].Radius))
			NOT_Firing_Me = FALSE;
				for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
				{
					if(!m_hero[i_hero].NowBleed)
						continue;
					if(m_hero[i_hero].pos.x <= Temp_Boom_i.x + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.x + HERO_SIZE_X >= Temp_Boom_i.x - m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y + HERO_SIZE_Y >= Temp_Boom_i.y - m_Weapon[now_Weapon].Radius)
					{
						if(NOT_Firing_Me && (i_hero == now_hero || (Temp_Boom_i.x >= m_hero[now_hero].pos.x - 10 && Temp_Boom_i.x <= m_hero[now_hero].pos.x + HERO_SIZE_X + 10)))
							continue;
						else
						{
							Touched = TRUE;
							Hero_Boomed = i_hero;
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		Temp_Boom_i.x = Temp_Boom.x;
		if(vely <= 0)
		{
		for(temp_boom_int = 0; temp_boom_int >= vely; temp_boom_int--)
		{
			temp_i++;
			m_Track.Points[temp_i].x = Temp_Boom_i.x;
			m_Track.Points[temp_i].y = Temp_Boom_i.y = Temp_Boom.y + temp_boom_int + 1;
			if(Temp_Boom_i.x >= WNDWIDTH * 2 || Temp_Boom_i.x <= 0)
			{
				Touched = TRUE;
				break;
			}
			if(Terrianheight[Temp_Boom_i.x].y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius)
			{
				Touched = TRUE;
				break;
			}
			else
			{
				if(NOT_Firing_Me && (m_hero[now_hero].pos.x - 10 > Temp_Boom_i.x + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.x + HERO_SIZE_X + 10 < Temp_Boom_i.x - m_Weapon[now_Weapon].Radius) && (m_hero[now_hero].pos.y - 10 > Temp_Boom_i.y + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.y + HERO_SIZE_Y + 10 < Temp_Boom_i.y - m_Weapon[now_Weapon].Radius))
			NOT_Firing_Me = FALSE;
				for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
				{
					if(!m_hero[i_hero].NowBleed)
						continue;
					if(m_hero[i_hero].pos.x <= Temp_Boom_i.x + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.x + HERO_SIZE_X >= Temp_Boom_i.x - m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y + HERO_SIZE_Y >= Temp_Boom_i.y - m_Weapon[now_Weapon].Radius)
					{
						if(NOT_Firing_Me && (i_hero == now_hero || (Temp_Boom_i.x >= m_hero[now_hero].pos.x - 10 && Temp_Boom_i.x <= m_hero[now_hero].pos.x + HERO_SIZE_X + 10)))
							continue;
						else
						{
							Touched = TRUE;
							Hero_Boomed = i_hero;
							break;
						}
					}
				}
			}
		}
		}
		else
		{
		for(temp_boom_int = 0; temp_boom_int <= vely; temp_boom_int++)
		{
			temp_i++;
			m_Track.Points[temp_i].x = Temp_Boom_i.x;
			m_Track.Points[temp_i].y = Temp_Boom_i.y = Temp_Boom.y + temp_boom_int + 1;
			if(Temp_Boom_i.x >= WNDWIDTH * 2 || Temp_Boom_i.x <= 0)
			{
				Touched = TRUE;
				break;
			}
			if(Terrianheight[Temp_Boom_i.x].y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius)
			{
				Touched = TRUE;
				break;
			}
			else
			{
				if(NOT_Firing_Me && (m_hero[now_hero].pos.x - 10 > Temp_Boom_i.x + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.x + HERO_SIZE_X + 10 < Temp_Boom_i.x - m_Weapon[now_Weapon].Radius) && (m_hero[now_hero].pos.y - 10 > Temp_Boom_i.y + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.y + HERO_SIZE_Y + 10 < Temp_Boom_i.y - m_Weapon[now_Weapon].Radius))
			NOT_Firing_Me = FALSE;
				for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
				{
					if(!m_hero[i_hero].NowBleed)
						continue;
					if(m_hero[i_hero].pos.x <= Temp_Boom_i.x + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.x + HERO_SIZE_X >= Temp_Boom_i.x - m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y + HERO_SIZE_Y >= Temp_Boom_i.y - m_Weapon[now_Weapon].Radius)
					{
						if(NOT_Firing_Me && (i_hero == now_hero || (Temp_Boom_i.x >= m_hero[now_hero].pos.x - 10 && Temp_Boom_i.x <= m_hero[now_hero].pos.x + HERO_SIZE_X + 10)))
							continue;
						else
						{
							Touched = TRUE;
							Hero_Boomed = i_hero;
							break;
						}
					}
				}
			}
		}
		}
	}
	}
	m_Track.Destination.x = x;
	m_Track.Destination.y = y;
   	m_Track.Lenth = temp_i;
	}
	else










		//下面同上面的，但是不需要画
	{
	NOT_Firing_Me = TRUE;
	Touched = FALSE;
	vel0 = vel0 * Power / 100;
	if(IF_LEFT)
		velx = - (int)(vel0 * cos(Degree * Pi / 180));
	else
		velx = (int)(vel0 * cos(Degree * Pi / 180));
	vely = - (int)(vel0 * sin(Degree * Pi / 180));
	m_Track.Points[0].x = Pre.x = x;
	m_Track.Points[0].y = Pre.y = y;
	if_add_wind = 0;
	while(!Touched)
	{
		Temp_Boom.x = x;
		Temp_Boom.y = y;
		x += velx;
		y += vely;
		if(WindSpeed == 1 || WindSpeed == -1)
		{
			if(!if_add_wind)
				velx += WindSpeed;
			if_add_wind = (if_add_wind + 1) % 4;
		}
		else if(WindSpeed == 2 || WindSpeed == -2)
		{
			if(!if_add_wind)
				velx += WindSpeed / 2;
			if_add_wind = (if_add_wind + 1) % 2;
		}
		else if(WindSpeed == 3 || WindSpeed == -3)
		{
			if(if_add_wind)
				velx += WindSpeed / 3;
			if_add_wind = (if_add_wind + 1) % 4;
		}
		else if(WindSpeed == 4 || WindSpeed == -4)
		{
			velx += WindSpeed / 4;
		}
		vely += g_y;


		//下面直接照搬hitting里面的内容，如果要改就都改！！！！！！！！！！
	if(velx > 0)
	{
		temp_tangent = (double)vely / velx;
		for(temp_boom_int = 0; temp_boom_int <= velx; temp_boom_int++)
		{
			Temp_Boom_i.x = Temp_Boom.x + temp_boom_int;
			Temp_Boom_i.y = int(Temp_Boom.y + temp_boom_int * temp_tangent) + 1;
			if(Temp_Boom_i.x >= WNDWIDTH * 2 || Temp_Boom_i.x <= 0)
			{
				Touched = TRUE;
				break;
			}
			if(Terrianheight[Temp_Boom_i.x].y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius)
			{
				Touched = TRUE;
				break;
			}
			else
			{
				if(NOT_Firing_Me && (m_hero[now_hero].pos.x - 10 > Temp_Boom_i.x + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.x + HERO_SIZE_X + 10 < Temp_Boom_i.x - m_Weapon[now_Weapon].Radius) && (m_hero[now_hero].pos.y - 10 > Temp_Boom_i.y + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.y + HERO_SIZE_Y + 10 < Temp_Boom_i.y - m_Weapon[now_Weapon].Radius))
			NOT_Firing_Me = FALSE;
				for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
				{
					if(!m_hero[i_hero].NowBleed)
						continue;
					if(m_hero[i_hero].pos.x <= Temp_Boom_i.x + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.x + HERO_SIZE_X >= Temp_Boom_i.x - m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y + HERO_SIZE_Y >= Temp_Boom_i.y - m_Weapon[now_Weapon].Radius)
					{
						if(NOT_Firing_Me && (i_hero == now_hero || (Temp_Boom_i.x >= m_hero[now_hero].pos.x - 10 && Temp_Boom_i.x <= m_hero[now_hero].pos.x + HERO_SIZE_X + 10)))
							continue;
						else
						{
							Touched = TRUE;
							Hero_Boomed = i_hero;
							break;
						}
					}
				}
			}
		}
	}
	//后一段复制前一段，如果要改要都改
	else if(velx < 0)
	{
		temp_tangent = (double)vely / velx;
		for(temp_boom_int = 0; temp_boom_int >= velx; temp_boom_int--)
		{
			Temp_Boom_i.x = Temp_Boom.x + temp_boom_int;
			Temp_Boom_i.y = int(Temp_Boom.y + temp_boom_int * temp_tangent) + 1;
			if(Temp_Boom_i.x >= WNDWIDTH * 2 || Temp_Boom_i.x <= 0)
			{
				Touched = TRUE;
				break;
			}
			if(Terrianheight[Temp_Boom_i.x].y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius)
			{
				Touched = TRUE;
				break;
			}
			else
			{
				if(NOT_Firing_Me && (m_hero[now_hero].pos.x - 10 > Temp_Boom_i.x + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.x + HERO_SIZE_X + 10 < Temp_Boom_i.x - m_Weapon[now_Weapon].Radius) && (m_hero[now_hero].pos.y - 10 > Temp_Boom_i.y + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.y + HERO_SIZE_Y + 10 < Temp_Boom_i.y - m_Weapon[now_Weapon].Radius))
			NOT_Firing_Me = FALSE;
				for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
				{
					if(!m_hero[i_hero].NowBleed)
						continue;
					if(m_hero[i_hero].pos.x <= Temp_Boom_i.x + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.x + HERO_SIZE_X >= Temp_Boom_i.x - m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y + HERO_SIZE_Y >= Temp_Boom_i.y - m_Weapon[now_Weapon].Radius)
					{
						if(NOT_Firing_Me && (i_hero == now_hero || (Temp_Boom_i.x >= m_hero[now_hero].pos.x - 10 && Temp_Boom_i.x <= m_hero[now_hero].pos.x + HERO_SIZE_X + 10)))
							continue;
						else
						{
							Touched = TRUE;
							Hero_Boomed = i_hero;
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		Temp_Boom_i.x = Temp_Boom.x;
		if(vely <= 0)
		{
		for(temp_boom_int = 0; temp_boom_int >= vely; temp_boom_int--)
		{
			Temp_Boom_i.y = Temp_Boom.y + temp_boom_int + 1;
			if(Temp_Boom_i.x >= WNDWIDTH * 2 || Temp_Boom_i.x <= 0)
			{
				Touched = TRUE;
				break;
			}
			if(Terrianheight[Temp_Boom_i.x].y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius)
			{
				Touched = TRUE;
				break;
			}
			else
			{
				if(NOT_Firing_Me && (m_hero[now_hero].pos.x - 10 > Temp_Boom_i.x + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.x + HERO_SIZE_X + 10 < Temp_Boom_i.x - m_Weapon[now_Weapon].Radius) && (m_hero[now_hero].pos.y - 10 > Temp_Boom_i.y + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.y + HERO_SIZE_Y + 10 < Temp_Boom_i.y - m_Weapon[now_Weapon].Radius))
			NOT_Firing_Me = FALSE;
				for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
				{
					if(!m_hero[i_hero].NowBleed)
						continue;
					if(m_hero[i_hero].pos.x <= Temp_Boom_i.x + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.x + HERO_SIZE_X >= Temp_Boom_i.x - m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y + HERO_SIZE_Y >= Temp_Boom_i.y - m_Weapon[now_Weapon].Radius)
					{
						if(NOT_Firing_Me && (i_hero == now_hero || (Temp_Boom_i.x >= m_hero[now_hero].pos.x - 10 && Temp_Boom_i.x <= m_hero[now_hero].pos.x + HERO_SIZE_X + 10)))
							continue;
						else
						{
							Touched = TRUE;
							Hero_Boomed = i_hero;
							break;
						}
					}
				}
			}
		}
		}
		else
		{
		for(temp_boom_int = 0; temp_boom_int <= vely; temp_boom_int++)
		{
			Temp_Boom_i.y = Temp_Boom.y + temp_boom_int + 1;
			if(Temp_Boom_i.x >= WNDWIDTH * 2 || Temp_Boom_i.x <= 0)
			{
				Touched = TRUE;
				break;
			}
			if(Terrianheight[Temp_Boom_i.x].y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius)
			{
				Touched = TRUE;
				break;
			}
			else
			{
				if(NOT_Firing_Me && (m_hero[now_hero].pos.x - 10 > Temp_Boom_i.x + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.x + HERO_SIZE_X + 10 < Temp_Boom_i.x - m_Weapon[now_Weapon].Radius) && (m_hero[now_hero].pos.y - 10 > Temp_Boom_i.y + m_Weapon[now_Weapon].Radius || m_hero[now_hero].pos.y + HERO_SIZE_Y + 10 < Temp_Boom_i.y - m_Weapon[now_Weapon].Radius))
			NOT_Firing_Me = FALSE;
				for(i_hero = 1; i_hero <= num_hero && num_alive[0]; i_hero++)
				{
					if(!m_hero[i_hero].NowBleed)
						continue;
					if(m_hero[i_hero].pos.x <= Temp_Boom_i.x + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.x + HERO_SIZE_X >= Temp_Boom_i.x - m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y <= Temp_Boom_i.y + m_Weapon[now_Weapon].Radius && m_hero[i_hero].pos.y + HERO_SIZE_Y >= Temp_Boom_i.y - m_Weapon[now_Weapon].Radius)
					{
						if(NOT_Firing_Me && (i_hero == now_hero || (Temp_Boom_i.x >= m_hero[now_hero].pos.x - 10 && Temp_Boom_i.x <= m_hero[now_hero].pos.x + HERO_SIZE_X + 10)))
							continue;
						else
						{
							Touched = TRUE;
							Hero_Boomed = i_hero;
							break;
						}
					}
				}
			}
		}
		}
	}
	}
	m_Track.Destination.x = x;
	m_Track.Destination.y = y;
	}
}