#include "Games.h"
#include "Console_Kids.h"

static void set_matrix_arr(int pos, int clr);
static void next_position(int* pos);
static void set_position(int pos, int player);
static int checkWinner();
const int MAX_PLAYS = 9;

int matrix_arr[8][8] =
{
  {0, 0, YELLOW, 0, 0, YELLOW, 0, 0},
  {0, 0, YELLOW, 0, 0, YELLOW, 0, 0},
  {YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW},
  {0, 0, YELLOW, 0, 0, YELLOW, 0, 0},
  {0, 0, YELLOW, 0, 0, YELLOW, 0, 0},
  {YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW},
  {0, 0, YELLOW, 0, 0, YELLOW, 0, 0},
  {0, 0, YELLOW, 0, 0, YELLOW, 0, 0}
};
int positions_taken[3][3] =
{
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};
GameProgressEnum tic_tac_toe(ButtonPressedEnum btn)
{
  static GameStateEnum game_state = START;
  static bool blinking = true;
  static int position = 0;
  static Timer blink_timer = Timer();
  static int plays = 0;
  if (!blink_timer.get_set())
  {
    Serial.println("start timer");
    blink_timer.start(500);
    blink_timer.set_auto_restart(true);
    set_matrix_arr(position, game_state == PLAYER_1 ? RED : GREEN);
  }
  if (blink_timer.isDone() && game_state != GAME_OVER)
  {
    if (blinking)
    {
      set_matrix_arr(position, OFF);
    }
    else
    {
      set_matrix_arr(position, game_state == PLAYER_1 ? RED : GREEN);
    }

    blinking = !blinking;
  }
  switch (game_state)
  {
    case START:
      plays = 0;
      memset(positions_taken, 0, sizeof(positions_taken[0][0]) * 3 * 3);
      for (int i = 0; i < 8; i++)
      {
        for (int j = 0; j < 8; j++)
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
      position = 0;
      game_state = PLAYER_1;
      break;
    case PLAYER_1:
      if (btn == BTN_A)
      {
        set_matrix_arr(position, OFF);
        blinking = true;
        blink_timer.restart();
        next_position(&position);
        set_matrix_arr(position, RED);
      }
      if (btn == BTN_B)
      {
        set_position(position, 1);
        plays++;
        if (check_winner() || plays == MAX_PLAYS)
        {
          game_state = GAME_OVER;
          set_matrix_arr(position, RED);
        }
        else
        {
          set_matrix_arr(position, RED);
          next_position(&position);
          game_state = PLAYER_2;
        }
      }
      break;
    case PLAYER_2:
      if (btn == BTN_C)
      {
        set_matrix_arr(position, OFF);
        blinking = true;
        blink_timer.restart();
        next_position(&position);
        set_matrix_arr(position, GREEN);
      }
      if (btn == BTN_D)
      {
        set_position(position, 2);
        plays++;
        if (check_winner() || plays == MAX_PLAYS)
        {

          game_state = GAME_OVER;
        }
        else
        {
          set_matrix_arr(position, GREEN);
          next_position(&position);
          game_state = PLAYER_1;
        }
      }
      break;
    case GAME_OVER:
      if (btn == BTN_A)
      {
        game_state = START;
      }
      else if (btn == BTN_B || btn == BTN_C)
      {
        memset(matrix_arr, check_winner() == 1 ? RED : GREEN, sizeof(matrix_arr[0][0] * 8 * 8));
      }
      else if (btn == BTN_D)
      {
        game_state = START;
        return QUIT;
      }
      Serial.println(check_winner());
      break;
  }
  set_matrix(matrix_arr);
}

static void set_matrix_arr(int pos, int clr)
{
  int i = 0;
  int j = 0;
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
  matrix_arr[i][j] = clr;
  matrix_arr[i + 1][j] = clr;
  matrix_arr[i][j + 1] = clr;
  matrix_arr[i + 1][j + 1] = clr;
}
static void next_position(int* pos)
{
  int i = 0;
  int j = 0;

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
static void set_position(int pos, int player)
{
  int i = 0;
  int j = 0;

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
  Serial.print("Set: ");
  Serial.print(i);
  Serial.print(" ");
  Serial.println(j);
}

static int check_winner()
{
  int last = 0;
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
