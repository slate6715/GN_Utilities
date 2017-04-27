/* 
 * File:   ConfigFile.cpp
 * Author: root
 * 
 * Created on October 1, 2012, 2:13 PM
 */

#ifdef _WIN32
#include "stdafx.h"
#endif
#include <cstring>
#include <algorithm>
#include <stdio.h>
#include "BufferedReader.h"
#include "miscutils.h"

namespace util {
    
BufferedReader::BufferedReader(FILE *fptr):_fptr(fptr) {
    _cur_pos = 0;
}

BufferedReader::~BufferedReader() {
    
}

//----------------------------------------------------------------------------
// readLine - Using a buffered read, gets the next line, terminated by the
//            newline char '\n' and populates dst with it
//
//----------------------------------------------------------------------------

bool BufferedReader::readLine(std::string &dst) {
    // If the iterator is already at the end, we need to read more from the file
    // then reset the iterator
    if (_cur_pos >= _buffer.length()) {
        if (!populateBuffer())
            return false;
    }
    
    // Find the end of this line.  If none was found, we might need more buffer
    size_t eol = _buffer.find('\n', _cur_pos);
    if (eol == std::string::npos)
    {
        if (populateBuffer()) {
            eol = _buffer.find('\n', _cur_pos);
            if (eol == std::string::npos)
                eol = _buffer.length();
        }
    }
    
    // Now get the string
    dst.assign(_buffer.begin() + _cur_pos, _buffer.begin() + eol);
    
    // Strip off any \r chars
    std::remove(dst.begin(), dst.end(), '\r');
    _cur_pos = eol+1;
    return true;
}

//----------------------------------------------------------------------------
// populateBuffer - Private class to fill the buffer with the next chars from
//                  the file
//
//----------------------------------------------------------------------------

#define FREAD_SIZE       1023

bool BufferedReader::populateBuffer() {
    char buf[FREAD_SIZE+1];
    memset(buf, 0, FREAD_SIZE+1);
    
    int result = fread(&buf, sizeof(char), FREAD_SIZE, _fptr);
    if (result < FREAD_SIZE) {
        if (!feof(_fptr)) {
            throw util::IOException("Read error reading file.");
        }
    }
        
    if (result == 0) {
        return false;
    }
    
    std::string newstr(_buffer.begin() + _cur_pos, _buffer.end());
    newstr.append(buf);
    _buffer.swap(newstr);
    _cur_pos = 0;
    
    return true;
}

} // namespace util

