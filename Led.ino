#include "Led.h"

Led::Led(int p)
{
  pin = p;
  state = false;
  timer_set = false;
}
void Led::begin()
{
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
}
void Led::set()
{
  digitalWrite(pin, state);
}
void Led::setOn()
{
  timer_set = false;
  state = true;
  set();
}

void Led::setOn(int duration)
{
  timer_set = true;
  state = true;
  timer.start(duration);
  set();

}
void Led::setOff()
{
  timer_set = false;
  state = false;
  set();
}

void Led::toggle()
{
  timer_set = false;
  state = !state;
  set();
}
void Led::update()
{
  if (timer_set)
  {
    if (timer.isDone())
    {
      timer_set = false;
      state = false;
      set();
    }
  }
}
bool Led::get()
{
  return state;
}
