/* 
 * File:   EMAT.cpp
 * Author: root
 * 
 * Created on October 3, 2012, 11:12 AM
 */

#ifdef _WIN32
#include "stdafx.h"
#endif

#ifdef USE_OPENCV

#include "EMat.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace util {
    
EMat::EMat(int rows, int cols, int type, cv::Scalar s) : cv::Mat(rows, cols, type, s)
{
}

cv::Mat EMat::operator()(const cv::Rect &roi)
{
    return cv::Mat::operator()(roi);
}

EMat::EMat(const EMat &orig):cv::Mat((cv::Mat) orig){
    
}


/*
 * operator () - Used to retrieve a submatrix of this matrix (header only).  If rowstep or colstep
 *				 is greater than zero, then this will create a new matrix in memory where some of
 *               the elements have been eliminated (such as A[1,2,10] in Matlab)
 *
 *		Params: rowRange - Range specifying the new submatrix
 *				colRange - new submatrix dims
 *				rowstep - How much to skip between elements
 *				colstep - how many cols to skip between elements
 *
 *		Returns:  Pointer to the resulting matrix
 */

cv::Mat EMat::operator()(cv::Range rowRange, cv::Range colRange, int rowstep, int colstep)
{
    assert((rowstep > 0) && (colstep > 0));

    cv::Mat ranged_mat = cv::Mat::operator()(rowRange, colRange);
    if ((rowstep == 1) && (colstep == 1))
        return ranged_mat;

    int new_cols = (ranged_mat.cols / colstep) + ((ranged_mat.cols % colstep) && 1);
    int new_rows = (ranged_mat.rows / rowstep) + ((ranged_mat.rows % rowstep) && 1);

    cv::Mat newmat(new_rows, new_cols, type());
    uchar *tptr = newmat.data;
    for (int i=0; i<ranged_mat.rows; i+=rowstep)
    {
        uchar *fptr = ranged_mat.ptr(i);
        for (int j=0; j<newmat.cols; j++)
        {
            int increment = CopyElement(fptr, tptr);
            tptr += increment;  fptr += increment; 
            fptr += ((colstep-1)*ranged_mat.elemSize());
        }
    }
    return newmat;
}

/*
 * shift - Shifts the matrix data by the amount specified in row, wrapping the data that falls
 *	       off the end.  Set amount high to simulate shifting left.
 *
 *		Params:  dst - the destination matrix for the new value
 *				 amount - How much to shift right
 *				 row - are we shifting rows or columns?
 *
 *		Returns:  Pointer to the resulting matrix
 */

cv::Mat EMat::shift(cv::Mat dst, int amount, bool row) {
    cv::Mat newmat(rows, cols, type());

    if (row) {
        assert(amount < cols);

        for (int i = 0; i < rows; i++) {
            uchar *fptr = ptr(i) + (amount * elemSize());
            uchar *tptr = newmat.ptr(i);
            for (int j = 0; j < cols; j++) {
                int increment = CopyElement(fptr, tptr);
                fptr += increment;
                tptr += increment;
            }

            // Start over our from pointer
            if (++amount > cols)
                fptr = ptr(i);
        }
    } else {
        assert(amount < rows);

        int offset = amount;
        for (int i = 0; i < rows; i++) {
            // wrap our rows
            if (offset + i >= rows)
                offset = -i;

            uchar *fptr = ptr(i);
            uchar *tptr = newmat.ptr(i + offset);

            for (int j = 0; j < cols; j++) {
                int increment = CopyElement(fptr, tptr);
                fptr += increment;
                tptr += increment;
            }
        }
    }

    // Add column code here later on (it's a bit messier)

    dst = newmat;
    return dst;
}

/*
 * CopyElement - internal use, copies bytes of an element in a matrix, no matter the type.
 */

int EMat::CopyElement(uchar *fptr, uchar *tptr) {
    int increment = 0;
    for (unsigned int k = 0; k < elemSize(); k++) {
        *tptr = *fptr;
        tptr++;
        fptr++;
        increment++;
    }
    return increment;
}

/* 
 * combine - merge two matrices together by alternating between the two matrices (interlace)
 *
 *		Params:		src1 - matrix 1 - dimensions must match
 *					src2 - matrix 2 - dimensions must match
 *					rowstep - how many rows from a matrix to copy before switching to the other
 *							  matrix.  Set equal to zero to not alternate
 *					colstep - how many columns from a matrix to copy before switching to the other
 *							  matrix.  Set equal to zero to not alternate
 *
 *		Returns:  pointer to this EMat cv::Mat matrix, the results
 */

cv::Mat EMat::combine(cv::Mat &src1, cv::Mat &src2, int rowstep, int colstep) {
    assert((rowstep >= 0) && (colstep >= 0) && (src1.type() == src2.type()));
    assert((src1.rows == src2.rows) && (src1.cols == src2.cols));
    assert((rowstep != 0) || (colstep != 0));

    create(src1.rows + src1.rows * (rowstep && 1), src1.cols + src1.cols * (colstep && 1), src1.type());
    uchar *tptr = data;

    int i, j, k, increment;
    cv::Mat *first = &src1;
    cv::Mat *second = &src2;
    for (i = 0; i < src1.rows; i++) {
        uchar *firstptr = first->ptr(i);
        uchar *secondptr = second->ptr(i);
        if (colstep != 0) {
            for (j = 0; j < src1.cols; j += colstep) {
                for (k = 0; k < colstep; k++) {
                    increment = CopyElement(firstptr, tptr);
                    tptr += increment;
                    firstptr += increment;
                }
                for (k = 0; k < colstep; k++) {
                    increment = CopyElement(secondptr, tptr);
                    tptr += increment;
                    secondptr += increment;
                }
            }
            if ((rowstep != 0) && ((i + 1) % rowstep)) {
                cv::Mat *tmp = first;
                first = second;
                second = tmp;
            }
        } else // Rowstep != 0 but colstep does
        {
            for (int j = 0; j < src1.cols; j++) {
                increment = CopyElement(firstptr, tptr);
                tptr += increment;
                firstptr += increment;
            }
            for (int j = 0; j < src2.cols; j++) {
                increment = CopyElement(secondptr, tptr);
                tptr += increment;
                secondptr += increment;
            }
        }
    }

    return *this;
}

/*
 * prepad - Internal function to create a matrix with the old matrix in the appropriate locale
 */

cv::Mat EMat::prepad(int x_padsize, int y_padsize, pad_dir dir, const cv::Scalar &padval) {
    cv::Mat new_mat;
    assert(x_padsize || y_padsize);

    // Create a new matrix of the appropriate size and initialize it
    if ((dir == pre) || (dir == post))
        new_mat.create(rows + y_padsize, cols + x_padsize, type());
    else
        new_mat.create(rows + (y_padsize * 2), cols + (x_padsize * 2), type());
    new_mat = padval;

    // Place the old matrix in the appropriate location
    cv::Mat target;
    if (dir == post)
        target = new_mat(cv::Rect(0, 0, cols, rows));
    else // Both pre and both have the same target rect
        target = new_mat(cv::Rect(x_padsize, y_padsize, cols, rows));

    copyTo(target);

    return new_mat;
}

/* 
 * padarray - adds padding on either the beginning of the rows/cols or the end, or both
 *
 *		Params:		x_padsize - size of the pad along the x direction from a particular side
 *					y_padsize - y direction pad
 *					method - How to pad.  Potential methods include:
 *						circular - Not implemented yet
 *						replicate - not implemented yet
 *						symmetric - mirrored data from the image
 *					dir - sides to add the pad.
 *						pre - padding at the beginning of the x or y
 *						post - padding at the end of the x or y
 *						both - pre and post
 *					padval - scalar value to use for the padding
 */

cv::Mat EMat::padarray(int x_padsize, int y_padsize, const cv::Scalar &padval, pad_dir dir) {
    // Generate a matrix with adequate padding
    cv::Mat new_mat = prepad(x_padsize, y_padsize, dir, padval);

    int padmult = 1;
    if (dir == both)
        padmult = 2;
    assert((new_mat.rows == (rows + (y_padsize * padmult))) && (new_mat.cols + (x_padsize * padmult)));

    // Since this function does a simple scalar padding, we're done
    *this = new_mat;
    return new_mat;
}

cv::Mat EMat::padarray(int x_padsize, int y_padsize, pad_method method, pad_dir dir) {
    // Create a new matrix of the appropriate size and initialize it
    cv::Mat new_mat;
    if ((dir == pre) || (dir == post))
        new_mat.create(rows + y_padsize, cols + x_padsize, type());
    else
        new_mat.create(rows + (y_padsize * 2), cols + (x_padsize * 2), type());

    int padmult = 1;
    if (dir == both)
        padmult = 2;
    assert((new_mat.rows == (rows + (y_padsize * padmult))) && (new_mat.cols + (x_padsize * padmult)));

    int dsize = elemSize();
    // Mirror our matrix
    if (method == symmetric) {
        // for each line, we have a reverse, followed by a forward, etc.  We just have to find a good starting point

        // First find a direction:
        int start_x_dir, x_dir, y_dir;
        int start_x, cur_x, cur_y;

        // Now find our starting position in our source matrix
        if ((x_padsize / cols) % 2) // If we have an odd multiple (plus some), go Forward
        {
            start_x_dir = 1;
            start_x = cols - (x_padsize % cols) - 1;
        } else {
            start_x_dir = -1;
            start_x = (x_padsize % cols) - 1;
        }

        if ((y_padsize / rows) % 2) {
            y_dir = 1;
            cur_y = rows - (y_padsize % rows) - 1;
        } else {
            y_dir = -1;
            cur_y = (y_padsize % rows) - 1;
        }

        // Loop through every matrix of the old matrix
        uchar *fptr, *tptr;
        for (int i = 0; i < new_mat.rows; i++) {
            // Always reset initial conditions for x values
            cur_x = start_x;
            x_dir = start_x_dir;

            // New row for the to matrix
            tptr = new_mat.ptr(i);

            fptr = ptr(cur_y) + (cur_x * dsize);

            for (int j = 0; j < new_mat.cols; j++) {
                // Copy the element and increment applicable pointers
                CopyElement(fptr, tptr);
                tptr += dsize;

                // Handle incrementing from row ptr and reversing directions if necessary
                cur_x += x_dir;
                if ((cur_x == cols) || (cur_x < 0)) {
                    x_dir = x_dir * (-1); // reverse direction
                    cur_x += x_dir; // Go back into safe areas
                }
                fptr = ptr(cur_y) + (cur_x * dsize);
            }

            // Handle incrementing from matrix column and reversing if necessary
            cur_y += y_dir;
            if ((cur_y == rows) || (cur_y < 0)) {
                y_dir = y_dir * (-1); // reverse direction
                cur_y += y_dir; // Go back into safe areas
            }
        }
    } else if (method == circular)
        assert(0); // Not coded yet
    else if (method == replicate)
        assert(0); // Not coded yet
    else
        assert(0); // Probably should raise an exception here

    *this = new_mat;
    return new_mat;
}

/* 
 * AppendBottom - Append one matrix to the bottom of another.  Cols must match
 *
 *		Params:		src - the matrix to add, with matching col dimensions
 */

cv::Mat EMat::AppendBottom(cv::Mat &src, cv::Mat &dst) {
    assert(src.cols == cols);

    if ((dst.cols < src.cols) || (dst.rows < src.rows + rows))
        dst.create(src.rows + rows, src.cols, type());

    cv::Mat top = dst(cv::Range(0, rows), cv::Range(0, cols));
    cv::Mat bottom = dst(cv::Range(rows, src.rows + rows), cv::Range(0, cols));

    copyTo(top);
    src.copyTo(bottom);
    return dst(cv::Range(0, src.rows + rows), cv::Range(0, cols));
}

cv::Mat EMat::AppendBottom(cv::Mat &src) {
    assert(src.cols == cols);

    cv::Mat newMat(src.rows + rows, cols, type());
    AppendBottom(src, newMat);
    *this = newMat;
    return newMat;
}

/* 
 * AppendRight - Append one matrix to the right side of another.  Rows must match
 *
 *		Params:		src - the matrix to add, with matching row dimensions
 */

cv::Mat EMat::AppendRight(cv::Mat &src, cv::Mat &dst) {
    assert(src.rows == rows);

    if ((dst.rows < src.rows) || (dst.cols < src.cols + cols))
        dst.create(src.rows, src.cols + cols, type());

    cv::Mat left = dst(cv::Range(0, src.rows), cv::Range(0, cols));
    cv::Mat right = dst(cv::Range(0, src.rows), cv::Range(cols, src.cols + cols));

    copyTo(left);
    src.copyTo(right);
    return dst(cv::Range(0, rows), cv::Range(0, src.cols + cols));
}

cv::Mat EMat::AppendRight(cv::Mat &src) {
    assert(src.rows == rows);

    cv::Mat newMat(rows, src.cols + cols, type());
    AppendRight(src, newMat);
    *this = newMat;
    return newMat;
}

/*
 * TrimEdges - Chops off equal bits to result in the given dimensions
 *
 *		Params:	new_rows, new_cols - the new rows and cols to bring the image to
 *
 *		Note:  Given uneven extra, removes more from the tail than the end
 *
 */

void EMat::TrimEdges(int new_rows, int new_cols) {
    assert((new_rows <= rows) && (new_cols <= cols));

    int y_diff = rows - new_rows;
    int x_diff = cols - new_cols;

    int x_chop = x_diff / 2;
    int y_chop = y_diff / 2;

    adjustROI(-y_chop, -(y_chop + (y_diff % 2)), -x_chop, -(x_chop + (x_diff % 2)));
}

cv::Mat EMat::Build1DHistMatrix(cv::Mat &dst, double min, double max, int buckets, bool normalize) {
    // assert(rows == 1);

    const int channels[] = {0}; // Only one channel
    const int sizes[] = {buckets};
    const float hrange[] = {(float) min, (float) max + 1}; // dynamically determined from min max value in the data
    const float *ranges[] = {hrange};

    int back_type = -1;
    if (type() != CV_32FC1) {
        back_type = type();
        convertTo(*this, CV_32F);
    }
    cv::calcHist(this, 1, channels, cv::Mat(), dst, 1, sizes, ranges, true, false);

    if (back_type != -1)
        convertTo(*this, back_type);

    // Now normalize it using total count and bucket width
    if (normalize) {
        for (int i = 0; i < dst.rows; i++) {
            dst.at<float>(i) = dst.at<float>(i) /= rows;
        }
    }

    return dst;
}


} // namespace util

#endif // def USE_OPENCV
