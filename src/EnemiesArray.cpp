#include "common.h"

EnemiesArray::EnemiesArray()
{
	currentEnemy = 0;
	currentExplosion = 0;
}

EnemiesArray::~EnemiesArray()
{
	
}

void EnemiesArray::add(int x, int y, int HP, int shipImgID, int callbackID, int waveIndex, bool polarity, bool hasRotation, int firebackAmount, bool ghost)
{
	data[currentEnemy].activate(x, y, HP, shipImgID, callbackID, waveIndex, polarity, hasRotation, firebackAmount, ghost);
	currentEnemy++;
	currentEnemy %= MAX_ENEMY;
}

void EnemiesArray::handle(Player *p, BulletArray *bArray)
{
	for(int i = 0; i < MAX_ENEMY; i++)
	{
		G_killedThisFrame[i] = -1;
		if(data[i].diedThisFrame)
		{
			G_killedThisFrame[i] = data[i].getPolarity();
			deadEnemies.activate(&data[i], i);
			explosionsAnims[currentExplosion].activate(fixtoi(data[i].getx()),
													   fixtoi(data[i].gety()),
													   data[i].getPolarity());
			currentExplosion++;
			currentExplosion %= MAX_ENEMY;
		}
		data[i].handle(p, bArray);
	}
}

void EnemiesArray::handleExplosions()
{
	for(int i = 0; i < MAX_ENEMY; i++)
		explosionsAnims[i].handle();
}

void EnemiesArray::resetEnemyCounter()
{
	currentEnemy = 0;
}

void EnemiesArray::destroyAllEnemies(Player *p, BulletArray *bArray)
{
	for(int i = 0; i < MAX_ENEMY; i++)
		if(data[i].isActive())
			data[i].damage(p, data[i].getPolarity(), 5000, bArray);
}