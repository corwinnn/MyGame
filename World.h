//
// Created by Mike on 15.03.18.
//

#ifndef MYGAME_WORLD_H
#define MYGAME_WORLD_H

#include "Units.h"

class GameMap {
private:
    static Map* m_instance;
public:
    GameMap() = delete;
    ~GameMap() = delete;
    GameMap(const GameMap&) = delete;
    GameMap& operator=(const GameMap&) = delete;
    static Map* instance() {
        if (m_instance == nullptr)
            m_instance = new Map();
        return m_instance;
    }
};
Map* GameMap::m_instance = nullptr;

class World {
public:

    World(){
        m = GameMap::instance();
        b_mask.resize(m->getSize());
        for (int i = 0;i < b_mask.size();i++)
            for (int j = 0; j < b_mask.size();j++)
                b_mask[i].push_back(NULL);
        b_mask[1][1] = new ChurchA(2, 2,m,2);
        b_mask[2][2] = new ChurchD(3, 3, m, 3);
        myRace = new Humans();
        enemyRace = new Dwarfes();
        mask.resize(m->getSize());
        for (int i = 0;i < mask.size();i++)
            for (int j = 0; j < mask.size();j++)
                mask[i].push_back(NULL);
        enemyArmy.resize(3);
        enemyArmy[0] = Factory::create("Spy", enemyRace, m, true);
        enemyArmy[1] = Factory::create("Archer", enemyRace, m, true);
        enemyArmy[2] = Factory::create("Warrior", enemyRace, m, true);
        enemyArmy[0]->setArmy(&enemyArmy);
        enemyArmy[1]->setArmy(&enemyArmy);
        enemyArmy[2]->setArmy(&enemyArmy);
        enemyArmy[0]->setPlace(15, 13);
        enemyArmy[1]->setPlace(25, 4);
        enemyArmy[2]->setPlace(5, 25);
        mask[14][12] = enemyArmy[0];
        mask[24][3] = enemyArmy[1];
        mask[4][24] = enemyArmy[2];

        myArmy.resize(3);
        myArmy[0] = Factory::create("Spy", myRace, m, false);
        myArmy[1] = Factory::create("Archer", myRace, m, false);
        myArmy[2] = Factory::create("Warrior", myRace, m, false);
        myArmy[0]->setArmy(&myArmy);
        myArmy[1]->setArmy(&myArmy);
        myArmy[2]->setArmy(&myArmy);
        myArmy[0]->setPlace(1, 3);
        myArmy[1]->setPlace(1, 2);
        myArmy[2]->setPlace(1, 1);
        mask[0][2] = myArmy[0];
        mask[0][1] = myArmy[1];
        mask[0][0] = myArmy[2];

        enemyArmy[0]->setMask(&mask);
        enemyArmy[1]->setMask(&mask);
        enemyArmy[2]->setMask(&mask);
        enemyArmy[0]->setBMask(&b_mask);
        enemyArmy[1]->setBMask(&b_mask);
        enemyArmy[2]->setBMask(&b_mask);
        myArmy[0]->setMask(&mask);
        myArmy[1]->setMask(&mask);
        myArmy[2]->setMask(&mask);
        myArmy[0]->setBMask(&b_mask);
        myArmy[1]->setBMask(&b_mask);
        myArmy[2]->setBMask(&b_mask);

    }

    IUnit* getMyUnit(int n){
        return myArmy[n];
    }
    void healing() const {
        for (auto i : myArmy) {
            if (i)
                i->healing();
        }
        for (auto i : myArmy) {
            if (i)
                i->healing();
        }
    }

    bool war() {
        bool win = true;
        bool lose = true;
        static int step = 0;
        m->showMap();
        step++;
        cout << endl << "STEP " << " " << step << endl;
        if (myArmy[0]) {
            myArmy[0]->takeOrders();
            lose = false;
        }
        if (myArmy[1]) {
            myArmy[1]->takeOrders();
            lose = false;
        }
        if (myArmy[2]) {
            myArmy[2]->takeOrders();
            lose = false;
        }

        healing();

        if (enemyArmy[0]) {
            cout << "Enemy:" << endl;
            enemyArmy[0]->getInfo();
            cout << endl;
            win = false;
        }
        if (enemyArmy[1]) {
            cout << "Enemy:" << endl;
            enemyArmy[1]->getInfo();
            cout << endl;
            win = false;
        }
        if (enemyArmy[2]) {
            cout << "Enemy:" << endl;
            enemyArmy[2]->getInfo();
            cout << endl;
            win = false;
        }
        if (win) {cout << "YOU WIN!" << endl;return true;}
        if (lose) {cout << "YOU LOSE!" << endl;return true;}
        //if (step > 1) return true;
        return false;
    }

    ~World() {
        delete m;
        delete myRace;
        delete enemyRace;
        for (auto &i : enemyArmy)
            delete i;
        for (auto &i : myArmy)
            delete i;
    }

private:
    Map *m;
    IRace* myRace;
    IRace* enemyRace;
    vector<vector<IUnit*>> mask;
    vector<vector<IBuilding*>> b_mask;
    vector<IUnit*> myArmy;
    vector<IUnit*> enemyArmy;

};

void check(vector<vector<IUnit*>> &mask) {
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10;j++)
            if (mask[i][j])
                cout << i << " " << j << " " <<  mask[i][j]->getName() << endl;
}

void makeWorld() {
    World *world = new World();
    bool end = false;
    while(!end) {
        end = world->war();
    }
    delete world;

}



#endif //MYGAME_WORLD_H
