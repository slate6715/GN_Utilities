/* 
 * File:   BinFile.cpp
 * Author: slate
 * 
 * Created on April 5, 2013, 9:03 AM
 */

#pragma once

#include "stdafx.h"
#include "BinFile.h"

namespace util {
   
#define GROW_PERCENT 0.10
    
BinFile::BinFile() {
    _data = NULL;
    _size = _buf_size = 0;
}

BinFile::BinFile(const BinFile& orig) {
    _data = NULL;
    _size = _buf_size = 0;
}

BinFile::~BinFile() {
    if (_data != NULL) {
        delete[] _data;
    }
}

unsigned int BinFile::resizeBuf(unsigned int new_size) {
    
    // Can't resize the buffer smaller than the loaded memory
    if ((new_size < _size) || (new_size == 0)) {
        return _buf_size;
    }
    
    wchar_t *old_buf = _data;
    _data = new wchar_t[new_size];
    memset(_data, 0, new_size);
    _buf_size = new_size;
    
    
    // Copy the old data if the buffer has any
    if (_size > 0)
        memcpy(_data, old_buf, _size);

    delete[] old_buf;
    return new_size;

}

unsigned int BinFile::loadFile(const char *filename) {
    std::wifstream readfile;
        
    readfile.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
    unsigned int results = loadFile(readfile);
    readfile.close();
    return results;
}

unsigned int BinFile::loadFile(const wchar_t *filename) {
    std::wifstream readfile;
    
    readfile.open(filename, std::ios::in | std::ios::binary | std::ios::ate );
    unsigned int results = loadFile(readfile);
    readfile.close();
    return results;
}

unsigned int BinFile::loadFile(std::wifstream &readfile) {
    
    if (!readfile.is_open()) {
        throw util::IOException("File did not open successfully.");
    }

    int fsize = (int) readfile.tellg();
    
    resizeBuf(fsize + 1);
    readfile.seekg(0, std::ios::beg);
    
    while (!readfile.eof()) {
        if (_size == _buf_size) {
            resizeBuf((unsigned int) (((double) _buf_size) * (1.0 + GROW_PERCENT)));
        }
#ifndef _WIN32
        _size += readfile.read(_data, _buf_size - _size);
#else
		throw util::RuntimeException("BinFile::loadFile - This code not yet ported to Win32");
#endif
    }
    return _size;
}

unsigned int BinFile::loadFile(std::string &filename) {
    return loadFile(filename.c_str());
}

unsigned int BinFile::loadFile(std::wstring &filename) {
    return loadFile(filename.c_str());
}

} // namespace util