#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const double RAD_CONV = 1.0 / 360 * 2 * 4 * atan(1);

/*
 ACTOR
 */

class Actor : public GraphObject
{
   public:
    Actor(int health, int image, double x, double y, Direction dir, int depth, StudentWorld* world);
    virtual void doSomething() = 0;
    bool isAlive() const;
   protected:
    int getHealth() const;
    StudentWorld* getWorld() const;
    void removeHealth(int h);
    void kill();
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
    void doSomething();
   private:
    void moveSocrates(Direction dir);
    int m_spray = 20;
    int m_fire = 5;
};

/*
 DIRT
 */

class Dirt : public Actor
{
   public:
    Dirt(double x, double y, StudentWorld* world);
    void doSomething() {}
};

/*
 FOOD
 */

class Food : public Actor
{
   public:
    Food(double x, double y, StudentWorld* world);
    void doSomething() {}
};

/*
 PIT
 */

class Pit : public Actor
{
   public:
    Pit(double x, double y, StudentWorld* world);
    void doSomething() {}
    /*
     CODE FUNCTIONS FOR BACTERIA
     */
   private:
    int m_sm = 5;
    int m_asm = 3;
    int m_ec = 2;
};

#endif // ACTOR_H_
