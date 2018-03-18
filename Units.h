//
// Created by Mike on 15.03.18.
//

#ifndef MYGAME_UNITS_H
#define MYGAME_UNITS_H

#include "Race.h"
#include "Map.h"
#include "Buildings.h"

class IUnit {
public:
    IUnit() = default;

    IUnit(IRace* r, Map* map) {}
    virtual void Move(int &mp)=0;
    virtual void Damage(int &move, int x, int y)=0;
    void TakeDamage(int damage) {
        damage -=  defence + _race->getBonusDefence() + defenceWeaponLevel*5;
        if (damage <= 0)
            damage = 0;
        if (damage > health)
            damage = health;
        if (damage) health -= damage;
        cout << UnitName << " " << "lost " << damage << " hp." << endl;
        if (health <= 0)
            Death();
    }

    void getInfo() const{
        cout << UnitName << endl;
        cout << "Health: " << health << endl;
        cout << "Attack: " << attack + _race->getBonusAttack() + getAWL()*5;
        (attackDistance > 1) ? cout << " Attack Distance: " << attackDistance << endl:cout << endl;
        cout << "Defence: " << defence + _race->getBonusDefence() + getDWL()* 5 << endl;
    }
    string getName() const { return UnitName; }
    void takeOrders() {
        int movepoints = move + _race->getBonusMove();
        while (movepoints) {
            getInfo();
            cout << "Movepoints: " << movepoints << endl << endl;
            cout << "Listen to your orders!" << endl;
            cout << "1)Attack" << endl;
            cout << "2)Move" << endl;
            cout << "3)Nothing" << endl;
            char choice = '3';
            cin >> choice;
            if (choice == '1') {
                cout << "Say the coordinares, my Lord!" << endl;
                int cx, cy;
                cin >> cx >> cy;
                if ((*mask)[cx - 1][cy - 1])
                   Damage(movepoints, cx, cy);
                else {
                    cout << "Wrong Information!" << endl << endl;
                    break;
                }
            }
            if (choice == '2')
                Move(movepoints);
            if (choice == '3') {
                int tx = -1;
                int ty = -1;
                if (x > 1 && map->map[x - 2][y - 1] == '+') {
                    tx = x - 2;
                    ty = y - 1;
                }
                if (tx != -1) {
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
                    tx = -1;
                }
                if (y > 1 && map->map[x - 1][y - 2] == '+') {
                    tx = x - 1;
                    ty = y - 2;
                }
                if (tx != -1) {
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
                    tx = -1;
                }
                if (y < map->getSize() && map->map[x - 1][y] == '+') {
                    tx = x - 1;
                    ty = y;
                }
                if (tx != -1) {
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
                    tx = -1;
                }
                if (y < map->getSize() && map->map[x][y - 1] == '+') {
                    tx = x;
                    ty = y - 1;
                }
                if (tx != -1) {
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
                    tx = -1;
                }
               break;
            }
            if (choice == '4'){
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
            cout << UnitName << " is healing." << endl;
        health = min(maxhealth, health + _race->getBonusHealth());
    }
    int getHealth() {return health;}
    int getMaxHealth(){return maxhealth;}
    void setArmy(vector<IUnit*> *a) {
        army = a;
    }
    void setMask(vector<vector<IUnit*>> *m) {
        mask = m;
    }
    void setBMask(vector<vector<IBuilding*>> *bm) {
        b_mask = bm;
    }
    int getAWL() const {return attackWeaponLevel;}
    int getDWL() const {return defenceWeaponLevel;}
    ~IUnit() {}

protected:
    IRace* _race;
    vector<IUnit*> *army;
    vector<vector<IUnit*>> *mask;
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
    void Death() {
        cout << UnitName << " has dead." << endl;
        (*army)[id] = NULL;
        (*mask)[x - 1][y - 1] = NULL;
        map->map[x - 1][y - 1] = '.';
        map->secret[x - 1][y - 1] = '*';
        delete this;
    };
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
        explicit Builder(IUnit* unit) {
            this->unit = unit;
        }
        void buildAttack(int x) {unit->attackWeaponLevel = x;}
        void buildDefence(int x) {unit->defenceWeaponLevel = x;}
    private:
        IUnit* unit;
    };
};

class Warrior : public IUnit {
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
        cout << "Say the string of moving includes 'u','d','r','l': ";
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
            (*mask)[cx - 1][cy - 1]->TakeDamage(attack + _race->getBonusAttack() + attackWeaponLevel * 5);
        }
        else {
            cout << "Sorry, it was a mistake!" << endl;
            TakeDamage(45);
        }
        map->showMap();
    }

};

class Archer : public IUnit {
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
        cout << "Say the string of moving includes 'u','d','r','l': ";
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
            cout << "Sorry, it was a mistake!" << endl;
            return;
        }

        if (abs(x - cx) + abs(y - cy) <= attackDistance + _race->getBonusArcherDistance()) {
            (*mask)[cx - 1][cy - 1]->TakeDamage(attack + _race->getBonusAttack() + attackWeaponLevel * 5);
        }
        else {
            cout << "Sorry, it was a mistake!" << endl;
            TakeDamage(45);
        }
        move = max(0, move - 3);
        map->showMap();
    }


};

class Spy : public IUnit {
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
        cout << "Say the strings of moving includes 'u','d','r','l' with '!' after the last: ";
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
                cout << endl << endl;
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
    static IUnit* create(string type, IRace* race, Map* m, bool ie) {
        if (type == "Spy")
            return new Spy(race, m, ie);
        if (type == "Archer")
            return new Archer(race, m, ie);
        if (type == "Warrior")
            return new Warrior(race, m, ie);
    }
};



#endif //MYGAME_UNITS_H
