#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// ─── Player ───────────────────────────────────────────────────────────────────

struct Player {
    string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int healingItems;
    int depth; // progress through the Everglades (0-10)

    Player(const string& n)
        : name(n), health(100), maxHealth(100),
          attack(15), defense(5), healingItems(2), depth(0) {}

    bool isAlive() const { return health > 0; }

    void heal(int amount) {
        health = min(health + amount, maxHealth);
        cout << "You recover " << amount << " HP. HP: "
             << health << "/" << maxHealth << endl;
    }

    void takeDamage(int amount) {
        int actual = max(0, amount - defense);
        health -= actual;
        health = max(0, health);
        cout << "You take " << actual << " damage! HP: "
             << health << "/" << maxHealth << endl;
    }

    void displayStats() const {
        cout << "\n--- " << name << " ---" << endl;
        cout << "HP: " << health << "/" << maxHealth << endl;
        cout << "Attack: " << attack << " | Defense: " << defense << endl;
        cout << "Healing items: " << healingItems << endl;
        cout << "Depth into Everglades: " << depth << "/10" << endl;
    }
};

// ─── Enemy ────────────────────────────────────────────────────────────────────

struct Enemy {
    string name;
    int health;
    int attack;
    int defense;
    string description;

    Enemy(const string& n, int h, int a, int d, const string& desc)
        : name(n), health(h), attack(a), defense(d), description(desc) {}

    bool isAlive() const { return health > 0; }

    void takeDamage(int amount) {
        int actual = max(0, amount - defense);
        health -= actual;
        health = max(0, health);
        cout << name << " takes " << actual
             << " damage! HP remaining: " << health << endl;
    }
};

// ─── Helpers ──────────────────────────────────────────────────────────────────

static int randomInt(int lo, int hi) {
    return lo + rand() % (hi - lo + 1);
}

static void pressEnterToContinue() {
    cout << "\nPress ENTER to continue...";
    cin.get();
}

static void printSeparator() {
    cout << "\n" << string(50, '=') << "\n" << endl;
}

// ─── Combat ───────────────────────────────────────────────────────────────────

static void combat(Player& player, Enemy enemy) {
    cout << "\n*** COMBAT ***" << endl;
    cout << "A " << enemy.name << " appears!" << endl;
    cout << enemy.description << endl;

    while (player.isAlive() && enemy.isAlive()) {
        cout << "\nYour HP: " << player.health << "/" << player.maxHealth
             << "  |  " << enemy.name << " HP: " << enemy.health << endl;
        cout << "1. Attack" << endl;
        cout << "2. Use healing item (" << player.healingItems << " left)" << endl;
        cout << "3. Attempt to flee" << endl;
        cout << "Choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            int dmg = randomInt(player.attack - 3, player.attack + 5);
            cout << "You strike the " << enemy.name << "!" << endl;
            enemy.takeDamage(dmg);
        } else if (choice == 2) {
            if (player.healingItems > 0) {
                --player.healingItems;
                player.heal(randomInt(20, 35));
            } else {
                cout << "No healing items left — you waste your turn." << endl;
            }
        } else if (choice == 3) {
            if (randomInt(1, 3) == 1) {
                cout << "You successfully flee!" << endl;
                player.takeDamage(5); // small penalty
                return;
            } else {
                cout << "Failed to flee!" << endl;
            }
        } else {
            cout << "Invalid choice — you hesitate." << endl;
        }

        // Enemy's turn
        if (enemy.isAlive()) {
            int dmg = randomInt(enemy.attack - 3, enemy.attack + 3);
            cout << "The " << enemy.name << " attacks!" << endl;
            player.takeDamage(dmg);
        }
    }

    if (player.isAlive()) {
        int reward = randomInt(10, 20);
        cout << "\nYou defeated the " << enemy.name << "!" << endl;
        cout << "You scavenge some supplies and recover " << reward << " HP." << endl;
        player.heal(reward);
    }
}

// ─── Boss Battle ──────────────────────────────────────────────────────────────

static bool bossBattle(Player& player) {
    printSeparator();
    cout << "*** BOSS BATTLE: THE ANCIENT GATOR ***" << endl;
    cout << "\nYou reach the deepest part of the Everglades." << endl;
    cout << "The swamp grows darker. The air is thick with humidity." << endl;
    cout << "Suddenly the water erupts — a massive alligator, ancient and" << endl;
    cout << "scarred, rises from the murky depths. Its eyes glow with" << endl;
    cout << "primal fury. This creature has ruled these swamps for a century." << endl;
    pressEnterToContinue();

    Enemy boss("Ancient Gator", 120, 25, 8,
               "An enormous 15-foot alligator with armor-like scales.");

    int phase = 1;

    while (player.isAlive() && boss.isAlive()) {
        // Trigger phase 2 at half health
        if (boss.health <= 60 && phase == 1) {
            phase = 2;
            boss.attack += 8;
            cout << "\n*** The Ancient Gator enters a FRENZY — attacks intensify! ***" << endl;
            pressEnterToContinue();
        }

        cout << "\nYour HP: " << player.health << "/" << player.maxHealth
             << "  |  Ancient Gator HP: " << boss.health << endl;
        if (phase == 2) cout << "[ENRAGED] ";
        cout << "\n1. Attack" << endl;
        cout << "2. Use healing item (" << player.healingItems << " left)" << endl;
        cout << "3. Aim for the weak spot (high risk / high reward)" << endl;
        cout << "Choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            int dmg = randomInt(player.attack - 3, player.attack + 5);
            cout << "You strike the Ancient Gator!" << endl;
            boss.takeDamage(dmg);
        } else if (choice == 2) {
            if (player.healingItems > 0) {
                --player.healingItems;
                player.heal(randomInt(20, 35));
            } else {
                cout << "No healing items left!" << endl;
            }
        } else if (choice == 3) {
            if (randomInt(1, 2) == 1) {
                int dmg = randomInt(25, 40);
                cout << "You find a gap in the armor and strike true!" << endl;
                boss.takeDamage(dmg);
            } else {
                cout << "You miss the weak spot! The gator retaliates!" << endl;
                player.takeDamage(15);
            }
        } else {
            cout << "Invalid choice — you hesitate." << endl;
        }

        // Boss attacks
        if (boss.isAlive()) {
            int atk = randomInt(boss.attack - 5, boss.attack + 5);
            int special = randomInt(1, 4);
            if (special == 1) {
                cout << "The Ancient Gator DEATH ROLLS!" << endl;
                player.takeDamage(atk + 10);
            } else if (special == 2) {
                cout << "The Ancient Gator TAIL SWEEPS you!" << endl;
                player.takeDamage(atk);
                if (randomInt(1, 2) == 1) {
                    cout << "You are knocked down and take a second hit!" << endl;
                    player.takeDamage(randomInt(10, 20));
                }
            } else {
                cout << "The Ancient Gator snaps at you!" << endl;
                player.takeDamage(atk);
            }
        }
    }

    return player.isAlive();
}

// ─── Random Events ────────────────────────────────────────────────────────────

static void randomEvent(Player& player) {
    int event = randomInt(1, 8);

    switch (event) {
        case 1: {
            cout << "An alligator lunges from the water's edge!" << endl;
            pressEnterToContinue();
            Enemy gator("Alligator", 35, 14, 3,
                        "A 6-foot gator with snapping jaws.");
            combat(player, gator);
            break;
        }
        case 2: {
            cout << "A wild boar charges from the underbrush!" << endl;
            pressEnterToContinue();
            Enemy boar("Wild Boar", 30, 12, 2,
                       "A tusked boar that means business.");
            combat(player, boar);
            break;
        }
        case 3: {
            cout << "A water moccasin drops from a branch onto your path!" << endl;
            pressEnterToContinue();
            Enemy snake("Water Moccasin", 20, 18, 1,
                        "A venomous snake with deadly fangs.");
            combat(player, snake);
            break;
        }
        case 4: {
            cout << "You find a patch of medicinal herbs by the water." << endl;
            int heal = randomInt(15, 25);
            cout << "You apply them to your wounds." << endl;
            player.heal(heal);
            break;
        }
        case 5: {
            cout << "You discover an abandoned ranger station!" << endl;
            cout << "Inside you find a first-aid kit and rations." << endl;
            player.heal(randomInt(20, 35));
            ++player.healingItems;
            cout << "Healing items +1 (now have " << player.healingItems << ")." << endl;
            break;
        }
        case 6: {
            cout << "You get caught in a downpour and struggle through knee-deep mud." << endl;
            cout << "The effort drains you." << endl;
            player.takeDamage(randomInt(5, 15));
            break;
        }
        case 7: {
            cout << "A friendly fisherman shares tips about the swamp." << endl;
            cout << "\"Watch out fer the big one,\" he says with a knowing look." << endl;
            cout << "He shares his lunch with you." << endl;
            player.heal(10);
            break;
        }
        case 8: {
            cout << "A rare Florida panther watches you from the shadows." << endl;
            cout << "It stares for a long moment, then silently vanishes." << endl;
            cout << "The encounter fills you with awe and determination." << endl;
            player.maxHealth += 5;
            player.health = min(player.health + 5, player.maxHealth);
            cout << "Max HP +5! HP: " << player.health << "/" << player.maxHealth << endl;
            break;
        }
        default:
            break;
    }
}

// ─── Main Game Loop ───────────────────────────────────────────────────────────

static void gameLoop(Player& player) {
    cout << "\nYou set off into the Everglades, rifle in hand." << endl;
    cout << "Your mission: survey the deepest reaches of the swamp." << endl;

    while (player.isAlive() && player.depth < 10) {
        printSeparator();
        player.displayStats();

        cout << "\nWhat do you do?" << endl;
        cout << "1. Explore deeper into the Everglades" << endl;
        cout << "2. Rest and recover (restore 10 HP)" << endl;
        cout << "3. Check inventory" << endl;
        cout << "4. Quit" << endl;
        cout << "\nChoice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            ++player.depth;
            cout << "\nYou push deeper into the swamp... (Depth: "
                 << player.depth << "/10)" << endl;
            if (randomInt(1, 3) <= 2) {
                cout << "\n--- Something happens... ---" << endl;
                randomEvent(player);
            } else {
                cout << "The swamp is quiet. You make good progress." << endl;
            }
        } else if (choice == 2) {
            cout << "\nYou find a dry spot and rest for a while." << endl;
            player.heal(10);
        } else if (choice == 3) {
            player.displayStats();
        } else if (choice == 4) {
            cout << "\nYou decide to turn back. Another day, perhaps..." << endl;
            return;
        } else {
            cout << "Invalid choice — try again." << endl;
        }

        pressEnterToContinue();
    }

    if (!player.isAlive()) {
        printSeparator();
        cout << "*** GAME OVER ***" << endl;
        cout << "You have been defeated in the Everglades." << endl;
        cout << "The swamp claims another victim..." << endl;
        return;
    }

    // Depth 10 reached — boss time
    bool victory = bossBattle(player);
    printSeparator();
    if (victory) {
        cout << "*** VICTORY! ***" << endl;
        cout << "You have defeated the Ancient Gator!" << endl;
        cout << "The Everglades are safe once more." << endl;
        cout << "You return home as a hero of the swamp." << endl;
    } else {
        cout << "*** GAME OVER ***" << endl;
        cout << "The Ancient Gator was too powerful." << endl;
        cout << "The swamp keeps its secrets a little longer..." << endl;
    }
}

// ─── Entry Point ──────────────────────────────────────────────────────────────

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "==========================================" << endl;
    cout << "         EVERGLADES RANGER RPG            " << endl;
    cout << "==========================================" << endl;
    cout << "\nWelcome to the Florida Everglades." << endl;
    cout << "As a ranger you must venture deep into the" << endl;
    cout << "swamp and face whatever lurks within." << endl;

    bool playAgain = true;
    while (playAgain) {
        cout << "\nEnter your ranger's name: ";
        string name;
        getline(cin, name);
        if (name.empty()) name = "Ranger";

        Player player(name);
        cout << "\nGood luck, " << player.name << "!" << endl;
        pressEnterToContinue();

        gameLoop(player);

        cout << "\nPlay again? (y/n): ";
        char again;
        cin >> again;
        cin.ignore();
        playAgain = (again == 'y' || again == 'Y');
    }

    cout << "\nThanks for playing Everglades Ranger RPG!" << endl;
    return 0;
}
