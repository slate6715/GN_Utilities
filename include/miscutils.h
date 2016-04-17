/* 
 * File:   miscutils.h
 * Author: root
 *
 * Created on October 2, 2012, 11:19 AM
 */

#ifndef MISCUTILS_H
#define	MISCUTILS_H

#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <boost/asio/io_service.hpp>

#if defined(_WIN32) && defined(USE_MFC)
#include <afx.h>
#endif

#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#endif

// #define _T(a)   a
#undef NULL
#define NULL 0

namespace util {

bool isImageFile(const char *filename);
bool isImageFile(const wchar_t *filename);

// WideToString - converts wide strings to standard ascii
//      functions that do not pass in a destination buffer are
//      not thread safe
const char *wideToString(std::wstring &s, std::string &to);
const char *wideToString(std::wstring &s);
const char *wideToString(const wchar_t *s);
const char *wideToString(const wchar_t *s, std::string &to);

// WideToString - converts standard ascii string to wide
//      functions that do not pass in a destination buffer are
//      not thread safe
#if defined(_WIN32) && defined(USE_MFC)
LPCTSTR CharToWide(const char *from, CString &to);
#endif

const wchar_t *charToWide(std::string &s, std::wstring &to);
const wchar_t *charToWide(const char *s, std::wstring &to);
const wchar_t *charToWide(std::string &s);
const wchar_t *charToWide(const char *s);

bool isAlpha(int thechar);
bool isTermPunct(int thechar);

void lowercase(std::string &src, std::string *dst=NULL);
void replace(const char *str1, const char *str2, std::string &src, std::string *dst=NULL);

#ifndef WIN32

void sprintf_s(const char *str, ...);
char *strtok_s(char *strToken, const char *strDelimit, char **context);
char *strcpy_s(char *strDestination, size_t numberOfElements, const char *strSource);
char *strncpy_s(char *strDestination, const char *strSource, int count);

#endif

void split(const std::string &src, char divider, std::vector<std::string> &dst, char quotechar=0);
std::string &trim(std::string &src, std::string &dst);

#ifdef USE_OPENCV
float vecAngle(cv::Mat vec1, cv::Mat vec2);
float vecCosine(cv::Mat vec1, cv::Mat vec2);
float vecDist(cv::Mat vec);

double logVecGausPDF(cv::Mat vec, cv::Mat mean, cv::Mat stddev);
#endif

double pdf(double x, double mean, double stddev);

// int ExtractIntBetween(LPCTSTR left, CString &line, LPCTSTR right);

// struct used in comparison stl classes (key comparison and the like)
/* struct wltstr
{
	bool operator() (LPCTSTR s1, LPCTSTR s2) const
	{
		return StrCmp(s1, s2) < 0;
	}
};

struct ltstr
{
	bool operator() (const std::string &s1, const std::string &s2) const
	{
		return s1.compare(s2) < 0;
	}
};*/

class IOException : public std::exception
{
public:
    IOException(std::string msg):_msg(msg) {};
    ~IOException() throw() {};
    const char *what() { return _msg.c_str(); };
    
private:
    std::string _msg;
};

class RuntimeException : public std::exception
{
public:
    RuntimeException(std::string msg):_msg(msg) {};
    ~RuntimeException() throw() {};
    const char *what() { return _msg.c_str(); };
    
private:
    std::string _msg;
};

class NetworkException : public std::exception
{
public:
    NetworkException(std::string msg):_msg(msg) {};
    ~NetworkException() throw() {};
    const char *what() { return _msg.c_str(); };
    
private:
    std::string _msg;
};

#ifdef USE_OPENCV

template <typename T>
void writemat(cv::Mat &to_write, const char *filename, int rows=INT_MAX, 
                                                        int cols=INT_MAX) {
   std::ofstream outfile(filename);
    if (!outfile.is_open())
        throw util::IOException("Could not open matrix write file");
    
    cv::MatConstIterator_<T> mat_it = to_write.begin<T>();
    for (int y=0; (y<to_write.rows); y++) {
        bool first = true;
        for (int x=0; (x<to_write.cols); x++) {
            if (x < cols) {
                if (!first) {
                    outfile << ",";
                }
                else
                    first = false;
                outfile << *mat_it;
            }
            mat_it++;
        }
        outfile << "\n";
        if (y >= rows)
            break;
    }
    outfile.close();
}

#endif

template <typename T>
int GetMaxIndex(std::vector<T> list)
{
	T max_val = list[0];
	int max_index = 0;
	for (int i=1; i<(int) list.size(); i++)
	{
		if (list[i] > max_val)
		{
			max_val = list[i];
			max_index = i;
		}
	}
	return max_index;
}


} // namespace util

#endif	/* MISCUTILS_H */

