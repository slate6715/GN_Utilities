/* 
 * File:   BufferedInsert.cpp
 * Author: root
 * 
 * Created on October 3, 2012, 2:06 PM
 */

#include "BufferedInsert.h"
#include <memory>

namespace util {
    
BufferedInsert::BufferedInsert(DBase &conn, const char *preface, const char *postface) :
                                                  _conn(conn)
                                                , _preface(preface)
                                                , _postface(postface) {
    buf_size = BUFFER_SIZE;
    count = 0;
    _buf.reserve(buf_size * 50);
    _buf = preface;
}

BufferedInsert::BufferedInsert(const BufferedInsert& orig):
                        _conn(orig._conn)
                        , _preface(orig._preface)
                        , _postface(orig._postface)
                        , _buf(orig._buf)
                        , buf_size(orig.buf_size)
{
}

BufferedInsert::~BufferedInsert(void) {
}

bool BufferedInsert::insertValues(const char *values) {
    if (count != 0)
        _buf += ", ";

    _buf += values;
    count++;
    if (count > buf_size)
        flush();
    return true;
}

bool BufferedInsert::insertValues(std::string &values) {
    if (count != 0)
        _buf += ", ";

    _buf += values;
    count++;
    if (count > buf_size)
        flush();
    return true;
}

void BufferedInsert::flush() {
    if (!_conn.isConnected())
        throw DBException("BufferedInsert Flush:  Database connection not established.");

    if (count == 0)
        return;

    if (_postface.length() > 0)
        _buf += _postface;

    std::unique_ptr<util::Query> stmt = _conn.getQueryObj();
    *stmt << _buf;
    stmt->execUpdate();

    _buf.clear();
    _buf.reserve(buf_size * 50); // a guess on the size of each value string
    _buf = _preface;
    count = 0;
}

} // namespace util