/* 
 * File:   MySQLDBase.h
 * Author: root
 *
 * Created on September 26, 2012, 11:33 AM
 */

#ifndef MYSQLDBASE_H
#define	MYSQLDBASE_H

#ifdef USE_MYSQL

#include "DBase.h"
#include "mysql++.h"
#include <memory>

namespace util {
    
class MySQLDBase : public DBase {
public:
    MySQLDBase();
    MySQLDBase(const MySQLDBase& orig);
    virtual ~MySQLDBase();
    
    virtual void connect(const char *server, DBase::conntype ctype, const char *user=NULL,
        const char *password=NULL, unsigned int port=3306, const char *db=NULL);
    virtual void reconnect();
    
    virtual bool isConnected();
    
    virtual std::unique_ptr<Query> getQueryObj();
    virtual void disconnect();
    
    std::unique_ptr<mysqlpp::Query> getMySQLQueryObj();
    
protected:
    virtual void verifyConnection();
    
private:
    mysqlpp::Connection _conn;
};

class MySQLQuery : public Query {
public:
    MySQLQuery(mysqlpp::Connection &conn);
    MySQLQuery(const MySQLQuery &orig) { assert(0); };  // no copy yet
    virtual ~MySQLQuery();
    
    virtual MySQLQuery& operator << (const std::string &val);
    virtual MySQLQuery& operator << (const char *val);
    virtual MySQLQuery& operator << (int val);
    virtual MySQLQuery& operator << (unsigned int val);
    virtual MySQLQuery& operator << (long val);
    virtual MySQLQuery& operator << (unsigned long val);
    virtual MySQLQuery& operator << (double val);
    virtual MySQLQuery& operator << (float val);
    
    virtual std::unique_ptr<StoreQueryResults> execStoreQuery(const char *stmt=NULL);
    virtual std::unique_ptr<UseQueryResults> execUseQuery(const char *stmt=NULL);
    virtual bool execUpdate(const char *stmt=NULL);
    
    virtual void reset();
    
private:
    MySQLQuery() { assert(0); };
    
    mysqlpp::Query *_qobj;
};

class MySQLUseQueryResults : public UseQueryResults {
public:
    MySQLUseQueryResults(mysqlpp::UseQueryResult &uqr);
    MySQLUseQueryResults(const MySQLUseQueryResults &orig);
    virtual ~MySQLUseQueryResults();
    
    virtual bool fetch_row();
    virtual void clear() { while (fetch_row()); } ;
    
    virtual std::string &getString(int idx, std::string &dst);
    virtual std::string &getString(const char *idx, std::string &dst);
    virtual int getInt(int idx);
    virtual int getInt(const char *idx);
    virtual unsigned int getUInt(int idx);
    virtual unsigned int getUInt(const char *idx);
    virtual long getLong(int idx);
    virtual long getLong(const char *idx);
    virtual unsigned long getULong(int idx);
    virtual unsigned long getULong(const char *idx);
    virtual bool getBool(int idx);
    virtual bool getBool(const char *idx);
    virtual float getFloat(int idx);
    virtual float getFloat(const char *idx);
    virtual void getBlob(int idx, void *buf, unsigned int bufsize);
    virtual void getBlob(const char *idx, void *buf, unsigned int bufsize);

    virtual bool hasField(const char *fieldname);
    
    virtual unsigned int getDataLength(int idx);
    virtual unsigned int getDataLength(const char *idx);
    
private:
    mysqlpp::UseQueryResult _uqr;
    mysqlpp::Row _cur_row;
};

class MySQLStoreQueryResults : public StoreQueryResults {
public:
    virtual ~MySQLStoreQueryResults();
    
    MySQLStoreQueryResults(mysqlpp::StoreQueryResult &sqr);
    MySQLStoreQueryResults(const MySQLStoreQueryResults &orig);
    virtual unsigned int num_rows() { return _sqr.num_rows(); };
    
    virtual std::string &getString(int row, int idx, std::string &dst);
    virtual std::string &getString(int row, const char *idx, std::string &dst);
    virtual int getInt(int row, int idx);
    virtual int getInt(int row, const char *idx);
    virtual unsigned int getUInt(int row, int idx);
    virtual unsigned int getUInt(int row, const char *idx);
    virtual long getLong(int row, int idx);
    virtual long getLong(int row, const char *idx);
    virtual unsigned long getULong(int row, int idx);
    virtual unsigned long getULong(int row, const char *idx);
    virtual bool getBool(int row, int idx);
    virtual bool getBool(int row, const char *idx);
    virtual float getFloat(int row, int idx);
    virtual float getFloat(int row, const char *idx);
    virtual void getBlob(int row, int idx, void *buf, unsigned int bufsize);
    virtual void getBlob(int row, const char *idx, void *buf, unsigned int bufsize);
    virtual unsigned int getDataLength(int row, int idx);
    virtual unsigned int getDataLength(int row, const char *idx);
    
    virtual bool hasField(const char *fieldname);
    
private:
    mysqlpp::StoreQueryResult _sqr;
};

} // namespace util

#endif // USE_MYSQL

#endif	/* MYSQLDBASE_H */

