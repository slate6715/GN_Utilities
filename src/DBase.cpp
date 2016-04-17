/* 
 * File:   DBase.cpp
 * Author: root
 * 
 * Created on September 26, 2012, 11:29 AM
 */

#include "stdafx.h"
#include "DBase.h"

namespace util {
    
DBase::DBase() {
    _port = -1;
}

DBase::DBase(const DBase& orig) {
    _ctype = orig._ctype;
    _host = orig._host;
    _port = orig._port;
    _username = orig._username;
    _password = orig._password;
    _dbname = orig._dbname;
    
}

DBase::~DBase() {
}

DBException::DBException(const char *msg):_msg(msg)
{
}

DBException::DBException(std::string &msg):_msg(msg)
{
}
////////////////////////////////////////////////////////////////////
// connectPersist - establishes a persistent connection that will
//           reconnect whenever it is needed using the information that
//           is passed in.
//                  
////////////////////////////////////////////////////////////////////

void DBase::connectPersist(const char *server, conntype ctype, const char *user,
                const char *password, unsigned int port, const char *db)
{
    _host = server;
    _username = user;
    _password = password;
    _port = port;
    _dbname = db;
    
    connect(_host.c_str(), ctype, _username.c_str(), _password.c_str(), 
                                                _port, _dbname.c_str());
}

bool DBase::isConnected() { 
    assert(0);
	return false;
}

UseQueryResults::UseQueryResults() {
    
}

UseQueryResults::~UseQueryResults() {

}

void UseQueryResults::clear() {
    assert(0);
}

StoreQueryResults::StoreQueryResults() {
    
}

StoreQueryResults::~StoreQueryResults() {
    
}

std::string &UseQueryResults::getString(int idx, std::string &dst) {
    assert(0);
    return dst;
}

std::string &UseQueryResults::getString(const char *idx, std::string &dst) {
    assert(0);
    return dst;
}

int UseQueryResults::getInt(int idx) {
    assert(0);
    return -1;
}

int UseQueryResults::getInt(const char *idx) {
    assert(0);
    return -1;
}

unsigned int UseQueryResults::getUInt(int idx) {
    assert(0);
    return 0;
}

unsigned int UseQueryResults::getUInt(const char *idx) {
    assert(0);
    return 0;
}

long UseQueryResults::getLong(int idx) {
    assert(0);
    return -1;
}

long UseQueryResults::getLong(const char *idx) {
    assert(0);
    return -1;
}

unsigned long UseQueryResults::getULong(int idx) {
    assert(0);
    return 0;
}

unsigned long UseQueryResults::getULong(const char *idx) {
    assert(0);
    return 0;
}

bool UseQueryResults::getBool(const char *idx) {
    assert(0);
    return false;
}

bool UseQueryResults::getBool(int idx) {
    assert(0);
    return false;
}

float UseQueryResults::getFloat(const char *idx) {
    assert(0);
    return 0;
}

float UseQueryResults::getFloat(int idx) {
    assert(0);
    return 0;
}

unsigned int UseQueryResults::getDataLength(int idx) {
    assert(0);
    return 0;
}

unsigned int UseQueryResults::getDataLength(const char *idx) {
    assert(0);
    return 0;
}

std::string &StoreQueryResults::getString(int row, int idx, std::string &dst) {
    assert(0);
    return dst;
}

std::string &StoreQueryResults::getString(int row, const char *idx, std::string &dst) {
    assert(0);
    return dst;    
}

int StoreQueryResults::getInt(int row, int idx) {
    assert(0);
    return -1;    
}

int StoreQueryResults::getInt(int row, const char *idx) {
    assert(0);
    return -1;        
}
unsigned int StoreQueryResults::getUInt(int row, int idx) {
    assert(0);
    return -1;    
}

unsigned int StoreQueryResults::getUInt(int row, const char *idx) {
    assert(0);
    return -1;        
}

long StoreQueryResults::getLong(int row, int idx) {
    assert(0);
    return -1;    
}

long StoreQueryResults::getLong(int row, const char *idx) {
    assert(0);
    return -1;        
}

unsigned long StoreQueryResults::getULong(int row, int idx) {
    assert(0);
    return -1;    
}

unsigned long StoreQueryResults::getULong(int row, const char *idx) {
    assert(0);
    return -1;        
}

bool StoreQueryResults::getBool(int row, int idx) {
    assert(0);
    return false;    
}

bool StoreQueryResults::getBool(int row, const char *idx) {
    assert(0);
    return false;        
}
float StoreQueryResults::getFloat(int row, int idx) {
    assert(0);
    return 0;    
}

float StoreQueryResults::getFloat(int row, const char *idx) {
    assert(0);
    return -1;        
}

unsigned int StoreQueryResults::getDataLength(int row, int idx) {
    assert(0);
    return -1;
}

unsigned int StoreQueryResults::getDataLength(int row, const char *idx) {
    assert(0);
    return -1;
}

Query::Query(const Query &orig)
{ 
    assert(0); 
}
    
Query::~Query() {
    
}
    
Query &Query::operator << (const std::string &val) {
    assert(0);
    return *this;    
}

Query &Query::operator << (const char *val) {
    assert(0);
    return *this;    
}
    
Query &Query::operator << (int val) {
    assert(0);
    return *this;        
}

Query &Query::operator << (long val) {
    assert(0);
    return *this;        
}

Query &Query::operator << (unsigned long val) {
    assert(0);
    return *this;        
}

Query &Query::operator << (unsigned int val) {
    assert(0);
    return *this;        
}

Query &Query::operator << (double val) {
    assert(0);
    return *this;        
}

Query &Query::operator << (float val) {
    assert(0);
    return *this;        
}
    
std::unique_ptr<StoreQueryResults> Query::execStoreQuery(const char *stmt) {
    assert(0);    
	return std::unique_ptr<StoreQueryResults>();
}

std::unique_ptr<UseQueryResults> Query::execUseQuery(const char *stmt) {
    assert(0);     
	return std::unique_ptr<UseQueryResults>();
}

bool Query::execUpdate(const char *stmt) {
    assert(0);
    return false;
}
    
void Query::reset() {
    assert(0);
}
    
Query::Query() {
    
}
    

} // namespace util

