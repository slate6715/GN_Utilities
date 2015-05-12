/* 
 * File:   BufferedReader.h
 * Author: root
 *
 * Created on October 2, 2012, 11:15 AM
 */

#ifndef BUFFEREDREADER_H
#define	BUFFEREDREADER_H

#include <string>

namespace util {
    
class BufferedReader {
public:
    BufferedReader(FILE *fptr);
    ~BufferedReader();
    
    bool readLine(std::string &dst);

private:

    bool populateBuffer();
    FILE *_fptr;
    size_t _cur_pos;
    std::string _buffer;
    
};

}

#endif	/* BUFFEREDREADER_H */

