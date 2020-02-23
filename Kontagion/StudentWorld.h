#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <string>
#include <vector>
#include "GameWorld.h"
#include "Actor.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
   public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    Socrates* player() const;
    bool checkOverlap(double x, double y, int num = -1, bool block = false) const;
    bool checkOverlap(Actor* original, int damage = 0, bool player = false, bool food = false);
    void addProjectile(int type, double x, double y, Direction dir);
    void addBacterium(int type, double x, double y);
    bool findFood(double x, double y, Direction& dir) const;
    bool findSocrates(double x, double y, Direction& dir, double dist) const;
    double distance(double x1, double x2, double y1, double y2) const;
   private:
    template<typename T>
        void generateActors(int num, int& existing, bool increment = true);
    void randPoint(double& x, double&y, double maxLength, bool only = false) const;
    std::vector<Actor*> m_actors;
    Socrates* m_player = nullptr;
    int numBact = 10;
};

#endif // STUDENTWORLD_H_
