#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

const double PI = 4 * atan(1);

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

/*
 ACTOR
 */

class Actor : public GraphObject
{
   public:
    Actor(int health, int image, double x, double y, Direction dir, int depth, StudentWorld* world);
    virtual void doSomething();
    virtual void func() = 0;
    bool isAlive() const;
    int getHealth() const;
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
    void func();
    void move();
   private:
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
    void func() {}
};

#endif // ACTOR_H_
