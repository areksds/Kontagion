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
    
    // INITIALIZE FOOD
    
    generateActors<Food>(min(5 * getLevel(), 25), generated);
    
    // INITIALIZE DIRT
    
    generateActors<Dirt>(max(180 - 20 * getLevel(), 20), generated, false);
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{

    m_player->doSomething();
    for (int i = 0; i != m_actors.size(); i++)
    {
        
        if (m_actors[i]->isAlive())
            m_actors[i]->doSomething();
        
        if (!m_player->isAlive())
            return GWSTATUS_PLAYER_DIED;
        
        // IF SOCRATES FINISHED LEVEL
        // return GWSTATUS_FINISHED_LEVEL;
    }
    
    /*
     REMOVE DEAD ACTORS
     */
    
    /*
     ADD NEW GOODIES IF NECESSARY
     */
    
    /*
     UPDATE THE GAME STATUS LINE
     */
    
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

bool StudentWorld::checkOverlap(Actor* original, int damage)
{
    for (int i = 0; i != m_actors.size(); i++)
    {
        if (sqrt(pow(m_actors[i]->getX() - original->getX(),2) + pow(m_actors[i]->getY() - original->getY(),2)) <= SPRITE_WIDTH)
        {
            if (damage > 0)
            {
                if (m_actors[i]->isDamagable())
                {
                    m_actors[i]->removeHealth(damage);
                    return true;
                }
            }
            else
                return true;
                
        }
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
