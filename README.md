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

Codespaces provides passwordless `sudo`, so `make bootstrap` can install `build-essential` and `libncurses-dev`. Use the integrated terminal (Terminal → New Terminal), not the Output or Debug Console — ncurses needs a real TTY.

> Codespace containers are rebuilt from scratch. After a rebuild or when creating a new Codespace, run `make setup` again before building.

### Local WSL/Linux

Clone the repo inside your WSL home directory (not `/mnt/c/...`), then run:

```bash
cd ~/CS355/FinalProj/CS355-Final-Project
make bootstrap
make run
```

`make bootstrap` installs the required build tools and the ncurses development package (it will prompt for your `sudo` password), then builds the game. After the first setup, use:

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

## Manual setup

If you prefer to install dependencies yourself:

```bash
sudo apt update
sudo apt install -y build-essential libncurses-dev
make
```

Verify ncurses is available:

```bash
pkg-config --modversion ncursesw || ls /usr/include/ncurses.h
```

## Troubleshooting

- **`ncurses development files are missing`** — run `make setup` or install `libncurses-dev` manually.
- **`make: *** missing separator`** — Makefile recipe lines must be tab-indented, not space-indented.
- **Terminal looks broken after quitting** — run `reset`. Make sure the game calls `endwin()` on exit.
- **Nothing renders or the output looks wrong** — run `./snake` from an interactive terminal (WSL or Codespaces). ncurses needs a real TTY.
- **`TERM` issues** — check `echo $TERM`; it should be something like `xterm-256color`.
- **CRLF or `bad interpreter` errors** — clone the repo into the Linux filesystem (`~/...`), not `/mnt/c/...`, and keep Makefile/source files LF-only.
- **Codespaces: dependencies disappear after a rebuild** — rerun `make setup` or `make bootstrap`.
- **Codespaces: `sudo: a password is required`** — the default Codespaces image supports passwordless `sudo`; if you use a custom dev container, install `build-essential` and `libncurses-dev` in its configuration.
- **Codespaces: ncurses output is garbled or missing** — run `make run` from the integrated terminal, not the Output or Debug Console, and make the terminal at least 80×24.

