#include "Games.h"
#include "Console_Kids.h"

typedef struct Point
{
  uint8_t x, y;
} Point;

static void next_position(uint8_t arr[8][8], Point *pos);
static void set_matrix_pos(uint8_t arr[8][8], Point pos, uint8_t clr);
static int8_t check_winner(uint8_t arr[8][8], Point *winning_pos);
GameProgressEnum four_in_a_row()
{
  GameStateEnum game_state = GAMING;
  Point position = {4, 7};
  Point winning_positions[4] = {{-1, -1}};
  ButtonPressedEnum btn;
  uint8_t matrix_arr[8][8] = {0};
  uint8_t plays = 0;
  bool led_on = true;
  uint8_t player = P1;
  int8_t winner = -1;
  Timer blink = Timer();
  blink.start(500);
  blink.set_auto_restart(true);
  set_matrix_pos(matrix_arr, position, RED);
  while (1)
  {
    btn = getButtonPressed();
    switch (game_state)
    {
    case GAMING:
      if ((btn == BTN_A && player == P1) || (btn == BTN_C && player == P2))
      {
        set_matrix_pos(matrix_arr, position, OFF);
        led_on = false;
        blink.restart();
        next_position(matrix_arr, &position);
        set_matrix_pos(matrix_arr, position, player == P1 ? RED : GREEN);
      }
      if ((btn == BTN_B && player == P1) || (btn == BTN_D && player == P2))
      {
        set_matrix_pos(matrix_arr, position, player == P1 ? RED : GREEN);
        plays++;
        winner = check_winner(matrix_arr, winning_positions);
        if ( winner != -1 || plays == PLAYS_MAX_4_IN_A_ROW)
        {
          game_state = GAME_OVER;
          set_matrix_pos(matrix_arr, position, player == P1 ? RED : GREEN);
        }
        else
        {
          set_matrix_pos(matrix_arr, position, player == P1 ? RED : GREEN);
          if (position.y != 0)
          {
            position.y--;
          }
          else
          {
            next_position(matrix_arr, &position);
          }

          player = player == P1 ? P2 : P1;
        }
      }
      if (blink.isDone())
      {
        if (led_on)
        {
          set_matrix_pos(matrix_arr, position, OFF);
        }
        else
        {
          set_matrix_pos(matrix_arr, position, player == P1 ? RED : GREEN);
        }
        led_on = !led_on;
      }
      break;
    case GAME_OVER:
      if (!blink.get_started())
      {
        blink.start(500);
        led_on = true;
      }
      if (blink.isDone())
      {
        if (led_on)
        {
          for (int i = 0; i < 4; i++)
          {
            set_matrix_pos(matrix_arr, winning_positions[i], OFF);
          }
        }
        else if (winner == -1)
        {
          for (int i = 0; i < 4; i++)
          {
            set_matrix_pos(matrix_arr, winning_positions[i], YELLOW);
          }
        }
        else
        {
          for (int i = 0; i < 4; i++)
          {
            set_matrix_pos(matrix_arr, winning_positions[i], winner == P1 ? RED : GREEN);
          }
        }
        led_on = !led_on;
      }
      if (btn == BTN_D)
      {
        return QUIT;
      }
      if (btn != NONE)
      {
        return RESTART;
      }
      break;
    default: // Should never happen
      return QUIT;
      break;
    }
    update_leds();
    set_matrix(matrix_arr);
  }
  return CONTINUE;
}

static void next_position(uint8_t arr[8][8], Point *pos)
{
  //if we're at the end of the collumns
  if ((*pos).x == 0)
  {
    //go back to the start
    (*pos).x = 7;
  }
  else
  {
    //just decrease it
    (*pos).x--;
  }
  //If the collumn is full, we'll look for another position
  if (arr[0][(*pos).x] != 0)
  {
    return next_position(arr, pos);
  }
  else
  {
    //look for the lowest row possible
    for (int y = 7; y >= 0; y--)
    {
      if (arr[y][(*pos).x] == 0)
      {
        (*pos).y = y;
        return;
      }
    }
  }
}

static void set_matrix_pos(uint8_t arr[8][8], Point pos, uint8_t clr)
{
  arr[(pos).y][(pos).x] = clr;
}
static int8_t check_winner(uint8_t arr[8][8], Point *winning_pos)
{
  int first;
  int in_a_row = 0;
  //Horizontal
  for (int y = 7; y >= 0; y--)
  {
    in_a_row = 0;
    first = arr[y][0];
    if (first != 0)
    {
      in_a_row++;
    }
    for (int x = 1; x < 8; x++)
    {
      if (arr[y][x] != 0)
      {
        if (first != 0)
        {
          if (arr[y][x] == first)
          {
            if (in_a_row >= 0)
            {
              in_a_row++;
            }
            else
            {
              in_a_row = 1;
            }
          }
          else
          {
            if (in_a_row >= 0)
            {
              in_a_row = -1;
            }
            else
            {
              in_a_row--;
            }
          }
        }
        else
        {
          first = arr[y][x];
          in_a_row = 1;
        }
      }
      else
      {
        in_a_row = 0;
      }
      if (in_a_row == 4)
      {
        winning_pos[0] = {x, y};
        winning_pos[1] = {x - 1, y};
        winning_pos[2] = {x - 2, y};
        winning_pos[3] = {x - 3, y};
        return first == RED ? P1 : P2;
      }
      else if (in_a_row == -4)
      {
        winning_pos[0] = {x, y};
        winning_pos[1] = {x - 1, y};
        winning_pos[2] = {x - 2, y};
        winning_pos[3] = {x - 3, y};
        return first == RED ? P2 : P1;
      }
    }
  }
  //Vertical
  for (int x = 0; x < 8; x++)
  {
    in_a_row = 0;
    first = arr[7][x];
    if (first != 0)
    {
      in_a_row++;
    }
    for (int y = 6; y >= 0; y--)
    {
      if (arr[y][x] != 0)
      {
        if (first != 0)
        {
          if (arr[y][x] == first)
          {
            if (in_a_row >= 0)
            {
              in_a_row++;
            }
            else
            {
              in_a_row = 1;
            }
          }
          else
          {
            if (in_a_row >= 0)
            {
              in_a_row = -1;
            }
            else
            {
              in_a_row--;
            }
          }
        }
        else
        {
          first = arr[y][x];
          in_a_row = 1;
        }
      }
      else
      {
        in_a_row = 0;
      }
      if (in_a_row == 4)
      {
        winning_pos[0] = {x, y};
        winning_pos[1] = {x, y + 1};
        winning_pos[2] = {x, y + 2};
        winning_pos[3] = {x, y + 3};
        return first == RED ? P1 : P2;
      }
      else if (in_a_row == -4)
      {
        return first == RED ? P2 : P1;
      }
    }
  }
  //Diagonal
  for (int y = 3; y < 5; y++)
  {
    for (int x = 0; x < 8; x++)
    {
      first = arr[y][x];
      if (first != 0)
      {
        in_a_row = 1;
        //m > 0
        for (int i = 1; i < 4; i++)
        {
          if ((x - i) < 0)
          {
            break;
          }
          else if (arr[y + i][x - i] == first)
          {
            in_a_row++;
          }
          else
          {
            i = 4;
          }
        }
        for (int i = 1; i < 4; i++)
        {
          if ((x + i) > 8)
          {
            break;
          }
          else if (arr[y - i][x + i] == first)
          {
            in_a_row++;
          }
          else
          {
            i = 4;
          }
        }

        if (in_a_row >= 4)
        {
          return first == RED ? P1 : P2;
        }
        //m < 0
        in_a_row = 1;
        for (int i = 1; i < 4; i++)
        {
          if ((x + i) > 8)
          {
            break;
          }
          else if (arr[y + i][x + i] == first)
          {
            in_a_row++;
          }
          else
          {
            i = 4;
          }
        }
        for (int i = 1; i < 4; i++)
        {
          if ((x - i) < 0)
          {
            break;
          }
          else if (arr[y - i][x - i] == first)
          {
            in_a_row++;
          }
          else
          {
            i = 4;
          }
        }

        if (in_a_row >= 4)
        {
          return first == RED ? P1 : P2;
        }
      }
    }
  }
  return -1;
}