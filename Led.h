#include "Timer.h"

#ifndef led_h
#define led_h

class Led
{
  private:
    boolean state;
    int pin;
    boolean timer_set;
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
