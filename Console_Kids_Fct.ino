#include "Console_Kids.h"
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
  static uint8_t old_btn_data = 0;
  uint8_t btn_data = 0;
  int btn_pins[] = {BTN_A_PIN, BTN_B_PIN, BTN_C_PIN, BTN_D_PIN};
  
  for (int i = 0; i < NB_LEDS; i++)
  {
    if (!digitalRead(btn_pins[i]))
    {
      btn_data |= (1 << i);
      
    }
  }
  if (pc.get_connected())
  {
    if (btn_data != old_btn_data || !pc.sentButtonData())
    {
      pc.set_buttons(btn_data);
    }
  }
  ButtonPressedEnum btnPressed = NONE;

  for(uint8_t i = 0; i < NB_LEDS; i++)
  {
    uint8_t mask = 1 << i;
    if(btn_data & mask && !(old_btn_data & mask))
    { 

      btnPressed = (ButtonPressedEnum) i;
    }
  }
  
  old_btn_data = btn_data;
  return btnPressed;
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
  uint8_t led_data = 0;
  static uint8_t old_led_data = 0;
  for (int i = 0; i < NB_LEDS; i++)
  {
    led_arr[i]->update();
    led_data |= led_arr[i]->get() << i;
  }
  if (pc.get_connected())
  {
    if (led_data != old_led_data || !pc.sentLedData())
    {
      pc.set_leds(led_data);
    }
    old_led_data = led_data;
  }
}

uint8_t old_matrix_data[8][8];
void set_matrix(uint8_t data[8][8])
{

  matrix.clear();
  matrix.setRotation(0);
  bool send_data_to_pc = false;
  static bool sent_first = false;
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
  if (pc.get_connected() && (send_data_to_pc || !pc.sentMatrixData()))
  {
    pc.set_matrix(data);
    sent_first = true;
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
    {0, 0, 3, 3, 3, 3, 0, 0}
  };
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
