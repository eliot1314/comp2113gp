#ifndef AI_H
#define AI_H

#include "Player.h"

// AI class, inherits from the Player class
class AI : public Player
{
public:
  AI(const string &playerName, int initialHealth, int bonus);

  // AI makes an action decision based on the current state
  ActionType decideAction(Player &opponent) const;

private:
  // Calculate the number of real bullets
  int countRealBullets() const;

  // Calculate the number of fake bullets
  int countFakeBullets() const;
};

#endif // AI_H