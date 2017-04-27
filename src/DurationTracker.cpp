/* 
 * File:   DurationTracker.cpp
 * Author: root
 * 
 * Created on October 10, 2012, 1:29 PM
 */

#ifdef _WIN32
#include "stdafx.h"
#endif
#include "DurationTracker.h"
#include <cassert>
#include <ctime>
#include <limits>
#include <sstream>

#ifdef WIN32
#undef max
#endif

namespace util {
    
const long DurationTracker::clocks_per_sec = CLOCKS_PER_SEC;
const long long DurationTracker::max_clock = std::numeric_limits<clock_t>::max();
const long DurationTracker::clock_rollover_secs = (long)
                        DurationTracker::max_clock / (long long) CLOCKS_PER_SEC;
 
DurationTracker::DurationTracker(void) {
    _start_c = _end_c = 0;
    _start_t = _end_t = 0;
    _iterations = 0;
}

DurationTracker::DurationTracker(const DurationTracker &orig):
                        _start_c(orig._start_c),
                        _end_c(orig._end_c),
                        _start_t(orig._start_t),
                        _end_t(orig._end_t),
                        _iterations(orig._iterations),
                        _total_iters(orig._total_iters)
{
    
}

DurationTracker::~DurationTracker(void) {
}

void DurationTracker::incrementIter() {
    assert(_start_c != 0);

    _iterations++;
}

void DurationTracker::startTimer() {
    _start_c = clock();
    _start_t = time(NULL);
}

void DurationTracker::endTimer() {
    _end_c = clock();
    _end_t = time(NULL);
}

time_t DurationTracker::getHours() {
    return ((_end_t - _start_t) / 3600);
}

time_t DurationTracker::getMinutes() {
    return ((_end_t - _start_t) / 60);
}

time_t DurationTracker::getSeconds() {
    return (_end_t - _start_t);
}

unsigned long DurationTracker::getMilliseconds() {
    
    if ((_end_t - _start_t) > clock_rollover_secs)     // Clock rollover reached
        return 0;
    
    return (_end_c - _start_c) / ((unsigned long) (clocks_per_sec / 1000));
}

clock_t DurationTracker::getDurationClock() {
    return (_end_c - _start_c);
}

clock_t DurationTracker::getAverageClock() {
    if (_iterations == 0)
        return 0;

    return (_end_c - _start_c) / _iterations;
}
time_t DurationTracker::getDurationSecs() {
    return (_end_t - _start_t);
}

time_t DurationTracker::getAverageSecs() {
    if (_iterations == 0)
        return 0;

    return (_end_t - _start_t) / _iterations;
}

void DurationTracker::estimatedRemaining(std::string &dst) {
    if (_iterations == 0) {
        dst = "Unk";
        return;
    }

    std::stringstream timestr;
    time_t expended = (time(NULL) - _start_t);
    time_t duration = expended / (long) _iterations;

    time_t remaining = duration * (long) (_total_iters - _iterations);

    time_t hours = (remaining / 3600L);
    remaining -= (hours * 3600L);
    time_t minutes = (remaining / 60L);
    remaining -= (minutes * 60L);
    time_t seconds = (remaining);

    timestr << hours << ":" << minutes << ":" << seconds;
    dst = timestr.str();
}


std::string &DurationTracker::timeToString(std::string &dst) {
    std::stringstream timestr;
    time_t endtime = _end_t;
    if (endtime == 0) {
        endtime = time(NULL);
    }
    
    time_t expended = (endtime - _start_t);

    time_t hours = (expended / 3600L);
    expended -= (hours * 3600L);
    time_t minutes = (expended / 60L);
    expended -= (minutes * 60L);
    time_t seconds = expended;

    timestr << hours << ":" << minutes << ":" << seconds;
    
    dst = timestr.str();
    return dst;
}

} //namespace util
