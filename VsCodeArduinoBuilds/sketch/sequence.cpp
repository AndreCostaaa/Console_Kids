
#include "Games.h"
#include "Console_Kids.h"
static void set_matrix_array(uint8_t matrix_arr[8][8], int pos);
static void lower_difficulty(uint8_t *dif);
static void up_difficulty(uint8_t *dif);

GameProgressEnum sequence()
{
  uint16_t LED_ON_TIME[] = {500, 350, 250, 150};
  uint16_t LED_OFF_TIME[] = {200, 150, 100, 50};
  uint16_t TIME_TO_PLAY[] = {5000, 3500, 2500, 1000};
  int8_t sequence_arr[PLAYS_MAX_SEQUENCE] = {-1};
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
  bool game_over;
  uint8_t difficulty = 0;
  while (1)
  {
    btn = getButtonPressed();
    new_state = game_state == old_game_state ? false : true;
    old_game_state = game_state;

    switch (game_state)
    {
    case START:
      game_over = false;
      //Initialize array
      memset(sequence_arr, -1, sizeof(sequence_arr[0]) * PLAYS_MAX_SEQUENCE);
      for (int i = 0; i < NB_LEDS; i++)
      {
        if (i <= difficulty)
        {
          led_arr[i]->setOn();
        }
        else
        {
          led_arr[i]->setOff();
        }
      }
      if (btn == BTN_A)
      {
        index = 0;
        plays = 0;
        sequence_arr[plays] = (int)random(0, 4);
        set_all_leds_off();
        timer_led.start(250);
        game_state = DISPLAY_SEQUENCE;
      }
      else if (btn == BTN_B)
      {
        up_difficulty(&difficulty);
      }
      else if (btn == BTN_C)
      {
        lower_difficulty(&difficulty);
      }
      else if (btn == BTN_D)
      {
        return QUIT;
      }
      break;

    case DISPLAY_SEQUENCE:
      if (index == plays + 1)
      {
        index = 0;
        game_state = WAIT;
        time_to_play.start(TIME_TO_PLAY[difficulty]);
      }
      else
      {
        if (timer_led.isDone() || !timer_led.get_started())
        {
          if (led_state == 0)
          {
            timer_led.start(LED_ON_TIME[difficulty]);
            led_arr[sequence_arr[index]]->setOn();
            led_state = 1;
          }
          else if (led_state == 1)
          {
            led_arr[sequence_arr[index]]->setOff();
            timer_led.start(LED_OFF_TIME[difficulty]);
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
            timer_led.start(500);
          }
          else if (timer_led.isDone())
          {
            set_all_leds_off();
            led_state = 0;
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
        if (time_to_play.isDone())
        {
          set_all_leds_on(500);
          game_over = true;
        }
        if (btn != NONE)
        {
          time_to_play.restart();
          if (sequence_arr[index] == (int8_t)btn)
          {
            led_green.setOn(200);
            index++;
          }
          else
          {
            set_all_leds_on(500);
            game_over = true;
          }
        }
        if (game_over && !led_red.get())
        {
          game_state = GAME_OVER;
          delay(200);
        }
      }

      break;

    case GAME_OVER:
      //Replay the sequence
      if (new_state)
      {
        //index = plays;
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
              else if (index < 64)
              {
                memset(matrix_arr, 0, sizeof(matrix_arr[0][0]) * 8 * 8);
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

static void lower_difficulty(uint8_t *dif)
{
  if (*dif > MIN_DIFFICULTY)
  {
    (*dif)--;
    return;
  }
  *dif = MAX_DIFFICULTY;
}

static void up_difficulty(uint8_t *dif)
{
  if (*dif < MAX_DIFFICULTY)
  {
    (*dif)++;
    return;
  }
  *dif = MIN_DIFFICULTY;
}