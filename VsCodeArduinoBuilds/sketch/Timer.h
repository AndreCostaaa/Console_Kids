#ifndef Timer_h
#define Timer_h

class Timer
{
  private:
    unsigned long start_time;
    unsigned long timer;
    bool is_set;
    bool auto_restart = false;
    
  public:
    Timer();
    bool isDone();
    void restart();
    void start(unsigned long duration);
    bool get_started();
    void set_auto_restart(bool b);
};

#endif
