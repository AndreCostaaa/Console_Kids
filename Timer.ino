#include "Timer.h"

Timer::Timer()
{
  start_time = millis();
  timer = 0;
  is_done = true;
}

bool Timer::getDone()
{
  if (millis() - start_time >= timer)
  {
    is_done = true;
  }
}
bool Timer::isDone()
{
  getDone();
  return is_done;
}

void Timer::restart()
{
  start_time = millis();
  is_done = false;
}
void Timer::start(unsigned long duration)
{
  start_time = millis();
  timer = duration;
  is_done = false;
}
