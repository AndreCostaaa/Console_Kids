#include <Arduino.h>
#line 1 "u:\\P. Console Kids\\Prog\\Console_Kids\\Console_Kids.ino"
#include "Console_Kids.h"
#include "Timer.h"
#include "Games.h"
#include "serial.h"

#line 6 "u:\\P. Console Kids\\Prog\\Console_Kids\\Console_Kids.ino"
void setup();
#line 13 "u:\\P. Console Kids\\Prog\\Console_Kids\\Console_Kids.ino"
void loop();
#line 17 "u:\\P. Console Kids\\Prog\\Console_Kids\\Console_Kids_Fct.ino"
void init_();
#line 43 "u:\\P. Console Kids\\Prog\\Console_Kids\\Console_Kids_Fct.ino"
ButtonPressedEnum getButtonPressed();
#line 108 "u:\\P. Console Kids\\Prog\\Console_Kids\\Console_Kids_Fct.ino"
void set_all_leds_off();
#line 116 "u:\\P. Console Kids\\Prog\\Console_Kids\\Console_Kids_Fct.ino"
void set_all_leds_on();
#line 124 "u:\\P. Console Kids\\Prog\\Console_Kids\\Console_Kids_Fct.ino"
void set_all_leds_on(int duration);
#line 131 "u:\\P. Console Kids\\Prog\\Console_Kids\\Console_Kids_Fct.ino"
void update_leds();
#line 155 "u:\\P. Console Kids\\Prog\\Console_Kids\\Console_Kids_Fct.ino"
void set_matrix(uint8_t data[8][8]);
#line 179 "u:\\P. Console Kids\\Prog\\Console_Kids\\Console_Kids_Fct.ino"
void set_crown(uint8_t data[8][8]);
#line 200 "u:\\P. Console Kids\\Prog\\Console_Kids\\Console_Kids_Fct.ino"
void turn_on_player_leds(uint8_t player);
#line 4 "u:\\P. Console Kids\\Prog\\Console_Kids\\FctAntirebond.ino"
void Antirebond(int iEntree,byte *bRead, bool boActif);
#line 6 "u:\\P. Console Kids\\Prog\\Console_Kids\\Console_Kids.ino"
void setup()
{
  //initializes pins, serial communication and matrix
  init_();
}

GameProgressEnum (*game_func_arr[])(void) = {tic_tac_toe, four_in_a_row, sequence, reaction};
void loop()
{
  static uint8_t matrix_arr[8][8] = {0};
  static ConsoleStateEnum state = SELECTING;
  static uint8_t game_index = 0;
  ButtonPressedEnum btn = NONE;
  pc.receive_data();
  //btn = pc.receive_data();
  btn = getButtonPressed();
  switch (state)
  {
  case SELECTING:
    led_arr[game_index]->setOn();
    if (btn == BTN_A) //Start game
    {
      state = PLAYING;
      led_arr[game_index]->setOff();
    }
    else if (btn == BTN_B || btn == BTN_C) //previous game
    {
      led_arr[game_index]->setOff();
      if (game_index == 3)
      {
        game_index = 0;
      }
      else
      {
        game_index++;
      }
      led_arr[game_index]->setOn();
    }

    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        matrix_arr[i][j] = GAME_PREVIEW_ARR[game_index][i][j];
      }
    }
    break;
  case PLAYING:
    if ((*game_func_arr[game_index])() == QUIT)
    {
      state = SELECTING;
    }
    set_all_leds_off();
    break;
  default:
    break;
  }

  /*
     Update every led to see if it's time to turn it off
     Complements the use of setOn(duration)
  */
  update_leds();
  set_matrix(matrix_arr);
}

#line 1 "u:\\P. Console Kids\\Prog\\Console_Kids\\Console_Kids_Fct.ino"
#include "Console_Kids.h"
#include "FctAntirebond.h"
#include "serial.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

PC_APP pc = PC_APP(Serial);
Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

Led led_red = Led(LED_RED_PIN);
Led led_green = Led(LED_GREEN_PIN);
Led led_yellow = Led(LED_YELLOW_PIN);
Led led_blue = Led(LED_BLUE_PIN);
Led *led_arr[NB_LEDS];

void init_()
{
  led_arr[0] = &led_red;
  led_arr[1] = &led_green;
  led_arr[2] = &led_yellow;
  led_arr[3] = &led_blue;
  for (int i = 0; i < NB_LEDS; i++)
  {
    led_arr[i]->begin();
  }
  pinMode(MIC_PIN, INPUT);
  pinMode(BTN_A_PIN, INPUT_PULLUP);
  pinMode(BTN_B_PIN, INPUT_PULLUP);
  pinMode(BTN_C_PIN, INPUT_PULLUP);
  pinMode(BTN_D_PIN, INPUT_PULLUP);

  for (int i = 0; i < NB_LEDS; i++)
  {
    led_arr[i]->setOn(500);
  }
  randomSeed(analogRead(6));

  pc.begin();
  matrix.begin(0x70);
}

ButtonPressedEnum getButtonPressed()
{
  static byte btn_a;
  static byte btn_b;
  static byte btn_c;
  static byte btn_d;
  static Timer anti_rebond = Timer();
  anti_rebond.set_auto_restart(true);
  byte *btn_state[] = {&btn_a, &btn_b, &btn_c, &btn_d};
  static int8_t old_btn_data = 0;
  int8_t btn_data = 0;
  ColorEnum btn_color[] = {CMD_RED, CMD_GREEN, CMD_ORANGE, CMD_BLUE};
  if (!anti_rebond.get_started())
  {
    anti_rebond.start(10);
  }
  if (anti_rebond.isDone())
  {
    Antirebond(BTN_A_PIN, &btn_a, ACTIF_A_0);
    Antirebond(BTN_B_PIN, &btn_b, ACTIF_A_0);
    Antirebond(BTN_C_PIN, &btn_c, ACTIF_A_0);
    Antirebond(BTN_D_PIN, &btn_d, ACTIF_A_0);
    for (int i = 0; i < NB_LEDS; i++)
    {
      if (*(btn_state[i]) != INACTIF)
      {
        btn_data |= (1 << i);
      }
      else
      {
        btn_data &= ~(1 << i);
      }
    }
    if (pc.get_connected())
    {
      if (btn_data != old_btn_data)
      {
        pc.set_buttons(btn_data);
      }
      old_btn_data = btn_data;
    }
    else
    {
      old_btn_data = -1;
    }
    if (btn_a == FLANC_ACTIF)
    {
      return BTN_A;
    }
    else if (btn_b == FLANC_ACTIF)
    {
      return BTN_B;
    }
    else if (btn_c == FLANC_ACTIF)
    {
      return BTN_C;
    }
    else if (btn_d == FLANC_ACTIF)
    {
      return BTN_D;
    }
  }
  return NONE;
}

void set_all_leds_off()
{
  for (int i = 0; i < NB_LEDS; i++)
  {
    led_arr[i]->setOff();
  }
}

void set_all_leds_on()
{
  for (int i = 0; i < NB_LEDS; i++)
  {
    led_arr[i]->setOn();
  }
}

void set_all_leds_on(int duration)
{
  for (int i = 0; i < NB_LEDS; i++)
  {
    led_arr[i]->setOn(duration);
  }
}
void update_leds()
{
  int8_t led_data = 0;
  static int8_t old_led_data = 0;
  for (int i = 0; i < NB_LEDS; i++)
  {
    led_arr[i]->update();
    led_data |= led_arr[i]->get() << i;
  }
  if (pc.get_connected())
  {
    if (led_data != old_led_data)
    {
      pc.set_leds(led_data);
    }
    old_led_data = led_data;
  }
  else
  {
    old_led_data = -1;
  }
}

uint8_t old_matrix_data[8][8];
void set_matrix(uint8_t data[8][8])
{

  matrix.clear();
  matrix.setRotation(0);
  bool send_data_to_pc = false;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      matrix.drawPixel(i, j, data[i][j]);
      if (data[i][j] != old_matrix_data[i][j])
      {
        send_data_to_pc = true;
        old_matrix_data[i][j] = data[i][j];
      }
    }
  }
  if (pc.get_connected() && send_data_to_pc)
  {
    pc.set_matrix(data);
  }
  matrix.writeDisplay();
}
void set_crown(uint8_t data[8][8])
{
  const uint8_t crown[8][8] =
      {
          {0, 0, 3, 3, 3, 3, 0, 0},
          {0, 3, 0, 0, 0, 0, 3, 0},
          {3, 0, 3, 0, 0, 3, 0, 3},
          {3, 0, 0, 0, 0, 0, 0, 3},
          {3, 0, 3, 0, 0, 3, 0, 3},
          {3, 0, 0, 3, 3, 0, 0, 3},
          {0, 3, 0, 0, 0, 0, 3, 0},
          {0, 0, 3, 3, 3, 3, 0, 0}};
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      data[i][j] = crown[i][j];
    }
  }
}

void turn_on_player_leds(uint8_t player)
{
  //player evaluates to either 0 or 1. so i*2 will evaluate to either 0 or 2
  //player 1's leds are the positions 0 and 1 at the led_arr
  //player 2's leds are the positions 2 and 3 at the led_arr
  for (int8_t i = 2 * player, j = 0; j < NB_LEDS / 2; i++, j++)
  {
    led_arr[i]->setOn();
  }
}
#line 1 "u:\\P. Console Kids\\Prog\\Console_Kids\\FctAntirebond.ino"
#include "FctAntirebond.h"

// Fonction Antirebond ******************************************************************
void Antirebond (int iEntree,byte *bRead, bool boActif){
  boolean boInput = 0;
  
  if(boActif == ACTIF_A_1){        // Si l'entrée utilise une pulldown (actif à 1)
    boInput = digitalRead(iEntree);
      }
  else{
    boInput = !digitalRead(iEntree);
      }
  
  if(boInput){ // Si l'entrée est active
    switch (*bRead){
      case ACTIF          : 
      case FLANC_ACTIF    : *bRead = ACTIF;
                             break;
      
      case INACTIF        : *bRead = JUSTE_ACTIF;
                             break;
      case JUSTE_ACTIF    : *bRead = FLANC_ACTIF;
                             break;
    }
  }
  else{       // Donc l'entrée est inactive
    switch (*bRead){
      case INACTIF        :
      case FLANC_INACTIF  : *bRead = INACTIF;
                             break;
      
      case ACTIF          : *bRead = JUSTE_INACTIF;
                             break;
      case JUSTE_INACTIF  : *bRead = FLANC_INACTIF;
                             break;
    }
  }
}

