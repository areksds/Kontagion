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

bool Actor::isEdible() const
{
    return false;
}

bool Actor::canBlock() const
{
    return false;
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

bool Dirt::canBlock() const
{
    return true;
}

/*
 FOOD FUNCTIONS
 */

Food::Food(double x, double y, StudentWorld* world) : Inanimate(IID_FOOD, x, y, 90, world) {}

bool Food::isEdible() const
{
    return true;
}

/*
 PIT FUNCTIONS
 */

Pit::Pit(double x, double y, StudentWorld* world) : Inanimate(IID_PIT, x, y, 0, world) {}

/*
 GOODIE FUNCTIONS
 */

Goodie::Goodie(int lifetime, int score, int image, double x, double y, Direction dir, StudentWorld* world) : Inanimate(image, x, y, dir, world), m_lifetime(lifetime), m_score(score) {}

void Goodie::Func()
{
    if (getWorld()->checkOverlap(this,0,true))
    {
        getWorld()->increaseScore(m_score);
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

HealthRestore::HealthRestore(double x, double y, StudentWorld* world) : Goodie(max(randInt(0, 300 - 10 * world->getLevel() - 1), 50), 250, IID_RESTORE_HEALTH_GOODIE, x, y, 0, world) {}

void HealthRestore::goodieEffects()
{
    getWorld()->player()->goodie(0);
}

/*
 FLAMETHROWER GOODIE
 */

Flamethrower::Flamethrower(double x, double y, StudentWorld* world) : Goodie(max(randInt(0, 300 - 10 * world->getLevel() - 1), 50), 300, IID_FLAME_THROWER_GOODIE, x, y, 0, world) {}

void Flamethrower::goodieEffects()
{
    getWorld()->player()->goodie(1);
}

/*
 EXTRA LIFE GOODIE
 */


ExtraLife::ExtraLife(double x, double y, StudentWorld* world) : Goodie(max(randInt(0, 300 - 10 * world->getLevel() - 1), 50), 500, IID_EXTRA_LIFE_GOODIE, x, y, 0, world) {}

void ExtraLife::goodieEffects()
{
    getWorld()->incLives();
}

/*
 FUNGUS
 */

Fungus::Fungus(double x, double y, StudentWorld* world) : Goodie(max(randInt(0, 300 - 10 * world->getLevel() - 1), 50), -50, IID_FUNGUS, x, y, 0, world) {}

void Fungus::goodieEffects()
{
    getWorld()->player()->removeHealth(20);
}

/*
 BACTERIUM
 */

Bacteria::Bacteria(int health, int damage, int image, double x, double y, StudentWorld* world) : Actor(health, image, x, y, 90, 0, world), m_damage(damage) {}

void Bacteria::Func()
{
    if (!getWorld()->checkOverlap(this,m_damage,true))
     {
         if (food() == 3)
         {
             double newx = getX();
             double newy = getY();
             if (getX() != VIEW_RADIUS)
                 newx = newx < VIEW_RADIUS ? + SPRITE_WIDTH/2 : - SPRITE_WIDTH/2;
             if (getY() != VIEW_RADIUS)
                 newy = newy < VIEW_RADIUS ? + SPRITE_WIDTH/2 : - SPRITE_WIDTH/2;
             generate(newx,newy);
             clearFood();
         } else if (getWorld()->checkOverlap(this,0,false,true)) {
             increaseFood();
         }
     }
    
    bacteriaActions();
}

void Bacteria::setRandDir()
{
    setDirection(randInt(0,359));
    setMovement(10);
}

int Bacteria::movement() const
{
    return m_movement;
}

void Bacteria::setMovement(int newMovement)
{
    m_movement = newMovement;
}

int Bacteria::food() const
{
    return m_food;
}

void Bacteria::increaseFood()
{
    m_food++;
}

void Bacteria::clearFood()
{
    m_food = 0;
}

/*
 SALMONELLA
 */

Salmonella::Salmonella(int health, int damage, double x, double y, StudentWorld* world) : Bacteria(health, damage, IID_SALMONELLA, x, y, world) {}

void Salmonella::bacteriaActions()
{
    if (movement() > 0)
    {
        setMovement(movement() - 1);
        double x, y;
        getPositionInThisDirection(getDirection(), 3, x, y);
        if (!getWorld()->checkOverlap(x,y,-1,true) && getWorld()->distance(x,VIEW_RADIUS,y,VIEW_RADIUS) <= VIEW_RADIUS)
        {
            moveAngle(getDirection(),3);
        }
        else
        {
            setRandDir();
        }
        return;
    } else {
        double dist;
        Direction dir;
        if (getWorld()->findFood(getX(),getY(),dist,dir))
        {
            setDirection(getDirection() + dir);
            setMovement(dist/3);
        } else {
            setRandDir();
            return;
        }
    }
   
}

/*
 REGULAR SALMONELLA
 */

RegularSalmonella::RegularSalmonella(double x, double y, StudentWorld* world) : Salmonella(4, 1, x, y, world) {}

void RegularSalmonella::generate(double x, double y)
{
    getWorld()->addBacterium(0,x,y);
}

/*
 AGGRESSIVE SALMONELLA
 */

// ADD AGGRESSIVE ACTION AT BEGINNING OF FUNC

AggressiveSalmonella::AggressiveSalmonella(double x, double y, StudentWorld* world) : Salmonella(10, 2, x, y, world) {}

void AggressiveSalmonella::generate(double x, double y)
{
    getWorld()->addBacterium(1,x,y);
}


