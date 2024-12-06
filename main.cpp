#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <random>
#include <thread>
#include <chrono>
#include "Player.h"
#include "AI.h"
#include "Item.h"

using namespace std;

void printWithDelay(const string &message, int delayMilliseconds)
{
    for (char c : message)
    {
        cout << c << flush; // 逐字符输出并刷新缓冲区
        this_thread::sleep_for(chrono::milliseconds(delayMilliseconds));
    }
    cout << endl;
}

int delay = 15;

random_device rd;
mt19937 gen(rd());

int sharedHealth = 0;

class Game
{
private:
    Player player;
    AI ai;
    bool isEndlessMode;
    int bonus;

public:
    Game() : player("Player", 4, 0), ai("AI", 4, 0), isEndlessMode(false), bonus(0) {}

    void startGame()
    {
        printWithDelay("Welcome to Buckshot Roulette!", delay);
        printWithDelay("Please sign your life-and-death contract and enter your name:", delay);
        string playerName;
        cin >> playerName;
        player = Player(playerName, 4, 0);

        printWithDelay("Choose whether to drink the potion: 1. Drink (Endless Mode) 2. Do not drink (Normal Mode)", delay);

        int choice = 0;
        while (choice != 1 && choice != 2)
        {
            cout << "Invalid input, please enter 1 or 2:" << endl;
            cin >> choice;
        }
        isEndlessMode = (choice == 1);

        cout << "Game start, " << (isEndlessMode ? "Endless Mode" : "Normal Mode") << "." << endl;

        mainGameLoop();
    }

    void mainGameLoop()
    {
        int round = 1;
        int temp = 0;
        int bulletCount = 0;
        while (!player.isDead() && round != 4 && bulletCount <= bulletChamber.size())
        {
            if (round != temp)
            {
                cout << "Round " << round << ":" << endl;
                temp = round;
            }
            generateBullets(); // 调用子弹生成函数
            playerTurn(bulletCount);
            if (player.isDead())
            {
                cout << "You lost! Game over!" << endl;
                break;
            }
            if (ai.isDead())
            {
                round++;
                uniform_int_distribution<> dist(2, 6);
                sharedHealth = dist(gen);
                ai.reset(sharedHealth);
                player.reset(sharedHealth);
            }

            aiTurn(bulletCount);
            if (ai.isDead())
            {
                round++;
                uniform_int_distribution<> dist(2, 5);
                sharedHealth = dist(gen);
                ai.reset(sharedHealth);
                player.reset(sharedHealth);
            }
            if (player.isDead())
            {
                cout << "You lost! Game over!" << endl;
                break;
            }
        }
        endGame();
    }

    void playerTurn(int &bulletCount)
    {
        cout << "It's your turn to act!" << endl;
        puts("");
        bool validAction = false; // 标记是否完成有效动作

        while (!validAction)
        {
            cout << "Choose your action: 1. Shoot 2. Use an item" << endl;
            int action;
            cin >> action;

            if (action == 1)
            { // 玩家选择开枪
                cout << "Choose your target: 1. Yourself 2. AI" << endl;
                int target;
                cin >> target;

                if (target == 1)
                {
                    // 玩家向自己开枪
                    bool hit = shoot(player, player);
                    bulletCount++;
                    if (hit)
                    {
                        cout << "You hit yourself!" << endl;
                        puts("");
                        bulletCount = 0;
                        if (!player.isDead())
                            generateBullets();
                        uniform_int_distribution<> dist(0, 3);
                        int temp1 = dist(gen);
                        player.addItem(player.items, temp1);
                        temp1 = dist(gen);
                        ai.addItem(ai.items, temp1);
                        player.printStatus();
                        validAction = true; // 有效动作，结束回合
                    }
                    else
                    {
                        cout << "You missed yourself!" << endl;
                        puts("");
                    }
                }
                else if (target == 2)
                {
                    // 玩家向 AI 开枪
                    bool hit = shoot(player, ai);
                    bulletCount++;
                    if (hit)
                    {
                        cout << "You hit the AI!" << endl;
                        puts("");
                        bulletCount = 0;
                        if (!ai.isDead())
                            generateBullets();
                        uniform_int_distribution<> dist(0, 3);
                        int temp1 = dist(gen);
                        int temp2 = dist(gen);
                        player.addItem(player.items, temp1);
                        temp1 = dist(gen);
                        ai.addItem(ai.items, temp1);
                        player.printStatus();
                    }
                    else
                    {
                        cout << "You missed the AI!" << endl;
                        puts("");
                    }
                    validAction = true; // 有效动作，结束回合
                }
                else
                {
                    cout << "Invalid target! Please choose again." << endl;
                    continue;
                }
            }
            else if (action == 2)
            { // 玩家选择使用道具
                auto &items = player.items;
                if (items.empty())
                {
                    cout << "You have no items to use! Please choose another action." << endl;
                }
                else
                {
                    cout << "hoose an item to use:" << endl;
                    for (size_t i = 0; i < items.size(); ++i)
                    {
                        cout << i + 1 << ". " << getItemName(items[i]) << endl;
                    }
                    int itemIndex;
                    cin >> itemIndex;
                    if (itemIndex > 0 && itemIndex <= items.size())
                    {
                        player.useItem(items[itemIndex - 1]);
                        items.erase(items.begin() + (itemIndex - 1));
                    }
                    else
                    {
                        cout << "Invalid item number! Please choose again." << endl;
                    }
                }
            }
            else
            {
                cout << "Invalid input, please choose again." << endl;
            }
        }
    }

    void aiTurn(int &bulletCount)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(1, 100); // 随机数生成器，用于模拟随机事件
        cout << "It's AI's turn to act!" << endl;
        int count_item = 0;
        while (1)
        {
            ActionType action = ai.decideAction(player);
            if (action == SHOOT_SELF)
            {
                bool hit = shoot(ai, ai);
                bulletCount++;
                if (hit)
                {
                    cout << "AI hit itself!" << endl;
                    bulletCount = 0;
                    generateBullets();
                    uniform_int_distribution<> dist(0, 3);
                    int temp1 = dist(gen);
                    int temp2 = dist(gen);
                    player.addItem(player.items, temp1);
                    temp1 = dist(gen);
                    ai.addItem(ai.items, temp1);
                    player.printStatus();
                    return;
                }
                else
                    cout << "AI missed itself!" << endl;
            }
            else if (action == SHOOT_OPPONENT)
            {
                bool hit = shoot(ai, player);
                bulletCount++;
                if (hit)
                {
                    cout << "AI hit you!" << endl;
                    bulletCount = 0;
                    generateBullets();
                    uniform_int_distribution<> dist(0, 3);
                    int temp1 = dist(gen);
                    int temp2 = dist(gen);
                    player.addItem(player.items, temp1);
                    temp1 = dist(gen);
                    ai.addItem(ai.items, temp1);
                    player.printStatus();
                    return;
                }
                else
                {

                    cout << "AI missed you!" << endl;
                    return;
                }
            }
            else if (action == USE_ITEM)
            {
                if (count_item <= 2)
                {
                    cout << "AI used an item!" << endl;
                    count_item++;
                }
                else
                    action = SHOOT_OPPONENT;
                bool hit = shoot(ai, player);
                bulletCount++;
                if (hit)
                {
                    cout << "AI hit you!" << endl;
                    bulletCount = 0;
                    generateBullets();
                    uniform_int_distribution<> dist(0, 3);
                    int temp1 = dist(gen);
                    player.addItem(player.items, temp1);
                    ai.addItem(ai.items, temp1);
                    player.printStatus();
                    return; // Exit the turn after shooting
                }
                else
                {
                    cout << "AI missed you!" << endl;
                    return; // Exit the turn after missing
                }
            }
        }
    }

    bool shoot(Player &shooter, Player &target)
    {
        int bullet = bulletChamber.front();
        bulletChamber.erase(bulletChamber.begin());
        int damage = doubleDamage ? 2 : 1;

        if (bullet == 1)
        { // 实弹命中
            target.setHealth(target.getHealth() - damage);

            cout << shooter.getName() << " hit the target! Damage caused:" << damage << endl;
            cout << target.getName() << " current health: " << target.getHealth() << endl;
            doubleDamage = false;
            return true;
        }
        else
        { // 空包弹未命中
            cout << shooter.getName() << " missed, used a blank bullet." << endl;
            doubleDamage = false;
            return false;
        }
    }

    void endGame()
    {
        if (player.isDead())
        {
            cout << "AI wins the game!" << endl;
        }
        else
        {
            cout << "Player wins the game!" << endl;
        }
    }
};

int main()
{
    cout << "        ▄█     █▄     ▄████████  ▄█        ▄████████  ▄██████▄    ▄▄▄▄███▄▄▄▄      ▄████████               ███      ▄██████▄  " << endl;
    cout << "       ███     ███   ███    ███ ███       ███    ███ ███    ███ ▄██▀▀▀███▀▀▀██▄   ███    ███           ▀█████████▄ ███    ███ " << endl;
    cout << "       ███     ███   ███    █▀  ███       ███    █▀  ███    ███ ███   ███   ███   ███    █▀               ▀███▀▀██ ███    ███ " << endl;
    cout << "       ███     ███  ▄███▄▄▄     ███       ███        ███    ███ ███   ███   ███  ▄███▄▄▄                   ███   ▀ ███    ███ " << endl;
    cout << "       ███     ███ ▀▀███▀▀▀     ███       ███        ███    ███ ███   ███   ███ ▀▀███▀▀▀                   ███     ███    ███ " << endl;
    cout << "       ███     ███   ███    █▄  ███       ███    █▄  ███    ███ ███   ███   ███   ███    █▄                ███     ███    ███ " << endl;
    cout << "       ███ ▄█▄ ███   ███    ███ ███▌    ▄ ███    ███ ███    ███ ███   ███   ███   ███    ███               ███     ███    ███ " << endl;
    cout << "       ▀███▀███▀    ██████████ █████▄▄██ ████████▀   ▀██████▀   ▀█   ███   █▀    ██████████              ▄████▀    ▀██████▀  " << endl;
    cout << endl;

    cout << "       ████████      ▄████████   ▄████         ████▄ ▄██████▄  ▄█        ███  ▄████████           ▄████████     ▄██████▄   ███    ███  ▄█          ▄████████     ███         ███        ▄████████  " << endl;
    cout << "       ███    ███    ███    ███   ███          ███     ███     ███        ███ ███    ███          ███    ███   ███    ███  ███    ███  ███         ███    ███ ▀█████████▄  ▀█████████▄  ███    ███ " << endl;
    cout << "       ███     ███   ███    █▀     ███        ███      ███     ███       ▄█   ███    █▀           ███    ███   ███    ███  ███    ███  ███         ███    █▀     ▀███▀▀██     ▀███▀▀██  ███    █▀  " << endl;
    cout << "       ███     ███  ▄███▄▄▄         ███      ███       ███     ███          ▀███████████         ▄███▄▄▄▄██▀   ███    ███  ███    ███  ███        ▄███▄▄▄         ███   ▀      ███   ▀ ▄███▄▄▄     " << endl;
    cout << "       ███     ███ ▀▀███▀▀▀          ███    ███        ███     ███                   ███         ▀███▀▀▀▀▀     ███    ███  ███    ███  ███       ▀▀███▀▀▀         ███          ███    ▀▀███▀▀▀     " << endl;
    cout << "       ███     ███   ███    █▄        ███  ███         ███     ███             ▄█    ███         ▀███████████  ███    ███  ███    ███  ███         ███    █▄      ███          ███      ███    █▄  " << endl;
    cout << "       ███    ██     ███    ███        █████           ███     ███           ▄████████▀           ███    ███   ███    ███  ███    ███  ███      ▄  ███    ███     ███          ███      ███    ███ " << endl;
    cout << "       ████████      ██████████       ▀██████▀       ▀██████▀  █████▄▄██                          ███    ███    ▀██████▀    ▀██████▀   █████▄▄██   ██████████    ▄████▀       ▄████▀    ██████████ " << endl;
    cout << endl;

    puts("");
    srand(static_cast<unsigned int>(time(0)));
    Game game;
    game.startGame();

    cout << "  ▄██████▄     ▄████████    ▄▄▄▄███▄▄▄▄      ▄████████       ▄██████▄   ▄█    █▄     ▄████████    ▄████████ " << endl;
    cout << "  ███    ███   ███    ███  ▄██▀▀▀███▀▀▀██▄   ███    ███      ███    ███ ███    ███   ███    ███   ███    ███ " << endl;
    cout << "  ███    █▀    ███    ███  ███   ███   ███   ███    █▀       ███    ███ ███    ███   ███    █▀    ███    ███ " << endl;
    cout << " ▄███          ███    ███  ███   ███   ███  ▄███▄▄▄          ███    ███ ███    ███  ▄███▄▄▄      ▄███▄▄▄▄██▀ " << endl;
    cout << "▀▀███ ████▄  ▀███████████  ███   ███   ███ ▀▀███▀▀▀          ███    ███ ███    ███ ▀▀███▀▀▀     ▀▀███▀▀▀▀▀   " << endl;
    cout << "  ███    ███   ███    ███  ███   ███   ███   ███    █▄       ███    ███ ███    ███   ███    █▄  ▀███████████ " << endl;
    cout << "  ███    ███   ███    ███  ███   ███   ███   ███    ███      ███    ███ ███    ███   ███    ███   ███    ███ " << endl;
    cout << "  ████████▀    ███    █▀    ▀█   ███   █▀    ██████████       ▀██████▀   ▀██████▀    ██████████   ███    ███ " << endl;
    cout << "                                                                                                                       " << endl;

    return 0;
}
