/*
 * ship.h
 *
 *  Created on: 15 mai 2020
 *      Author: Thierry
 */

int ShipInitGraphic(SDL_Renderer*pRenderer, char*pShipImageFileName);

int ShipReleaseGraphic(void);

struct s_ship;
typedef struct s_ship s_ship;


s_ship*ShipNew(
		int iLocX, int iLocY, int iWidth, int iHeight,
		float fHotRatioX, float fHotRatioY, int iSpeedX, int iSpeedY);

s_ship*ShipDel(s_ship*pShip);

s_ship*ShipDraw(s_ship*pShip);

s_ship*ShipMoveToXY(s_ship*pShip, int iLocX, int iLocY);

s_ship*ShipMove(s_ship*pShip, int iLocX, int iLocY);

s_ship*ShipDrawLives(s_ship*pShip, int iLocX, int iLocY, int iLocH, int iLocW);

SDL_Rect ShipGetCoordinates(s_ship*pShip);

s_ship*ShipSetSize(s_ship*pShip,int factor);
