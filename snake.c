#include <stdio.h> 
#include <ncurses.h> 
#include <string.h> 
#include <time.h> 
#include <stdlib.h> 

#define GAME_H 20  // was 20 
#define GAME_W 40  // was 60 
#define TICK 85 // in ms 
int score = 0; 

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

static void lose_screen (void) {
  static const char* lines[] ={"GAME OVER!", "Press 'e' to exit"}; 
  int n = (int)(sizeof lines / sizeof lines[0]); 
  int top = (GAME_H - n)/2; 

  WINDOW* over = newwin(GAME_H, GAME_W, 1, 2); 
  if (over == NULL) return; 

  box(over, 0, 0); 

  for (int i = 0; i < n; i++) {
    int len = (int)strlen(lines[i]); 
    int left = (GAME_W - len) / 2; 
    mvwaddstr(over, top +i, left, lines[i]); 
  }
  
  char score_line[50]; 
  snprintf(score_line, sizeof score_line, "Final score: %d", score); 
  int score_len = (int)strlen(score_line); 
  mvwaddstr(over, top + n + 1, (GAME_W - score_len) / 2, score_line); 

  wrefresh(over);
  while (wgetch(over) != 'e') {

  }
  delwin(over); 
}

static void win_screen(void) {
  static const char* lines[] = {
    "YOU WIN!", 
    "Snake has grown to equal size of half the border!", 
    "Press any key to exit"
  };

  int n = (int)(sizeof lines / sizeof lines[0]); 
  int top = (GAME_H - n) / 2; 

  WINDOW* win = newwin(GAME_H, GAME_W, 1, 2); 
  if (win == NULL) return; 

  box(win, 0, 0); 

  for (int i = 0; i < n; i++) {
    int len = (int)strlen(lines[i]); 
    int left = (GAME_W - len) / 2; 

    mvwaddstr(win, top + i, left, lines[i]);
  }

  char score_line[50]; 
  snprintf(score_line, sizeof(score_line), "Final Score: %d", score); 

  int score_len = (int)strlen(score_line); 

  mvwaddstr(win, top + n + 1, (GAME_W - score_len) / 2, score_line); 

  wrefresh(win); 
  wgetch(win); 
  delwin(win); 
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

static void set_dir (int key, char* direction, char* player) {
  switch (key) {
    case KEY_UP: 
      case 'w': 
        *direction = 'n'; 
        *player = '^'; 
        break; 
    case KEY_DOWN: 
      case 's': 
        *direction = 's';
        *player = 'v'; 
        break; 
    case KEY_LEFT: 
      case 'a': 
        *direction = 'w';
        *player = '<'; 
        break; 
    case KEY_RIGHT: 
      case 'd': 
        *direction = 'e';
        *player = '>'; 
        break; 
  }
}

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

/*   SNAKE STUFF   */
/* snake struct, going to be a linked list */
struct Snake {
  int x, y;  // segment position  
  char head; // depends on direction snake is facing ^, <, >, V 
  char body; // =, |
  struct Snake* next; 
}; 

static struct Snake* snake_node (int x, int y) {
  struct Snake* n = (struct Snake*)malloc(sizeof(*n)); // has to be freed btw (free_snake_mem) 
  if (n == NULL) return NULL; 
  n->x = x; 
  n->y = y; 
  n->head = '^'; 
  n->body = '='; 
  n->next = NULL; 
  return n; 
}

static struct Snake* snake_create (int x, int y) {
  return snake_node(x,y); 
}

static void append_snake_segment (struct Snake* head) {
  if (head == NULL) return; 
  struct Snake* tail = head; 

  while (tail->next != NULL) tail = tail->next; 

  struct Snake* n = snake_node(tail->x, tail->y); 
  if (n != NULL) tail->next = n; 
}

static void snake_step (struct Snake* head, char dir) {
  if (head == NULL) return; 

  int px = head->x; 
  int py = head->y; 

  move_in_dir (dir, &head->x, &head->y); 
  for (struct Snake* s = head->next; s != NULL; s = s->next) {
    int tx = s->x; 
    int ty = s->y; 
    s->x = px; 
    s->y = py; 
    px = tx; 
    py = ty; 
  }
}

/* consumable shouldn't spawn on where the player is occupying space  */
static int occupied (const struct Snake* head, int x, int y) {
  for (const struct Snake* s = head; s != NULL; s = s->next) {
    if (s->x == x && s->y == y) return 1; 
  }
  return 0; 
}

static void draw_snake (WINDOW* game, struct Snake* head) {
  struct Snake* prev = NULL; 

  for (struct Snake* s = head; s!= NULL; s = s->next) {
    if (s == head) mvwaddch(game, s->y, s->x, s->head); 
    else {
      if (s->next != NULL) s->body = (s->y == s->next->y) ? '=' : '|'; 
      else if (prev != NULL) s->body = (s->y == prev->y) ? '=' : '|';

      mvwaddch(game, s->y, s->x, s->body);
    }
    prev = s; 
  }
}

static void free_snake_mem (struct Snake* head) {
  while (head != NULL) {
    struct Snake* next = head->next; 
    free(head); 
    head = next; 
  }
} 

// issue with snake being able to move 180 degrees 
static int is_reverse (char from, char to) {
  return (from == 'n' && to == 's') || (from== 's'&& to == 'n')||
  (from == 'e'&& to == 'w') || (from == 'w' && to == 'e'); 
}

// quick idea for lose condition, really needs to be checked later 
static int snake_hits_self (const struct Snake* head) {
  if (head == NULL) return 0; 

  for (const struct Snake* s = head->next; s != NULL; s = s->next) {
    if (s->x == head->x && s->y == head->y) return 1; 
  }
  return 0; 
}

static int snake_hits_border (const struct Snake* head){ // Checks to see if the snake hit the border
  if (head == NULL) return 0; // returns true if snake hasn't hit anything

  if (head->x <= 0 || head->x >= GAME_W - 1 || head->y <= 0 || head->y >= GAME_H - 1){ // Checks to see if the snake has hit either side of the border
        return 1; // returns false
    }
    return 0;
}

static int snake_length (const struct Snake* head) {
  int length = 0; 

  for (const struct Snake* s = head; s != NULL; s = s->next) {
    length++; 
  }
  return length; 
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

  struct Snake* snake = snake_create(x,y); 
  if (snake == NULL) {
    endwin(); 
    return 1; 
  }
  struct Position food = generate_food(GAME_W - 2, GAME_H - 2);
  while (occupied(snake, food.x, food.y)) {
    food = generate_food(GAME_W - 2, GAME_H - 2); 
  }

  // Creates score board
  WINDOW *score_point = newwin(4, 20, 10, 65); // Pointer for score box
  box(score_point, 0, 0); // Creates box for score
  wprintw(score_point, "Score %d", score); // Prints the score
  wrefresh(score_point); // Refreshes score

  game = newwin(GAME_H, GAME_W, 1, 2); 
  keypad(game, TRUE); // for arrow keys
  wtimeout(game, TICK); 
  
  werase(game); 
  box(game, 0, 0); //border 
  mvwaddch(game, food.y, food.x, 'X'); // we can change food char to something esle also 
  draw_snake(game, snake); 
  wrefresh(game); 
  
  
  char player = '^'; 
  
  // init with 3 bodty segments 
  for (int i = 0; i < 3; i++) {
    append_snake_segment(snake);
  }

  while ((c = wgetch(game))!= 'q') {
    if (c != ERR) {
      char old_dir = direction; 
      char old_player = player; 

      set_dir(c, &direction, &player); 
      if (is_reverse(old_dir, direction)) {
        direction = old_dir; 
        player = old_player; 
      }
      snake->head = player; 
    }
    
    int nx = snake->x; 
    int ny = snake->y; 
    move_in_dir (direction, &nx, &ny); 

    int ate = (nx == food.x && ny == food.y); 
    if (ate) {
      score += 5; 
      append_snake_segment(snake); 
    }

    snake_step(snake, direction); 

    if (ate) {
      do {
        food = generate_food(GAME_W - 2, GAME_H - 2); 
      } while (occupied(snake, food.x, food.y));
      score_board(score_point); 
    }
    
    if (snake_length(snake) >= GAME_H + GAME_W) { // snake_length(snake)
      win_screen(); 
      break; 
    }
    
    
    if (snake_hits_self(snake) || snake_hits_border(snake)) { // If the snake hits itself or the border
      lose_screen(); // Calls the function where the player gets a loser screen. 
      break;  // Breaks the loop.
    } 
    werase(game); 
    box(game, 0,0); 
    mvwaddch(game, food.y, food.x, 'X'); 
    draw_snake(game, snake); 
    wrefresh(game); 
  }
  
  free_snake_mem(snake); 
  delwin(score_point); 
  delwin(game); 
  endwin(); 

  return 0; 
}
