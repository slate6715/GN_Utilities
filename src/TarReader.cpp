/* 
 * File:   TarReader.cpp
 * Author: slate
 * 
 * Created on October 30, 2012, 8:29 AM
 */

#ifdef _WIN32
#include "stdafx.h"
#endif
#include "TarReader.h"
#include "TarFile.h"
#include <cstdlib>
#include "miscutils.h"

namespace util {
    
TarReader::TarReader(void) {
    _buffer = NULL;
    _datasize = 0;
    _bufsize = 0;

    _readpos = 0;
    _num_read = 0;
}

TarReader::~TarReader(void) {
    reset();
}

void TarReader::reset() {
    if (_buffer != NULL)
        delete[] _buffer;
    _buffer = NULL;

    while (_loaded.size() > 0) {
        delete _loaded.front();
        _loaded.pop_front();
    }
    _datasize = 0;
    _bufsize = 0;

    _readpos = 0;
    _num_read = 0;
}

#define BUFSIZE_INCREASE	65536		// Increase by a meg each time

void TarReader::addData(unsigned char *data, size_t new_datasize) {
    // If we need to grow the _buffer
    if (_datasize + new_datasize > _bufsize) {
        unsigned char *tmpbuf = _buffer;
        _bufsize = _datasize + new_datasize + BUFSIZE_INCREASE;
        _buffer = new unsigned char[_bufsize];
        if (tmpbuf != NULL) {
            memcpy(_buffer, tmpbuf, _datasize);
            delete[] tmpbuf;
        }
    }

    // Copy the new data into the _buffer
    memcpy(_buffer + _datasize, data, new_datasize);
    _datasize = _datasize + new_datasize;
}

int TarReader::parseTarfile() {
    // The file header is at least 512 bytes, so return false if we don't have enough yet
    if (_readpos + 512 > _datasize)
        return 0;

    // Check the file size and see if we have enough
    char *begin = (char *) _buffer + _readpos + 124;
    char *end = (char *) _buffer + _readpos + 135;
    unsigned long filesize = strtoul(begin, &end, 0);

    // If the filesize is zero, we've (probably) reached the two empty records marking the end
    if (filesize == 0)
        return 0;

    if (filesize + 512 > _datasize - _readpos)
        return 0;

    // Read in the Tarfile details here
    TarFile *newfile = new TarFile();

    if (!newfile->loadData(_buffer + _readpos, filesize + 512)) {
        delete newfile;
        return 0;
    }
    _loaded.push_back(newfile);
    _num_read++;

    // Distance to the next file should be set to the next 512 bytes
    size_t multiple = ((size_t) filesize / 512) + 1; // Data blocks plus header
    if ((filesize % 512) > 0) // Account for the extra block (if not perfectly divisible)
        multiple += 1;
    //	size_t checkpos = _readpos + filesize + 512;
    _readpos = _readpos + (multiple * 512);

    // Temporary check of what's in the tail data
    /*	int charval;
            while (checkpos != _readpos)
            {
                    charval = _buffer[checkpos];
                    checkpos++;
            }*/
    return 1;
}

bool TarReader::loadTarfile(const char *filename, int number) {
    FILE *readfile = NULL;
    unsigned char read_buffer[BUFSIZE_INCREASE];

#ifdef _WIN32
	fopen_s(&readfile, filename, "rb");
	if (readfile == NULL)
		return false;
#else
    if ((readfile = fopen(filename, "rb")) == NULL)
        return false;
#endif

    size_t sizeread;
    while (((sizeread = fread(read_buffer, sizeof (unsigned char), BUFSIZE_INCREASE,  
                                                readfile)) > 0) && (_num_read < number)) {
        if (_readpos > _datasize)
            throw util::RuntimeException("(TarReader:LoadTarfile) Readpos was greater than the amount of data in the _buffer");

        addData(read_buffer, sizeread);
        while ((_num_read < number) && (parseTarfile()));
        if ((_readpos != 0) && (_readpos < _datasize)) // This may not happen if we reach end of file
            resetBuffer();
    }
/*
    size_t check = _readpos;
    int charval = 0;
    while (check < _datasize) {
        charval = _buffer[check];
        check++;
    }*/
    fclose(readfile);

    return true;
}

TarFile *TarReader::popNext() {
    if (_loaded.size() <= 0)
        return NULL;

    TarFile *retval = _loaded.front();
    _loaded.pop_front();
    return retval;
}

// Move the rest of the data to the beginning of the _buffer to prep for reading more in

void TarReader::resetBuffer() {
    memmove(_buffer, _buffer + _readpos, _datasize - _readpos);
    _datasize -= _readpos;
    _readpos = 0;
}

int TarReader::parseData() {
    int _num_read = 0;
    while (parseTarfile() > 0)
        _num_read++;

    if ((_readpos != 0) && (_readpos < _datasize)) // This may not happen if we reach end of file
        resetBuffer();
    return _num_read;
}

int TarReader::writeAndPop(FILE *outfile, int max_loaded) {
    std::list<TarFile *>::iterator search = _loaded.begin();
    int count = 0;

    for (; search != _loaded.end(); search++)
        if (!(*search)->isWritten()) {
            (*search)->write(outfile);
            count++;
        }

    // Pop off any images that we're writing, but not storing
    TarFile *delptr;
    while ((int) _loaded.size() > max_loaded) {
        delptr = _loaded.back();
        _loaded.pop_back();
        delete delptr;
    }
    return count;
}

} // namespace util
