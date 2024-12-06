#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include "Item.h"

using namespace std;

vector<int> bulletChamber;
int currentBulletIndex;
bool isPlayerTurn = true;
bool doubleDamage = false;

// Randomly generate bullet chamber
void generateBullets()
{

    bulletChamber.clear();
    int totalBullets = rand() % 7 + 2;
    int realBullets = rand() % (totalBullets - 1) + 1;
    int fakeBullets = totalBullets - realBullets;
    for (int i = 0; i < realBullets; ++i)
        bulletChamber.push_back(1);
    for (int i = 0; i < fakeBullets; ++i)
        bulletChamber.push_back(0);
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::shuffle(bulletChamber.begin(), bulletChamber.end(), eng);
    printBullets(bulletChamber);
  //  currentBulletIndex = 0;
}

// Current bullet information
void printBullets(vector<int> bulletChamber)
{
    int realCount = 0, fakeCount = 0;
    for (int bullet : bulletChamber)
    {
        if (bullet == 1)
        {
            realCount++;
        }
        else
        {
            fakeCount++;
        }
    }
    cout << "Bullets in the GUN" << endl;
    cout << "Real Bullet: " << realCount << ", Fake Bullet: " << fakeCount << endl;
    puts("");
}
