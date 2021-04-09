#include "Console_Kids.h"

#ifndef Games_h
#define Games_h

typedef enum GameStateEnum
{
  SET_GAME_MODE,
  PLAYER_1,
  PLAYER_2,
  GAME_OVER,
  START,
  WAIT,
  DISPLAY_SEQUENCE,
  GAMING,
  SETUP_MIC,
  GAME_OVER_CROWN //Easter Egg
}GameStateEnum;


typedef enum GameModeEnum
{
  ONE_PLAYER,
  TWO_PLAYERS
}GameModeEnum;

#define GOOD_REACTION_TIME 500
#define BAD_REACTION_TIME 1000
#define PLAYS_MAX_SEQUENCE 64
#define PLAYS_MAX_4_IN_A_ROW 64
#define MAX_PLAYS_TIC 9

#define MAX_DIFFICULTY 3
#define MIN_DIFFICULTY 0

#define P1 (uint8_t) 0
#define P2 (uint8_t) 1

GameProgressEnum reaction();
GameProgressEnum four_in_a_row();
GameProgressEnum sequence();
GameProgressEnum tic_tac_toe();
#endif
