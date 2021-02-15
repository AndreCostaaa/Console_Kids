#include "Timer.h"

Timer::Timer()
{
  start_time = millis();
  timer = 0;
  is_set = false;
  auto_restart = false;
}

bool Timer::isDone()
{
  if (is_set && (millis() - start_time >= timer))
  {
    is_set = false;
    if(auto_restart)
    {
      restart();
    }
    return true;
  }
  return false;
}

void Timer::restart()
{
  start_time = millis();
  is_set = true;
}
void Timer::start(unsigned long duration)
{
  start_time = millis();
  timer = duration;
  is_set = true;
}
bool Timer::get_set()
{
  return is_set;
}
void Timer::set_auto_restart(bool b)
{
  auto_restart = b;
}
