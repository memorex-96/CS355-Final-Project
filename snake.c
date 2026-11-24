#include <stdio.h> 
#include <ncurses.h> 
#include <string.h> 

#define GAME_H 20 
#define GAME_W 60 

static void intro_screen (void) {
  static const char* lines[] = {"SNAKE", "CS355", "Made by:", "Carson Crowley", "and", "Trebor Labree", " ", "Press 's' to start"}; 
  int n = (int)(sizeof lines / sizeof lines[0]);
  int top = (GAME_H - n) / 2; 

  WINDOW* intro = newwin(GAME_H, GAME_W, 1, 2); 
  if (intro == NULL) return; 
  
  box(intro, 0,0); 

  for (int i = 0; i < n; i++) {
    int len = (int)strlen(lines[i]); 
    int left = (GAME_W - len) / 2; 
    mvwaddstr(intro, top + i, left, lines[i]);
  }
  wrefresh(intro); 

  while (wgetch(intro) != 's') {
    /* continusaly loop, do nothing until s is pressed */
  }

  delwin(intro); 
}

int main (void) {   

  WINDOW* game; 
  /* window coords for character */
  int y = GAME_H / 2; 
  int x = GAME_W / 2; 
  
  //char direction; // need a n,s,e,w for the player to constantly move in a direction 

  int c;
  initscr(); 
  cbreak(); 
  noecho();  
  curs_set(0); 

  intro_screen(); 

  game = newwin(GAME_H, GAME_W, 1, 2); 
  keypad(game, TRUE); // fopr arrow keys 

  box(game, 0, 0); //border 
  
  mvwaddch(game, y, x, '.');
  wrefresh(game); 

  while ((c = wgetch(game))!= 'q') {
    mvwaddch(game, y, x, ' ');  // erase prev dot 
    
    switch (c) {
      case 'w' : 
        case KEY_UP: 
          if (y > 1) y--; 
          break; 
      case 's': 
        case KEY_DOWN:
          if (y < GAME_H - 2) y++; 
          break; 
      case 'a': 
        case KEY_LEFT:
          if (x > 1) x--; 
          break; 
      case 'd': 
        case KEY_RIGHT: 
          if (x < GAME_W - 2) x++; 
          break; 
    }

    mvwaddch(game, y, x, '.');   // at new pos add the character 
    wrefresh(game); 
  }
  
  delwin(game); 
  endwin(); 

  return 0; 
}
