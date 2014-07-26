#include "common.h"
#include "levels.h"
#include "../gfx/kanji.h"

int G_skipFrame = 0, G_waveTimer = 0, G_score, G_killedThisFrame[MAX_ENEMY], G_frameChainOffset, G_chainStatus;

void playGame();

int main(int argc, char **argv) {
	bool donePlaying = false;
	
	enable_relative_paths(argv);
	
	buildGameLUTs();
	
	// Init display
	initBuffering();
	clearBufferW();
	
	while(!donePlaying)
	{
		drawSprite(image_entries[image_LUT_titleScreen], 0, 0);
		updateScreen();		
		
		if(isKeyPressed(KEY_NSPIRE_ENTER))
			playGame();
		else if(isKeyPressed(KEY_NSPIRE_DEL))
			donePlaying = true;
	}
	
	deinitBuffering();
	
	return 0;
}

void playGame()
{
	KeyEvent kEv = 0;
	int levelCounter, levelTimer, enemyCounter, waveIndex, scrollOffset = 0, pxScrollStart, pxScrollEnd;
	bool levelEnded = false, displayBg = true, inTransitionFromIntro = false;
	int readKeys = 0, gpTimer = 0;
	
	Rect statsRect, levelRect;
	int chainColor[3] = { 0 }, inChainCount;
	
	unsigned short *bg;
	// Variables for transition animation
	int currentW = 0, chapterNum = 0, dX = 0, dY = 0;
	static const char *levelStrs[5] = { "Chapter 1\nIdeal", "Chapter 2\nTrial", "Chapter 3\nFaith", "Chapter 4\nReality", "Chapter 5\nMetempsychosis" };
	static unsigned short *levelKanjis[1] = { image_kanji_1 };
	
	// Skip header
	bg = image_entries[image_LUT_background] + 6;
	
	BulletArray* bArray = new BulletArray;
	Player ship;
	Enemy *enemiesArray[MAX_ENEMY];
	ChainNotif chainNotifsArray[MAX_ENEMY];
	DestroyedEnemies deadEnemies;
	int currentNotif;
	
	for(int i = 0; i < MAX_ENEMY; i++)
	{
		enemiesArray[i] = new Enemy;
		enemiesArray[i]->deactivate();
	}
	
	ship.x = itofix(160);
	ship.y = itofix(220);
	
	levelCounter = 0;
	levelTimer = 0;
	enemyCounter = 0;
	waveIndex = 0;
	
	G_score = 0;
	G_killedThisFrame[G_frameChainOffset] = -1;
	for(int i = 0; i < 3; i++)
		chainColor[i] = 0;
	G_chainStatus = 0;
	G_frameChainOffset = 0;
	inChainCount = 0;
	currentNotif = 0;
	
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
						G_waveTimer = 0;
						waveIndex = 0;
						levelCounter++;
					}
					else if(currentLevelByte == LVLSTR_WAIT)
					{
						levelTimer = levelStream[levelCounter + 1];
						levelCounter += 2;
					}
					else if(currentLevelByte == LVLSTR_KILLED)
					{
						int levelCanProgress = 1;
						for(int i = 0; i < MAX_ENEMY; i++)
						{
							if(enemiesArray[i]->isActive())
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
						inTransitionFromIntro = true;
						currentW = 0;
						gpTimer = 0;
						levelCounter++;
						chapterNum = levelStream[levelCounter];
						levelCounter++;
					}
					else if(currentLevelByte == LVLSTR_BKPT)
					{
						printf("Current enemy : %d\n \
						Current wave timer : %d\n \
						Global timer : %d\n \
						Current wave index : %d\n",
						enemyCounter, G_waveTimer, gpTimer, waveIndex);
						bkpt();
						levelCounter++;
					}
					else
					{
						printf("Error : %d : unknown command !\n", currentLevelByte);
						bkpt();
					}
				}
				else if(currentLevelByte == LVLSTR_END)
				{
					levelEnded = true;
				}
				else
				{
					enemiesArray[enemyCounter]->activate(itofix(levelStream[levelCounter]), itofix(levelStream[levelCounter + 1]), levelStream[levelCounter + 2], levelStream[levelCounter + 3],
														levelStream[levelCounter + 4], waveIndex, levelStream[levelCounter + 5], levelStream[levelCounter + 6]);
					levelCounter += 7;
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
					drawString(&levelRect.x, &levelRect.y, levelRect.x, levelStrs[chapterNum], 0xffff);
					drawSprite(levelKanjis[chapterNum], 10, 80);
				}
			if(gpTimer > 768)
				inTransitionFromIntro = false;
		}
		
		if(!(readKeys % 4))
			kEv = getk();
		readKeys++;
		
		if(inTransitionFromIntro)
		{
			kEv = 0;
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
			if(enemiesArray[i]->diedThisFrame)
			{
				G_killedThisFrame[i] = enemiesArray[i]->getPolarity();
				deadEnemies.activate(enemiesArray[i], i);
			}
			enemiesArray[i]->handle(&ship, bArray);
		}
		
		bArray->handle(&ship, enemiesArray);
		
		if(!ship.getLives())
			levelEnded = 1;
		
		if(!G_skipFrame)
		{
			// Draw score and chains
			statsRect.x = statsRect.y = 0;
			drawStringF(&statsRect.x, &statsRect.y, 0, 0xffff, "Score : %d\n\n\n\nCH %d", G_score, G_chainStatus);
			
			// Draw chain count
			for(int i = 0, j = 0; i < inChainCount; i++, j += 18)
				drawSprite(image_entries[chainColor[i] == LIGHT ? image_LUT_chain_hit_light : image_LUT_chain_hit_shadow], j, 12);			
			
			// Draw score-chaining notifs
			for(int i = 0; i < MAX_ENEMY; i++)
				chainNotifsArray[i].handle();
			
			updateScreen();
		
			if(displayBg)
			{
				// Display a scrolling background
				pxScrollStart = scrollOffset * 160;
				pxScrollEnd = 160*240 - pxScrollStart;
				// cheat using cast to copy faster since the ARM9 registers are 32-bits
				for(int i = pxScrollStart, j = 0; i < 160 * 240; i++, j++)
					((unsigned int*)BUFF_BASE_ADDRESS)[i] = ((unsigned int *)bg)[j];
				for(int i = 0, j = pxScrollEnd; j < 160 * 240; i++, j++)
					((unsigned int*)BUFF_BASE_ADDRESS)[i] = ((unsigned int *)bg)[j];
			}
			else
				clearBufferW();
		}
		if(!has_colors) sleep(6);
		
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
		
		if(K7(kEv)) displayBg = true;
		if(K8(kEv)) displayBg = false;
		
		#ifdef DEBUG_NKARUGA
		sleep(6);
		#endif
	}
	
	for(int i = 0; i < MAX_ENEMY; i++)
		delete enemiesArray[i];
	
	delete bArray;
}
