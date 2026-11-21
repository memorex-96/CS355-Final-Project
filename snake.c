#include <stdio.h> 
#include <ncurses.h> 

int main (void) {
  initscr(); 
  cbreak(); 
  noecho(); 
  keypad(stdscr, TRUE); 
  curs_set(0); 


  printw("ncurses works! Press any key...");
  refresh(); 
  getch(); 

  endwin(); 
  return 0; 
}
