#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

/*
TO DO LIST:
 
 */


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const double RAD_CONV = 1.0 / 360 * 2 * 4 * atan(1);

/*
 ACTOR (BASE)
 */

class Actor : public GraphObject
{
   public:
    Actor(int health, int image, double x, double y, Direction dir, int depth, StudentWorld* world);
    virtual void doSomething();
    virtual void Func() {}
    virtual bool isDamagable() const;
    int getHealth() const;
    bool isAlive() const;
    void removeHealth(int h);
    void kill();
   protected:
    StudentWorld* getWorld() const;
   private:
    StudentWorld* m_world;
    bool m_alive = true;
    int m_health;
};

/*
 SOCRATES
 */

class Socrates : public Actor
{
   public:
    Socrates(StudentWorld* world);
    void Func();
   private:
    void moveSocrates(Direction dir);
    int m_spray = 20;
    int m_fire = 5;
};

/*
 IMMORTALS (BASE)
 */

class Immortal : public Actor
{
   public:
    Immortal(int image, double x, double y, Direction dir, StudentWorld* world);
    bool isDamagable() const;
};

/*
 PROJECTILES (BASE)
 */

class Projectile : public Immortal
{
   public:
    Projectile(int distance, int image, double x, double y, Direction dir, StudentWorld* world);
    void Func();
   private:
    int m_distance;
};

/*
 FLAME
 */

class Flame : public Projectile
{
   public:
    Flame(double x, double y, Direction dir, StudentWorld* world);
};

/*
 SPRAY
 */

class Spray : public Projectile
{
   public:
    Spray(double x, double y, Direction dir, StudentWorld* world);
};

/*
 DIRT
 */

class Dirt : public Actor
{
   public:
    Dirt(double x, double y, StudentWorld* world);
    void doSomething () {}
};

/*
 FOOD
 */

class Food : public Immortal
{
   public:
    Food(double x, double y, StudentWorld* world);
    void doSomething () {}
};

/*
 PIT
 */

class Pit : public Immortal
{
   public:
    Pit(double x, double y, StudentWorld* world);
    /*
     CODE FUNCTIONS FOR BACTERIA
     */
   private:
    int m_sm = 5;
    int m_asm = 3;
    int m_ec = 2;
};

/*
 GOODIE (BASE)
 */

class Goodie : public Immortal
{
   public:
    Goodie(int lifetime, int image, double x, double y, Direction dir, StudentWorld* world);
    virtual void goodieEffects() = 0;
    void Func();
   private:
    int m_lifetime;
};

/*
 RESTORE HEALTH GOODIE
 */

class HealthRestore : public Goodie
{
   public:
    HealthRestore(double x, double y, StudentWorld* world);
    void goodieEffects();
};

/*
 FLAMETHROWER GOODIE
 */

class Flamethrower : public Goodie
{
   public:
    Flamethrower(double x, double y, StudentWorld* world);
    void goodieEffects();
};

/*
 EXTRA LIFE GOODIE
 */

class ExtraLife : public Goodie
{
   public:
    ExtraLife(double x, double y, StudentWorld* world);
    void goodieEffects();
};

/*
 FUNGUS
 */

class Fungus : public Goodie
{
   public:
    Fungus(double x, double y, StudentWorld* world);
    void goodieEffects();
};

#endif // ACTOR_H_
