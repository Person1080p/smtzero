# SMT:ZER0
A simple turn based RPG based off the SMT/Persona series of JRPG's.

Built using SDL, GF2D, GFC, SimpleLogger/JSON, Nuklear

# Library Installation
External Library Requirements
 - SDL2
 - SDL2_image
 - SDL2_mixer
 - SDL2_ttf

If you are on a Linux machine running Debian/Ubuntu (or any distro using APT for package management), simply run the `libinstall.sh` script as root (using sudo) to obtain all of the libraries. Additionally, if you are running on a (WSL)[https://learn.microsoft.com/en-us/windows/wsl/install] instance, it will also install install the required libraries to have it pass through with x11. If using WSL, no GUI pops up, in powershell on your physical machine, type `wsl --update`, then in your WSL instance, update and upgrade packages (for ubuntu and debian people, type `sudo apt update` then `sudo apt upgrade`), then re-run the script and you'll be good to go.

# Build Process
Make sure you fetch submodules: `git submodule update --init --recursive`
Go into each submodule's src directory and type:
`make`
`make static`
Once each submodule has been made you can go into the base project src folder anre simply type:
`make`
You should now have a `gf2d` binary within the root of your git repository. Executing this will start your game.
---
If the above build process does not work, and you are running any linux distro with the correct libraries, then run the other script with non root premissions `autoSub.sh` and the script should automatically get all submodules, build, and run.

# Debug Features
    - [O] Loads a new map
    - [P] Force Starts a new Battle
    - [ESC] Exits the game

# Code Reuse
Most important pieces are handled in the battle.c/h file that handles most of the turn based combat. If you want to reuse feel free to modify the files
The `./config` folder is where most game functions are

# Midterm Deliverables

## Common Deliverables
- Functioning Entity System
    - Done
- Fully interact-able environment
    - Done
- User Interface (HUD, Stats, Messages, menus)
    - Done
- File read / write access (high score list, saved games, saved songs, etc)
    - Done, Load all enemines and player, write player level
- README for how to play and see all game features (this should be in the git repo)
    - Done, you are reading it right now

## Assigned Deliverables
- Turn Based Combat System
    - Done
- 10 recruitable monsters
    - Done, hit a chest for new ally
- Player Upgrade System (working)
    - Done, player level up system adding additional max hp
- Tile based world that is randomly generated
    - Done, selected randomly from existing maps files to combine to a new map
- 5 In-world Interactables (NPC's, Chests, etc.)
    - Chests
    - Encounters
    - NPC
    - Door
    - Lava Floor

<!---10/10-->
# Final Deliverables

## Common Deliverables
- Contained Game Experience
    - Done
- Audio
    - Done
- Research Component - Procedurely Generated Levels
    - Done
- Content Editor - Level Editor
    - Maybe, using html and js if it counts for anything
- Asset Consistency
    - Incomplete

## Assigned Deliverables
- Customizable Team (Abilities)
    - Incomplete
- Monster Negotiation
    - Incomplete
- Monster Fusion
    - Incomplete
- Items (Consumables, 10 Consumables), (Damage, heal, new ability)
    - Done
- Ability Affinities (Fire, water, weak, strong)
    - Done
<!---6/10-->