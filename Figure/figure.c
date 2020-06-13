/*
 * figure.c
 *
 *  Created on: 15 mai 2020
 *      Author: Laymajoux Gilles Pascual
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>



//////STATIC FIGURE ENTITY SECTION----------------------------------------------------------------////

#include "figure.h"

static struct s_graphic{
	SDL_Renderer* 	pRenderer;
	SDL_Rect		gameArea;
}graphic;

int FigureInitGraphic(
		SDL_Renderer	*pRenderer,
		SDL_Rect		 gameArea
	){
	graphic.pRenderer=pRenderer;
	SDL_memcpy(&graphic.gameArea, &gameArea, sizeof(SDL_Rect));
	return 0;
}

int FigureReleaseGraphic(void){
	/**
	 * DO NOTHING
	 */
	return 0;
}


//////"DYNAMIC" FIGURE ENTITY SECTION-------------------------------------------------------------////

struct s_figure{
	SDL_Rect	frame;
	SDL_FPoint	hotPointRatio;
	SDL_Point	speed;
};

s_figure*FigureNew(
		int iLocX,
		int iLocY,
		int iWidth,
		int iHeight,
		float fHotRatioX,
		float fHotRatioY,
		int iSpeedX,
		int iSpeedY
	){

	s_figure*pNewFigure = (s_figure*)malloc(sizeof(s_figure));
	pNewFigure->frame.x = iLocX;
	pNewFigure->frame.y = iLocY;
	pNewFigure->frame.w = iWidth;
	pNewFigure->frame.h = iHeight;
	pNewFigure->hotPointRatio.x = fHotRatioX;
	pNewFigure->hotPointRatio.y = fHotRatioY;
	pNewFigure->speed.x = iSpeedX;
	pNewFigure->speed.y = iSpeedY;
	return pNewFigure;
}

s_figure*FigureDel(s_figure*pFigure){
	free(pFigure);
	return NULL;
}

s_figure*FigureDraw(s_figure*pFigure, SDL_Texture*pTexture){
	SDL_SetTextureBlendMode(pTexture, SDL_BLENDMODE_ADD);
	SDL_RenderCopy(graphic.pRenderer, pTexture, NULL, &pFigure->frame);
	return pFigure;
}

s_figure*FigureMoveToXY(s_figure*pFigure, int iLocX, int iLocY){
	pFigure->frame.x = iLocX;
	pFigure->frame.y = iLocY;
	if(!SDL_HasIntersection(&pFigure->frame,&graphic.gameArea)){
		return NULL;
	}
	return pFigure;
}

s_figure*FigureMoveX(s_figure*pFigure, int iLocX){
	pFigure->frame.x += pFigure->speed.x;

	//To remove-----------------------------------------/
	if(pFigure->frame.x < graphic.gameArea.x || (pFigure->frame.x) > (graphic.gameArea.w)){
		return NULL;
	}

	return pFigure;
}

s_figure*FigureMove(s_figure*pFigure, int iLocX, int iLocY){
	pFigure->frame.x += pFigure->speed.x;
	pFigure->frame.y += pFigure->speed.y;
	//To remove-----------------------------------------/
	if(pFigure->frame.x < graphic.gameArea.x || (pFigure->frame.x) > (graphic.gameArea.w)){
		return NULL;
	}

	return pFigure;
}

s_figure*FigureCollision(s_figure*pFigureA, s_figure*pFigureB,  t_ptfV pParam){

	if(SDL_HasIntersection(&pFigureA->frame,&pFigureB->frame)){
		pParam(pFigureB);
		return pFigureB;
	}
	return NULL;
}

SDL_Rect FigureGetCoordinates(s_figure*pFigure){
	SDL_Rect figure = pFigure->frame;
	return figure;
}

s_figure*FigureSetSize(s_figure*pFigure,int factor){
	pFigure->frame.h = factor;
	pFigure->frame.w = factor;
	return pFigure;
}
