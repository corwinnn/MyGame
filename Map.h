//
// Created by Mike on 15.03.18.
//

#ifndef MYGAME_MAP_H
#define MYGAME_MAP_H

#include <iostream>
#include <vector>
using std::vector;
using std::string;
using std::cout;
using std::endl;
class Map {
public:
    string base;
    vector<string> map;
    vector<string> secret;
    Map() {
        map.resize(size);
        secret.resize(size);
        base = "";
        for (int i = 0;i < size; i++){
            base+="*";
        }
        for (int i = 0;i < size; i++){
            map[i] = base;
            secret[i] = base;
        }
    }



    void showMap() {
        cout << endl;
        for (int i = 0; i <= 9; i++) {
            cout << i << "  ";
        }
        for (int i = 10; i <= size; i++) {
            cout << i << " ";
        }
        cout << endl;
        for (int i = 1; i <= 9; i++) {
            cout << i << "  ";
            for (int j = 0; j < size; j++)
                cout << map[i-1][j] << "  ";
            cout << endl;
        }
        for (int i = 10; i <= size;i++) {
            cout << i << " ";
            for (int j = 0; j < size; j++)
                cout << map[i-1][j] << "  ";
            cout << endl;
        }
        cout << endl;
        cout << endl;
    }

    int getSize(){ return size; }
private:
    int size = 30;

};


#endif //MYGAME_MAP_H
