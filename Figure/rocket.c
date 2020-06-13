/*
 * rocket.c
 *
 *  Created on: 15 mai 2020
 *      Author: Laymajoux Gilles Pascual
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "rocket.h"


//////STATIC SHIP ENTITY SECTION----------------------------------------------------------------////

static struct s_rocketGraphic{
	SDL_Texture  * 	pTexture;
}rocketGraphic,alienRocketGraphic;

int RocketInitGraphic(SDL_Renderer*pRenderer, char*pRocketImageFileName){
	SDL_Surface * image=IMG_Load(pRocketImageFileName);
	if(image==NULL) return EXIT_FAILURE;
	rocketGraphic.pTexture=SDL_CreateTextureFromSurface(pRenderer, image);
	SDL_FreeSurface(image);
	return 0;
}

int AlienRocketInitGraphic(SDL_Renderer*pRenderer, char*pRocketImageFileName){
	SDL_Surface * image=IMG_Load(pRocketImageFileName);
	if(image==NULL) return EXIT_FAILURE;
	alienRocketGraphic.pTexture=SDL_CreateTextureFromSurface(pRenderer, image);
	SDL_FreeSurface(image);
	return 0;
}


int RocketReleaseGraphic(void){
	if(rocketGraphic.pTexture){
		SDL_DestroyTexture(rocketGraphic.pTexture);
		rocketGraphic.pTexture=NULL;
	}
	return 0;
}

int AlienRocketReleaseGraphic(void){
	if(alienRocketGraphic.pTexture){
		SDL_DestroyTexture(alienRocketGraphic.pTexture);
		alienRocketGraphic.pTexture=NULL;
	}
	return 0;
}

//////"DYNAMIC" SHIP ENTITY SECTION-------------------------------------------------------------////

#include "figure.h"

struct s_rocket{
	SDL_Rect	frame;
	SDL_FPoint	hotPointRatio;
	SDL_Point	speed;
};


s_rocket*RocketNew(
		int iLocX,
		int iLocY,
		int iWidth,
		int iHeight,
		float fHotRatioX,
		float fHotRatioY,
		int iSpeedX,
		int iSpeedY
	){
	return (s_rocket*)FigureNew(iLocX, iLocY, iWidth, iHeight, fHotRatioX, fHotRatioY, iSpeedX, iSpeedY);
}

s_rocket*RocketDel(s_rocket*pRocket){
	return (s_rocket*)FigureDel((s_figure*)pRocket);
}

s_rocket*RocketDraw(s_rocket*pRocket,int type){
	return (s_rocket*)FigureDraw((s_figure*)pRocket, type?rocketGraphic.pTexture:alienRocketGraphic.pTexture);
}

s_rocket*RocketMove(s_rocket*pRocket, int scroll){

	return (s_rocket*)FigureMoveToXY(
			(s_figure*)pRocket,
			pRocket->frame.x,
			scroll + pRocket->frame.y);
}
