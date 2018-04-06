//
// Created by Mike on 15.03.18.
//

#ifndef MYGAME_UNITS_H
#define MYGAME_UNITS_H

#include "Race.h"
#include "Map.h"
#include "Buildings.h"
using std::cin;
using std::min;
using std::max;
class Writer {
public:
    static void wChoise(int movepoints) {
        cout << "Movepoints: " << movepoints << endl << endl;
        cout << "Listen to your orders!" << endl;
        cout << "1)Attack" << endl;
        cout << "2)Move" << endl;
        cout << "3)Nothing" << endl;
    }
    static void wTakeCoordinates() {
        cout << "Say the coordinares, my Lord!" << endl;
    }
    static void wWrongInfo() {
        cout << "Wrong Information!" << endl << endl;
    }

    static void wDeath(string& UnitName) {
        cout << UnitName << " has dead." << endl;
    }
    static void wDamage(string& UnitName, int damage) {
        cout << UnitName << " " << "lost " << damage << " hp." << endl;
    }
    static void wHealing(string& UnitName) {
        cout << UnitName << " is healing." << endl;
    }
    static void wMoveInfoWarriorArcher() {
        cout << "Say the string of moving includes 'u','d','r','l': ";
    }
    static void wMistake() {
        cout << "Sorry, it was a mistake!" << endl;
    }
    static void wMoveInfoSpy() {
        cout << "Say the strings of moving includes 'u','d','r','l' with '!' after the last: ";
    }
    static void wLine() {
        cout << endl << endl;
    }
    
};
class CUnit {
public:
    CUnit() = default;

    CUnit(IRace* r, Map* map) {}
    virtual void Move(int &mp)=0;
    virtual void Damage(int &move, int x, int y)=0;
    void TakeDamage(int damage) {
        damage -=  defence + _race->getBonusDefence() + defenceWeaponLevel*5;
        if (damage <= 0)
            damage = 0;
        if (damage > health)
            damage = health;
        if (damage) health -= damage;
        Writer::wDamage(UnitName, damage);
        if (health <= 0)
            Death();
    }

    void getInfo() const{
        cout << UnitName << endl;
        cout << "Health: " << health << endl;
        cout << "Attack: " << attack + _race->getBonusAttack() + getAWL()*5;
        (attackDistance > 1) ? cout << " Attack Distance: " << attackDistance << endl:cout << endl;
        cout << "Defence: " << defence + _race->getBonusDefence() + getDWL()* powerWeaponLevel << endl;
    }
    string getName() const { return UnitName; }
    void takeOrders() {
        int movepoints = move + _race->getBonusMove();
        while (movepoints) {
            getInfo();
            Writer::wChoise(movepoints);
            char choice = '3';
            cin >> choice;
            if (choice == '1') {
                Writer::wTakeCoordinates();
                int cx, cy;
                cin >> cx >> cy;
                if ((*mask)[cx - 1][cy - 1])
                   Damage(movepoints, cx, cy);
                else {
                    Writer::wWrongInfo();
                    break;
                }
            }
            if (choice == '2')
                Move(movepoints);
            if (choice == '3') {
                int tx = -1;
                int ty = -1;
                findChurche(x - 2, y - 1);
                findChurche(x - 1, y - 2);
                findChurche(x - 1, y);
                findChurche(x, y - 1);
               break;
            }
            if (choice == '#'){
                Death();
                break;
            }
        }
    }

    void setPlace(int x, int y) {
        this->x = x;
        this->y = y;
        if (!isEnemy)
          map->map[x-1][y-1] = symbol;
        map->secret[x-1][y-1] = symbol;
        if (!isEnemy)
          look();
    }
    char getSymbol() const{ return symbol;}
    int getX() const {return x; }
    int getY() const {return y; }
    void healing(){
        if (health != maxhealth)
            Writer::wHealing(UnitName);
        health = min(maxhealth, health + _race->getBonusHealth());
    }
    int getHealth() const {return health;}
    int getMaxHealth() const {return maxhealth;}
    void setArmy(vector<CUnit*> *a) {
        army = a;
    }
    void setMask(vector<vector<CUnit*>> *m) {
        mask = m;
    }
    void setBMask(vector<vector<IBuilding*>> *bm) {
        b_mask = bm;
    }
    int getAWL() const {return attackWeaponLevel;}
    int getDWL() const {return defenceWeaponLevel;}
    ~CUnit() {}

protected:
    IRace* _race;
    vector<CUnit*> *army;
    vector<vector<CUnit*>> *mask;
    vector<vector<IBuilding*>> *b_mask;
    string UnitName;
    int move;
    int defence;
    int attack;
    int attackDistance = 1;
    int lookDistance;
    int health;
    int maxhealth;
    int attackWeaponLevel = 0;
    int defenceWeaponLevel = 0;
    int x;
    int y;
    char symbol;
    Map* map;
    bool isEnemy;
    int id;
    int powerWeaponLevel = 5;
    void Death() {
        Writer::wDeath(UnitName);
        (*army)[id] = NULL;
        (*mask)[x - 1][y - 1] = NULL;
        map->map[x - 1][y - 1] = '.';
        map->secret[x - 1][y - 1] = '*';
        delete this;
    };
    void churchEffect(int tx, int ty) {
        if ((*b_mask)[tx][ty]->getPurpose() == "Attack") {
            Builder *b = new Builder(this);
            b->buildAttack((*b_mask)[tx][ty]->getPower());
            delete b;
        }
        if ((*b_mask)[tx][ty]->getPurpose() == "Defence") {
            Builder *b = new Builder(this);
            b->buildDefence((*b_mask)[tx][ty]->getPower());
            delete b;
        }
    }
    void findChurche(int x, int y) {
        if (y < map->getSize() && y >= 0 && x < map->getSize() && x >=0 && map->map[x][y] == '+') {
            churchEffect(x, y);
        }
    }
    void look(){
        int d = lookDistance + _race->getBonusLookDistance();
        int i0 = max(1, x - d);
        int j0 = max(1, y - d);
        int il = min(map->getSize(), x + d);
        int jl = min(map->getSize(), y + d);
        for (int i = i0; i <= il; i++)
            for (int j = j0; j <= jl; j++)
                if (map->map[i - 1][j - 1] == '*')
                    map->map[i - 1][j - 1] = map->secret[i - 1][j - 1] == '*' ? '.' : map->secret[i - 1][j - 1];
    }
    void unlook() {
        int d = lookDistance + _race->getBonusLookDistance();
        int i0 = max(1, x - d);
        int j0 = max(1, y - d);
        int il = min(map->getSize(), x + d);
        int jl = min(map->getSize(), y + d);
        for (int i = i0; i <= il; i++)
            for (int j = j0; j <= jl; j++)
                if (map->map[i - 1][j - 1] == '.' || !(map->map[i - 1][j - 1] >= 'A' && map->map[i - 1][j - 1] <='Z'))
                    map->map[i - 1][j - 1] = '*';
        for (int i = 0;i < (*army).size();i++)
            if ((*army)[i] != this && (*army)[i])
                (*army)[i]->look();
    }

private:
    class Builder{
    public:
        explicit Builder(CUnit* unit) {
            this->unit = unit;
        }
        void buildAttack(int x) {unit->attackWeaponLevel = x;}
        void buildDefence(int x) {unit->defenceWeaponLevel = x;}
    private:
        CUnit* unit;
    };
};

class Warrior : public CUnit {
public:
    Warrior() {}
    Warrior(IRace* race, Map* m, bool ie) {
        id = 2;
        _race = race;
        map = m;
        UnitName = _race->getNameWarrior();
        move = 6;
        defence = 20;
        attack = 250;
        lookDistance = 4;
        health = 600;
        maxhealth = 600;
        symbol = 'w';
        isEnemy = ie;
        if (!ie)
            symbol += 'A' - 'a';
    }
    void Move(int &mp) override {
        Writer::wMoveInfoWarriorArcher();
        string s;
        cin >> s;
        if (s.length() > mp)
            s = s.substr(0, mp);
        unlook();
        map->map[x-1][y-1] = '*';
        map->secret[x-1][y-1] = '*';
        (*mask)[x-1][y-1] = NULL;
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == 'r' && y < map->getSize() && map->secret[x - 1][y] == '*')
                y++;
            if (s[i] == 'l' && y > 1 && map->secret[x - 1][y - 2] == '*')
                y--;
            if (s[i] == 'u' && x > 1 && map->secret[x - 2][y - 1] == '*')
                x--;
            if (s[i] == 'd' && x < map->getSize() && map->secret[x][y - 1] == '*')
                x++;
        }
        map->map[x-1][y-1] = symbol;
        map->secret[x-1][y-1] = symbol;
        (*mask)[x-1][y-1] = this;
        look();
        map->showMap();
        mp = 0;
    }

    void Damage(int &move, int cx, int cy) override {
        Move(move);
        if (abs(x - cx) + abs(y - cy) == 1) {
            (*mask)[cx - 1][cy - 1]->TakeDamage(attack + _race->getBonusAttack() + attackWeaponLevel * powerWeaponLevel);
        }
        else {
            Writer::wMistake();
            TakeDamage(45);
        }
        map->showMap();
    }

};

class Archer : public CUnit {
public:
    Archer() {}
    Archer(IRace* race, Map* m, bool ie) {
        _race = race;
        map = m;
        UnitName = _race->getNameArcher();
        move = 6;
        defence = 10;
        attack = 120;
        lookDistance = 5;
        health = 400;
        symbol = 'a';
        isEnemy = ie;
        maxhealth = 400;
        attackDistance = 5;
        id = 1;
        if (!ie)
            symbol += 'A' - 'a';
    }
    void Move(int &mp) override {
        Writer::wMoveInfoWarriorArcher();
        string s;
        cin >> s;
        if (s.length() > mp)
            s = s.substr(0, mp);
        unlook();
        map->map[x-1][y-1] = '*';
        map->secret[x-1][y-1] = '*';
        (*mask)[x-1][y-1] = NULL;
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == 'r' && y < map->getSize() && map->secret[x - 1][y] == '*')
                y++;
            if (s[i] == 'l' && y > 1 && map->secret[x - 1][y - 2] == '*')
                y--;
            if (s[i] == 'u' && x > 1 && map->secret[x - 2][y - 1] == '*')
                x--;
            if (s[i] == 'd' && x < map->getSize() && map->secret[x][y - 1] == '*')
                x++;
        }
        map->map[x-1][y-1] = symbol;
        map->secret[x-1][y-1] = symbol;
        (*mask)[x-1][y-1] = this;
        look();
        map->showMap();
        mp = 0;
    }

    void Damage(int &move, int cx, int cy) override {
        if (move < 3) {
            Writer::wMistake();
            return;
        }

        if (abs(x - cx) + abs(y - cy) <= attackDistance + _race->getBonusArcherDistance()) {
            (*mask)[cx - 1][cy - 1]->TakeDamage(attack + _race->getBonusAttack() + attackWeaponLevel * 5);
        }
        else {
            Writer::wMistake();
            TakeDamage(45);
        }
        move = max(0, move - 3);
        map->showMap();
    }


};

class Spy : public CUnit {
public:
    Spy() {}
    Spy(IRace* race, Map* m, bool ie) {
        _race = race;
        map = m;
        UnitName = _race->getNameSpy();
        move = 8;
        defence = 20;
        attack = 1090;
        lookDistance = 7;
        health = 300;
        maxhealth = 300;
        symbol = 's';
        isEnemy = ie;
        id = 0;
        if (!ie)
            symbol += 'A' - 'a';
    }
    void Move(int &mp) override {
        Writer::wMoveInfoSpy();
        int d = mp;
        map->map[x-1][y-1] = '*';
        char t;
        string s="";
        int j = 0;
        for (int i = 0; i < d; i++) {
            map->map[x-1][y-1] = '*';
            map->secret[x-1][y-1] = '*';
            (*mask)[x-1][y-1] = NULL;
            unlook();
            mp--;
            if (j == s.length()) {
                j = 0;
                cin >> s;
            }
            if (s[j] == 'r' && y < map->getSize() && map->secret[x - 1][y] == '*')
                y++;
            if (s[j] == 'l' && y > 1 && map->secret[x - 1][y - 2] == '*')
                y--;
            if (s[j] == 'u' && x > 1 && map->secret[x - 2][y - 1] == '*')
                x--;
            if (s[j] == 'd' && x < map->getSize() && map->secret[x][y - 1] == '*')
                x++;
            map->map[x - 1][y - 1] = symbol;
            map->secret[x-1][y-1] = symbol;
            (*mask)[x-1][y-1] = this;
            look();
            if (s[j] == '!') {
                mp++;
                map->showMap();
                return;
            }
            j++;
            if (j == s.length() || i == d - 1) {
                Writer::wLine();
                map->showMap();
            }
        }
    }

    void Damage(int &move, int cx, int cy) override {
        move -= 2;
        if (move > 0) {
            Move(move);
        }
        if (abs(x - cx) + abs(y - cy) == 1) {
            (*mask)[cx - 1][cy - 1]->TakeDamage(attack + _race->getBonusAttack() + attackWeaponLevel * 5);
        }
        else
            TakeDamage(45);
        map->showMap();
    }


};

class Factory {
public:
    static CUnit* create(string type, IRace* race, Map* m, bool ie) {
        if (type == "Spy")
            return new Spy(race, m, ie);
        if (type == "Archer")
            return new Archer(race, m, ie);
        if (type == "Warrior")
            return new Warrior(race, m, ie);
    }
};



#endif //MYGAME_UNITS_H
