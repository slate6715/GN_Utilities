/* 
 * File:   StdOutStream.h
 * Author: root
 *
 * Created on October 5, 2012, 4:28 PM
 */

#ifndef STDOUTSTREAM_H
#define	STDOUTSTREAM_H

#include "OutStream.h"

namespace util {
    
class StdOutStream : public OutStream {
public:
    StdOutStream(int max_lvl);
    StdOutStream(const StdOutStream& orig);
    virtual ~StdOutStream();
    
    virtual OutStream &operator << (bool val);
    virtual OutStream &operator << (short val);
    virtual OutStream &operator << (unsigned short val);
    virtual OutStream &operator << (int val);
    virtual OutStream &operator << (unsigned int val);
    virtual OutStream &operator << (long val);
    virtual OutStream &operator << (unsigned long val);
    virtual OutStream &operator << (float val);
    virtual OutStream &operator << (double val);
    virtual OutStream &operator << (long double val);
    virtual OutStream &operator << (const char *val);
    virtual OutStream &operator << (const void *val);
    virtual OutStream &operator << (const std::string &val);

    virtual void sendMessage(int msgcode, int channel, const char *msg);
    virtual void sendMessage(int msgcode, int channel, int msg);

private:

};

} // namespace util

#endif	/* STDOUTSTREAM_H */

