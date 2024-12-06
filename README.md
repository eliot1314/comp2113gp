
# COMP2113 Group Project

Devil's Roulette


## Authors

- Team members: 
- Chen Haodong  （3035974030）
- Liu Huashitian   (3036128494)
- Rao Zhicheng    (3036432651)
- Sang Jingyao     (3036264468)
- Xu Chun Heung (3036220450)


## Discription

Welcome to Devil's Roulette, a turn-based strategy game that mixes high-stakes thrills, suspense, and intense challenges. Step into a dark, virtual world where danger lurks around every corner, and you face off against a mysterious demon in a game of life and death. This isn’t your ordinary casino game—it’s a tense, high-risk version of Russian Roulette, where the stakes are nothing less than your very survival. 

In Devil's Roulette, each round of betting is full of uncertainty, which could either reward you with massive winnings or blow you to pieces. The buckshot is unforgiving, every move you make could be your last. Your fate will be determined by luck and strategy. So, Dare to rise to the challenge, and become a rich winner?

## GameRules

1. The game has a gun with a given number of bullets in it. Before the game starts, the player knows how many real bullets and how many fake bullets are in the gun (real bullets do damage, fake bullets do not).

2. Each round, the player shoots first. The player may choose to shoot the demon or shoot himself. If shooting at himselves, if it is a real bullets, the player loses one drop of blood. And the next round is the demon's turn to choose how to shoot. If it's a fake bullets, the player doesn't lose any blood, and it's still the player's turn to choose how to fire the next round. If a player fires at the demon, whether it's a real bullet or a fake bullet, the demon chooses how to fire the next round. The first one to lose blood loses.

3. Props: There are four types of props. Two are randomly refreshed every round in Prop Match.
    1. Knife: When used, doubles the damage if the next round is a real bullet.
    2. Magnifier: After using it, you can check whether the current bullet is real or fake.
    3. Cigarettes: When used, they will give you one drop of blood back.
    4. Beer: When used, you can skip the current round and move on to the next round. It will show whether the skipped round is a real bullet or a fake bullet.

## Features Implemented

1. Multiple Game Modes (Endless and Standard)
    The game starts by prompting the player to choose between "Endless Mode" and "Standard Mode." This is implemented through user input where isEndlessMode is set to true for Endless Mode or false for Standard Mode based on the player's choice.

isEndlessMode = (choice == 1);

In Endless Mode, the game will continue until one player dies, while in Standard Mode, the game ends after 3 rounds.

2. Turn-Based System with Player and AI Interactions

    Players and AI alternate turns. Each player can either shoot themselves, shoot the opponent, or use an item. This is controlled using the mainGameLoop() function which alternates turns and checks for win conditions.

playerTurn(bulletCount);
aiTurn(bulletCount);

This system ensures that both the AI and player get a chance to perform an action during each round.

3. Dynamic Bullet Chamber and Shooting Mechanics

    The shoot() function controls the shooting logic, where a shot either hits or misses based on whether a real bullet or blank is selected. The player and AI can shoot during their turn, and the results are displayed to the console.

bool hit = shoot(player, ai);  

The bulletChamber array dynamically generates bullets each round, and the shoot() method checks if the bullet is real or a blank.

4. Item Usage and Randomization

    Players and AI can use items to gain advantages in the game. The items are randomly assigned to each player and AI at the start of each round. The player selects an item from their inventory using the useItem() function.

player.addItem(player.items, temp1);  

This adds an element of strategy, as the player can choose to use an item to recover health, attack, or perform other actions.

5. Game Status Updates with Delayed Output

    The printWithDelay() function allows for a dramatic, step-by-step display of the game's events, printing messages with a slight delay to enhance the game’s tension.

printWithDelay("Welcome to Buckshot Roulette!", delay);

The delayed output helps create a suspenseful atmosphere, especially during key moments like shooting or item use.

## Non-Standard Libraries Used

<random> – Used for generating random numbers in various parts of the game:

    Used to simulate the randomness of bullet outcomes (real vs. blank).
    Used to determine item randomization.
    Used for generating health boosts.

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dist(0, 3);  

<thread> and <chrono> – Used for controlling the output delays between game events:

    The printWithDelay() function introduces delays between character prints to simulate suspense during the game.

this_thread::sleep_for(chrono::milliseconds(delay));

## Compilation and Execution Instructions

Prerequisites:

    A C++17 compatible compiler (e.g., GCC, Clang, or MSVC).

    The code assumes the existence of a Player.h, AI.h, and Item.h header files with the necessary class definitions and methods. You will need to include these files or implement similar ones for the game to work.

Compilation Steps:

Enter "make" first. Then enter "./game". The program will run.

Below is our makefile：

FLAGS = -pedantic-errors -std=c++11

game: AI.o main.o Item.o Player.o
 g++ $(FLAGS) $^ -o $@
 
AI.o: AI.cpp AI.h Player.h Item.h
 g++ $(FLAGS) -c $<

main.o: main.cpp AI.h Player.h Item.h
 g++ $(FLAGS) -c $<

Player.o: Player.cpp Player.h Item.h
 g++ $(FLAGS) -c $<

Item.o: Item.cpp Item.h Player.h
 g++ $(FLAGS) -c $<

clean:
 rm -f game *.o

.PHONY: clean
