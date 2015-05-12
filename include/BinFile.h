/* 
 * File:   BinFile.h
 * Author: slate
 *
 * Created on April 5, 2013, 9:03 AM
 */

#ifndef BINFILE_H
#define	BINFILE_H

#include <string>

namespace util {
    
class BinFile {
public:
    BinFile();
    BinFile(const BinFile& orig);
    virtual ~BinFile();
    
    unsigned int loadFile(const char *filename);
    unsigned int loadFile(const wchar_t *filename);
    unsigned int loadFile(std::string &filename);
    unsigned int loadFile(std::wstring &filename);
    unsigned int loadFile(std::wifstream &readfile);

    const wchar_t *getData() { return _data; };
    unsigned int size() { return _size; };
    
    unsigned int resizeBuf(unsigned int new_size);
    
private:
    wchar_t *_data;
    unsigned int _buf_size;
    unsigned int _size;
};

} // namespace util

#endif	/* BINFILE_H */

