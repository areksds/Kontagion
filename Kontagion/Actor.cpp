#include <cmath>
#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

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

void Socrates::goodie(int which)
{
    switch(which)
    {
        case 0: // Health goodie
            removeHealth(-(100-getHealth()));
            break;
        case 1: // Flamethrower goodie
            m_fire += 5;
            break;
    }
}

/*
 INANIMATE FUNCTIONS
 */

Inanimate::Inanimate(int image, double x, double y, Direction dir, StudentWorld* world) : Actor(0, image, x, y, dir, 1, world) {}

bool Inanimate::isDamagable() const
{
    // Not damagable by default
    return false;
}

/*
 PROJECTILE FUNCTIONS
 */

Projectile::Projectile(int distance, int image, double x, double y, Direction dir, StudentWorld* world) : Inanimate(image, x, y, dir, world), m_distance(distance) {}

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

Food::Food(double x, double y, StudentWorld* world) : Inanimate(IID_FOOD, x, y, 90, world) {}

/*
 PIT FUNCTIONS
 */

Pit::Pit(double x, double y, StudentWorld* world) : Inanimate(IID_PIT, x, y, 0, world) {}

/*
 GOODIE FUNCTIONS
 */

Goodie::Goodie(int lifetime, int image, double x, double y, Direction dir, StudentWorld* world) : Inanimate(image, x, y, dir, world), m_lifetime(lifetime) {}

void Goodie::Func()
{
    if (getWorld()->checkOverlap(this,0,true))
    {
        goodieEffects();
        kill();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        return;
    }
    if (m_lifetime <= 0)
        kill();
    m_lifetime--;
}

bool Goodie::isDamagable() const
{
    return true;
}

/*
 RESTORE HEALTH GOODIE
 */

HealthRestore::HealthRestore(double x, double y, StudentWorld* world) : Goodie(max(randInt(0, 300 - 10 * world->getLevel() - 1), 50), IID_RESTORE_HEALTH_GOODIE, x, y, 0, world) {}

void HealthRestore::goodieEffects()
{
    getWorld()->increaseScore(250);
    getWorld()->player()->goodie(0);
}

/*
 FLAMETHROWER GOODIE
 */

Flamethrower::Flamethrower(double x, double y, StudentWorld* world) : Goodie(max(randInt(0, 300 - 10 * world->getLevel() - 1), 50), IID_FLAME_THROWER_GOODIE, x, y, 0, world) {}

void Flamethrower::goodieEffects()
{
    getWorld()->increaseScore(300);
    getWorld()->player()->goodie(1);
}

/*
 EXTRA LIFE GOODIE
 */


ExtraLife::ExtraLife(double x, double y, StudentWorld* world) : Goodie(max(randInt(0, 300 - 10 * world->getLevel() - 1), 50), IID_EXTRA_LIFE_GOODIE, x, y, 0, world) {}

void ExtraLife::goodieEffects()
{
    getWorld()->increaseScore(500);
    getWorld()->incLives();
}

/*
 FUNGUS
 */

Fungus::Fungus(double x, double y, StudentWorld* world) : Goodie(max(randInt(0, 300 - 10 * world->getLevel() - 1), 50), IID_FUNGUS, x, y, 0, world) {}

void Fungus::goodieEffects()
{
    getWorld()->increaseScore(-50);
    getWorld()->player()->removeHealth(20);
}


