#include "Actor.h"
#include "StudentWorld.h"

/*
 ACTOR FUNCTIONS
 */

Actor::Actor(int health, int image, double x, double y, Direction dir, int depth) : GraphObject(image, x, y, dir, depth), m_health(health), m_alive(true) {}

bool Actor::isAlive()
{
    return m_alive;
}

int Actor::getHealth()
{
    return m_health;
}

void Actor::removeHealth(int h)
{
    if (m_health != -1)
    {
        m_health -= h;
        if (m_health <= 0)
            kill();
    }
}

void Actor::kill()
{
    m_alive = false;
}

/*
 DIRT FUNCTIONS
 */

Dirt::Dirt(double x, double y) : Actor(-1, IID_DIRT, x, y, 90, 1) {}

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
