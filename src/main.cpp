#include "common.h"
#include "levels.h"

int main(int argc, char **argv) {
	KeyEvent kEv = 0;
	int levelCounter, levelTimer, enemyCounter, waveIndex;
	bool levelEnded = false;
	
	initBuffering();
	
	enable_relative_paths(argv);
	
	buildGameLUTs();
	
	Player ship(32);
	Enemy **enemiesArray;
	
	enemiesArray = (Enemy**)malloc(sizeof(Enemy*) * MAX_ENEMY);
	if(!enemiesArray)
	{
		printf("Couldn't allocate %d bytes for enemiesArray.\n", sizeof(Enemy) * MAX_ENEMY);
		exit(0);
	}
	
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
	
	while(!KQUIT(kEv) && !levelEnded)
	{
		if(!levelTimer)
		{
			// Load the current enemy from the level stream
			if(levelStream[levelCounter] == LVLSTR_CMD)
			{
				levelCounter++;
				if(levelStream[levelCounter] == LVLSTR_NEWWAVE)
				{
					waveIndex = 0;
					levelCounter++;
				}
				else if(levelStream[levelCounter] == LVLSTR_WAIT)
				{
					levelTimer = levelStream[levelCounter + 1];
					levelCounter += 2;
				}
				else if(levelStream[levelCounter] == LVLSTR_KILLED)
				{
					int levelCanProgress = 1;
					for(int i = 0; i < MAX_ENEMY; i++)
					{
						if(enemiesArray[i]->isActive())
						{
							levelCanProgress = 0;
						}
					}
					if(levelCanProgress) levelCounter++;
					else levelCounter--;
				}
			}
			else if(levelStream[levelCounter] == LVLSTR_END)
			{
				levelEnded = true;
			}
			else
			{
				enemiesArray[enemyCounter]->activate(itofix(levelStream[levelCounter]), itofix(levelStream[levelCounter + 1]), levelStream[levelCounter + 2], levelStream[levelCounter + 3],
													levelStream[levelCounter + 4], levelStream[levelCounter + 5], waveIndex, levelStream[levelCounter + 6], levelStream[levelCounter + 7]);
				levelCounter += 8;
				enemyCounter = (enemyCounter + 1) & MAX_ENEMY - 1;
				waveIndex++;
			}
		}
		else
			levelTimer--;
		
		kEv = getk();
		ship.handle(kEv, enemiesArray);
		
		for(int i = 0; i < MAX_ENEMY; i++)
			enemiesArray[i]->handle(&ship);
	
		if(!ship.getLives())
			levelEnded = 1;
	
		updateScreen();
		clearBufferW();
		sleep(4);
	}
	
	for(int i = 0; i < MAX_ENEMY; i++)
		delete enemiesArray[i];
	free(enemiesArray);
	
	deinitBuffering();
	return 0;
}
