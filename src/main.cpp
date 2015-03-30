#include "common.h"
#include "ExplosionEffect.h"
#include "levels.h" 
#include "../gfx/kanji.h"
#include "misc_data.h"

#define ENEMY_W(i) G_enemiesArray.data[i].img[0]
#define ENEMY_H(i) G_enemiesArray.data[i].img[1]

int G_gpTimer;
int G_skipFrame = 0, G_waveTimer = 0, G_killedThisFrame[MAX_ENEMY] = { 0 }, G_frameChainOffset = 0, G_chainStatus = 0, G_inChainCount = 0, G_maxChain = 0;
int G_score = 0, G_power = 0;
bool G_displayBg = true, G_fireback = true, G_hardMode = false;
bool G_hasFiredOnce = false;
bool G_fightingBoss = false;
int G_difficulty = 1;
bool G_usingTouchpad = false;
touchpad_info_t *G_tpinfo;
touchpad_report_t G_tpstatus;
t_key G_fireKey, G_polarityKey, G_fragmentKey, G_pauseKey;

DrawingCandidates *DC;

EnemiesArray G_enemiesArray;
BossEnemy *G_bossEnemy;
Particles *G_particles;

void playGame();

static unsigned short image_cursor[] = { 5, 8, 1,
0, 0, 1, 1, 1,
1, 0, 0, 1, 1,
1, 1, 0, 0, 1,
1, 1, 1, 0, 0,
1, 1, 1, 0, 0,
1, 1, 0, 0, 1,
1, 0, 0, 1, 1,
0, 0, 1, 1, 1
};

inline void writeKeyToConfig(FILE* out, t_key* key)
{
	fputc(key->row, out);
	fputc(key->col & 0xff, out);
	fputc(key->col >> 8, out);
	fputc(key->tpad_row, out);
	fputc(key->tpad_col & 0xff, out);
	fputc(key->tpad_col >> 8, out);
}

inline void readKeyFromConfig(FILE* in, t_key* key)
{
	key->row = fgetc(in);
	key->col = fgetc(in) | (fgetc(in) << 8);
	key->tpad_row = fgetc(in);
	key->tpad_col = fgetc(in) | (fgetc(in) << 8);
	key->tpad_arrow = TPAD_ARROW_NONE;
}

inline void writeToConfig(FILE* out)
{
	writeKeyToConfig(out, &G_fireKey);
	writeKeyToConfig(out, &G_polarityKey);
	writeKeyToConfig(out, &G_fragmentKey);
	writeKeyToConfig(out, &G_pauseKey);
	fputc(G_difficulty, out);
	fputc(G_usingTouchpad, out);
	fputc(G_displayBg, out);
}

inline void readFromConfig(FILE* in)
{
	readKeyFromConfig(in, &G_fireKey);
	readKeyFromConfig(in, &G_polarityKey);
	readKeyFromConfig(in, &G_fragmentKey);
	readKeyFromConfig(in, &G_pauseKey);
	G_difficulty = fgetc(in);
	G_usingTouchpad = !!fgetc(in);
	G_displayBg = !!fgetc(in);
}

int main(int argc, char **argv)
{
	UNUSED(argc);
	int x, y, blink = 0;
	bool donePlaying = false, openedMenu = false;
	G_usingTouchpad = false;
	FILE* configFile;
	// Inline menu vars
	void* optionValues[TITLE_OPTIONS] = { NULL, &G_difficulty, &G_usingTouchpad, &G_displayBg, NULL };
	// Custom keys vars
	t_key* customKeys[KEYS_TO_BIND] = { &G_fireKey, &G_polarityKey, &G_fragmentKey, &G_pauseKey };
	int choice = 0;
	
	enable_relative_paths(argv);
	
	configFile = fopen(string_nKaruga_config, "rb");
	if(configFile)
	{
		readFromConfig(configFile);
		fclose(configFile);
	}
	else
	{
		G_fireKey = KEY_NSPIRE_CTRL;
		G_polarityKey = KEY_NSPIRE_SHIFT;
		G_fragmentKey = KEY_NSPIRE_DEL;
		G_pauseKey = KEY_NSPIRE_P;
	}
	
	if(is_touchpad)
		G_tpinfo = touchpad_getinfo();
	
	G_particles = new Particles;
	
	DC = new DrawingCandidates;
	
	G_bossEnemy = new BossEnemy;
	
	buildGameLUTs();
	
	// Init things
	initBuffering();
	clearBufferW();
	initExplosionEngine();
	timer_init(1);
	timer_load(1, 0);

	while(!donePlaying)
	{
		drawSprite(image_entries[image_LUT_titleScreen], 0, 0);
		if(!openedMenu)
		{
			x = (320 - strlen(string_title) * 8) / 2;
			y = 160;
			if(blink % 96 < 48)
				drawString(&x, &y, x, string_title, 0, 0xffff);
			blink++;
			if(isKeyPressed(KEY_NSPIRE_ENTER))
			{
				wait_no_key_pressed();
				openedMenu = true;
			}
		}
		else if(openedMenu)
		{
			fillRect(35, 118, 250, TITLE_OPTIONS * 8 + 4, 0xffff);
			drawHLine(118, 35, 285, 0);
			drawHLine(TITLE_OPTIONS * 8 + 118 + 4, 35, 285, 0);
			drawVLine(35, 118, TITLE_OPTIONS * 8 + 118 + 4, 0);
			drawVLine(285, 118, TITLE_OPTIONS * 8 + 118 + 4, 0);
			y = 120;
			for(int i = 0; i < TITLE_OPTIONS; i++)
			{
				char parameter = string_options[i][0];
				bool isParameter = parameter != 'n';
				x = (320 - (strlen(string_options[i] + 1) + isParameter * 3) * 8) / 2;
				drawString(&x, &y, 0, string_options[i] + 1, 0, 0xffff);
				if(parameter == 'b')
					drawString(&x, &y, 0, *(bool*)optionValues[i] ? "yes\n" : "no\n", 0, 0xffff);
				else if(parameter == 'i')
					drawString(&x, &y, 0, string_difficulties[*(int*)optionValues[i]], 0, 0xffff);
			}
			drawSprite(image_cursor, (320 - (strlen(string_options[choice] + 1) + (string_options[choice][0] != 'n') * 3) * 8) / 2 - 8, choice * 8 + 120);
			
			sleep(100);
			
			if(isKeyPressed(KEY_NSPIRE_ENTER))
			{
				if(choice > 0 && choice < TITLE_OPTIONS - 1)
				{
					if(string_options[choice][0] == 'b')
						*(bool*)optionValues[choice] = !*(bool*)optionValues[choice];
					else
					{
						*(int*)optionValues[choice] = (*(int*)optionValues[choice] + 1) % 3;
					}
					wait_no_key_pressed();
				}
				else if(choice)
				{
					wait_no_key_pressed();
					clearBufferB();		
					x = 0;
					y = 0;
					drawString(&x, &y, 0, "Press the key you want to bind to this\naction.\n\n", 0xffff, 0);
					
					for(int i = 0; i < KEYS_TO_BIND; i++)
					{
						drawString(&x, &y, 0, string_keys[i], 0xffff, 0);
						updateScreen();
						while(!get_key_pressed(customKeys[i]));
						wait_no_key_pressed();
					}
				}
				else
				{
					configFile = fopen(string_nKaruga_config, "wb");
					if(configFile)
					{
						writeToConfig(configFile);
						fclose(configFile);
					}
					G_fireback = G_difficulty > 0;
					G_hardMode = G_difficulty == 2;
					playGame();
					openedMenu = false;
				}
			}
			choice += isKeyPressed(KEY_NSPIRE_DOWN) - isKeyPressed(KEY_NSPIRE_UP);
			choice = choice < 0 ? 0 : (choice >= TITLE_OPTIONS ? TITLE_OPTIONS - 1 : choice);
		}
		updateScreen();
		
		if(isKeyPressed(KEY_NSPIRE_DEL))
			donePlaying = true;
	}
	
	delete G_particles;
	
	delete DC;
	
	delete G_bossEnemy;
	
	deinitExplosionEngine();
	deinitBuffering();
	
	timer_restore(1);
	
	return 0;
}

int getBossPhase(int timer)
{
	if(timer < 1024)
		return PHASE_BOSSCINEMATIC;
	else
	{
		return PHASE_BOSSFIGHT;
	}
}

void playGame()
{
	KeyEvent kEv = 0;
	int levelCounter = 0, levelTimer = 0, waveIndex = 0, scrollOffset = 0, pxScrollStart = 0, pxScrollEnd = 0, pauseTimer = 0;
	bool gameEnded = false, parsedEnemy = true;
	int readKeys = 0;
	int x = 0, y = 0;
	
	Rect statsRect, levelRect;
	int chainColor[3] = { 0 };
	
	// Load first background right away
	unsigned int* bg = (unsigned int*)image_entries[image_LUT_background0];
	
	// Game phase
	int gamePhase = PHASE_GAME;
	G_fightingBoss = false;
	BossData bossData;
	int bossBonus = 0;
	
	// Variables for transition animation
	int currentW = 0, currentH = 0, chapterNum = 0, dX = 0, dY = 0;
	bool drawPowerSlot = true;
	static const char *levelStrs[5] = { "Chapter 1\nIdeal", "Chapter 2\nTrial", "Chapter 3\nFaith", "Chapter 4\nReality", "Chapter 5\nMetempsychosis" };
	static unsigned short *levelKanjis[1] = { image_kanji_1 };
	
	BulletArray* bArray = new BulletArray;
	Player ship;
	
	ChainNotif chainNotifsArray[MAX_ENEMY];
	int currentNotif;
	
	ship.setx(itofix(160));
	ship.sety(itofix(180));
	
	levelCounter = 0;
	levelTimer = 0;
	pauseTimer = 0;
	waveIndex = 0;
	G_hasFiredOnce = false;
	
	gamePhase = PHASE_GAME;
	
	for(int i = 0; i < MAX_ENEMY; i++)
	{
		G_enemiesArray.data[i].deactivate();
		G_killedThisFrame[i] = 0;
	}
	
	G_score = 0;
	G_power = 0;
	for(int i = 0; i < 3; i++)
		chainColor[i] = 0;
	G_chainStatus = 0;
	G_frameChainOffset = 0;
	G_waveTimer = 0;
	
	G_inChainCount = 0;
	currentNotif = 0;
	G_gpTimer = 0;
	
	while(!KQUIT(kEv) && !gameEnded)
	{
		G_gpTimer++;
		G_waveTimer++;
		if(gamePhase == PHASE_GAME)
		{
			if(!levelTimer)
			{
				do
				{
					parsedEnemy = false;
					// Load the current byte from the level stream
					int currentLevelByte = levelStream[levelCounter];
					if(currentLevelByte == LVLSTR_CMD)
					{
						levelCounter++;
						currentLevelByte = levelStream[levelCounter];
						if(currentLevelByte == LVLSTR_NEWWAVE)
						{
							// Start a new wave
							G_waveTimer = 0;
							waveIndex = 0;
							levelCounter++;
							G_enemiesArray.resetEnemyCounter();
							// Reset relative camera
							DC->cam.relX = DC->cam.relY = 0;
						}
						else if(currentLevelByte == LVLSTR_NEWCAMERA)
						{
							// Follow a new camera path
							DC->loadCameraPath(levelStream[++levelCounter]);
							levelCounter++;
						}
						else if(currentLevelByte == LVLSTR_WAIT)
						{
							// Wait some frames
							levelTimer = levelStream[levelCounter + 1];
							levelCounter += 2;
						}
						else if(currentLevelByte == LVLSTR_KILLED)
						{
							// Wait for every enemy to be killed before progressing
							int levelCanProgress = 1;
							for(int i = 0; i < MAX_ENEMY; i++)
							{
								if(G_enemiesArray.data[i].isActive())
								{
									levelCanProgress = 0;
									break;
								}
							}
							if(levelCanProgress) levelCounter++;
							else levelCounter--;
						}
						else if(currentLevelByte == LVLSTR_REINIT)
						{
							// Prepare for a new chapter
							int nextChapter = levelStream[++levelCounter];
							UNUSED(nextChapter);
							G_enemiesArray.destroyAllEnemies(&ship, bArray);
							bArray->clear();
							// Reset everything chains-related
							G_chainStatus = 0;
							G_frameChainOffset = 0;
							G_inChainCount = 0;
							// bg = (unsigned int*)image_entries[image_LUT_background0 + nextChapter];
							G_power = 0;
							clearBufferB();
							updateScreen();
							sleep(1000);
							levelCounter++;
							// Reset all camera position
							DC->cam.absX = DC->cam.absY = DC->cam.relX = DC->cam.relY = 0;
							// And here we go for a new chapter
						}
						else if(currentLevelByte == LVLSTR_CHAPTER)
						{
							// Start a new chapter
							// Set up transition
							gamePhase = PHASE_TRANSITION;
							currentW = 0;
							G_gpTimer = 0;
							levelCounter++;
							chapterNum = levelStream[levelCounter];
							levelCounter++;
						}
						else if(currentLevelByte == LVLSTR_JOINT)
						{
							// Constrain an enemy to another
							G_enemiesArray.data[levelStream[levelCounter + 1]].joint(levelStream[levelCounter + 2],
											itofix(levelStream[levelCounter + 3]),
											itofix(levelStream[levelCounter + 4]), levelStream[levelCounter + 5]);
							levelCounter += 6;
						}
						else if(currentLevelByte == LVLSTR_BOSS)
						{
							// Cinematic
							G_fightingBoss = true;
							gamePhase = PHASE_BOSSCINEMATIC;
							G_gpTimer = 0;
							// fight boss
							levelCounter++;
							bossData = createBossData(levelStream[levelCounter]);
							G_bossEnemy->activate(&bossData);
							levelCounter++;
						}
						else if(currentLevelByte == LVLSTR_BKPT)
						{
							// Debug stuff
							printf("Current wave timer : %d\nGlobal timer : %d\nCurrent wave index : %d\n",
							G_waveTimer, G_gpTimer, waveIndex);
							for(int i = 0; i < MAX_ENEMY; i++)
							{
								if(G_enemiesArray.data[i].isActive())
									printf("Enemy %d :\nX,Y : %d, %d\n", i, fixtoi(G_enemiesArray.data[i].getx()), fixtoi(G_enemiesArray.data[i].gety()));
							}
							bkpt();
							levelCounter++;
						}
						else
						{
							// You messed up your level stream bro
							printf("Error : %d : unknown command !\n", currentLevelByte);
							bkpt();
						}
					}
					else if(currentLevelByte == LVLSTR_CHAPTEREND)
					{
						gamePhase = PHASE_RESULTS;
						currentW = 0;
						currentH = 0;
						G_gpTimer = 0;
						levelCounter++;
					}
					else if(currentLevelByte == LVLSTR_END)
					{
						// End of the game
						gameEnded = true; 
					}
					else
					{
						// Dunno what it is ? Then it's an enemy by default
						G_enemiesArray.add(itofix(levelStream[levelCounter]), itofix(levelStream[levelCounter + 1]), levelStream[levelCounter + 2],
											levelStream[levelCounter + 3], levelStream[levelCounter + 4], waveIndex, levelStream[levelCounter + 5],
											levelStream[levelCounter + 6], levelStream[levelCounter + 7], false, levelStream[levelCounter + 8]);
						levelCounter += 9;
						waveIndex++;
						parsedEnemy = true;
					}
				} while(parsedEnemy);
			}
			else
				levelTimer--;
		}
		else if(gamePhase == PHASE_TRANSITION)
		{
			if(!(G_skipFrame % 4)) fillRect(0, 0, currentW, 240, 0);
			if(currentW == 0)
			{
				#define TRANSLATE 240
				dX = (itofix(220) - ship.getx()) / TRANSLATE;
				dY = (itofix(180) - ship.gety()) / TRANSLATE;
				currentW++;
			}
			else if(currentW < 120)
			{
				if(!(G_gpTimer % 2))
					currentW++;
			}
			else
				if(!(G_skipFrame % 4))
				{
					levelRect.x = 10;
					levelRect.y = 60;
					drawString(&levelRect.x, &levelRect.y, levelRect.x, levelStrs[chapterNum], 0xffff, 0);
					drawSprite(levelKanjis[chapterNum], 10, 80);
				}
			if(G_gpTimer > 768)
			{
				// Reset all camera position
				DC->cam.absX = DC->cam.absY = DC->cam.relX = DC->cam.relY = 0;
				gamePhase = PHASE_GAME;
			}
		}
		else if(gamePhase == PHASE_BOSSFIGHT)
		{
			if(G_bossEnemy->handle(&ship, bArray))
			{
				gamePhase = PHASE_BOSSEXPLODEINIT;
				G_fightingBoss = false;
				G_enemiesArray.destroyAllEnemies(&ship, bArray);
			}
		}
		
		if(G_fightingBoss)
			gamePhase = getBossPhase(G_gpTimer);
		
		if(!(readKeys % 4))
		{
			if(G_usingTouchpad)
				touchpad_scan(&G_tpstatus);
			kEv = getk();
		}
		readKeys++;
			
		if(gamePhase == PHASE_TRANSITION)
		{
			kEv = 0;
			G_tpstatus.x = G_tpinfo->width / 2;
			G_tpstatus.y = G_tpinfo->height / 2;
			if(G_gpTimer < TRANSLATE)
			{
				ship.setx(ship.getx() + dX);
				ship.sety(ship.gety() + dY);
			}
			sleep(5);
		}
		
		ship.handle(kEv, bArray);
		
		G_enemiesArray.handle(&ship, bArray);
		
		bArray->handle(&ship, G_bossEnemy);
		
		G_particles->handle();
		
		// Draw everything that has to be drawn
		DC->flush();
		
		// Put that here and test G_skipFrame so that the game is fully drawn when the game over screen shows
		// Wait for the ship's explosion and the small delay to complete
		if(!ship.getLives() && ship.deathCounter > 12 && !(G_skipFrame % 4))
		{
			// #####################
			// TODO : save highscore
			// #####################
			
			bool hasPressed = false;
			// Display "continue" screen
			int x = (320 - stringWidth(string_continue)) / 2;
			int y = 120;
			drawString(&x, &y, x, string_continue, 0xffff, 0x0000);
			updateScreen();
			while(!hasPressed)
			{
				if(isKeyPressed(KEY_NSPIRE_ENTER))
				{
					hasPressed = true;
					// - initialise a new ship
					ship.reset();
					// - score and power set to 0
					G_score = 0; // ouch
					G_power = 0;
					// - chains to 0
					G_chainStatus = 0;
					G_frameChainOffset = 0;
					G_inChainCount = 0;
					G_maxChain = 0; // I know that this one hurts but it has to be done ;_;
					// - DO NOT RESET DOT EATER ACHIEVEMENT
					// - destroy all bullets and dead enemies data (no posthumous scoring)
					bArray->clear();
					G_enemiesArray.deadEnemies.clear();
				}
				else if(isKeyPressed(KEY_NSPIRE_DEL))
				{
					hasPressed = true;
					gameEnded = 1;
				}
			}
		}
		
		// Things drawn within this block MUST NOT be candidates
		if(!(G_skipFrame % 4))
		{
			// Draw score and chains
			statsRect.x = statsRect.y = 0;
			if(!G_fightingBoss)
			{
				drawStringF(&statsRect.x, &statsRect.y, 0, 0xffff, 0, "Score : %d\n\n\n\nCH %d", G_score, G_chainStatus);
				// Draw chain count
				for(int i = 0, j = 0; i < G_inChainCount; i++, j += 18)
					drawSprite(image_entries[chainColor[i] == LIGHT ? image_LUT_chain_hit_light : image_LUT_chain_hit_shadow], j, 12);
			}
			else
			{
				drawStringF(&statsRect.x, &statsRect.y, 0, 0xffff, 0, "Score : %d\nTime : %d", G_score, G_bossEnemy->getTimeout());
			}
			
			// Draw explosions
			G_enemiesArray.handleExplosions();
			
			// Draw power
			for(int i = MAX_FRAGMENT - 1; i >= 0; i--)
			{
				drawSprite(image_entries[image_LUT_powerslot], 5, i * 14 + 40);
				for(int j = 0; j < 10; j++)
				{
					if(G_power > (MAX_FRAGMENT - 1 - i) * 10 + j)
						drawHLine(i * 14 + 40 + 10 - j, 5 + power_fill_offsets[j * 2], 5 + power_fill_offsets[j * 2 + 1],
								drawPowerSlot || G_power < (MAX_FRAGMENT - i) * 10 ? (ship.getPolarity() ? 0xf800 : 0x3ff) : 0xffff );
					else
						break;
				}
			}
			drawPowerSlot = !drawPowerSlot;
			
			// Draw score-chaining notifs
			if(!G_fightingBoss)
				for(int i = 0; i < MAX_ENEMY; i++)
					chainNotifsArray[i].handle();
			
			// Draw remaining lives
			drawSprite(image_entries[image_LUT_lives], 0, 224);
			statsRect.x = image_entries[image_LUT_lives][0] + 2;
			statsRect.y = 226;
			drawChar(&statsRect.x, &statsRect.y, 0, 'x', 0xffff, 0);
			drawDecimal(&statsRect.x, &statsRect.y, ship.getLives() - 1, 0xffff, 0);
			
			// Overwrite all of that
			if(gamePhase == PHASE_RESULTS)
			{
				if(currentH == 120 && currentW == 160)
				{
					clearBufferB();
					statsRect.x = (320 - stringWidth(string_results[0])) / 2;
					statsRect.y = 16;
					drawString(&statsRect.x, &statsRect.y, (320 - stringWidth(string_results[1])) / 2, string_results[0], 0xffff, 0);
					if(G_gpTimer > 128)
					{
						// Boss bonus
						drawString(&statsRect.x, &statsRect.y, (320 - numberWidth(G_bossEnemy->getTimeout() * 10000)) / 2, string_results[1], 0xffff, 0);
						drawDecimal(&statsRect.x, &statsRect.y, G_bossEnemy->getTimeout() * 10000, 0xffff, 0);
					}
					if(G_gpTimer > 256)
					{
						G_score += bossBonus;
						bossBonus = 0;
						// Score
						statsRect.x = (320 - stringWidth(string_results[2])) / 2;
						statsRect.y += 16;
						drawString(&statsRect.x, &statsRect.y, (320 - numberWidth(G_score)) / 2, string_results[2], 0xffff, 0);
						drawDecimal(&statsRect.x, &statsRect.y, G_score, 0xffff, 0);
						statsRect.x = (320 - stringWidth(string_results[3]) - stringWidth(string_results[4]) - numberWidth(G_maxChain)) / 2;
						statsRect.y += 16;
						drawString(&statsRect.x, &statsRect.y, 0, string_results[3], 0xffff, 0);
						drawDecimal(&statsRect.x, &statsRect.y, G_maxChain, 0xffff, 0);
						drawString(&statsRect.x, &statsRect.y, (320 - stringWidth(string_results[5])) / 2, string_results[4], 0xffff, 0);
						// Grade
						drawString(&statsRect.x, &statsRect.y, (320 - stringWidth("Dot eater !")) / 2, string_results[5], 0xffff, 0);
						if(!G_hasFiredOnce)
							drawString(&statsRect.x, &statsRect.y, statsRect.x, "Dot eater !", 0xffff, 0);
					}
					if(G_gpTimer > 384 && KFIRE(kEv))
					{
						gamePhase = PHASE_GAME;
					}
				}
				else
				{
					if(currentH < 120) currentH++;
					else if(currentW < 160) currentW += 2;
						
					fillRect(160 - currentW, 120 - currentH, currentW * 2 + 1, currentH * 2, 0);
					G_gpTimer = 0;
				}
			}
			else if(gamePhase == PHASE_BOSSCINEMATIC)
				drawSprite(image_entries[image_LUT_bossWarning], 0, 72);
			else if(gamePhase == PHASE_BOSSEXPLODEINIT)
			{
				initExplosionEffect(160, 120, 500, 0);
				gamePhase = PHASE_BOSSEXPLODE;
				bossBonus = G_bossEnemy->getTimeout() * 10000;
			}
			else if(gamePhase == PHASE_BOSSEXPLODE)
			{
				if(updateExplosionEffect())
					gamePhase = PHASE_GAME;
			}
			
			if(!pauseTimer) 
			{
				if(KPAUSE(kEv))
				{
					// Pause the game until another pauseKey is pressed
					wait_no_key_pressed();

					// Display a "paused" box. It will be cleared in the next frame.
					x = 140, y = 116;
					fillRect(60, 100, 200, 40, 0xffff);
					drawHLine(100, 60, 260, 0);
					drawHLine(140, 60, 260, 0);
					drawString(&x, &y, 0, "Pause", 0, 0xffff);
					updateScreen();
					
					while(!isKeyPressed(G_pauseKey)) 
					{
						sleep(5);
						if(isKeyPressed(KEY_NSPIRE_ESC))
						{
							kEv = 128; // KQUIT
							break;
						}
					}
					wait_no_key_pressed();
					pauseTimer = 10;
				}
			}
			else
				pauseTimer--;
			
			renderExplosionEffect();
			
			updateScreen();	
			
			// The background is dispayed after to keep the enemies(power slots, player ship ...) 
			// on the screen when pausing the game
			if(G_displayBg)
			{
				// Display a scrolling background
				pxScrollStart = scrollOffset * 160;
				pxScrollEnd = 160*240 - pxScrollStart;
				// cheat using cast to copy faster since the ARM9 registers are 32-bits
				for(int i = pxScrollStart, j = 0; i < 160 * 240; i++, j++)
					((unsigned int*)BUFF_BASE_ADDRESS)[i] = bg[j];
				for(int i = 0, j = pxScrollEnd; j < 160 * 240; i++, j++)
					((unsigned int*)BUFF_BASE_ADDRESS)[i] = bg[j];
			}
			else
				clearBufferW();
		}

		#ifndef DEBUG_NKARUGA
		if(!has_colors) sleep(2);
		#endif
		
		scrollOffset = (scrollOffset + 1) % 240;
		G_skipFrame++;
		
		// handle chaining
		if(!G_fightingBoss)
		{
			for(int i = 0; i < MAX_ENEMY; i++)
			{
				if(G_killedThisFrame[i] != -1)
				{
					if(G_inChainCount == 3) G_inChainCount = 0;
					
					if(G_inChainCount)
					{
						if(chainColor[G_inChainCount - 1] != G_killedThisFrame[i])
						{
							G_inChainCount = 0;
							G_chainStatus = 0;
						}
					}
					
					chainColor[G_inChainCount] = G_killedThisFrame[i];
					G_inChainCount++;
					
					if(G_inChainCount == 3)
					{
						G_score += 100 * (1 << min(G_chainStatus, 8));
						for(int j = 0; j < MAX_ENEMY; j++)
						{
							if(G_enemiesArray.deadEnemies.relevant[j])
							{
								if(j == i)
								{
									chainNotifsArray[currentNotif].activate(G_enemiesArray.deadEnemies.x[j], G_enemiesArray.deadEnemies.y[j], 100 * (1 << min(G_chainStatus, 8)));
									currentNotif = (currentNotif + 1) % MAX_ENEMY;
								}
								G_enemiesArray.deadEnemies.relevant[j] = false;
							}
						}
						G_chainStatus++;
					}
				}
			}
			G_maxChain = max(G_chainStatus, G_maxChain);
			G_frameChainOffset = 0;
		}
		
		#ifdef DEBUG_NKARUGA
		sleep(6);
		#endif
	}
	
	delete bArray;
}
