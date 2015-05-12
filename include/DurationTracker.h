/* 
 * File:   DurationTracker.h
 * Author: root
 *
 * Created on October 10, 2012, 1:29 PM
 */

#ifndef DURATIONTRACKER_H
#define	DURATIONTRACKER_H

#include <ctime>
#include <string>

namespace util {
    
class DurationTracker {
public:
    DurationTracker();
    DurationTracker(const DurationTracker& orig);
    virtual ~DurationTracker();
    
    void startTimer();
    void endTimer();

    inline void setTotalIters(unsigned int newval) { _total_iters = newval; };
    void incrementIter();

    time_t getHours();
    time_t getMinutes();
    time_t getSeconds();

    int getIterations() { return _iterations; }
    unsigned long getMilliseconds();
    clock_t getDurationClock();
    clock_t getAverageClock();
    time_t getDurationSecs();
    time_t getAverageSecs();

    void estimatedRemaining(std::string &dst);
    std::string &timeToString(std::string &dst);
    
    static const long clocks_per_sec;
    static const long long max_clock;
    static const long clock_rollover_secs;
    
private:
    clock_t _start_c;
    clock_t _end_c;
    time_t _start_t;
    time_t _end_t;

    int _iterations;
    unsigned int _total_iters;
};

} // namespace util
#endif	/* DURATIONTRACKER_H */

