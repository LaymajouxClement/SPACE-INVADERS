/*
 * app.h
 *
 *  Created on: 14 mai 2020
 *      Author: Gilles Laymajoux Pascual
 */

#define MAIN_WINDOW_SCALE				(2.0/1.0)
#define MAIN_WINDOW_WIDTH				(600*MAIN_WINDOW_SCALE)
#define MAIN_WINDOW_HEIGHT				(400*MAIN_WINDOW_SCALE)
#define MAIN_WINDOW_TITLE				"Space Invaders"
#define MAIN_WINDOW_BACKGROUND_IMAGE	"./Resources/Images/galaxy.jpeg"

#define MAIN_WINDOW_PADDING_BOT			(30*MAIN_WINDOW_SCALE)
#define MAIN_WINDOW_PADDING_HRZ			(30*MAIN_WINDOW_SCALE)
#define MAIN_WINDOW_PADDING_TOP			(30*MAIN_WINDOW_SCALE)

#define MAIN_ANIMATION_TICK				(20)

#define MAIN_GAME_FONT 					"./Resources/Fonts/PixelEmulator-xq08.ttf"

#define SPACESHIP_IMAGE					"./Resources/Images/spaceship.jpeg"
#define SPACESHIP_SIZE					(100)


#define ALIEN_IMAGE					"./Resources/Images/alien.jpeg"
#define ALIEN_SIZE						(50)
#define ALIEN_SPACING					(10)
#define ALIEN_HOT_RATIO_X				(1.0) // A MODIFIER
#define ALIEN_HOT_RATIO_Y				(1.0) // A MODIFIER
#define ALIEN_SPEED_HRZ					(2) // A MODIFIER
#define ALIEN_SPEED_VRT					30

#define ROCKET_IMAGE					"./Resources/Images/rocket.jpeg"
#define ALIEN_ROCKET_IMAGE				"./Resources/Images/laser.jpeg"
#define ROCKET_SIZE_X						(5)
#define ROCKET_SIZE_Y						(25)
#define ROCKET_SPEED						(10)
//#define ROCKET_HOT_RATIO_X				(1.0) // A MODIFIER
//#define ROCKET_HOT_RATIO_Y				(1.0) // A MODIFIER
//#define ROCKET_SPEED_HRZ					(1) // A MODIFIER


#define FONT_HEIGHT 						(20)
#define FONT_WIDTH							(200)
#define FONT_SCORES_Y						(5)
#define FONT_SCORE_X_P1						(20)
#define FONT_SCORE_X_P2						(980)
#define FONT_HIGHSCORE_X					(550)

#define ROCKET_SOUND				"./Resources/Sounds/shoot.wav"

#define mBitsSet(f,m)			((f)|=(m))
#define mBitsClr(f,m)			((f)&=(~(m)))
#define mBitsTgl(f,m)			((f)^=(m))
#define mBitsMsk(f,m)			((f)& (m))
#define mIsBitsSet(f,m)			(((f)&(m))==(m))
#define mIsBitsClr(f,m)			(((~(f))&(m))==(m))

int AppNew(void);
int AppRun(void);
int AppDel(void);
int MenuRun(void);
