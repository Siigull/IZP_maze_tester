#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
  int rows;
  int cols;
  unsigned char *cells; // 0.bit -> left, 1.bit -> right, 2.bit -> top/bottom
} Map;

typedef struct {
  int row;
  int col;
} Cell;

Map* init_map(int rows, int cols) {
  Map* map = malloc(sizeof(Map));
  if(map == NULL) return NULL;
    
  map->cells = malloc(sizeof(unsigned char) * (size_t)rows * (size_t)cols);
  if(map->cells == NULL) {
    free(map);
    return NULL;
  }

  map->rows = rows;
  map->cols = cols;

  for(int i=0; i < map->rows; i++) {
    for(int j=0; j < map->cols; j++) {
      map->cells[i*map->cols + j] = 0;
    }
  }

  return map;
}

void free_map(Map* map) {
  free(map->cells);
  free(map);
}

void print_map(Map *map) {
  printf("%d %d\n", map->rows, map->cols);
  
  for(int i=0; i < map->rows; i++) {
    for(int j=0; j < map->cols; j++) {
      printf("%d ", map->cells[i*map->cols + j]);
    }
    printf("\n");
  }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        swap(&arr[i], &arr[j]);
    }
}

void generate_full(Map *map) {
  for(int i=0; i < map->rows; i++) {
    for(int j=0; j < map->cols; j++) {
      map->cells[i*map->cols + j] = 7;
    }
  }
}

bool isborder(Map* map, int row, int col, int border) {
  if((map->cells[row*map->cols + col]) & border) return true;
  return false;
}

Cell get_move_cell(int move, bool rotated) {
  Cell cell = {0,0};

  if(move == 1) 
    cell.col = -1;

  else if(move == 2)  
    cell.col = 1;

  else { 
    if(rotated) 
      cell.row = -1;
    
    else 
      cell.row = 1;
  }

  return cell;
}

bool is_outside(Map* map, int row, int col) {
  return (row >= map->rows || row < 0 || col >= map->cols || col < 0);
}

bool is_rotated(int row, int col) {
  return ((row + col) % 2) == 0;
}

bool generate_perfect_from_full(Map* map, int row, int col, bool** vis) {
  if (is_outside(map, row, col) || vis[row][col]) return false;

  vis[row][col] = true;
   

  int moves[] = {1, 2, 4};
  
  shuffle(moves, 3);

  for(int i=0; i < 3; i++) {

    Cell cell = get_move_cell(moves[i], is_rotated(row, col));

    bool x = generate_perfect_from_full(map, row+cell.row, col+cell.col, vis);
           
    if(x) {
      map->cells[row*map->cols + col] &= ~moves[i];
      map->cells[(row + cell.row)*map->cols + col + cell.col] &= ~(moves[i] == 4? 4 : moves[i] ^ 0b11);
    }
  }

  return true;
}

typedef struct {
  Cell* moves;
  int size;
  int curr;
} Stack;

Stack* stack_init() {
  Stack* stack = malloc(sizeof(Stack));

  stack->size = 8;
  stack->curr = 0;

  stack->moves = malloc(sizeof(Cell) * stack->size);

  return stack;  
}

void stack_push(Stack* stack, Cell cell) {
  if(stack->size - 2 <= stack->curr) {
    stack->size *= 2;
    void* result = realloc(stack->moves, stack->size * sizeof(Cell));
    if(result == NULL) exit(1);
    stack->moves = result;
  }
  
  stack->moves[stack->curr++] = cell;
}

Cell stack_pop(Stack* stack) {
  return stack->moves[--stack->curr];
}

void stack_free(Stack* stack) {
  free(stack->moves);
  free(stack);
}

bool stack_empty(Stack* stack) {
  return stack->curr == 0;
}

void stack_generate_perfect_from_full(Map* map, int row, int col, bool** vis) {
  Stack* stack = stack_init();

  stack_push(stack, (Cell){0, 0});
  vis[0][0] = true;

  while(!stack_empty(stack)) {
    Cell cell = stack_pop(stack);

    int moves[] = {1, 2, 4};
    shuffle(moves, 3);

    for(int i=0; i < 3; i++) {
      Cell move = get_move_cell(moves[i], is_rotated(cell.row, cell.col));

      if(is_outside(map, cell.row + move.row, cell.col + move.col)) continue;
      
      if(!vis[cell.row + move.row][cell.col + move.col]) {
        // stack_push(stack, cell);
        stack_push(stack, (Cell){cell.row + move.row, cell.col + move.col});

        map->cells[cell.row*map->cols + cell.col] &= ~moves[i];
        map->cells[(cell.row + move.row)*map->cols + cell.col + move.col] &= ~(moves[i] == 4? 4 : moves[i] ^ 0b11);

        vis[cell.row + move.row][cell.col + move.col] = true;
      }
    }
  }

  stack_free(stack);
}

int main(int argc, char** argv) {
  srand(time(NULL));

  if(argc > 4) {
    printf("bad number of args\n");
    return 0;
  }
  
  Map* map = init_map(atoi(argv[1]), atoi(argv[2]));

  bool** vis = calloc(map->rows, sizeof(bool*));
  for(int i=0; i < map->rows; i++) {
    vis[i] = calloc(map->cols, sizeof(bool));
  }

  generate_full(map);

  if(argc == 3) {
    generate_perfect_from_full(map, 0, 0, vis);
  } else {
    if(argc == 4 && !strcmp(argv[3], "--stack")) {
      stack_generate_perfect_from_full(map, 0, 0, vis);
    } else {
      printf("Arg error.\n");
      return 1;
    }
  }

  map->cells[0] &= ~1;
  map->cells[(map->rows) * map->cols - 1] &= ~2;

  print_map(map);

  free(vis);
  free(map->cells);
  free(map);
    
  return 0;
}
