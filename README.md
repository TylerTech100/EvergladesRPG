# Everglades Ranger RPG

A simple console-based role-playing game written in C++. The player controls a ranger exploring the Everglades while encountering random events, managing health, and eventually facing a boss battle. The project was made to practice programming in C++ with concepts relating to conditional logic, random events, and game-state management.

## How to Build & Run

### Requirements
- A C++ compiler that supports C++11 (e.g. `g++`, `clang++`)
- GNU Make (optional, but convenient)

### Using Make
```bash
make        # compile
make run    # compile and launch the game
make clean  # remove the compiled binary
```

### Manual compilation
```bash
g++ -std=c++11 -Wall -O2 -o everglades_rpg main.cpp
./everglades_rpg
```

### Windows (MinGW / MSVC)
```bash
# MinGW
g++ -std=c++11 -Wall -O2 -o everglades_rpg.exe main.cpp
everglades_rpg.exe

# MSVC (Developer Command Prompt)
cl /EHsc /std:c++14 main.cpp /Fe:everglades_rpg.exe
everglades_rpg.exe
```

## Gameplay

- Enter a name for your ranger and explore the Everglades across **10 depth levels**.
- At each step you may **explore**, **rest**, or **check your inventory**.
- Random events include wildlife encounters (alligators, wild boar, water moccasins),
  helpful discoveries (medicinal herbs, abandoned ranger stations), and environmental hazards.
- **Combat** is turn-based: attack, use a healing item, or attempt to flee.
- Reach depth 10 and face the final boss — the **Ancient Gator**.
