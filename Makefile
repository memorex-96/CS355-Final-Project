TARGET := snake
SRC    := snake.c

CC     := gcc
CFLAGS := -Wall -Wextra -std=c11 -g
CFLAGS += $(shell pkg-config --cflags ncursesw 2>/dev/null)
LDLIBS += $(shell pkg-config --libs ncursesw 2>/dev/null || echo -lncursesw)

.DEFAULT_GOAL := all

all: check $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

check:
	@command -v $(CC) >/dev/null 2>&1 || { \
		echo "$(CC) not found. Run: make setup"; exit 1; \
	}
	@if ! pkg-config --exists ncursesw 2>/dev/null && [ ! -f /usr/include/ncurses.h ]; then \
		echo "ncurses development files are missing. Run: make setup"; \
		exit 1; \
	fi

setup:
	@if command -v $(CC) >/dev/null 2>&1 \
		&& { pkg-config --exists ncursesw 2>/dev/null || [ -f /usr/include/ncurses.h ]; }; then \
		echo "Build tools and ncurses development files already installed."; \
	elif command -v apt-get >/dev/null 2>&1; then \
		echo "Installing build tools and libncurses-dev..."; \
		sudo apt-get update && sudo apt-get install -y build-essential libncurses-dev; \
	elif command -v dnf >/dev/null 2>&1; then \
		echo "Installing build tools and ncurses-devel..."; \
		sudo dnf install -y gcc make ncurses-devel; \
	elif command -v pacman >/dev/null 2>&1; then \
		echo "Installing build tools and ncurses..."; \
		sudo pacman -S --needed --noconfirm base-devel ncurses; \
	else \
		echo "Could not detect a package manager. Install ncurses development headers manually."; \
		exit 1; \
	fi

bootstrap: setup
	@$(MAKE) --no-print-directory all

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

help:
	@echo "Targets:"
	@echo "  make bootstrap  install dependencies, then build"
	@echo "  make setup      install dependencies only"
	@echo "  make            build $(TARGET)"
	@echo "  make run        build and run"
	@echo "  make clean      remove the built binary"

.PHONY: all check setup bootstrap run clean help
