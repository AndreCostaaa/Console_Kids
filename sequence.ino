#include "Games.h"
#include "Console_Kids.h"

GameProgressEnum sequence(ButtonPressedEnum btn)
{
  static int sequence_arr[100];
  static GameStateEnum old_game_state;
  static GameStateEnum game_state = START;
  static int index = 0;
  static int plays = 0;
  static int led_state = 0;
  static Timer timer_led = Timer();

  bool new_state = game_state == old_game_state ? false : true;
  old_game_state = game_state;
  switch (game_state)
  {
    case START:
      memset(sequence_arr, -1, 100);

      DisplayStartLedSequence(new_state);

      if (btn != NONE)
      {
        index = 0;
        plays = 0;
        sequence_arr[plays] = (int) random(0, 3);
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
        Serial.println("waiting");
      }
      else
      {
        if (timer_led.isDone())
        {
          if (led_state == 0)
          {
            timer_led.start(500);
            led_arr[sequence_arr[index]]->setOn();
            led_state = 1;
            Serial.println("led on");
          }
          else if (led_state == 1)
          {
            Serial.println("led off");
            led_arr[sequence_arr[index]]->setOff();
            timer_led.start(200);
            led_state = 2;
          }
          else if (led_state == 2)
          {
            Serial.println("index+");
            index++;
            led_state = 0;
          }
        }
      }
      break;

    case WAIT:
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
            sequence_arr[plays] = (int) random(0, 3);
            game_state = DISPLAY_SEQUENCE;
            Serial.println("Display Sequence");
          }
        }

      }
      else
      {
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
      DisplayEndLedSequence(new_state);
      //display plays
      if (btn != NONE)
      {
        game_state = START;
        if (btn == BTN_D)
        {
          return QUIT;
        }
      }
      break;
  }



  return CONTINUE;
}
void DisplayStartLedSequence(bool new_state)
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
void DisplayEndLedSequence(bool new_state)
{
  static int index = 0;
  static Timer timer = Timer();

  if (new_state)
  {
    timer.start(500);
  }
  if (timer.isDone())
  {

    led_arr[index]->setOn(500);
    timer.restart();
    index++;
    if (index == NB_LEDS)
    {
      index = 0;
    }
  }

}
