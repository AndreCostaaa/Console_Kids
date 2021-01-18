#include "Games.h"
#include "Console_Kids.h"

GameProgressEnum tic_tac_toe(ButtonPressedEnum btn)
{
  static GameStateEnum game_state = PLAYER_1;

  Serial.println("tic");
  switch (game_state)
  {
    case PLAYER_1:
      break;
    case PLAYER_2:
      break;
    case GAME_OVER:
      break;
  }
  return CONTINUE;
}
