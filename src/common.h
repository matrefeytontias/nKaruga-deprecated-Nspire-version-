#ifndef INCLUDE_COMMON
#define INCLUDE_COMMON

#include <libndls.h>

#include "n2DLib.h"

#define UNUSED(var) (void)var

// Add those functions for C++ compiler under Linux
#if defined(__cplusplus) && !defined(min) && !defined(max)
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

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
#define KPAUSE(x) (x & 4096)

extern KeyEvent getk();

// Bullets

#define LIGHT 0
#define SHADOW 1
#define SWITCHING0 2
#define SWITCHING1 4

#define MAX_BULLET 400
#define MAX_HOMING 12
#define HOMING_TRAILING 15
#define MAX_FRAGMENT 12
#define MAX_LASER 4
#define FRAGMENT_TRAILING 7
// 12*10
#define MAX_POWER 120

#define LASER_SPEED 3
#define LASER_THICKNESS 25

#define MAX_DISPLAYABLE 1000

class Entity
{
public:
	Entity();
	~Entity();
	Fixed getx();
	Fixed gety();
	void setx(Fixed);
	void sety(Fixed);
	bool isActive();
	void activate();
	void deactivate();
protected:
	bool active;
	bool isEnemy;
	Fixed x, y;
};

typedef struct
{
	int absX, absY;
	int relX, relY;
} Camera;

typedef void (*cameraTravelling)(Camera*);

class DrawingCandidate
{
public:
	DrawingCandidate();
	~DrawingCandidate();
	void activate(unsigned short *img, Rect *pos, int camRelation);
	void activate(unsigned short *img, Rect *pos, Rect *center, Fixed angle, int camRelation);
	void deactivate();
	void draw();
private:
	Rect pos, center;
	bool rotates, centered, active;
	// How the camera affects the object's display
	// See the enum lower in the file
	int camRel;
	Fixed angle;
	unsigned short *img;
};

class DrawingCandidates
{
public:
	DrawingCandidates();
	~DrawingCandidates();
	void add(unsigned short *img, Rect *pos, int camRelation);
	void add(unsigned short *img, Rect *pos, Rect *center, Fixed angle, int camRelation);
	void flush();
	void loadCameraPath(int id);
	Camera cam;
private:
	DrawingCandidate data[MAX_DISPLAYABLE];
	int candidatesCount;
	cameraTravelling cameraPath;
};

// Both enemies and player can fire them
class Bullet : public Entity
{
public:
	Bullet();
	~Bullet();
	Rect* makeRect();
	void activate(Fixed x, Fixed y, Fixed dx, Fixed dy, int imageID, bool polarity, bool hurtsPlayer, int camRel);
	bool getPolarity();
	bool hurtsPlayer();
	bool handle();
	void draw();
	// speed
	Fixed dx, dy;
	unsigned short *img;
protected:
	bool polarity;
	bool hurtPlayer;
};

class Player;

// Only enemies can fire them
// So hurtPlayer is always true
class Homing : public Bullet
{
public:
	Homing();
	~Homing();
	void activate(Fixed x, Fixed y, Fixed initialAngle, Player *target, bool polarity);
	bool handle();
	void draw();
	Player* target;
private:
	Fixed previousX[HOMING_TRAILING];
	Fixed previousY[HOMING_TRAILING];
	Fixed angle;
	int aimTimer;
};

class Enemy;
class BossEnemy;

// Both enemies and player can fire them
class PowerFragment : public Bullet
{
public:
	PowerFragment();
	~PowerFragment();
	void activate(Fixed x, Fixed y, Fixed initialAngle, Player *target, bool polarity, bool hurtsPlayer);
	bool handle();
	void draw();
	// Polar coordinates of speed
	Fixed speed;
	Fixed angle;
	Enemy *targetE;
	Player *targetP;
	BossEnemy *targetB;
private:
	Fixed previousX[FRAGMENT_TRAILING];
	Fixed previousY[FRAGMENT_TRAILING];
	// Determine if the initial angle has been reached once already
	bool hasReachedAngle;
	int skipPositionRecord;
};

// Only enemies can fire them
class Laser : public Bullet
{
public:
	Laser();
	~Laser();
	void activate(Enemy *origin, bool polarity, Fixed angOffset);
	void handle();
	void draw();
	Rect* getVector();
	void getSides(Rect *side1, Rect *side2);
	int getAmplitude();
	void setAmplitude(int amp);
	Fixed angle;
	Enemy *origin;
private:
	// Lasers are not immediate, they grow
	Fixed amplitude;
	Fixed angleOffset;
};

class BossEnemy;

#define SCORE_HIT 20
#define SCORE_HIT_OP 40
#define SCORE_ABSORB 100

// BulletArray
class BulletArray
{
public:
	BulletArray();
	~BulletArray();
	void handle(Player *player, BossEnemy *be);
	void add(Fixed x, Fixed y, Fixed dx, Fixed dy, int imageID, bool polarity, bool hurtsPlayer, int camRel);
	void add_fragment(Fixed x, Fixed y, Fixed initialAngle, Player *target, bool polarity, bool hurtsPlayer);
	void add_homing(Fixed x, Fixed y, Fixed initialAngle, Player *target, bool polarity);
	void fire_laser(Enemy *origin, bool polarity, Fixed angleOffset);
	void deactivate(int offset);
	void deactivate_fragment(int offset);
	void deactivate_homing(int offset);
	void stop_laser(Enemy *origin);
	void clear();
private:
	Bullet data[MAX_BULLET];
	PowerFragment data_fragment[MAX_FRAGMENT];
	Homing data_homing[MAX_HOMING];
	Laser data_laser[MAX_LASER];
	// keep track of current bullet ...
	int bulletCount;
	// ... power fragment ...
	int fragmentCount;
	// ... homing ...
	int homingCount;
	// ... and laser
	int laserCount;
};

// Player
class Player : public Entity
{
public:
	Player();
	~Player();
	void reset();
	void handle(KeyEvent k, BulletArray *bArray);
	bool getPolarity();
	void switchPolarity();
	void hurt();
	int getLives();
	void setLives(int);
	bool isHurtable();
	// Ship images, light and shadow
	// First two are "normal" ship
	// Other four are polarity transition animation frames (frame 0 light/shadow, frame 1 light/shadow)
	// images have same dimensions
	unsigned short *img[6];
	int deathCounter;
private:
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

// Enemy
// For real this time
#define MAX_ENEMY 46

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();
	void handle(Player *player, BulletArray *bArray);
	void activate(int x, int y, int HP, int shipImgID, int callbackID, int waveIndex, bool polarity, bool hasRotation, int firebackAmount, bool ghost, bool prop);
	void damage(Player *player, bool polarity, int amount, BulletArray *bArray);
	void joint(int offset, Fixed x, Fixed y, bool diesWithJoint);
	Fixed getRotation();
	void setRotation(Fixed angle);
	bool getPolarity();
	int getWaveIndex();
	bool isGhost();
	Fixed getx();
	Fixed gety();
	int getCamRel();
	// x, y on-screen
	// Enemy image
	unsigned short *img;
	bool diedThisFrame;
private:
	// Ghost enemies have no interaction with anything
	bool ghost;
	// Props have special properties, like being unkillable
	bool prop;
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
	bool diesWithJoint;
	int jointedTo;
	Fixed jointX;
	Fixed jointY;
	// How many bullets does the enemy fireback when dying
	int fireback;
};

/* Bosses' data' format
 * - Number of patterns
 * - For each pattern :
 *   - Amount of HP
 *   - Collision callback ID (called to test collision between the boss and bullets, it can change depending on the boss's state)
 * - Array of every needed images
 * 
 * The callback is supposed to handle all patterns by testing the amount of HP.
 * Exceptionally, drawing should be taken care of in the callback using drawing candidates since bosses have complex graphical behaviors.
 */

// Initialization callback, handling pre-battle cinematics and such
// Inputs :
// - boss enemy to initialize
// Output : none
typedef void (*boss_icb)(BossEnemy*);
// Behaviour callback, handling the boss's patterns during battle
// Inputs :
// - boss enemy to handle
// - player object for aiming
// - bullet array for firing bullets
// Output : none
typedef void (*boss_cb)(BossEnemy*, Player*, BulletArray*);
// Collision callback, handling whether or not the boss is being hit by a bullet
// Inputs :
// - boss enemy to test
// - bullet object
// - amount of damage to deal if the boss is hit
typedef int (*boss_ccb)(BossEnemy*, Bullet*, int);
// Distance callback, handling distance calcualtion for when the boss is being aimed by power fragments
// Inputs :
// - boss enemy to test
// - power fragment object
// Output : distance to boss
typedef Fixed (*boss_dcb)(BossEnemy*, PowerFragment*);
// Angle callback, handling angle calculation for when the boss is being aimed by power fragments
// Inputs :
// - boss enemy to test
// - power fragment object
// Output : angle to boss
typedef Fixed (*boss_acb)(BossEnemy*, PowerFragment*);

typedef struct
{
	int HP;
	boss_cb callback;
	// Every following member contains this amount of data, because it's pattern-specific
	int patternsNb;
	// Self-explanatory
	int *HPperPattern;
	int *timeoutPerPattern;
	boss_icb *initCallbacks;
	boss_ccb *collisionCallbacks;
	boss_dcb *distanceCallbacks;
	boss_acb *angleCallbacks;
} BossData;

// The really big bad one
class BossEnemy : public Entity
{
public:
	BossEnemy();
	~BossEnemy();
	void activate(BossData *d); // drawing is taken care of by the callback code
	int handle(Player *p, BulletArray *bArray);
	void damage(int amount);
	bool isHurtable();
	void makeHurtable();
	void setInternal(int offset, int value);
	void incInternal(int offset);
	void decInternal(int offset);
	int getInternal(int offset);
	int getTimeout();
	Fixed getDistance(PowerFragment*);
	Fixed getAngle(PowerFragment*);
	Fixed angle;
	int HP;
	int maxHP;
	int currentPattern;
	int lastPattern;
	int *HPperPattern;
	int *timeoutPerPattern;
	int patternsNb;
	bool readyToGo;
	bool initCallbackCalled;
	boss_icb *initCallbacks;
	boss_cb callback;
	boss_ccb *collisionCallbacks;
	boss_dcb *distanceCallbacks;
	boss_acb *angleCallbacks;
private:
	int remainingTime;
	bool *hurtable;
	// LOTS of internal registers
	int internal[32];
};

// Used to hold information on killed enemies in order to get the position for ChainNotifs
class DestroyedEnemies
{
public:
	DestroyedEnemies();
	~DestroyedEnemies();
	void activate(Enemy *origin, int offset);
	void clear();
	int x[MAX_ENEMY];
	int y[MAX_ENEMY];
	bool relevant[MAX_ENEMY];
};

// Explosion animations
class ExplosionAnim : public Entity
{
public:
	ExplosionAnim();
	~ExplosionAnim();
	void activate(int, int, bool);
	void handle();
private:
	int counter;
	bool polarity;
};

class EnemiesArray
{
public:
	EnemiesArray();
	~EnemiesArray();
	void add(int x, int y, int HP, int shipImgID, int callbackID, int waveIndex, bool polarity, bool hasRotation, int firebackAmount, bool ghost, bool prop);
	void handle(Player *p, BulletArray *bArray);
	void handleExplosions();
	void resetEnemyCounter();
	void destroyAllEnemies(Player *p, BulletArray *bArray);
	Enemy data[MAX_ENEMY];
	DestroyedEnemies deadEnemies;
private:
	ExplosionAnim explosionsAnims[MAX_ENEMY];
	int currentExplosion;
	int currentEnemy;
};

// Particles
// Because they are pretty *_*
#define MAX_PARTICLE 256
#define PARTICLE_RADIUS 4
class Particles
{
public:
	Particles();
	~Particles();
	void add(Fixed x, Fixed y, Fixed angle, bool polarity, int lifetime);
	void handle();
private:
	Fixed x[MAX_PARTICLE];
	Fixed y[MAX_PARTICLE];
	Fixed dx[MAX_PARTICLE];
	Fixed dy[MAX_PARTICLE];
	int time[MAX_PARTICLE];
	int dt[MAX_PARTICLE];
	bool polarity[MAX_PARTICLE];
	int counter;
};

// Text notifications of score-chaining
class ChainNotif : public Entity
{
public:
	ChainNotif();
	~ChainNotif();
	void activate(int x, int y, int value);
	void handle();
private:
	Fixed backupX;
	// Number to display
	int value;
	bool maxChain;
	// Stay visible for a certain amount of frames
	int untilDeath;
	int counter;
};

// Level streams
// x, y, HP, image ID, callback ID, polarity, has rotation ?, fireback amount, follows absolute camera ?
#define enemy(x, y, HP, iID, cbID, p, hR, f, absCam) x, y, HP, iID, cbID, p, hR, f, absCam

// Game phases
enum
{
	PHASE_GAME,
	PHASE_TRANSITION,
	PHASE_BOSSCINEMATIC,
	PHASE_BOSSFIGHT,
	PHASE_BOSSEXPLODEINIT,
	PHASE_BOSSEXPLODE,
	PHASE_RESULTS,
};

// Special values
#define LVLSTR_CHAPTEREND -3
#define LVLSTR_END -2
#define LVLSTR_CMD -1

// Commands
enum
{
	LVLSTR_NEWWAVE,
	LVLSTR_NEWCAMERA,
	LVLSTR_WAIT,
	LVLSTR_KILLED,
	LVLSTR_REINIT,
	LVLSTR_CHAPTER,
	LVLSTR_JOINT,
	LVLSTR_BOSS,
	LVLSTR_BKPT
};

#define cmd_newWave LVLSTR_CMD, LVLSTR_NEWWAVE
#define cmd_newCameraPath(n) LVLSTR_CMD, LVLSTR_NEWCAMERA, n
#define cmd_wait(x) LVLSTR_CMD, LVLSTR_WAIT, x
#define cmd_killed LVLSTR_CMD, LVLSTR_KILLED
#define cmd_newChapter(n) LVLSTR_CMD, LVLSTR_REINIT, n
#define cmd_startChapter(n) LVLSTR_CMD, LVLSTR_CHAPTER, n
#define cmd_joint(which, to, x, y, jointDependant) LVLSTR_CMD, LVLSTR_JOINT, which, to, x, y, jointDependant
#define cmd_bkpt LVLSTR_CMD, LVLSTR_BKPT
#define cmd_fightBoss(n) LVLSTR_CMD, LVLSTR_BOSS, n

// LUT-related

// Base images LUT
enum
{
	image_LUT_player_ship_light,
	image_LUT_player_ship_shadow,
	image_LUT_player_ship_polarityswitch_0_light,
	image_LUT_player_ship_polarityswitch_0_shadow,
	image_LUT_player_ship_polarityswitch_1_light,
	image_LUT_player_ship_polarityswitch_1_shadow,
	image_LUT_player_ship_invincible_light,
	image_LUT_player_ship_invincible_shadow,
	image_LUT_player_bullet_light,
	image_LUT_player_bullet_shadow,
	image_LUT_player_homing_bullet_light_0,
	image_LUT_player_homing_bullet_light_1,
	image_LUT_player_homing_bullet_light_2,
	image_LUT_player_homing_bullet_shadow_0,
	image_LUT_player_homing_bullet_shadow_1,
	image_LUT_player_homing_bullet_shadow_2,
	image_LUT_player_explosion_0,
	image_LUT_player_explosion_1,
	image_LUT_player_explosion_2,
	image_LUT_player_explosion_3,
	image_LUT_player_explosion_4,
	image_LUT_player_explosion_5,
	image_LUT_player_explosion_6,
	image_LUT_player_explosion_7,
	image_LUT_player_explosion_8,
	image_LUT_player_explosion_9,
	image_LUT_player_explosion_10,
	image_LUT_player_explosion_11,
	image_LUT_enemy_bullet_0_light,
	image_LUT_enemy_bullet_0_shadow,
	image_LUT_enemy_bullet_1_light,
	image_LUT_enemy_bullet_1_shadow,
	image_LUT_enemy_bullet_2_light,
	image_LUT_enemy_bullet_2_shadow,
	image_LUT_enemy_homing_bullet_light,
	image_LUT_enemy_homing_bullet_shadow,
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
	image_LUT_enemy_ship_5_light,
	image_LUT_enemy_ship_5_shadow,
	image_LUT_enemy_ship_6_light,
	image_LUT_enemy_ship_6_shadow,
	image_LUT_box_light_1,
	image_LUT_box_light_2,
	image_LUT_box_light_3,
	image_LUT_box_light_4,
	image_LUT_box_light_5,
	image_LUT_box_light_6,
	image_LUT_box_shadow_1,
	image_LUT_box_shadow_2,
	image_LUT_box_shadow_3,
	image_LUT_box_shadow_4,
	image_LUT_box_shadow_5,
	image_LUT_box_shadow_6,
	image_LUT_box_solid_1,
	image_LUT_box_solid_2,
	image_LUT_box_solid_3,
	image_LUT_box_solid_4,
	image_LUT_box_solid_5,
	image_LUT_box_solid_6,
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
	image_LUT_powerslot,
	image_LUT_lives,
	image_LUT_background0,
	image_LUT_titleScreen,
	image_LUT_bossWarning,
	image_LUT_boss1_enemy_ship_light,
	image_LUT_boss1_enemy_ship_shadow,
	image_LUT_boss1_grenade_light,
	image_LUT_boss1_grenade_shadow,
	NB_IMAGES
};

// Boss images LUT
enum
{
	bossImage_LUT_1_body,
	bossImage_LUT_1_leftarm_armed,
	bossImage_LUT_1_rightarm_armed1,
	bossImage_LUT_1_rightarm_armed2,
	bossImage_LUT_1_leftarm_nonarmed,
	bossImage_LUT_1_rightarm_nonarmed,
	NB_BOSS_IMAGES
};

// Enemies' patterns
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
	Pattern_1_19,
	Pattern_1_20,
	Pattern_1_21,
	Pattern_1_boss,
	Pattern_1_bossGrenade,
	Pattern_2_1,
	Pattern_2_2,
	Pattern_2_3,
	Pattern_2_4,
	Pattern_2_5,
	Pattern_2_6,
	Pattern_2_7,
	NB_CALLBACKS
};

// Camera travelling handlers
enum
{
	CameraPath_i1,
	CameraPath_c1,
	CameraPath_i2
};

// Camera relations
enum
{
	CAMREL_NONE,
	CAMREL_ABSOLUTE,
	CAMREL_RELATIVE
};

extern unsigned short *image_entries[NB_IMAGES];
extern unsigned short *bossImage_entries[NB_BOSS_IMAGES];

extern void buildGameLUTs();
extern void freeGameLUTs();

extern DrawingCandidates *DC;
// Global vars
extern int G_gpTimer;
extern int G_skipFrame, G_waveTimer, G_killedThisFrame[MAX_ENEMY], G_frameChainOffset, G_chainStatus, G_inChainCount, G_maxChain;
extern int G_score, G_power;
extern bool G_fightingBoss;
extern bool G_usingTouchpad;
extern bool G_fireback, G_hardMode;
extern bool G_hasFiredOnce;
// Custom keys
extern t_key G_fireKey, G_polarityKey, G_fragmentKey, G_pauseKey;

extern touchpad_info_t *G_tpinfo;
extern touchpad_report_t G_tpstatus;
extern EnemiesArray G_enemiesArray;
extern BossEnemy *G_bossEnemy;
extern Particles *G_particles;

// Utils
extern Fixed angleToEntity(Entity*, Entity*);
extern Enemy* findNearestEnemy(Fixed x, Fixed y);
extern int distance(int x1, int y1, int x2, int y2);
extern int iToScreenX(int x, int camRel);
extern int iToScreenY(int y, int camRel);
extern Fixed fToScreenX(Fixed x, int camRel);
extern Fixed fToScreenY(Fixed y, int camRel);
extern BossData createBossData(int bossID);

#endif
