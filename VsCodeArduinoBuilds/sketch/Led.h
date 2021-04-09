#include "Timer.h"

#ifndef led_h
#define led_h

class Led
{
  private:
    bool state;
    int pin;
    bool  timer_set;
    Timer timer = Timer();
    void set();
  public:

    Led(int p);
    void begin();
    void setOn();
    void setOn(int duration);
    void setOff();
    void toggle();
    void update();
    bool get();
    
};
#endif
