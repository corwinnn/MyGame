//
// Created by Mike on 18.03.18.
//

#ifndef MYGAME_BUILDINGS_H
#define MYGAME_BUILDINGS_H

#include "Map.h"
/**
 * \brief building's interface
 */
class IBuilding {
public:
    IBuilding(){}
    /**
     *
     * @return goal of building, defence or attack
     */
    string getPurpose() { return purpose;}
    /**
     *
     * @return purpose(bonus) that unit can take staying near the building
     */
    int getPower() { return power;}
protected:
    int high;
    int wall;
    string name;
    string purpose;
    int power;
};
/**
 * \brief buffs attack or defence
 */
class Church : public IBuilding {
public:
    Church() {}
    Church(int x, int y, Map* m, int p) {
        this->x = x;
        this->y = y;
        this->m = m;
        power = p;
        m->secret[x - 1][y - 1] = '+';
        high = 1;
        wall = 1;
        string name = "Church";
    }

protected:
    int x;
    int y;
    Map* m;
};
/**
 * \brief buffs attack
 */
class ChurchA : public Church {
public:
    ChurchA(int x, int y, Map* m, int p) : Church (x, y, m, p) {
        purpose = "Attack";
    }
};
/**
 * \brief buffs defence
 */
class ChurchD : public Church {
public:
    ChurchD(int x, int y, Map* m, int p) : Church (x, y, m, p) {
        purpose = "Defence";
    }
};
#endif //MYGAME_BUILDINGS_H
