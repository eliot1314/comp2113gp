#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm> 
#include <random>

using namespace std;

// Bullet definition
class Bullet {
public:
    enum BulletType { Real, Fake }; 

private:
    BulletType type;

public:
    Bullet(BulletType type) : type(type) {}

    BulletType getType() const {
        return type;
    }

    int getDamage() const {
        return (type == Real) ? 1 : 0; 
    }

    string getTypeName() const {
        return (type == Real) ? "Real Bullet" : "Fake Bullet";
    }
};

// Global variables
extern vector<int> bulletChamber; 
//extern int currentBulletIndex;
extern bool isPlayerTurn;
extern bool doubleDamage;

// Randomly generate bullet chamber
void generateBullets();

// Current bullet information
void printBullets(vector<int> bulletChamber);

// Bullet damage
void fireBullet(bool &doubleDamage, int targethealth);

// Enum definition for item types
enum Item { knife, magnifiers, /*handcuff,*/ beer, cigarette };

// Item management class

#endif