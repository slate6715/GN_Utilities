/* 
 * File:   TarFile.h
 * Author: slate
 *
 * Created on October 30, 2012, 8:38 AM
 */

#ifndef TARFILE_H
#define	TARFILE_H

#include <cstring>
#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#endif

#ifdef _WIN32
#include <cstdio>
#endif

namespace util {
    
class TarFile {
public:
    TarFile(void);
    virtual ~TarFile(void);

    bool loadData(const unsigned char *data, size_t data_size);
    unsigned long extractULong(const unsigned char *data, size_t startpos, size_t length);
    unsigned int extractUInt(const unsigned char *data, size_t startpos, size_t length);
    void extractChar(const unsigned char *data, char *dst, size_t startpos, size_t length);
    bool testChecksum(const unsigned char *header, unsigned long testsum);

#ifdef USE_OPENCV
    void buildMatrix(cv::Mat &dst);
#endif

    size_t write(FILE *outfile);

    inline bool isWritten() {
        return written;
    };

    // tar header information
    char filename[100];
    unsigned int filemode;
    unsigned int ownerid;
    unsigned int groupid;
    size_t filesize;
    time_t lastmod;
    unsigned int checksum;
    unsigned char type_id;
    char linkedfile[100];

    char ustar[6];
    char ustar_ver[2];
    char owner_name[32];
    char group_name[32];
    unsigned int dev_majnum;
    unsigned int dev_minnum;
    char filename_prefix[156];

    char header[512];

    unsigned char *filedata;
    size_t datasize;

    bool written;
};

} // namespace util

#endif	/* TARFILE_H */

