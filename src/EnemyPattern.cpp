#include "common.h"

EnemyPattern::EnemyPattern()
{
}

EnemyPattern::~EnemyPattern()
{
}

void EnemyPattern::set(void (*func)(Enemy*, Player*))
{
	callback = func;
}

void EnemyPattern::execute(Enemy *e, Player *p)
{
	(*callback)(e, p);
}
