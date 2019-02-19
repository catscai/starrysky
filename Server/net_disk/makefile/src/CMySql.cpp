/*************************************************************************
	> File Name: CMySql.cpp
	> Author: 
	> Mail: 
	> Created Time: jeu. 20 déc. 2018 20:28:17 CST
 ************************************************************************/

#include<iostream>
#include<string>
using namespace std;
#include "CMySql.h"

CMySql::CMySql()
{
    connection = mysql_init(NULL);
    if(connection == NULL)
    {
        cout<<"mysql_init Error"<<mysql_error(connection);
        exit(1);
    }
    mysql_set_character_set(connection,"gb2312"); 
}

CMySql::~CMySql()
{
    if(connection != NULL)
    {
        mysql_close(connection);
        connection = NULL;
    }
}

bool    CMySql::initDB(string host,string user,string pwd,string db_name)
{
    connection = mysql_real_connect(connection,host.c_str(),
            user.c_str(),pwd.c_str(),db_name.c_str(),0,NULL,0);
    if(connection == NULL)
    {
        cout<<"mysql_real_connect Error:"<<mysql_error(connection);
        exit(1);
    }
    if(mysql_query(connection,"set global max_allowed_packet=67108864"))
    {
        exit(1);
    }
    return true;
}

bool    CMySql::QuerySql(const string& sql,int nColumns,vector<string>& res)
{
    res.clear();
    if(mysql_query(connection,sql.c_str()))
    {
        cout<<"Query error"<<mysql_error(connection);
        exit(1);
    }else{
        //取得结果集
        result = mysql_store_result(connection);
        if(result == NULL)
            return false;
        while(row = mysql_fetch_row(result))
        {
            for(int i = 0; i < nColumns;i++)
            {
                res.push_back(row[i]);
            }
        }
    }
    return true;
}

bool    CMySql::UpdateSql(const string& sql)
{
    if(mysql_query(connection,sql.c_str()))
        return false;
    return true;
}
