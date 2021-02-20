#include "Games.h"
#include "Console_Kids.h"

static void DisplayStartLedSequence(bool new_state);
static void set_matrix_array(uint8_t matrix_arr[8][8], int pos);

GameProgressEnum sequence()
{
  int8_t sequence_arr[PLAYS_MAX_SEQUENCE];
  GameStateEnum old_game_state = SET_GAME_MODE;
  GameStateEnum game_state = START;
  int index = 0;
  int plays = 0;
  int8_t led_state = 0;
  Timer timer_led = Timer();
  Timer time_to_play = Timer();
  ButtonPressedEnum btn;
  uint8_t matrix_arr[8][8] = {0};
  bool new_state;
  while (1)
  {
    btn = getButtonPressed();
    new_state = game_state == old_game_state ? false : true;
    old_game_state = game_state;

    switch (game_state)
    {
    case START:
      memset(sequence_arr, -1, sizeof(sequence_arr[0]) * PLAYS_MAX_SEQUENCE);

      DisplayStartLedSequence(new_state);

      if (btn != NONE)
      {
        index = 0;
        plays = 0;
        sequence_arr[plays] = (int)random(0, 4);
        for (int i = 0; i < NB_LEDS; i++)
        {
          led_arr[i]->setOff();
        }
        Serial.println("Display Sequence");
        game_state = DISPLAY_SEQUENCE;
      }
      break;

    case DISPLAY_SEQUENCE:
      if (index == plays + 1)
      {
        index = 0;
        game_state = WAIT;
        time_to_play.start(TIME_TO_PLAY_SEQUENCE);
        Serial.println("waiting");
      }
      else
      {
        if (timer_led.isDone() || !timer_led.get_started())
        {
          if (led_state == 0)
          {
            timer_led.start(500);
            led_arr[sequence_arr[index]]->setOn();
            led_state = 1;
          }
          else if (led_state == 1)
          {
            led_arr[sequence_arr[index]]->setOff();
            timer_led.start(200);
            led_state = 2;
          }
          else if (led_state == 2)
          {
            index++;
            led_state = 0;
          }
        }
      }
      break;

    case WAIT:
      //all good
      if (index == plays + 1)
      {
        if (!led_green.get())
        {
          if (led_state == 0)
          {
            led_state = 1;
            timer_led.start(200);
          }
          else if (timer_led.isDone())
          {
            led_state = 0;
            led_green.setOff();
            index = 0;
            plays++;
            set_matrix_array(matrix_arr, plays - 1);
            if (plays >= PLAYS_MAX_SEQUENCE)
            {
              game_state = GAME_OVER_CROWN;
            }
            sequence_arr[plays] = (int)random(0, 4);
            game_state = DISPLAY_SEQUENCE;
          }
        }
      }
      //Hitting sequence
      else
      {
        if(time_to_play.isDone())
        {
          game_state = GAME_OVER;
        }
        if(btn != NONE)
        {
          time_to_play.restart();
        }
        switch (btn)
        {
        case BTN_A:
          if (sequence_arr[index] == 0)
          {
            led_green.setOn(200);
            index++;
          }
          else
          {
            led_red.setOn(1000);
            game_state = GAME_OVER;
          }
          break;
        case BTN_B:
          if (sequence_arr[index] == 1)
          {
            led_green.setOn(200);
            index++;
          }
          else
          {
            led_red.setOn(1000);
            game_state = GAME_OVER;
          }
          break;
        case BTN_C:
          if (sequence_arr[index] == 2)
          {
            led_green.setOn(200);
            index++;
          }
          else
          {
            led_red.setOn(1000);
            game_state = GAME_OVER;
          }
          break;
        case BTN_D:
          if (sequence_arr[index] == 3)
          {
            led_green.setOn(200);
            index++;
          }
          else
          {
            led_red.setOn(1000);
            game_state = GAME_OVER;
          }
          break;
        default:
          break;
        }
      }

      break;

    case GAME_OVER:
      //Replay the sequence
      if (new_state)
      {
        index = plays;
        timer_led.start(0);
      }

      if (!led_arr[sequence_arr[index]]->get())
      {
        if (!timer_led.get_started())
        {
          timer_led.start(200);
        }
        else
        {
          if (timer_led.isDone())
          {
            index++;
            if (index == plays + 1)
            {
              if (index == 64)
              {
                for (int i = 0; i < 8; i++)
                {
                  for (int j = 0; j < 8; j++)
                  {
                    matrix_arr[i][j] = GREEN;
                  }
                }
              }
              else if(index < 64)
              {
                memset(matrix_arr, 0, sizeof(matrix_arr[0][0])* 8 *8 );
              }
              index = 0;
            }
            led_arr[sequence_arr[index]]->setOn(500);
            set_matrix_array(matrix_arr, index);
          }
        }
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
    case GAME_OVER_CROWN:
      set_crown(matrix_arr);
      break;

    default: // Should never happen;
      return QUIT;
    }
    set_matrix(matrix_arr);
    update_leds();
  }
  return CONTINUE;
}
static void DisplayStartLedSequence(bool new_state)
{
  static Timer timer = Timer();
  if (new_state)
  {
    Serial.println("timer start");
    timer.start(500);
    led_red.setOn();
    led_yellow.setOn();
  }
  if (timer.isDone())
  {
    Serial.println("timer done");
    for (int i = 0; i < NB_LEDS; i++)
    {
      led_arr[i]->toggle();
    }
    timer.start(500);
  }
}
static void set_matrix_array(uint8_t matrix_arr[8][8], int pos)
{
  uint8_t x, y;
  if (pos > 63)
  {
    pos = 63 - (pos - 64);
    y = pos / 8;
    x = pos - (y * 8);
    matrix_arr[y][x] = OFF;
  }
  else
  {
    y = pos / 8;
    x = pos - (y * 8);
    matrix_arr[y][x] = YELLOW;
  }
}