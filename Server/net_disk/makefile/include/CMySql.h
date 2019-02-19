/*************************************************************************
	> File Name: CMySql.h
	> Author: 
	> Mail: 
	> Created Time: jeu. 20 déc. 2018 20:24:09 CST
 ************************************************************************/

#ifndef _CMYSQL_H
#define _CMYSQL_H
#include<iostream>
#include<string>
#include<mysql/mysql.h>
#include<vector>
using namespace std;
 
 class CMySql
 {
     public:
        CMySql();
        ~CMySql();
        bool initDB(string host, string user, string pwd, string db_name);
        bool QuerySql(const string& sql,int nColumns,vector<string>& res);
        bool UpdateSql(const string& sql);
     private:
        MYSQL *connection;
        MYSQL_RES *result;
        MYSQL_ROW row;
 };
  
#endif
