#include "common.h"

Entity::Entity()
{
	x = y = 0;
	active = false;
}

Entity::~Entity()
{
}

Fixed Entity::getx()
{
	return x;
}

void Entity::setx(Fixed _x)
{
	x = _x;
}

Fixed Entity::gety()
{
	return y;
}

void Entity::sety(Fixed _y)
{
	y = _y;
}

bool Entity::isActive()
{
	return active;
}

void Entity::activate()
{
	active = true;
}

void Entity::deactivate()
{
	active = false;
}
