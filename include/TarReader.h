/* 
 * File:   TarReader.h
 * Author: slate
 *
 * Created on October 30, 2012, 8:29 AM
 */

#ifndef TARREADER_H
#define	TARREADER_H

#include <cstring>
#include <stdio.h>
#include <list>


namespace util {

class TarFile;    
    
class TarReader
{
public:
	TarReader(void);
	virtual ~TarReader(void);

	void addData(unsigned char *data, size_t newdatasize);
	bool loadTarfile(const char *filename, int number);
	TarFile *popNext();
	void reset();

	inline int numLoaded() { return _num_read; };
	int parseData();
	int writeAndPop(FILE *outfile, int max_loaded);

private:
	int parseTarfile();
	void resetBuffer();

	unsigned char *_buffer;
	size_t _datasize;
	size_t _bufsize;

	size_t _readpos;

	std::list<TarFile *> _loaded;
	int _num_read;
};

} // namespace util

#endif	/* TARREADER_H */

