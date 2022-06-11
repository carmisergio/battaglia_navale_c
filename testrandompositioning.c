/********************************************************************************
 * Ship positioning algorithm for Battleship
 *
 * Sergio Carmine 2CIT
 * Navi by Emanuele Debeffi
 *
 * 21/04/2022
********************************************************************************/

// Graphics Width: 98 columns

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

/********************************************************************************
 * Fills a ship map with default state
 *
 * ship_map: reference to the ship map that needs to be initialized
********************************************************************************/
void init_ship_map(int ship_map[10][10]) {
  int i, j;
  for(i = 0; i < 10; i++) {
    for(j = 0; j < 10; j++) {
      ship_map[i][j] = 99; 
    }
  }
}

void position_random_single_ship(int player_ship_map[10][10], int ship_index) {
  int row, column;
  bool already_positioned = false;

  while(true) {
    // Get random position
    row = (rand() % 10);
    column = (rand() % 10);

    // If we already have a ship positioned there, we pass and change the message
    if(player_ship_map[row][column] != 99) {
      already_positioned = true;
      continue;
    }

    // Position the ship on the display map and ship map
    player_ship_map[row][column] = ship_index;

    // We're done positioning
    return;
  }
}

void position_random_multi_ship(int player_ship_map[10][10], int ship_index, int ship_lives) {
  int start_row, start_column;
  bool is_horizontal_possible, is_vertical_possible;
  // Is_X_possible: 0 -> possible, 1 -> out of board, 2 -> ship in the way
  int direction;
  // Direction: 0 -> horizontal, 1 -> vertical

  int i;
  
  // Get ship start position
  while(true) {
    // Get random position
    start_row = (rand() % 10);
    start_column = (rand() % 10);

    // Verify that there isn't a ship already there
    if(player_ship_map[start_row][start_column] != 99) {
      continue;
    }

    // Verify that we can fit a ship there vertically and horizontally
    is_vertical_possible = true;
    is_horizontal_possible = true;
    for(i = 0; i < ship_lives; i++) {
      // Check if cell is inside board boundaries
      if(start_row + i >= 10) {
        is_vertical_possible = false;
        break;
      }
      // Check if cell is available
      if(player_ship_map[start_row + i][start_column] != 99)
        is_vertical_possible = false;
    }
    for(i = 0; i < ship_lives; i++) {
      // Check if cell is inside board boundaries
      if(start_column + i >= 10) {
        is_horizontal_possible = false;
        break;
      }
      // Check if cell is available
      if(player_ship_map[start_row][start_column + i] != 99) 
        is_horizontal_possible = false;
    }

    // If we cant' place the ship either horizontally or vertically
    if(!is_vertical_possible && !is_horizontal_possible) {
      continue;
    }


    // Exit from the loop
    break;
  }
  
  // We need to ask the user for a position only if the ship can be placed
  // both horizontally and vertically, else we set it to what's possible
  if(is_horizontal_possible && is_vertical_possible) {
    // Get ship direction
    while(true) {


      // Get random direction
      direction = (rand() % 1);

      break;
    }
  }
  else if(is_horizontal_possible) direction = 0;
  else direction = 1;

  /* printf("Direction [0: horizontal, 1: vertical]: %d", direction);
  fflush(stdout);
  sleep(5); */

  // Position ship on display map and player ship map
  if(direction == 0)
    // For horizontal ships
    for(i = 0; i < ship_lives; i++) {
      player_ship_map[start_row][start_column + i] = ship_index;
    }
  else {
    // For vertical ships
    for(i = 0; i < ship_lives; i++) {
      player_ship_map[start_row + i][start_column] = ship_index;
    }
  }
}

void position_random_ships(int ship_map[10][10], int ship_lives[10]) {
  int current_ship;

  for(current_ship = 0; current_ship < 10; current_ship++) {
    if(ship_lives[current_ship] == 1) position_random_single_ship(ship_map, current_ship);
    else position_random_multi_ship(ship_map, current_ship, ship_lives[current_ship]);
  }

}

int main() {
  // Game wide maps
  int computer_ship_map[10][10];
  int computer_ship_lives [10] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
  int i, j;

  // Init random nuber generator
  srand(time(0));
  init_ship_map(computer_ship_map);
  
  // Position the computer's ships
  position_random_ships(computer_ship_map, computer_ship_lives);

  for(i = 0; i < 10; i++) {
    for (j = 0; j < 10; j++) {
      printf("%-2d ", computer_ship_map[i][j]);
    }
    printf("\n");
  }
}
