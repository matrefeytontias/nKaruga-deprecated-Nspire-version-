#include "common.h"
#include "ExplosionEffect.h"
#include "levels.h" 
#include "../gfx/kanji.h"
#include "../gfx/bossWarning.h"
#include "misc_data.h"

#define ENEMY_W(i) G_enemiesArray[i]->img[0]
#define ENEMY_H(i) G_enemiesArray[i]->img[1]

int G_skipFrame = 0, G_waveTimer = 0, G_killedThisFrame[MAX_ENEMY], G_frameChainOffset, G_chainStatus, G_inChainCount, G_maxChain = 0; int G_score, G_power, G_bossBonus = 0;
bool G_displayBg = true, G_fireback = true, G_hardMode = false;
bool G_hasFiredOnce;
int G_difficulty = 1;
bool G_usingTouchpad;
touchpad_info_t *G_tpinfo;
touchpad_report_t G_tpstatus;
t_key G_fireKey, G_polarityKey, G_fragmentKey, G_pauseKey;

DrawingCandidates *DC;

Enemy *G_enemiesArray[MAX_ENEMY];
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
	
	for(int i = 0; i < MAX_ENEMY; i++)
		G_enemiesArray[i] = new Enemy;
	
	G_particles = new Particles;
	
	DC = new DrawingCandidates;
	DC->init();
	
	buildGameLUTs();
	
	// Init things
	initBuffering();
	clearBufferW();
	initExplosionEngine();

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
	
	for(int i = 0; i < MAX_ENEMY; i++)
		delete G_enemiesArray[i];
	
	delete G_particles;
	
	DC->release();
	
	delete DC;
	
	deinitExplosionEngine();
	deinitBuffering();
	
	return 0;
}

int getBossPhase(int timer)
{
	if(timer < 1023)
		return PHASE_BOSSCINEMATIC;
	else
	{
		if(isKeyPressed(KEY_NSPIRE_ENTER)) return PHASE_BOSSEXPLODE;
		else return PHASE_BOSSFIGHT;
	}
}

void playGame()
{
	KeyEvent kEv = 0;
	int levelCounter, levelTimer, enemyCounter, waveIndex, scrollOffset = 0, pxScrollStart, pxScrollEnd, pauseTimer;
	bool levelEnded = false, parsedEnemy = true;
	int readKeys = 0, gpTimer = 0;
	int x, y;
	
	Rect statsRect, levelRect;
	int chainColor[3] = { 0 };
	
	unsigned int* bg;
	
	// Game phase
	int gamePhase;//, bossPhase;
	bool fightingBoss = false;
	
	// Variables for transition animation
	int currentW = 0, currentH = 0, chapterNum = 0, dX = 0, dY = 0;
	bool drawPowerSlot = true;
	static const char *levelStrs[5] = { "Chapter 1\nIdeal", "Chapter 2\nTrial", "Chapter 3\nFaith", "Chapter 4\nReality", "Chapter 5\nMetempsychosis" };
	static unsigned short *levelKanjis[1] = { image_kanji_1 };
	
	bg = (unsigned int*)image_entries[image_LUT_background];
	
	BulletArray* bArray = new BulletArray;
	Player ship;
	
	ChainNotif chainNotifsArray[MAX_ENEMY];
	DestroyedEnemies deadEnemies;
	ExplosionAnim explosionsAnims[MAX_ENEMY];
	int currentNotif, currentExplosion;
	
	ship.x = itofix(160);
	ship.y = itofix(180);
	
	levelCounter = 0;
	levelTimer = 0;
	pauseTimer = 0;
	enemyCounter = 0;
	waveIndex = 0;
	G_hasFiredOnce = false;
	
	gamePhase = PHASE_GAME;
	
	for(int i = 0; i < MAX_ENEMY; i++)
	{
		G_enemiesArray[i]->deactivate();
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
	currentExplosion = 0;
	gpTimer = 0;
	
	while(!KQUIT(kEv) && !levelEnded)
	{
		gpTimer++;
		if(gamePhase == PHASE_GAME)
		{
			G_waveTimer++;
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
							enemyCounter = 0;
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
								if(G_enemiesArray[i]->isActive())
								{
									levelCanProgress = 0;
									break;
								}
							}
							if(levelCanProgress) levelCounter++;
							else levelCounter--;
						}
						else if(currentLevelByte == LVLSTR_CHAPTER)
						{
							// Start a new chapter
							// Set up transition
							gamePhase = PHASE_TRANSITION;
							currentW = 0;
							gpTimer = 0;
							levelCounter++;
							chapterNum = levelStream[levelCounter];
							levelCounter++;
						}
						else if(currentLevelByte == LVLSTR_JOINT)
						{
							// Constraint an enemy to another
							G_enemiesArray[levelStream[levelCounter + 1]]->joint(levelStream[levelCounter + 2],
											itofix(levelStream[levelCounter + 3]),
											itofix(levelStream[levelCounter + 4]), levelStream[levelCounter + 5]);
							levelCounter += 6;
						}
						else if(currentLevelByte == LVLSTR_BOSS)
						{
							// Cinematic
							fightingBoss = true;
							gamePhase = PHASE_BOSSCINEMATIC;
							gpTimer = 0;
							// TODO
							// fight boss
							levelCounter += 2;
						}
						else if(currentLevelByte == LVLSTR_BKPT)
						{
							// Debug stuff
							printf("Current enemy : %d\nCurrent wave timer : %d\nGlobal timer : %d\nCurrent wave index : %d\n",
							enemyCounter, G_waveTimer, gpTimer, waveIndex);
							for(int i = 0; i < enemyCounter; i++)
								printf("Enemy %d :\nX,Y : %d, %d\n", i, fixtoi(G_enemiesArray[i]->getx()), fixtoi(G_enemiesArray[i]->gety()));
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
						gpTimer = 0;
						levelCounter++;
					}
					else if(currentLevelByte == LVLSTR_END)
					{
						// End of the level
						levelEnded = true;
					}
					else
					{
						// Dunno what it is ? Then it's an enemy by default
						G_enemiesArray[enemyCounter]->activate(itofix(levelStream[levelCounter]), itofix(levelStream[levelCounter + 1]), levelStream[levelCounter + 2],
															levelStream[levelCounter + 3], levelStream[levelCounter + 4], waveIndex, levelStream[levelCounter + 5],
															levelStream[levelCounter + 6], levelStream[levelCounter + 7]);
						levelCounter += 8;
						enemyCounter = (enemyCounter + 1) % MAX_ENEMY;
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
			if(!G_skipFrame) fillRect(0, 0, currentW, 240, 0);
			if(currentW == 0)
			{
				#define TRANSLATE 240
				dX = (itofix(220) - ship.x) / TRANSLATE;
				dY = (itofix(180) - ship.y) / TRANSLATE;
				currentW++;
			}
			else if(currentW < 120)
			{
				if(!(gpTimer % 2))
					currentW++;
			}
			else
				if(!G_skipFrame)
				{
					levelRect.x = 10;
					levelRect.y = 60;
					drawString(&levelRect.x, &levelRect.y, levelRect.x, levelStrs[chapterNum], 0xffff, 0);
					drawSprite(levelKanjis[chapterNum], 10, 80);
				}
			if(gpTimer > 768)
				gamePhase = PHASE_GAME;
		}
		
		if(fightingBoss)
			gamePhase = getBossPhase(gpTimer);
		
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
			if(gpTimer < TRANSLATE)
			{
				ship.x += dX;
				ship.y += dY;
			}
			sleep(5);
		}
		
		ship.handle(kEv, bArray);
		
		for(int i = 0; i < MAX_ENEMY; i++)
		{
			G_killedThisFrame[i] = -1;
			if(G_enemiesArray[i]->diedThisFrame)
			{
				G_killedThisFrame[i] = G_enemiesArray[i]->getPolarity();
				deadEnemies.activate(G_enemiesArray[i], i);
				explosionsAnims[currentExplosion].activate(fixtoi(G_enemiesArray[i]->getx()),
														   fixtoi(G_enemiesArray[i]->gety()),
														   G_enemiesArray[i]->getPolarity());
				currentExplosion = (currentExplosion + 1) % MAX_ENEMY;
			}
			G_enemiesArray[i]->handle(&ship, bArray);
		}
		
		//
		// Test
		//
		if(gamePhase == PHASE_BOSSFIGHT)
			drawSprite(bossImage_entries[bossImage_LUT_1_body], 0, 0);
		
		bArray->handle(&ship);
		
		G_particles->handle();
		
		if(!ship.getLives())
			levelEnded = 1;
		
		// Draw everything that has to be drawn
		DC->flush();
		
		// Things drawn within this block MUST NOT be candidates
		if(!G_skipFrame)
		{
			// Draw score and chains
			statsRect.x = statsRect.y = 0;
			if(gamePhase != PHASE_BOSSCINEMATIC)
			{
				drawStringF(&statsRect.x, &statsRect.y, 0, 0xffff, 0, "Score : %d\n\n\n\nCH %d", G_score, G_chainStatus);
				// Draw chain count
				for(int i = 0, j = 0; i < G_inChainCount; i++, j += 18)
					drawSprite(image_entries[chainColor[i] == LIGHT ? image_LUT_chain_hit_light : image_LUT_chain_hit_shadow], j, 12);
			}
			else
				drawStringF(&statsRect.x, &statsRect.y, 0, 0xffff, 0, "Score : %d", G_score);
			
			// Draw explosions
			for(int i = 0; i < MAX_ENEMY; i++)
				explosionsAnims[i].handle();
			
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
					if(gpTimer > 256)
					{
						drawString(&statsRect.x, &statsRect.y, (320 - numberWidth(G_bossBonus)) / 2, string_results[1], 0xffff, 0);
						drawDecimal(&statsRect.x, &statsRect.y, G_bossBonus, 0xffff, 0);
					}
					if(gpTimer > 512)
					{
						statsRect.x = (320 - stringWidth(string_results[2])) / 2;
						statsRect.y += 16;
						drawString(&statsRect.x, &statsRect.y, (320 - numberWidth(G_score)) / 2, string_results[2], 0xffff, 0);
						drawDecimal(&statsRect.x, &statsRect.y, G_score, 0xffff, 0);
						statsRect.x = (320 - stringWidth(string_results[3]) - stringWidth(string_results[4]) - numberWidth(G_maxChain)) / 2;
						statsRect.y += 16;
						drawString(&statsRect.x, &statsRect.y, 0, string_results[3], 0xffff, 0);
						drawDecimal(&statsRect.x, &statsRect.y, G_maxChain, 0xffff, 0);
						drawString(&statsRect.x, &statsRect.y, (320 - stringWidth(string_results[5])) / 2, string_results[4], 0xffff, 0);
						// don't display grade for now as it doesn't exist yet
					}
					if(gpTimer > 1024 && KFIRE(kEv))
					{
						gamePhase = PHASE_GAME;
					}
				}
				else
				{
					if(currentH < 120) currentH++;
					else if(currentW < 160) currentW += 2;
						
					fillRect(160 - currentW, 120 - currentH, currentW * 2 + 1, currentH * 2, 0);
					gpTimer = 0;
				}
			}
			else if(gamePhase == PHASE_BOSSCINEMATIC)
				drawSprite(image_bossWarning, 0, 72);
			else if(gamePhase == PHASE_BOSSEXPLODE)
			{
				if(gpTimer == 1024)
					initExplosionEffect(160, 120, 256, 0);
				else
					if(renderExplosionEffect())
					{
						fightingBoss = false;
						gamePhase = PHASE_GAME;
					}
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
		if(!has_colors) sleep(6);
		#endif
		
		scrollOffset = (scrollOffset + 1) % 240;
		G_skipFrame = (G_skipFrame + 1) % 4;
		
		// handle chaining
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
						if(deadEnemies.relevant[j])
						{
							if(j == i)
							{
								chainNotifsArray[currentNotif].activate(deadEnemies.x[j], deadEnemies.y[j], 100 * (1 << min(G_chainStatus, 8)));
								currentNotif = (currentNotif + 1) % MAX_ENEMY;
							}
							deadEnemies.relevant[j] = false;
						}
					}
					G_chainStatus++;
				}
			}
		}
		G_maxChain = max(G_chainStatus, G_maxChain);
		G_frameChainOffset = 0;
		
		#ifdef DEBUG_NKARUGA
		if(isKeyPressed(KEY_NSPIRE_SPACE)) bkpt();
		sleep(6);
		#endif
	}
	
	delete bArray;
}
