#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
#include "Player.h"
#include "Item.h"

using namespace std;

Player::Player(const string &playerName, int initialHealth, int bonus)
    : name(playerName), health(initialHealth), bonus(bonus) {}

string Player::getName() const { return name; }
int Player::getHealth() const { return health; }
int Player::getBonus() const { return bonus; }
void Player::setHealth(int newHealth) { health = newHealth; }

/*
void Player::addItem(const vector<Item> &newItems)
{
  for (const auto &item : newItems)
  {
    if (items.size() < 10)
    {
      items.push_back(item);
    }
  }
}
*/

void Player::addItem(vector<Item> &newItems, int x)
{
  vector<Item> item_list;
  item_list.push_back(knife);
  item_list.push_back(magnifiers);
  item_list.push_back(beer);
  item_list.push_back(cigarette);
  if (newItems.size() < 4)
  {
    newItems.push_back(item_list[x]);
    cout << left;
    cout << getItemName(item_list[x]) << "is added to your inventory" << endl;
    puts("");
  }
}

// Check if the player is dead
bool Player::isDead() const
{
  return health <= 0;
}

bool Player::hasItem(const string &itemName) const
{
  return any_of(items.begin(), items.end(), [&](const Item &item)
                { return getItemName(item) == itemName; });
}

// Perform an action
void Player::performAction(ActionType action)
{
  switch (action)
  {
  case SHOOT_SELF:
    cout << name << " chose to shoot himself!" << endl;
    health -= 10; // Lose 10 health
    break;
  case SHOOT_OPPONENT:
    cout << name << " chose to shoot the opponent!" << endl;
    break;
  case USE_ITEM:
    if (canUseItem())
    {
      cout << name << " chose to use an item!" << endl;
      if (!items.empty())
      {
        Item &item = items.back(); // Assume the last item is used
        useItem(item);             // Apply item effects
        items.pop_back();          // Remove the item after use
        incrementUseItemCount();   // Increase item usage count
      }
      else
      {
        cout << "No items to use!" << endl;
      }
    }
    else
    {
      cout << "You have reached the maximum limit of 4 item uses this round!" << endl;
    }
    break;
  default:
    cout << "Invalid action!" << endl;
  }
}

void Player::printStatus() const
{
  cout << "Player Name: " << name << endl;
  cout << "Health: " << health << endl;
  cout << "Bonus: " << bonus << endl;
  cout << "Inventory: ";
  for (const auto &item : items)
  {
    cout << left << setw(15) << getItemName(item);
  }
  cout << endl;
  puts("");
}

// Generate a random action
ActionType Player::decideAction() const
{
  int action;
  cout << "Enter your action (0: Shoot yourself, 1: Shoot opponent, 2: Use item): ";
  cin >> action;
  switch (action)
  {
  case 0:
    return SHOOT_SELF;
  case 1:
    return SHOOT_OPPONENT;
  case 2:
    return USE_ITEM;
  default:
    cout << "Invalid action. Defaulting to Shoot Yourself." << endl;
    return SHOOT_SELF;
  }
}

void Player::reset(int sharedHealth)
{
  health = sharedHealth; // Reset the player's health based on shared health
  items.clear();         // Clear the inventory
  resetUseItemCount();   // Reset item usage count
  printStatus();         // Print the reset status
}

void Player::updateBonus(bool win)
{
  if (win)
  {
    bonus *= 2; // Double the bonus if the player wins
  }
  else
  {
    bonus = 0; // Reset bonus to 0 if the player loses
  }
}
/*
void Player::useItem(Item item) {
    if(item == )
    switch (item) {
        case knife:
            useKnife();
            break;
        case magnifiers:
            useMagnifier();
            break;
        case beer:
            useBeer();
            break;
        case cigarette:
            useCigarette();
            break;
    }
}
*/
void Player::useItem(Item item)
{
  switch (item)
  {
  case knife:
    useKnife();
    break;
  case magnifiers:
    useMagnifier();
    break;
  case beer:
    useBeer();
    break;
  case cigarette:
    useCigarette();
    break;
  }
}

string getItemName(Item item)
{
  switch (item)
  {
  case knife:
    return "knife";
  case magnifiers:
    return "magnifiers";
    // case handcuff: return "handcuff";
  case beer:
    return "beer";
  case cigarette:
    return "cigarette";
  default:
    return "unknown";
  }
}