/* 
 * File:   StdOutStream.cpp
 * Author: root
 * 
 * Created on October 5, 2012, 4:28 PM
 */

#include "stdafx.h"
#include "StdOutStream.h"

using namespace std;

namespace util {
    
StdOutStream::StdOutStream(int max_lvl):OutStream(max_lvl) {
}

StdOutStream::StdOutStream(const StdOutStream& orig):OutStream(orig) {
}

StdOutStream::~StdOutStream() {
}

OutStream &StdOutStream::operator << (bool val) {
    if (_msg_lvl.top() <= _max_lvl) {
        cout << val;
        if (logfile.is_open())
            logfile << val;
    }
    return *this;
}

OutStream &StdOutStream::operator << (short val) {
    if (_msg_lvl.top() <= _max_lvl) {
        cout << val;
        if (logfile.is_open())
            logfile << val;
    }
    return *this;
}

OutStream &StdOutStream::operator << (unsigned short val) {
    if (_msg_lvl.top() <= _max_lvl) {
        cout << val;
        if (logfile.is_open())
            logfile << val;
    }
    return *this;
}

OutStream &StdOutStream::operator << (int val) {
    if (_msg_lvl.top() <= _max_lvl) {
        cout << val;
        if (logfile.is_open())
            logfile << val;
    }
    return *this;
}

OutStream &StdOutStream::operator << (unsigned int val) {
    if (_msg_lvl.top() <= _max_lvl) {
        cout << val;
        if (logfile.is_open())
            logfile << val;
    }
    return *this;
}

OutStream &StdOutStream::operator << (long val) {
    if (_msg_lvl.top() <= _max_lvl) {
        cout << val;
        if (logfile.is_open())
            logfile << val;
    }
    return *this;
}

OutStream &StdOutStream::operator << (unsigned long val) {
    if (_msg_lvl.top() <= _max_lvl) {
        cout << val;
        if (logfile.is_open())
            logfile << val;
    }
    return *this;
}

OutStream &StdOutStream::operator << (float val) {
    if (_msg_lvl.top() <= _max_lvl) {
        cout << val;
        if (logfile.is_open())
            logfile << val;
    }
    return *this;
}

OutStream &StdOutStream::operator << (double val) {
    if (_msg_lvl.top() <= _max_lvl) {
        cout << val;
        if (logfile.is_open())
            logfile << val;
    }
    return *this;
}

OutStream &StdOutStream::operator << (long double val) {
    if (_msg_lvl.top() <= _max_lvl) {
        cout << val;
        if (logfile.is_open())
            logfile << val;
    }
    return *this;
}

OutStream &StdOutStream::operator << (const char *val) {
    if (_msg_lvl.top() <= _max_lvl) {
        cout << val;
        if (logfile.is_open())
            logfile << val;
    }
    return *this;
}

OutStream &StdOutStream::operator << (const void *val) {
    if (_msg_lvl.top() <= _max_lvl) {
        cout << val;
        if (logfile.is_open())
            logfile << val;
    }
    return *this;
}

OutStream &StdOutStream::operator << (const std::string &val) {
    if (_msg_lvl.top() <= _max_lvl) {
        cout << val.c_str();
        if (logfile.is_open())
            logfile << val.c_str();
    }
    return *this;   
}

void StdOutStream::sendMessage(int msgcode, int channel, const char *msg)
{
    // Do nothing for now
}

void StdOutStream::sendMessage(int msgcode, int channel, int msg)
{
    // Do nothing for now
}

} // namespace util