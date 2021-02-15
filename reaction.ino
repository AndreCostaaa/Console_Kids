#include "Games.h"
#include "Console_Kids.h"

bool getClapped()
{
  return !digitalRead(MIC_PIN);
}
GameProgressEnum reaction(ButtonPressedEnum btn)
{
  static GameStateEnum game_state = SET_GAME_MODE;
  static unsigned long start_time = 0;
  static unsigned long reaction_time = 0;
  static unsigned long reaction_time_p1 = 0;
  static unsigned long reaction_time_p2 = 0;
  static int winner = 0;
  int winner_time = 0;
  static GameModeEnum game_mode = ONE_PLAYER;
  static Timer start_timer  = Timer();

  switch (game_state)
  {
    case SET_GAME_MODE:
      led_green.setOn();
      if (game_mode == TWO_PLAYERS)
      {
        led_yellow.setOn();
      }
      else
      {
        led_yellow.setOff();
      }

      if (btn == BTN_A)
      {
        game_state = START;
        Serial.println("Start");
        start_timer.start(random(100, 500));
        led_green.setOff();
        led_yellow.setOff();
      }
      else if (btn == BTN_B || btn == BTN_C)
      {
        game_mode = game_mode == ONE_PLAYER ? TWO_PLAYERS : ONE_PLAYER;
      }
      else if (btn == BTN_D)
      {
        return QUIT;
      }
      break;
    case START:
      if (start_timer.isDone())
      {
        led_arr[random(0, 3)]->setOn(200);
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
            reaction_time =  millis() - start_time;
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
            if (winner_time == reaction_time_p1)
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
      if (btn != NONE)
      {
        reaction_time_p1 = 0;
        reaction_time_p2 = 0;
        winner = 0;
        game_state = SET_GAME_MODE;
        for(int i= 0; i< NB_LEDS; i++)
        {
          led_arr[i]->setOff();
        }
        if (btn == BTN_D)
        {
          return QUIT;
        }
      }
      break;
  }
  return CONTINUE;
}
