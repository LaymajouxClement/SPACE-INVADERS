/*
 * aliens.h
 *
 *  Created on: 15 mai 2020
 *      Author: Laymajoux Gilles Pascual
 */

int AlienInitGraphic(SDL_Renderer*pRenderer, char*pAlienImageFileName);
int AlienReleaseGraphic(void);

struct s_alien;
typedef struct s_alien s_alien;


s_alien*AlienNew(
		int iLocX, int iLocY, int iWidth, int iHeight,
		float fHotRatioX, float fHotRatioY, int iSpeedX, int iSpeedY);

s_alien*AlienDel(s_alien*pAlien);

s_alien*AlienDraw(s_alien*pAlien);

s_alien*AlienMoveToXY(s_alien*pAlien, int iLocX, int iLocY);

s_alien*AlienMove(s_alien*pAlien, int* scroll);

s_alien*AlienMoveX(s_alien*pAlien,int *scroll);

s_alien*AlienReverseSpeedX(s_alien*pAlien,int scrollX);

SDL_Rect AlienGetCoordinates(s_alien*pAlien);

s_alien*AlienSerialize(s_alien*pAlien, FILE*hFile);

int AlienUnSerialize(FILE*hFile);


