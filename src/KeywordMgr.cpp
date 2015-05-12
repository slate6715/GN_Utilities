/* 
 * File:   KeywordMgr.cpp
 * Author: slate
 * 
 * Created on March 22, 2013, 9:52 AM
 */

#ifndef _WIN32

#include <regex.h>
#include <fstream>
#include <sstream>
#include "KeywordMgr.h"
#include "miscutils.h"

namespace util {
    
KeywordMgr::KeywordMgr(regex_type keyword_type) {
    _rtype = keyword_type;
}

KeywordMgr::KeywordMgr(const KeywordMgr& orig) {
}

KeywordMgr::~KeywordMgr() {
}


void KeywordMgr::loadKeywords(const char *filename) {
    std::ifstream keyword_file;
    keyword_file.open(filename);
    if (!keyword_file.is_open()) {
        std::stringstream msg;
        msg << "Unable to open the keyword file " << filename << 
                    " for reading.\n";
        throw util::IOException(msg.str().c_str());          
    }
        
        
    std::string buf;
    while (keyword_file.good()) {
            
        getline(keyword_file, buf);
        if (buf.length() == 0)
            continue;
            
        // Keywords should be in the format of <keyword>,<weight>
        std::vector<std::string> kw_line;
        util::split(buf, ',', kw_line);
        if (kw_line.size() < 2) {
            std::stringstream msg;
            msg << "Invalid format in keyword file for entry: " << buf << ".  Should be: keyword, weight\n";
            throw util::IOException(msg.str().c_str());
            return;
        }
    
        _kw_list.emplace_back();
        int last_pos = _kw_list.size()-1;
        _kw_list[last_pos].keyword = kw_line[0];
        _kw_list[last_pos].weight = atof(kw_line[1].c_str());
    }
    
    if (_rtype == regex_Extended) {
        compileRegex();
    }
    
    keyword_file.close();
    
}


void KeywordMgr::convertType(regex_type new_type) {
    
    if (_rtype == new_type) {
        return;
    }

    for (unsigned int i=0; i<_kw_list.size(); i++) {
        if ((_rtype == regex_MySQL) && (new_type == regex_Extended)) {
            util::replace("[[:<:]]", "\\b", _kw_list[i].keyword);
            util::replace("[[:>:]]", "\\b", _kw_list[i].keyword);        
        }
    }
    
    if (new_type == regex_Extended) {
        compileRegex();
    }
    
}


KeywordMgr::keyword &KeywordMgr::operator [] (unsigned int idx) {
    return _kw_list[idx];
}

void KeywordMgr::compileRegex() {
    char errbuf[1000];
    errbuf[0] = 0;
    int results;
    
    for (unsigned int i=0; i<_kw_list.size(); i++) {
        if ((results = regcomp(&_kw_list[i].regex, _kw_list[i].keyword.c_str(), 
                                                REG_ICASE | REG_EXTENDED)) != 0) {
            regerror(results, &_kw_list[i].regex, errbuf, 1000);
            std::stringstream errmsg;
            errmsg << "Regcomp error: " << errbuf << "\n";
            throw new util::IOException(errmsg.str());
        }
    }

}


} // namespace util

#endif  // ndef _WIN32
