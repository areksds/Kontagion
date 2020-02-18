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
    func();
}

bool Actor::isAlive() const
{
    return m_alive;
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

void Socrates::func()
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
                setDirection(getDirection() + 5);
                move();
                break;
            }
            case KEY_PRESS_RIGHT:
            {
                setDirection(getDirection() - 5);
                move();
                break;
            }
        }
    }
}

void Socrates::move()
{
    double x = VIEW_RADIUS + cos((getDirection() + 180) * 1.0 / 360 * 2 * PI)*VIEW_RADIUS;
    double y = VIEW_RADIUS + sin((getDirection() + 180) * 1.0 / 360 * 2 * PI)*VIEW_RADIUS;
    moveTo(x, y);
}

/*
 DIRT FUNCTIONS
 */

Dirt::Dirt(double x, double y, StudentWorld* world) : Actor(0, IID_DIRT, x, y, 0, 1, world) {}

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
