#include <cmath>
#include "Actor.h"
#include "StudentWorld.h"

/*
 ACTOR FUNCTIONS
 */

Actor::Actor(int health, int image, double x, double y, Direction dir, int depth, StudentWorld* world) : GraphObject(image, x, y, dir, depth), m_health(health), m_world(world) {}

void Actor::doSomething()
{
    if (!isAlive())
        return;
    Func();
}

bool Actor::isAlive() const
{
    return m_alive;
}

bool Actor::isDamagable() const
{
    return true;
}

int Actor::getHealth() const
{
    return m_health;
}

void Actor::removeHealth(int h)
{
    m_health -= h;
    if (m_health <= 0)
        kill();
}

void Actor::kill()
{
    m_alive = false;
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

/*
 SOCRATES FUNCTIONS
 */

Socrates::Socrates(StudentWorld* world) : Actor(100, IID_PLAYER, 0, VIEW_HEIGHT/2, 0, 0, world) {}

void Socrates::Func()
{
    int ch;
    if (getWorld()->getKey(ch))
    {
        switch (ch)
        {
            case KEY_PRESS_SPACE:
            // code later
                break;
            case KEY_PRESS_ENTER:
            // code later
                break;
            case KEY_PRESS_LEFT:
            {
                moveSocrates(5);
                break;
            }
            case KEY_PRESS_RIGHT:
            {
                moveSocrates(-5);
                break;
            }
        }
    }
}

void Socrates::moveSocrates(Direction dir)
{
    setDirection(getDirection() + dir);
    double x = VIEW_RADIUS + cos((getDirection() + 180) * RAD_CONV)*VIEW_RADIUS;
    double y = VIEW_RADIUS + sin((getDirection() + 180) * RAD_CONV)*VIEW_RADIUS;
    moveTo(x, y);
}

/*
 FLAME FUNCTIONS
 */

Flame::Flame(double x, double y, Direction dir, StudentWorld* world) : Actor(0, IID_FLAME, x, y, dir, 1, world) {}

void Flame::Func()
{
    while (m_distance > 0)
    {
        if (getWorld()->checkOverlap(this,5))
        {
            kill();
            return;
        }
        moveAngle(getDirection(), SPRITE_WIDTH);
        m_distance -= SPRITE_WIDTH;
    }
}

/*
 FLAME FUNCTIONS
 */

Spray::Spray(double x, double y, Direction dir, StudentWorld* world) : Actor(0, IID_SPRAY, x, y, dir, 1, world) {}

void Spray::Func()
{
    while (m_distance > 0)
    {
        if (getWorld()->checkOverlap(this,2))
        {
            kill();
            return;
        }
        moveAngle(getDirection(), SPRITE_WIDTH);
        m_distance -= SPRITE_WIDTH;
    }
}

/*
 DIRT FUNCTIONS
 */

Dirt::Dirt(double x, double y, StudentWorld* world) : Actor(0, IID_DIRT, x, y, 0, 1, world) {}

/*
 FOOD FUNCTIONS
 */

Food::Food(double x, double y, StudentWorld* world) : Actor(0, IID_FOOD, x, y, 90, 1, world) {}

/*
 PIT FUNCTIONS
 */

Pit::Pit(double x, double y, StudentWorld* world) : Actor(0, IID_PIT, x, y, 0, 1, world) {}
