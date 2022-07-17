/********************************************************************************
 * Test main game logic for battleship
 *
 * Sergio CarmSine 2CIT
 * Navi by Emanuele Debeffi
 *
 * 04/05/2022
********************************************************************************/

// Graphics Width: 98 columns

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

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

/********************************************************************************
 * Draws the ship select phase board to screen
 * Parameters:
 *  - display_map: reference to the display map to be drawn
********************************************************************************/
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
        // Print the subcolumn in its wanted colour
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
 * Parameters:
 *  - display_map: reference to the display map that needs to be initialized
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
 * Parameters
 *  - ship_map: reference to the ship map that needs to be initialized
********************************************************************************/
void init_ship_map(int ship_map[10][10]) {
  int i, j;
  for(i = 0; i < 10; i++) {
    for(j = 0; j < 10; j++) {
      ship_map[i][j] = 99; 
    }
  }
}

/********************************************************************************
 * Fills a hit map with default state
 * Parameters
 *  - hit_map: reference to the hit map that needs to be initialized
********************************************************************************/
void init_hit_map(bool hit_map[10][10]) {
  int i, j;
  for(i = 0; i < 10; i++) {
    for(j = 0; j < 10; j++) {
      hit_map[i][j] = false; 
    }
  }
}

/********************************************************************************
 * Gets coordinate as input from the user
 * Parameters
 *  - row: reference to the numerical index of the row the user has inputed
 *  - column: reference to the numerical index of the column 
 *  		  the user has inputed
 * Returns: true if succesful
********************************************************************************/
bool get_input_coordinate(int* row, int* column) {
  char raw_row;
  int raw_column, tmp_row, tmp_column;
  // Get input and fail if not succesful
  if(!scanf("%c%d", &raw_row, &raw_column)) return false;

  // Flush any unwanted characters
  while (getchar() != '\n');

  // Begin checking column
  tmp_column = raw_column - 1;
  if(tmp_column < 0 || tmp_column > 9) return false;

  // Then check row
  // Subtract ascii offset to get row number
  tmp_row = raw_row - 65;
  if(tmp_row < 0 || tmp_row > 9) {
    // If it's not correct in uppercase, we try lowercase
    tmp_row = raw_row - 97;
    if(tmp_row < 0 || tmp_row > 9) return false;
  }

  // Everything was succesful, so we set our output variables
  *row = tmp_row;
  *column = tmp_column;

  return true;
}

/********************************************************************************
 * Get ship direcion as input from user
 * Parameters
 *  - direction: reference to the direction [0: horizontal, 1: vertical]
 * Returns: true if succesful
********************************************************************************/
bool get_ship_direction(int *direction) {
  char raw_direction;
  // Get input and fail if not succesful
  if(!scanf("%c", &raw_direction)) return false;

  // Flush any unwanted characters
  while (getchar() != '\n');

  // See if inputed carachter is valid
  if(raw_direction == 'o') {
    *direction = 0;
    return true;
  } else if(raw_direction == 'v') {
    *direction = 1;
    return true;
  }

  return false;
}

/********************************************************************************
 * Handle positioning for a single width ship
 * Parameters
 *  - display_map: reference to the display map on which to save the ship
 *  - player_ship_map: reference to the player's ship map
 *  - ship_index: index of the ship to be positioned
********************************************************************************/
void position_single_ship(int display_map[2][10][10], int player_ship_map[10][10], int ship_index) {
  int row, column;
  bool input_result = true; // Was the input succesful?
  bool already_positioned = false;	// Is there already a ship here?

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

/********************************************************************************
 * Handle positioning for a multiple width ship
 * Parameters
 *  - display_map: reference to the display map on which to save the ship
 *  - player_ship_map: reference to the player's ship map
 *  - ship_index: index of the ship to be positioned
 *  - ship_lives: dimension of the ship
********************************************************************************/
void position_multi_ship(int display_map[2][10][10], int player_ship_map[10][10], int ship_index, int ship_lives) {
  int start_row, start_column;
  bool is_horizontal_possible, is_vertical_possible; // Is_X_possible: 0 -> possible, 
                                                     // 1 -> out of board, 2 -> ship in the way
  int direction;	// Direction: 0 -> horizontal, 1 -> vertical

  int input_result = 0;	// Input Result: 0 -> success, 1 -> starting cell doesn't exist / ship out of board,
						//               2 -> starting cell already used / ship crosses other ship
						//               3 -> ship can't be positioned starting from this cell
  
  int i;	// Counter
  
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
	// Check vertical
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
	// Check horizontal
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
  else if(is_horizontal_possible) direction = 0;	// Only horizontal is possible
  else direction = 1;	// Only vertical is possible

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

/********************************************************************************
 * Handle ship poisitioning phase
 * Parameters
 *  - display_map: reference to the display map on which to save the ships
 *  - player_ship_map: reference to the player's ship map
 *  - player_ship_lives: reference to the array of the amount of lives
 *  					 each ship has
********************************************************************************/
void ship_positioning_stage(int display_map[2][10][10], int player_ship_map[10][10], int player_ship_lives[10]) {
  int current_ship;

  // Iterate on every ship
  for(current_ship = 0; current_ship < 10; current_ship++) {
    // Use the appropriate positioning function for single and mutiple width ships
    if(player_ship_lives[current_ship] == 1) position_single_ship(display_map, player_ship_map, current_ship);
    else position_multi_ship(display_map, player_ship_map, current_ship, player_ship_lives[current_ship]);
  }
}


/********************************************************************************
 * Randomly position a single width ship
 * Parameters
 *  - ship_map: reference to the player's ship map
 *  - ship_index: index of the ship to position
********************************************************************************/
void position_random_single_ship(int ship_map[10][10], int ship_index) {
  int row, column;
  bool already_positioned = false;

  while(true) {
    // Get random position
    row = (rand() % 10);
    column = (rand() % 10);

    // If we already have a ship positioned there, we pass and change the message
    if(ship_map[row][column] != 99) {
      already_positioned = true;
      continue;
    }

    // Position the ship on the display map and ship map
    ship_map[row][column] = ship_index;

    // We're done positioning
    return;
  }
}

/********************************************************************************
 * Randomly position a multiple width ship
 * Parameters
 *  - ship_map: reference to the player's ship map
 *  - ship_index: index of the ship to position
********************************************************************************/
void position_random_multi_ship(int ship_map[10][10], int ship_index, int ship_lives) {
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
    if(ship_map[start_row][start_column] != 99) {
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
      if(ship_map[start_row + i][start_column] != 99)
        is_vertical_possible = false;
    }
    for(i = 0; i < ship_lives; i++) {
      // Check if cell is inside board boundaries
      if(start_column + i >= 10) {
        is_horizontal_possible = false;
        break;
      }
      // Check if cell is available
      if(ship_map[start_row][start_column + i] != 99) 
        is_horizontal_possible = false;
    }

    // If we cant' place the ship either horizontally or vertically
    if(!is_vertical_possible && !is_horizontal_possible) {
      continue;
    }


    // Exit from the loop
    break;
  }
  
  // We need to randomly select an orientation only if both vertical and horizontal are possible
  // both horizontally and vertically, else we set it to what's possible
  if(is_horizontal_possible && is_vertical_possible) {
    // Get ship direction
    while(true) {

      // Get random direction
      direction = (rand() % 2);

      break;
    }
  }
  else if(is_horizontal_possible) direction = 0; // Only horizontal is possible
  else direction = 1; // Only vertical is possible


  // Position ship on ship map
  if(direction == 0)
    // For horizontal ships
    for(i = 0; i < ship_lives; i++) {
      ship_map[start_row][start_column + i] = ship_index;
    }
  else {
    // For vertical ships
    for(i = 0; i < ship_lives; i++) {
      ship_map[start_row + i][start_column] = ship_index;
    }
  }
}

/********************************************************************************
 * Handle random positioning for the computer's ships
 * Parameters
 *  - ship_map: reference to the computer's ship map
 *  - ship_lives: array containing the lives of ships to be positioned
********************************************************************************/
void position_random_ships(int ship_map[10][10], int ship_lives[10]) {
  int current_ship;

  // Iterate on every ship
  for(current_ship = 0; current_ship < 10; current_ship++) {
    // Use appropriate function for single and multiple witdth ship
    if(ship_lives[current_ship] == 1) position_random_single_ship(ship_map, current_ship);
    else position_random_multi_ship(ship_map, current_ship, ship_lives[current_ship]);
  }

}

/********************************************************************************
 * Draws the main game board to screen
 * Parameters:
 *  - display_map: reference to the display map to be drawn
********************************************************************************/
void draw_game_board(int display_map[2][10][10]) {
  int row, subrow, table, column, subcolumn;
  int selected_graphics_cell, selected_color;
  // We only print the second table if the game has started (We are not in ship select mode)
        
  // Clear the screen
  printf("\e[1;1H\e[2J");

  // Print graphics divider line
  printf("%s# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #\n",
      color_codes[2]);

  // Print the table titles
  printf("#                    ## YOU ##                                       ## COMPUTER ##             #\n");
  // Print the legends and table row start line
  printf("#%s      1   2   3   4   5   6   7   8   9   10           1   2   3   4   5   6   7   8   9   10  %s#\n",
      color_codes[0], color_codes[2]);
  printf("#%s    +---+---+---+---+---+---+---+---+---+---+        +---+---+---+---+---+---+---+---+---+---+ %s#%s\n",
      color_codes[0], color_codes[2], color_codes[0]);

  // Iterate on every row
  for(row = 0; row < 10; row++) {
    // Print the left column of hastags
    printf("%s# %s", color_codes[2], color_codes[0]);
    // We need to print two tables: the player ship table and the computer hits table
    for(table = 0; table < 2; table++) {

      // Print the left legend and the vertical table row start line
      printf(" %c |", letters[row]);

      // Iterate on every column inside a row
      for(column = 0; column < 10; column++) {

        // Every column is divided in subcolumns (single carachters) to print
        for(subcolumn = 0; subcolumn < 3; subcolumn++) {

          // Find the correct graphic cell indexed by our position on the board 
          selected_graphics_cell = display_map[table][row][column];
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

      // Print space between tables
      if(table == 0) printf("     ");
    }

    printf("%s #%s", color_codes[2], color_codes[0]);
    // Print the table row end line
    printf("\n%s#    +---+---+---+---+---+---+---+---+---+---+        +---+---+---+---+---+---+---+---+---+---+ #%s", color_codes[2], color_codes[0]);
    printf("\n");
  }
  // Print graphics divider
  printf("%s# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #%s\n",
    color_codes[2], color_codes[0]);
}

/********************************************************************************
 * Let player make a move
 * Parameters
 *  - display_map: reference to the display map on which to save the ships
 *  - player_ship_map: reference to the player's ship map
 *  - player_ship_lives: reference to the array of the amount of lives
 *  					 each ship has
 * Returns: 0 for miss, 1 for hit, 2 for hit and sunk
********************************************************************************/
int player_turn(int display_map[2][10][10], int computer_ship_map[10][10], int computer_ship_lives[10], bool player_hit_map[10][10]) {
  int hit_row, hit_column;
  bool is_horizontal_possible, is_vertical_possible; // Is_X_possible: 0 -> possible, 
                                                     // 1 -> out of board, 2 -> ship in the way

  int input_result = 0;	// Input Result: 0 -> success, 1 -> hit cell doesn't exist / hit out of board,
						//               2 -> hit cell has already been hit

  int ship_hit_index;
  
  int i;	// Counter
  
  // Get ship start position
  while(true) {
    draw_game_board(display_map);
    printf("Fai la tua mossa.\n");
    
    // Print prompt
    switch(input_result) {
      case 0:
        printf("Seleziona la casella ce vuoi colpire [es. D4] -> ");
        break;
      case 1:
        printf("La casella inserita non esiste. Riprova [es. D4] -> ");
        break;
      case 2:
        printf("Hai già colpito questa casella. Riprova [es. D4] -> ");
        break;
    }

    // Get input from user
    if(!get_input_coordinate(&hit_row, &hit_column)) {
      input_result = 1;
      continue;
    };

    // Verify that the player hasn't already hit there
    if(player_hit_map[hit_row][hit_column]) {
      input_result = 2;
      continue;
    }
    break;
   }

		player_hit_map[hit_row][hit_column] = true;

		// See if we hit a ship
		ship_hit_index = computer_ship_map[hit_row][hit_column];
		if(ship_hit_index != 99) {
		display_map[1][hit_row][hit_column] = 1; // Update display map
		computer_ship_lives[ship_hit_index]--; // Decrement lives of the ship we hit
		if(computer_ship_lives[ship_hit_index] == 0) return 2; // The ship has no more lives left (ship sank)
		return 1; // Report normal hit
		}
		else {
		display_map[1][hit_row][hit_column] = 2; // Update display map
		return 0;
		}

}

/********************************************************************************
 * Checks if one of the parties has won
 * Parameters:
 *  - player_ship_lives: reference to the player's ships' lives
 *  - computer_ship_lives: reference to the computer's ships' lives
 * Returns: 0 if noone has won, 1 if the player has won,
 *          2 if the computer has won
********************************************************************************/
int check_for_victory(int player_ship_lives[10], int computer_ship_lives[10]) {
  bool has_player_won = true, has_computer_won = true;
  int i, j;
  // Check if the player has won
  for(i = 0; i < 10; i++) {
    if(computer_ship_lives > 0) { // If the ship still has lives
      has_player_won = false;
	  break;
	}
  }

  // If the player has won, we dont even check the computer
  if(has_player_won) return 1;

  // Check if the computer has won
  for(i = 0; i < 10; i++) {
    if(player_ship_lives > 0) { // If the ship still has lives
      has_computer_won = false;
	  break;
	}
  }
  // If the computer has won
  if(has_computer_won) return 2;

  // If noone has won
  return 0;
}


/********************************************************************************
 * Handle main game logic
 * Parameters:
 *  - display_map: reference to the display map to be drawn
 * Returns: true if the player has won
********************************************************************************/
int main_game(int display_map[2][10][10], int player_ship_map[10][10], bool player_hit_map[10][10], int player_ship_lives[10], int computer_ship_map[10][10], bool computer_hit_map[10][10], int computer_ship_lives[10]) {
  bool game_running = true, is_player_turn = true;
  int turn_result, has_someone_won;
  while(game_running) {
    if(is_player_turn) {
     turn_result = player_turn(display_map, computer_ship_map, computer_ship_lives, player_hit_map);
	 switch(turn_result) {
       case 0:
			   printf("Mancato!");
			   break;
       case 1:
			   printf("Colpito!");
			   break;
       case 2:
			   printf("Colpito e affondato!");
			   break;
       }
	}
	fflush(stdout);
	 sleep(1);


	 // Check if someone has won
    has_someone_won = check_for_victory(player_ship_lives, computer_ship_lives);

	if(has_someone_won == 1) return true; // The player has won
	else if( has_someone_won == 2) return false; // The computer has won
  }
  return false;
}

/********************************************************************************
 * Print a ship map for debug porpuses
 * Parameters:
 *  - ship_map: reference to the ship_map to be printed
********************************************************************************/
void debug_ship_map(int ship_map[10][10]) {
  int i, j;
  for(i = 0; i < 10; i++) {
    for (j = 0; j < 10; j++) {
      printf("%-2d ", ship_map[i][j]);
    }
    printf("\n");
  }
}

/********************************************************************************
 * Main function
********************************************************************************/
int main() {
  // Game wide maps
  int display_map[2][10][10];
  int player_ship_map[10][10];
  bool player_hit_map[10][10];
  int player_ship_lives [10] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
  int computer_ship_map[10][10];
  bool computer_hit_map[10][10];
  int computer_ship_lives [10] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};

  // Initialize maps
  init_display_map(display_map);
  init_ship_map(player_ship_map);
  init_ship_map(computer_ship_map);
  init_hit_map(player_hit_map);
  init_hit_map(computer_hit_map);

  // Init random nuber generator
  srand(time(0));

  // Position player ships
  ship_positioning_stage(display_map, player_ship_map, player_ship_lives);

  // Position the computer's ships
  position_random_ships(computer_ship_map, computer_ship_lives);

  debug_ship_map(computer_ship_map);
  sleep(10);

  // Start main game
  bool game_result = main_game(display_map, player_ship_map, player_hit_map, player_ship_lives, computer_ship_map, computer_hit_map, computer_ship_lives);

  if(game_result) printf("Complimenti, hai vinto!");
  else printf("Hai perso, riprova!");
}
