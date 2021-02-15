#ifndef Timer_h
#define Timer_h

class Timer
{
  private:
    unsigned long start_time;
    unsigned long timer;
    boolean is_set;
    boolean auto_restart = false;
    
  public:
    Timer();
    bool isDone();
    void restart();
    void start(unsigned long duration);
    bool get_set();
    void set_auto_restart(bool b);
};

#endif
