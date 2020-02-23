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
    virtual bool isEdible() const;
    virtual bool canBlock() const;
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
    void goodie(int which);
   private:
    void moveSocrates(Direction dir);
    int m_spray = 20;
    int m_fire = 5;
};

/*
 INANIMATES (BASE)
 */

class Inanimate : public Actor
{
   public:
    Inanimate(int image, double x, double y, Direction dir, StudentWorld* world);
    bool isDamagable() const;
};

/*
 PROJECTILES (BASE)
 */

class Projectile : public Inanimate
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
    bool canBlock() const;
};

/*
 FOOD
 */

class Food : public Inanimate
{
   public:
    Food(double x, double y, StudentWorld* world);
    bool isEdible() const;
    void doSomething () {}
};

/*
 PIT
 */

class Pit : public Inanimate
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

class Goodie : public Inanimate
{
   public:
    Goodie(int lifetime, int score, int image, double x, double y, Direction dir, StudentWorld* world);
    virtual void goodieEffects() = 0;
    bool isDamagable() const;
    void Func();
   private:
    int m_score;
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

/*
 BACTERIUM
 */

class Bacteria : public Actor
{
   public:
    Bacteria(int health, int damage, int image, double x, double y, StudentWorld* world);
    void Func();
    virtual void bacteriaActions() = 0;
    virtual void generate(double x, double y) = 0;
    void setRandDir();
    int food() const;
    int movement() const;
    void setMovement(int newMovement);
    void increaseFood();
    void clearFood();
   private:
    int m_damage;
    int m_movement = 0;
    int m_food = 0;
};

/*
 SALMONELLA
 */

class Salmonella : public Bacteria
{
   public:
    Salmonella(int health, int damage, double x, double y, StudentWorld* world);
    void bacteriaActions();
};

/*
 REGULAR SALMONELLA
*/

class RegularSalmonella : public Salmonella
{
   public:
    RegularSalmonella(double x, double y, StudentWorld* world);
    void generate(double x, double y);
};

/*
 AGGRESSIVE SALMONELLA
*/

class AggressiveSalmonella : public Salmonella
{
   public:
    AggressiveSalmonella(double x, double y, StudentWorld* world);
    void generate(double x, double y);
};

/*
 ECOLI
 */

class Ecoli : public Bacteria
{
   public:
    Ecoli(double x, double y, StudentWorld* world);
    void generate(double x, double y);
};

#endif // ACTOR_H_
