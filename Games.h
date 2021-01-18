#include "Console_Kids.h"

#ifndef Games_h
#define Games_h

typedef enum
{
  SET_GAME_MODE,
  PLAYER_1,
  PLAYER_2,
  GAME_OVER,
  START,
  WAIT,
  DISPLAY_SEQUENCE
}GameStateEnum;


typedef enum
{
  ONE_PLAYER,
  TWO_PLAYERS
}GameModeEnum;

GameProgressEnum reaction(ButtonPressedEnum btn);
GameProgressEnum four_in_a_row(ButtonPressedEnum btn);
GameProgressEnum sequence(ButtonPressedEnum btn);
GameProgressEnum tic_tac_toe(ButtonPressedEnum btn);
#endif
