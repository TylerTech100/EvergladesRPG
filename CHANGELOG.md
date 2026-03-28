# Changelog

All notable changes to this project are documented here.

---

## [Initial Release] – 2026-03-13

The repository originally contained only a brief description in `README.md` with
no actual source code, making the project impossible to build or play.
The following files were added to make the game fully runnable:

### Added

#### `main.cpp` — Complete C++ game implementation
- **`Player` struct** — tracks name, HP (current and max), attack, defense,
  healing-item count, and depth (progress through the Everglades, 0–10).
- **`Enemy` struct** — reused for both regular encounters and the boss fight;
  holds name, HP, attack, defense, and a description.
- **Exploration loop** — the player chooses to explore, rest, check inventory,
  or quit each turn. Exploring increases depth by 1 and has a ~67 % chance of
  triggering a random event.
- **8 random events**:
  1. Alligator combat encounter
  2. Wild boar combat encounter
  3. Water moccasin combat encounter
  4. Medicinal herb healing (+15–25 HP)
  5. Abandoned ranger station (heal + gain a healing item)
  6. Mudstorm environmental hazard (take 5–15 damage)
  7. Friendly fisherman NPC (+10 HP)
  8. Florida panther sighting (permanent +5 max HP)
- **Turn-based combat** — each round the player can attack, use a healing item,
  or attempt to flee (small HP penalty on a successful escape).
- **Two-phase boss fight** — the Ancient Gator (120 HP) gains extra attack power
  when its HP drops to 50 %, and can use special moves (Death Roll, Tail Sweep).
  A high-risk "aim for the weak spot" option deals extra damage on a hit.
- **Play-again loop** — after a win or game-over, the player is offered a rematch.

#### `Makefile` — Build automation
| Target | Description |
|--------|-------------|
| `make` | Compile the game (`g++ -std=c++11 -Wall -Wextra -O2`) |
| `make run` | Compile and immediately launch the game |
| `make clean` | Remove the compiled binary |

#### `README.md` — Updated documentation
- Added a project title and corrected the description.
- Added build & run instructions for Linux/macOS (Make or manual `g++`) and
  Windows (MinGW and MSVC).
- Added a gameplay overview section.

### Fixed

- **Invalid-choice depth bug** — an erroneous `--player.depth` that would have
  appeared in the main menu's "invalid choice" branch was caught during
  development and never shipped. Depth is only incremented when the player
  successfully chooses to explore, so no decrement belongs in that branch.

### Other

- Added `everglades_rpg` and `everglades_rpg.exe` to `.gitignore` so compiled
  binaries are not tracked by Git.
