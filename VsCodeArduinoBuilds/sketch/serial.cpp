#include "serial.h"
#include "Console_Kids.h"
PC_APP::PC_APP(HardwareSerial &serial) : _serial(serial)
{
    _is_connected = true;
}
void PC_APP::begin()
{
    _serial.begin(115200);
}
bool PC_APP::get_connected()
{
    return _is_connected;
}
void PC_APP::receive_data()
{
    if (_serial.available())
    {
        while (_serial.available())
        {
            if (_serial.read() == CONNECTED)
            {
                _is_connected = true;
            }
            if (_serial.read() == SET)
            {
                //TODO
            }
        }
    }
}

void PC_APP::set_matrix_position(int x, int y, ColorEnum color)
{
  //not used. all leds are set at the same time
}
void PC_APP::set_led_on(ColorEnum led)
{
    /*_serial.write(SET);
    _serial.write(LED);
    _serial.write(led);
    _serial.write(ON);
    _serial.println();*/
}
void PC_APP::set_led_off(ColorEnum led)
{
    /*_serial.write(SET);
    _serial.write(LED);
    _serial.write(led);
    _serial.write(OFF);
    _serial.println();*/
}
void PC_APP::set_btn_pressed(ColorEnum btn)
{
    /*_serial.write(SET);
    _serial.write(BTN);
    _serial.write(btn);
    _serial.write(ON);
    _serial.println();*/
}
void PC_APP::set_btn_idle(ColorEnum btn)
{
    /*_serial.write(SET);
    _serial.write(BTN);
    _serial.write(btn);
    _serial.write(OFF);
    _serial.println();*/
}
void PC_APP::set_mic_clapped()
{
    /*_serial.write(SET);
    _serial.write(MIC);
    _serial.write(ON);
    _serial.println();*/
}
void PC_APP::set_mic_idle()
{
    /*_serial.write(SET);
    _serial.write(MIC);
    _serial.write(OFF);
    _serial.println();*/
}
void PC_APP::set_matrix(uint8_t data[8][8])
{

  uint16_t data_to_send = 0;      
  _serial.write(SET);
  _serial.write(MATRIX);
  for(int8_t y = 0; y < MATRIX_ROWS; y++)
  {
      for(int8_t x = 0; x < MATRIX_ROWS; x++)
      {
          data_to_send |= data[y][x] << x* 2;
      }
    _serial.write(data_to_send >> 8);
    _serial.write(data_to_send & 0xFF);
    data_to_send = 0;
  }
  _serial.println();
}
void PC_APP::set_leds(uint8_t data)
{
  _serial.write(SET);
  _serial.write(LED);
  _serial.write(data);
  _serial.println();
}
void PC_APP::set_buttons(uint8_t data)
{
  _serial.write(SET);
  _serial.write(BTN);
  _serial.write(data);
  _serial.println();
}
