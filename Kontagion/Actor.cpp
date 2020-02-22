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
            {
                if (m_spray >= 1)
                {
                    getWorld()->addProjectile(0, getX(),getY(),getDirection());
                    m_spray--;
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                }
                break;
            }
            case KEY_PRESS_ENTER:
            {
                if (m_fire >= 1)
                {
                    for (int i = 0; i < 16; i++)
                        getWorld()->addProjectile(1, getX(),getY(),getDirection() + i * 22);
                    m_fire--;
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                break;
            }
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
    } else {
        if (m_spray < 20)
            m_spray++;
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
 IMMORTAL FUNCTIONS
 */

Immortal::Immortal(int image, double x, double y, Direction dir, StudentWorld* world) : Actor(0, image, x, y, dir, 1, world) {}

bool Immortal::isDamagable() const
{
    return false;
}

/*
 PROJECTILE FUNCTIONS
 */

Projectile::Projectile(int distance, int image, double x, double y, Direction dir, StudentWorld* world) : Immortal(image, x, y, dir, world), m_distance(distance) {}

void Projectile::Func()
{
    if (getWorld()->checkOverlap(this,5))
    {
        kill();
        return;
    }
    moveAngle(getDirection(), SPRITE_WIDTH);
    m_distance -= SPRITE_WIDTH;
    if (m_distance < 0)
        kill();
}

/*
 FLAME FUNCTIONS
 */

Flame::Flame(double x, double y, Direction dir, StudentWorld* world) : Projectile(32,  IID_FLAME, x, y, dir, world) {}

/*
 FLAME FUNCTIONS
 */

Spray::Spray(double x, double y, Direction dir, StudentWorld* world) : Projectile(112,  IID_SPRAY, x, y, dir, world) {}

/*
 DIRT FUNCTIONS
 */

Dirt::Dirt(double x, double y, StudentWorld* world) : Actor(0, IID_DIRT, x, y, 0, 1, world) {}

/*
 FOOD FUNCTIONS
 */

Food::Food(double x, double y, StudentWorld* world) : Immortal(IID_FOOD, x, y, 90, world) {}

/*
 PIT FUNCTIONS
 */

Pit::Pit(double x, double y, StudentWorld* world) : Immortal(IID_PIT, x, y, 0, world) {}

/*
 GOODIE FUNCTIONS
 */

Goodie::Goodie(int lifetime, int image, double x, double y, Direction dir, StudentWorld* world) : Immortal(image, x, y, dir, world), m_lifetime(lifetime) {}

void Goodie::Func()
{
    if (getWorld()->checkOverlap(this,0,true))
        goodieEffects();
    if (m_lifetime <= 0)
        kill();
    m_lifetime--;
}
