/*
 * figure.h
 *
 *  Created on: 15 mai 2020
 *      Author: Laymajoux Gilles Pascual
 */

int FigureInitGraphic(SDL_Renderer *pRenderer, SDL_Rect gameArea);


struct s_figure;
typedef struct s_figure s_figure;

typedef void*(*t_ptfV)(void*);
typedef void*(*t_ptfVV)(void*, void*);
typedef void*(*t_ptfVVV)(void*, void*, void*);
typedef void*(*t_ptfVVVVVVVV)(void*, void*, void*, void*, void*, void*, void*, void*);

s_figure*FigureNew(
		int iLocX, int iLocY, int iWidth, int iHeight,
		float fHotRatioX, float fHotRatioY, int iSpeedX, int iSpeedY);

s_figure*FigureDel(s_figure*pFigure);

s_figure*FigureDraw(s_figure*pFigure, SDL_Texture*pTexture);

s_figure*FigureMoveToXY(s_figure*pFigure, int iLocX, int iLocY);

s_figure*FigureMove(s_figure*pFigure, int iLocX, int iLocY);

s_figure*FigureMoveX(s_figure*pFigure, int iLocX);

s_figure*FigureCollision(s_figure*pFigureA, s_figure*pFigureB, t_ptfV pParam);

SDL_Rect FigureGetCoordinates(s_figure*pFigure);

s_figure*FigureSetSize(s_figure*pFigure,int factor);
