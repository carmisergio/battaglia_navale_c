/********************************************************************************
 * First graphics test for Battleship 
 *
 * Sergio Carmine 2CIT
 * Navi by Emanuele Debeffi
 *
 * 28/03/2022
********************************************************************************/

// Graphics Width: 98 columns

#include <stdio.h>
#include <stdbool.h>

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
  {3, 3, 3},
  {4, 4, 4},
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

void draw_welcome_screen() {

}

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

void draw_game_info() {
  printf("%s#                                                                                               #%s\n", color_codes[2], color_codes[0]);
}

int main() {
  int input_row;
  // 0 -> Blank cell
  // 1 -> Sea
  // 2 -> Single ship
  int display_map[2][10][10] = {
    {
      // Player ships display map
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 3, 0, 0, 2, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 10, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 12, 0},
      {0, 0, 0, 0, 1, 0, 0, 0, 14, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 4, 6, 6, 9, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 4, 6, 8, 0, 0, 0},
      {0, 0, 0, 0, 0, 3, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    // Hits display map
    {
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },

  };

  draw_ship_select_board(display_map);
  draw_game_info();

  printf("Premi RETURN per continuare..."); 
  while( getchar() != '\n' ); // Wait for enter

  while(1) {
    draw_game_board(display_map);

    printf("Select a cell to colour -> ");
    scanf("%d", &input_row);

    display_map[1][0][input_row]++;
  }

}

