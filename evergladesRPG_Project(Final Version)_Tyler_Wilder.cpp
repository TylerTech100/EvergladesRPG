// Name: Tyler Wilder
// Project: Lost in the Everglades RPG
// Date: August 5, 2025
// Description: A 5x5 map-based RPG game where the player (ranger) must survive and 
//rescue tourists before time runs out.

//Summary: This a simple RPG game where the player controls a ranger navigating through the Everglades 
// encountering dangers while rescuing tourists. The game features a 5x5 grid map, random danger placements,
// and even power-ups to enhance gameplay. The player has limited resources (gongs) and health,
// need a strategic element to movement and decision-making, as well as a scoring system to track wins and losses.
// Additional features include a menu system, rules display, and replayability. Further even movement keys with WASD keys for user input to move thier avatar

//IPO Chart:
 
//Input: User inputs to navigate the map (WASD keys)
//       make decisions during encounters (fight or wait).  
//       Menu selections (view rules, start game, quit).

//Processing: The game processes user inputs to update the ranger's position on the map,
//            checks for encounters with dangers or tourists, updates gongs and health,
//            and determines game outcomes (win/loss).

//Output: The game outputs the current state of the map, gongs left, health status, and rescued tourists.
//        It also provides feedback on encounters and game results.
//        Displays menu and rules to the player.



#include <iostream>
#include <cstdlib>   // For random number generation
#include <ctime>     // For seeding the random number generator
#include <string>

using namespace std;

// Function prototypes
void viewMenu();
void viewRules();
bool startGame(const string& playerName);
void initializeMap(char map[5][5], int touristCount);// modified to add touristCount
void showMap(char map[5][5]);
void placeDangers(int dangerMap[5][5], int dangerTypeMap[5][5], char map[5][5]);
void placePowerUps(char map[5][5]);
// Updated movement function to include dangerMap and tourist tracking
void moveRanger(char map[5][5], int &row, int &col, char direction, int &gongsLeft, int dangerMap[5][5], int dangerTypeMap[5][5], int &rescuedTourists, int touristCount, int &health);
bool handleRangerMovement(char map[5][5], int &row, int &col, int &gongsLeft, int dangerMap[5][5], int dangerTypeMap[5][5], int &rescuedTourists, int touristCount, int &health);
// Main function to run the game ( had to also remember at first what comes first in the code)
int main() {
    string playerName;
    int wins = 0;
    int losses = 0;
    char playAgain = 'y';
    cout << "What's your name Ranger?: ";
    getline(cin, playerName);
    while (playAgain == 'y' || playAgain == 'Y') {
        int selection;
        do {
            viewMenu();
            cin >> selection;
            cin.ignore(); // clear newline
            switch (selection) {
                case 1:
                    viewRules();
                    break;
                case 2: {
                    bool didWin = startGame(playerName);
                    if (didWin) wins++;
                    else losses++;
                    cout << "Score for " << playerName << ": " << wins << " Wins, " << losses << " Losses." << endl;
                    
                    // Keep asking until a valid response is given (Also fixed for invalid responses too)
                    bool validResponse = false;
                    while (!validResponse) {
                        cout << "Play again? (y/n): ";
                        cin >> playAgain;
                        cin.ignore();
                        
                        if (playAgain == 'y' || playAgain == 'Y' || playAgain == 'n' || playAgain == 'N') {
                            validResponse = true;
                        } else {
                            cout << "Invalid response. Please enter 'y' or 'n'." << endl;
                        }
                    }
                    break;
                }
                case 3:
                    cout << "Thank you for playing!" << endl;
                    playAgain = 'n';
                    break;
                default:
                    cout << "Invalid selection. Please try again." << endl;
            }
        } while (selection != 3 && playAgain == 'y');
    }
    return 0;
}

// Function to display the main menu of the game
void viewMenu() {   
    cout << "Welcome to the Everglades RPG!" << endl;
    cout << "1. View Rules" << endl;
    cout << "2. Start Game" << endl;
    cout << "3. Quit" << endl;
    cout << "Please enter your selection: ";
}
// Function to display the rules of the game
void viewRules() {
    cout << "Game Rules:" << endl;
    cout << "1. You are a ranger in search of lost tourists in a 5x5 cell map of the Everglades." << endl;
    cout << "2. You must rescue them before time runs out (12 gongs)." << endl;
    cout << "3. Your decisions will impact the outcome of the game." << endl;
    cout << "4. Beware of all the dangers hidden in the Everglades!" << endl;
    cout << "5. Good Luck Ranger!" << endl;
}

// Updated startGame function ( constantly modified)
// Returns true if player wins, false if loses
bool startGame(const string& playerName) {
    // Game initialization
    cout << "Starting game for " << playerName << "..." << endl;
    char map[5][5];
    int dangerMap[5][5];
    int dangerTypeMap[5][5]; // 0 = no danger, 1 = Snake, 2 = Mosquito Swarm, 3 = Alligator, 4 = Venomous Spider
    int gongsLeft = 12;
    int health = 10; // Player health to add as a mechanic for dangers, events, and power ups ( had to be tweaked, alot)
    int rangerRow = 0;
    int rangerCol = 0;
    // Track rescued tourists
    const int touristCount = 3;
    int rescuedTourists = 0;
    initializeMap(map, touristCount);
    placeDangers(dangerMap, dangerTypeMap, map);
    placePowerUps(map);
    showMap(map);

    // Main game loop: continue until gongs run out or all tourists are rescued (had to update this)
    // Main game loop: keep going while you have gongs, health, and tourists left
    while (gongsLeft > 0 && rescuedTourists < touristCount && health > 0) {
        bool stillPlaying = handleRangerMovement(map, rangerRow, rangerCol, gongsLeft, dangerMap, dangerTypeMap, rescuedTourists, touristCount, health);
        if (gongsLeft < 0) gongsLeft = 0;
        if (!stillPlaying) break;
        showMap(map);
        cout << "Gongs left: " << gongsLeft << " | Health: " << health << endl;
        if (gongsLeft <= 0 || health <= 0) break;
    }
    if (rescuedTourists == touristCount) {
        cout << "You rescued all the tourists! YOU WIN!" << endl;
        return true;
    } else if (gongsLeft <= 0) {
        cout << "You have run out of gongs! GAME OVER." << endl;
    } else if (health <= 0) {
        cout << "You have run out of health! GAME OVER." << endl;
    }
    return false;
}
// Function to initialize the game map with default values
// Fills all cells with '*', places the ranger at (0,0), and randomly places tourists
void initializeMap(char map[5][5], int touristCount) {
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            map[row][col] = '*'; // Default cell
        }
    }
    map[0][0] = 'R'; // Ranger starts in upper left

    // Randomly place tourists
    int placed = 0;
    srand(static_cast<unsigned int>(time(0)) + 1); // Different seed from dangers
    while (placed < touristCount) {
        int row = rand() % 5;
        int col = rand() % 5;
        // Avoid placing tourist on ranger
        if ((row != 0 || col != 0) && map[row][col] == '*') {
            map[row][col] = 'T';
            placed++;
        }
    }
}

// Function to show where the user is on the map
// Function to display the current state of the map to the player
void showMap(char map[5][5]) {
    cout << "\nCurrent Map:" << endl;
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            cout << map[row][col] << " ";
        }
        cout << endl;
    }
}

// Function to randomly place 10 dangers on the map
// Dangers are tracked in a separate dangerMap array (1 = danger, 0 = safe)
// Ensures dangers are not placed on the ranger or tourist starting positions
void placeDangers(int dangerMap[5][5], int dangerTypeMap[5][5], char map[5][5]) {
    srand(static_cast<unsigned int>(time(0)));
    int placed = 0;
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            dangerMap[row][col] = 0; // Initialize all cells as safe
            dangerTypeMap[row][col] = 0; // No danger type
        }
    }
    while (placed < 10) {
        int row = rand() % 5;
        int col = rand() % 5;
        // Only place danger if not on ranger or tourist, and cell is empty of danger
        if (dangerMap[row][col] == 0 && map[row][col] != 'R' && map[row][col] != 'T') {
            dangerMap[row][col] = 1;
            dangerTypeMap[row][col] = (rand() % 4) + 1; // 1=Snake, 2=Mosquito Swarm, 3=Alligator, 4=Venomous Spider
            placed++;
        }
    }
}

// Place 3 power-ups ('+') randomly on the map
void placePowerUps(char map[5][5]) {
    srand(static_cast<unsigned int>(time(0)) + 2);
    int placed = 0;
    while (placed < 3) {
        int row = rand() % 5;
        int col = rand() % 5;
        if (map[row][col] == '*') {
            map[row][col] = '+';
            placed++;
        }
    }
}

// Function to move the ranger based on user input
// Handles movement, boundary checks, danger encounters, and tourist rescue
void moveRanger(char map[5][5], int &row, int &col, char direction, int &gongsLeft, int dangerMap[5][5], int dangerTypeMap[5][5], int &rescuedTourists, int touristCount, int &health) {
    int oldRow = row;
    int oldCol = col;

    // Move ranger according to direction, with bounds limiting
    switch (direction) {
        case 'w':
        case 'W':
            if (row > 0) row--;
            break;
        case 's':
        case 'S':
            if (row < 4) row++;
            break;
        case 'a':
        case 'A':
            if (col > 0) col--;
            break;
        case 'd':
        case 'D':
            if (col < 4) col++;
            break;
        default:
            cout << "Invalid direction. Use w/a/s/d." << endl;
            return;
    }

    gongsLeft--; // Each move costs a gong
    // Prevent gongs from going negative
    if (gongsLeft < 0) gongsLeft = 0;
    // If you run out of gongs, stop moving
    if (gongsLeft <= 0) {
        return; // Return silently, let startGame function handle the game over message
    }

    // Check for danger at new position
    if (dangerMap[row][col] == 1) {
        // Show danger type
        string dangerName;
        int type = dangerTypeMap[row][col];
        if (type == 1) dangerName = "Snake";
        else if (type == 2) dangerName = "Mosquito Swarm";
        else if (type == 3) dangerName = "Alligator";
        else if (type == 4) dangerName = "Venomous Spider";
        else dangerName = "Unknown";
        cout << "You encountered a danger! Type: " << dangerName << ". (f)ight or (w)ait? ";
        char action;
        cin >> action;
        if (action == 'f' || action == 'F') {
            int fightResult = rand() % 2; // 0 = win, 1 = lose
            if (fightResult == 0) {
                // Unique effects for each danger type
                if (type == 1) { // Snake
                    cout << "You beat the snake! Lost 1 gong." << endl;
                    gongsLeft -= 1;
                } else if (type == 2) { // Mosquito Swarm
                    cout << "You escaped the mosquito swarm! Lost 2 gongs." << endl;
                    gongsLeft -= 2;
                } else if (type == 3) { // Alligator
                    cout << "You fought the alligator! Lost 1 health and 2 gongs." << endl;
                    health--;
                    gongsLeft -= 2;
                } else if (type == 4) { // Venomous Spider
                    cout << "You defeated the venomous spider! Lost 1 health." << endl;
                    health--;
                }
                if (gongsLeft < 0) gongsLeft = 0;
                dangerMap[row][col] = 0; // Remove danger
                dangerTypeMap[row][col] = 0;
            } else {
                // Unique lose effects
                if (type == 1) {
                    cout << "The snake bit you! Lost 1 health and 2 gongs. Returning to previous position." << endl;
                    health--;
                    gongsLeft -= 2;
                } else if (type == 2) {
                    cout << "You got swarmed by mosquitoes! Lost 3 gongs. Returning to previous position." << endl;
                    gongsLeft -= 3;
                } else if (type == 3) {
                    cout << "The alligator bit you! Lost 2 health and 3 gongs. Returning to previous position." << endl;
                    health -= 2;
                    gongsLeft -= 3;
                } else if (type == 4) {
                    cout << "The venomous spider bit you! Lost 2 health and 2 gongs. Returning to previous position." << endl;
                    health -= 2;
                    gongsLeft -= 2;
                }
                if (gongsLeft < 0) gongsLeft = 0;
                row = oldRow;
                col = oldCol;
                // Do not update map position
                return;
            }
        } else if (action == 'w' || action == 'W') {
            cout << "You waited and lost 3 gongs." << endl;
            gongsLeft -= 3;
            if (gongsLeft < 0) gongsLeft = 0;
            // Stay in place, danger remains
        } else {
            cout << "Invalid action. You need to type (f)ight or (w)ait." << endl;
            gongsLeft -= 0;
            if (gongsLeft < 0) gongsLeft = 0;
        }
        if (gongsLeft <= 0 || health <= 0) return;
    }

    // Power-up logic
    if (map[row][col] == '+') {
        cout << "You found a power-up! All gongs and health restored!" << endl;
        gongsLeft = 12; // Restore all gongs
        health = 10;    // Restore all health
        map[row][col] = '*'; // Remove the power-up from the map
    }

    // Check for tourist at new position
    if (map[row][col] == 'T') {
        rescuedTourists++;
        map[oldRow][oldCol] = '*'; // Clear old position
        map[row][col] = 'R';       // Place ranger at tourist position
        cout << "You rescued a tourist! " << (touristCount - rescuedTourists) << " remaining." << endl;
    } else {
        // Update map to reflect ranger's new position
        map[oldRow][oldCol] = '*';
        map[row][col] = 'R';
    }

    cout << "Gongs left: " << gongsLeft << endl;
}

// Function to handle user input for ranger movement
// Prompts for direction, calls moveRanger, and returns whether the game continues
bool handleRangerMovement(char map[5][5], int &row, int &col, int &gongsLeft, int dangerMap[5][5], int dangerTypeMap[5][5], int &rescuedTourists, int touristCount, int &health) {
    char direction;
    cout << "Enter direction (w/a/s/d): ";
    cin >> direction;
    moveRanger(map, row, col, direction, gongsLeft, dangerMap, dangerTypeMap, rescuedTourists, touristCount, health);
    return gongsLeft > 0 && health > 0;
}


