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

    // PLAYER ACTION
    m_player->doSomething();
    
    // ACTOR ACTIONS
    for (int i = 0; i != m_actors.size(); i++)
    {
        if (m_actors[i]->isAlive())
            m_actors[i]->doSomething();
        
        if (!m_player->isAlive())
            return GWSTATUS_PLAYER_DIED;
        
        // IF SOCRATES FINISHED LEVEL
        // return GWSTATUS_FINISHED_LEVEL;
    }
    
    // ACTOR CLEANUP
    for (int i = 0; i != m_actors.size(); i++)
    {
        if (!m_actors[i]->isAlive())
        {
            delete m_actors[i];
            m_actors.erase(m_actors.begin() + i);
            i--;
        }
    }
    
    /*
     ADD NEW ACTORS/GOODIES IF NECESSARY
     */

    // FUNGUS GENERATION
    if (randInt(0,max(510 - getLevel() * 10, 200)) == 0)
    {
        double x, y;
        randPoint(x,y,VIEW_RADIUS,true);
        m_actors.push_back(new Fungus(x,y,this));
    }
    
    // GOODIE GENERATION
    if (randInt(0,max(510 - getLevel() * 10, 250)) == 0)
    {
        double x, y;
        randPoint(x,y,VIEW_RADIUS,true);
        int type = randInt(1,10);
        switch (type)
        {
            case 1:
                m_actors.push_back(new ExtraLife(x,y,this));
                break;
            case 2:
            case 3:
            case 4:
                m_actors.push_back(new Flamethrower(x,y,this));
                break;
            default:
                m_actors.push_back(new HealthRestore(x,y,this));
                break;
        }
    }
     
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

Socrates* StudentWorld::player() const
{
    return m_player;
}

bool StudentWorld::checkOverlap(double x, double y, int num, bool block) const
{
    if (num >= m_actors.size())
        return false;
    if (num == -1)
        num = static_cast<int>(m_actors.size());
    for (int i = 0; i != num; i++)
    {
        if (block)
        {
            if (m_actors[i]->canBlock() && distance(x, m_actors[i]->getX(), y, m_actors[i]->getY()) <= SPRITE_WIDTH/2)
                return true;
        }
        else if (distance(x, m_actors[i]->getX(), y, m_actors[i]->getY()) <= SPRITE_WIDTH)
            return true;
    }
    return false;
}

bool StudentWorld::checkOverlap(Actor* original, int damage, bool player, bool food)
{
    if (player)
    {
        if (distance(original->getX(), m_player->getX(), original->getY(), m_player->getY()) <= SPRITE_WIDTH)
        {
            if (damage > 0)
            {
                m_player->removeHealth(damage);
            }
            return true;
        }
        else
            return false;
    }
    
    if (food)
    {
        for (int i = 0; i != m_actors.size(); i++)
        {
            if (distance(original->getX(), m_actors[i]->getX(), original->getY(), m_actors[i]->getY()) <= SPRITE_WIDTH && m_actors[i]->isEdible())
            {
                m_actors[i]->kill();
                return true;
            }
        }
        return false;
    }
    
    for (int i = 0; i != m_actors.size(); i++)
    {
        if (distance(original->getX(), m_actors[i]->getX(), original->getY(), m_actors[i]->getY()) <= SPRITE_WIDTH && original != m_actors[i])
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

void StudentWorld::addProjectile(int type, double x, double y, Direction dir)
{
    if (type == 0)
        m_actors.push_back(new Spray(x, y, dir, this));
    else
        m_actors.push_back(new Flame(x, y, dir, this));
}

void StudentWorld::addBacterium(int type, double x, double y)
{
    switch (type)
    {
        case 1:
            m_actors.push_back(new RegularSalmonella(x, y, this));
            break;
        case 2:
            m_actors.push_back(new AggressiveSalmonella(x, y, this));
            break;
        case 3:
            m_actors.push_back(new Ecoli(x, y, this));
            break;
    }
}

bool StudentWorld::findFood(double x, double y, Direction& dir) const
{
    for (int i = 0; i != m_actors.size(); i++)
    {
        if (m_actors[i]->isEdible() && distance(x, m_actors[i]->getX(), y, m_actors[i]->getY()) <= VIEW_RADIUS)
        {
            // COMPUTE DIRECTION TO MOVE IN
            /*
            dir = atan2(m_actors[i]->getY(), m_actors[i]->getX()) * 180 / (atan(1) * 4);
             */
            
            return true;
        }
    }
    return false;
}

bool StudentWorld::findSocrates(double x, double y, Direction& dir, double dist) const
{
    
    return false;
}

/*
 PRIVATE MEMBER TEMPLATES AND FUNCTIONS
 */

template<typename T>
void StudentWorld::generateActors(int num, int& existing, bool increment)
{
    double x, y;
    while (num > 0)
    {
        for (;;)
        {
            randPoint(x,y,120);
            if (!checkOverlap(x,y,existing))
                break;
        }
        m_actors.push_back(new T(x, y, this));
        if (increment)
            existing++;
        num--;
    }
}

void StudentWorld::randPoint(double &x, double &y, double maxLength, bool only) const
{
    double a = randInt(0,360) * RAD_CONV;
    double r;
    if (!only)
        r = randInt(0,maxLength);
    else
        r = maxLength;
    x = VIEW_RADIUS + r * cos(a);
    y = VIEW_RADIUS + r * sin(a);
}

double StudentWorld::distance(double x1, double x2, double y1, double y2) const
{
    return sqrt(pow(x2-x1,2)+(pow(y2-y1,2)));
}
