/* 
 * File:   OutStream.h
 * Author: root
 *
 * Created on October 5, 2012, 4:28 PM
 */

#ifndef OUTSTREAM_H
#define	OUTSTREAM_H

#include <stack>
#include <fstream>

namespace util {
    
class OutStream {
public:
    OutStream(int max_lvl);
    OutStream(const OutStream& orig);
    
    virtual ~OutStream();
    
    bool setLogFile(const char *filename);
    
    virtual OutStream &operator << (bool val) { return *this; };
    virtual OutStream &operator << (short val) { return *this; };
    virtual OutStream &operator << (unsigned short val) { return *this; };
    virtual OutStream &operator << (int val) { return *this; };
    virtual OutStream &operator << (unsigned int val) { return *this; };
    virtual OutStream &operator << (long val) { return *this; };
    virtual OutStream &operator << (unsigned long val) { return *this; };
    virtual OutStream &operator << (float val) { return *this; };
    virtual OutStream &operator << (double val) { return *this; };
    virtual OutStream &operator << (long double val) { return *this; };
    virtual OutStream &operator << (const char *val) { return *this; };
    virtual OutStream &operator << (const void *val) { return *this; };
    virtual OutStream &operator << (const std::string &val) { return *this; };
    
    virtual void sendMessage(int msgcode, int channel, const char *msg) {};
    virtual void sendMessage(int msgcode, int channel, int msg) {};
    
    void setMsgPriority(int lvl) {
        _msg_lvl.top() = lvl;
    }
    
    void push_msgpri(int lvl);
    void pop_msgpri();
    
protected:
    OutStream();
    
    std::ofstream logfile;
    
    std::stack<int> _msg_lvl;
    int _max_lvl;
};

} // namespace util
#endif	/* OUTSTREAM_H */

