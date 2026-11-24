#include <stdio.h> 
#include <ncurses.h> 
#include <string.h> 

#define GAME_H 20 
#define GAME_W 60 
#define TICK 120 // in ms 
 
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
 
void move_in_dir(char c, int* x, int* y) {  // const movement in dir 
  switch (c) {
    case 'n':
      (*y)--; 
      break; 
    case 's': 
      (*y)++; 
      break; 
    case 'e':
      (*x)++; 
      break; 
    case 'w':
      (*x)--; 
      break; 
  }
}

static void set_dir (int key, char* direction) {
  switch (key) {
    case KEY_UP: 
      case 'w': 
        *direction = 'n'; 
        break; 
    case KEY_DOWN: 
      case 's': 
        *direction = 's'; 
        break; 
    case KEY_LEFT: 
      case 'a': 
        *direction = 'w'; 
        break; 
    case KEY_RIGHT: 
      case 'd': 
        *direction = 'e'; 
        break; 
  }
}

int main (void) {   

  WINDOW* game; 
  /* window coords for character */
  int y = GAME_H / 2; 
  int x = GAME_W / 2; 
  
  char direction = 'n'; // need a n,s,e,w for the player to constantly move in a direction 

  int c;
  initscr(); 
  cbreak(); 
  noecho();  
  curs_set(0); 

  intro_screen(); 

  game = newwin(GAME_H, GAME_W, 1, 2); 
  keypad(game, TRUE); // fopr arrow keys 
  
  werase(game); 
  box(game, 0, 0); //border 
  
  mvwaddch(game, y, x, '.');
  wrefresh(game); 
  
  wtimeout(game, TICK); 

  while ((c = wgetch(game))!= 'q') {
    if (c != ERR) set_dir(c, &direction); 
     
    mvwaddch(game, y, x, ' ');  // erase prev dot 
    move_in_dir(direction, &x, &y); // step 
    
    /* Right now, just going to wrap when hit border, but hitting border should
     * make player lose */

    if (y < 1) y = GAME_H - 2; 
    else if (y > GAME_H - 2) y = 1; 

    if (x < 1) x = GAME_W - 2; 
    else if (x > GAME_W - 2) x = 1; 

    mvwaddch(game, y, x, '.');   // at new pos add the character 
    wrefresh(game); 
  }
  
  delwin(game); 
  endwin(); 

  return 0; 
}
