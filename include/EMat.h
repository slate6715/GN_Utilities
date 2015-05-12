/* 
 * File:   EMAT.h
 * Author: root
 *
 * Created on October 3, 2012, 11:12 AM
 */

#ifndef EMAT_H
#define	EMAT_H

#ifdef USE_OPENCV

#include "opencv2/opencv.hpp"

namespace util {
    
class EMat : public cv::Mat {

public:
    EMat(void):cv::Mat() {};
    EMat(int rows, int cols, int type):cv::Mat(rows, cols, type) {};
    EMat(int rows, int cols, int type, cv::Scalar s);
    EMat(const EMat &orig);
    EMat(const cv::Mat &m):cv::Mat(m) {};
    
    virtual ~EMat(void) {};

    enum pad_dir {pre, post, both};
    enum pad_method {circular, replicate, symmetric};
    
    cv::Mat operator()(cv::Range rowRange, cv::Range colRange, int rowstep=1, int colstep=1);
    cv::Mat operator()(const cv::Rect &roi);
    inline cv::Mat& operator = (const cv::MatExpr& expr) { return cv::Mat::operator = (expr); }
    
    cv::Mat shift(cv::Mat dst, int amount, bool row);
    cv::Mat combine(cv::Mat & src1, cv::Mat & src2, int rowstep=1, int colstep=1);
    
    cv::Mat padarray(int x_padsize, int y_padsize, const cv::Scalar &padval=cv::Scalar(0), pad_dir dir=pre);
    cv::Mat padarray(int x_padsize, int y_padsize, pad_method method, pad_dir dir=pre);
    
    cv::Mat AppendRight(cv::Mat &src);
    cv::Mat AppendRight(cv::Mat &src, cv::Mat &dst);
    
    cv::Mat AppendBottom(cv::Mat &src, cv::Mat &dst);
    cv::Mat AppendBottom(cv::Mat &src);
    
    void SetExpectedSize(int new_rows, int new_cols);
    void TrimEdges(int new_rows, int new_cols);
    cv::Mat Build1DHistMatrix(cv::Mat &dst, double min, double max, int buckets, bool normalize=false);
    
private:
    cv::Mat prepad(int x_padsize, int y_padsize, pad_dir dir, const cv::Scalar &padval);
    
    int CopyElement(uchar *fptr, uchar *tptr);
};

} // namespace util

#endif // def USE_OPENCV
 
#endif	/* EMAT_H */

