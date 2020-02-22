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
    
    int generated = 0;
    
    // INITIALIZE PITS

    generateActors<Pit>(getLevel(), generated);
    
    /*
    
    num = getLevel();
     
    while (num > 0)
    {
        for (;;)
        {
            x = randInt(VIEW_RADIUS - 120, VIEW_RADIUS + 120);
            y = randInt(VIEW_RADIUS - 120, VIEW_RADIUS + 120);
            if (sqrt(pow(x-VIEW_RADIUS,2)+(pow(y-VIEW_RADIUS,2))) <= 120 && !checkOverlap(x,y,generated))
                break;
        }
        m_actors.push_back(new Pit(x, y, this));
        generated++;
        num--;
    } */
    
    // INITIALIZE FOOD
    
    generateActors<Food>(min(5 * getLevel(), 25), generated);
    
    /*
    num = min(5 * getLevel(), 25);
    
    while (num > 0)
    {
        for (;;)
        {
            x = randInt(VIEW_RADIUS - 120, VIEW_RADIUS + 120);
            y = randInt(VIEW_RADIUS - 120, VIEW_RADIUS + 120);
            if (sqrt(pow(x-VIEW_RADIUS,2)+(pow(y-VIEW_RADIUS,2))) <= 120 && !checkOverlap(x,y,generated))
                break;
        }
        m_actors.push_back(new Food(x, y, this));
        generated++;
        num--;
    } */
    
    // INITIALIZE DIRT
    
    generateActors<Dirt>(max(180 - 20 * getLevel(), 20), generated, false);
    
    /*
    num = max(180 - 20 * getLevel(), 20);

    while (num > 0)
    {
        for (;;)
        {
            x = randInt(VIEW_RADIUS - 120, VIEW_RADIUS + 120);
            y = randInt(VIEW_RADIUS - 120, VIEW_RADIUS + 120);
            if (sqrt(pow(x-VIEW_RADIUS,2)+(pow(y-VIEW_RADIUS,2))) <= 120 && !checkOverlap(x,y,generated))
                break;
        }
        m_actors.push_back(new Dirt(x, y, this));
        num--;
    }
     */
    
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
        
        if (!m_player->isAlive())
            return GWSTATUS_PLAYER_DIED;
        
        // IF SOCRATES FINISHED LEVEL
        // return GWSTATUS_FINISHED_LEVEL;
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

bool StudentWorld::checkOverlap(double x, double y, int num)
{
    if (num >= m_actors.size())
        return false;
    if (num == -1)
        num = static_cast<int>(m_actors.size());
    for (int i = 0; i != num; i++)
    {
        if (sqrt(pow(m_actors[i]->getX() - x,2) + pow(m_actors[i]->getY() - y,2)) <= SPRITE_WIDTH)
            return true;
    }
    return false;
}

/*
 PRIVATE MEMBER TEMPLATES AND FUNCTIONS
 */

template<typename T>
void StudentWorld::generateActors(int num, int& existing, bool increment)
{
    int x, y;
    while (num > 0)
    {
        for (;;)
        {
            x = randInt(VIEW_RADIUS - 120, VIEW_RADIUS + 120);
            y = randInt(VIEW_RADIUS - 120, VIEW_RADIUS + 120);
            if (sqrt(pow(x-VIEW_RADIUS,2)+(pow(y-VIEW_RADIUS,2))) <= 120 && !checkOverlap(x,y,existing))
                break;
        }
        m_actors.push_back(new T(x, y, this));
        if (increment)
            existing++;
        num--;
    }
}
