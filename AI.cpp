#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include "AI.h"
#include "Player.h"
#include "Item.h"
using namespace std;

AI::AI(const string &playerName, int initialHealth, int bonus)
    : Player(playerName, initialHealth, bonus) {}

ActionType AI::decideAction(Player &opponent) const
{

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dist(1, 100);

  int myHealth = getHealth();
  int opponentHealth = opponent.getHealth();

  int realBullets = countRealBullets();
  int fakeBullets = countFakeBullets();

  int itemCount = items.size();
  // 判断是否需要恢复血量
  if (myHealth <= 2)
  {
    if (!hasItem("cigarette"))
    {
      cout << "Cigarette is insufficient!" << endl;
    }
    for (const auto &item : items)
    {
      if (getItemName(item) == "cigarette")
      {
        return USE_ITEM; // 使用香烟恢复血量
      }
    }
  }

  // 如果血量较高，优先进攻
  if (myHealth >= opponentHealth)
  {
    int realBullets = countRealBullets();
    int fakeBullets = countFakeBullets();

    // 1. 判断是否使用放大镜
    if (hasItem("magnifier"))
    {
      return USE_ITEM; // 使用放大镜查看下一发子弹
    }

    // 2. 如果有实弹，优先攻击对方
    if (realBullets >= 1)
    {
      return SHOOT_OPPONENT; // 有实弹，优先攻击对手
    }

    // 3. 如果没有实弹，检查是否有啤酒
    if (fakeBullets >= 1 && hasItem("beer"))
    {
      return USE_ITEM; // 使用啤酒跳过虚弹
    }

    // 4. 使用手铐限制对方行动（如果对手血量较低且自己血量较低）
    if (opponent.getHealth() < 30 && getHealth() < opponent.getHealth() && hasItem("handcuff"))
    {
      return USE_ITEM; // 使用手铐限制对方
    }

    // 5. 使用小刀增加伤害
    if (hasItem("knife"))
    {
      return USE_ITEM; // 使用小刀提高伤害
    }
  }

  // 如果血量较低，选择稳健策略
  return SHOOT_SELF; // 向自己开枪增加下一回合的行动机会
}

int AI::countRealBullets() const
{
  int count = 0;
  for (const auto &bullet : bulletChamber)
  {
    if (bullet == 1)
    {
      count++;
    }
  }
  return count;
}

int AI::countFakeBullets() const
{
  int count = 0;
  for (const auto &bullet : bulletChamber)
  {
    if (bullet != 1)
    {
      count++;
    }
  }
  return count;
}
