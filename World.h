//
// Created by Mike on 15.03.18.
//

#ifndef MYGAME_WORLD_H
#define MYGAME_WORLD_H
#include "Units.h"
class Period {
public:
    string name;
    Period* next;
    Period(string name):
            name(name)
    {}

};
/**
 * Interface of command
 */
class Command{
protected:
    CUnit* unit;
public:
    virtual bool Execute(int& m) = 0;
    Command(CUnit* _unit) {
        unit = _unit;
    }
};
/**
 * command 'Atack'
 */
class Attack : public Command {
public:
    Attack(CUnit* _unit) : Command(_unit){};
    bool Execute(int& movepoints){
        Writer::wTakeCoordinates();
        int cx, cy;
        cin >> cx >> cy;
        if ((*unit->mask)[cx - 1][cy - 1])
            unit->Damage(movepoints, cx, cy);
        else {
            Writer::wWrongInfo();
            return false;
        }
        return true;
    }
};
/**
 * command 'Move'
 */
class Move : public Command {
public:
    Move(CUnit* _unit) : Command(_unit) {};

    bool Execute(int &movepoints) {
        unit->Move(movepoints);
        return true;
    }
};
/**
 * command to do nothing
 */
class Relax : public Command {
public:
    Relax(CUnit* _unit) : Command(_unit) {};

    bool Execute(int &movepoints) {
        unit->findChurche(unit->x - 2, unit->y - 1);
        unit->findChurche(unit->x - 1, unit->y - 2);
        unit->findChurche(unit->x - 1, unit->y);
        unit->findChurche(unit->x, unit->y - 1);
        return false;
    }
};

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
 * \brief Army Decorator. Show the order of army groups.
 */
class UnitsOrderDecorator{
public:
    Army* army;
    UnitsOrderDecorator(Army* a) {
        army = a;
    }
    /**
     *
     * @return string scheme of warriors
     */
    string lineWarriors() {
        string ans = "";
        for (unsigned int i = 0; i < army->army.size(); i++) {
            string c = "";
            for (int j = 0; j < army->army[i]->getASize();j++)
                c += 'W';
            ans += c + ' ';
        }
        return ans;
    }
};

/**
 * \brief Makes the gameworld. Initializes maps with objects.
 */
class World {
public:
    /**
     * Initializing
     */
    World(){
        Periods[0] = new Period("Summer");
        Periods[1] = new Period("Autumn");
        Periods[2] = new Period("Winter");
        Periods[3] = new Period("Spring");
        for (int i = 0; i < 4; i++)
            Periods[i]->next = Periods[(i + 1) % 4];
        curPeriod = Periods[0];

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
        isf = new UnitsOrderDecorator(my_army);


    }
    /**
     * Add an attack church
     * @param x - first coordinate
     * @param y - second coordinate
     * @param p - power of the church
     */
    void AddChurchA(int x, int y, int p) {
        b_mask[x][y] = new ChurchA(x + 1, y + 1, m, p);
    }
    /**
     * Add an defence church church
     * @param x - first coordinate
     * @param y - second coordinate
     * @param p - power of the church
     */
    void AddChurchB(int x, int y, int p) {
        b_mask[x][y] = new ChurchD(x + 1, y + 1, m, p);
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
        if (step % 4 == 0) {
            curPeriod = curPeriod->next;
            cout << "New Period:" << curPeriod->name << endl;
        }
        step++;
        cout << endl << "STEP " << " " << step << endl;
        for (int i = 0 ; i < count_units; i++) {
            if (my_army->get(i)) {
                CUnit* unit = my_army->get(i);
                int movepoints = unit->move + unit->_race->getBonusMove();
                while (movepoints) {
                    unit->getInfo();
                    Writer::wChoise(movepoints);
                    char choice = '3';
                    cin >> choice;
                    if (choice == '1') {
                        Command* command = new Attack(unit);
                        if (!command->Execute(movepoints))
                            break;
                        delete command;
                    }
                    if (choice == '2') {
                        Command *command = new Move(unit);
                        if (!command->Execute(movepoints))
                            break;
                        delete command;
                    }
                    if (choice == '3') {
                        Command *command = new Relax(unit);
                        if (!command->Execute(movepoints))
                            break;
                        delete command;
                    }
                    if (choice == '#') {
                        unit->Death();
                        break;
                    }
                }
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

    UnitsOrderDecorator* isf;

private:
    Period* Periods[4], *curPeriod;
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
