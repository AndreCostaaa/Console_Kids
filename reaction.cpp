#include "Games.h"
#include "Console_Kids.h"

static bool getClapped();
static void set_matrix_arr(uint8_t matrix_arr[8][8], GameModeEnum mode, int8_t data);
GameProgressEnum reaction()
{
  GameStateEnum game_state = SET_GAME_MODE;
  unsigned long start_time = 0;
  unsigned long reaction_time = 0;
  unsigned long reaction_time_p1 = 0;
  unsigned long reaction_time_p2 = 0;
  int winner = 0;
  int winner_time = 0;
  GameModeEnum game_mode = ONE_PLAYER;
  Timer start_timer = Timer();
  ButtonPressedEnum btn;
  uint8_t matrix_arr[8][8] = {0};
  while (1)
  {
    btn = getButtonPressed();
    switch (game_state)
    {
    case SET_GAME_MODE:
      led_green.setOn();
      led_red.setOn();
      switch (game_mode)
      {
      case ONE_PLAYER:
        led_green.setOn();
        led_red.setOn();
        led_yellow.setOff();
        led_blue.setOff();
        break;
      case TWO_PLAYERS:
        led_yellow.setOn();
        led_blue.setOn();
        break;
      }
      switch (btn)
      {
      case BTN_A:
        game_state = START;
        Serial.println("Start");
        start_timer.start(random(500, 5000));
        set_all_leds_off();
        break;
      case BTN_B:
        game_mode = game_mode == ONE_PLAYER ? TWO_PLAYERS : ONE_PLAYER;
        break;
      case BTN_C:
        game_state = SETUP_MIC;
        set_all_leds_off();
        break;
      case BTN_D:
        return QUIT;
        break;
      }
      break;
    case SETUP_MIC:
      if (getClapped())
      {
        led_green.setOn(150);
        Serial.println("Clapped!");
      }
      if (btn == BTN_D)
      {
        game_state = SET_GAME_MODE;
      }
      break;
    case START:
      if (start_timer.isDone())
      {
        for(int i = 0; i < NB_LEDS; i++)
        {
          led_arr[i]->setOn(200);
        }
        start_time = millis();
        game_state = WAIT;
      }
      break;
    case WAIT:
      switch (game_mode)
      {
      case ONE_PLAYER:
        if (getClapped())
        {
          reaction_time = millis() - start_time;
          Serial.print("One player-> reaction_time: ");
          Serial.println(reaction_time);
          Serial.println("Game Over");
          game_state = GAME_OVER;
        }
        break;
      case TWO_PLAYERS:
        if ((btn == BTN_A || btn == BTN_B) && reaction_time_p1 == 0)
        {
          reaction_time_p1 = millis() - start_time;
        }
        if ((btn == BTN_C || btn == BTN_D) && reaction_time_p2 == 0)
        {
          reaction_time_p2 = millis() - start_time;
        }
        if (reaction_time_p1 > 0 && reaction_time_p2 > 0)
        {
          winner_time = min(reaction_time_p1, reaction_time_p2);
          if ((unsigned long)winner_time == reaction_time_p1)
          {
            winner = 1;
          }
          else
          {
            winner = 2;
          }
          game_state = GAME_OVER;
          Serial.println("Two players");
          Serial.print("Player 1 reaction: ");
          Serial.println(reaction_time_p1);
          Serial.print("Player 2 reaction: ");
          Serial.println(reaction_time_p2);
          Serial.print("Winner: ");
          Serial.println(winner);
        }

        break;
      }
      break;
    case GAME_OVER:
      if (game_mode == ONE_PLAYER)
      {
        if (reaction_time < GOOD_REACTION_TIME)
        {
          set_matrix_arr(matrix_arr, game_mode, GREEN);
        }
        else if (reaction_time > BAD_REACTION_TIME)
        {
          set_matrix_arr(matrix_arr, game_mode, RED);
        }
        else
        {
          set_matrix_arr(matrix_arr, game_mode, YELLOW);
        }
      }
      else
      {
        set_matrix_arr(matrix_arr, game_mode, winner);
      }

      if (winner == 1)
      {
        led_green.setOn();
        led_red.setOn();
        led_yellow.setOff();
        led_blue.setOff();
      }
      else if (winner == 2)
      {
        led_green.setOff();
        led_red.setOff();
        led_yellow.setOn();
        led_blue.setOn();
      }
      else
      {
        led_red.setOff();
        led_green.setOn();
        led_yellow.setOff();
        led_blue.setOn();
      }
      if (btn == BTN_D)
      {
        return QUIT;
      }
      else if (btn != NONE)
      {
        return RESTART;
      }
      break;
    default: // Should never happen;
      return QUIT;
    }
    update_leds();
    set_matrix(matrix_arr);
  }
  return CONTINUE;
}

static bool getClapped()
{
  return !digitalRead(MIC_PIN);
}
static void set_matrix_arr(uint8_t matrix_arr[8][8], GameModeEnum mode, int8_t data)
{
  if (mode == ONE_PLAYER)
  {
    for (int i = 0; i < MATRIX_ROWS; i++)
    {
      for (int j = 0; j < MATRIX_ROWS; j++)
      {
        matrix_arr[i][j] = data;
      }
    }
  }
  else
  {
    if (data == 1)
    {
      for (int i = 0; i < MATRIX_ROWS; i++)
      {
        for (int j = MATRIX_ROWS / 2; j < MATRIX_ROWS ; j++)
        {
          matrix_arr[i][j] = GREEN;
        }
      }
    }
    else
    {
      for (int i = 0; i < MATRIX_ROWS; i++)
      {
        for (int j = 0; j < MATRIX_ROWS / 2; j++)
        {
          matrix_arr[i][j] = GREEN;
        }
      }
    }
  }
}