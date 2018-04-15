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
/**
 * \brief Map text image
 *
 * Map is showing like number of symbols.
 * - <b>*</b> - unknown place
 * - <b>.</b> - place in your area of visibility
 * - <b>A - Z</b> - your units
 * - <b>a - z</b> - enemy units
 * - <b>+</b> - church
 *
 */
class Map {
public:
    string base;
    vector<string> map;
    vector<string> secret;
    /**
     * initialize map with '*'
     */
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


    /**
     * show the map
     */
    void showMap() const{
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
    /**
     *
     * @return map's size
     */
    int getSize() const { return size; }
private:
    int size = 30;

};


#endif //MYGAME_MAP_H
