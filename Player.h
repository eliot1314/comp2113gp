#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Item.h" // Include item class header file

using namespace std;

enum ActionType
{
  SHOOT_SELF,
  SHOOT_OPPONENT,
  USE_ITEM
};

extern int sharedHealth;

// Player class
class Player
{
public:
  Player(const string &playerName, int initialHealth, int bonus);

  // Get player's name, health, and bonus
  string getName() const;
  int getHealth() const;
  int getBonus() const;
  vector<Item> items; // Items owned by the player
  int health;         // Player's health
  
  // Set player's health
  void setHealth(int newHealth);

  // Add items to the player's inventory
  void addItem(vector<Item> &newItems, int x);

  // Check if the player is dead
  bool isDead() const;

  // Check if the player owns an item
  bool hasItem(const string &itemName) const;

  // Perform an action
  void performAction(ActionType action);

  // Print player's status
  void printStatus() const;

  // Generate a random action
  ActionType decideAction() const;

  // Reset player's status
  void reset(int sharedHealth);

  // Update bonus (e.g., double it)
  void updateBonus(bool win);

  // Use items up to 4 times per round
  bool canUseItem() const { return useItemCount < 4; }

  // Increment the item usage count
  void incrementUseItemCount() { useItemCount++; }

  // Reset item usage count
  void resetUseItemCount() { useItemCount = 0; }

  void useKnife() { doubleDamage = true; }
  void useMagnifier()
  {
    int bullet = bulletChamber.front();
    cout << (bullet == 1 ? "The next round is a real bullet." : "The next shot is empty.") << endl;
  }
  void useBeer()
  {
    int skippedBullet = bulletChamber.front();
    bulletChamber.erase(bulletChamber.begin());
    cout << "Remove current bullet: " << (skippedBullet ? "Real Bullet" : "Fake Bullet") << endl;
  }
  void useCigarette()
  {
    if(health < sharedHealth)
      health++;
    printStatus();
  }

  void useItem(Item item);

private:
  string name;          // Player's name
  int bonus;            // Player's bonus
  int useItemCount = 0; // Tracks the number of times items are used (max 4 per round)
  
};

string getItemName(Item item);

#endif // PLAYER_H