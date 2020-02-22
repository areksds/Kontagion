#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

/*
 
 ADD DERIVED BASE CLASSES FOR FOLLOWING FUNCTIONS:
 isDamagable()
 
 AND OBJECTS:
 Spray and Flame (projectiles?)
 
 
 */


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const double RAD_CONV = 1.0 / 360 * 2 * 4 * atan(1);

/*
 ACTOR
 */

class Actor : public GraphObject
{
   public:
    Actor(int health, int image, double x, double y, Direction dir, int depth, StudentWorld* world);
    virtual void doSomething();
    virtual void Func() = 0;
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
 PROJECTILES
 */

class Projectile : public Actor
{
   public:
    Projectile(int distance);
    void Func();
    bool isDamagable() { return false; }
   private:
    int m_distance;
};

/*
 FLAME
 */

class Flame : public Actor
{
   public:
    Flame(double x, double y, Direction dir, StudentWorld* world);
    void Func();
    bool isDamagable() { return false; }
   private:
    int m_distance = 32;
};

/*
 SPRAY
 */

class Spray : public Actor
{
   public:
    Spray(double x, double y, Direction dir, StudentWorld* world);
    void Func();
    bool isDamagable() { return false; }
   private:
    int m_distance = 112;
};

/*
 DIRT
 */

class Dirt : public Actor
{
   public:
    Dirt(double x, double y, StudentWorld* world);
    void doSomething () {}
    void Func() {}
    
};

/*
 FOOD
 */

class Food : public Actor
{
   public:
    Food(double x, double y, StudentWorld* world);
    void doSomething () {}
    void Func() {}
    bool isDamagable() { return false; }
};

/*
 PIT
 */

class Pit : public Actor
{
   public:
    Pit(double x, double y, StudentWorld* world);
    void Func() {}
    bool isDamagable() { return false; }
    /*
     CODE FUNCTIONS FOR BACTERIA
     */
   private:
    int m_sm = 5;
    int m_asm = 3;
    int m_ec = 2;
};

#endif // ACTOR_H_
