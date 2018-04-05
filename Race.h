//
// Created by Mike on 14.03.18.
//

#ifndef MYGAME_RACE_H
#define MYGAME_RACE_H

#include <bits/stdc++.h>

using std::vector;
using std::string;
using std::cout;
class IRace {
public:
    IRace() = default;

    string getNameRace() {return nameRace; }
    string getNameArcher() {return nameArcher; }
    string getNameWarrior() {return nameWarrior; }
    string getNameSpy() {return nameSpy; }
    int getBonusMove() {return bonusMove; }
    int getBonusAttack() {return bonusAttack; }
    int getBonusDefence() {return bonusDefence; }
    int getBonusArcherDistance() {return bonusArcherDistance; }
    int getBonusLookDistance() {return bonusLookDistance; }
    int getBonusHealth() { return bonusHealth; }

    virtual void superAction()=0;

protected:
    string nameRace;
    string nameArcher;
    string nameWarrior;
    string nameSpy;
    int bonusMove;
    int bonusAttack;
    int bonusDefence;
    int bonusArcherDistance;
    int bonusLookDistance;
    int bonusHealth;
};

class Dwarfes : public IRace {
public:
    Dwarfes() {
        nameRace = "Dwarfes";
        nameArcher = "Dwarf_Archer";
        nameWarrior = "War_Dwarf";
        nameSpy = "Spy_Dwarf";
        bonusMove = -2;
        bonusAttack = 5;
        bonusDefence = 5;
        bonusArcherDistance = -1;
        bonusLookDistance = 1;
        bonusHealth = 10;
    }

    void superAction() override { bonusAttack++; }
};

class Humans : public IRace {
public:
    Humans() {
        nameRace = "Humans";
        nameArcher = "Human_Archer";
        nameWarrior = "War_Human";
        nameSpy = "Spy_Human";
        bonusMove = 0;
        bonusAttack = 0;
        bonusDefence = 0;
        bonusArcherDistance = 0;
        bonusLookDistance = 0;
        bonusHealth = 0;
    }

    void superAction() override { bonusDefence++; }
};

class Elves : public IRace {
public:
    Elves() {
        nameRace = "Elves";
        nameArcher = "Elf_Archer";
        nameWarrior = "War_Elf";
        nameSpy = "Spy_Elf";
        bonusMove = 0;
        bonusAttack = 0;
        bonusDefence = 0;
        bonusArcherDistance = 0;
        bonusLookDistance = -1;
        bonusHealth = 10;
    }

    void superAction() override { bonusArcherDistance++; }
};

#endif //MYGAME_RACE_H