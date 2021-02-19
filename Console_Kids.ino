#include "Console_Kids.h"
#include "Timer.h"
#include "Games.h"

void setup()
{
  init_();
}

GameProgressEnum (*game_func_arr[])(void) = {tic_tac_toe, four_in_a_row, sequence, reaction};
void loop()
{
  static uint8_t matrix_arr[8][8] = {0};
  static ConsoleStateEnum state = SELECTING;
  static uint8_t game_index = 0;
  ButtonPressedEnum btn = NONE;

  btn = getButtonPressed();
  switch (state)
  {
  case SELECTING:
    led_arr[game_index]->setOn();
    if (btn == BTN_A) //Start game
    {
      state = PLAYING;
      led_arr[game_index]->setOff();
    }
    else if (btn == BTN_B || btn == BTN_C) //previous game
    {
      led_arr[game_index]->setOff();
      if (game_index == 3)
      {
        game_index = 0;
      }
      else
      {
        game_index++;
      }
      led_arr[game_index]->setOn();
    }

    else if (btn == BTN_D) //next game
    {
    }

    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        matrix_arr[i][j] = GAME_PREVIEW_ARR[game_index][i][j];
      }
    }
    break;
  case PLAYING:
    Serial.print("gaming: ");
    Serial.println(game_index);
    if ((*game_func_arr[game_index])() == QUIT)
    {
      state = SELECTING;
    }
    break;
  default:
    break;
  }

  /*
     Update every led to see if it's time to turn it off
     Complements the use of setOn(duration)
  */
  update_leds();
  set_matrix(matrix_arr);
}
