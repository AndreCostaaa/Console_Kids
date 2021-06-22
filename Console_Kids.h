
#include "Led.h"
#include "serial.h"
#include <Arduino.h>


#ifndef Console_Kids_h
#define Console_Kids_h


#define NB_LEDS 4
#define LED_RED_PIN A0
#define LED_GREEN_PIN 12
#define LED_YELLOW_PIN 2
#define LED_BLUE_PIN 3

#define BTN_A_PIN 13
#define BTN_B_PIN A1
#define BTN_C_PIN 4
#define BTN_D_PIN 5

#define SDA A4
#define SCL A5

#define MIC_PIN 7

#define OFF 0
#define RED 1
#define YELLOW 2
#define GREEN 3

#define MATRIX_ROWS 8

typedef enum
{
  SEQUENCE,
  TIC_TAC_TOE,
  FOUR_IN_A_ROW
} GameEnum;

typedef enum
{
  NONE,
  BTN_A,
  BTN_B,
  BTN_C,
  BTN_D
} ButtonPressedEnum;

typedef enum
{
  ANTI_REBOND
} TimerEnum;

typedef enum
{
  SELECTING,
  PLAYING
} ConsoleStateEnum;

typedef enum
{
  CONTINUE,
  QUIT,
  RESTART
} GameProgressEnum;

const uint8_t GAME_PREVIEW_ARR[4][8][8] =
{
  {
    {1, 1, 2, 0, 0, 2, 0, 0},
    {1, 1, 2, 0, 0, 2, 0, 0},
    {2, 2, 2, 2, 2, 2, 2, 2},
    {0, 0, 2, 3, 3, 2, 0, 0},
    {0, 0, 2, 3, 3, 2, 0, 0},
    {2, 2, 2, 2, 2, 2, 2, 2},
    {0, 0, 2, 0, 0, 2, 1, 1},
    {0, 0, 2, 0, 0, 2, 1, 1}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 3, 0, 0},
    {0, 0, 3, 0, 0, 3, 0, 0},
    {0, 0, 3, 0, 0, 3, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 3, 3, 3, 0, 0},
    {0, 0, 0, 0, 0, 3, 0, 0},
    {0, 0, 0, 0, 0, 3, 0, 0},
    {0, 0, 3, 3, 3, 3, 0, 0},
    {0, 0, 3, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 0, 0, 0, 0},
    {0, 0, 3, 3, 3, 3, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0}
  }
};

void init_();
ButtonPressedEnum getButtonPressed();
void update_leds();
void set_matrix(uint8_t data[8][8]);
void set_crown(uint8_t data[8][8]);
void set_all_leds_off();
void turn_on_player_leds(uint8_t player);
extern Led led_red;
extern Led led_green;
extern Led led_yellow;
extern Led led_blue;
extern Led* led_arr[NB_LEDS];
extern PC_APP pc;
#endif
