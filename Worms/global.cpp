#include "global.h"




Hero CreateHero(int posX, int posY, int sizeX, int sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize, int position_hero, int allbleed, int nowbleed)
{
	Hero hero;
	hero.hBmp = hBmp;
	hero.pos.x = posX;
	hero.pos.y = posY;
	hero.vel.x = 0;
	hero.vel.y = 0;
	hero.size.cx = sizeX;
	hero.size.cy = sizeY;
	hero.curFrameIndex = curFrameIndex;
	hero.maxFrameSize = maxFrameSize;
	hero.position = position_hero;
	hero.AllBleed = allbleed;
	hero.NowBleed = nowbleed;
	return hero;
}

GameButton CreateGameButton(int posX, int posY, int sizeX, int sizeY, HBITMAP hBmp, int status)
{
	GameButton gameButton;
	gameButton.pos.x = posX;
	gameButton.pos.y = posY;
	gameButton.size.cx = sizeX;
	gameButton.size.cy = sizeY;
	gameButton.hBmp = hBmp;
	gameButton.status = status;
	return gameButton;
}

//Terrian CreateTerrian(int posX, int posY, int sizeX, int sizeY,
//	HBITMAP hBlockBmp)
//{
//	Terrian terrian;
//	terrian.pos.x = posX;
//	terrian.pos.y = posY;
//	terrian.size.cx = sizeX;
//	terrian.size.cy = sizeY;
//	terrian.hBmp = hBlockBmp;
//	return terrian;
//}
