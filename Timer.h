#ifndef Timer_h
#define Timer_h

class Timer
{
  private:
    unsigned long start_time;
    unsigned long timer;
    boolean is_done;
    boolean getDone();
    
  public:
    Timer();
    bool isDone();
    void restart();
    void start(unsigned long duration);
};

#endif
