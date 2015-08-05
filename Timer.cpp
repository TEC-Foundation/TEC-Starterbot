#include "Timer.h"

Timer::Timer(unsigned long (*getSystemTime)())
{
  _getSystemTime = getSystemTime;
  doneAt = -1;
}

void Timer::set(long milliseconds)
{
  doneAt = _getSystemTime() + milliseconds;
}

bool Timer::isDone()
{
  if(_getSystemTime() >= doneAt && doneAt != -1)
  {
    doneAt = -1;
    return true;
  }
}
