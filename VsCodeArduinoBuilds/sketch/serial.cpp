#include "serial.h"

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
    _serial.write(SET);
    _serial.write(MATRIX);
    _serial.write(x + '0');
    _serial.write(y + '0');
    _serial.write(color);
    _serial.println();
}
void PC_APP::set_led_on(ColorEnum led)
{
    _serial.write(SET);
    _serial.write(LED);
    _serial.write(led);
    _serial.write(ON);
    _serial.println();
}
void PC_APP::set_led_off(ColorEnum led)
{
    _serial.write(SET);
    _serial.write(LED);
    _serial.write(led);
    _serial.write(OFF);
    _serial.println();
}
void PC_APP::set_btn_pressed(ColorEnum btn)
{
    _serial.write(SET);
    _serial.write(BTN);
    _serial.write(btn);
    _serial.write(ON);
    _serial.println();
}
void PC_APP::set_btn_idle(ColorEnum btn)
{
    _serial.write(SET);
    _serial.write(BTN);
    _serial.write(btn);
    _serial.write(OFF);
    _serial.println();
}
void PC_APP::set_mic_clapped()
{
    _serial.write(SET);
    _serial.write(MIC);
    _serial.write(ON);
    _serial.println();
}
void PC_APP::set_mic_idle()
{
    _serial.write(SET);
    _serial.write(MIC);
    _serial.write(OFF);
    _serial.println();
}
void PC_APP::set_matrix(uint8_t data[8][8])
{
    ColorEnum color;
    char buffer[8];
    int index = 0;
    bool skip = false;
    for (int i = 0; i < 8; i++)
    {
        if (!skip)
        {
            _serial.write(SET);
            _serial.write(MATRIX);
            _serial.write(EVERYTHING);
            _serial.write(i + '0');
            skip = true;
        }
        else
        {
            skip = false;
        }
        for (int j = 0; j < 8; j++)
        {
            switch (data[i][j])
            {
            case 1:
                color = CMD_RED;
                break;
            case 3:
                color = CMD_GREEN;
                break;
            case 2:
                color = CMD_ORANGE;
                break;
            case 0:
                color = CMD_OFF;
                break;
            default:
                break;
            }
            _serial.write(color);
        }
        if(!skip)
        {
            _serial.println();
        }
    }
}
void PC_APP::set_leds(bool *data)
{
    _serial.write(SET);
    _serial.write(LED);
    _serial.write(EVERYTHING);
    for (int i = 0; i < 4; i++)
    {
        _serial.write(data[i] == true ? ON : OFF);
    }
    _serial.println();
}
void PC_APP::set_buttons(bool *data)
{
    _serial.write(SET);
    _serial.write(BTN);
    _serial.write(EVERYTHING);
    for (int i = 0; i < 4; i++)
    {
        _serial.write(data[i] == true ? ON : OFF);
    }
    _serial.println();
}