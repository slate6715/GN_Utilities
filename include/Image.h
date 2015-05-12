/* 
 * File:   Image.h
 * Author: root
 *
 * Created on October 26, 2012, 7:45 AM
 */

#ifndef IMAGE_H
#define	IMAGE_H

#ifdef USE_OPENCV

#include <opencv2/opencv.hpp>
#include <memory>
#include "DBase.h"

namespace util {

class DBase;

class Image {
public:
    Image();
    ~Image(void);
    inline void setPath(const char *new_path) { _img_path = new_path; };
    inline void setName(const char *new_name) { _name = new_name; };
    inline void setLocation(int new_loc) { _location = new_loc; };
    inline void setDocID(const int new_docID) { _docID = new_docID; };
    inline void setImgID(const int new_imgID) { _imgID = new_imgID; };
   // inline void SetXY(int new_x, int new_y) { x_size = new_x; y_size = new_y; };
    void setImage(cv::Mat img, const char *path);

    void loadData(util::DBase &conn, unsigned int imgID);
    void loadData(std::unique_ptr<util::UseQueryResults> &rs);
    void loadData(std::unique_ptr<util::StoreQueryResults> &rs, unsigned int idx);
    bool loadImage(const char *prefix);
    bool loadImage(const char *prefix, const char *path);

    inline void unloadImage() { _image.release(); };

//    mysqlpp::UseQueryResult getSignaturesRS(DBaseConn &conn, const char *sigID = NULL);
//    Signature *loadSignature(DBaseConn &conn, const char *sigID, NDistSig &nsig, MNDistSig &mnsig, SectorMeanSig &ssig);

    inline int getXSize() { return _x_size; };
    inline int getYSize() { return _y_size; };
    inline const char *getPath() { return _img_path.c_str(); };
    inline int getLocation() { return _location; };
    inline const char *getName() { return _name.c_str(); };
    inline int getImgID() { return _imgID; };
    inline int getDocID() { return _docID; };
    inline int getCategory() { return _category; };

    inline cv::Mat &getImage() { return _image; };

protected:
    cv::Mat _image;

private:

    //	std::map<CString, Signature *, wltstr> sigs;
    //	std::map<CString, Signature *, wltstr>::iterator iter;
    // mysqlpp::UseQueryResult _siglist;

    unsigned int _x_size;
    unsigned int _y_size;

    unsigned int _imgID;
    unsigned int _docID;
    std::string _img_path;
    std::string _name;
    unsigned int _location;
    unsigned int _category;
};

} // namespace util

#endif	/* IMAGE_H */

#endif // def USE_OPENCV