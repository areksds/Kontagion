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

void Socrates::removeHealth(int h)
{
    Actor::removeHealth(h);
    if (isAlive())
    {
       getWorld()->playSound(SOUND_PLAYER_HURT);
    } else {
       getWorld()->playSound(SOUND_PLAYER_DIE);
    }
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

int Socrates::getSprays() const
{
    return m_spray;
}

int Socrates::getFlames() const
{
    return m_fire;
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

Projectile::Projectile(int damage, int distance, int image, double x, double y, Direction dir, StudentWorld* world) : Inanimate(image, x, y, dir, world), m_damage(damage), m_distance(distance) {}

void Projectile::Func()
{
    if (getWorld()->checkOverlap(this,m_damage))
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

Flame::Flame(double x, double y, Direction dir, StudentWorld* world) : Projectile(5, 32,  IID_FLAME, x, y, dir, world) {}

/*
 SPRAY FUNCTIONS
 */

Spray::Spray(double x, double y, Direction dir, StudentWorld* world) : Projectile(2, 112,  IID_SPRAY, x, y, dir, world) {}

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

void Pit::doSomething()
{
    if (bact[0] == 0 && bact[1] == 0 && bact[2] == 0)
        kill();
    else
    {
        if (randInt(1,50) == 1)
        {
            int check = 0;
            for (int i = 0; i < 3; i++)
                if (bact[i] > 0)
                    check++;
            check = randInt(0,check);
            for (int i = 0; i < 3; i++)
            {
                if (bact[i] > 0)
                {
                    if (check == 0)
                    {
                        getWorld()->playSound(SOUND_BACTERIUM_BORN);
                        getWorld()->addBacterium(i, getX(), getY());
                        bact[i]--;
                        break;
                    }
                    check--;
                }
            }
        }
    }
}

/*
 GOODIE FUNCTIONS
 */

Goodie::Goodie(int lifetime, int score, int image, double x, double y, StudentWorld* world) : Inanimate(image, x, y, 0, world), m_lifetime(lifetime), m_score(score) {}

void Goodie::Func()
{
    if (getWorld()->checkOverlap(this,0,true))
    {
        getWorld()->increaseScore(m_score);
        goodieEffects();
        kill();
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

HealthRestore::HealthRestore(double x, double y, StudentWorld* world) : Goodie(max(randInt(0, 300 - 10 * world->getLevel() - 1), 50), 250, IID_RESTORE_HEALTH_GOODIE, x, y, world) {}

void HealthRestore::goodieEffects()
{
    getWorld()->player()->goodie(0);
    getWorld()->playSound(SOUND_GOT_GOODIE);
}

/*
 FLAMETHROWER GOODIE
 */

Flamethrower::Flamethrower(double x, double y, StudentWorld* world) : Goodie(max(randInt(0, 300 - 10 * world->getLevel() - 1), 50), 300, IID_FLAME_THROWER_GOODIE, x, y, world) {}

void Flamethrower::goodieEffects()
{
    getWorld()->player()->goodie(1);
    getWorld()->playSound(SOUND_GOT_GOODIE);
}

/*
 EXTRA LIFE GOODIE
 */


ExtraLife::ExtraLife(double x, double y, StudentWorld* world) : Goodie(max(randInt(0, 300 - 10 * world->getLevel() - 1), 50), 500, IID_EXTRA_LIFE_GOODIE, x, y, world) {}

void ExtraLife::goodieEffects()
{
    getWorld()->incLives();
    getWorld()->playSound(SOUND_GOT_GOODIE);
}

/*
 FUNGUS
 */

Fungus::Fungus(double x, double y, StudentWorld* world) : Goodie(max(randInt(0, 300 - 10 * world->getLevel() - 1), 50), -50, IID_FUNGUS, x, y, world) {}

void Fungus::goodieEffects()
{
    getWorld()->player()->removeHealth(20);
}

/*
 BACTERIUM
 */

Bacteria::Bacteria(int health, int damage, int hurtSound, int deathSound, int image, double x, double y, StudentWorld* world) : Actor(health, image, x, y, 90, 0, world), m_damage(damage), m_hurtSound(hurtSound), m_deathSound(deathSound) {}

void Bacteria::Func()
{
    bool wasAggressive = aggressive(); // checks for aggressive actions
    
    if (!getWorld()->checkOverlap(this,m_damage,true))
     {
         if (food() == 3)
         {
             double newx = getX();
             double newy = getY();
             if (getX() != VIEW_RADIUS)
                 newx = newx < VIEW_RADIUS ? newx + SPRITE_WIDTH/2 : newx - SPRITE_WIDTH/2;
             if (getY() != VIEW_RADIUS)
                 newy = newy < VIEW_RADIUS ? newy + SPRITE_WIDTH/2 : newy - SPRITE_WIDTH/2;
             generate(newx,newy);
             getWorld()->increaseBact();
             clearFood();
         } else if (getWorld()->checkOverlap(this,0,false,true))
             increaseFood();
     }
    
    if (!wasAggressive)
    {
        bacteriaActions();
    }
}

void Bacteria::removeHealth(int h)
{
    Actor::removeHealth(h);
    if (isAlive())
    {
        getWorld()->playSound(m_hurtSound);
    } else {
        getWorld()->decreaseBact();
        getWorld()->playSound(m_deathSound);
        getWorld()->increaseScore(100);
        if (randInt(0,1) == 0)
            getWorld()->addFood(getX(),getY());
    }
}

bool Bacteria::move(int units)
{
    double x, y;
    getPositionInThisDirection(getDirection(), units, x, y);
    if (!getWorld()->checkOverlap(x,y,-1,true) &&  getWorld()->distance(x,VIEW_RADIUS,y,VIEW_RADIUS) <= VIEW_RADIUS)
    {
        moveAngle(getDirection(),units);
        return true;
    }
    else
        return false;
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

Salmonella::Salmonella(int health, int damage, double x, double y, StudentWorld* world) : Bacteria(health, damage, SOUND_SALMONELLA_HURT, SOUND_SALMONELLA_DIE, IID_SALMONELLA, x, y, world) {}

void Salmonella::bacteriaActions()
{
    if (movement() > 0)
    {
        setMovement(movement() - 1);
        if(!move(3))
            setRandDir();
        return;
    } else {
        Direction dir;
        if (getWorld()->findFood(getX(),getY(),dir))
        {
            setDirection(dir);
            if(!move(3))
                setRandDir();
        } else
            setRandDir();
        return;
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

AggressiveSalmonella::AggressiveSalmonella(double x, double y, StudentWorld* world) : Salmonella(10, 2, x, y, world) {}

void AggressiveSalmonella::generate(double x, double y)
{
    getWorld()->addBacterium(1,x,y);
}

bool AggressiveSalmonella::aggressive()
{
    Direction dir;
    if (getWorld()->findSocrates(getX(), getY(), dir, 72))
    {
        setDirection(dir);
        move(3);
        return true;
    }
    return false;
}

/*
 ECOLI
 */

Ecoli::Ecoli(double x, double y, StudentWorld* world) : Bacteria(5, 4, SOUND_ECOLI_HURT, SOUND_ECOLI_DIE, IID_ECOLI, x, y, world) {}

void Ecoli::generate(double x, double y)
{
    getWorld()->addBacterium(2,x,y);
}

void Ecoli::bacteriaActions()
{
    Direction dir;
    if (getWorld()->findSocrates(getX(), getY(), dir, 256))
    {
        setDirection(dir);
        for (int i = 0; i < 10; i++)
        {
            if (move(2))
                return;
            else
                setDirection(getDirection() + 10);
        }
    }
}

