#include "common.h"
#include "levels.h"
#include "../gfx/kanji.h"
#include "misc_data.h"

#define ENEMY_W(i) G_enemiesArray[i]->img[0]
#define ENEMY_H(i) G_enemiesArray[i]->img[1]

int G_skipFrame = 0, G_waveTimer = 0, G_killedThisFrame[MAX_ENEMY], G_frameChainOffset, G_chainStatus;
int G_score, G_power;
bool G_displayBg = true, G_fireback = true, G_hardMode = false;
int G_difficulty = 1;
bool G_usingTouchpad;
touchpad_info_t *G_tpinfo;
touchpad_report_t G_tpstatus;
t_key G_fireKey, G_polarityKey, G_fragmentKey;

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
	fputc(G_difficulty, out);
	fputc(G_usingTouchpad, out);
	fputc(G_displayBg, out);
}

inline void readFromConfig(FILE* in)
{
	readKeyFromConfig(in, &G_fireKey);
	readKeyFromConfig(in, &G_polarityKey);
	readKeyFromConfig(in, &G_fragmentKey);
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
	t_key* customKeys[KEYS_TO_BIND] = { &G_fireKey, &G_polarityKey, &G_fragmentKey };
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
	}
	
	if(is_touchpad)
		G_tpinfo = touchpad_getinfo();
	
	for(int i = 0; i < MAX_ENEMY; i++)
		G_enemiesArray[i] = new Enemy;
	
	G_particles = new Particles;
	
	buildGameLUTs();
	
	// Init display
	initBuffering();
	clearBufferW();

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
	
	deinitBuffering();
	
	return 0;
}

void playGame()
{
	KeyEvent kEv = 0;
	int levelCounter, levelTimer, enemyCounter, waveIndex, scrollOffset = 0, pxScrollStart, pxScrollEnd;
	bool levelEnded = false, inTransitionFromIntro = false;
	int readKeys = 0, gpTimer = 0;
	
	Rect statsRect, levelRect;
	int chainColor[3] = { 0 }, inChainCount;
	
	unsigned short *bg;
	// Variables for transition animation
	int currentW = 0, chapterNum = 0, dX = 0, dY = 0;
	bool drawPowerSlot = true;
	static const char *levelStrs[5] = { "Chapter 1\nIdeal", "Chapter 2\nTrial", "Chapter 3\nFaith", "Chapter 4\nReality", "Chapter 5\nMetempsychosis" };
	static unsigned short *levelKanjis[1] = { image_kanji_1 };
	
	bg = image_entries[image_LUT_background];
	
	BulletArray* bArray = new BulletArray;
	Player ship;
	
	ChainNotif chainNotifsArray[MAX_ENEMY];
	DestroyedEnemies deadEnemies;
	ExplosionAnim explosionsAnims[MAX_ENEMY];
	int currentNotif, currentExplosion;
	
	ship.x = itofix(160);
	ship.y = itofix(220);
	
	levelCounter = 0;
	levelTimer = 0;
	enemyCounter = 0;
	waveIndex = 0;
	
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
	
	inChainCount = 0;
	currentNotif = 0;
	currentExplosion = 0;
	gpTimer = 0;
	
	while(!KQUIT(kEv) && !levelEnded)
	{
		gpTimer++;
		if(!inTransitionFromIntro)
		{
			G_waveTimer++;
			if(!levelTimer)
			{
				// Load the current enemy from the level stream
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
						inTransitionFromIntro = true;
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
										levelStream[levelCounter + 3],
										levelStream[levelCounter + 4]);
						levelCounter += 5;
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
				}
			}
			else
				levelTimer--;
		}
		else
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
				inTransitionFromIntro = false;
		}		
		
		if(!(readKeys % 4))
		{
			if(G_usingTouchpad)
				touchpad_scan(&G_tpstatus);
			kEv = getk();
		}
		readKeys++;
		
		if(inTransitionFromIntro)
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
		
		bArray->handle(&ship);
		
		G_particles->handle();
		
		if(!ship.getLives())
			levelEnded = 1;
		
		if(!G_skipFrame)
		{
			// Draw score and chains
			statsRect.x = statsRect.y = 0;
			drawStringF(&statsRect.x, &statsRect.y, 0, 0xffff, 0, "Score : %d\n\n\n\nCH %d", G_score, G_chainStatus);
			
			// Draw explosions
			for(int i = 0; i < MAX_ENEMY; i++)
				explosionsAnims[i].handle();
			
			// Draw chain count
			for(int i = 0, j = 0; i < inChainCount; i++, j += 18)
				drawSprite(image_entries[chainColor[i] == LIGHT ? image_LUT_chain_hit_light : image_LUT_chain_hit_shadow], j, 12);
			
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
			
			updateScreen();
		
			if(G_displayBg)
			{
				// Display a scrolling background
				pxScrollStart = scrollOffset * 160;
				pxScrollEnd = 160*240 - pxScrollStart;
				// cheat using cast to copy faster since the ARM9 registers are 32-bits
				for(int i = pxScrollStart, j = 0; i < 160 * 240; i++, j++)
					((unsigned int*)BUFF_BASE_ADDRESS)[i] = ((unsigned int*)bg)[j];
				for(int i = 0, j = pxScrollEnd; j < 160 * 240; i++, j++)
					((unsigned int*)BUFF_BASE_ADDRESS)[i] = ((unsigned int*)bg)[j];
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
				if(inChainCount == 3) inChainCount = 0;
				
				if(inChainCount)
				{
					if(chainColor[inChainCount - 1] != G_killedThisFrame[i])
					{
						inChainCount = 0;
						G_chainStatus = 0;
					}
				}
				
				chainColor[inChainCount] = G_killedThisFrame[i];
				inChainCount++;
				
				if(inChainCount == 3)
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
		G_frameChainOffset = 0;
		
		if(K4(kEv)) G_usingTouchpad = is_touchpad ? true : false;
		if(K5(kEv)) G_usingTouchpad = false;
		if(K7(kEv)) G_displayBg = true;
		if(K8(kEv)) G_displayBg = false;
		
		#ifdef DEBUG_NKARUGA
		sleep(6);
		#endif
	}
	
	delete bArray;
}
