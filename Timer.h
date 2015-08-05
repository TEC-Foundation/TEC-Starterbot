#ifndef TIMER_H
#define TIMER_H
#endif

class Timer
{
public:
  Timer(unsigned long (*getSystemTime)());
  void set(long milliseconds);
  bool isDone();
private:
  long doneAt;
  unsigned long (*_getSystemTime)();
};
