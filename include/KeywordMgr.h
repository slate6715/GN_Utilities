/* 
 * File:   KeywordMgr.h
 * Author: slate
 *
 * Created on March 22, 2013, 9:52 AM
 */

#ifndef KEYWORDMGR_H
#define	KEYWORDMGR_H

#ifndef _WIN32

#include <string>
#include <vector>
#include <regex.h>

namespace util {
    
class KeywordMgr {
public:
    
    struct keyword {
        std::string keyword;
        regex_t regex;
        float weight;
    };
    
    enum regex_type {
        String, regex_MySQL, regex_Extended
    };
    
    KeywordMgr(regex_type kw_type);
    KeywordMgr(const KeywordMgr& orig);
    
    virtual ~KeywordMgr();
  
    void loadKeywords(const char *filename);
    inline unsigned int num_keywords() const { return _kw_list.size(); };
    keyword &operator [] (unsigned int idx);
    void convertType(regex_type new_type);
    
private:
    void compileRegex();
    
    std::vector<keyword> _kw_list;
    regex_type _rtype;
};

} // namespace util

#endif // ndef _WIN32

#endif	/* KEYWORDMGR_H */

