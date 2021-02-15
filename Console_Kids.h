
#include "Led.h"
#ifndef Console_Kids_h
#define Console_Kids_h


#define NB_LEDS 4
#define LED_RED_PIN A1
#define LED_GREEN_PIN A2
#define LED_YELLOW_PIN A3
#define LED_BLUE_PIN 2

#define BTN_A_PIN 3
#define BTN_B_PIN 4
#define BTN_C_PIN 5
#define BTN_D_PIN 6

#define SDA A4
#define SCL A5

#define MIC_PIN 7

#define OFF 0
#define RED 1
#define YELLOW 2
#define GREEN 3

typedef enum
{
  SEQUENCE,
  TIC_TAC_TOE,
  FOUR_IN_A_ROW
}GameEnum;

typedef enum
{
  NONE,
  BTN_A,
  BTN_B,
  BTN_C,
  BTN_D
}ButtonPressedEnum;

typedef enum
{
  ANTI_REBOND
}TimerEnum;

typedef enum
{
  SELECTING,
  PLAYING
}ConsoleStateEnum;

typedef enum
{
  CONTINUE,
  QUIT
}GameProgressEnum;

void initPins();

ButtonPressedEnum getButtonPressed();
void set_matrix(int data[8][8]);

Led led_red = Led(LED_RED_PIN);
Led led_green = Led(LED_GREEN_PIN);
Led led_yellow = Led(LED_YELLOW_PIN);
Led led_blue = Led(LED_BLUE_PIN);

Led* led_arr[NB_LEDS] = {&led_red, &led_green, &led_yellow, &led_blue};

#endif
