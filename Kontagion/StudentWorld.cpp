#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

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
    m_numBact = getLevel() * 10;
    
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
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        
        if (m_numBact == 0)
        {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
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
    
    // PRINT TOP TEXT
    ostringstream oss;
    oss.fill('0');
    if (getScore() < 0)
        oss << "Score: -" << setw(5) << -1 * getScore() << "  ";
    else
        oss << "Score: " << setw(6) << getScore() << "  ";
    oss << "Level: " << getLevel() << "  ";
    oss << "Lives: " << getLives() << "  ";
    oss << "Health: " << m_player->getHealth() << "  ";
    oss << "Sprays: " << m_player->getSprays() << "  ";
    oss << "Flames: " << m_player->getFlames();
    setGameStatText(oss.str());
    
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
    m_player = nullptr;
}

Socrates* StudentWorld::player() const
{
    return m_player;
}

bool StudentWorld::checkOverlap(double x, double y, int num, bool block) const
{
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
            if (distance(original->getX(), m_actors[i]->getX(), original->getY(), m_actors[i]->getY()) <= SPRITE_WIDTH && m_actors[i]->isEdible() && m_actors[i]->isAlive())
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
                if (m_actors[i]->isAlive() && m_actors[i]->isDamagable())
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
        case 0:
            m_actors.push_back(new RegularSalmonella(x, y, this));
            break;
        case 1:
            m_actors.push_back(new AggressiveSalmonella(x, y, this));
            break;
        case 2:
            m_actors.push_back(new Ecoli(x, y, this));
            break;
    }
}

void StudentWorld::addFood(double x, double y)
{
    m_actors.push_back(new Food(x,y,this));
}

bool StudentWorld::findFood(double x, double y, Direction& dir) const
{
    double dist = 0;
    bool found = false;
    for (int i = 0; i != m_actors.size(); i++)
    {
        if (m_actors[i]->isEdible() && m_actors[i]->isAlive() && distance(x, m_actors[i]->getX(), y, m_actors[i]->getY()) <= VIEW_RADIUS)
        {
            if (dist == 0)
                dist = distance(x, m_actors[i]->getX(), y, m_actors[i]->getY());
            else
            {
                double dist2 = distance(x, m_actors[i]->getX(), y, m_actors[i]->getY());
                if (dist2 < dist)
                    dist = dist2;
            }
            dir = atan2(m_actors[i]->getY() - y,m_actors[i]->getX() - x) * (180 / (atan(1) * 4));
            found = true;
        }
    }
    if (found)
        return true;
    else
        return false;
}

bool StudentWorld::findSocrates(double x, double y, Direction& dir, double dist) const
{
    if (distance(x, m_player->getX(), y, m_player->getY()) <= dist)
    {
        dir = atan2(m_player->getY() - y,m_player->getX() - x) * (180 / (atan(1) * 4));
        return true;
    }
    return false;
}

void StudentWorld::decreaseBact()
{
    m_numBact--;
}

void StudentWorld::increaseBact()
{
    m_numBact++;
}

double StudentWorld::distance(double x1, double x2, double y1, double y2) const
{
    return sqrt(pow(x2-x1,2)+(pow(y2-y1,2)));
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
