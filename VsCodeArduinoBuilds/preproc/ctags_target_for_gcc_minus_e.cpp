# 1 "c:\\Users\\André\\Desktop\\Console_Kids\\Console_Kids\\Console_Kids.ino"
# 2 "c:\\Users\\André\\Desktop\\Console_Kids\\Console_Kids\\Console_Kids.ino" 2

# 4 "c:\\Users\\André\\Desktop\\Console_Kids\\Console_Kids\\Console_Kids.ino" 2
# 5 "c:\\Users\\André\\Desktop\\Console_Kids\\Console_Kids\\Console_Kids.ino" 2

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
# 67 "c:\\Users\\André\\Desktop\\Console_Kids\\Console_Kids\\Console_Kids.ino"
  update_leds();
  set_matrix(matrix_arr);
}
# 1 "c:\\Users\\André\\Desktop\\Console_Kids\\Console_Kids\\Console_Kids_Fct.ino"
# 2 "c:\\Users\\André\\Desktop\\Console_Kids\\Console_Kids\\Console_Kids_Fct.ino" 2
# 3 "c:\\Users\\André\\Desktop\\Console_Kids\\Console_Kids\\Console_Kids_Fct.ino" 2
# 4 "c:\\Users\\André\\Desktop\\Console_Kids\\Console_Kids\\Console_Kids_Fct.ino" 2
# 5 "c:\\Users\\André\\Desktop\\Console_Kids\\Console_Kids\\Console_Kids_Fct.ino" 2
# 6 "c:\\Users\\André\\Desktop\\Console_Kids\\Console_Kids\\Console_Kids_Fct.ino" 2
# 7 "c:\\Users\\André\\Desktop\\Console_Kids\\Console_Kids\\Console_Kids_Fct.ino" 2

PC_APP pc = PC_APP(Serial);
Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

Led led_red = Led(A0);
Led led_green = Led(12);
Led led_yellow = Led(2);
Led led_blue = Led(3);
Led *led_arr[4];

void init_()
{
  led_arr[0] = &led_red;
  led_arr[1] = &led_green;
  led_arr[2] = &led_yellow;
  led_arr[3] = &led_blue;
  for (int i = 0; i < 4; i++)
  {
    led_arr[i]->begin();
  }
  pinMode(7, INPUT);
  pinMode(13, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  for (int i = 0; i < 4; i++)
  {
    led_arr[i]->setOn(500);
  }
  randomSeed(analogRead(A0));

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
  bool data[4];
  ColorEnum btn_color[] = {CMD_RED, CMD_GREEN, CMD_ORANGE, CMD_BLUE};
  if (!anti_rebond.get_started())
  {
    anti_rebond.start(10);
  }

  if (anti_rebond.isDone())
  {
    Antirebond(13, &btn_a, (bool)0 /* Si l'entrée est active à 0                            */);
    Antirebond(A1, &btn_b, (bool)0 /* Si l'entrée est active à 0                            */);
    Antirebond(4, &btn_c, (bool)0 /* Si l'entrée est active à 0                            */);
    Antirebond(5, &btn_d, (bool)0 /* Si l'entrée est active à 0                            */);

    for (int i = 0; i < 4; i++)
    {
      if (*(btn_state[i]) == 0 /* 0: L'entree est non actionnee*/)
      {
        data[i] = false;
      }
      else
      {
        data[i] = true;
      }
    }

    if (pc.get_connected())
    {
      pc.set_buttons(data);
    }
    if (btn_a == 3 /* 3: L'entree passe a l'etat actif*/)
    {
      return BTN_A;
    }
    else if (btn_b == 3 /* 3: L'entree passe a l'etat actif*/)
    {
      return BTN_B;
    }
    else if (btn_c == 3 /* 3: L'entree passe a l'etat actif*/)
    {
      return BTN_C;
    }
    else if (btn_d == 3 /* 3: L'entree passe a l'etat actif*/)
    {
      return BTN_D;
    }
  }
  return NONE;
}
void set_all_leds_off()
{
  for (int i = 0; i < 4; i++)
  {
    led_arr[i]->setOff();
  }
}
void update_leds()
{
  bool led_state[4];

  for (int i = 0; i < 4; i++)
  {
    led_arr[i]->update();
    led_state[i] = led_arr[i]->get();
  }
  if (pc.get_connected())
  {
    pc.set_leds(led_state);
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
  if (pc.get_connected())
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
# 1 "c:\\Users\\André\\Desktop\\Console_Kids\\Console_Kids\\FctAntirebond.ino"


// Fonction Antirebond ******************************************************************
void Antirebond (int iEntree,byte *bRead, bool boActif){
  boolean boInput = 0;

  if(boActif == (bool)1 /* Si l'entrée est active à 1*/){ // Si l'entrée utilise une pulldown (actif à 1)
    boInput = digitalRead(iEntree);
      }
  else{
    boInput = !digitalRead(iEntree);
      }

  if(boInput){ // Si l'entrée est active
    switch (*bRead){
      case 2 /* 2: L'entree est active*/ :
      case 3 /* 3: L'entree passe a l'etat actif*/ : *bRead = 2 /* 2: L'entree est active*/;
                             break;

      case 0 /* 0: L'entree est non actionnee*/ : *bRead = 6 /* 6: L'entree est active avant antirebond (ne pas utiliser)*/;
                             break;
      case 6 /* 6: L'entree est active avant antirebond (ne pas utiliser)*/ : *bRead = 3 /* 3: L'entree passe a l'etat actif*/;
                             break;
    }
  }
  else{ // Donc l'entrée est inactive
    switch (*bRead){
      case 0 /* 0: L'entree est non actionnee*/ :
      case 1 /* 1: L'entree passe en position de repos*/ : *bRead = 0 /* 0: L'entree est non actionnee*/;
                             break;

      case 2 /* 2: L'entree est active*/ : *bRead = 9 /* 9: L'entree est inactive avant antirebond (ne pas utiliser)*/;
                             break;
      case 9 /* 9: L'entree est inactive avant antirebond (ne pas utiliser)*/ : *bRead = 1 /* 1: L'entree passe en position de repos*/;
                             break;
    }
  }
}
