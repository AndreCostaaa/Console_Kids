#include "Arduino.h"

#ifndef Serial_h
#define Serial_h

typedef enum
{
    CMD_RED = 'R',
    CMD_GREEN = 'G',
    CMD_ORANGE = 'O',
    CMD_BLUE = 'B',
    CMD_OFF = 'V',
    MATRIX_CLEAR = 0x0,
    MATRIX_RED = 0x01,
    MATRIX_YELLOW = 0x02,
    MATRIX_GREEN = 0x03
} ColorEnum;

class PC_APP
{
private:    
    typedef enum
    {
        CONNECTED = 'k',
        ARE_YOU_THERE = '-',
        SET = 'S',
        GET = 'G',
        ON = 'H',
        OFF = 'L',
        EVERYTHING = '8'
    } CommandEnum;
    typedef enum
    {
        MATRIX = 'M',
        LED = 'L',
        MIC = 'C',
        BTN = 'B'
    } ComponentEnum;
    
    HardwareSerial &_serial;
    bool _is_connected;
    char data_in[50];


public:
    PC_APP(HardwareSerial &serial);
    void begin();
    bool get_connected();
    void receive_data();
    //Set
    void set_matrix_position(int x, int y, ColorEnum color);
    void set_matrix(uint8_t data[8][8]);
    void set_led_on(ColorEnum led);
    void set_led_off(ColorEnum led);
    void set_btn_pressed(ColorEnum btn);
    void set_btn_idle(ColorEnum btn);
    void set_mic_clapped();
    void set_mic_idle();
    void set_leds(uint8_t data);
    void set_buttons(uint8_t data);
    //Get
    //Extra feautures todo

};
#endif
