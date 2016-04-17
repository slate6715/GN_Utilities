#include "stdafx.h"
#include <cmath>
#include "miscutils.h"
#include <assert.h>
#include <cstdarg>
#include <cstdio>
#include <algorithm>
#include <string.h>

using namespace std;

const wchar_t *imgtypelist[] = {L".jpg", L".bmp", L".tiff", L".pgm", NULL};

namespace util {
    
bool isImageFile(wstring &filename)
{
        for (int i=0; imgtypelist[i] != NULL; i++)
	{
            if (filename.compare(imgtypelist[i]) == 0)
                return true;
	}
	return false;
}

static string w2st_buf;
static wstring st2w_buf;

const char *wideToString(wstring &s, string &to)
{
    to.assign(s.begin(), s.end());
    return to.c_str();
}

const char *wideToString(wstring &s)
{
    return wideToString(s, w2st_buf);
}

const char *wideToString(const wchar_t *s)
{
    wstring buf = s;
    return wideToString(buf);
}

const char *wideToString(const wchar_t *s, string &to)
{
    wstring buf = s;
    return wideToString(buf, to);
}

const wchar_t *charToWide(string &s, wstring &to)
{
    to.assign(s.begin(), s.end());
    return to.c_str();
}

const wchar_t *charToWide(const char *s, wstring &to)
{
    string buf = s;
    return charToWide(buf, to);
}

const wchar_t *charToWide(string &s)
{
    return charToWide(s, st2w_buf);
}

const wchar_t *charToWide(const char *s)
{
    string buf = s;
    return charToWide(buf, st2w_buf);
}

bool isAlpha(int thechar)
{
	if ((thechar < 0) || (thechar > 255))
		return false;

	return (isalpha(thechar) != 0);
}


bool isTermPunct(int thechar)
{
	if ((thechar == '.') || (thechar == '?') || (thechar == '!') || (thechar == ';'))
		return true;
	return false;
}

void lowercase(std::string &src, std::string *dst)
{
    if (dst == NULL)
        dst = &src;
    else
        *dst = src;
    std::transform(dst->begin(), dst->end(), dst->begin(), ::tolower);
 /*
    std::string::iterator s_it = src.begin();

    if (src.length() == 0)
        return false;
    
    // If the destination is not null, then we need to create a buffer and
    // load it up, then copy it all over to the destination
    if (dst != NULL)
    {
        dst->clear();
        
        char *newstr = new char[src.length()+1];
        newstr[src.length()] = 0;
        
        int pos = 0;
        
        
        for ( ; s_it != src.end(); s_it++)
        {
           newstr[pos++] = tolower(*s_it);
        }
        *dst = newstr;
        delete[] newstr;
    } else {
        
        // Loop through the string, converting it
        for ( ; s_it != src.end(); s_it++) {
            *s_it = tolower(*s_it);
        }
        
    }*/
}

void replace(const char *str1, const char *str2, std::string &src, std::string *dst) {
    std::string buf;
    if (dst == NULL)
        dst = &buf;
    
    unsigned int lastpos = 0;
    unsigned int curpos;
    while ((curpos = src.find(str1, lastpos)) != std::string::npos) {
        dst->append(src.substr(lastpos, curpos-lastpos));
        dst->append(str2);
        lastpos = curpos + strlen(str1);
    }
    
    if (lastpos < src.length())
        dst->append(src.substr(lastpos, src.length()));   
    if (buf.length() > 0)
        src = buf;
}

void split(const std::string &src, char divider, std::vector<std::string> &dst, char quotechar)
{
    dst.clear();
    
    unsigned int start = 0;
    int ignore_divider=false; // If we're inside a quote, divider's don't count
    
    for (unsigned int i=0; i<src.length(); i++) {
        if ((quotechar != 0) && (src[i] == quotechar))
            ignore_divider = !ignore_divider;
        
        if ((!ignore_divider) && (src[i] == divider)) {
            dst.push_back(std::string());
            dst[dst.size()-1] = src.substr(start, i-start);
            start = i+1;
        }
    }
    if (start != src.length()) {
        dst.push_back(std::string());
        dst[dst.size()-1] = src.substr(start, src.size()-start);
    }
        
}

std::string &trim(std::string &src, std::string &dst)
{
    unsigned int begin = 0;
    unsigned int end = src.size()-1;
    
    while ((begin < src.length()) && ((src[begin] == ' ') || (src[begin] == '\r') || 
            (src[begin] == '\n') || (src[begin] == '\t')))
        begin++;

    while ((end > 0) && ((src[end] == ' ') || (src[end] == '\r') || 
            (src[end] == '\n') || (src[end] == '\t')))
        end--;
    
    dst = src.substr(begin, end+1);
    return dst;
}

#ifndef _WIN32
int sprintf_s(char *buffer, const char *str, va_list args)
{
    return vsprintf(buffer, str, args);
}


int sprintf_s(char *buffer, const char *str, ...)
{
    va_list args;
    va_start(args, str);
    int result = sprintf_s(buffer, str, args);
    va_end(args);
    return result;
}


char *strtok_s(char *strToken, const char *strDelimit, char **context)
{
    return strtok_r(strToken, strDelimit, context);
}

char *strcpy_s(char *strDestination, size_t numberOfElements, const char *strSource)
{
    if (strlen(strSource) > numberOfElements)
        throw util::RuntimeException("strcpy_s Buffer overflow");
    
    return strcpy(strDestination, strSource);
}

char *strcpy_s(char *strDestination, const char *strSource, size_t count)
{
    return strncpy(strDestination, strSource, count);
}
#endif

#ifdef USE_OPENCV
float vecAngle(cv::Mat vec1, cv::Mat vec2) {
    return acos(vecCosine(vec1, vec2));
}

float vecCosine(cv::Mat vec1, cv::Mat vec2) {
    assert((vec1.rows == vec2.rows) && (vec1.cols == vec2.cols));
    assert(vec1.type() == vec2.type());
    assert(vec1.type() == CV_64F);
    
    cv::Mat m1, d1, d2;
    double d1_s, d2_s;
    
    m1 = (vec1 * vec2.t());
    assert((m1.rows == 1) && (m1.cols == 1));
    d1 = (vec1*vec1.t());
    assert((d1.rows == 1) && (d1.cols == 1));
    d1_s = sqrt(d1.at<double>(0,0));
    d2 = (vec2*vec2.t());
    assert((d2.rows == 1) && (d2.cols == 1));
    d2_s = sqrt(d2.at<double>(0,0));
    
    return (float) (m1.at<double>(0,0) / (double) (d1_s * d2_s));
}

float vecDist(cv::Mat vec) {
    assert((vec.rows == 1) || (vec.cols == 1));
    assert(vec.type() == CV_64F);
    cv::Mat results;
    
    if (vec.rows > 0)
        results = vec * vec.t();
    else
        results = vec.t() * vec;
    return (float) std::sqrt(results.at<double>(0));
}

double logVecGausPDF(cv::Mat vec, cv::Mat mean, cv::Mat stddev) {
    cv::Mat covar = cv::Mat::diag(stddev.t());
    cv::Mat result = -.5 * (vec - mean) * covar.inv() * (vec - mean).t();
    
    double det = cv::determinant(covar);
    
    double x1 = (1 / (std::pow(2 * M_PI, vec.cols/2)) * std::pow(det, .5));
        
    return result.at<double>(0,0) + std::log(x1);
}
#endif

double pdf(double x, double mean, double stddev) {
    double pi = 4.0*atan(1.0);
    return (1/(stddev * sqrt(2*pi))) * exp((-1/2)*pow((x-mean)/stddev, 2));
}

/*
int ExtractIntBetween(LPCTSTR left, CString &line, LPCTSTR right)
{
	int leftpos = line.Find(left);
	int rightpos = line.Find(right);

	if ((leftpos == -1) || (rightpos == -1))
		throw CodeException("(CBICADlg::OnNMDblclkDoclist) Bad string loaded in image list");

	return _wtoi(line.Mid(leftpos+1, rightpos-leftpos-1));
}

size_t urlwritefunct(void *buffer, size_t size, size_t nmemb, void *userdata)
{
	ImgDBase *images = (ImgDBase *) userdata;

	return images->WriteTar(buffer, size, nmemb);
}

#pragma warning (push)
#pragma warning (disable: 4100)

int urlprogressfunct (void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	ImgDBase *images = (ImgDBase *) clientp;

	if (images->ImagesRead() >= MAX_IMG_DOWNLOAD)
		return 1;

	if ((time(0) - images->start_download) > DOWNLOAD_TIMEOUT_SECS)
	{
		images->timed_out = true;
		return 1;
	}

	if (*(images->isDying))
		return 1;

	if ((images->ImagesRead()- images->last_displayed) >= 10)
	{
		images->msg_link->AddMessage(MSG_ROUTINE2, _T("*"));
		images->last_displayed = images->ImagesRead();
	}

	return 0;
}
*/

} // namespace util