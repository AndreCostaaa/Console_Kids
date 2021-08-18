#include "serial.h"
#include "Console_Kids.h"
PC_APP::PC_APP(HardwareSerial &serial) : _serial(serial)
{
  _is_connected = false;
  _sentMatrixData = false;
  _sentButtonData = false;
  _sentLedData = false;
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
  if (_serial.available() > 0)
  {
    char data_in = _serial.read();
    if (data_in == CONNECTED)
    {
      _is_connected = true;
      _sentMatrixData = false;
      _sentButtonData = false;
      _sentLedData = false;
    }
    if (_serial.read() == SET)
    {
      //TODO
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
  for (int8_t y = 0; y < MATRIX_ROWS; y++)
  {
    for (int8_t x = 0; x < MATRIX_ROWS; x++)
    {
      data_to_send |= data[y][x] << x * 2;
    }
    _serial.write(data_to_send >> 8);
    _serial.write(data_to_send & 0xFF);
    data_to_send = 0;
  }
  _sentMatrixData = true;
}
void PC_APP::set_leds(uint8_t data)
{
  _serial.write(SET);
  _serial.write(LED);
  _serial.write(data);
  for (uint8_t i = 0; i < COMMUNICATION_BYTE_SIZE - 3; i++)
  {
    _serial.write(0xFF); //send dummy bytes
  }
  _sentLedData = true;

}
void PC_APP::set_buttons(uint8_t data)
{
  _serial.write(SET);
  _serial.write(BTN);
  _serial.write(data);
  for (uint8_t i = 0; i < COMMUNICATION_BYTE_SIZE - 3; i++)
  {
    _serial.write(0xFF); //send dummy bytes
  }
  _sentButtonData = true;
}

bool PC_APP::sentMatrixData()
{
  return _sentMatrixData;
}
bool PC_APP::sentButtonData()
{
  return _sentButtonData;
}
bool PC_APP::sentLedData()
{
  return _sentLedData;
}
