//
// Created by Mike on 15.03.18.
//

#ifndef MYGAME_WORLD_H
#define MYGAME_WORLD_H

#include "Units.h"

class World {
public:
    Map *m;
    IRace* myRace;
    IRace* enemyRace;
    vector<vector<IUnit*>> mask;
    vector<IUnit*> myArmy;
    vector<IUnit*> enemyArmy;

    World(){
        m = new Map();
        myRace = new Humans();
        enemyRace = new Dwarfes();
        mask.resize(m->getSize());
        for (int i = 0;i < mask.size();i++)
            for (int j = 0; j < mask.size();j++)
                mask[i].push_back(NULL);
        enemyArmy.resize(3);
        enemyArmy[0] = Factory::create("Spy", enemyRace, m, 1);
        enemyArmy[1] = Factory::create("Archer", enemyRace, m, 1);
        enemyArmy[2] = Factory::create("Warrior", enemyRace, m, 1);
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
        myArmy[0] = Factory::create("Spy", myRace, m, 0);
        myArmy[1] = Factory::create("Archer", myRace, m, 0);
        myArmy[2] = Factory::create("Warrior", myRace, m, 0);
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
        myArmy[0]->setMask(&mask);
        myArmy[1]->setMask(&mask);
        myArmy[2]->setMask(&mask);

    }

    void healing() {
        for (int i = 0; i < myArmy.size(); i++) {
            if (myArmy[i])
                myArmy[i]->healing();
        }
        for (int i = 0; i < myArmy.size(); i++) {
            if (enemyArmy[i])
                enemyArmy[i]->healing();
        }
    }

    void war() {
        bool win = false;
        bool lose = false;
        int step = 0;
        while(!win && !lose) {
            m->showMap();
            step++;
            cout << endl << "STEP " << " " << step << endl;
            win = true;
            lose = true;
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
        }
        if (win) cout << "YOU WIN!" << endl;
        if (lose) cout << "YOU LOSE!" << endl;
    }

};

void check(vector<vector<IUnit*>> &mask) {
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10;j++)
            if (mask[i][j])
                cout << i << " " << j << " " <<  mask[i][j]->getName() << endl;
}

void makeWorld() {
    World *world = new World();
    world->war();

}



#endif //MYGAME_WORLD_H
