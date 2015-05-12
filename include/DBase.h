/* 
 * File:   DBase.h
 * Author: root
 *
 * Created on September 26, 2012, 11:29 AM
 */

#ifndef DBASE_H
#define	DBASE_H

#include <string>
#include <memory>
#include <cassert>

class Query;
class StoreQueryResult;
class UseQueryResult;

namespace util {
    
class DBException : public std::exception
{
public:
    DBException(const char *msg);
    DBException(std::string &msg);
    virtual ~DBException() throw() {};
    
    const char *what() { return _msg.c_str(); };
    
private:
    std::string _msg;
        
};


class UseQueryResults {
public:
    virtual ~UseQueryResults();
    
    virtual bool fetch_row() { return false; };

    virtual void clear();
    
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
    virtual void getBlob(int idx, void *buf, unsigned int bufsize) {};
    virtual void getBlob(const char *idx, void *buf, unsigned int bufsize) {};

    virtual unsigned int getDataLength(int idx);
    virtual unsigned int getDataLength(const char *idx);
    
    virtual bool hasField(const char *fieldname) { return false; };
    
protected:
    UseQueryResults(); 
    UseQueryResults(const UseQueryResults &orig);
    
};

class StoreQueryResults {
public:
    virtual ~StoreQueryResults();
    virtual unsigned int num_rows() { return false; };    
    
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
    virtual void getBlob(int row, int idx, void *buf, unsigned int bufsize) {};
    virtual void getBlob(int row, const char *idx, void *buf, unsigned int bufsize) {};
    
    virtual unsigned int getDataLength(int row, int idx);
    virtual unsigned int getDataLength(int row, const char *idx);

    virtual bool hasField(const char *fieldname) { return false; };
    
protected:
    StoreQueryResults();
    StoreQueryResults(const StoreQueryResults &orig);
};

class Query {
public:
    Query(const Query &orig);
    virtual ~Query();
    
    virtual Query& operator << (const std::string &val);
    virtual Query& operator << (const char *val);
    virtual Query& operator << (int val);
    virtual Query& operator << (long val);
    virtual Query& operator << (unsigned long val);
    virtual Query& operator << (unsigned int val);
    virtual Query& operator << (double val);
    virtual Query& operator << (float val);
    
    virtual std::unique_ptr<StoreQueryResults> execStoreQuery(const char *stmt=NULL);
    virtual std::unique_ptr<UseQueryResults> execUseQuery(const char *stmt=NULL);
    virtual bool execUpdate(const char *stmt=NULL);
    
    virtual void reset();
    
protected:    
    Query();
    
private:  
};

class DBase {
public:
    virtual ~DBase();
    
    enum conntype {tcp, namedpipe};
    
    virtual void connectPersist(const char *server, conntype ctype, const char *user,
                const char *password, unsigned int port, const char *db);
    virtual void connect(const char *server, conntype ctype, const char *user,
                const char *password, unsigned int port, const char *db) {};
    virtual bool isConnected();
    virtual void reconnect() {};

    virtual std::unique_ptr<Query> getQueryObj() { assert(0); return std::unique_ptr<Query>(); };
    virtual void disconnect() { assert(0); };
                    
protected:
    DBase();
    DBase(const DBase& orig);
    virtual void verifyConnection() {};

    conntype _ctype;
    std::string _host;
    int _port;
    std::string _username;
    std::string _password;
    std::string _dbname;
    
private:

};

} // namespace 

#endif	/* DBASE_H */

