/*
 * ship.c
 *
 *  Created on: 15 mai 2020
 *      Author: Thierry
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ship.h"


//////STATIC SHIP ENTITY SECTION----------------------------------------------------------------////

static struct s_shipGraphic{
	SDL_Texture  * 	pTexture;
}shipGraphic;

int ShipInitGraphic(SDL_Renderer*pRenderer, char*pShipImageFileName){
	SDL_Surface * image=IMG_Load(pShipImageFileName);
	if(image==NULL) return EXIT_FAILURE;
	shipGraphic.pTexture=SDL_CreateTextureFromSurface(pRenderer, image);
	SDL_FreeSurface(image);
	return 0;
}

int ShipReleaseGraphic(void){
	if(shipGraphic.pTexture){
		SDL_DestroyTexture(shipGraphic.pTexture);
		shipGraphic.pTexture=NULL;
	}
	return 0;
}


//////"DYNAMIC" SHIP ENTITY SECTION-------------------------------------------------------------////

#include "figure.h"

struct s_ship{
	SDL_Rect	frame;
	SDL_FPoint	hotPointRatio;
	SDL_Point	speed;
};


s_ship*ShipNew(
		int iLocX,
		int iLocY,
		int iWidth,
		int iHeight,
		float fHotRatioX,
		float fHotRatioY,
		int iSpeedX,
		int iSpeedY
	){
	return (s_ship*)FigureNew(iLocX, iLocY, iWidth, iHeight, fHotRatioX, fHotRatioY, iSpeedX, iSpeedY);
}

s_ship*ShipDel(s_ship*pShip){
	return (s_ship*)FigureDel((s_figure*)pShip);
}

s_ship*ShipDraw(s_ship*pShip){
	return (s_ship*)FigureDraw((s_figure*)pShip, shipGraphic.pTexture);
}

s_ship*ShipMoveToXY(s_ship*pShip, int iLocX, int iLocY){
	return (s_ship*)FigureMoveToXY(
			(s_figure*)pShip,
			iLocX-pShip->frame.w/2,
			pShip->frame.y);
}
s_ship*ShipMove(s_ship*pShip, int iLocX, int iLocY){
	return (s_ship*)FigureMoveToXY(
			(s_figure*)pShip,
			iLocX,
			iLocY);
}


s_ship*ShipDrawLives(s_ship*pShip, int iLocX, int iLocY, int iLocH, int iLocW){
	pShip->frame.x = iLocX;
	pShip->frame.y = iLocY;
	pShip->frame.h = iLocH;
	pShip->frame.w = iLocW;
	return (s_ship*)FigureDraw((s_figure*)pShip, shipGraphic.pTexture);
}

SDL_Rect ShipGetCoordinates(s_ship*pShip){
	return FigureGetCoordinates((s_figure *)pShip);
}

s_ship*ShipSetSize(s_ship*pShip,int factor){
	return (s_ship*)FigureSetSize((s_figure *)pShip,factor);
}
