#ifndef INCLUDE_COMMON
#define INCLUDE_COMMON

#include <os.h>
#include <math.h>

#include "n2DLib.h"

typedef int KeyEvent;

typedef int Fixed;

// KeyEvent
#define KDOWN(x) (x & 1)
#define KLEFT(x) (x & 2)
#define KRIGHT(x) (x & 4)
#define KUP(x) (x & 8)
#define KESC(x) (x & 16)
#define KTAB(x) (x & 32)
#define KHOME(x) (x & 64)
#define KQUIT(x) (x & 128)

extern KeyEvent getk(void);

// Bullets

#define LIGHT 0
#define SHADOW 1
#define SWITCHING0 2
#define SWITCHING1 4
#define MAX_BULLETS 64

class Bullet
{
public:
	Bullet();
	~Bullet();
	Rect* makeRect();
	bool isActive();
	void activate(Fixed, Fixed, Fixed, Fixed, bool);
	void deactivate();
	bool getPolarity();
	// x, y on-screen
	Fixed x, y;
	// speed
	Fixed dx, dy;
private:
	bool active;
	bool polarity;
};

// Player
// We need it in BulletArray, and Player needs BulletArray
class Player;

// Funnily enough, we also need Enemy in BulletArray, and vice-versa
class Enemy;

// BulletArray
class BulletArray
{
public:
	BulletArray(int, int);
	~BulletArray();
	void handle(Player*, bool, Enemy**);
	void setImage(int);
	void add(Fixed, Fixed, Fixed, Fixed, bool);
private:
	Bullet* data;
	// Bullet images, light and shadow
	// Both images have same dimensions
	unsigned short *img[2];
	// keep track of current bullet
	int bulletCount;
	int maxBullet;
};

// Player
class Player
{
public:
	Player(int);
	~Player();
	void handle(KeyEvent, Enemy**);
	bool getPolarity();
	void switchPolarity();
	void hurt();
	int getLives();
	// x, y on-screen
	Fixed x, y;
	// Ship images, light and shadow
	// First two are "normal" ship
	// Other four are polarity transition animation frames (frame 0 light/shadow, frame 1 light/shadow)
	// images have same dimensions
	unsigned short *img[6];
private:
	// bullet array
	BulletArray bArray;
	int isSwitchingPolarity;
	// keep firing at a reasonable rate
	int fireDelay;
	// True is light, false is shadow
	bool polarity;
	// Key repeat handlers
	bool fireRepeat;
	bool polarityRepeat;
	int lives;
} ;

// You know what's even funnier ? Enemy is also needed in EnemyPattern

// EnemyPattern
class EnemyPattern
{
public:
	EnemyPattern();
	~EnemyPattern();
	void set(void (*func)(Enemy*, Player*));
	void execute(Enemy*, Player*);
private:
	void (*callback)(Enemy*, Player*);
};

// Enemy
// For real this time
#define MAX_ENEMY 32

class Enemy
{
public:
	Enemy();
	~Enemy();
	void handle(Player*);
	bool isActive();
	void activate(int, int, int, int, int, bool, bool);
	void deactivate();
	void damage(bool);
	Fixed getRotation();
	void setRotation(Fixed);
	bool getPolarity();
	int getWaveIndex();
	int getInternal(int);
	void setInternal(int, int);
	// x, y on-screen
	Fixed x, y;
	// Enemy image
	unsigned short *img;
	BulletArray bArray;
private:
	bool active;
	int HP;
	// Does the enemy use rotation (achieved in the pattern)
	bool hasRotation;
	Fixed rotationAngle;
	// What bullets does the enemy fire
	bool polarity;
	EnemyPattern callback;
	// The position of the enemy in the wave
	int waveIndex;
	int internal[3];
};

// Level streams
#define enemy(HP, iID, biID, cbID, p, hR) HP, iID, biID, cbID, p, hR

// Special values
#define LVLSTR_END -2
#define LVLSTR_CMD -1

// Commands
#define LVLSTR_NEWWAVE 0
#define LVLSTR_WAIT 1
#define LVLSTR_KILLED 2

#define cmd_newWave LVLSTR_CMD, LVLSTR_NEWWAVE
#define cmd_wait(x) LVLSTR_CMD, LVLSTR_WAIT, x
#define cmd_killed LVLSTR_CMD, LVLSTR_KILLED

/* 
 * Level streams structure
 * x, y, HP, imageID, bulletImgID, callbackID, polarity, hasRotation
 * or
 * LVLSTR_CMD, command [, value]
*/

// LUT-related

enum image_LUT
{
	image_LUT_player_ship_light,
	image_LUT_player_ship_shadow,
	image_LUT_player_ship_polarityswitch_0_light,
	image_LUT_player_ship_polarityswitch_0_shadow,
	image_LUT_player_ship_polarityswitch_1_light,
	image_LUT_player_ship_polarityswitch_1_shadow,
	image_LUT_player_bullet_light,
	image_LUT_player_bullet_shadow,
	image_LUT_enemy_bullet_0_light,
	image_LUT_enemy_bullet_0_shadow,
	image_LUT_enemy_bullet_1_light,
	image_LUT_enemy_bullet_1_shadow,
	image_LUT_enemy_ship_0_light,
	image_LUT_enemy_ship_0_shadow,
	NB_IMAGES
};

enum callback_LUT
{
	callback_LUT_0,
	callback_LUT_1,
	callback_LUT_2,
	callback_LUT_3,
	NB_CALLBACKS
};

extern unsigned short *image_entries[NB_IMAGES];
extern void (*callback_entries[NB_CALLBACKS])(Enemy*, Player*);

extern void buildGameLUTs();
extern void freeGameLUTs();

extern Enemy **enemiesArray;

#endif