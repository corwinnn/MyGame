//
// Created by Mike on 15.03.18.
//

#ifndef MYGAME_WORLD_H
#define MYGAME_WORLD_H

#include "Units.h"
/**
 * \brief Singleton class. Makes gamemap.
 */

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
/**
 * \brief Makes the gameworld. Initializes maps with objects.
 */
class UnitsOrder{
public:
    Army* army;
    UnitsOrder(Army* a) {
        army = a;
    }
    string lineWarriors() {
        string ans = "";
        for (int i = 0; i < army->army.size(); i++) {
            string c = "";
            for (int j = 0; j < army->army[i]->getASize();j++)
                c += 'W';
            ans += c + ' ';
        }
        return ans;
    }
};

class World {
public:
    /**
     * Initializing
     */
    World(){
        types = new string [count_units];
        types[0] = "Spy";
        types[1] = "Archer";
        types[2] = "Warrior";
        types[3] = "Hero";
        myRace = new Humans();
        enemyRace = new Dwarfes();
        m = GameMap::instance();
        b_mask.resize(m->getSize());
        mask.resize(m->getSize());
        for (size_t i = 0; i < b_mask.size();i++)
            for (size_t j = 0; j < b_mask.size();j++)
                b_mask[i].push_back(NULL);
        for (size_t i = 0;i < mask.size();i++)
            for (size_t j = 0; j < mask.size();j++)
                mask[i].push_back(NULL);
        AddChurchA(1, 1, 2);
        AddChurchB(5, 5, 3);

        Army* triple = new Army();
        for (int i = 0; i < 3; i++) {
            triple->addUnit(Factory::create(types[i], enemyRace, m, true));
        }
        triple->army[0]->setPlace(5, 3);
        triple->army[1]->setPlace(25, 4);
        triple->army[2]->setPlace(5, 25);
        mask[4][2] = triple->army[0];
        mask[24][3] = triple->army[1];
        mask[4][24] = triple->army[2];
        enemy_army = new Army();
        enemy_army->addUnit(triple);
        enemy_army->addUnit(Factory::create(types[3], enemyRace, m, true));
        enemy_army->army[1]->setPlace(7, 7);
        mask[6][6] = enemy_army->army[1];

        Army* triple1 = new Army();
        for (int i = 0; i < 3; i++) {
            triple1->addUnit(Factory::create(types[i], myRace, m, false));
        }
        triple1->army[0]->setPlace(1, 3);
        triple1->army[1]->setPlace(1, 2);
        triple1->army[2]->setPlace(1, 1);
        mask[0][2] = triple1->army[0];
        mask[0][1] = triple1->army[1];
        mask[0][0] = triple1->army[2];
        my_army = new Army();
        my_army->addUnit(triple1);
        my_army->addUnit(Factory::create(types[3], myRace, m, false));
        my_army->army[1]->setPlace(1, 4);
        mask[0][3] = my_army->army[1];

        int x = 0;
        my_army->setIds(x);
        x = 0;
        enemy_army->setIds(x);
        count_units = 4;

        for (int i = 0; i < count_units; i++) {
            enemy_army->get(i)->setMask(&mask);
            enemy_army->get(i)->setBMask(&b_mask);
            my_army->get(i)->setMask(&mask);
            my_army->get(i)->setBMask(&b_mask);
        }

        myArmy.resize(0);
        enemyArmy.resize(0);
        for (int i = 0; i  < count_units; i++) {
            myArmy.push_back(my_army->get(i));
            myArmy[i]->setArmy(&myArmy);
            enemyArmy.push_back(enemy_army->get(i));
            enemyArmy[i]->setArmy(&enemyArmy);
        }
        int ww = 0;
        isf = new UnitsOrder(my_army);


    }

    void AddChurchA(int x, int y, int p) {
        b_mask[x][y] = new ChurchA(x + 1, y + 1, m, p);
    }
    void AddChurchB(int x, int y, int p) {
        b_mask[x][y] = new ChurchD(x + 1, y + 1, m, p);
    }

    void setArmy(vector<CUnit*> &army, int size, bool enemy) {
        army.resize(size);
        for (int i = 0; i < size; i++) {
            army[i] = Factory::create(types[i], enemyRace, m, enemy);
            army[i]->setId(i);
        }
        for (int i = 0; i < size; i++) {
            army[i]->setArmy(&army);
        }

    }

    /**
     *
     * @param n unit's number
     * @return unit
     */
    CUnit* getMyUnit (int n) const{
        return myArmy[n];
    }
    /**
     * Heal alive units
     */
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
    /**
     * One step of the game. Every alive unit takes orders. If there nobody alive, game ends.
     * @return is the game ended
     */
    bool war() {
        bool win = true;
        bool lose = true;
        static int step = 0;
        m->showMap();
        step++;
        cout << endl << "STEP " << " " << step << endl;
        for (int i = 0 ; i < count_units; i++) {
            if (my_army->get(i)) {
                my_army->get(i)->takeOrders();
                lose = false;
            }
        }

        healing();
        for (int i = 0 ; i < count_units; i++) {
            if (enemy_army->get(i)) {
                cout << "Enemy:" << endl;
                enemy_army->get(i)->getInfo();
                cout << endl;
                win = false;
            }
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
        for (size_t i = 0; i < mask.size();i++)
            for (size_t j = 0; j < mask.size();j++) {
                if (b_mask[i][j])
                   delete b_mask[i][j];
            }
    }

    UnitsOrder* isf;

private:

    Map *m;
    IRace* myRace;
    IRace* enemyRace;
    vector<vector<CUnit*>> mask;
    vector<vector<IBuilding*>> b_mask;
    Army* my_army;
    Army* enemy_army;
    vector<CUnit*> myArmy, enemyArmy;
    string* types;
    int count_units = 4;

};
/**
 * function for testing
 * @param mask
 */
void check(vector<vector<CUnit*>> &mask) {
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10;j++)
            if (mask[i][j])
                cout << i << " " << j << " " <<  mask[i][j]->getName() << endl;
}

/**
 * Run steps
 */
void makeWorld() {
    World *world = new World();
    cout << world->isf->lineWarriors();
    bool end = false;
    while(!end) {
        end = world->war();


    }
    delete world;
}



#endif //MYGAME_WORLD_H
