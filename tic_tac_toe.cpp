#include "Games.h"
#include "Console_Kids.h"

#define P1 (uint8_t) 0
#define P2 (uint8_t) 1

static void next_position(uint8_t* pos);
static void set_position(uint8_t pos, uint8_t player);
static void set_matrix_arr(uint8_t arr[8][8], uint8_t pos, uint8_t clr);
static uint8_t check_winner();
static void get_winning_positions(int8_t *positions, uint8_t winner);
static int get_position(uint8_t x, uint8_t y);

const int MAX_PLAYS = 9;
int positions_taken[3][3] = {0};

GameProgressEnum tic_tac_toe()
{
  GameStateEnum game_state = START;
  bool blinking = true;
  uint8_t position = 0;
  int plays = 0;
  uint8_t matrix_arr[MATRIX_ROWS][MATRIX_ROWS] = {0};
  int player = 0;
  int8_t winning_positions[3] = {-1};
  ButtonPressedEnum btn;
  Timer blink_timer = Timer();
  blink_timer.set_auto_restart(true);
  blink_timer.start(500);
  set_matrix_arr(matrix_arr, position, game_state == PLAYER_1 ? RED : GREEN);

  //Game loop
  while(1)
  {
    btn = getButtonPressed();
    switch (game_state)
    {
      case START:
        //Empty positions taken
        memset(positions_taken, 0, (sizeof(positions_taken[0][0])) * 3 * 3);
        memset(winning_positions, -1, sizeof(winning_positions[0]) * 3);
        //Init board
        for (int i = 0; i <MATRIX_ROWS; i++)
        {
          for (int j = 0; j < MATRIX_ROWS; j++)
          {
            if (i == 2 || j == 2 || i == 5 || j == 5)
            {
              matrix_arr[i][j] = YELLOW;
            }
            else
            {
              matrix_arr[i][j] = OFF;
            }
          }
        }
        game_state = GAMING;
        break;

      case GAMING:

        if (blink_timer.isDone())
        {
          if (blinking)
          {
            set_matrix_arr(matrix_arr, position, OFF);
          }
          else
          {
            set_matrix_arr(matrix_arr, position, player == P1 ? RED : GREEN);
          }
          blinking = !blinking;
        }

        if ((btn == BTN_A && player == P1) || (btn == BTN_C && player == P2))
        {
          set_matrix_arr(matrix_arr, position, OFF);
          blinking = true;
          blink_timer.restart();
          next_position(&position);
          set_matrix_arr(matrix_arr, position, player == P1 ? RED : GREEN);
        }

        if ((btn == BTN_B && player == P1) || (btn == BTN_D && player == P2))
        {
          set_position(position, player + 1);
          plays++;
          if (check_winner() || plays == MAX_PLAYS)
          {
            game_state = GAME_OVER;
            set_matrix_arr(matrix_arr, position, player == P1 ? RED : GREEN);
          }
          else
          {
            set_matrix_arr(matrix_arr, position, player == P1 ? RED : GREEN);
            next_position(&position);
            player = player == P1 ? P2 : P1;
          }
        }

        break;

      case GAME_OVER:
        if(check_winner() != 0)
        {
          if(winning_positions[0] == -1)
          {
            get_winning_positions(winning_positions, check_winner());
          }
          else
          {
            if (blink_timer.isDone())
            {            
              for(int i =0; i < 3; i++)
              {
                if(blinking)
                {
                  set_matrix_arr(matrix_arr, winning_positions[i], OFF);
                }
                else
                {
                  set_matrix_arr(matrix_arr, winning_positions[i], check_winner() == 1 ? RED : GREEN);
                }
              }
              blinking = !blinking;
            }
          }
        }
        if (btn == BTN_A)
        {
          return RESTART;
        }
        else if (btn == BTN_D)
        {
          return QUIT;
        }
        break;
      default: // Should never happen;
        return QUIT;
    }
    update_leds();
    set_matrix(matrix_arr);
  }
}

static void set_matrix_arr(uint8_t arr[8][8], uint8_t pos, uint8_t clr)
{
  uint8_t i = 0;
  uint8_t j = 0;
  if (pos < 3)
  {
    i = 0;
  }
  else if (pos < 6)
  {
    i = 3;
  }
  else if (pos < 9)
  {
    i = 6;
  }
  else
  {
    return;
  }

  if (pos - i == 0)
  {
    j = 6;
  }
  else if (pos - i == 1)
  {
    j = 3;
  }
  else if (pos - i == 2)
  {
    j = 0;
  }
  arr[i][j] = clr;
  arr[i + 1][j] = clr;
  arr[i][j + 1] = clr;
  arr[i + 1][j + 1] = clr;
}

static void next_position(uint8_t* pos)
{
  uint8_t i = 0;
  uint8_t j = 0;

  (*pos)++;
  if (*pos == 9)
  {
    *pos = 0;
  }
  if (*pos < 3)
  {
    i = 0;
  }
  else if (*pos < 6)
  {
    i = 1;
  }
  else if (*pos < 9)
  {
    i = 2;
  }
  j = (*pos - (i * 3));
  if (positions_taken[i][j] != 0)
  {
    return next_position(pos);
  }
}

static void set_position(uint8_t pos, uint8_t player)
{
  uint8_t i = 0;
  uint8_t j = 0;

  if (pos == 9)
  {
    pos = 0;
  }
  if (pos < 3)
  {
    i = 0;
  }
  else if (pos < 6)
  {
    i = 1;
  }
  else if (pos < 9)
  {
    i = 2;
  }
  j = (pos - (i * 3));

  positions_taken[i][j] = player;
}

static uint8_t check_winner()
{
  int last = 0;

  //Vertical
  for (int x = 0; x < 3; x++)
  {
    last = positions_taken[0][x];
    if (last != 0)
    {
      for (int i = 1; i < 3; i++)
      {
        if (last != positions_taken[i][x])
        {
          break;
        }
        else
        {
          if (i == 2)
          {
            return last;
          }
        }
      }
    }
  }
  //Horizontal
  for (int y = 0; y < 3; y++)
  {
    last = positions_taken[y][0];
    if (last != 0)
    {
      for (int i = 1; i < 3; i++)
      {
        if (last != positions_taken[y][i])
        {
          break;
        }
        else
        {
          if (i == 2)
          {
            return last;
          }
        }
      }
    }
  }

  //Diagonal
  if (positions_taken[1][1] != 0)
  {
    if (positions_taken[0][0] == positions_taken[1][1] && positions_taken[0][0] == positions_taken[2][2])
    {
      return positions_taken[1][1];
    }
    if (positions_taken[0][2] == positions_taken[1][1] && positions_taken[0][2] == positions_taken[2][0])
    {
      return positions_taken[1][1];
    }
  }
  return 0;

}

static void get_winning_positions(int8_t *positions, uint8_t winner )
{
  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < 3; j++)
    {
      if(positions_taken[i][j] == winner)
      {
        if(j == 0)
        {
          // Horizontal
          if((positions_taken[i][j+ 1] == winner) && (positions_taken[i][j+ 2] == winner))
          {
            positions[0] = get_position(i,j);
            positions[1] = get_position(i,j+1);
            positions[2] = get_position(i,j+2);
            return;
          }
        }
        if(i == 0)
        {
          //Vertical
          if((positions_taken[i +1][j] == winner) && (positions_taken[i +2][j] == winner))
          {
            positions[0] = get_position(i,j);
            positions[1] = get_position(i+1,j);
            positions[2] = get_position(i+2,j);
            return;
          }
        }
        if(j !=1)
        {
          //diagonal
          if(j == 0)
          {
            if(positions_taken[2][2] == winner && positions_taken[1][1] == winner)
            {
              positions[0] = 0;
              positions[1] = 4; //middle pos
              positions[2] = 8;
              return;
            }
          }
          else if(j == 2)
          {
            if(positions_taken[2][0] == winner && positions_taken[1][1] == winner)
            {
              positions[0] = 2;
              positions[1] = 4;
              positions[2] = 6;
              return;
            }
          }
        }
      }
    }
  }
    
}

static int get_position(uint8_t y, uint8_t x)
{
    return  x +(y*3);
}