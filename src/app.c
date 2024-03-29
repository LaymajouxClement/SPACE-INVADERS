/*
 * app.c
 *
 *  Created on: 14 mai 2020
 *      Author: Laymajoux Gilles Pascual
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <commdlg.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>


#include "figure.h"
#include "ship.h"
#include "alien.h"
#include "container.h"
#include "app.h"
#include "rocket.h"


//Private function prototypes--------------------------------------------//
Uint32 _AppAnimateCallback(Uint32 interval, void*pParam);
int _AppMessageBox(char*msg, int xpos, int ypos, int var);
int _AppGameUpdate(void * figure);
void _App_SaveProc(void);
void _App_LoadProc(void);

static struct{
	Uint32			uStatus;
	Uint32			nWindowID;
	SDL_Window 	 * 	pWindow;
	SDL_Point		ptWindowSize;
	SDL_Renderer * 	pRenderer;
	SDL_Texture	 *	pTextureBkgnd;
	SDL_TimerID		nTimerID;
	HWND			hMainDlg;

	//s_ship		 *	pShip; // to remove

	s_container	 *  pShips;
	s_container  *  pShipRockets;
	s_container  *  pAlienRockets;
	s_container  *  pAliens;
	TTF_Font     *  pFont;

	SDL_Texture	 *	pTexture;
	SDL_Surface	 *	pString;
	SDL_Color	 	colorR;
	SDL_Rect		r;
	Mix_Chunk 	 *	wave;

	unsigned int	fileTypeID_HIGH;
	unsigned int	fileTypeID_LOW;

	int 			gameState;
	int				scoreA;
	int				scoreB;
	int				hiScore;
	int 			timer;
}app;



enum APP_STATUS_FLAGS{
	ST_ALL_CLEARED 	= 0x00000000,
	ST_ALL_SETTED 	= 0xFFFFFFFF,
	ST_APP_INITIED	= 0x00000001,
	ST_APP_PAUSED	= 0x00000002,
	ST_APP_DIRTY	= 0x00000004,
};

#define SetInitied()	mBitsSet(app.uStatus,ST_APP_INITIED)
#define IsInitied()		mIsBitsSet(app.uStatus,ST_APP_INITIED)
#define IsNotInitied()	mIsBitsClr(app.uStatus,ST_APP_INITIED)

#define SetPaused()		mBitsSet(app.uStatus,ST_APP_PAUSED)
#define ClrPaused()		mBitsClr(app.uStatus,ST_APP_PAUSED)
#define TglPaused()		mBitsTgl(app.uStatus,ST_APP_PAUSED)
#define IsPaused()		mIsBitsSet(app.uStatus,ST_APP_PAUSED)
#define IsRun()			mIsBitsClr(app.uStatus,ST_APP_PAUSED)

#define SetAppDirty()				mBitsSet(app.uStatus, ST_APP_DIRTY)
#define ClrAppDirty()				mBitsClr(app.uStatus, ST_APP_DIRTY)
#define IsAppDirty()				mIsBitsSet(app.uStatus, ST_APP_DIRTY)

int AppNew(void){

	app.uStatus=ST_ALL_CLEARED;
	app.nWindowID=-1;
	app.pRenderer=NULL;
	app.pTextureBkgnd=NULL;
	app.nTimerID=-1;
	app.pAliens=ContainerNew((t_ptfV)AlienDel);
	app.pShipRockets = ContainerNew((t_ptfV)RocketDel);
	app.pAlienRockets = ContainerNew((t_ptfV)RocketDel);
	app.pShips=ContainerNew((t_ptfV)ShipDel);
	app.colorR.r = 0;
	app.colorR.g = 255;
	app.colorR.b = 0;
	app.colorR.a = 255;
	app.scoreA = 0;
	app.scoreB = 0;
	app.hiScore = 0;
	app.timer = 0;
	app.gameState = 0;
	app.wave = NULL;

	app.fileTypeID_HIGH = 0x11092001;
	app.fileTypeID_LOW = 0x13062001;

	if (TTF_Init() == -1) {
		fprintf(stderr, "TTF_Init() failed :%s\n", TTF_GetError());
		return EXIT_FAILURE;
	}
	app.pFont = TTF_OpenFont(MAIN_GAME_FONT, 20);
	if (app.pFont == NULL) {
		fprintf(stderr, "TTF_OpenFont() failed : %s\n", TTF_GetError());
		return EXIT_FAILURE;
	}
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0) {
		fprintf(stderr, "SDL video init failed ! %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ){
		return EXIT_FAILURE;
	}
	app.wave = Mix_LoadWAV(ROCKET_SOUND);
	if (app.wave == NULL)return EXIT_FAILURE;

	//Main window------------------------------------------------------------------------------//
	 app.pWindow = SDL_CreateWindow(
			MAIN_WINDOW_TITLE,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			MAIN_WINDOW_WIDTH,
			MAIN_WINDOW_HEIGHT,
			SDL_WINDOW_SHOWN
	);

	if(app.pWindow==NULL){
		SDL_Quit();
		fprintf(stderr, "SDL_CreateWindow() failed with error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	app.nWindowID=SDL_GetWindowID(app.pWindow);
	SDL_GetWindowSize(app.pWindow, &app.ptWindowSize.x, &app.ptWindowSize.y);

	//Main renderer-----------------------------------------------------------------------------//
	app.pRenderer = SDL_CreateRenderer(app.pWindow, -1, SDL_RENDERER_ACCELERATED);
	if (app.pRenderer == NULL) {
		fprintf(stderr, "Failed to create accelerated renderer.\n");
		app.pRenderer = SDL_CreateRenderer(app.pWindow, -1, SDL_RENDERER_SOFTWARE);
		if (app.pRenderer == NULL) {
			fprintf(stderr, "Failed to create software renderer.\n");
			return EXIT_FAILURE;
		}
		fprintf(stderr, "Software renderer created instead!\n");
	}

	//Main window background--------------------------------------------------------------------//
	SDL_Surface* pImageBkgnd=IMG_Load(MAIN_WINDOW_BACKGROUND_IMAGE);
	if(pImageBkgnd==NULL){
		fprintf(stderr,"Failed to load image: %s\n", SDL_GetError());
	}
	else{
		app.pTextureBkgnd=SDL_CreateTextureFromSurface(app.pRenderer, pImageBkgnd);
		SDL_FreeSurface(pImageBkgnd);
	}
	if(app.pTextureBkgnd){
		SDL_RenderCopy(app.pRenderer, app.pTextureBkgnd, NULL, NULL);
	}

	//Figures initialization section------------------------------------------------------------//
	app.r.x = MAIN_WINDOW_PADDING_HRZ;
	app.r.y = MAIN_WINDOW_PADDING_TOP+ROCKET_SIZE_Y;
	app.r.h = MAIN_WINDOW_HEIGHT - MAIN_WINDOW_PADDING_TOP - MAIN_WINDOW_PADDING_BOT- MAIN_WINDOW_PADDING_HRZ;
	app.r.w = MAIN_WINDOW_WIDTH - MAIN_WINDOW_PADDING_HRZ;

	RocketInitGraphic(app.pRenderer, ROCKET_IMAGE);
	AlienRocketInitGraphic(app.pRenderer, ALIEN_ROCKET_IMAGE);

	FigureInitGraphic(app.pRenderer, app.r);
	ShipInitGraphic(app.pRenderer, SPACESHIP_IMAGE);
	for(int m=0;m<5;m++){
		ContainerPushback(
			app.pShips,
			ShipNew(
				(app.ptWindowSize.x-SPACESHIP_SIZE)/2,
				app.ptWindowSize.y-MAIN_WINDOW_PADDING_BOT-SPACESHIP_SIZE,
				SPACESHIP_SIZE,
				SPACESHIP_SIZE,
				SPACESHIP_SIZE/2,
				0,
				0,
				0
			)
		);
	}
/*
	app.pShip = ShipNew(
			(app.ptWindowSize.x-SPACESHIP_SIZE)/2,
			app.ptWindowSize.y-MAIN_WINDOW_PADDING_BOT-SPACESHIP_SIZE,
			SPACESHIP_SIZE,
			SPACESHIP_SIZE,
			SPACESHIP_SIZE/2,
			0,
			0,
			0
	);
*/
	AlienInitGraphic(app.pRenderer, ALIEN_IMAGE);
	for(int k=0;k<5;k++){
		for(int m=0;m<5;m++){
			ContainerPushback(
				app.pAliens,
				AlienNew(
						(MAIN_WINDOW_PADDING_HRZ+(ALIEN_SIZE+ALIEN_SPACING)*m),
						(MAIN_WINDOW_PADDING_TOP+(ALIEN_SIZE+ALIEN_SPACING)*k),
						ALIEN_SIZE,
						ALIEN_SIZE,
						ALIEN_HOT_RATIO_X,
						ALIEN_HOT_RATIO_Y,
						ALIEN_SPEED_HRZ,
						0
				)
			);
		}
	}


	//Animation timer start up------------------------------------------------------------------//
	app.nTimerID = SDL_AddTimer(MAIN_ANIMATION_TICK, _AppAnimateCallback, NULL);

	return 0;
}

int AppRun(void){
	if(IsNotInitied()) return -1;

	int quit=0;
	SDL_Event event;
	do{
		while(SDL_PollEvent(&event)){
			switch(event.type){
			case SDL_MOUSEMOTION:
				if((MAIN_WINDOW_PADDING_HRZ+(SPACESHIP_SIZE/2) < event.motion.x) && ((event.motion.x+(SPACESHIP_SIZE)/2) < (MAIN_WINDOW_WIDTH - MAIN_WINDOW_PADDING_HRZ)) && ContainerCard(app.pShips) && app.gameState != 0){
					ShipMoveToXY(ContainerGetback(app.pShips),event.motion.x,0);
				}
				break;
			case SDL_QUIT:
				quit=1;
				break;
			case SDL_KEYUP:
				if(event.key.keysym.sym==SDLK_ESCAPE){
					quit=1;
				}else if(event.key.keysym.sym==SDLK_SPACE && app.gameState != 0){
					TglPaused();
				}else if(event.key.keysym.sym==SDLK_s && app.gameState != 0){
					SetPaused();
					_App_SaveProc();
				}else if(event.key.keysym.sym==SDLK_l && app.gameState != 0){
					SetPaused();
					_App_LoadProc();
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button == SDL_BUTTON_LEFT  && ContainerCard(app.pShips) && app.gameState != 0){
					if(ContainerCard(app.pShipRockets) < 3){
						SDL_Rect r = ShipGetCoordinates(ContainerGetback(app.pShips));
						ContainerPushback(app.pShipRockets, RocketNew(r.x+(r.w/2),
							app.ptWindowSize.y-MAIN_WINDOW_PADDING_BOT-SPACESHIP_SIZE,
							ROCKET_SIZE_X,
							ROCKET_SIZE_Y,
							ROCKET_SIZE_X/2,
							ROCKET_SIZE_Y/2,
							0,
							0
						));
						if ( Mix_PlayChannel(-1, app.wave, 0) == -1 )return EXIT_FAILURE;
					}
				}
				break;
			default:
				break;
			}
		}
	}while(quit!=1);
	return 0;
}

int AppDel(void){

	//Animation timer break off----------------------------------------------------------------//
	SDL_RemoveTimer(app.nTimerID); app.nTimerID=-1;

	//Figure resources releasing---------------------------------------------------------------//
	//app.pShip=ShipDel(app.pShip);
	if(app.wave){
		Mix_FreeChunk(app.wave);
	}
	Mix_CloseAudio();
	if (app.pFont) {
		TTF_CloseFont(app.pFont);
		app.pFont = NULL;
	}

	TTF_Quit();

	app.pShips = ContainerDel(app.pShips);
	ShipReleaseGraphic();

	app.pAliens = ContainerDel(app.pAliens);
	AlienReleaseGraphic();

	app.pShipRockets = ContainerDel(app.pShipRockets);
	RocketReleaseGraphic();

	app.pAlienRockets = ContainerDel(app.pAlienRockets);
	AlienRocketReleaseGraphic();


	//SDL resources releasing------------------------------------------------------------------//
	if(app.pWindow) 	{ SDL_DestroyWindow(app.pWindow); app.pWindow=NULL; }
	if(app.pRenderer) 	{ SDL_DestroyRenderer(app.pRenderer); app.pRenderer = NULL; }
	app.nWindowID = -1;

	SDL_Quit();
	return 0;
}


Uint32 _AppAnimateCallback(Uint32 interval, void*pParam) {
	if(IsPaused()){
		_AppMessageBox("PAUSE",0,MAIN_WINDOW_HEIGHT/2,-1);
		return interval;
	}

	SDL_Rect alienPos;
	///Check game state-------------------------------------------------------------------------------/
	if(app.gameState == 0){
		SDL_RenderCopy(app.pRenderer, app.pTextureBkgnd, NULL, NULL);
		_AppMessageBox("SPACE INVADERS",0,MAIN_WINDOW_HEIGHT/4-SPACESHIP_SIZE,-1);
		_AppMessageBox("NEW GAME",0,MAIN_WINDOW_HEIGHT/2-SPACESHIP_SIZE,-2);
		_AppMessageBox("LOAD GAME",0,MAIN_WINDOW_HEIGHT/2,-2);
		SDL_RenderPresent(app.pRenderer);
		return interval;
	}
	if(!ContainerCard(app.pShips)){
		_AppMessageBox("GAME OVER",0,MAIN_WINDOW_HEIGHT/2,-1);
		_AppMessageBox("YOUR SCORE:",MAIN_WINDOW_WIDTH/4,MAIN_WINDOW_HEIGHT/2+ MAIN_WINDOW_HEIGHT/5,1);
		return interval;
	}
	if(!ContainerCard(app.pAliens)){
		_AppMessageBox("SUCCESS",0,MAIN_WINDOW_HEIGHT/2,-1);
		_AppMessageBox("YOUR SCORE:",MAIN_WINDOW_WIDTH/4,MAIN_WINDOW_HEIGHT/2+ MAIN_WINDOW_HEIGHT/5,1);
		return interval;
	}
	else{
		alienPos = AlienGetCoordinates(ContainerGetback(app.pAliens));
	}
	if(alienPos.y > app.r.h - SPACESHIP_SIZE){
		_AppMessageBox("GAME OVER",0,MAIN_WINDOW_HEIGHT/2,-1);
		_AppMessageBox("YOUR SCORE:",MAIN_WINDOW_WIDTH/4,MAIN_WINDOW_HEIGHT/2+ MAIN_WINDOW_HEIGHT/5,1);
		return interval;
	}

	///Check figures positions & states-------------------------------------------------------------------------------/

	srand(time(NULL));
	SDL_RenderCopy(app.pRenderer, app.pTextureBkgnd, NULL, NULL);

	SDL_Rect ship = ShipGetCoordinates(ContainerGetback(app.pShips));
	if (ship.w  != SPACESHIP_SIZE){
		ShipSetSize(ContainerGetback(app.pShips),SPACESHIP_SIZE);
		ShipMove(ContainerGetback(app.pShips),(app.ptWindowSize.y-SPACESHIP_SIZE)/2,MAIN_WINDOW_HEIGHT-MAIN_WINDOW_PADDING_BOT-SPACESHIP_SIZE);
	}
	ShipDraw(ContainerGetback(app.pShips));

	ContainerParseDelIf(app.pShipRockets,(t_ptfVV)RocketMove, (void*)-ROCKET_SPEED);
	if(app.timer < 50)
	{
		app.timer++;
	}
	else{
		app.timer = 0;

		int random = rand() % (ContainerCard(app.pAliens));
		SDL_Rect genRocket = AlienGetCoordinates(ContainerGetat(app.pAliens, random));
		ContainerPushback(app.pAlienRockets, RocketNew(genRocket.x + ALIEN_SIZE/2,
				genRocket.y + ALIEN_SIZE/2,
				ROCKET_SIZE_X,
				ROCKET_SIZE_Y,
				ROCKET_SIZE_X/2,
				ROCKET_SIZE_Y/2,
				0,
				0
			));

	}
	ContainerParseDelIf(app.pAlienRockets,(t_ptfVV)RocketMove, (void*)ROCKET_SPEED);

	ContainerIntersectDelElemsIf(app.pShipRockets, app.pAliens, (t_ptfVVV)FigureCollision, (t_ptfV)_AppGameUpdate);
	ContainerIntersectDelElemsIf(app.pAlienRockets, app.pShips, (t_ptfVVV)FigureCollision, (t_ptfV)_AppGameUpdate);

	int scroll = 0;
	ContainerParse(app.pAliens,(t_ptfVV)AlienMove,&scroll);
	if(scroll == 1){
		ContainerParse(app.pAliens,(t_ptfVV)AlienReverseSpeedX,(void*)ALIEN_SPEED_VRT);
	}

	///Draw sprites-------------------------------------------------------------------------------/

	ContainerParse(app.pAliens,(t_ptfVV)AlienDraw, NULL);
	ContainerParse(app.pShipRockets,(t_ptfVV)RocketDraw,(void*)1);
	ContainerParse(app.pAlienRockets,(t_ptfVV)RocketDraw,NULL);

	for(int i = 0; i < ContainerCard(app.pShips) - 1; i++){
		ShipDrawLives(ContainerGetat(app.pShips, i),
				MAIN_WINDOW_WIDTH - i*0.5*MAIN_WINDOW_PADDING_HRZ - SPACESHIP_SIZE,
				MAIN_WINDOW_HEIGHT-MAIN_WINDOW_PADDING_BOT,
				SPACESHIP_SIZE/4,
				SPACESHIP_SIZE/4);
	}

	///Finality-------------------------------------------------------------------------------/

	_AppMessageBox("SCORE<1>", FONT_SCORE_X_P1, FONT_SCORES_Y, app.scoreA);
	_AppMessageBox("HI-SCORE", FONT_HIGHSCORE_X, FONT_SCORES_Y, app.hiScore);
	_AppMessageBox("SCORE<2>", FONT_SCORE_X_P2, FONT_SCORES_Y, app.scoreB);

	SDL_RenderPresent(app.pRenderer);
	return interval;
}

int _AppMessageBox(char*msg, int xpos, int ypos, int var){
	char buf[256];
	SDL_Rect rectFin;
	rectFin.x = xpos;
	rectFin.y = ypos;

	if(var == 1){
		rectFin.h = MAIN_WINDOW_HEIGHT/8;
		rectFin.w = MAIN_WINDOW_WIDTH/2;
		sprintf(buf, "%s %03d", msg,app.scoreA);
	}else if(var==-1){
		rectFin.h = MAIN_WINDOW_HEIGHT/5;
		rectFin.w = MAIN_WINDOW_WIDTH;
		sprintf(buf, "%s", msg);
	}else if(var==-2){
			rectFin.h = MAIN_WINDOW_HEIGHT/10;
			rectFin.w = MAIN_WINDOW_WIDTH/2;
			rectFin.x = MAIN_WINDOW_WIDTH/4;
			sprintf(buf, "%s", msg);
	}else{
		sprintf(buf, "%s %03d", msg, var);
		rectFin.h = FONT_HEIGHT;
		rectFin.w = FONT_WIDTH;
	}

	SDL_RenderDrawRect(app.pRenderer, &rectFin);
	SDL_RenderFillRect(app.pRenderer, &rectFin);
	app.pString = TTF_RenderText_Blended(app.pFont, buf, app.colorR);
	app.pTexture = SDL_CreateTextureFromSurface(app.pRenderer, app.pString);
	SDL_RenderCopy(app.pRenderer, app.pTexture, NULL, &rectFin);
	SDL_DestroyTexture(app.pTexture);
	SDL_FreeSurface(app.pString);
	SDL_RenderPresent(app.pRenderer);
	return 0;
}

int _AppGameUpdate(void * figure){

	if(ContainerIsElem(app.pAliens, figure)){
		app.scoreA += 100;
	}

	if(ContainerIsElem(app.pShips, figure)){
		app.scoreA -= 100;
	}
	if(ContainerIsElem(app.pShipRockets, figure)){
		app.scoreA -= 50;
	}


	return app.scoreA;
}


int MenuRun(void){

	int quit=0;
	SDL_Event event;
	do{
		while(SDL_PollEvent(&event)){
			switch(event.type){
			case SDL_QUIT:
				quit=1;
				break;
			case SDL_KEYUP:
				if(event.key.keysym.sym==SDLK_ESCAPE){
					quit=1;
				}else if(event.key.keysym.sym==SDLK_TAB){
					app.gameState =1;
					SetInitied();
					//SetAppDirty();
					return 1;
				}
				break;
			default:
				break;
			}
		}
	}while(quit!=1);
	return 0;

}

void _App_SaveProc(void) {
	char buffer[512] = "*.spi";
	char customFilter[512] = "*.spi\0";
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = app.hMainDlg;
	ofn.lpstrFile = buffer;
	//ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(buffer);
	ofn.lpstrFilter = "Space\0*.spi\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrCustomFilter = customFilter;
	ofn.nMaxCustFilter = sizeof(customFilter);
	ofn.lpstrTitle = "Save container...";
	ofn.lpstrInitialDir = "<LE CHEMIN COMPLET>\\SpaceFiles";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn)) {
		FILE * hFile;
		hFile = fopen(ofn.lpstrFile, "w");

		if (hFile == NULL) {
			MessageBox(app.hMainDlg, ofn.lpstrFile, "Unable to open or create file !", MB_OK);
			return;
		}

		unsigned int value;


		// FileTypeID_HIGH
		value = 0x01200911;
		fwrite(&value, sizeof(value), 1, hFile);

		// FileTypeID_LOW
		value = 0x01200613;
		fwrite(&value, sizeof(value), 1, hFile);

		// Extension
		fprintf(hFile, ".spi");

		value = strtol("00001001", NULL, 2);
		fwrite(&value, sizeof(value), 1, hFile);


		value =  ContainerCard(app.pAliens);
		fwrite(&value, sizeof(value), 1, hFile);

		ContainerParse(app.pAliens, (t_ptfVV)AlienSerialize, hFile);

		value =  ContainerCard(app.pShips);
		fwrite(&value, sizeof(value), 0.25, hFile);
		fprintf(hFile, "%4d", value);

		fclose(hFile);
		//ClrAppDirty();
	}
}

void _App_LoadProc(void) {
	char buffer[512] = "*.spi";
	char customFilter[512] = "*.spi\0";
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = app.hMainDlg;
	ofn.lpstrFile = buffer;
	//ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(buffer);
	ofn.lpstrFilter = "Space\0*.spi\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrCustomFilter = customFilter;
	ofn.nMaxCustFilter = sizeof(customFilter);
	ofn.lpstrTitle = "Load file to container...";
	ofn.lpstrInitialDir = "<LE CHEMIN COMPLET>\\ClientFiles";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if (GetOpenFileName(&ofn)) {
		//SetWindowText(app.hMainDlg, ofn.lpstrFile);
		FILE * hFile;
		hFile = fopen(ofn.lpstrFile, "rb");	//Open for read only : binary mode

		if (hFile == NULL) {
			MessageBox(app.hMainDlg, ofn.lpstrFile, "Unable to open or create file !", MB_OK);
			return;
		}
		//To complete...

		unsigned int fileTest;

		// FileTypeID_HIGH
		fread(&fileTest, sizeof(app.fileTypeID_HIGH), 1, hFile);
		if (fileTest != 0x01200911) {
			MessageBox(app.hMainDlg, ofn.lpstrFile, "File Incorrect !", MB_OK);
			return;
		}

		// FileTypeID_LOW
		fread(&fileTest, sizeof(app.fileTypeID_LOW), 1, hFile);
		if (fileTest != 0x01200613) {
			MessageBox(app.hMainDlg, ofn.lpstrFile, "File Incorrect !", MB_OK);
			return;
		}

		//Extension
		char buf[5];
		fread(buf, sizeof(char), 4, hFile);
		buf[4] = '\0';
		if (strcmp(buf, ".spi") != 0) {
			MessageBox(app.hMainDlg, ofn.lpstrFile, "Extension Incorrect !", MB_OK);
			return;
		}

		//NB_FIELDS
		int fields;
		fread(&fields, sizeof(int), 1, hFile);


		//NB_CLIENTS
		int nbAliens;
		int getX, getY;
		fread(&nbAliens, sizeof(int), 1, hFile);
		app.pAliens = ContainerDel(app.pAliens);
		app.pAliens = ContainerNew((t_ptfV)AlienDel);

		app.pShips = ContainerDel(app.pShips);
		app.pShips = ContainerNew((t_ptfV)AlienDel);

		for (int k = 0; k < nbAliens; k++){
			getX = AlienUnSerialize(hFile);
			getY = AlienUnSerialize(hFile);

			ContainerPushback(
				app.pAliens,
				AlienNew(
						getX,
						getY,
						ALIEN_SIZE,
						ALIEN_SIZE,
						ALIEN_HOT_RATIO_X,
						ALIEN_HOT_RATIO_Y,
						ALIEN_SPEED_HRZ,
						0
				)
			);
		}

		fread(buf, sizeof(int), 1, hFile);

		for (int k = 0; k < atoi(buf); k++){
			ContainerPushback(
				app.pShips,
				ShipNew(
					(app.ptWindowSize.x-SPACESHIP_SIZE)/2,
					app.ptWindowSize.y-MAIN_WINDOW_PADDING_BOT-SPACESHIP_SIZE,
					SPACESHIP_SIZE,
					SPACESHIP_SIZE,
					SPACESHIP_SIZE/2,
					0,
					0,
					0
				)
			);
		}


		fclose(hFile);
	}



}
