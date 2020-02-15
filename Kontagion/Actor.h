#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

/*
 ACTOR BASE CLASS
 */

class Actor : public GraphObject
{
   public:
    Actor(int health, int image, double x, double y, Direction dir, int depth);
    virtual void doSomething() = 0;
    bool isAlive();
    int getHealth();
    void removeHealth(int h);
    void kill();
   private:
    bool m_alive;
    int m_health;
};

/*
 SOCRATES
 */

/*
 DIRT
 */

class Dirt : public Actor
{
   public:
    Dirt(double x, double y);
    void doSomething() {}
};

#endif // ACTOR_H_
