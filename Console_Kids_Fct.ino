#include "Console_Kids.h"
#include "FctAntirebond.h"

void initPins()
{
  for (int i = 0; i < NB_LEDS; i++)
  {
    led_arr[i]->begin();
  }
  pinMode(MIC_PIN, INPUT);
  pinMode(BTN_A_PIN, INPUT_PULLUP);
  pinMode(BTN_B_PIN, INPUT_PULLUP);
  pinMode(BTN_C_PIN, INPUT_PULLUP);
  pinMode(BTN_D_PIN, INPUT_PULLUP);
  matrix.begin(0x70);
}
ButtonPressedEnum getButtonPressed()
{
  static byte btn_a;
  static byte btn_b;
  static byte btn_c;
  static byte btn_d;

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
  return NONE;

}

void set_matrix(int data[8][8])
{
  matrix.clear();
  matrix.setRotation(0);
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      matrix.drawPixel(i, j, data[i][j] );
    }
  }
  matrix.writeDisplay();
  return CONTINUE;
}
