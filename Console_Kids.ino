#include "Console_Kids.h"
#include "Timer.h"
#include "Games.h"

Timer anti_rebond = Timer();
void setup() {
  initPins();
  Serial.begin(115200);
  anti_rebond.start(10);
  for (int i = 0; i < NB_LEDS; i++)
  {
    led_arr[i]->setOn(500);
  }
  randomSeed(analogRead(0));
}

GameProgressEnum (*game_func_arr[])(ButtonPressedEnum) = {tic_tac_toe, four_in_a_row, sequence, reaction};


void loop() {
  static ConsoleStateEnum state = SELECTING;
  static int game_index = 2;

  ButtonPressedEnum btn = NONE;
  if (anti_rebond.isDone())
  {
    btn = getButtonPressed();
    anti_rebond.restart();

  }
  switch (state)
  {
    case SELECTING:
      led_arr[game_index]->setOn();
      if (btn == BTN_A) //Start game
      {
        Serial.print("Playing: ");
        Serial.println(game_index);
        state = PLAYING;
      }
      else if (btn == BTN_B || btn == BTN_C ) //previous game
      {
        led_arr[game_index]->setOff();
        if (game_index == 3)
        {
          game_index = 0;
        }
        else
        {
          game_index ++;
        }
        led_arr[game_index]->setOn();
      }

      else if (btn == BTN_D) //next game
      {
      }
      break;
    case PLAYING:
      if ((*game_func_arr[game_index])(btn) == QUIT)
      {
        state = SELECTING;
      }
      break;
    default: break;
  }

  /*
     Update every led to see if it's time to turn it off
     Complements the use of setOn(duration)
  */
  for (int i = 0; i < NB_LEDS; i++)
  {
    led_arr[i]->update();
  }

  //to do -> output matrix
}
