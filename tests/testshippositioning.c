/********************************************************************************
 * Ship positioning algorithm for Battleship
 *
 * Sergio Carmine 2CIT
 * Navi by Emanuele Debeffi
 *
 * 05/04/2022
********************************************************************************/

// Graphics Width: 98 columns

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

/*******************************************************************************
 * GRAPHICS CONSTANTS */
// Graphics display cells
// 0: Sea
// 1: Opponent Hit
// 2: Opponent Miss
// 3: Ship Sunk
// 4: Single Ship
// 5: Horizontal Multi Ship Start
// 6: Horizontal Multi Ship Start Hit
// 7: Horizontal Multi Ship Middle
// 8: Horizontal Multi Ship Middle Hit
// 9: Horizontal Multi Ship End
// 10: Horizontal Multi Ship End Hit
// 11: Vertical Multi Ship Start
// 12: Vertical Multi Ship Start Hit
// 13: Vertical Multi Ship Middle
// 14: Vertical Multi Ship Middle Hit
// 15: Vertical Multi Ship End
// 16: Vertical Multi Ship End Hit
char graphics_cells[17][3] = {
  {' ', '~', ' '},
  {' ', '~', ' '},
  {' ', '~', ' '},
  {' ', '~', ' '},
  {'<', '=', '>'},
  {' ', '<', ' '},
  {' ', '<', ' '},
  {' ', '=', ' '},
  {' ', '=', ' '},
  {' ', '>', ' '},
  {' ', '>', ' '},
  {' ', 'A', ' '},
  {' ', 'A', ' '},
  {' ', 'N', ' '},
  {' ', 'N', ' '},
  {' ', 'V', ' '},
  {' ', 'V', ' '},
};
char graphics_cells_colors[17][3] = {
  {0, 1, 0},
  {4, 4, 4},
  {3, 3, 3},
  {5, 5, 5},
  {2, 2, 2},
  {0, 2, 0},
  {4, 4, 4},
  {0, 2, 0},
  {4, 4, 4},
  {0, 2, 0},
  {4, 4, 4},
  {0, 2, 0},
  {4, 4, 4},
  {0, 2, 0},
  {4, 4, 4},
  {0, 2, 0},
  {4, 4, 4},
};

// Colors:
// 0: Default
// 1: Blue
// 2: Bold White
// 3: Cyan Background Bold
// 4: Red Background Bold
// 5: Purple Background Bold
char* color_codes[6] = {"\033[0m", "\033[34m", "\033[37;1m", "\033[46;1m", "\033[41;1m", "\033[45;1m"};

// Letters for each row
char letters[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};

/******************************************************************************/

void draw_ship_select_board(int display_map[2][10][10]) {
  int row, subrow, table, column, subcolumn;
  int selected_graphics_cell, selected_color;
        
  // Clear the screen
  printf("\e[1;1H\e[2J");

  // Print graphics divider line
  printf("%s# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #\n",
      color_codes[2]);

  // Print the table title
  printf("%s#                                   ## POSIZIONA LE TUE NAVI ##                                 #%s\n",
      color_codes[2], color_codes[0]);
  // Print the legends and table row start line
  printf("%s#%s                              1   2   3   4   5   6   7   8   9   10                           %s#%s\n",
      color_codes[2], color_codes[0], color_codes[2], color_codes[0]);
  printf("%s#%s                            +---+---+---+---+---+---+---+---+---+---+                          %s#%s\n",
      color_codes[2], color_codes[0], color_codes[2], color_codes[0]);

  // Iterate on every row
  for(row = 0; row < 10; row++) {
    // Print the left column of hastags
    printf("%s# %s", color_codes[2], color_codes[0]);
    // We need to print two tables: the player ship table and the computer hits table

    // Print the left legend and the vertical table row start line
    printf("                         %c |", letters[row]);

    // Iterate on every column inside a row
    for(column = 0; column < 10; column++) {

      // Every column is divided in subcolumns (single carachters) to print
      for(subcolumn = 0; subcolumn < 3; subcolumn++) {

        // Find the correct graphic cell indexed by our position on the board 
        selected_graphics_cell = display_map[0][row][column];
        // Find the color of the subcolumn we want to print
        selected_color = graphics_cells_colors[selected_graphics_cell][subcolumn];
        // Print the column in its wanted colour
        printf("%s%c", color_codes[selected_color], 
            graphics_cells[selected_graphics_cell][subcolumn]);
        // Reset color
        printf("\033[0m");
      }

      // Print the table column end line
      printf("|");
    }

    printf("     ");

    printf("%s                     #%s", color_codes[2], color_codes[0]);
    // Print the table row end line
    printf("\n%s#%s                            +---+---+---+---+---+---+---+---+---+---+                          %s#%s",
        color_codes[2], color_codes[0], color_codes[2], color_codes[0]);
    printf("\n");
  }
  // Print graphics divider
  printf("%s# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #%s\n",
    color_codes[2], color_codes[0]);

}


/********************************************************************************
 * Fills a display map with default state
 *
 * display_map: reference to the display map that needs to be initialized
********************************************************************************/
void init_display_map(int display_map[2][10][10]) {
  int i, j, k;
  for(i = 0; i < 2; i++) {
    for(j = 0; j < 10; j++) {
      for(k = 0; k < 10; k++) {
        display_map[i][j][k] = 0; 
      }
    }
  }
}

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

bool get_input_coordinate(int* row, int* column) {
  char raw_row;
  int raw_column;
  // Get input and fail if not succesful
  if(!scanf("%c%d", &raw_row, &raw_column)) return false;

  // Flush any unwanted characters
  while (getchar() != '\n');

  // Begin checking column
  *column = raw_column - 1;
  if(*column < 0 || *column > 9) return false;

  // Then check row
  // Subtract ascii offset to get row number
  *row = raw_row - 65;
  if(*row < 0 || *row > 9) {
    // If it's not correct in uppercase, we try lowercase
    *row = raw_row - 97;
    if(*row < 0 || *row > 9) return false;
  }

  return true;
}

void position_single_ship(int display_map[2][10][10], int player_ship_map[10][10], int ship_index) {
  char raw_row;
  int raw_column;
  int row, column;
  bool input_result = true;
  bool already_positioned = false;

  while(true) {
    // Draw the ship select board
    draw_ship_select_board(display_map);
    printf("Nave da posizionare: <=>\n");
    
    // Print prompt
    if(input_result && !already_positioned) printf("Dove vuoi posizionarla? [es. D4] -> ");
    else if (!input_result) printf("La casella inserita non esiste. Riprova [es. D4] -> ");
    else printf("Hai già posizionato una nave in questa casella. Riprova [es. D4] -> ");

    // Get input from user
    input_result = get_input_coordinate(&row, &column);

    if(input_result) {
      // If we already have a ship positioned there, we pass and change the message
      if(player_ship_map[row][column] != 99) {
        already_positioned = true;
        continue;
      }

      // Position the ship on the display map and ship map
      display_map[0][row][column] = 4;
      player_ship_map[row][column] = ship_index;

      // We're done positioning
      return;
    }
  }
}

bool get_ship_direction(int *direction) {
  char raw_direction;
  // Get input and fail if not succesful
  if(!scanf("%c", &raw_direction)) return false;

  // Flush any unwanted characters
  while (getchar() != '\n');

  if(raw_direction == 'o') {
    *direction = 0;
    return true;
  } else if(raw_direction == 'v') {
    *direction = 1;
    return true;
  }

  return false;

}
void position_multi_ship(int display_map[2][10][10], int player_ship_map[10][10], int ship_index, int ship_lives) {
  char raw_row, raw_direction;
  int raw_column;
  int start_row, start_column;
  bool is_horizontal_possible, is_vertical_possible;
  // Is_X_possible: 0 -> possible, 1 -> out of board, 2 -> ship in the way
  int direction;
  // Direction: 0 -> horizontal, 1 -> vertical

  int input_result = 0;
  // Input Result: 0 -> success, 1 -> starting cell doesn't exist / ship out of board,
  //               2 -> starting cell already used / ship crosses other ship
  //               3 -> ship can't be positioned starting from this cell
  
  int i;
  
  // Get ship start position
  while(true) {
    draw_ship_select_board(display_map);
    printf("Nave da posizionare: <=> x %d\n", ship_lives);
    
    // Print prompt
    switch(input_result) {
      case 0:
        printf("Seleziona la casella di partenza [es. D4] -> ");
        break;
      case 1:
        printf("La casella inserita non esiste. Riprova [es. D4] -> ");
        break;
      case 2:
        printf("Hai già positionato una nave in questa casella. Riprova [es. D4] -> ");
        break;
      case 3:
        printf("Non puoi posizionare una nave da %d a partire da questa casella. Riprova [es. D4] -> ", ship_lives);
        break;
    }

    // Get input from user
    if(!get_input_coordinate(&start_row, &start_column)) {
      input_result = 1;
      continue;
    };

    // Verify that there isn't a ship already there
    if(player_ship_map[start_row][start_column] != 99) {
      input_result = 2;
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
      input_result = 3;
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
      // Position ship start position indicator
      display_map[0][start_row][start_column] = 2;

      // Draw the board
      draw_ship_select_board(display_map);
      printf("Nave da posizionare: <=> x %d\n", ship_lives);

      // Print prompt
      printf("La nave è orizzontale o verticale? [o, v] -> ");

      // Get input from user
      if(!get_ship_direction(&direction)) {
        input_result = 1;
        continue;
      };

      break;
    }
  }
  else if(is_horizontal_possible) direction = 0;
  else direction = 1;

  /* printf("Direction [0: horizontal, 1: vertical]: %d", direction);
  fflush(stdout);
  sleep(5); */

  // Position ship on display map and player ship map
  if(direction == 0) {
    // For horizontal ships
    for(i = 0; i < ship_lives; i++) {
      if(i == 0) display_map[0][start_row][start_column + i] = 5; // Ship Start
      else if(i == ship_lives - 1) display_map[0][start_row][start_column + i] = 9; // Ship end
      else display_map[0][start_row][start_column + i] = 7; // Ship middle
      player_ship_map[start_row][start_column + i] = ship_index;
    }
  } else {
    // For vertical ships
    for(i = 0; i < ship_lives; i++) {
      if(i == 0) display_map[0][start_row + i][start_column] = 11; // Ship Start
      else if(i == ship_lives - 1) display_map[0][start_row + i][start_column] = 15; // Ship end
      else display_map[0][start_row + i][start_column] = 13; // Ship middle
      player_ship_map[start_row + i][start_column] = ship_index;
    }
  }
}

void draw_game_info() {
  printf("%s#                                                                                               #%s\n", color_codes[2], color_codes[0]);
}

int main() {
  // Game wide maps
  int display_map[2][10][10];
  int player_ship_map[10][10];
  int player_ship_lives [10] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
  init_display_map(display_map);
  init_ship_map(player_ship_map);
  
  // Ship selection specific maps
  int current_ship;
  bool has_positioned_ship = false;
  int ship_position_row, ship_position_column;
  int input_status = 1;

  for(current_ship = 0; current_ship < 10; current_ship++) {
    if(player_ship_lives[current_ship] == 1) position_single_ship(display_map, player_ship_map, current_ship);
    else position_multi_ship(display_map, player_ship_map, current_ship, player_ship_lives[current_ship]);
    /* printf("Positioning ship %d", current_ship);
    fflush(stdout);
    sleep(1); */
  }

  printf("Premi RETURN per continuare..."); 
  while( getchar() != '\n' ); // Wait for enter
}
