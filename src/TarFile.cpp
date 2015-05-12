/* 
 * File:   TarFile.cpp
 * Author: slate
 * 
 * Created on October 30, 2012, 8:38 AM
 */

#include "TarFile.h"
#include "miscutils.h"

namespace util {
    
TarFile::TarFile(void) {
    filename[0] = '\0';
    filemode = 0;
    ownerid = 0;
    groupid = 0;
    filesize = 0;
    lastmod = 0;
    checksum = 0;
    type_id = 0;
    linkedfile[0] = '\0';

    ustar[0] = '\0';
    ustar_ver[0] = '\0';
    owner_name[0] = '\0';
    group_name[0] = '\0';
    dev_majnum = 0;
    dev_minnum = 0;
    filename_prefix[0] = '\0';

    filedata = NULL;
    datasize = 0;

    written = false;
}

TarFile::~TarFile(void) {
    if (filedata != NULL)
        delete[] filedata;
}

bool TarFile::loadData(const unsigned char *data, size_t data_size) {
    if (data_size <= 512)
        throw util::RuntimeException("(TarFile:LoadData) Data size was not large enough to include a header and data");

    filesize = extractULong(data, 124, 135);
    if (data_size != filesize + 512)
        throw util::RuntimeException("(TarFile:LoadData) File size does not match that indicated in the header.");

    if (filesize <= 0)
        throw util::RuntimeException("(TarFile::LoadData) File size was <= 0");

    // tar header information
    extractChar(data, filename, 0, 100);
    filemode = extractUInt(data, 100, 8);
    ownerid = extractUInt(data, 108, 8);
    groupid = extractUInt(data, 116, 8);
    lastmod = extractULong(data, 136, 12);
    checksum = extractULong(data, 148, 8);
    type_id = (unsigned char) extractUInt(data, 156, 1);
    extractChar(data, linkedfile, 157, 100);
    extractChar(data, ustar, 257, 6);
    extractChar(data, ustar_ver, 263, 2);
    extractChar(data, owner_name, 265, 32);
    extractChar(data, group_name, 297, 32);
    dev_majnum = extractUInt(data, 329, 8);
    dev_minnum = extractUInt(data, 337, 8);
    extractChar(data, filename_prefix, 345, 155);

    filedata = new unsigned char[data_size - 512 + 1];
    datasize = data_size - 512;

    memcpy(header, data, 512);
    memcpy(filedata, data + 512, datasize);
    filedata[datasize] = '\0';

    // Check checksum (figure this out later)
    //	if (!TestChecksum(filedata, checksum))
    //		throw RuntimeException("(TarFile:LoadData) Checksum does not match header checkcsum.");
    return true;
}

unsigned long TarFile::extractULong(const unsigned char *data, size_t startpos, size_t length) {
    // Check the file size and see if we have enough
    char *begin = (char *) data + startpos;
    char *end = (char *) data + startpos + length;
    return strtoul(begin, &end, 0);
}

unsigned int TarFile::extractUInt(const unsigned char *data, size_t startpos, size_t length) {
    // Check the file size and see if we have enough
    char *begin = (char *) data + startpos;
    char *end = (char *) data + startpos + length;
    return (unsigned int) strtoul(begin, &end, 0);
}

void TarFile::extractChar(const unsigned char *data, char *dst, size_t startpos, size_t length) {
    // Check the file size and see if we have enough
    char *begin = (char *) data + startpos;
    memcpy(dst, begin, length);
}

bool TarFile::testChecksum(const unsigned char *header, unsigned long testsum) {
    // Doesn't match it yet
    char *charptr = (char *) header;
    unsigned char *ucharptr = (unsigned char *) header;
    int sum = 0;
    unsigned int usum = 0;
    for (int i = 0; i <= 256; i++) {
        if ((i >= 148) && (i < 156)) {
            usum += 32;
            sum += 32;
        } else {
            sum += charptr[i];
            usum += ucharptr[i];
        }
    }
    return (((long) testsum == sum) || (testsum == usum));

    // Wrong checksum implementation
    /*	unsigned short r = 55665;
            unsigned short c1 = 52845;
            unsigned short c2 = 22719;
            unsigned char cipher;
            unsigned long sum = 0;;
            for (int i=0; i<datasize; i++)
            {
                    cipher = (data[i] ^ (r >> 8));
                    r = (cipher + r) * c1 + c2;
                    sum += cipher;
            }
            return sum;*/
}

#ifdef USE_OPENCV

void TarFile::buildMatrix(cv::Mat &dst) {
    cv::Mat rawdata;
    rawdata.create(1, filesize, CV_8U);
    unsigned char *ptr = rawdata.ptr<unsigned char>();
    memcpy(ptr, filedata, filesize);
    dst = cv::imdecode(rawdata, 1);
}

#endif

size_t TarFile::write(FILE *outfile) {
    char nulls[512];
    memset(nulls, 0, 512);

    if (outfile == NULL)
        throw util::RuntimeException("(TarFile::Write) Outfile was not open.");

    fwrite(header, sizeof (char), 512, outfile);
    fwrite(filedata, sizeof (char), filesize, outfile);
    size_t fillsize = 512 - (filesize % 512);
    if (fillsize != 512)
        fwrite(nulls, sizeof (char), fillsize, outfile);
    written = true;
    return filesize + fillsize + 512;
}

} // namespace util