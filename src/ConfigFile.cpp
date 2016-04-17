/* 
 * File:   ConfigFile.cpp
 * Author: root
 * 
 * Created on October 1, 2012, 2:13 PM
 */

#include "stdafx.h"
#include "ConfigFile.h"
#include "BufferedReader.h"
#include "miscutils.h"

namespace util {
    

config_data::config_data(const config_data &orig)
{
    num_val = orig.num_val;
    float_val = orig.float_val;
    str_val = orig.str_val;
    bool_val = orig.bool_val;
}

unsigned int ConfigFile::Max_CF_Iters = 30;

ConfigFile::ConfigFile() {
}

ConfigFile::ConfigFile(const ConfigFile& orig) {
}

ConfigFile::~ConfigFile() {    

}

//-----------------------------------------------------------------------------
// readFile - reads in the configuration file and loads the entries into a
//            std::map structure for quick lookup.  Stores the entries in three
//            forms when possible--string, integer, and boolean.  
//
//   Params:  filename - the pathname of the config file to open and read in 
//src/ConfigFile.cpp:121:56: error: ‘atoi’ was not declared in this scope
//   Returns: true for success, false if there was a problem and the details
//            can be found in the class attribute error
//-----------------------------------------------------------------------------
bool ConfigFile::readFile(const char *filename)
{
    FILE *fptr = NULL;
#ifndef _WIN32
    fptr = fopen(filename, "r");
#else
	fopen_s(&fptr, filename, "r");
#endif


    // if fopen failed    
    if (fptr == NULL)
    {
        std::string msg("Unable to open config file: ");
        msg.append(filename);
        throw util::IOException(msg.c_str());
    }
    
    // Use a buffered reader to read in each line    
    util::BufferedReader buf_rdr(fptr);
    std::string read_line;
    while (buf_rdr.readLine(read_line))
    {
        std::string::const_iterator b_ptr = read_line.begin();
        
        // Skip beginning white space
        skipWhiteSpace(read_line, b_ptr);
        
        // If it is a blank line or a comment
        if ((b_ptr == read_line.end()) || (*b_ptr == '#'))
            continue;
        
        std::string::const_iterator e_ptr = b_ptr;
        while ((e_ptr != read_line.end()) && ((isalpha(*e_ptr)) || (*e_ptr=='_') 
                || (*e_ptr == '-') || (*e_ptr == '.')))
            e_ptr++;
        
        std::string varname(b_ptr, e_ptr);
        b_ptr = e_ptr;
        
        // Skip white space
        skipWhiteSpace(read_line, b_ptr);
        
        // Check the next char for the command
        
        // If the next char is an assignment, get the value
        if (*b_ptr == '=') {
            b_ptr++;
            
            // Skip white space
            skipWhiteSpace(read_line, b_ptr);
        
            // Whoops, line ran out early
            if (b_ptr == read_line.end())
            {
                error = "Invalid format in line: ";
                error.append(read_line);
                return false;
            }
                
            // Get the char that will close out this line (or end of line)
            char expect_close = ' ';
            if ((*b_ptr == '\"') || (*b_ptr == '\''))
            {
                expect_close = *b_ptr;
                b_ptr++;
            }
            
            if (b_ptr == read_line.end())
            {
                error = "Invalid format in line: ";
                error.append(read_line);
                return false;
            }

            // Now get the rest of the line until we hit the expected closing
            // character, the end of the line, or the start of a comment
            e_ptr = b_ptr;
            while ((e_ptr != read_line.end()) && (*e_ptr != expect_close) && 
                    (*e_ptr != '#'))
                e_ptr++;
            
            std::string data(b_ptr, e_ptr);
            
            // Load up the data into a new entry
            std::pair<std::string, config_data> new_data;
            new_data.first = varname;
            new_data.second.str_val = data;
            new_data.second.num_val = atoi(data.c_str());
            new_data.second.float_val = (float) atof(data.c_str());
            util::lowercase(data);
            new_data.second.bool_val = (data.compare("true") == 0) ? true : false;
            
            // Insert into the map
            std::pair<std::map<std::string, config_data>::iterator, bool> result;
            result = settings.insert(new_data);
            
            // if the entry was already there, we have a duplicate
            if (result.second == false) {
                error = "Duplicate entry in config file: ";
                error.append(read_line);
                return false;
            }
            
        } else {
            error = "Invalid format in line: ";
            error.append(read_line);
            return false;
        }
        
    }
    
    fclose(fptr);
    
    
    return true;
    
}

//-----------------------------------------------------------------------------
// getConfig - Gets the config information associated with the parameter name
//
//   Params:  name - the attribute name to lookup--case sensitive
//            dst - where to put the attribute data
//
//   Returns: true if found, false otherwise
//-----------------------------------------------------------------------------

bool ConfigFile::getConfig(const char *name, config_data &dst) {    
    std::map<std::string, config_data>::iterator entry;
    std::string entry_name(name);
    
    entry = settings.find(entry_name);
    if (entry == settings.end())
        return false;
    
    dst = entry->second;
    return true;
}

int ConfigFile::getConfigInt(const char *name) {
    config_data dst;
    
    if (!getConfig(name, dst))
    {
        std::string msg("Unable to find the config entry: ");
        msg.append(name);
        throw util::IOException(msg);
    }
    
    return dst.num_val;
}

float ConfigFile::getConfigFloat(const char *name) {
    config_data dst;
    
    if (!getConfig(name, dst))
    {
        std::string msg("Unable to find the config entry: ");
        msg.append(name);
        throw util::IOException(msg);
    }
    
    return dst.float_val;
}

bool ConfigFile::getConfigBool(const char *name) {
    config_data dst;
    
    if (!getConfig(name, dst))
    {
        std::string msg("Unable to find the config entry: ");
        msg.append(name);
        throw util::IOException(msg);
    }
    
    return dst.bool_val;    
}

std::string &ConfigFile::getConfigStr(const char *name, std::string &dst) {
    config_data cdst;
    
    if (!getConfig(name, cdst))
    {
        std::string msg("Unable to find the config entry: ");
        msg.append(name);
        throw util::IOException(msg);
    }
    
    dst = cdst.str_val;
    return dst;    
}


void ConfigFile::skipWhiteSpace(std::string &line, std::string::const_iterator &ptr)
{
    // Skip white space
    while ((ptr != line.end()) && ((*ptr == ' ') || (*ptr == '\t')))
        ptr++;
}

int ConfigFile::overrideConfigs(std::string overrides) {
    util::trim(overrides, overrides);
    if (overrides.length() == 0)
        return 0;
    
    int count = 0;
    std::vector<std::string> splitstr;
    util::split(overrides, ',', splitstr);
    std::map<std::string, config_data>::iterator c_it;
    for (unsigned int i=0; i<splitstr.size(); i++) {
        std::string varname;
        std::string value;
        
        unsigned int pos = splitstr[i].find_first_of("=");
        
        if (pos == std::string::npos) {
            std::string msg("Invalid format in override config: ");
            msg.append(splitstr[i]);
            throw util::IOException(msg);
        }
        varname = splitstr[i].substr(0, pos);
        util::trim(varname, varname);
        value = splitstr[i].substr(pos+1);
        util::trim(value, value);
        
        util::lowercase(varname);
        if ((c_it = settings.find(varname)) == settings.end()) {
            std::string msg("Unable to find the override config entry: ");
            msg.append(splitstr[i]);
            throw util::IOException(msg);
        }
  
        util::trim(value, value);
        // Remove a leading or trailing quotes
        if ((value[0] == '\"') || (value[0] == '\''))
            value = value.substr(1, value.size()-1);
        if ((value[value.size()-1] == '\"') || (value[value.size()-1] == '\''))
            value = value.substr(0, value.size()-1);
        
        
        c_it->second.str_val = value;
        c_it->second.num_val = atoi(value.c_str());
        c_it->second.float_val = (float) atof(value.c_str());
        util::lowercase(value);
        c_it->second.bool_val = (value.compare("true") == 0) ? true : false;   
        count++;
    }
    return count;
}

void ConfigFile::replaceVars() {

    bool not_found = false;
    std::map<std::string, config_data>::iterator set_it = settings.begin();
    for ( ; set_it != settings.end() || not_found; set_it++) {        
        if (set_it->second.str_val.length() > 0) {
                                    
            for (unsigned int i=0; (i<Max_CF_Iters) && !not_found; i++) {
                // Locate any variable markers
                unsigned int var_pos;
                while ((var_pos = set_it->second.str_val.find("$(")) != std::string::npos)
                {
                    not_found = false;
                    if (var_pos != std::string::npos) {
                        unsigned int end_pos = set_it->second.str_val.find(")", var_pos);
                        if (end_pos == std::string::npos) {
                            std::stringstream msg;
                            msg << "Variable marker $( found with no terminating ')' in setting: " << set_it->first;       
                            throw util::IOException(msg.str());    
                        }
                
                        // Extract the variable name and look for its match
                        std::string varname = set_it->second.str_val.substr(var_pos+2, end_pos-var_pos-2);
                        std::map<std::string, config_data>::const_iterator var_it = settings.find(varname);
                        if (var_it == settings.end()) {
                            std::stringstream msg;
                            msg << "Variable " << varname << "not found in config file for entry: " << set_it->first;       
                            throw util::IOException(msg.str());                        
                        }
                        
                        if (var_it == set_it) {
                            std::stringstream msg;
                            msg << "Variable marker cannot point to itself, config file entry: " << set_it->first;       
                            throw util::IOException(msg.str());                                                
                        }
                    
                        std::string replacestr = "$(";
                        replacestr.append(varname);
                        replacestr.append(")");
                    
                        util::replace(replacestr.c_str(), var_it->second.str_val.c_str(), set_it->second.str_val);
                    }
                }
            }
        }
    }
}

} // namespace util