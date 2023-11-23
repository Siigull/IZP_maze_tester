#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

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
    
  map->cells = malloc(sizeof(unsigned char) * rows * cols);
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
  Cell cell = {0, 0};
  
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

bool generate_perfect_from_full(Map *map, int row, int col, bool **vis) {
  if (is_outside(map, row, col) || vis[row][col]) return false;

  vis[row][col] = true;
   
  bool rotated = ((row + col) % 2) == 0;

  int moves[] = {1, 2, 4};
  shuffle(moves, 3);

  for(int i=0; i < 3; i++) {

    Cell cell = get_move_cell(moves[i], rotated);

    bool x = generate_perfect_from_full(map, row+cell.row, col+cell.col, vis);
           
    if(x) {
      map->cells[row*map->cols + col] &= ~moves[i];
      map->cells[(row + cell.row)*map->cols + col + cell.col] &= ~(moves[i] == 4? 4 : moves[i] ^ 0b11);
    }
      
  }

  return true;
}


int main(int argc, char** argv) {
  srand(time(NULL));

  if(argc != 3) {
    printf("bad number of args\n");
    return 0;
  }
  
  Map* map = init_map(atoi(argv[1]), atoi(argv[2]));

  bool** vis = calloc(map->rows, sizeof(bool*));
  for(int i=0; i < map->rows; i++) {
    vis[i] = calloc(map->cols, sizeof(bool));
  }

  generate_full(map);
  generate_perfect_from_full(map, 0, 0, vis);

  map->cells[0] &= ~1;
  map->cells[(map->rows - 1) * map->cols + map->cols - 1] &= ~2;

  print_map(map);
  
  return 0;
}
