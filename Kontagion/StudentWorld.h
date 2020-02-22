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
    bool checkOverlap(double x, double y, int num = -1);
   private:
    template<typename T>
        void generateActors(int num, int& existing, bool increment = true);
    std::vector<Actor*> m_actors;
    Socrates* m_player = nullptr;
};

#endif // STUDENTWORLD_H_
