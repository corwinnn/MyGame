//
// Created by Mike on 18.03.18.
//

#ifndef MYGAME_TEST_H
#define MYGAME_TEST_H
#include <gtest/gtest.h>
#include "World.h"

TEST(Race, names) {
    IRace *r = new Humans();
    string s1 = r->getNameRace();
    string s2 = r->getNameSpy();
    string s3 = r->getNameArcher();
    string s4 = r->getNameWarrior();
    EXPECT_EQ("Humans", s1);
    EXPECT_EQ("Spy_Human", s2);
    EXPECT_EQ("War_Human", s4);
    EXPECT_EQ("Human_Archer", s3);
    delete r;
}

TEST(Race, bonuses) {
    IRace *r = new Dwarfes();
    int b1 = r->getBonusHealth();
    int b2 = r->getBonusArcherDistance();
    int b3 = r->getBonusDefence();
    int b4 = r->getBonusAttack();
    int b5 = r->getBonusMove();
    int b6 = r->getBonusLookDistance();
    EXPECT_LT(b1,20);
    EXPECT_LT(b2,10);
    EXPECT_LT(b3,10);
    EXPECT_LT(b4,10);
    EXPECT_LT(b5,10);
    EXPECT_LT(b6,10);
    delete r;
}

TEST(Map, size) {
    Map *map = new Map();
    int size = map->getSize();
    EXPECT_LE(size, 60);
    EXPECT_GE(size, 10);
    delete map;
}

TEST(Map, init) {
    Map *map = new Map();
    for (int i = 0; i< map->getSize();i++)
        for (int j = 0; j < map->getSize(); j++){
            EXPECT_EQ('*', map->map[i][j]);
            EXPECT_EQ('*', map->secret[i][j]);
        }
    delete map;
}

TEST(CUnit, get) {
    Map* m = new Map();
    IRace* r = new Dwarfes();
    CUnit *u = Factory::create("Warrior", r, m, false);
    string s1 = u->getName();
    char c1 = u->getSymbol();
    int a1 = u->getDWL();
    int a2 = u->getAWL();
    u->setPlace(5,5);
    int a3 = u->getY();
    int a4 = u->getX();
    EXPECT_EQ(c1, 'W');
    EXPECT_EQ(s1, "War_Dwarf");
    EXPECT_EQ(a1, 0);
    EXPECT_EQ(a2, 0);
    EXPECT_EQ(a3, 5);
    EXPECT_EQ(a4, 5);
    delete u;
    delete m;
    delete r;
}

TEST(CUnit, heal) {
    Map* m = new Map();
    IRace* r = new Dwarfes();
    CUnit *u = Factory::create("Warrior", r, m, false);
    string s1 = u->getName();
    char c1 = u->getSymbol();
    int a1 = u->getDWL();
    int a2 = u->getAWL();
    u->setPlace(5,5);
    int a3 = u->getY();
    int a4 = u->getX();
    u->healing();
    EXPECT_LE(u->getHealth(), u->getMaxHealth());
    delete m;
    delete r;
    delete u;
}

TEST(CUnit, play) {
    World *world = new World();
    bool end = false;
    while(!end) {
        if (world->getMyUnit(0))
           EXPECT_LE(world->getMyUnit(0)->getX(), 60);
        if (world->getMyUnit(0))
           EXPECT_GE(world->getMyUnit(0)->getX(), 1);
        if (world->getMyUnit(0))
           EXPECT_LE(world->getMyUnit(0)->getY(), 60);
        if (world->getMyUnit(0))
           EXPECT_GE(world->getMyUnit(0)->getY(), 1);
        end = world->war();
    }
    delete world;

}


#endif //MYGAME_TEST_H
