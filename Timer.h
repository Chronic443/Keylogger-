#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <chrono>
//class for asynchronous code execution
//we use threads new C++11 features (chrono) to achieve this
class Timer
    {
    //private variables

    //our thread (if async execution is performed)
    std::thread Thread;
    //indicates state of timer (is running)
    bool Alive = false;
    //how many times function will be executed?
    long CallNumber = -1L;
    //same as above, we use it in our functions, and we MODIFY it
    //that's why we need copy
    long repeat_count = -1L;
    //repetition interval (in millisconds)
    std::chrono::milliseconds interval = std::chrono::milliseconds(0);
    //user function that timer periodically calls
    std::function<void(void)> funct = nullptr;
    //helper function that calls sleeps and calls user function
    void SleepAndRun ()
        {
        //pause THIS thread for interval milliseconds
        //this thread is current thread if Async = false
        //otherwise it is Timer::Thread
        std::this_thread::sleep_for(interval);
        if (Alive)
            Function ()(); //call user function
        }
    //function that is passed to thread
    void ThreadFunc ()
        {
        //if infinite, call while (Alive)
        //user must stop timer with Timer::Stop method
        if (CallNumber == Infinite)
            while (Alive)
                SleepAndRun ();
        else
            //otherwise, call it definite number of times
            //repeat_count is DECREASING (-- operator)
            //in while's header
            while (repeat_count--)
                SleepAndRun ();
        }


public:
    static const long Infinite = -1L;
    //constructors
    Timer () {} //empty
    //accepting user function as a parameter
    Timer(const std::function<void(void)> &f) : funct (f) {}
    //accepting function, interval ond repeat count
    Timer(const std::function<void(void)> &f,
          const unsigned long &i,
          const long repeat = Timer::Infinite) : funct (f),
                                                 interval (std::chrono::milliseconds(i)), CallNumber (repeat) {}
    //start timer
    //if #Async is true, then the Function will be executed (called)
    //in a new thread, otherwise the call will be blocking
    void Start (bool Async = true)
        {
        //if running, return, already started
        if (IsAlive ())
            return;
        //if not running, set status to "running"
        Alive = true;
        //set value of repeat count to CallNumber (make copy)
        repeat_count = CallNumber;
        //if asynchronous call, call function in new thread
        if (Async)
            Thread = std::thread (ThreadFunc, this);
        else //else, call function in current thread (blocking call)
            this->ThreadFunc ();
        }
    //stop timer
    void Stop ()
        {
        //set status to "not alive"
        Alive = false;
        //and join threads, this means that there will not be
        //concurrent execution (only one, main, thread)
        Thread.join ();
        }
    void SetFunction (const std::function<void (void)> &f)
        {
        funct = f;
        }
    bool IsAlive () const {return Alive;}
    void RepeatCount (const long r)
        {
        //if running, discard changes
        if (Alive)
            return;
        CallNumber = r;
        }
    //how many times are left to call a function
    long GetLeftCount () const {return repeat_count;}
    //get repeat count
    long RepeatCount () const {return CallNumber;}
    void SetInterval (const unsigned long &i)
        {
        //if running, discard changes
        if (Alive)
            return;;
        //set interval
        interval = std::chrono::milliseconds(i);
        }
    //get interval
    unsigned long Interval () const {return interval.count();}
    //function we passed to our thread
    const std::function<void(void)> &Function () const
        {
        //return user function (that is passed to timer)
        return funct;
        }
    };

#endif // TIMER_H
