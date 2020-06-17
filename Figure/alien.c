/*
 * aliens.c
 *
 *  Created on: 15 mai 2020
 *      Author: Laymajoux Gilles Pascual
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "alien.h"

//////STATIC SHIP ENTITY SECTION----------------------------------------------------------------////


static struct s_alienGraphic{
	SDL_Texture  * 	pTexture;
}alienGraphic;

int AlienInitGraphic(SDL_Renderer*pRenderer, char*pAlienImageFileName){
	SDL_Surface * image=IMG_Load(pAlienImageFileName);
	if(image==NULL) return EXIT_FAILURE;
	alienGraphic.pTexture=SDL_CreateTextureFromSurface(pRenderer, image);
	SDL_FreeSurface(image);
	return 0;
}

int AlienReleaseGraphic(void){
	if(alienGraphic.pTexture){
		SDL_DestroyTexture(alienGraphic.pTexture);
		alienGraphic.pTexture=NULL;
	}
	return 0;
}


//////"DYNAMIC" SHIP ENTITY SECTION-------------------------------------------------------------////

#include "figure.h"

struct s_alien{
	SDL_Rect	frame;
	SDL_FPoint	hotPointRatio;
	SDL_Point	speed;
};


s_alien*AlienNew(
		int iLocX,
		int iLocY,
		int iWidth,
		int iHeight,
		float fHotRatioX,
		float fHotRatioY,
		int iSpeedX,
		int iSpeedY
	){
	return (s_alien*)FigureNew(iLocX, iLocY, iWidth, iHeight, fHotRatioX, fHotRatioY, iSpeedX, iSpeedY);
}

s_alien*AlienDel(s_alien*pAlien){
	return (s_alien*)FigureDel((s_figure*)pAlien);
}

s_alien*AlienDraw(s_alien*pAlien){
	return (s_alien*)FigureDraw((s_figure*)pAlien, alienGraphic.pTexture);
}

s_alien*AlienMoveToXY(s_alien*pAlien, int iLocX, int iLocY){

	return (s_alien*)FigureMoveToXY(
			(s_figure*)pAlien,
			iLocX,
			iLocY);
}

s_alien*AlienMoveX(s_alien*pAlien,int *scroll){

	if(FigureMoveX((s_figure*)pAlien, 2*(*scroll)) == NULL){
		*scroll = 1;
	}
	//--------------------------------------------------/
	return pAlien;
}

s_alien*AlienMove(s_alien*pAlien,int *scroll){

	if(FigureMove((s_figure*)pAlien, 2*(*scroll), 0) == NULL){
		*scroll = 1;
	}
	//--------------------------------------------------/
	return pAlien;
}

s_alien*AlienReverseSpeedX(s_alien*pAlien,int scroll){

	pAlien->speed.x *= -1;
	pAlien->frame.y += scroll;
	return pAlien;
}

SDL_Rect AlienGetCoordinates(s_alien*pAlien){
	return FigureGetCoordinates((s_figure *)pAlien);
}


s_alien*AlienSerialize(s_alien*pAlien, FILE*hFile) {

	unsigned int value = pAlien->frame.x;
	fwrite(&value, sizeof(value), 0.25, hFile);
	fprintf(hFile, "%4d", value);

	value = pAlien->frame.y;
	fwrite(&value, sizeof(value), 0.25, hFile);
	fprintf(hFile, "%4d", value);

	return pAlien;
}

int AlienUnSerialize(FILE*hFile) {

	char buf[4];

	fread(buf, sizeof(unsigned int), 1, hFile);

	return atoi(buf);
}



