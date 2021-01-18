#include "Games.h"
#include "Console_Kids.h"
GameProgressEnum four_in_a_row(ButtonPressedEnum btn)
{
  static GameStateEnum game_state = PLAYER_1;
  Serial.println("four");
  switch (game_state)
  {
    case PLAYER_1:
      break;
    case PLAYER_2:
      break;
  }
  return CONTINUE;
}
