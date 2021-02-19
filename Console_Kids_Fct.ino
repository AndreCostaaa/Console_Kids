#include "Console_Kids.h"
#include "FctAntirebond.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

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
  randomSeed(analogRead(A0));

  Serial.begin(115200);
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

void update_leds()
{
  for (int i = 0; i < NB_LEDS; i++)
  {
    led_arr[i]->update();
  }
}
void set_matrix(uint8_t data[8][8])
{
  matrix.clear();
  matrix.setRotation(0);
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      matrix.drawPixel(i, j, data[i][j]);
    }
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