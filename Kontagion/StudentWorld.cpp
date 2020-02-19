#include <string>
#include <vector>
#include <cmath>
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    // INITIALIZE PLAYER
    m_player = new Socrates(this);
    
    int num;
    int x, y;
    
    // INITIALIZE DIRT
    num = max(180 - 20 * getLevel(), 20);

    while (num > 0)
    {
        for (;;)
        {
            x = randInt(VIEW_RADIUS - 120, VIEW_RADIUS + 120);
            y = randInt(VIEW_RADIUS - 120, VIEW_RADIUS + 120);
            if (sqrt(pow(x-VIEW_RADIUS,2)+(pow(y-VIEW_RADIUS,2))) <= 120)
                break;
        }
        m_actors.push_back(new Dirt(x, y, this));
        num--;
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    /*
     BE SURE TO CHECK FOR DEAD ACTORS
     */
    m_player->doSomething();
    for (int i = 0; i != m_actors.size(); i++)
    {
        m_actors[i]->doSomething();
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if (m_player != nullptr)
        delete m_player;
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); )
    {
        delete *it;
        it = m_actors.erase(it);
    }
}

/*
 PRIVATE MEMBER FUNCTIONS
 */
