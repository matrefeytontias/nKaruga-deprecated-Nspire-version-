#ifndef INCLUDE_COMMON
#define INCLUDE_COMMON

#include <os.h>
#include <math.h>

#include "n2DLib.h"

extern "C" {
void bufcpy(void*, void*);
}

typedef int KeyEvent;

typedef int Fixed;

// KeyEvent
#define KDOWN(x) (x & 1)
#define KLEFT(x) (x & 2)
#define KRIGHT(x) (x & 4)
#define KUP(x) (x & 8)
#define KFIRE(x) (x & 16)
#define KPOLARITY(x) (x & 32)
#define KPOWER(x) (x & 64)
#define KQUIT(x) (x & 128)
#define K4(x) (x & 256)
#define K5(x) (x & 512)
#define K7(x) (x & 1024)
#define K8(x) (x & 2048)

extern KeyEvent getk(void);

// Bullets

#define LIGHT 0
#define SHADOW 1
#define SWITCHING0 2
#define SWITCHING1 4

#define MAX_BULLET 400
#define MAX_FRAGMENT 12
#define MAX_LASER 4
#define FRAGMENT_TRAILING 7
// 12*10
#define MAX_POWER 120

#define LASER_SPEED 2
#define LASER_THICKNESS 25

// Both enemies and player can fire them
class Bullet
{
public:
	Bullet();
	~Bullet();
	Rect* makeRect();
	bool isActive();
	void activate(Fixed, Fixed, Fixed, Fixed, int, bool, bool);
	void deactivate();
	bool getPolarity();
	bool hurtsPlayer();
	bool handle();
	void draw();
	// x, y on-screen
	Fixed x, y;
	// speed
	Fixed dx, dy;
	unsigned short *img;
private:
	bool active;
	bool polarity;
	bool hurtPlayer;
};

// Player
// We need it in BulletArray, and Player needs BulletArray
class Player;

// Funnily enough, we also need Enemy in BulletArray, and vice-versa
class Enemy;

// Both enemies and player can fire them
class PowerFragment
{
public:
	PowerFragment();
	~PowerFragment();
	bool isActive();
	void activate(Fixed, Fixed, Fixed, Player*, bool, bool);
	void deactivate();
	bool getPolarity();
	bool hurtsPlayer();
	bool handle();
	void draw();
	Fixed x;
	Fixed y;
	// Polar coordinates of speed
	Fixed speed;
	Fixed angle;
	Enemy *targetE;
	Player *targetP;
private:
	Fixed previousX[FRAGMENT_TRAILING];
	Fixed previousY[FRAGMENT_TRAILING];
	bool polarity;
	bool active;
	bool hurtPlayer;
	// Determine if the initial angle has been reached once already
	bool hasReachedAngle;
	int skipPositionRecord;
};

// Only enemies can fire them
class Laser
{
public:
	Laser();
	~Laser();
	bool isActive();
	void activate(Enemy*, bool);
	void deactivate();
	void handle();
	void draw();
	bool getPolarity();
	Rect* getVector();
	void getSides(Rect*, Rect*);
	int getAmplitude();
	void setAmplitude(int);
	Fixed angle;
	Enemy *origin;
	int x;
	int y;
private:
	bool active;
	bool polarity;
	// Lasers are not immediate, they grow
	Fixed amplitude;
};

// BulletArray
class BulletArray
{
public:
	BulletArray();
	~BulletArray();
	void handle(Player*);
	void add(Fixed, Fixed, Fixed, Fixed, int, bool, bool);
	void add_homing(Fixed, Fixed, Fixed, Player*, bool, bool);
	void fire_laser(Enemy*, bool);
	void deactivate(int);
	void deactivate_homing(int);
	void stop_laser(Enemy*);
	Bullet data[MAX_BULLET];
	PowerFragment data_homing[MAX_FRAGMENT];
	Laser data_laser[MAX_LASER];
private:
	// keep track of current bullet ...
	int bulletCount;
	// ... power fragment ...
	int homingCount;
	// ... and laser
	int laserCount;
};

// Player
class Player
{
public:
	Player();
	~Player();
	void handle(KeyEvent, BulletArray*);
	bool getPolarity();
	void switchPolarity();
	void hurt();
	int getLives();
	bool isDying();
	// x, y on-screen
	Fixed x, y;
	// Ship images, light and shadow
	// First two are "normal" ship
	// Other four are polarity transition animation frames (frame 0 light/shadow, frame 1 light/shadow)
	// images have same dimensions
	unsigned short *img[6];
private:
	int isSwitchingPolarity;
	// keep firing at a reasonable rate
	int fireDelay;
	// True is light, false is shadow
	bool polarity;
	// Key repeat handlers
	bool fireRepeat;
	bool polarityRepeat;
	bool dying;
	int lives;
} ;

// Enemy
// For real this time
#define MAX_ENEMY 36

class Enemy
{
public:
	Enemy();
	~Enemy();
	void handle(Player*, BulletArray*);
	bool isActive();
	void activate(int, int, int, int, int, int, bool, bool, int);
	void deactivate();
	void damage(Player*, bool, int, BulletArray*);
	void joint(int, Fixed, Fixed);
	Fixed getRotation();
	void setRotation(Fixed);
	bool getPolarity();
	int getWaveIndex();
	int getInternal(int);
	void setInternal(int, int);
	Fixed getx();
	Fixed gety();
	// x, y on-screen
	Fixed x, y;
	// Enemy image
	unsigned short *img;
	bool diedThisFrame;
private:
	bool active;
	int HP;
	// Does the enemy use rotation (achieved in the pattern)
	bool hasRotation;
	Fixed rotationAngle;
	// What bullets does the enemy fire
	bool polarity;
	// The enemy's behaviour
	int callback;
	// The position of the enemy in the wave
	int waveIndex;
	// Internal data for free use by the behaviour code
	int internal[6];
	// Used to constraint an enemy to another
	bool isJointed;
	int jointedTo;
	Fixed jointX;
	Fixed jointY;
	// How many bullets does the enemy fireback when dying
	int fireback;
};

// Used to hold information on killed enemies in order to get the position for ChainNotifs
class DestroyedEnemies
{
public:
	DestroyedEnemies();
	~DestroyedEnemies();
	void activate(Enemy*, int);
	int x[MAX_ENEMY];
	int y[MAX_ENEMY];
	bool relevant[MAX_ENEMY];
};

// Explosion animations
class ExplosionAnim
{
public:
	ExplosionAnim();
	~ExplosionAnim();
	void activate(int, int, bool);
	void handle();
private:
	int x;
	int y;
	int counter;
	bool polarity;
};

// Particles
// Because they are pretty *_*
#define MAX_PARTICLE 128
class Particles
{
public:
	Particles();
	~Particles();
	void add(Fixed, Fixed, Fixed, Fixed, bool);
	void handle();
private:
	Fixed x[MAX_PARTICLE];
	Fixed y[MAX_PARTICLE];
	Fixed dx[MAX_PARTICLE];
	Fixed dy[MAX_PARTICLE];
	int time[MAX_PARTICLE];
	bool polarity[MAX_PARTICLE];
	int counter;
};

// Text notifications of score-chaining
class ChainNotif
{
public:
	ChainNotif();
	~ChainNotif();
	void activate(int, int, int);
	void handle();
private:
	int x, backupX;
	int y;
	// Number to display
	int value;
	bool maxChain;
	// Stay visible for a certain amount of frames
	int untilDeath;
	int counter;
};

// Level streams
// x, y, HP, image ID, callback ID, polarity, has rotation ?, fireback
#define enemy(x, y, HP, iID, cbID, p, hR, f) x, y, HP, iID, cbID, p, hR, f

// Special values
#define LVLSTR_END -2
#define LVLSTR_CMD -1

// Commands
#define LVLSTR_NEWWAVE 0
#define LVLSTR_WAIT 1
#define LVLSTR_KILLED 2
#define LVLSTR_CHAPTER 3
#define LVLSTR_JOINT 4
#define LVLSTR_BKPT 5

#define cmd_newWave LVLSTR_CMD, LVLSTR_NEWWAVE
#define cmd_wait(x) LVLSTR_CMD, LVLSTR_WAIT, x
#define cmd_killed LVLSTR_CMD, LVLSTR_KILLED
#define cmd_startChapter(n) LVLSTR_CMD, LVLSTR_CHAPTER, n
#define cmd_joint(which, to, x, y) LVLSTR_CMD, LVLSTR_JOINT, which, to, x, y
#define cmd_bkpt LVLSTR_CMD, LVLSTR_BKPT

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
	image_LUT_player_homing_bullet_light_0,
	image_LUT_player_homing_bullet_light_1,
	image_LUT_player_homing_bullet_light_2,
	image_LUT_player_homing_bullet_shadow_0,
	image_LUT_player_homing_bullet_shadow_1,
	image_LUT_player_homing_bullet_shadow_2,
	image_LUT_enemy_bullet_0_light,
	image_LUT_enemy_bullet_0_shadow,
	image_LUT_enemy_bullet_1_light,
	image_LUT_enemy_bullet_1_shadow,
	image_LUT_enemy_bullet_2_light,
	image_LUT_enemy_bullet_2_shadow,
	image_LUT_enemy_laser_light,
	image_LUT_enemy_laser_shadow,
	image_LUT_enemy_ship_0_light,
	image_LUT_enemy_ship_0_shadow,
	image_LUT_enemy_ship_1_light,
	image_LUT_enemy_ship_1_shadow,
	image_LUT_enemy_ship_2_light,
	image_LUT_enemy_ship_2_shadow,
	image_LUT_enemy_ship_3_light,
	image_LUT_enemy_ship_3_shadow,
	image_LUT_enemy_ship_4_light,
	image_LUT_enemy_ship_4_shadow,
	image_LUT_enemy_ship_4_lightball,
	image_LUT_enemy_ship_4_shadowball,
	image_LUT_chain_hit_light,
	image_LUT_chain_hit_shadow,
	image_LUT_explosion_light_0,
	image_LUT_explosion_light_1,
	image_LUT_explosion_light_2,
	image_LUT_explosion_light_3,
	image_LUT_explosion_light_4,
	image_LUT_explosion_light_5,
	image_LUT_explosion_shadow_0,
	image_LUT_explosion_shadow_1,
	image_LUT_explosion_shadow_2,
	image_LUT_explosion_shadow_3,
	image_LUT_explosion_shadow_4,
	image_LUT_explosion_shadow_5,
	image_LUT_particle_light,
	image_LUT_particle_shadow,
	image_LUT_powerslot,
	image_LUT_background,
	image_LUT_titleScreen,
	NB_IMAGES
};

enum
{
	Pattern_1_1,
	Pattern_1_2,
	Pattern_1_3,
	Pattern_1_4,
	Pattern_1_5,
	Pattern_1_6,
	Pattern_1_7,
	Pattern_1_8,
	Pattern_1_9,
	Pattern_1_10,
	Pattern_1_11,
	Pattern_1_12,
	Pattern_1_13,
	Pattern_1_14,
	Pattern_1_15,
	Pattern_1_16,
	Pattern_1_17,
	Pattern_1_18,
	NB_CALLBACKS
};

extern unsigned short *image_entries[NB_IMAGES];

extern void buildGameLUTs();
extern void freeGameLUTs();

extern Enemy **enemiesArray;

// Global vars
extern int G_skipFrame, G_waveTimer, G_killedThisFrame[MAX_ENEMY], G_frameChainOffset, G_chainStatus;
extern int G_score, G_power;
extern bool G_usingTouchpad;
extern touchpad_info_t *G_tpinfo;
extern touchpad_report_t G_tpstatus;
extern Enemy *G_enemiesArray[MAX_ENEMY];
extern Particles *G_particles;

// Utils
extern Fixed angleToPlayer(Enemy*, Player*);
extern Fixed angleToEnemy(PowerFragment*, Enemy*);
extern Fixed angleToPlayer(PowerFragment*, Player*);
extern Enemy* findNearestEnemy(Fixed, Fixed);

#endif