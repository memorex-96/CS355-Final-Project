#include <stdio.h> 
#include <ncurses.h> 
#include <string.h> 
#include <time.h> 
#include <stdlib.h> 

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

int score = 0;

void score_board(WINDOW *score_point){
  wclear(score_point); // Clear the score board
  box(score_point, 0, 0);
  wmove(score_point, 1, 1);
  wprintw(score_point, "Score: %d", score);
  wrefresh(score_point); // Refresh to show update.
}

// primary use for consumable position 
struct Position {
  int x; 
  int y; 
}; 

struct Position generate_food (int max_x, int max_y) {
  struct Position food; 
  food.x = (rand() % (max_x - 2)) + 1; 
  food.y = (rand() % (max_y - 2)) + 1; 
  return food; 
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
  srand(time(NULL));

  intro_screen(); 
  
  struct Position food = generate_food(GAME_W - 2, GAME_H - 2);


  // Creates score board
  WINDOW *score_point = newwin(4, 20, 10, 65);
  box(score_point, 0, 0);
  wprintw(score_point, "Score %d", score);
  wrefresh(score_point);

  game = newwin(GAME_H, GAME_W, 1, 2); 
  keypad(game, TRUE); // fopr arrow keys 
  
  werase(game); 
  box(game, 0, 0); //border 
  
  mvwaddch(game, y, x, '.');
  wrefresh(game); 
  
  wtimeout(game, TICK); 

  mvwaddch(game, food.y, food.x, 'X');

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

    if (x == food.x && y == food.y) {
      score += 5; 
      food = generate_food(GAME_W - 2, GAME_H - 2);
      werase(score_point);
      box(score_point, 0, 0);
      mvwprintw(score_point, 1, 1, "Score: %d", score); 
      wrefresh(score_point); 

      mvwaddch(game, food.y, food.x, 'X'); 

    }


    mvwaddch(game, y, x, '.');   // at new pos add the character 
    wrefresh(game); 
  }
  
  delwin(game); 
  endwin(); 

  return 0; 
}
