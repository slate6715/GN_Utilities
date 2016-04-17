/* 
 * File:   MySQLDBase.cpp
 * Author: root
 * 
 * Created on September 26, 2012, 11:33 AM
 */

#include "stdafx.h"

#ifdef USE_MYSQL

#include "MySQLDBase.h"
#include <memory>

namespace util {
    
MySQLDBase::MySQLDBase() {
}

MySQLDBase::MySQLDBase(const MySQLDBase& orig) {
    
}

MySQLDBase::~MySQLDBase() {
}

////////////////////////////////////////////////////////////////////
// connect - connect to the database using a tcp connection
//
// Params:  server - the hostname of the server to connect to
//          user - username if required
//          password - password if required
//          port - the port for the server, if not the default 3306
//          db - the database name to set this connection to
////////////////////////////////////////////////////////////////////

void MySQLDBase::connect(const char *server, DBase::conntype ctype, const char *user,
        const char *password, unsigned int port, const char *db)
{    

    std::stringstream url;
    _host = server;
    _ctype = ctype;
    _username = user;
    _password = password;
    _port = port;
    _dbname = db;

    if (isConnected())
        throw DBException("Connect called on connection already established.");

    if (ctype == namedpipe)
    {
        url << "\\pipe\\mysql.sock";
    }
    else
    {
        if ((port == 0) & (_port == -1))
            throw DBException("TCP connection requested but not port provided");
        else if (_port != -1)
            port = _port;
        
        url << server << ":" << port;
    }
    
    try {
        if (!_conn.connect(db, url.str().c_str(), user, password, port)) {
            throw DBException("(MySQLDBase::connect) Connection to database failed");
        }
    } catch (mysqlpp::ConnectionFailed &ex) {
        std::string msg("(MySQLDBase::connect) Connection to database failed: ");
        msg += ex.what();
        throw DBException(msg);
    }
    
}

////////////////////////////////////////////////////////////////////
// reconnect - use the saved parameters from a previous connection to
//             reconnect
////////////////////////////////////////////////////////////////////

void MySQLDBase::reconnect() {
    connect(_host.c_str(), _ctype, _username.c_str(), _password.c_str(), _port, _dbname.c_str());
}

////////////////////////////////////////////////////////////////////
// isConnected - returns true if a valid connection is established, false
//               otherwise
////////////////////////////////////////////////////////////////////

bool MySQLDBase::isConnected() {
    return _conn.connected();
}

////////////////////////////////////////////////////////////////////
// verifyConnection - makes sure the connection is established before
//          using it.  If persistent connection is set up, it will 
//          re-establish connections as necessary.  If not, it
//          raises a DBException
//                  
////////////////////////////////////////////////////////////////////

void MySQLDBase::verifyConnection()
{
    if (!isConnected())
    {
        if (_port == -1)
            throw DBException("(MySQLDBase::verifyConnection) Connection not "
                    "active and no persist data set.");
        
        connect(_host.c_str(), _ctype, _username.c_str(), _password.c_str(), 
                                        _port, _dbname.c_str());
    }
}

void MySQLDBase::disconnect()
{ 
    std::cout << "Disconnecting\n";
    _conn.disconnect();
    std::cout << "Disconnected\n";
}

std::unique_ptr<mysqlpp::Query> MySQLDBase::getMySQLQueryObj() { 
    return std::unique_ptr<mysqlpp::Query>(new mysqlpp::Query(&_conn));
}

std::unique_ptr<Query> MySQLDBase::getQueryObj()
{
    std::unique_ptr<Query> results(new MySQLQuery(_conn));
    return results;
}

MySQLQuery::MySQLQuery(mysqlpp::Connection &conn) {
    _qobj = new mysqlpp::Query(&conn);
}

MySQLQuery::~MySQLQuery() {
    delete _qobj;
}

MySQLQuery& MySQLQuery::operator << (const std::string &val) {
    *_qobj << val;
    return *this;
}

MySQLQuery& MySQLQuery::operator << (const char *val) {
    *_qobj << val;
    return *this;
}

MySQLQuery& MySQLQuery::operator << (int val) {
    *_qobj << val;
    return *this;
}

MySQLQuery& MySQLQuery::operator << (long val) {
    *_qobj << val;
    return *this;
}

MySQLQuery& MySQLQuery::operator << (unsigned int val) {
    *_qobj << val;
    return *this;
}

MySQLQuery& MySQLQuery::operator << (double val) {
    *_qobj << val;
    return *this;
}

MySQLQuery& MySQLQuery::operator << (float val) {
    *_qobj << val;
    return *this;
}


void MySQLQuery::reset() {
    _qobj->reset();
}

std::unique_ptr<StoreQueryResults> MySQLQuery::execStoreQuery(const char *stmt) {
    if (stmt != NULL)
        (*this) << stmt;
    
    mysqlpp::StoreQueryResult qrs = _qobj->store();
    MySQLStoreQueryResults *new_sqr = new MySQLStoreQueryResults(qrs);
    std::unique_ptr<StoreQueryResults> results(new_sqr);
    return results;
}

std::unique_ptr<UseQueryResults> MySQLQuery::execUseQuery(const char *stmt) {
    if (stmt != NULL)
        (*this) << stmt;
    
    mysqlpp::UseQueryResult qrs = _qobj->use();
    MySQLUseQueryResults *new_uqr = new MySQLUseQueryResults(qrs);
    std::unique_ptr<UseQueryResults> results(new_uqr);
    return results;    
}

bool MySQLQuery::execUpdate(const char *stmt){
    if (stmt != NULL)
        (*this) << stmt;

    return _qobj->exec();
}

MySQLUseQueryResults::~MySQLUseQueryResults() {
    
}

bool MySQLUseQueryResults::fetch_row() {
   _cur_row = _uqr.fetch_row();
   if (_cur_row == NULL)
       return false;
   return true;
}

std::string &MySQLUseQueryResults::getString(int idx, std::string &dst) {
    dst = _cur_row[idx].c_str();
    return dst;
}

std::string &MySQLUseQueryResults::getString(const char *idx, std::string &dst) {
    dst = _cur_row[idx].c_str();
    return dst; 

}

int MySQLUseQueryResults::getInt(int idx) {
    return (int) _cur_row[idx];
}
int MySQLUseQueryResults::getInt(const char *idx) {
    return (int) _cur_row[idx];
}

unsigned int MySQLUseQueryResults::getUInt(int idx) {
    return (unsigned int) _cur_row[idx];
}
unsigned int MySQLUseQueryResults::getUInt(const char *idx) {    
    return (unsigned int) _cur_row[idx];
}


long MySQLUseQueryResults::getLong(int idx) {
    return (long) _cur_row[idx];
}

long MySQLUseQueryResults::getLong(const char *idx) {
    return (long) _cur_row[idx];
}

unsigned long MySQLUseQueryResults::getULong(int idx) {
    return (long) _cur_row[idx];
}

unsigned long MySQLUseQueryResults::getULong(const char *idx) {
    return (long) _cur_row[idx];
}

bool MySQLUseQueryResults::getBool(int idx) {
    return (bool) _cur_row[idx];
}

bool MySQLUseQueryResults::getBool(const char *idx) {
    return (bool) _cur_row[idx];
}

float MySQLUseQueryResults::getFloat(int idx) {
    return (bool) _cur_row[idx];
}

float MySQLUseQueryResults::getFloat(const char *idx) {
    return (bool) _cur_row[idx];
}

unsigned int MySQLUseQueryResults::getDataLength(int idx) {
    return _cur_row[idx].length();
}

unsigned int MySQLUseQueryResults::getDataLength(const char *idx) {
    return _cur_row[idx].length();
}


void MySQLUseQueryResults::getBlob(int idx, void *buf, unsigned int bufsize) {
    unsigned int length = _cur_row[idx].length();
    assert(bufsize >= length);
    
    memcpy(buf, (void*) _cur_row[idx].c_str(), length);
}

void MySQLUseQueryResults::getBlob(const char *idx, void *buf, 
                                                        unsigned int bufsize) {
    unsigned int length = _cur_row[idx].length();
    assert(bufsize >= length);
    
    memcpy(buf, (void*) _cur_row[idx].c_str(), length);
}

bool MySQLUseQueryResults::hasField(const char *fieldname) {
    if (_cur_row.field_num(fieldname) < _cur_row.size()) {
        return true;
    }
    return false;
}


/*virtual std::unique_ptr<StoreQueryResult> MySQLQuery::execStoreQuery() {
    
}

virtual std::unique_ptr<UseQueryResult> MySQLQuery::execUseQuery() {
    
}*/

    
MySQLUseQueryResults::MySQLUseQueryResults(mysqlpp::UseQueryResult &uqr):
                                        _uqr(uqr) 
{
    
}

MySQLUseQueryResults::MySQLUseQueryResults(const MySQLUseQueryResults &orig):
                                                                _uqr(orig._uqr)
{
}

MySQLStoreQueryResults::~MySQLStoreQueryResults() {

}
    
MySQLStoreQueryResults::MySQLStoreQueryResults(mysqlpp::StoreQueryResult &sqr):
                                                                _sqr(sqr)
{
}

MySQLStoreQueryResults::MySQLStoreQueryResults(const MySQLStoreQueryResults &orig):
                                                                _sqr(orig._sqr)
{
    _sqr = orig._sqr;
}

std::string &MySQLStoreQueryResults::getString(int row, int idx, std::string &dst) {
    dst = (std::string) _sqr[row][idx];
    return dst;
}

std::string &MySQLStoreQueryResults::getString(int row, const char *idx, std::string &dst) {
    dst = (std::string) _sqr[row][idx];
    return dst;
}

int MySQLStoreQueryResults::getInt(int row, int idx) {
    return (int) _sqr[row][idx];
}

int MySQLStoreQueryResults::getInt(int row, const char *idx) {
    return (int) _sqr[row][idx];
}

unsigned int MySQLStoreQueryResults::getUInt(int row, int idx) {
    return (unsigned int) _sqr[row][idx];
}
unsigned int MySQLStoreQueryResults::getUInt(int row, const char *idx) {    
    return (unsigned int) _sqr[row][idx];
}

long MySQLStoreQueryResults::getLong(int row, int idx) {
    return (long) _sqr[row][idx];
}

long MySQLStoreQueryResults::getLong(int row, const char *idx) {
    return (long) _sqr[row][idx];    
}

unsigned long MySQLStoreQueryResults::getULong(int row, int idx) {
    return (long) _sqr[row][idx];
}

unsigned long MySQLStoreQueryResults::getULong(int row, const char *idx) {
    return (long) _sqr[row][idx];    
}

bool MySQLStoreQueryResults::getBool(int row, int idx) {
    return (bool) _sqr[row][idx];
}

bool MySQLStoreQueryResults::getBool(int row, const char *idx) {
    return (bool) _sqr[row][idx];    
}

float MySQLStoreQueryResults::getFloat(int row, int idx) {
    if (_sqr[row][idx].is_null())
        return 0.0f;
    return (float) _sqr[row][idx];
}

float MySQLStoreQueryResults::getFloat(int row, const char *idx) {
    return (float) _sqr[row][idx];    
}

unsigned int MySQLStoreQueryResults::getDataLength(int row, int idx) {
    return _sqr[row][idx].length();
}

unsigned int MySQLStoreQueryResults::getDataLength(int row, const char *idx) {
    return _sqr[row][idx].length();
}

void MySQLStoreQueryResults::getBlob(int row, int idx, void *buf, 
                                                        unsigned int bufsize) {
    unsigned int length = _sqr[row][idx].length();
    assert(length <= bufsize);
    
    memcpy(buf, (void*) _sqr[row][idx].c_str(), length);
}

void MySQLStoreQueryResults::getBlob(int row, const char *idx, void *buf, 
                                                        unsigned int bufsize) {
    unsigned int length = _sqr[row][idx].length();
    assert(bufsize <= length);
    
    memcpy(buf, (void*) _sqr[row][idx].c_str(), length);
}

bool MySQLStoreQueryResults::hasField(const char *fieldname) {
    if (_sqr[0].field_num(fieldname) < _sqr[0].size()) {
        return true;
    }
    return false;
}

} // namespace util

#endif // USE_MYSQL