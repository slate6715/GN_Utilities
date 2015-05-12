/* 
 * File:   BufferedInsert.h
 * Author: root
 *
 * Created on October 3, 2012, 2:06 PM
 */

#ifndef BUFFEREDINSERT_H
#define	BUFFEREDINSERT_H

#include "DBase.h"

namespace util {
    
#define BUFFER_SIZE 1000
    
class BufferedInsert {
public:
    BufferedInsert(DBase &conn, const char *preface, const char *postface = ";");
    BufferedInsert(const BufferedInsert& orig);
    ~BufferedInsert(void);

    bool insertValues(const char *values);
    bool insertValues(std::string &values);
    void flush();
    
    void setBufferSize(int new_size) { buf_size = new_size; };

private:
    DBase &_conn;
    std::string _preface;
    std::string _postface;

    std::string _buf;
    int count;
    
    int buf_size;
    
private:

};

} // namespace util

#endif	/* BUFFEREDINSERT_H */

