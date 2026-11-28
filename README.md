# CS355-Final-Project

This repo tracks the progress of our contributions to our development on the Snake game project.

## Requirements

- Linux, WSL2 (Ubuntu recommended), or GitHub Codespaces
- `gcc` and `make`
- ncurses development headers (`libncurses-dev` on Ubuntu/Debian)

> This project targets Linux/WSL and GitHub Codespaces. On Windows, run all commands from an Ubuntu WSL terminal, not PowerShell or CMD. To install WSL from Windows, run `wsl --install -d Ubuntu-24.04`.

## Quick start

### GitHub Codespaces

Open the repository in a Codespace. The VS Code terminal starts inside the repo, so no cloning or `cd` is needed:

```bash
make bootstrap
make run
```

Codespaces provides passwordless `sudo`, so `make bootstrap` can install `build-essential` and `libncurses-dev`. 

> Codespace containers are rebuilt from scratch. After a rebuild or when creating a new Codespace, run `make setup` again before building.

### Local WSL/Linux

Clone the repo inside your WSL home directory (not `/mnt/c/...`), then run:

```bash
cd ~/CS355/FinalProj/CS355-Final-Project
make bootstrap
make run
```

`make bootstrap` installs the required build tools and the ncurses development package, then builds the game. After the first setup, use:

```bash
make        # build
make run    # build and play
```

## Make targets

| Command | Description |
| --- | --- |
| `make` / `make all` | Check dependencies and build `snake` |
| `make setup` | Install `build-essential` and `libncurses-dev` (one time) |
| `make bootstrap` | Run `setup`, then build |
| `make run` | Build and run the game |
| `make clean` | Remove the built binary |
| `make help` | Show available targets |

## Snake Game 
### Controls 
The snake is controlled via keyboard input: 
  > WASD 
  > Arrow keys 
and each window prompt explains which required key needs to be pressed so that the player can continue. 

### Gameplay
Snake is a game where you move a snake within an area called a `snake pit`. 
The `Goal` is to grow your snake within the `snake pit` until the snake is equal half the length of the perimeter of the border size. 
To grow the snake, you must eat randomly-spawned consumable items. Each consumed item increases the snake length by 1 segment, and adds to the score. 
However, there are some obstacles: 
 > The snake must avoid touching the walls 
 > The snake must avoid running into itself
Both situations will cause the snake to instantly die. 
