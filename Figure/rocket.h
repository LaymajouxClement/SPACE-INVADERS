/*
 * Rockets.h
 *
 *  Created on: 15 mai 2020
 *      Author: Laymajoux Gilles Pascual
 */

int RocketInitGraphic(SDL_Renderer*pRenderer, char*pRocketImageFileName);
int AlienRocketInitGraphic(SDL_Renderer*pRenderer, char*pRocketImageFileName);
int RocketReleaseGraphic(void);
int AlienRocketReleaseGraphic(void);

struct s_rocket;
typedef struct s_rocket s_rocket;


s_rocket*RocketNew(
		int iLocX, int iLocY, int iWidth, int iHeight,
		float fHotRatioX, float fHotRatioY, int iSpeedX, int iSpeedY);

s_rocket*RocketDel(s_rocket*pRocket);

s_rocket*RocketDraw(s_rocket*pRocket, int type);

s_rocket*RocketMove(s_rocket*pRocket, int scroll);
