/* 
 * File:   ConfigFile.h
 * Author: slate
 *
 * Created on October 1, 2012, 2:03 PM
 */

#ifndef CONFIGFILE_H
#define	CONFIGFILE_H

#include <string>
#include <map>

namespace util {
    
class config_data {
public:
    config_data() {};
    ~config_data() {};
    config_data(const config_data &orig);
    
    int num_val;
    float float_val;
    std::string str_val;
    bool bool_val;
};


class ConfigFile {
public:
    ConfigFile();
    ConfigFile(const ConfigFile& orig);
    virtual ~ConfigFile();
    
    bool readFile(const char *filename);
    bool getConfig(const char *name, config_data &dst);
    int getConfigInt(const char *name);
    float getConfigFloat(const char *name);
    bool getConfigBool(const char *name);
    std::string &getConfigStr(const char *name, std::string &dst);
    
    int overrideConfigs(std::string overrides);

    void replaceVars();
    
    std::string error;

    // How many times the string will recursively insert variables.  Prevents infinite loops
    static unsigned int Max_CF_Iters;
    
private:
    std::map<std::string, config_data> settings;
    void skipWhiteSpace(std::string &line, std::string::const_iterator &ptr);
    
};

} // namespace util
#endif	/* CONFIGFILE_H */

